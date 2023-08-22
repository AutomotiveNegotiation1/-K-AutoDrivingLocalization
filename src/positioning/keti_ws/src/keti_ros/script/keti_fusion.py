#!/usr/bin/env python3
###############################################################################
#
# Copyright (C) 2023 - 2028 KETI, All rights reserved.
#                           (Korea Electronics Technology Institute)
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# Use of the Software is limited solely to applications:
# (a) running for Korean Government Project, or
# (b) that interact with KETI project/platform.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
# KETI BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
# WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
# OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
# Except as contained in this notice, the name of the KETI shall not be used
# in advertising or otherwise to promote the sale, use or other dealings in
# this Software without prior written authorization from KETI.
#
##############################################################################

import rospy, time, serial, os, math
import numpy as np
import threading

from sensor_msgs.msg import Imu
from localizer_dwm1001.msg import Anchor
from localizer_dwm1001.msg import Tag
from keti_imu_kalman import IMUKalmanFilter
from keti_uwb_kalman import UWBKalmanFilter

from scipy.spatial.transform import Rotation
from live_plotter import LivePlotter
import tkinter as tk

class Sensor_fusion:
    def __init__(self):
        # ROS 노드 초기화
        rospy.init_node("sensor_fusion_node")


        # 변수 초기화
        self.gravity    = 9.86
        self.K = 1
        self.gyro_phi   = 0
        self.gyro_theta = 0
        self.gyro_psi   = 0

        self.kf_phi     = 0
        self.kf_theta   = 0
        self.kf_psi     = 0

        self.b_acc      = np.zeros((3, 1))
        self.b_acc_n    = np.zeros((3, 1))

        self.b_gyro     = np.zeros((1, 3))
        self.pos        = np.zeros((3, 1))
        self.vel        = np.zeros((3, 1))

        self.acc_b_m    = np.zeros((3, 1))

        self.Cb2n       = np.eye(3)

        self.b_acc_o    = np.array([-0.2927, 0.1928, -0.0194])  # 초기값
        self.b_acc      = np.array([-0.2921, 0.1928, -0.019])
        self.i          = 1
        self.Acc_S      = np.zeros((3, 1))
        self.Gyro_S     = np.zeros((3, 1))
        self.dt         = 0.01
        self.Lv         = 10

        self.IMUacc     = np.zeros((2*self.Lv, 3))
        self.IMUgyro    = np.zeros((2*self.Lv, 3))
        self.Acc_Acc    = np.zeros((3, 2*self.Lv))
        self.Acc_Vel    = np.zeros((3, 2*self.Lv))
        self.Acc_Pos    = np.zeros((3, 2*self.Lv))
        self.Acc_PQR    = np.zeros((3, 2*self.Lv))
        self.Acc_Steady = np.zeros((3, 2*self.Lv))
        self.UWB_LS_Pos = np.zeros((3, 2*self.Lv))
        self.UWB_M_Pos  = np.zeros((3, 2*self.Lv))
        self.UWB_M_Vel  = np.zeros((3, 2*self.Lv))
        self.UWB_RAW    = np.zeros((3, 10))

        self.ANCHOR = {}
        self.IMU = {}
        self.FUSION = {}
        self.DWM1001 = {}

        self.statusImu  = None
        self.statusTag  = None
        self.statusPlotter = None
        self.statusAnchor  = None
        self.i = 1
        self.kf_imu = None
        self.pi = math.pi
        self.init_pos = None
        self.alpha = 1.0
        self.update_interval = 500

        # ROS 노드 구독
        rospy.Subscriber("/zed_f9r/imu", Imu, self.ImuCallback)
        rospy.Subscriber("/dwm1001/anchor/tag1", Anchor, self.Anchorcallback)
        rospy.Subscriber("/dwm1001/anchor/tag2", Anchor, self.Anchorcallback)
        rospy.Subscriber("/dwm1001/anchor/tag3", Anchor, self.Anchorcallback)
        rospy.Subscriber("/dwm1001/anchor/tag4", Anchor, self.Anchorcallback)
        rospy.Subscriber("/dwm1001/tag1", Tag, self.TagCallback)
        rospy.Subscriber("/dwm1001/tag2", Tag, self.TagCallback)
        rospy.Subscriber("/dwm1001/tag3", Tag, self.TagCallback)
        rospy.Subscriber("/dwm1001/tag4", Tag, self.TagCallback)


    def ImuCallback(self, msg):
        if self.statusImu == None:
            self.statusImu = True

        if self.statusImu == True:
            self.IMUacc[:-1, :] = self.IMUacc[1:, :]
            self.IMUacc[-1:, :] = [
                msg.linear_acceleration.x,
                msg.linear_acceleration.y,
                msg.linear_acceleration.z
            ]

            self.IMUgyro[:-1, :] = self.IMUgyro[1:, :]
            self.IMUgyro[-1:, :] = [
                msg.angular_velocity.x,
                msg.angular_velocity.y,
                msg.angular_velocity.z
            ]

            self.p = self.IMUgyro[-1:, 0] - self.b_gyro[0][0]
            self.q = self.IMUgyro[-1:, 1] - self.b_gyro[0][1]
            self.r = self.IMUgyro[-1:, 2] - self.b_gyro[0][2]
            self.acc_b = self.IMUacc[-1:,:]-self.b_acc_o
            self.acc_b = self.acc_b.T


            #---- Phi, Theta, Psi --------------#
            self.gyro_phi_prev   = self.gyro_phi
            self.gyro_theta_prev = self.gyro_theta
            self.gyro_psi_prev   = self.gyro_psi
            self.kf_phi_prev     = self.kf_phi
            self.kf_theta_prev   = self.kf_theta
            self.kf_psi_prev     = self.kf_psi
            #-----------------------------------#

            self.main()

    def Anchorcallback(self, msg):
        if self.statusAnchor == None:
            self.uwb_itme = float(msg.header.stamp.secs) +  float(msg.header.stamp.nsecs/10**9)
            self.ranging = msg.distanceFromTag

            print(msg.id[3])
            self.anchor_info = {
                "AN1": {"id": msg.id[0], "pos": [msg.x[0], msg.y[0], msg.z[0]], "dist": msg.distanceFromTag[0]},
                "AN2": {"id": msg.id[1], "pos": [msg.x[1], msg.y[1], msg.z[1]], "dist": msg.distanceFromTag[1]},
                "AN3": {"id": msg.id[2], "pos": [msg.x[2], msg.y[2], msg.z[2]], "dist": msg.distanceFromTag[2]},
                "AN4": {"id": msg.id[3], "pos": [msg.x[3], msg.y[3], msg.z[3]], "dist": msg.distanceFromTag[3]}
            }
            self.uwb = {}
            self.UWB = self.uwb
            self.UWB['flag'] = False
            self.statusAnchor = True


        if self.statusAnchor == True:
            self.uwb['time'] = msg.header.stamp.secs + msg.header.stamp.nsecs/10**9
            anchorID_tmp = msg.id
            ranging_tmp = msg.distanceFromTag
            if len(anchorID_tmp) != 0:
                self.uwb['anchorID'] = anchorID_tmp

            self.uwb['ranging'] = np.array([ranging_tmp[p] for p in range(len(ranging_tmp))]).reshape(-1, 1)
            self.UWB = self.uwb
            self.UWB['dist'] = self.UWB['ranging']
            self.UWB['flag'] = True
            self.UWB['num'] = len(self.uwb['anchorID'])
            self.UWB['anchor'] = np.array([np.array(msg.x), np.array(msg.y), np.array(msg.z)]).T
            self.UWB['id'] = self.uwb['anchorID']
            self.UWB['x'] = msg.x
            self.UWB['y'] = msg.y
            self.ANCHOR['id'] = msg.header.frame_id
            self.ANCHOR['x'] = msg.x
            self.ANCHOR['y'] = msg.y
            self.DWM1001[msg.header.frame_id] = self.UWB


    def TagCallback(self, msg):
        if self.statusTag  == None:
            self.init_pos = np.array([[msg.x, msg.y, msg.z]])
            self.statusTag = True
            self.TAG = {}
            self.tag_data_x = []
            self.tag_data_y = []

        if self.statusTag  == True:
            if msg.header.frame_id not in self.TAG:
                self.tag_data_x.append(msg.x)
                self.tag_data_y.append(msg.y)
            self.TAG[str(msg.header.frame_id)] = {'x': msg.x, 'y': msg.y}
            # print(self.TAG[self.ANCHOR['id']])


    def rotation_vector_to_matrix(self, rotation_vector):
        used_phi, used_theta, used_psi = rotation_vector.ravel()  # ravel() 함수를 사용하여 (3, 1) 형태를 (3,) 형태로 변환

        # Rotation matrices around x, y, and z axes
        R_x = np.array([[1, 0, 0],
                        [0, np.cos(used_phi), -np.sin(used_phi)],
                        [0, np.sin(used_phi), np.cos(used_phi)]])

        R_y = np.array([[np.cos(used_theta), 0, np.sin(used_theta)],
                        [0, 1, 0],
                        [-np.sin(used_theta), 0, np.cos(used_theta)]])

        R_z = np.array([[np.cos(used_psi), -np.sin(used_psi), 0],
                        [np.sin(used_psi), np.cos(used_psi), 0],
                        [0, 0, 1]])

        # Combine rotations
        R = np.dot(R_z, np.dot(R_y, R_x))

        return R

    def euler_gyro_update(self, p, q, r, dt, phi_p, theta_p, psi_p):
        update_mat = np.array([
            [1, np.sin(phi_p) * np.tan(theta_p), np.cos(phi_p) * np.tan(theta_p)],
            [0, np.cos(phi_p), -np.sin(phi_p)],
            [0, np.sin(phi_p) / np.cos(theta_p), np.cos(phi_p) / np.cos(theta_p)]
        ])

        next_euler = np.array([phi_p, theta_p, psi_p]).reshape(-1, 1) + dt * update_mat @ np.array([p, q, r]).reshape(-1, 1)

        phi = next_euler[0, 0]
        theta = next_euler[1, 0]
        psi = next_euler[2, 0]

        return phi, theta, psi

    def euler_acc(self, ax, ay, az):
        phi = math.atan(ay / az)
        theta = math.atan(ax / math.sqrt(ay**2 + az**2))

        return phi, theta

    def euler_to_quaternion(self, phi, theta, psi):
        sin_phi = math.sin(phi / 2)
        cos_phi = math.cos(phi / 2)
        sin_theta = math.sin(theta / 2)
        cos_theta = math.cos(theta / 2)
        sin_psi = math.sin(psi / 2)
        cos_psi = math.cos(psi / 2)

        z = np.array([
            cos_phi * cos_theta * cos_psi + sin_phi * sin_theta * sin_psi,
            sin_phi * cos_theta * cos_psi - cos_phi * sin_theta * sin_psi,
            cos_phi * sin_theta * cos_psi + sin_phi * cos_theta * sin_psi,
            cos_phi * cos_theta * sin_psi - sin_phi * sin_theta * cos_psi,
        ])

        return z

    def uwb_LS_positioning(self, UWB, PosE):
        global Pos_prev, firstRun

        if 'firstRun' not in globals():
            firstRun = 1
            Pos_prev = np.array([0, 0, 0]).reshape(-1, 1)

        nAnchor = UWB['num']

        meas_dist = UWB['dist']
        anchor = UWB['anchor']
        # print("anchor :", anchor)

        Est_Dist = np.linalg.norm(anchor[0:4, :] - PosE.reshape(1, -1), axis=1)

        ErroDist = np.abs(Est_Dist - meas_dist)

        Ind = np.where(ErroDist > 0.2)[0]
        meas_dist[Ind] = meas_dist[Ind] - 0.2
        UWB['dist'] = meas_dist
        UWB['pos'] = self.uwb_pos_update(UWB, PosE)  # Assuming the uwb_pos_update_1 function is defined

        return UWB

    def uwb_pos_update(self, UWB, Pos):
        anchor = UWB['anchor']

        P = 1000 * np.eye(3)

        p_ned = Pos

        zhat_tmp = np.zeros((1, UWB['num']))
        # print("zhat_tmp", zhat_tmp.shape)
        # print("zhat_tmp", zhat_tmp)
        for i in range(UWB['num']):
            # print(i, ": zhat_tmp[i] :", zhat_tmp[0][i].shape)
            # print(i, ": zhat_tmp[i] :", zhat_tmp[0][i])
            # print(i, ": anchor[i, 0:] :", anchor[i, 0:].shape)
            # print(i, ": anchor[i, 0:] :", anchor[i, 0:])
            # print(i, ": p_ned.T :", p_ned.T.shape)
            # print(i, ": p_ned.T :", p_ned.T)
            # print(i, ": np.linalg.norm(anchor[i, 0:] - p_ned.T): ", np.linalg.norm(anchor[i, 0:] - p_ned.T))
            zhat_tmp[0][i] = np.linalg.norm(anchor[i, 0:] - p_ned.T)
        zhat = zhat_tmp.T

        # print("zhat :", zhat)

        los = np.zeros((UWB['num'], 3))
        for i in range(UWB['num']):
            # print("(anchor[i, 0:] - p_ned.T) / np.linalg.norm(anchor[i, 0:] - p_ned.T) :",(anchor[i, 0:] - p_ned.T) / np.linalg.norm(anchor[i, 0:] - p_ned.T))
            # print("(anchor[i, 0:] - p_ned.T): ", (anchor[i, 0:] - p_ned.T))
            los[i, :] = (anchor[i, 0:] - p_ned.T) / np.linalg.norm(anchor[i, 0:] - p_ned.T)

        # print("los :", los)
        H = np.zeros((UWB['num'], 3))
        H[:, 0:3] = -los
        # print("H :", H)
        # print("UWB['dist'] :", UWB['dist'])

        R = 100 * np.eye(UWB['num'])
        Z = UWB['dist'] - zhat

        K = np.dot(np.dot(P, H.T), np.linalg.inv(np.dot(np.dot(H, P), H.T) + R))
        # print("K :", K.shape)
        # print("K :", K)
        # print("Z :", Z.shape)
        # print("Z :", Z)
        P = P - np.dot(np.dot(K, H), P)
        # print("P :", P.shape)
        # print("P :", P)
        # print("np.dot(K, Z) :", np.dot(K, Z).shape)
        # print("np.dot(K, Z) :", np.dot(K, Z))
        # print("np.zeros((3, 1)) :", np.zeros(3).shape)
        # print("np.zeros((3, 1)) :", np.zeros(3))
        err_X = np.zeros((3,1)) + np.dot(K, Z)
        # print("err_X :", err_X.shape)
        # print("err_X :", err_X)
        # print("p_ned :", p_ned.shape)
        # print("p_ned :", p_ned)
        # print("err_X[:3] :", err_X[:3])
        p_ned = p_ned + err_X[:3]

        return p_ned

    def main(self):
        if self.statusImu == True:
            if self.i > 50:
                self.used_phi        = self.kf_phi
                self.used_theta      = self.kf_theta
                self.used_psi        = self.kf_psi
                self.used_phi_next   = self.kf_phi
                self.used_theta_next = self.kf_theta
                self.used_psi_next   = self.kf_psi

                self.Acc_Vel[:, :-1] = self.Acc_Vel[:, 1:]
                self.Acc_Acc[:, :-1] = self.Acc_Acc[:, 1:]
                self.Acc_Pos[:, :-1] = self.Acc_Pos[:, 1:]
                self.Acc_Steady[:, :-1] = self.Acc_Steady[:, 1:]
                self.Acc_Acc[:, :-1]   = self.Acc_Acc[:, 1:]


                self.rotation_vector = np.array([self.used_phi, self.used_theta, self.used_psi]).T
                vel_t = np.dot(self.rotation_vector_to_matrix(self.rotation_vector), self.Acc_Vel[:,-2])
                self.Acc_PQR[:,-1:]     = 1*np.dot(np.array([[0, vel_t[2], -vel_t[1]], [-vel_t[2], 0, 0], [vel_t[2], 0, 0]]), np.array([self.p, self.q, self.r]))
                self.Acc_Steady[:, -1:] = (self.gravity * np.array([[np.sin(self.used_theta_next), -np.cos(self.used_theta_next) * np.sin(self.used_phi_next),-np.cos(self.used_theta_next) * np.cos(self.used_phi_next)]]).T)
                self.Acc_Acc[:,-1:]     = np.dot(self.rotation_vector_to_matrix(self.rotation_vector).T, self.acc_b - self.Acc_PQR[:, -1:] - self.Acc_Steady[:, -1:])

                self.gyro_phi, self.gyro_theta, self.gyro_psi = self.euler_gyro_update(self.p, self.q, self.r, self.dt, self.gyro_phi, self.gyro_theta, self.gyro_psi)
                self.acc_b_phi, self.acc_b_theta = self.euler_acc(self.acc_b[0], self.acc_b[1], self.acc_b[2])

                self.qua_acc = self.euler_to_quaternion(self.acc_b_phi,
                                                        self.acc_b_theta,
                                                        self.gyro_psi
                )
                self.qua_kf = self.euler_to_quaternion(self.kf_phi,
                                                    self.kf_theta,
                                                    self.kf_psi
                )
                self.AA = np.eye(4) + self.dt * 1/2 * np.array([[0, -self.p, -self.q, -self.r],
                                                    [self.p,  0,  self.r, -self.q],
                                                    [self.q, -self.r,  0,  self.p],
                                                    [self.r,  self.q, -self.p,  0],
                                                    ])

                euler_kalman = IMUKalmanFilter(self.qua_kf, self.AA, self.qua_acc)
                euler_kalman.predict()
                euler_kalman.update()
                self.kf_phi, self.kf_theta, self.kf_psi = euler_kalman.result()

                self.kf_phi   = np.real(self.kf_phi)
                self.kf_theta = np.real(self.kf_theta)
                self.kf_psi   = np.real(self.kf_psi)

                self.Acc_Vel[:, -1] = self.Acc_Vel[:, -2] + self.Acc_Acc[:, -1] * self.dt
                self.Acc_Pos[:, -1] = self.Acc_Pos[:, -2] + self.Acc_Vel[:, -2] * self.dt + self.Acc_Acc[:, -1] * self.dt**2 / 2

                self.IMU['x'] = self.Acc_Pos[0, -1]
                self.IMU['y'] = self.Acc_Pos[1, -1]
                # print("self.IMU :", self.IMU)

            else:
                self.Acc_Vel[:, :-1] = self.Acc_Vel[:, 1:]
                self.Acc_Acc[:, :-1] = self.Acc_Acc[:, 1:]
                self.Acc_Pos[:, :-1] = self.Acc_Pos[:, 1:]

                self.Acc_Acc[:, -1:] = np.zeros((3, 1))
                self.Acc_Vel[:, -1:] = np.zeros((3, 1))
                try:
                    self.Acc_Pos[:, -1:] = self.init_pos.T
                except AttributeError:
                    self.Acc_Pos[:, -1:] = np.array([[1.93, 3.82, 0.25]]).T


                self.acc_b_phi, self.acc_b_theta = self.euler_acc(self.acc_b[0], self.acc_b[1], self.acc_b[2])
                self.gyro_phi   = self.gyro_phi * (self.i - 1) / self.i + self.acc_b_phi / self.i
                self.kf_phi     = self.gyro_phi
                self.gyro_theta = self.gyro_theta * (self.i - 1) / self.i + self.acc_b_theta / self.i
                self.kf_theta   = self.gyro_theta
                self.gyro_psi   = self.pi/2
                self.b_gyro     = self.b_gyro * (self.i - 1) / self.i + self.IMUgyro[-1:,:] / self.i


        for an in self.DWM1001:
            if ((self.i>50) and self.DWM1001[an]['flag']):
                if self.DWM1001[an]['num'] >= 1:
                    self.DWM1001[an] = self.uwb_LS_positioning(self.DWM1001[an], self.Acc_Pos[:, -1:])
                    self.UWB_LS_Pos[:, :-1] = self.UWB_LS_Pos[:, 1:]
                    self.UWB_LS_Pos[:, -1:] = self.DWM1001[an]['pos']

                else:
                    self.UWB_LS_Pos[:, :-1] = self.UWB_LS_Pos[:, 1:]
                    self.UWB_LS_Pos[:, -1:] = self.UWB_LS_Pos[:, -2]

                if (self.K > self.Lv*2):
                    Lv = 10  # Lv 값을 정의해야 합니다.
                    Ta = np.mean(self.UWB_LS_Pos[:, -Lv:], axis=1)
                    Tb = np.mean(self.UWB_LS_Pos[:, -2 * Lv:-Lv], axis=1)
                    Tm = (Ta + Tb) / 2
                    Td = Ta - Tm

                    self.UWB_M_Pos[:, :-1] = self.UWB_M_Pos[:, 1:]
                    self.UWB_M_Vel[:, :-1] = self.UWB_M_Vel[:, 1:]

                    self.UWB_M_Pos[:, -1] = (Tm + Td)
                    self.UWB_M_Vel[:, -1] = (self.UWB_M_Pos[:, -1] - self.UWB_M_Pos[:, -10])/1

                else:
                    self.UWB_M_Pos[:, :-1] = self.UWB_M_Pos[:, 1:]
                    self.UWB_M_Vel[:, :-1] = self.UWB_M_Vel[:, 1:]

                    # print("self.init_pos.T :", self.init_pos.T.shape)
                    # print("self.init_pos.T :", self.init_pos.T)
                    self.UWB_M_Pos[:, -1:] = self.init_pos.T
                    self.UWB_M_Vel[:, -1:] = np.zeros((3, 1))

                self.R = 1e-2 * np.eye(6);
                # print("self.IMUacc[:, -1] :", self.IMUacc[-1, :].shape)
                # print("self.IMUacc[:, -1] :", self.IMUacc[-1, :])
                self.TEMP = self.IMUacc[-1, :].T - self.Acc_Steady[:, -1]
                # print("self.TEMP :", self.TEMP)
                self.TEMP_bias = self.b_acc_o

                # self.x = [self.Acc_Pos[:, -2], self.Acc_Vel[:, -2], self.TEMP, self.TEMP_bias]
                # print("w")
                self.x = np.concatenate((self.Acc_Pos[:, -2], self.Acc_Vel[:, -2]), axis=0)
                self.x = np.concatenate((self.x, self.TEMP), axis=0)
                self.x = np.concatenate((self.x, self.TEMP_bias), axis=0)
                # print("self.x", self.x)
                # print("self.x", self.x.shape)

                self.Cb2n = self.rotation_vector_to_matrix(self.rotation_vector).T
                self.A = np.eye(12)
                self.A[0:3, 3:6] = self.dt * np.eye(3) - 1/2 * self.dt**2 * np.array([[0, -self.r, self.q], [self.r, 0, -self.p], [-self.q, self.p, 0]])
                self.A[0:3, 6:9] = self.Cb2n * self.dt**2 / 2
                self.A[0:3, 9:12] = -self.Cb2n * self.dt**2 / 2
                self.A[3:6, 3:6] = self.A[3:6, 3:6] - self.dt * np.array([[0, -self.r, self.q], [self.r, 0, -self.p], [-self.q, self.p, 0]])
                self.A[3:6, 6:9] = self.Cb2n * self.dt
                self.A[3:6, 9:12] = -self.Cb2n * self.dt

                self.z = np.concatenate((self.UWB_M_Pos[:, -1], self.UWB_M_Vel[:, -1]), axis=0)
                self.x_n = UWBKalmanFilter(self.x, self.A, self.z, self.R)
                self.x_n.predict()
                self.x_n = self.x_n.update()

                # print("self.x_n:", self.x_n)
                # print("self.x_n:", self.x_n.shape)
                # print("self.x_n[0:3, 0] :", self.x_n[0:3].shape)
                # print("self.x_n[0:3, 0] :", self.x_n[0:3])
                self.Acc_Pos[:, -1] = self.x_n[0:3]
                self.Acc_Vel[:, -1] = self.x_n[3:6]
                self.acc_g = self.x_n[6:9]
                self.b_acc_o = self.x_n[9:12].T
                self.K += 1

                self.DWM1001[an]['flag'] = False
                self.Acc_S = np.zeros((3,1))

        # self.fig.tight_layout()
        # print(self.Acc_Pos[:, -1])
        self.FUSION['x'] = self.Acc_Pos[0, -1]
        self.FUSION['y'] = self.Acc_Pos[1, -1]
        # self.canvas.draw()
        # live plotting
        # self.live_plotter.add_data_point_uwb_pos(mode='uwb', object_name=self.TAG['id'], x=self.Acc_Pos[0, -1], y=self.Acc_Pos[1, -1])

        self.i += 1

    def get_anchor_data(self):
        return self.ANCHOR['id'], self.ANCHOR['x'], self.ANCHOR['y']

    def get_uwb_data(self):
        return self.ANCHOR['id'], self.TAG[self.ANCHOR['id']]['x'], self.TAG[self.ANCHOR['id']]['y']

    def get_imu_data(self):
        return self.ANCHOR['id'], self.IMU['x'], self.IMU['y']

    def get_fusion_data(self):
        return self.ANCHOR['id'], self.FUSION['x'], self.FUSION['y']

if __name__ == '__main__':
    fusion = Sensor_fusion()
    root = tk.Tk()
    plotter = LivePlotter(root, fusion)
    plotter.update_plot()
    root.mainloop()
