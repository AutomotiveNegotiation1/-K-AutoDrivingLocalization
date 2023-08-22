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

from ctypes.wintypes import PPOINTL
import rospy
import numpy as np
import matplotlib.pyplot as plt
import cmath

from sensor_msgs.msg import Imu
from localizer_dwm1001.msg import Anchor
from localizer_dwm1001.msg import Tag

class Sensor_fusion:
    def __init__(self):
        # ROS 노드 초기화
        rospy.init_node("sensor_fusion_node")

        # self.id_order = ['991B', '439D', '9B8F', '89A4'] # The desired order of uwb id numbers
        self.id_order = None
        self.id_order_check = False

        self.UWB = dict()
        self.uwb0 = dict()
        self.uwb1 = dict()
        self.uwb2 = dict()
        self.uwb3 = dict()

        # self.xt_b = np.array([0.05, 0.05, -0.05, -0.05])
        # self.yt_b = np.array([0.05, -0.05, 0.05, -0.05])
        self.xt_b = np.array([-0.09, 0.09, -0.09, 0.09])
        self.yt_b = np.array([0.12, 0.12, -0.12, -0.12])


        self.xt_b_center = np.mean(self.xt_b);
        self.yt_b_center = np.mean(self.yt_b);
        self.angles_from_heading = np.arctan2(self.yt_b, self.xt_b);
        self.rl = np.sqrt(self.xt_b**2+self.yt_b**2)
        self.tag_pos_b = self.xt_b + 1j*self.yt_b;
        self.r = 0

        self.heading_est_a = np.array([]).reshape(-1 ,1)
        self.centerest_a = np.array([]).reshape(-1, 2)
        self.centerest_a_aver = np.array([]).reshape(-1, 2)

        self.headingest_a_aver = np.array([]).reshape(-1 ,1)
        self.K_centerest_a_aver = np.array([]).reshape(-1, 2)
        self.K_headingest_a_aver = np.array([]).reshape(-1 ,1)

        # ROS 노드 구독
        # rospy.Subscriber("/zed_f9r/imu", Imu, self.ImuCallback)
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

    # def uwb_sort(self, num, msg, uwb):
    #     self.create_id_order(msg.id)
    #     uwb['tag_id'] = num
    #     uwb['id'] = [id if id in msg.id else np.nan for id in self.id_order]

    #     order_indices = [msg.id.index(id) if id in msg.id else np.nan for id in self.id_order]

    #     # 이제 order_indices를 사용해서 msg의 나머지 속성을 재정렬하거나 'Nan'을 할당합니다:
    #     uwb['x'] = [msg.x[i] if not np.isnan(i) else np.nan for i in order_indices]
    #     uwb['y'] = [msg.y[i] if not np.isnan(i) else np.nan for i in order_indices]
    #     uwb['z'] = [msg.z[i] if not np.isnan(i) else np.nan for i in order_indices]
    #     uwb['dist'] = [msg.distanceFromTag[i] if not np.isnan(i) else np.nan for i in order_indices]

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


        # print(self.UWB['x'])
        # print(self.UWB['y'])
        # print(self.UWB['z'])

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
        # print("GUPv1-A :", A)
        # print("GUPv1-Y :", Y)
        # Calc. Tag position
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
        # print("A :", A)
        # print("A.shape :", A.shape)
        Y1 = np.array([[d * np.sqrt((Ya[1] - Ya[0])**2 + (Xa[1] - Xa[0])**2) + Xa[0] * Ya[1] - Xa[1] * Ya[0], -(C**2 - B**2 - Xa[0]**2 + Xa[1]**2 - Ya[0]**2 + Ya[1]**2)]]).T
        # print("Y1 :", Y1)
        # print("Y1.shape :", Y1.shape)
        Y2 = np.array([[-d * np.sqrt((Ya[1] - Ya[0])**2 + (Xa[1] - Xa[0])**2) + Xa[0] * Ya[1] - Xa[1] * Ya[0], -(C**2 - B**2 - Xa[0]**2 + Xa[1]**2 - Ya[0]**2 + Ya[1]**2)]]).T
        # print("Y2 :", Y2)
        # print("Y2.shape :", Y2.shape)

        X1 = np.linalg.inv(A.T @ A) @ A.T @ Y1
        # print("X1 :", X1)
        # print("X1.shape :", X1.shape)
        X2 = np.linalg.inv(A.T @ A) @ A.T @ Y2
        # print("X2 :", X2)
        # print("X2.shape :", X2.shape)

        X1S = np.sum(np.abs(np.abs(anch_pos - (X1[0] + 1j * X1[1])) - dist_a.T))
        X2S = np.sum(np.abs(np.abs(anch_pos - (X2[0] + 1j * X2[1])) - dist_a.T))
        Y1S = np.sum((X1 - tag_pos.T)**2)
        Y2S = np.sum((X2 - tag_pos.T)**2)
        Z1S = np.abs(np.sum((X1 - EstCenter.T)**2) - 0.5)
        Z2S = np.abs(np.sum((X2 - EstCenter.T)**2) - 0.5)

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
            # Prob = np.append(Prob, np.array([X1S + Y1S, X2S + Y2S]), axis=0)

        # Pos = Pos.squeeze()

        # Pos = np.unique(Pos, axis=0)

        return Pos, Prob

    def TwoAnchPos4(self, Xa, Ya, dist, tag_pos, EstCenter, anch_pos, dist_a):
        AA = np.sqrt((Xa[0] - Xa[1])**2 + (Ya[0] - Ya[1])**2)
        B = dist[0]
        C = dist[1]

        s1 = (AA+B+C)/2
        S2 = np.real(cmath.sqrt(s1*(s1-AA)*(s1-B)*(s1-C)))
        d = 2*S2/AA

        A = np.array([[Ya[1] - Ya[0], -(Xa[1] - Xa[0])], [2 * (Xa[1] - Xa[0]), 2 * (Ya[1] - Ya[0])]])
        # print("A.shape :", A.shape)
        Y1 = np.array([[d * np.sqrt((Ya[1] - Ya[0])**2 + (Xa[1] - Xa[0])**2) + Xa[0] * Ya[1] - Xa[1] * Ya[0], (-C**2 + B**2 - Xa[0]**2 + Xa[1]**2 - Ya[0]**2 + Ya[1]**2)]]).T
        # print("Y1 :", Y1)
        # print("Y1.shape :", Y1.shape)
        Y2 = np.array([[-d * np.sqrt((Ya[1] - Ya[0])**2 + (Xa[1] - Xa[0])**2) + Xa[0] * Ya[1] - Xa[1] * Ya[0], (-C**2 + B**2 - Xa[0]**2 + Xa[1]**2 - Ya[0]**2 + Ya[1]**2)]]).T
        # print("Y2 :", Y2)
        # print("Y2 :", Y2)
        # print("Y2.shape :", Y2.shape)

        X1 = np.linalg.inv(A.T @ A) @ A.T @ Y1
        # print("X1 :", X1)
        # print("X1.shape :", X1.shape)
        X2 = np.linalg.inv(A.T @ A) @ A.T @ Y2
        # print("X2 :", X2)
        # print("X2.shape :", X2.shape)

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
            # Prob = np.append(Prob, np.array([X1S + Y1S, X2S + Y2S]), axis=0)

        # Pos = Pos.squeeze()

        # Pos = np.unique(Pos, axis=0)

        return Pos, Prob


    def main(self):
        Ln = 4
        Lp = 4
        self.average_len = 10

        dist_log = []

        while not rospy.is_shutdown():
            try:

                self.uwb_init()
                # self.UWB['id'] = np.array(['991B', '9B8F', '439D', '89A4' ])
                # self.UWB['x'] = np.array([0, 0, 1.67, 1.67])
                # self.UWB['y'] = np.array([4.22, 0, 4.22, 0])
                # self.UWB['dist'] = np.array([[1.06, 3.14, 1.89, 3.51], [0.99, 3.19, 1.75, 3.43], [1.33, 2.89, 1.99, 3.32], [1.48, 3.04, 2.06, 3.31]]).T
                # print("ID :", self.UWB['id'])
                # print("x :", self.UWB['x'])
                # print("y :", self.UWB['y'])
                # print("dist :", self.UWB['dist'])
                anch_pos = np.array([x + 1j*y for x, y in zip(self.UWB['x'], self.UWB['y'])])


                if self.r == 0:
                    tag_pos_est, heading_est = self.GetUWBPos_v1(self.UWB['x'], self.UWB['y'], self.UWB['dist'], self.angles_from_heading)
                    print("r:0-tag_post_est: ",tag_pos_est)
                    print("r:0-heading_est: ",heading_est)
                else:
                    tag_pos_est = K_tag_pos_est
                    heading_est = K_heading_est
                    print("r:1~-K_tag_pos_est: ",K_tag_pos_est)
                    print("r:1~-K_heading_est: ",K_heading_est)

                # if np.isnan(tag_pos_est).all() and np.isnan(heading_est):
                    # continue
                # test_tag = np.array([[np.real(tag), np.imag(tag)] for tag in tag_pos_est])

                Xt_e = np.real(tag_pos_est)
                Yt_e = np.imag(tag_pos_est)
                print("Xt_e :", Xt_e)
                print("Yt_e :", Yt_e)


                Xt_c_e = np.mean(Xt_e)
                Yt_c_e = np.mean(Yt_e)
                print("Xt_c_e :", Xt_c_e)
                print("Yt_c_e :", Yt_c_e)


                # plt.figure(6)
                # plt.clf()
                # plt.plot(self.UWB['x'], self.UWB['y'], 'bo')
                # plt.plot(Xt_e, Yt_e, 'r^')
                # plt.show()

                # ## Check Blocked Anchor-Tag
                # A1 = [3, 4, 5]
                # A2 = [1, 2, 5]
                # A3 = [0, 2, 4]
                # A4 = [0, 1, 3]

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
                    print("num :", PP)
                    Pos[PP] = np.array([]).reshape(-1, 2)
                    PosB[PP] = np.array([]).reshape(-1, 2)
                    Prob2T[PP] = np.array([]).reshape(-1, 2)
                    for NN in range(Ln):
                        for MM in range(NN+1, Ln):
                            if NN != MM:
                                Pos2, Prob2 = self.TwoAnchPos4([self.UWB['x'][NN], self.UWB['x'][MM]], [self.UWB['y'][NN], self.UWB['y'][MM]], [self.UWB['dist'][NN, PP], self.UWB['dist'][MM, PP]], np.array([np.real(tag_pos_est[0][PP]), np.imag(tag_pos_est[0][PP])]), np.array([Xt_c_e, Yt_c_e]), anch_pos, self.UWB['dist'][:, PP])
                                print("Pos2 :", Pos2)
                                print("Prob2 :", Prob2)
                                Pos[PP] = np.append(Pos[PP], np.array([Pos2[0, :]]), axis=0)
                                PosB[PP] = np.append(PosB[PP], np.array([Pos2[1, :]]), axis=0)
                                Prob2T[PP] = np.append(Prob2T[PP], np.array(Prob2.T), axis=0)

                    print("Prob2T[PP] :", Prob2T[PP])
                    print("PosB[PP] :", PosB[PP])
                    print("Pos[PP] :", Pos[PP])
                    print("Prob2T[PP][:, 0] :", Prob2T[PP][:, 0])
                    print("Prob2T[PP][:, 1] :", Prob2T[PP][:, 1])
                    Temp = Prob2T[PP][:, 0].reshape(-1, 1) / Prob2T[PP][:, 1].reshape(-1, 1) * 2
                    print("Temp", Temp)
                    ValM, IndM = np.sort(Temp, axis=0)[::-1], np.argsort(Temp, axis=0)[::-1]
                    print("IndM :", IndM)
                    print("ValM :", ValM)

                    TopI = 0
                    # while TopI < len(ValM) and ValM[TopI][0] > 1:
                    while (ValM[TopI] > 1) and (ValM[TopI] > (ValM[-1] * 1)):
                        if np.sum((PosB[PP][IndM[0]] - Pos[PP][IndM[-1]])**2) - np.sum((Pos[PP][IndM[0]] - Pos[PP][IndM[-1]])**2) < 0:
                            Pos[PP][IndM[0]] = PosB[PP][IndM[0]]
                        TopI += 1
                    print("2-Pos[PP] :", Pos[PP])


                    # PosC[PP] = [np.array([x[0] + 1j*x[1]]) for x in Pos[PP]]
                    # QQ[PP] = np.abs(PosC[PP] - anch_pos) - self.UWB['dist'][:, PP].T
                    # KK += np.sum(QQ[PP])

                    # PosC[PP] = [np.array([x[0] + 1j*x[1]]) for x in Pos[PP]]
                    print("Pos :", Pos)
                    PosC[PP] = np.array([np.array([x[0]+1j*x[1]]) for x in Pos[PP]])
                    print("PosC :", PosC[PP])
                    print("PosC :", PosC[PP].shape)
                    if np.all(KK == 0):
                        KK = np.abs(PosC[PP] - anch_pos) - self.UWB['dist'][:, PP].T
                        print("first :", KK)
                    else:
                        KK = KK + np.abs(PosC[PP] - anch_pos) - self.UWB['dist'][:, PP].T
                        print("before :", KK)
                        print("- :", np.abs(PosC[PP] - anch_pos) - self.UWB['dist'][:, PP].T)
                        print("dist :", self.UWB['dist'][:, PP].T)
                        print("1 :", np.abs(PosC[PP] - anch_pos))
                        print("2 :", anch_pos)
                        print("3 :", PosC[PP])


                    QQ[PP] = np.abs(PosC[PP] - anch_pos) - self.UWB['dist'][:, PP].T
                    print("QQ[PP] :",QQ[PP])
                AB = np.zeros(Ln)
                BB = np.zeros(Ln)


                for CD in range(Ln):
                    TE = list(range(Ln))
                    TE.remove(CD)
                    AB[CD] = np.sum(np.abs(KK[np.ix_(A[CD], TE)]))
                    BB[CD] = -np.sum(KK[np.ix_(A[CD], [CD])])
                # for CD in range(Ln):
                #     TE = list(range(Ln))
                #     print(TE)
                #     TE.pop(CD)
                #     print(TE)
                #     print(KK[3:,:3])
                #     print(A[CD])
                #     print(KK[0][A[CD]])
                #     AB[CD] = np.sum(np.abs(KK[A[CD],:][:,TE]))
                #     BB[CD] = -np.sum(KK[A[CD], CD])
                CC = BB / AB
                AB[BB < 0] += 10
                Val, indi = np.sort(np.abs(AB)), np.argsort(np.abs(AB))
                print("CC :", CC)
                print("Val :", Val)
                print("indi :", indi)
                # print(Val)
                if indi[0] != 2:  # Python index starts from 0, so subtract 1 from original MATLAB index
                    dd = 1

                AT = A[indi[0]]

                biasV = np.zeros(Lp)

                for ls in range(Lp):
                    biasV[ls] = np.mean(QQ[ls][AT, indi[0]])

                dist_n = self.UWB['dist']
                # /dist_n[indi[0], :] += biasV


                tag_center_pos_est, heading_est, Xt_c_e, Yt_c_e = self.GetUWBPosUpdate_v1(self.UWB['x'], self.UWB['y'], Xt_c_e, Yt_c_e, heading_est, self.rl, dist_n, self.angles_from_heading)
                tag_pos_est = self.get_tag_pos(tag_center_pos_est, heading_est, self.tag_pos_b)
                print("tag_center_pos_est :", tag_center_pos_est)
                print("heading_est :", heading_est)
                print("tag_pos_est :", tag_pos_est)
                self.heading_est_a = np.append(self.heading_est_a, [heading_est], axis=0)
                self.centerest_a = np.append(self.centerest_a, np.array([Xt_c_e, Yt_c_e]).T, axis=0)

                if (self.r > (self.average_len*2)):
                    MeanA = np.mean(self.centerest_a[self.r-19:self.r-10, 0] + 1j*self.centerest_a[self.r-19:self.r-10, 1])
                    MeanB = np.mean(self.centerest_a[self.r-9:self.r, 0] + 1j*self.centerest_a[self.r-9:self.r, 1])
                    # self.centerest_a_aver = np.append(self.centerest_a_aver, np.array([np.real(MeanB + (MeanB-MeanA)/2), np.imag(MeanB + (MeanB-MeanA)/2)]).T, axis=0)
                    self.centerest_a_aver = np.append(self.centerest_a_aver, np.array([[np.real(MeanB + (MeanB-MeanA)/2), np.imag(MeanB + (MeanB-MeanA)/2)]]), axis=0)
                    MeanA_head = np.mean(self.heading_est_a[self.r-self.average_len*2+1:self.r-self.average_len])
                    MeanB_head = np.mean(self.heading_est_a[self.r-self.average_len+1:self.r])
                    self.headingest_a_aver = np.append(self.headingest_a_aver, np.array([[MeanB_head + (MeanB_head-MeanA_head)/2]]), axis=0)
                else:
                    self.centerest_a_aver = np.append(self.centerest_a_aver, np.array([Xt_c_e, Yt_c_e]).T, axis=0)
                    self.headingest_a_aver = np.append(self.headingest_a_aver, [heading_est], axis=0)

                tag_pos_est_aver = self.get_tag_pos(self.centerest_a_aver[self.r, 0] + 1j*self.centerest_a_aver[self.r, 1], self.headingest_a_aver[self.r], self.tag_pos_b)
                K_Xt_c_e = self.centerest_a_aver[self.r, 0]
                K_Yt_c_e = self.centerest_a_aver[self.r, 1]
                K_tag_pos_est = self.tag_pos_b * np.exp(1j*(self.headingest_a_aver[self.r])) + K_Xt_c_e + 1j*K_Yt_c_e
                K_tag_pos_est = K_tag_pos_est.reshape(1, -1)
                K_heading_est = self.headingest_a_aver[self.r]
                self.K_centerest_a_aver = np.append(self.K_centerest_a_aver, [self.centerest_a_aver[self.r, :]], axis=0)
                self.K_headingest_a_aver = np.append(self.K_headingest_a_aver, [self.headingest_a_aver[self.r]], axis=0)

                print("tag_pos_est_aver :", tag_pos_est_aver)
                print("K_tag_pos_est :", K_tag_pos_est)
                print("K_heading_est :", K_heading_est)


                # Figure 1
                plt.figure(2)
                plt.clf()  # Clear the figure

                plt.text(self.UWB['x'][0], self.UWB['y'][0], self.UWB['id'][0], fontsize=12, color='red')
                plt.text(self.UWB['x'][1], self.UWB['y'][1], self.UWB['id'][1], fontsize=12, color='red')
                plt.text(self.UWB['x'][2], self.UWB['y'][2], self.UWB['id'][2], fontsize=12, color='red')
                plt.text(self.UWB['x'][3], self.UWB['y'][3], self.UWB['id'][3], fontsize=12, color='red')
                plt.plot(self.UWB['x'], self.UWB['y'], 'bo')

                # plt.plot(tag_pos_g[0], 'go')
                # plt.plot(tag_pos_g[1], 'g*')
                # plt.plot(tag_pos_g[2], 'gv')
                # plt.plot(tag_pos_g[3], 'g^')


                # print(tag_pos_est)
                tag_e = np.array([[np.real(tag), np.imag(tag)] for tag in tag_pos_est]).T
                K_tag_e = np.array([[np.real(K_tag), np.imag(K_tag)] for K_tag in K_tag_pos_est]).T

                print(tag_e[0])
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

                # plt.plot(K_tag_pos_est[0], 'ro')
                # plt.plot(K_tag_pos_est[1], 'r*')
                # plt.plot(K_tag_pos_est[2], 'rv')
                # plt.plot(K_tag_pos_est[3], 'r^')

                # plt.quiver(x[r], y[r], np.cos(heading[r]), np.sin(heading[r]), color='g', scale=1, scale_units='xy', angles='xy')
                plt.quiver(Xt_c_e, Yt_c_e, np.cos(heading_est), np.sin(heading_est), color='b', scale=1, scale_units='xy', angles='xy')
                plt.quiver(K_Xt_c_e, K_Yt_c_e, np.cos(K_heading_est), np.sin(K_heading_est), color='r', scale=1, scale_units='xy', angles='xy')

                # if np.abs((x[r] + 1j*y[r]) - (Xt_c_e + 1j*Yt_c_e)) > 1:
                #     gg = 1

                plt.title('Original(Blue) and estimated(Red) Tag to Anchor distance')

                # Figure 4
                plt.figure(3)
                plt.clf()  # Clear the figure
                plt.plot(self.UWB['x'], self.UWB['y'], 'go')
                plt.plot(self.centerest_a_aver[:,0], self.centerest_a_aver[:,1], 'bo')
                # plt.plot(x[:], y[:], 'g^')
                plt.plot(self.K_centerest_a_aver[:,0], self.K_centerest_a_aver[:,1], 'ro')
                plt.plot(self.centerest_a[:,0], self.centerest_a[:,1], 'ko')

                # centerpos_error = np.sqrt(np.mean((self.centerest_a_aver[self.InitLeng:,0] - x[0, InitLeng:]) ** 2 + (centerest_a_aver[InitLeng:,1] - y[0, InitLeng:]) ** 2))
                # heading_error = np.mean(np.abs(heading[InitLeng:] - headingest_a_aver[InitLeng:])) * 180 / np.pi

                # K_centerpos_error = np.sqrt(np.mean((K_centerest_a_aver[InitLeng:,0] - x[0, InitLeng:]) ** 2 + (K_centerest_a_aver[InitLeng:,1] - y[0, InitLeng:]) ** 2))
                # K_heading_error = np.mean(np.abs(heading[InitLeng:] - K_headingest_a_aver[InitLeng:])) * 180 / np.pi

                plt.pause(0.01)




                # print(self.r)

                self.r += 1
            except KeyError:
                pass

        # # save_data = np.array(dist_log)
        # np.save('./dist_log_20230622_2.npy', save_data)
        # import scipy.io as sio

        # data = np.load('./dist_log_20230622_2.npy')

        # sio.savemat('./dist_log_20230622.mat', {'dist_log':data})
        plt.show()




if __name__ == '__main__':
    fusion = Sensor_fusion()
    fusion.main()
    rospy.spin()