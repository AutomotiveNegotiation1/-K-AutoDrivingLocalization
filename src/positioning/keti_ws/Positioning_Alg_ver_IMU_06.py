#!/usr/bin/env python3.8
# from ctypes.wintypes import PPOINTL
from stringprep import c6_set
import rospy
import numpy as np
import matplotlib.pyplot as plt
import cmath
import math
import time

from sensor_msgs.msg import Imu
from geometry_msgs.msg import Quaternion
from localizer_dwm1001.msg import Anchor
from localizer_dwm1001.msg import Tag

import numpy as np
from numpy.linalg import inv

from utils import *


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
    
class UWBKalmanFilter(object):
    def __init__(self, x, A, z, R):
        self.threshold = 100
        self.x = x
        self.P = 1 * np.eye(12)
        self.P[0:3,3:6] = 1 * np.eye(3)
        self.P[0:6,6:12] = 10 * np.ones((6,6))
        self.P[9:12, 9:12] = 100 * np.eye(3)
        self.A = A
        self.H = np.zeros((6,12))
        self.H[0:6, 0:6] = np.eye(6)
        self.R = R
        self.Q = 1e-4 * np.eye(12)
        self.I = None
        self.S = None
        self.K = None
        self.z = z


    def predict(self):
        
        self.xp = np.dot(self.A, self.x)
        # print("self.xp", self.xp.shape)
        # print("self.xp", self.xp)
        self.Pp = np.dot(np.dot(self.A, self.P), self.A.T) + self.Q
        # print("self.Pp", self.Pp.shape)
        # print("self.Pp", self.Pp)
        self.H = self.H.astype(np.float64)
        self.R = self.R.astype(np.float64)
        self.Pp = self.Pp.astype(np.float64)

    def update(self):
        self.K = np.dot(np.dot(self.Pp, self.H.T), inv(np.dot(np.dot(self.H, self.Pp), self.H.T) + self.R))
        self.x = self.xp + np.dot(self.K, (self.z - np.dot(self.H, self.xp)))
        self.P = self.Pp - np.dot(np.dot(self.K, self.H), self.Pp)
        
        return self.x
        
        
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
        
        self.x = dict()
        self.y = dict()
        self.z = dict()
        
        self.IMU = dict()
        self.acc = dict()
        self.ang = dict()
        
        self.xt_b = np.array([-0.09, 0.09, -0.09, 0.09])
        self.yt_b = np.array([0.12, 0.12, -0.12, -0.12])
        
        
        self.statusIMU = False
        self.statusUWB = False
        
        # self.accel_bias = np.array([0, 0, 0])
        # self.gyro_bias = np.array([0, 0, 0])
        
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
        
        self.average_len = 10
        
        self.gyro_bias    = np.array([-0.2927, 0.1928, -0.0194])  # 초기값
        
        self.accel = np.array([[0],[0],[0]])
        # self.vel = np.zeros((1,3))
        self.velocity = np.array([0, 0, 0])
        self.pos = np.zeros((3,1))
        self.orientation = None
        
        self.UWB_vel = np.array([0,0,0])
        self.UWB_pos = np.array([0,0,0]).reshape(-1,1)
        
        self.xt_b_center = np.mean(self.xt_b)
        self.yt_b_center = np.mean(self.yt_b)
        self.angles_from_heading = np.arctan2(self.yt_b, self.xt_b)
        self.rl = np.sqrt(self.xt_b**2+self.yt_b**2)
        self.tag_pos_b = self.xt_b + 1j*self.yt_b
        self.I = 1
        self.U = 1
        self.r = 1
        self.Lp = 4
        self.Ln = 4
        
        # self.NumInterpPoint = 4
        
        
        self.TagDistInit = np.zeros((self.Ln, self.Lp))
        
        
        self.heading_est_a = np.array([]).reshape(-1 ,1)
        self.centerest_a = np.array([]).reshape(-1, 2)
        
        self.centerest_a_aver = None
        self.headingest_a_aver = None
        
        self.K_centerest_a_aver = np.array([]).reshape(-1, 2)
        self.K_headingest_a_aver = np.array([]).reshape(-1 ,1)
        
        self.flag = None
        
        
        
        self.NumRxID = []
        self.TagDistInitCount = np.zeros((self.Ln, self.Lp))
        self.TagDistInit = np.zeros((self.Ln,self.Lp))
        
        self.NumInterpPoint = 9
        self.Tag_Pos_List = np.zeros((self.NumInterpPoint, 2, self.Lp), dtype=complex)
        self.InterpPosition = np.zeros((self.Lp, 2))
        
        # ROS 노드 구독
        rospy.Subscriber("/zed_f9r/imu", Imu, self.ImuCallback)
        # rospy.Subscriber("/dwm1001/anchor/ttyUWB0", Anchor, self.Anchorcallback0)
        # rospy.Subscriber("/dwm1001/anchor/ttyUWB1", Anchor, self.Anchorcallback1)
        # rospy.Subscriber("/dwm1001/anchor/ttyUWB2", Anchor, self.Anchorcallback2)
        # rospy.Subscriber("/dwm1001/anchor/ttyUWB3", Anchor, self.Anchorcallback3)
        # rospy.Subscriber("/dwm1001/ttyUWB0", Tag, s3elf.TagCallback)
        # rospy.Subscriber("/dwm1001/ttyUWB1", Tag, self.TagCallback)
        # rospy.Subscriber("/dwm1001/ttyUWB2", Tag, self.TagCallback)
        # rospy.Subscriber("/dwm1001/ttyUWB3", Tag, self.TagCallback)
        
        
    def create_id_order(self, ids):
        if self.id_order is None:
            self.id_order = ids
        
        if self.id_order is not None:
            # Check if ids is not equal to self.id_order
            if set(ids) != set(self.id_order):
                # if len(self.id_order) < 4:
                # ids에서 self.id_order에 없는 요소를 찾습니다.
                if len(self.id_order) == 4:
                    pass
                
                else:
                    difference = [item for item in ids if item not in self.id_order]

                    # 이 요소들을 self.id_order에 추가합니다.
                    self.id_order.extend(difference)
                    
                    
                    self.id_order_check = True
                
                
    def uwb_sort(self, num, msg, uwb):
        x = []
        y = []
        z = []
        dist = []
        if len(msg.id) >= 3:
            self.create_id_order(msg.id)
            if self.statusUWB == False and self.id_order_check == True:
                uwb['id'] = self.id_order
                uwb['tag_id'] = num
                fd = 0
                try:
                    for i, id in enumerate(self.id_order):
                        try:
                            order_indices = msg.id.index(id)
                            x.append(msg.x[order_indices])
                            y.append(msg.y[order_indices])
                            z.append(msg.z[order_indices])
                            dist.append(msg.distanceFromTag[order_indices])
                            fd += msg.x[order_indices]
                        except ValueError:

                            order_indices = i
                            x.append(self.x[id])
                            y.append(self.y[id])
                            z.append(self.z[id])
                            dist.append(0)
                    
                    uwb['x'] = x
                    uwb['y'] = y
                    uwb['z'] = z
                    uwb['dist'] = dist
                except TypeError:
                    pass
        
        
        
        # self.create_id_order(msg.id)
        # # if self.id_order is not None and len(self.id_order) == 4:
        # if self.id_order is not None and len(self.id_order) == 4:
        #     x = []
        #     y = []
        #     z = []
        #     dist = []
        #     if len(msg.id) >= 3:
        #         uwb['id'] = self.id_order
        #         uwb['tag_id'] = num
        #         print(self.id_order)
        #         for i, id in enumerate(self.id_order):    
        #             try:
        #                 order_indices = msg.id.index(id)                        
        #                 x.append(msg.x[order_indices])
        #                 y.append(msg.y[order_indices])
        #                 z.append(msg.z[order_indices])
        #                 dist.append(msg.distanceFromTag[order_indices])

        #             except ValueError:
        #                 print(num, id)
        #                 # print(f'ValueError: "{id}" is not in list')
        #                 if len(msg.id) != 4:
        #                     order_indices = i
        #                     x.append(self.x[id])
        #                     y.append(self.y[id])
        #                     z.append(self.z[id])
        #                     dist.append(25)
                        
        #         uwb['x'] = x
        #         uwb['y'] = y
        #         uwb['z'] = z
        #         uwb['dist'] = dist
                
        #         print(uwb['x'])
        #         print(uwb['y'])
        #         print(uwb['z'])
        #         print(uwb['dist'])
                
                    
            # order_indices = [msg.id.index(id) for id in self.id_order]
            # print(order_indices)

            # # 이제 order_indices를 사용해서 msg의 나머지 속성을 재정렬하거나 'Nan'을 할당합니다:
            # uwb['x'] = [msg.x[i] for i in order_indices]
            # uwb['y'] = [msg.y[i] for i in order_indices]
            # uwb['z'] = [msg.z[i] for i in order_indices]
            # uwb['dist'] = [msg.distanceFromTag[i] for i in order_indices]
        # else:
        #     pass
        
    def uwb_init(self):
        try:
            self.UWB['dist'] = np.array([self.uwb0['dist'], self.uwb1['dist'], self.uwb2['dist'], self.uwb3['dist']])
            self.UWB['id'] = np.array(self.uwb0['id'])
            self.UWB['x'] = np.array(self.uwb0['x'])
            self.UWB['y'] = np.array(self.uwb0['y'])
            self.UWB['z'] = np.array(self.uwb0['z'])
            
            self.UWB['dist'] = self.UWB['dist'].T

        except:
            self.statusUWB = False
            
        else:
            self.id_order_check = False
            self.id_order = None
            self.statusUWB = True

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
        for i, id in enumerate(msg.id):
            self.x[id] = msg.x[i]
            self.y[id] = msg.y[i]
            self.z[id] = msg.z[i]
        self.uwb_sort(1, msg, self.uwb1)

    def Anchorcallback2(self, msg):
        for i, id in enumerate(msg.id):
            self.x[id] = msg.x[i]
            self.y[id] = msg.y[i]
            self.z[id] = msg.z[i]
        self.uwb_sort(2, msg, self.uwb2)
    
    def Anchorcallback3(self, msg):
        for i, id in enumerate(msg.id):
            self.x[id] = msg.x[i]
            self.y[id] = msg.y[i]
            self.z[id] = msg.z[i]
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
        accel_bias[2] += 9.81

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
    
    def IMU_Positioning(self):
        if (self.I > (self.average_len*2)):
            
            # accel_bias, gyro_bias = self.calibrate_imu(imu_data, self.average_len*2)
            # imu_data.append(np.array([self.IMU['lx'], self.IMU['ly'], self.IMU['lz'], self.IMU['gx'], self.IMU['gy'], self.IMU['gz']]))
            # imu_data.pop()
            # self.acc = np.array([self.IMU['lx']-self.accel_bias[0], self.IMU['ly']-self.accel_bias[1], self.IMU['lz']-self.accel_bias[2]])
            # self.ang = np.array([self.IMU['gx']-self.gyro_bias[0], self.IMU['gy']-self.gyro_bias[1], self.IMU['gz']-self.gyro_bias[2]])
            self.acc = np.array([self.IMU['lx'], self.IMU['ly'], self.IMU['lz']])
            self.ang = np.array([self.IMU['gx'], self.IMU['gy'], self.IMU['gz']])
            
            self.rotation_vector = np.array([self.acc[0], self.acc[1], self.acc[2]]).reshape(-1, 1)
            vel_t = self.rotation_vector_to_matrix(self.rotation_vector) @ self.velocity
            gyro = 1*np.array([[0, vel_t[2], -vel_t[1]], [-vel_t[2], 0, 0], [vel_t[2], 0, 0]]) @ np.array([self.ang[0], self.ang[1], self.ang[2]])
            self.gyro_st = self.gravity * np.array([np.sin(self.acc[1]), -np.cos(self.acc[1]) * np.sin(self.acc[2]), -np.cos(self.acc[1]) * np.cos(self.acc[2])])
            # print("gyro_st :", self.acc - gyro -self.gyro_st)
            self.accel = vel_t.reshape(1, -1) @ self.acc - gyro -self.gyro_st
            self.accel = self.accel.reshape(1, -1)
            gyro_gx, gyro_gy, gyro_gz = self.euler_gyro_update(self.ang[0]-self.gyro_bias[0], self.ang[1]-self.gyro_bias[1], self.ang[2]-self.gyro_bias[2], self.dt, self.ang[0], self.ang[1], self.ang[2])
            acc_gx, acc_gy = self.euler_acc(self.ang[0]-self.gyro_bias[0], self.ang[1]-self.gyro_bias[1], self.ang[2]-self.gyro_bias[2])
            qua_acc = self.euler_to_quaternion(acc_gx, acc_gy, self.UWB_Heading)
            qua_kf = self.euler_to_quaternion(gyro_gx, gyro_gy, gyro_gz)
            # print(self.qua_acc)
            # np.eye(4) + self.dt * 1/2 *
            # print("sensor acc :", self.acc)
            # print("accelation :",self.accel)
            # print("velocity :", self.velocity)
            A = np.array([])
            
            AA = np.eye(4) + (self.dt * 1/2 * np.array([[0, -gyro_gx, -gyro_gy, -gyro_gz],
                                        [gyro_gx,  0,  gyro_gz, -gyro_gy],
                                        [gyro_gy, -gyro_gz,  0,  gyro_gx],
                                        [gyro_gz,  gyro_gy, -gyro_gx,  0],
                                        ]))
            
            euler_kalman = IMUKalmanFilter(qua_kf, AA, qua_acc)
            euler_kalman.predict()
            euler_kalman.update()
            
            gyro_gx, gyro_gy, gyro_gz = euler_kalman.result()
            
            gyro_gx   = np.real(gyro_gx)
            gyro_gy   = np.real(gyro_gy)
            gyro_gz   = np.real(gyro_gz)
            
            gyro_gz_D = np.sqrt(gyro_gx**2 + gyro_gy**2 + gyro_gz**2)
            
            if (self.I == self.average_len*2+1):
                ax, ay, az = [sum(coord[i] for coord in self.pos) / len(self.pos) for i in range(3)]
                self.IMU_Position = np.array([ax, ay, az])
            
            # self.IMU_Position = self.UWB_Position + self.velocity * self.dt
            self.velocity = self.velocity + self.accel  * self.dt
            print(self.velocity)
            self.velocity = np.array([self.velocity[0][0], self.velocity[0][1], self.velocity[0][2]])
            self.IMU_Position = self.UWB_Position + self.velocity * self.dt + self.accel * self.dt**2 / 2
            self.IMU_Position = np.array([self.IMU_Position[0][0], self.IMU_Position[0][1], self.IMU_Position[0][2]])
            self.IMU_Heading = gyro_gz + self.UWB_Heading
            
            
    
        else:              
            # imu_data.append(np.array([self.IMU['lx'], self.IMU['ly'], self.IMU['lz'], self.IMU['gx'], self.IMU['gy'], self.IMU['gz']]))
            self.acc = np.array([self.IMU['lx'], self.IMU['ly'], self.IMU['lz']])
            self.ang = np.array([self.IMU['gx'], self.IMU['gy'], self.IMU['gz']])
            
            self.acc_gx, self.acc_gy = self.euler_acc(self.acc[0], self.acc[1], self.acc[2])
            self.gyro_gx  = self.gyro_gx * (self.I - 1) / self.I + self.acc_gx / self.I
            self.gyro_gy = self.gyro_gy * (self.I - 1) / self.I + self.acc_gy / self.I
            self.gyro_gz = np.pi/2

            self.pos.append(self.UWB_Position)
            
            self.IMU_Position = self.UWB_Position
            # self.IMU_Heading = 0
            
            # Position = self.pos / self.I
            
        self.I += 1
        self.statusIMU = False
        
        # return Position, Heading
    
    def UWB_Positioning(self):

        # dist = np.array([[11.2340, 10.1285, 10.9581, 9.5884], [10.4505, 11.0299, 9.7431, 10.7766]])
        dist = np.array([[11.0983, 10.1610, 10.7430, 9.8274], [10.1465, 11.0458, 9.8153, 10.7711],
                         [19.0115, 19.5951, 19.9074, 20.3512], [19.5780, 19.0296, 20.4718, 19.9609]])
        
        xa = np.array([10, 10, -10, -10, 20, -20])
        ya = np.array([10, -10, -10, 10, 15, -15])
        anch_pos = np.array([xa + 1j*ya])

        xt_b = np.array([-0.5, 0.5, -0.5, 0.5])
        yt_b = np.array([0.5, 0.5, -0.5, -0.5])
        tag_pos_b = np.array([xt_b + 1j*yt_b])
        
        x = [_ for _ in range(730)]
        s_time = np.arange(0, len(x)) / 10.0

        
        # tag_pos_est=None
        heading_est=None
        CandPos = None
        tag_center_pos_est = None
        heading_est_a = None
        dg = None
        
        while not rospy.is_shutdown():
            Nanchor = min(np.random.randint(1, 9) + 1, 4)
            RxID = np.array(np.random.choice(range(1, self.Ln+1), Nanchor, replace=False)-1)
            # print("RxID",RxID)
            
            # MATLAB의 randi(Lp)는 1부터 Lp까지의 수 중 하나를 무작위로 선택합니다.
            RxTag = np.random.randint(0, self.Lp) 
            PP = RxTag
            # print("Nanchor", Nanchor)
            # print("RxID", RxID)
            # print("PP", PP)
            
            # Nanchor = 4
            # RxID = [1, 2, 0, 3]
            # PP = 1
            
            RxDist = np.array(dist[RxID,PP])
            # print("RxDist", np.array(RxDist).reshape(-1, 1))
            # print("RxDist", np.array(RxDist).reshape(-1, 1).shape)
            self.NumRxID.append(Nanchor)
            
            print("RxDist", RxDist)
            
            if self.r < 10*self.Lp:   
                for NN in range(Nanchor):
                    self.TagDistInitCount[RxID[NN], PP] = self.TagDistInitCount[RxID[NN],PP] + 1
                    div = self.TagDistInitCount[RxID[NN], PP]
                    self.TagDistInit[RxID[NN],PP] = self.TagDistInit[RxID[NN],PP]*(div-1)/div+RxDist[NN]/div
                    print(self.TagDistInit)
                
                # self.centerest_a = np.append(self.centerest_a, [[None,None]], axis=0)
                # self.heading_est_a = np.append(self.heading_est_a, [[None]], axis=0)
                
            elif self.r == 10*self.Lp:
                for NN in range(Nanchor):
                    self.TagDistInitCount[RxID[NN], PP] = self.TagDistInitCount[RxID[NN],PP] + 1
                    div = self.TagDistInitCount[RxID[NN], PP]
                    self.TagDistInit[RxID[NN],PP] = self.TagDistInit[RxID[NN],PP]*(div-1)/div+RxDist[NN]/div
                    
                # self.centerest_a = np.append(self.centerest_a, [[None,None]], axis=0)
                # self.heading_est_a = np.append(self.heading_est_a, [[None]], axis=0)
                
                DistT = self.TagDistInit
                DistT[self.TagDistInit == 0] = 1000000
                IndT = np.where(DistT.sum(axis=1) < 100000)[0]
                DistT = DistT[IndT, :]
                AnchID = IndT
                # print("DistT", DistT)
                # print("DistT2", DistT)
                # print("IndT", IndT)
                # print("DistT", DistT)
                # print("AnchID", AnchID)
                
                tag_pos_est, heading_est =  GetInitPos(xa,ya,dist,anch_pos,tag_pos_b,self.Ln,self.Lp)
                # print("tag_pos_est, heading_est", tag_pos_est, heading_est)
                
                s_time_prev = 0
                for LLp in range(1, self.Lp + 1):
                    for lo in range(1, self.NumInterpPoint):
                        self.Tag_Pos_List[lo - 1, :, LLp - 1] = [s_time[self.r - self.NumInterpPoint+lo], tag_pos_est[0][LLp - 1]]
                        
            else:
                #######################################################################################################################################################################
                ################################################## Next position Prediction  ##########################################################################################
                for PPC in range(1, self.Lp + 1):
                    # Calculate the interpolated position for real part
                    self.InterpPosition[PPC - 1, 0] = InterpPos(np.real(self.Tag_Pos_List[:, 0, PPC - 1].reshape(1, -1)), np.real(self.Tag_Pos_List[:, 1, PPC - 1]), s_time[self.U - 1])
                    # print(self.InterpPosition[PPC - 1, 0])
                    # Calculate the interpolated position for imaginary part
                    self.InterpPosition[PPC - 1, 1] = InterpPos(np.real(self.Tag_Pos_List[:, 0, PPC - 1].reshape(1, -1)), np.imag(self.Tag_Pos_List[:, 1, PPC - 1]), s_time[self.U - 1])
                    # print(self.InterpPosition[PPC - 1, 1])
                #######################################################################################################################################################################
                                
                Xt_c_e = np.mean(self.InterpPosition[:, 0])
                Yt_c_e = np.mean(self.InterpPosition[:, 1])
                # print("eeeeeeeeeeeee", Xt_c_e, Yt_c_e)
                if np.isnan(Xt_c_e) or np.isnan(Yt_c_e):
                    dg = 1
                PosC = []
                PosC_E = []
                
                #######################################################################################################################################################################
                ################################################## Next position Calc  ################################################################################################
                
                if len(RxID) > 1:
                    tag_pos_est, heading_est, CandPos = GetPos2(xa, ya, RxDist, RxID, tag_pos_b[0], Nanchor, PP, self.InterpPosition[:, 0] + 1j * self.InterpPosition[:, 1])
                    # print("result", tag_pos_est, heading_est, CanPos)
                
                else:
                    CandPos = self.InterpPosition[PP, 0] + 1j * self.InterpPosition[PP, 1]
                    
                self.Tag_Pos_List[:self.NumInterpPoint-1,:,PP] = self.Tag_Pos_List[1:self.NumInterpPoint,:,PP]
                self.Tag_Pos_List[self.NumInterpPoint-1,:,PP] = [s_time[self.U], CandPos]
                
                Xt_c_e = np.real(np.mean(tag_pos_est))
                Yt_c_e = np.imag(np.mean(tag_pos_est))
                
                # print(self.U)
                # print(Xt_c_e, Yt_c_e)
                
                tag_center_pos_est = np.mean(tag_pos_est)
            
                # print(self.U)
                # print(self.heading_est_a[39])
                if self.heading_est_a.size != 0:
                    if (heading_est - self.heading_est_a[self.U-2]) > np.pi:
                        self.heading_est_a = np.append(self.heading_est_a, [[heading_est - 2 * np.pi]], axis=0)
                    elif (self.heading_est_a[self.U-2] - heading_est) > np.pi:
                        self.heading_est_a = np.append(self.heading_est_a, [[heading_est + 2 * np.pi]], axis=0)
                    else:
                        self.heading_est_a = np.append(self.heading_est_a, [[heading_est]], axis=0)
                else:
                    self.heading_est_a = np.append(self.heading_est_a, [[heading_est]], axis=0)
                
                self.centerest_a = np.append(self.centerest_a, np.array([[Xt_c_e, Yt_c_e]]), axis=0)
                # print("H", self.heading_est_a)
                # print("C", self.centerest_a)
                
                if (self.U > (self.average_len*2)):
                    MeanA = np.mean(self.centerest_a[self.U-19:self.U-10, 0] + 1j*self.centerest_a[self.U-19:self.U-10, 1])
                    MeanB = np.mean(self.centerest_a[self.U-9:self.U, 0] + 1j*self.centerest_a[self.U-9:self.U, 1])
                    self.centerest_a_aver = np.array([[np.real(MeanB + (MeanB-MeanA)/2), np.imag(MeanB + (MeanB-MeanA)/2)]]).T
                    MeanA_head = np.mean(self.heading_est_a[self.U-self.average_len*2+1:self.U-self.average_len])
                    MeanB_head = np.mean(self.heading_est_a[self.U-self.average_len+1:self.U])
                    self.headingest_a_aver = np.mod(MeanB_head + (MeanB_head - MeanA_head) / 2, 2 * np.pi)
                else:
                    self.centerest_a_aver = np.array([[Xt_c_e, Yt_c_e]]).T
                    self.headingest_a_aver = heading_est
                    
                
                # print("C_aver", self.centerest_a_aver)
                tag_pos_est_aver = get_tag_pos(self.centerest_a_aver[0] + 1j * self.centerest_a_aver[1], self.headingest_a_aver, tag_pos_b)
                K_Xt_c_e = self.centerest_a_aver[0]
                K_Yt_c_e = self.centerest_a_aver[1]
                tag_pos_est = tag_pos_b * np.exp(1j * (self.headingest_a_aver)) + K_Xt_c_e + 1j * K_Yt_c_e
                
                # print(tag_pos_est)

                # plt.figure(1)
                # plt.clf()  # Clear the figure
                # plt.axes().set_aspect('equal')
                # plt.plot(xa, ya, 'bo')

                # plt.plot(tag_pos_est[0][0], 'ro')
                # plt.plot(tag_pos_est[0][1], 'r*')
                # plt.plot(tag_pos_est[0][2], 'rv')
                # plt.plot(tag_pos_est[0][3], 'r^')

                # plt.quiver(Xt_c_e, Yt_c_e, np.cos(self.headingest_a_aver), np.sin(self.headingest_a_aver), color='b', linewidth=1, headwidth=6)
                self.U += 1
                plt.pause(0.001)
        
            self.r += 1
        plt.show()
        # return Position, Heading
    
    def main(self):
        while not rospy.is_shutdown():
            self.UWB_Positioning()
        
        
    
    
if __name__ == '__main__':
    fusion = Sensor_fusion()
    fusion.main()
    rospy.spin()