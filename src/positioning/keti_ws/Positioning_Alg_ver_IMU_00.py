#!/usr/bin/env python3.8

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

# from ctypes.wintypes import PPOINTL
from stringprep import c6_set
import rospy
import numpy as np
import matplotlib.pyplot as plt
import cmath
import math
import time

from sensor_msgs.msg import Imu
from localizer_dwm1001.msg import Anchor
from localizer_dwm1001.msg import Tag

import numpy as np
from numpy.linalg import inv


class IMUKalmanFilter(object):
    def __init__(self, x, A, z):
        self.threshold = 100
        self.x = x
        self.P = 2 * np.eye(4)
        self.A = A
        self.H = np.eye(4)
        self.R = 1 * np.eye(4)
        self.Q = 1e-4 * np.eye(4)
        self.I = np.identity(6)
        self.S = None
        self.K = None
        self.z = z

    def predict(self):

        self.xp = np.dot(self.A, self.x)
        self.Pp = np.dot(np.dot(self.A, self.P), self.A.T) + self.Q
        self.H = self.H.astype(np.float64)
        self.R = self.R.astype(np.float64)
        self.Pp = self.Pp.astype(np.float64)

    def update(self):
        self.K = np.dot(np.dot(self.Pp, self.H.T), inv(np.dot(np.dot(self.H, self.Pp), self.H.T) + self.R))

        self.x = self.xp + np.dot(self.K, (self.z - np.dot(self.H, self.xp)))
        self.P = self.Pp - np.dot(np.dot(self.K, self.H), self.Pp)


    def result(self):
        phi = np.arctan2(2 * (self.x[2] * self.x[3] + self.x[0] * self.x[1]), 1 - 2 * (self.x[1] ** 2 + self.x[2] ** 2))
        theta = -np.arcsin(2 * (self.x[1] * self.x[3] - self.x[0] * self.x[2]))
        psi = np.arctan2(2 * (self.x[1] * self.x[2] + self.x[0] * self.x[3]), 1 - 2 * (self.x[2] ** 2 + self.x[3] ** 2))

        return phi, theta, psi

class IMUExtendedKalmanFilter(object):
    def __init__(self, x, A, z):
        self.x = x
        self.A = A
        self.z = z
        self.Delta_t = 0.001
        self.Variance_phi_D = 0.0035
        self.Variance_a = 0.01**2

        self.Variance_x = 10**(-4)
        self.Variance_y = 10**(-4)
        self.Variance_phi = 10**(-5)

        self.Varince_x_gps = 0.01
        self.Varince_y_gps = 0.01
        self.Variance_Acc  = 0.001
        self.Variance_gyro = 5*(10**(-5))

        self.P_kM1 = np.array([[self.Variance_x, 0,0,0,0,0],
                               [0,self.Variance_x,0,0,0,0],
                               [0,0,self.Variance_x,0,0,0],
                               [0,0,0,0,0,0],
                               [0,0,0,0,0,0],
                               [0,0,0,0,0,0]])

        self.Q = np.array([[self.Variance_phi_D, 0],
                           [0, self.Variance_a]])
        self.GQG_T = np.array([[0,0,0,0,0,0],
                               [0,0,0,0,0,0],
                               [0,0,0,0,0,0],
                               [0,0,0,self.Variance_phi_D*self.Delta_t**2,0,0],
                               [0,0,0,0,0,0],
                               [0,0,0,0,0,self.Variance_a*self.Delta_t**2]])

        self.R_gps = np.array([[self.Varince_x_gps,0,0,0],
                               [0,self.Varince_y_gps,0,0],
                               [0,0,self.Variance_Acc,0],
                               [0,0,0,self.Variance_gyro]])

        self.H = np.array([[1,0,0,0,0,0],
                               [0,1,0,0,0,0],
                               [0,0,0,0,0,1],
                               [0,0,0,1,0,0]])


    def predict(self):
        # b = self.A
        # c = self.A.T
        # print("b", b)
        # print("b", b.shape)
        # print("c", c)
        # print("c", c.shape)

        self.P_kM = (self.A @ self.P_kM1 @ self.A.T) + self.GQG_T
        a = self.A @ self.P_kM1 @ self.A.T
        # print("a", a)
        # print("a", a.shape)
        # print("#####################################################")
        # print("self.P_kM1 :", self.P_kM1)
        # print("self.P_kM1 :", self.P_kM1.shape)
        # print("self.GQG_T :", self.GQG_T)
        # print("self.GQG_T :", self.GQG_T.shape)
        # print("self.P_kM :", self.P_kM)
        # print("self.P_kM :", self.P_kM.shape)

    def update(self):
        # print("x :", self.x)
        # print("x :", self.x.shape)
        # print("A :", self.A.shape)
        # print("A :", self.A.shape)
        # print("self.H : ", self.H)
        # print("self.H : ", self.H.shape)
        # print("self.R_gps : ", self.R_gps)
        # print("self.R_gps : ", self.R_gps.shape)

        H = self.H @ self.x
        # print("H :", H)
        # print("H :", H.shape)
        K = self. P_kM @ self.H.T @ np.array(np.linalg.inv(self.H @ self.H.T @ self.R_gps))
        # print("K :", K)
        # print("K :", K.shape)
        x = self.x + K @ np.array(self.z - H)
        # print("z", self.z)
        # print("z", self.z.shape)
        # print("x2", x)
        # print("x2", x.shape)
        P = np.array(np.eye(6) - K @ self.H) @ self.P_kM

        self.P_kM1 = self.P_kM

        return x


# class Quaternion:
#     def __init__(self, w=1, i=0, j=0, k=0):
#         self.w = w
#         self.i = i
#         self.j = j
#         self.k = k

#     def normalize(self):
#         norm = (self.w**2 + self.i**2 + self.j**2 + self.k**2)**0.5
#         self.w /= norm
#         self.i /= norm
#         self.j /= norm
#         self.k /= norm

# class MadgwickFilter:
#     def __init__(self, beta, vel, pos):
#         self.beta = beta
#         self.vel = vel
#         self.pos = pos
#         self.q = Quaternion()

#     def update(self, accel, gyro, xt_c, yt_c, dt):
#         q = self.q
#         q_dot = Quaternion(
#             0.5 * (-q.i * gyro[0] - q.j * gyro[1] - q.k * gyro[2]),
#             0.5 * ( q.w * gyro[0] + q.j * gyro[2] - q.k * gyro[1]),
#             0.5 * ( q.w * gyro[1] - q.i * gyro[2] + q.k * gyro[0]),
#             0.5 * ( q.w * gyro[2] + q.i * gyro[1] - q.j * gyro[0])
#         )


#         accel = accel - np.cross(gyro, self.vel)
#         self.vel = self.vel + accel * 0.01
#         self.pos = np.array([xt_c, yt_c, 0]) + self.vel * 0.01

#         if np.linalg.norm(accel) == 0:
#             return
#         accel = accel / np.linalg.norm(accel)

#         f = np.array([
#             2 * (q.i * q.k - q.w * q.j) - accel[0],
#             2 * (q.w * q.i + q.j * q.k) - accel[1],
#             2 * (0.5 - q.i**2 - q.j**2) - accel[2]
#         ])

#         j = np.array([
#             [-2 * q.j,  2 * q.k, -2 * q.w, 2 * q.i],
#             [ 2 * q.i,  2 * q.w,  2 * q.k, 2 * q.j],
#             [     0, -4 * q.i, -4 * q.j,     0]
#         ])

#         grad = Quaternion(*np.dot(j.T, f))
#         grad.normalize()

#         q_dot.w -= self.beta * grad.w
#         q_dot.i -= self.beta * grad.i
#         q_dot.j -= self.beta * grad.j
#         q_dot.k -= self.beta * grad.k

#         q.w += q_dot.w * dt
#         q.i += q_dot.i * dt
#         q.j += q_dot.j * dt
#         q.k += q_dot.k * dt

#         self.q.normalize()


class Sensor_fusion:
    def __init__(self):
        # ROS 노드 초기화
        rospy.init_node("sensor_fusion_node")
        np.set_printoptions(threshold=np.inf)

        self.id_order = None
        self.id_order_check = False

        self.pos_gt = np.array([0.411, 3.083, 0])

        self.gravity = 9.86
        self.dt         = 0.01

        self.UWB = dict()
        self.uwb0 = dict()
        self.uwb1 = dict()
        self.uwb2 = dict()
        self.uwb3 = dict()

        self.IMU = dict()
        self.acc = dict()
        self.ang = dict()

        self.xt_b = np.array([-0.09, 0.09, -0.09, 0.09])
        self.yt_b = np.array([0.12, 0.12, -0.12, -0.12])


        self.statusIMU = False
        self.statusUWB = False

        self.accel_bias = np.array([0, 0, 0])
        self.gyro_bias = np.array([0, 0, 0])

        self.gyro_gx = 0
        self.gyro_gy = 0
        self.gyro_gz = 0

        self.acc_gx = 0
        self.acc_gy = 0
        self.acc_gz = 0

        self.gyro_gx = 0
        self.gyro_gy = 0
        self.gyro_gz = 0
        self.gyro_gz_D = 0

        self.accel = np.array([[0],[0],[0]])
        self.vel = np.zeros((3,1))
        self.pos = np.zeros((3,1))
        self.orientation = None

        self.xt_b_center = np.mean(self.xt_b);
        self.yt_b_center = np.mean(self.yt_b);
        self.angles_from_heading = np.arctan2(self.yt_b, self.xt_b);
        self.rl = np.sqrt(self.xt_b**2+self.yt_b**2)
        self.tag_pos_b = self.xt_b + 1j*self.yt_b;
        self.I = 1
        self.U = 0

        self.heading_est_a = np.array([]).reshape(-1 ,1)
        self.centerest_a = np.array([]).reshape(-1, 2)
        self.centerest_a_aver = np.array([]).reshape(-1, 2)

        self.headingest_a_aver = np.array([]).reshape(-1 ,1)
        self.K_centerest_a_aver = np.array([]).reshape(-1, 2)
        self.K_headingest_a_aver = np.array([]).reshape(-1 ,1)

        self.flag = None

        # ROS 노드 구독
        rospy.Subscriber("/zed_f9r/imu", Imu, self.ImuCallback)
        rospy.Subscriber("/dwm1001/anchor/ttyUWB0", Anchor, self.Anchorcallback0)
        rospy.Subscriber("/dwm1001/anchor/ttyUWB1", Anchor, self.Anchorcallback1)
        rospy.Subscriber("/dwm1001/anchor/ttyUWB2", Anchor, self.Anchorcallback2)
        rospy.Subscriber("/dwm1001/anchor/ttyUWB3", Anchor, self.Anchorcallback3)
        # rospy.Subscriber("/dwm1001/ttyUWB0", Tag, s3elf.TagCallback)
        # rospy.Subscriber("/dwm1001/ttyUWB1", Tag, self.TagCallback)
        # rospy.Subscriber("/dwm1001/ttyUWB2", Tag, self.TagCallback)
        # rospy.Subscriber("/dwm1001/ttyUWB3", Tag, self.TagCallback)


    def create_id_order(self, ids):
        if self.id_order is None:
            self.id_order = ids
        else:
            # Check if ids is not equal to self.id_order
            if set(ids) != set(self.id_order):
                # ids에서 self.id_order에 없는 요소를 찾습니다.
                difference = [item for item in ids if item not in self.id_order]

                # 이 요소들을 self.id_order에 추가합니다.
                self.id_order.extend(difference)

    def uwb_sort(self, num, msg, uwb):
        self.create_id_order(msg.id)
        try:
            uwb['tag_id'] = num
            uwb['id'] = [id for id in self.id_order]

            order_indices = [msg.id.index(id) for id in self.id_order]

            # 이제 order_indices를 사용해서 msg의 나머지 속성을 재정렬하거나 'Nan'을 할당합니다:
            uwb['x'] = [msg.x[i] for i in order_indices]
            uwb['y'] = [msg.y[i] for i in order_indices]
            uwb['z'] = [msg.z[i] for i in order_indices]
            uwb['dist'] = [msg.distanceFromTag[i] for i in order_indices]
        except ValueError:
            pass

    def uwb_init(self):
        self.UWB['dist'] = np.array([self.uwb0['dist'], self.uwb1['dist'], self.uwb2['dist'], self.uwb3['dist']])
        self.UWB['dist'] = self.UWB['dist'].T

        id = np.array(next((tag for tag in [self.uwb0['id'], self.uwb1['id'], self.uwb2['id'], self.uwb3['id']] if 'Nan' not in tag), None))
        x = np.array(next((tag for tag in [self.uwb0['x'], self.uwb1['x'], self.uwb2['x'], self.uwb3['x']] if 'Nan' not in tag), None))
        y = np.array(next((tag for tag in [self.uwb0['y'], self.uwb1['y'], self.uwb2['y'], self.uwb3['y']] if 'Nan' not in tag), None))
        z = np.array(next((tag for tag in [self.uwb0['z'], self.uwb1['z'], self.uwb2['z'], self.uwb3['z']] if 'Nan' not in tag), None))

        if x is not None and y is not None and z is not None:
            self.UWB['id'] = id
            self.UWB['x'] = x
            self.UWB['y'] = y
            self.UWB['z'] = z
        else:
            self.UWB['id'] = self.UWB['id']
            self.UWB['x'] = self.UWB['x']
            self.UWB['y'] = self.UWB['y']
            self.UWB['z'] = self.UWB['z']

    def ImuCallback(self, msg):
        self.IMU['lx'] = msg.linear_acceleration.x
        self.IMU['ly'] = msg.linear_acceleration.y
        self.IMU['lz'] = msg.linear_acceleration.z

        self.IMU['gx'] = msg.angular_velocity.x
        self.IMU['gy'] = msg.angular_velocity.y
        self.IMU['gz'] = msg.angular_velocity.z

        self.statusIMU = True


    def Anchorcallback0(self, msg):
        self.uwb_sort(0, msg, self.uwb0)

    def Anchorcallback1(self, msg):
        self.uwb_sort(1, msg, self.uwb1)

    def Anchorcallback2(self, msg):
        self.uwb_sort(2, msg, self.uwb2)

    def Anchorcallback3(self, msg):
        self.uwb_sort(3, msg, self.uwb3)


    def GetUWBPos_v1(self, xa, ya, dist, angles_from_heading):
        # print(angles_from_heading)
        Li = 0
        A = np.zeros((144, 8))   # Adjust the shape according to your need
        Y = np.zeros((144, 1))   # Adjust the shape according to your need
        for n in range(4):
            for m in range(4):
                for p in range(4):
                    for q in range(4):
                        if n != m and p != q:
                            A[Li, 2*p:2*p+2] = [2*(xa[n]-xa[m]), 2*(ya[n]-ya[m])]
                            A[Li, 2*q:2*q+2] = [2*(xa[n]-xa[m]), 2*(ya[n]-ya[m])]
                            Y[Li, 0] = dist[n, p]**2 + dist[n, q]**2 - dist[m, p]**2 - dist[m, q]**2 - 2*xa[n]**2 + 2*xa[m]**2 - 2*ya[n]**2 + 2*ya[m]**2
                            Li += 1


        np.set_printoptions(threshold=np.inf)
        Res = np.linalg.inv(A.T @ A) @ A.T @ Y
        Xt_e = -Res[::2]
        Yt_e = -Res[1::2]

        # Estimated Center Position
        Xt_c_e = np.mean(Xt_e)
        Yt_c_e = np.mean(Yt_e)

        # Estimated Heading
        tag_pos_est = Xt_e.T + 1j*Yt_e.T
        tag_arrow_est = np.sum((tag_pos_est - Xt_c_e - 1j*Yt_c_e) * np.exp(-1j*angles_from_heading))
        heading_est = np.arctan2(np.imag(tag_arrow_est), np.real(tag_arrow_est))

        return tag_pos_est, heading_est

    def GetUWBPosUpdate_v1(self, xa, ya, Xt_c_e, Yt_c_e, heading_est, rl, dist, angles_from_heading):
        Lj = 0
        B = np.zeros((48, 3))
        W = np.zeros((48, 1))

        for p in range(4):
            for n in range(4):
                for m in range(4):
                    if n != m:
                        if n > 100:
                            continue
                        else:
                            B[Lj,0] = -2 * (xa[n] - xa[m])
                            B[Lj,1] = -2 * (ya[n] - ya[m])
                            B[Lj,2] = 2 * (xa[n] - xa[m]) * rl[p] * np.sin(heading_est + angles_from_heading[p]) - 2 * (ya[n] - ya[m]) * rl[p] * np.cos(heading_est + angles_from_heading[p])
                            W[Lj,0] = dist[n,p]**2 - dist[m,p]**2 - xa[n]**2 + xa[m]**2 + 2 * (rl[p] * np.cos(heading_est + angles_from_heading[p]) + Xt_c_e) * (xa[n] - xa[m]) - ya[n]**2 + ya[m]**2 + 2 * (rl[p] * np.sin(heading_est + angles_from_heading[p]) + Yt_c_e) * (ya[n] - ya[m])
                            Lj += 1

        Refined = np.linalg.inv(B.T @ B) @ B.T @ W
        Xt_c_e = Xt_c_e + Refined[0]
        Yt_c_e = Yt_c_e + Refined[1]
        tag_center_pos_est = Xt_c_e + 1j * Yt_c_e
        heading_est = heading_est + Refined[2]

        return tag_center_pos_est, heading_est, Xt_c_e, Yt_c_e

    def get_tag_pos(self, tag_center_pos_est, heading_est, tag_pos_b):
        tag_pos_est = tag_pos_b * np.exp(1j * heading_est) + tag_center_pos_est
        return tag_pos_est.reshape(1, -1)


    def TwoAnchPos3(self, Xa, Ya, dist, tag_pos, EstCenter, anch_pos, dist_a):
        AA = np.sqrt((Xa[0] - Xa[1])**2 + (Ya[0] - Ya[1])**2)
        B = dist[0]
        C = dist[1]

        if B**2 - ((B**2 - C**2 + AA**2) / (2*AA))**2 > 0:
            d = np.sqrt(B**2 - ((B**2 - C**2 + AA**2) / (2*AA))**2)
        else:
            d = 0

        A = np.array([[Ya[1] - Ya[0], -(Xa[1] - Xa[0])], [2 * (Xa[1] - Xa[0]), 2 * (Ya[1] - Ya[0])]])
        Y1 = np.array([[d * np.sqrt((Ya[1] - Ya[0])**2 + (Xa[1] - Xa[0])**2) + Xa[0] * Ya[1] - Xa[1] * Ya[0], -(C**2 - B**2 - Xa[0]**2 + Xa[1]**2 - Ya[0]**2 + Ya[1]**2)]]).T
        Y2 = np.array([[-d * np.sqrt((Ya[1] - Ya[0])**2 + (Xa[1] - Xa[0])**2) + Xa[0] * Ya[1] - Xa[1] * Ya[0], -(C**2 - B**2 - Xa[0]**2 + Xa[1]**2 - Ya[0]**2 + Ya[1]**2)]]).T

        X1 = np.linalg.inv(A.T @ A) @ A.T @ Y1
        X2 = np.linalg.inv(A.T @ A) @ A.T @ Y2

        X1S = np.sum(np.abs(np.abs(anch_pos - (X1[0] + 1j * X1[1])) - dist_a.T))
        X2S = np.sum(np.abs(np.abs(anch_pos - (X2[0] + 1j * X2[1])) - dist_a.T))
        Y1S = np.sum((X1 - tag_pos.T)**2)
        Y2S = np.sum((X2 - tag_pos.T)**2)
        Z1S = np.abs(np.sum((X1 - EstCenter.T)**2) - 0.5)
        Z2S = np.abs(np.sum((X2 - EstCenter.T)**2) - 0.5)

        Prob = np.array([[]])


        if (X1S + Y1S) > (X2S + Y2S):
            list_of_arrays = [X2.T, X1.T]
            Pos = np.vstack(list_of_arrays)
            Prob = np.array([X2S + Y2S, X1S + Y1S]).reshape(-1, 1)
        else:
            list_of_arrays = [X1.T, X2.T]
            Pos = np.vstack(list_of_arrays)
            Prob = np.array([X1S + Y1S, X2S + Y2S]).reshape(-1, 1)

        return Pos, Prob

    def TwoAnchPos4(self, Xa, Ya, dist, tag_pos, EstCenter, anch_pos, dist_a):
        AA = np.sqrt((Xa[0] - Xa[1])**2 + (Ya[0] - Ya[1])**2)
        B = dist[0]
        C = dist[1]

        s1 = (AA+B+C)/2
        S2 = np.real(cmath.sqrt(s1*(s1-AA)*(s1-B)*(s1-C)))
        d = 2*S2/AA

        A = np.array([[Ya[1] - Ya[0], -(Xa[1] - Xa[0])], [2 * (Xa[1] - Xa[0]), 2 * (Ya[1] - Ya[0])]])
        Y1 = np.array([[d * np.sqrt((Ya[1] - Ya[0])**2 + (Xa[1] - Xa[0])**2) + Xa[0] * Ya[1] - Xa[1] * Ya[0], (-C**2 + B**2 - Xa[0]**2 + Xa[1]**2 - Ya[0]**2 + Ya[1]**2)]]).T
        Y2 = np.array([[-d * np.sqrt((Ya[1] - Ya[0])**2 + (Xa[1] - Xa[0])**2) + Xa[0] * Ya[1] - Xa[1] * Ya[0], (-C**2 + B**2 - Xa[0]**2 + Xa[1]**2 - Ya[0]**2 + Ya[1]**2)]]).T

        X1 = np.linalg.inv(A.T @ A) @ A.T @ Y1
        X2 = np.linalg.inv(A.T @ A) @ A.T @ Y2

        X1S = np.sum(np.abs(np.abs(anch_pos - (X1[0] + 1j * X1[1])) - dist_a.T))
        X2S = np.sum(np.abs(np.abs(anch_pos - (X2[0] + 1j * X2[1])) - dist_a.T))
        Y1S = np.sum((X1 - tag_pos.T.reshape(-1, 1))**2)
        Y2S = np.sum((X2 - tag_pos.T.reshape(-1, 1))**2)
        Z1S = np.abs(np.sum((X1 - EstCenter.T.reshape(-1, 1))**2) - 0.5)
        Z2S = np.abs(np.sum((X2 - EstCenter.T.reshape(-1, 1))**2) - 0.5)

        # Pos = np.array([[]])
        Prob = np.array([[]])


        if (X1S + Y1S) > (X2S + Y2S):
            list_of_arrays = [X2.T, X1.T]
            # Pos = np.append(Pos, np.array([[X2.T, X1.T]]), axis=0)
            Pos = np.vstack(list_of_arrays)
            Prob = np.array([X2S + Y2S, X1S + Y1S]).reshape(-1, 1)
            # Prob = np.append(Prob, np.array([X2S + Y2S, X1S + Y1S]), axis=0)
        else:
            list_of_arrays = [X1.T, X2.T]
            # Pos = np.append(Pos, np.array([X1.T, X2.T]), axis=0)
            Pos = np.vstack(list_of_arrays)
            Prob = np.array([X1S + Y1S, X2S + Y2S]).reshape(-1, 1)

        return Pos, Prob

    def calibrate_imu(self, imu_data, num_samples):
        # imu_data는 가속도계 및 자이로스코프 데이터의 2D 배열이다.
        # 각 행은 [ax, ay, az, gx, gy, gz] 형식이다.

        # 캘리브레이션에 사용할 샘플 수 선택
        data = np.array(imu_data[:num_samples])

        # 가속도계 및 자이로스코프 측정값의 평균 계산
        accel_bias = np.mean(data[:, :3], axis=0)
        gyro_bias = np.mean(data[:, 3:], axis=0)

        # 중력 벡터의 크기 (일반적으로 9.81 m/s^2)에서 z축 가속도의 바이어스를 차감
        # accel_bias[2] -= 9.81

        return accel_bias, gyro_bias

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
        # R = np.dot(R_z, np.dot(R_y, R_x))
        R = R_z @ R_y @ R_x

        return R

    def euler_acc(self, ax, ay, az):
        phi = math.atan(ay / az)
        theta = math.atan(ax / math.sqrt(ay**2 + az**2))

        return phi, theta

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

    def main(self):
        Ln = 4
        Lp = 4
        self.average_len = 10
        imu_data = []
        error_list = []
        while not rospy.is_shutdown():
            try:
                start_time = time.time()
                if self.statusIMU == True:
                    # print("IMU:", self.acc)
                    end_time_IMU = time.time()
                    # print(f"IMU eat:{end_time_IMU-start_time} second")
                    # if self.I % 10 == 0:
                    if (self.I > (self.average_len*2)):
                        # if self.I == 20:
                        self.accel_bias, self.gyro_bias = self.calibrate_imu(imu_data, self.average_len*2)
                        imu_data.append(np.array([self.IMU['lx'], self.IMU['ly'], self.IMU['lz'], self.IMU['gx'], self.IMU['gy'], self.IMU['gz']]))
                        imu_data.pop()
                        self.acc = np.array([self.IMU['lx']-self.accel_bias[0], self.IMU['ly']-self.accel_bias[1], self.IMU['lz']-self.accel_bias[2]])
                        self.ang = np.array([self.IMU['gx']-self.gyro_bias[0], self.IMU['gy']-self.gyro_bias[1], self.IMU['gz']-self.gyro_bias[2]])
                        # self.acc = np.array([self.IMU['lx'], self.IMU['ly'], self.IMU['lz']])
                        # self.ang = np.array([self.IMU['gx'], self.IMU['gy'], self.IMU['gz']])

                        self.rotation_vector = np.array([self.acc[0], self.acc[1], self.acc[2]]).reshape(-1, 1)
                        vel_t = self.rotation_vector_to_matrix(self.rotation_vector) @ self.vel
                        self.gyro = 1*np.array([[0, vel_t[2][0], -vel_t[1][0]], [-vel_t[2][0], 0, 0], [vel_t[2][0], 0, 0]]) @ np.array([self.ang[0], self.ang[1], self.ang[2]])
                        self.gyro_st = self.gravity * np.array([np.sin(self.acc[1]), -np.cos(self.acc[1]) * np.sin(self.acc[2]), -np.cos(self.acc[1]) * np.cos(self.acc[2])])
                        self.accel = vel_t.reshape(1, -1) @ self.acc - self.gyro - self.gyro_st
                        self.accel = self.accel.reshape(-1, 1)

                        self.gyro_gx, self.gyro_gy, self.gyro_gz = self.euler_gyro_update(self.ang[0], self.ang[1], self.ang[2], self.dt, self.gyro_gx, self.gyro_gy, self.gyro_gz)
                        self.acc_gx, self.acc_gy = self.euler_acc(self.acc[0], self.acc[1], self.acc[2])
                        self.qua_acc = self.euler_to_quaternion(self.acc_gx, self.acc_gy, self.gyro_gz)
                        self.qua_kf = self.euler_to_quaternion(self.gyro_gx, self.gyro_gy, self.gyro_gz)
                        # print(self.qua_acc)
                        # np.eye(4) + self.dt * 1/2 *
                        self.AA = np.eye(4) + (self.dt * 1/2 * np.array([[0, -self.ang[0], -self.ang[1], -self.ang[2]],
                                                    [self.ang[0],  0,  self.ang[2], -self.ang[1]],
                                                    [self.ang[1], -self.ang[2],  0,  self.ang[0]],
                                                    [self.ang[2],  self.ang[1], -self.ang[0],  0],
                                                    ]))

                        euler_kalman = IMUKalmanFilter(self.qua_kf, self.AA, self.qua_acc)
                        euler_kalman.predict()
                        euler_kalman.update()

                        self.gyro_gx, self.gyro_gy, self.gyro_gz = euler_kalman.result()

                        self.gyro_gx   = np.real(self.gyro_gx)
                        self.gyro_gy = np.real(self.gyro_gy)
                        self.gyro_gz   = np.real(self.gyro_gz)

                        self.gyro_gz_D = np.sqrt(self.gyro_gx**2 + self.gyro_gy**2 + self.gyro_gz**2)

                        self.accel[2] = self.accel[2] - self.gravity


                        # self.vel = self.vel + self.accel*self.dt
                        # print(self.accel)
                        self.a0 = np.sqrt(self.accel[0]**2 + self.accel[1]**2 + self.accel[2]**2)
                        self.v0 = np.sqrt(self.vel[0]**2 + self.vel[1]**2 + self.vel[2]**2)
                        self.v0 = self.v0 + self.a0 * self.dt
                        self.accel_bias = np.sqrt(self.accel_bias[0]**2 + self.accel_bias[1]**2 + self.accel_bias[2]**2)
                        self.gyro_bias = np.sqrt(self.gyro_bias[0]**2 + self.gyro_bias[1]**2 + self.gyro_bias[2]**2)

                        if self.statusUWB == True:
                            self.pos = self.UWBpos
                        # self.pos =self.pos.reshape(-1, 1) + self.v0[0] * self.dt + self.a0[0] * self.dt**2 /2
                        # print("pos", self.pos)
                        self.IHeading = self.headingest_a_aver[self.U-1]
                        self.IHeading = self.IHeading[0]
                        # print("eee", self.IHeading)

                        x_kM = np.array([[self.pos[0][0] + self.v0[0] * self.dt * np.cos(self.IHeading)],
                                         [self.pos[1][0] + self.v0[0] * self.dt * np.sin(self.IHeading)],
                                         [self.IHeading + self.gyro_gz_D * self.dt],
                                         [self.gyro_gz_D],
                                         [self.v0[0] + self.a0[0] * self.dt],
                                         [self.a0[0]]])

                        A = np.array([[1,0,-self.v0[0] * self.dt * np.sin(self.IHeading), 0, self.dt * np.cos(self.IHeading), 0],
                                      [0,1, self.v0[0] * self.dt * np.cos(self.IHeading), 0, self.dt * np.sin(self.IHeading), 0],
                                      [0,0,1, self.dt, 0, 0],
                                      [0,0,0,1,0,0],
                                      [0,0,0,0,0,self.dt],
                                      [0,0,0,0,0,1]])
                        # print("v0", self.v0[0])
                        # print("A", A)

                        z = np.array([[self.pos[0][0]],
                                      [self.pos[1][0]],
                                      [self.accel_bias],
                                      [self.gyro_bias]])

                        ekf = IMUExtendedKalmanFilter(x_kM, A, z)
                        ekf.predict()
                        result = ekf.update()
                        # print("result :", result)
                        # print("result :", result.shape)
                        self.pos[0][0] = result[0,0]
                        self.pos[1][0] = result[1,0]
                        self.IHeading  = result[2,0]
                        self.gyro_gz_D = result[3,0]
                        self.v0[0]       = result[4,0]
                        self.a0[0]     = result[5,0]

                        # print(self.pos)
                        # self.FH = (self.UH + self.KH + self.IH) /3
                        self.IMU_pos_est = self.tag_pos_b * np.exp(1j*(self.IHeading)) + self.pos[0][0] + 1j*self.pos[1][0]
                        self.IMU_pos_est = self.IMU_pos_est.reshape(1, -1)

                        Xt_e = np.real(self.IMU_pos_est)
                        Yt_e = np.imag(self.IMU_pos_est)
                        self.IXc = np.mean(Xt_e)
                        self.IYc = np.mean(Yt_e)
                        error = np.sqrt((self.IXc - self.pos_gt[0])**2 + (self.IYc - self.pos_gt[1])**2)


                        if self.I > 30:
                            e = np.mean(error_list)
                            error_list.append(error)
                            print("center point error :", error)
                            error_list.pop()
                            print("average error :", e)
                            print("center point amount of change :", error-e)
                        else:
                            error_list.append(error)

                    else:
                        imu_data.append(np.array([self.IMU['lx'], self.IMU['ly'], self.IMU['lz'], self.IMU['gx'], self.IMU['gy'], self.IMU['gz']]))
                        self.acc = np.array([self.IMU['lx'], self.IMU['ly'], self.IMU['lz']])
                        self.ang = np.array([self.IMU['gx'], self.IMU['gy'], self.IMU['gz']])

                        try:
                            self.pos = self.UWBpos
                        except:
                            pass

                        # acc_b_phi --> acc_gx
                        # acc_b_theta --> acc_w
                        self.acc_gx, self.acc_gy = self.euler_acc(self.acc[0], self.acc[1], self.acc[2])
                        self.gyro_gx  = self.gyro_gx * (self.I - 1) / self.I + self.acc_gx / self.I
                        self.gyro_gy = self.gyro_gy * (self.I - 1) / self.I + self.acc_gy / self.I
                        self.gyro_gz = np.pi/2
                        # print(self.I)
                    self.I += 1
                    self.statusIMU = False

                if self.statusUWB == True:
                    anch_pos = np.array([x + 1j*y for x, y in zip(self.UWB['x'], self.UWB['y'])])

                    if self.U == 0:
                        tag_pos_est, heading_est = self.GetUWBPos_v1(self.UWB['x'], self.UWB['y'], self.UWB['dist'], self.angles_from_heading)
                        # heading_est = self.IMU['ang']['z']
                    else:
                        tag_pos_est = self.K_tag_pos_est
                        heading_est = K_heading_est

                    Xt_e = np.real(tag_pos_est)
                    Yt_e = np.imag(tag_pos_est)
                    Xt_c_e = np.mean(Xt_e)
                    Yt_c_e = np.mean(Yt_e)

                    A = [[] for _ in range(Ln)]  # List of empty lists

                    QDD = 0
                    for qd in range(Ln):
                        for qc in range(qd+1, Ln):
                            if qd != qc:
                                for gg in range(Ln):
                                    if qd != gg and qc != gg:
                                        A[gg].append(QDD)
                                QDD += 1
                    # print(A)
                    KK = np.zeros((6,4))

                    Pos = [[] for _ in range(Lp)]  # 2D list
                    PosB = [[] for _ in range(Lp)]  # 2D list
                    Prob2T = [[] for _ in range(Lp)]  # 2D list
                    PosC = [[] for _ in range(Lp)]
                    QQ = [[] for _ in range(Lp)]

                    for PP in range(Lp):
                        Pos[PP] = np.array([]).reshape(-1, 2)
                        PosB[PP] = np.array([]).reshape(-1, 2)
                        Prob2T[PP] = np.array([]).reshape(-1, 2)
                        for NN in range(Ln):
                            for MM in range(NN+1, Ln):
                                if NN != MM:
                                    Pos2, Prob2 = self.TwoAnchPos4([self.UWB['x'][NN], self.UWB['x'][MM]], [self.UWB['y'][NN], self.UWB['y'][MM]], [self.UWB['dist'][NN, PP], self.UWB['dist'][MM, PP]], np.array([np.real(tag_pos_est[0][PP]), np.imag(tag_pos_est[0][PP])]), np.array([Xt_c_e, Yt_c_e]), anch_pos, self.UWB['dist'][:, PP])
                                    Pos[PP] = np.append(Pos[PP], np.array([Pos2[0, :]]), axis=0)
                                    PosB[PP] = np.append(PosB[PP], np.array([Pos2[1, :]]), axis=0)
                                    Prob2T[PP] = np.append(Prob2T[PP], np.array(Prob2.T), axis=0)

                        Temp = Prob2T[PP][:, 0].reshape(-1, 1) / Prob2T[PP][:, 1].reshape(-1, 1) * 2
                        ValM, IndM = np.sort(Temp, axis=0)[::-1], np.argsort(Temp, axis=0)[::-1]

                        TopI = 0
                        # while TopI < len(ValM) and ValM[TopI][0] > 1:
                        while (ValM[TopI] > 1) and (ValM[TopI] > (ValM[-1] * 1)):
                            if np.sum((PosB[PP][IndM[0]] - Pos[PP][IndM[-1]])**2) - np.sum((Pos[PP][IndM[0]] - Pos[PP][IndM[-1]])**2) < 0:
                                Pos[PP][IndM[0]] = PosB[PP][IndM[0]]
                            TopI += 1

                        PosC[PP] = np.array([np.array([x[0]+1j*x[1]]) for x in Pos[PP]])
                        if np.all(KK == 0):
                            KK = np.abs(PosC[PP] - anch_pos) - self.UWB['dist'][:, PP].T
                        else:
                            KK = KK + np.abs(PosC[PP] - anch_pos) - self.UWB['dist'][:, PP].T

                        QQ[PP] = np.abs(PosC[PP] - anch_pos) - self.UWB['dist'][:, PP].T
                    AB = np.zeros(Ln)
                    BB = np.zeros(Ln)


                    for CD in range(Ln):
                        TE = list(range(Ln))
                        TE.remove(CD)
                        AB[CD] = np.sum(np.abs(KK[np.ix_(A[CD], TE)]))
                        BB[CD] = -np.sum(KK[np.ix_(A[CD], [CD])])

                    CC = BB / AB
                    AB[BB < 0] += 10
                    Val, indi = np.sort(np.abs(AB)), np.argsort(np.abs(AB))
                    if indi[0] != 2:  # Python index starts from 0, so subtract 1 from original MATLAB index
                        dd = 1

                    AT = A[indi[0]]

                    biasV = np.zeros(Lp)

                    for ls in range(Lp):
                        biasV[ls] = np.mean(QQ[ls][AT, indi[0]])

                    dist_n = self.UWB['dist']
                    dist_n[indi[0], :] += biasV


                    # tag_center_pos_est, heading_est, Xt_c_e, Yt_c_e = self.GetUWBPosUpdate_v1(self.UWB['x'], self.UWB['y'], Xt_c_e, Yt_c_e, heading_est, self.rl, dist_n, self.angles_from_heading)
                    try:
                        tag_center_pos_est, heading_est, Xt_c_e, Yt_c_e = self.GetUWBPosUpdate_v1(self.UWB['x'], self.UWB['y'], Xt_c_e, Yt_c_e, self.IHeading, self.rl, dist_n, self.angles_from_heading)
                    except:
                        tag_center_pos_est, heading_est, Xt_c_e, Yt_c_e = self.GetUWBPosUpdate_v1(self.UWB['x'], self.UWB['y'], Xt_c_e, Yt_c_e, heading_est, self.rl, dist_n, self.angles_from_heading)
                    self.tag_pos_est = self.get_tag_pos(tag_center_pos_est, heading_est, self.tag_pos_b)

                    self.heading_est_a = np.append(self.heading_est_a, [heading_est], axis=0)
                    self.centerest_a = np.append(self.centerest_a, np.array([Xt_c_e, Yt_c_e]).T, axis=0)
                    acc = np.array([0, 0, 0])
                    ang = np.array([0, 0, 0])
                    if (self.U > (self.average_len*2)):
                        MeanA = np.mean(self.centerest_a[self.U-19:self.U-10, 0] + 1j*self.centerest_a[self.U-19:self.U-10, 1])
                        MeanB = np.mean(self.centerest_a[self.U-9:self.U, 0] + 1j*self.centerest_a[self.U-9:self.U, 1])
                        # self.centerest_a_aver = np.append(self.centerest_a_aver, np.array([np.real(MeanB + (MeanB-MeanA)/2), np.imag(MeanB + (MeanB-MeanA)/2)]).T, axis=0)
                        self.centerest_a_aver = np.append(self.centerest_a_aver, np.array([[np.real(MeanB + (MeanB-MeanA)/2), np.imag(MeanB + (MeanB-MeanA)/2)]]), axis=0)
                        MeanA_head = np.mean(self.heading_est_a[self.U-self.average_len*2+1:self.U-self.average_len])
                        MeanB_head = np.mean(self.heading_est_a[self.U-self.average_len+1:self.U])
                        self.headingest_a_aver = np.append(self.headingest_a_aver, np.array([[MeanB_head + (MeanB_head-MeanA_head)/2]]), axis=0)
                    else:
                        self.centerest_a_aver = np.append(self.centerest_a_aver, np.array([Xt_c_e, Yt_c_e]).T, axis=0)
                        self.headingest_a_aver = np.append(self.headingest_a_aver, [heading_est], axis=0)

                    # print(self.IMU['ang']['x']-self.gyro_bias[0],"  ", self.IMU['ang']['y']-self.gyro_bias[1],"  ", self.IMU['ang']['z']-self.gyro_bias[2])
                    tag_pos_est_aver = self.get_tag_pos(self.centerest_a_aver[self.U, 0] + 1j*self.centerest_a_aver[self.U, 1], self.headingest_a_aver[self.U], self.tag_pos_b)


                    K_Xt_c_e = self.centerest_a_aver[self.U, 0]
                    K_Yt_c_e = self.centerest_a_aver[self.U, 1]
                    self.UWBpos = np.array([K_Xt_c_e, K_Yt_c_e, 0]).reshape(-1, 1)

                    self.K_tag_pos_est = self.tag_pos_b * np.exp(1j*(self.headingest_a_aver[self.U])) + K_Xt_c_e + 1j*K_Yt_c_e
                    self.K_tag_pos_est = self.K_tag_pos_est.reshape(1, -1)
                    K_heading_est = self.headingest_a_aver[self.U]
                    self.KH = K_heading_est
                    self.UH = heading_est
                    self.K_centerest_a_aver = np.append(self.K_centerest_a_aver, [self.centerest_a_aver[self.U, :]], axis=0)
                    self.K_headingest_a_aver = np.append(self.K_headingest_a_aver, [self.headingest_a_aver[self.U]], axis=0)

                    end_time_UWB = time.time()
                    # print(f"UWB eat:{end_time_UWB-start_time} second")


                    self.U += 1

                    self.statusUWB = False
                else:
                    try:
                        self.UWB['dist'] = np.array([self.uwb0['dist'], self.uwb1['dist'], self.uwb2['dist'], self.uwb3['dist']])
                        self.UWB['dist'] = self.UWB['dist'].T

                        id = np.array(next((tag for tag in [self.uwb0['id'], self.uwb1['id'], self.uwb2['id'], self.uwb3['id']] if 'Nan' not in tag), None))
                        x = np.array(next((tag for tag in [self.uwb0['x'], self.uwb1['x'], self.uwb2['x'], self.uwb3['x']] if 'Nan' not in tag), None))
                        y = np.array(next((tag for tag in [self.uwb0['y'], self.uwb1['y'], self.uwb2['y'], self.uwb3['y']] if 'Nan' not in tag), None))
                        z = np.array(next((tag for tag in [self.uwb0['z'], self.uwb1['z'], self.uwb2['z'], self.uwb3['z']] if 'Nan' not in tag), None))

                        if x is not None and y is not None and z is not None:
                            self.UWB['id'] = id
                            self.UWB['x'] = x
                            self.UWB['y'] = y
                            self.UWB['z'] = z
                        else:
                            self.UWB['id'] = self.UWB['id']
                            self.UWB['x'] = self.UWB['x']
                            self.UWB['y'] = self.UWB['y']
                            self.UWB['z'] = self.UWB['z']
                    except KeyError:
                        self.statusUWB = False

                    else:
                        self.statusUWB = True


                    try:
                    # Figure 1
                        plt.figure(2)
                        plt.clf()  # Clear the figure
                        plt.axes().set_aspect('equal')
                        plt.text(self.UWB['x'][0], self.UWB['y'][0], self.UWB['id'][0], fontsize=12, color='red')
                        plt.text(self.UWB['x'][1], self.UWB['y'][1], self.UWB['id'][1], fontsize=12, color='red')
                        plt.text(self.UWB['x'][2], self.UWB['y'][2], self.UWB['id'][2], fontsize=12, color='red')
                        plt.text(self.UWB['x'][3], self.UWB['y'][3], self.UWB['id'][3], fontsize=12, color='red')
                        plt.plot(self.UWB['x'], self.UWB['y'], 'bo')

                        tag_e = np.array([[np.real(tag), np.imag(tag)] for tag in self.tag_pos_est]).T
                        K_tag_e = np.array([[np.real(K_tag), np.imag(K_tag)] for K_tag in self.K_tag_pos_est]).T
                        IMU_e = np.array([[np.real(IMU_e), np.imag(IMU_e)] for IMU_e in self.IMU_pos_est]).T

                        plt.plot(IMU_e[0][0], IMU_e[0][1], 'go')
                        plt.plot(IMU_e[1][0], IMU_e[1][1], 'g*')
                        plt.plot(IMU_e[2][0], IMU_e[2][1], 'gv')
                        plt.plot(IMU_e[3][0], IMU_e[3][1], 'g^')

                        plt.plot(tag_e[0][0], tag_e[0][1], 'bo')
                        plt.plot(tag_e[1][0], tag_e[1][1], 'b*')
                        plt.plot(tag_e[2][0], tag_e[2][1], 'bv')
                        plt.plot(tag_e[3][0], tag_e[3][1], 'b^')

                        plt.text(tag_e[0][0], tag_e[0][1], "1", fontsize=12, color='blue')
                        plt.text(tag_e[1][0], tag_e[1][1], "2", fontsize=12, color='blue')
                        plt.text(tag_e[2][0], tag_e[2][1], "3", fontsize=12, color='blue')
                        plt.text(tag_e[3][0], tag_e[3][1], "4", fontsize=12, color='blue')

                        plt.plot(K_tag_e[0][0], K_tag_e[0][1], 'ro')
                        plt.plot(K_tag_e[1][0], K_tag_e[1][1], 'r*')
                        plt.plot(K_tag_e[2][0], K_tag_e[2][1], 'rv')
                        plt.plot(K_tag_e[3][0], K_tag_e[3][1], 'r^')

                        # plt.quiver(x[r], y[r], np.cos(heading[r]), np.sin(heading[r]), color='g', scale=1, scale_units='xy', angles='xy')
                        plt.quiver(Xt_c_e, Yt_c_e, np.cos(heading_est+np.pi/2), np.sin(heading_est+np.pi/2), color='b', scale=1, scale_units='xy', angles='xy')
                        plt.quiver(K_Xt_c_e, K_Yt_c_e, np.cos(K_heading_est+np.pi/2), np.sin(K_heading_est+np.pi/2), color='r', scale=1, scale_units='xy', angles='xy')
                        plt.quiver(self.IXc, self.IYc, np.cos(self.IHeading+np.pi/2), np.sin(self.IHeading+np.pi/2), color='g', scale=1, scale_units='xy', angles='xy')
                        # print(self.IMU['ang']['z']-self.gyro_bias[2])

                        plt.title('Original(Blue) and estimated(Red) Tag to Anchor distance')

                        # Figure 4
                        if (self.U > (self.average_len*2)):
                            plt.figure(3)
                            plt.clf()  # Clear the figure
                            plt.axes().set_aspect('equal')
                            plt.plot(self.UWB['x'], self.UWB['y'], 'go')
                            # plt.plot(self.centerest_a_aver[:,0], self.centerest_a_aver[:,1], 'b^')
                            # plt.plot(x[:], y[:], 'g^')
                            # plt.plot(self.K_centerest_a_aver[:,0], self.K_centerest_a_aver[:,1], 'ro')
                            plt.plot(K_Xt_c_e, K_Yt_c_e, 'ro')
                            # plt.plot(self.centerest_a[:,0], self.centerest_a[:,1], 'ko')
                    except:
                        pass
                    plt.pause(0.01)
                end_time = time.time()
                print(f"Processing Time :{end_time-start_time}")
            except KeyError:
                pass
        plt.show()




if __name__ == '__main__':
    fusion = Sensor_fusion()
    fusion.main()
    rospy.spin()