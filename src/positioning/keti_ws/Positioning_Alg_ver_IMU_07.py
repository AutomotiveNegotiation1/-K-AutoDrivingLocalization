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
from geometry_msgs.msg import Point
from visualization_msgs.msg import Marker

import numpy as np
from numpy.linalg import inv

from utils import *


import ctypes
from ctypes import CDLL
import os
from datetime import datetime

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

        self.P_kM = (self.A @ self.P_kM1 @ self.A.T) + self.GQG_T
        a = self.A @ self.P_kM1 @ self.A.T

    def update(self):

        
        H = self.H @ self.x
        K = self. P_kM @ self.H.T @ np.array(np.linalg.inv(self.H @ self.H.T @ self.R_gps))
        x = self.x + K @ np.array(self.z - H)
        P = np.array(np.eye(6) - K @ self.H) @ self.P_kM
        
        self.P_kM1 = self.P_kM
        
        return x


class Sensor_fusion:
    def __init__(self):
        # ROS 노드 초기화
        rospy.init_node("sensor_fusion_node")
        np.set_printoptions(threshold=np.inf)
        
        # 절대 경로로 DLL 파일 지정
        dll_path = os.path.abspath("./libuwb.a")
        print(dll_path)
        self.c_func = CDLL(dll_path)
        self.creal_T = creal_T * 4
        self.emxCreate_creal_T = emxCreate_creal_T
        self.tag_pos_est = None
        self.tag_pos_est_aver = None
        
        # self.c_func.argtypes = [ctypes.c_double,  # Ln
        #                         ctypes.c_double,  # Lp
        #                         ctypes.c_double,  # TagNum
        #                         ctypes.c_double,  # Nanchor
        #                         ctypes.POINTER(ctypes.c_double),  # RxID_data
        #                         ctypes.POINTER(ctypes.c_int),  # RxID_size
        #                         ctypes.POINTER(ctypes.c_double),  # RxDist_data
        #                         ctypes.POINTER(ctypes.c_int),  # RxDist_size
        #                         ctypes.c_double,  # s_time
        #                         ctypes.POINTER(Complex),  # dcv
        #                         ctypes.POINTER(ctypes.c_double),  # xa_tmp
        #                         ctypes.POINTER(ctypes.c_double),  # xa_tmp
        #                         ctypes.POINTER(emxArray_creal_T),  # tag_pos_est
        #                         ctypes.POINTER(ctypes.c_double),  # heading_est
        #                         ctypes.POINTER(emxArray_creal_T),  # tag_pos_est_aver
        #                         ctypes.POINTER(ctypes.c_double)]  # headingest_a_aver_v
          
        self.UWB = dict()
        
        self.RxID_list = None
        self.RxID_data_list=None
        
        self.statusUWB = None
        
        self.Lp = 4
        self.Ln = 4
        
        heading_est = 0
        self.heading_est = ctypes.c_double(heading_est)
        heading_a_aver_v = 0
        self.heading_a_aver_v = ctypes.c_double(heading_a_aver_v)
        
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
        
    def ImuCallback(self, msg):
        self.IMU['lx'] = msg.linear_acceleration.x
        self.IMU['ly'] = msg.linear_acceleration.y
        self.IMU['lz'] = msg.linear_acceleration.z
        
        self.IMU['gx'] = msg.angular_velocity.x
        self.IMU['gy'] = msg.angular_velocity.y
        self.IMU['gz'] = msg.angular_velocity.z
        
        self.statusIMU = True

    def create_RxID(self, ids):
        if self.RxID_list is None:
            self.RxID_data_list = ids
            self.RxID_list = [Num for Num, data in enumerate(self.RxID_data_list)]
        else:
            # Check if ids is not equal to self.id_order
            if set(ids) != set(self.RxID_data_list):
                # ids에서 self.id_order에 없는 요소를 찾습니다.
                difference = [item for item in ids if item not in self.RxID_data_list]

                # 이 요소들을 self.id_order에 추가합니다.
                self.RxID_data_list.extend(difference)
                self.RxID_list = [Num for Num, data in enumerate(self.RxID_data_list)]
                

    def uwb_sort(self, num, msg):
        if self.statusUWB is None:
            print('Ok')
            self.statusUWB = False
            
        if len(msg.id) > 1 and self.statusUWB == False:
            self.create_RxID(msg.id)
            self.UWB['TaID'] = num
            self.UWB['AnID'] = np.array(msg.id)
            self.UWB['x'] = np.array(msg.x)
            self.UWB['y'] = np.array(msg.y)
            self.UWB['z'] = np.array(msg.z)
            self.UWB['dist'] = np.array(msg.distanceFromTag)
            self.UWB['time'] = msg.header.stamp.secs
            self.UWB['ntime'] = msg.header.stamp.nsecs
            
            RxID = []
            for item in msg.id:
                if item in self.RxID_data_list:
                    RxID.append(self.RxID_list[self.RxID_data_list.index(item)])
                    
            self.UWB['RxID'] = RxID
            
            print("num:",self.UWB['TaID'])
            print("id:", len(np.array(msg.id)))
            print("RxID:", RxID)
            
            self.statusUWB = True
        else:
            self.statusUWB = False
            
    def Anchorcallback0(self, msg):
        self.uwb_sort(0, msg)

    def Anchorcallback1(self, msg):
        self.uwb_sort(1, msg)

    def Anchorcallback2(self, msg):
        self.uwb_sort(2, msg)
    
    def Anchorcallback3(self, msg):
        self.uwb_sort(3, msg)
        
    def ros_time_to_datetime(self, seconds, nsecs):
        from datetime import timedelta
        epoch = datetime.utcfromtimestamp(0)
        ros_time = epoch + timedelta(seconds=seconds, microseconds=nsecs/1000)
        return ros_time
            
    # Function to convert EmxArrayCRealT instance to Python complex number list
    def emxArray_to_pycomplex_list(self, emxArray):
        return [(self.get_creal_ptr(emxArray.contents.data, i).contents.re, self.get_creal_ptr(emxArray.contents.data, i).contents.im) for i in range(4)]


    def get_creal_ptr(self, base_ptr, offset):
        Creal_T_Ptr = ctypes.POINTER(creal_T)
        creal_ptr = ctypes.cast(ctypes.addressof(base_ptr.contents) + offset * ctypes.sizeof(creal_T), Creal_T_Ptr)
        return creal_ptr
    
    

    def UWB_Positioning(self):
        # print("total RxID_data", self.RxID_data_list)
        # print("RxID", self.UWB['RxID'])
        # print("RxID_data", self.UWB['AnID'])
        
        xa = self.UWB['x']
        ya = self.UWB['y']
        
        tag_pos_est = self.c_func.emxCreate_creal_T(4,1)
        tag_pos_est_aver = self.c_func.emxCreate_creal_T(4,1)
        
        if len(xa) < len(ya):
            xa = np.concatenate((xa, np.zeros(len(ya) - len(xa))))
        else:
            ya = np.concatenate((ya, np.zeros(len(xa) - len(ya))))
            
        anch_pos = xa + 1j*ya
        
        xt_b = np.array([-0.5, 0.5, -0.5, 0.5])
        yt_b = np.array([0.5, 0.5, -0.5, -0.5])
        tag_pos_b = creal_T * 4
        # tag_pos_b.re = xt_b
        # tag_pos_b.im = 1j*yt_b
        # tag_pos_b = xt_b + 1j*yt_b
        
        RxID_data = np.array(self.UWB['RxID']).reshape(-1, 1)
        RxDist_data = np.array(self.UWB['dist']).reshape(-1, 1)
        # RxID_size = len(self.UWB['RxID'])
        # RxDist_size = len(self.UWB['dist'])
        RxID_size = [len(self.UWB['RxID']), 0]
        RxDist_size = [len(self.UWB['dist'])]
        

        Nanchor = len(self.UWB['AnID'])
        TagNum = self.UWB['TaID']
        
        s_time = self.UWB['time']
        
        print("Ln:",self.Ln)
        print("Lp:",self.Lp)
        print("TagNum:",TagNum)
        print("Nanchor:",Nanchor)
        print("RxID_data:",RxID_data)
        print("RxID_size:",RxID_size)
        print("RxDist_data:",RxDist_data)
        print("s_time:",s_time)
        # print("tag_pos_b:",tag_pos_b)
        print("xa:",xa)
        print("xa_size:",len(xa))
        print("ya:",ya)
        print("ya_size:",len(ya))
        # print("tag_pos_est:",tag_pos_est)
        # print("heading_est:",heading_est)
        # print("tag_pos_est_aver:",tag_pos_est_aver)
        # print("headingest_a_aver_v:",headingest_a_aver_v)

        # UWBpos 함수 호출
        xa = (ctypes.c_double * len(xa))(*xa)
        xa_size = ctypes.c_double(len(xa))
        ya = (ctypes.c_double * len(ya))(*ya)
        ya_size = ctypes.c_double(len(ya))
        Lp = ctypes.c_double(self.Lp)
        Ln = ctypes.c_double(self.Ln)
        RxID_data = (ctypes.c_double * len(RxID_data))(*RxID_data)
        RxDist_data = (ctypes.c_double * len(RxDist_data))(*RxDist_data)
        TagNum = ctypes.c_double(TagNum)
        Nanchor = ctypes.c_double(Nanchor)
        # heading_est = ctypes.POINTER(self.heading_est)
        heading_est = ctypes.pointer(self.heading_est)
        headingest_a_aver_v = ctypes.pointer(self.heading_a_aver_v)
        # RxID_size = ctypes.c_int(RxID_size)
        # RxDist_size = ctypes.c_int(RxDist_size)
                    # (ctypes.c_double * len(xa))(*xa)
        RxID_size = (ctypes.c_double * len(RxID_size))(*RxID_size)
        RxDist_size = (ctypes.c_double * len(RxDist_size))(*RxDist_size)
        
        start_time = time.time()
        self.c_func.UWBpos(Ln, Lp, TagNum, Nanchor,RxID_data, RxID_size,RxDist_data, RxDist_size, s_time, tag_pos_b, xa, xa_size, ya, ya_size,tag_pos_est, heading_est,tag_pos_est_aver, headingest_a_aver_v)  
        

    
    def main(self):
        self.s_time = []
        while not rospy.is_shutdown():

            
            if self.statusUWB == True:
                self.UWB_Positioning()
                self.statusUWB = False
                # self.c_func.main_UWBpos()
        
        
# ctypes로 사용할 C 언어의 데이터 형식 정의
class creal_size_T(ctypes.Structure):
    _fields_ = [('rows', ctypes.c_int),
                ('cols', ctypes.c_int)]
    
class creal_T(ctypes.Structure):
    _fields_ = [('re', ctypes.c_double),
                ('im', ctypes.c_double)]

class emxCreateWrapper_creal_T(ctypes.Structure):
    _fields_ = [("data", ctypes.POINTER(creal_T)),
                ("size", ctypes.POINTER(creal_size_T)),
                ("numDimensions", ctypes.c_int),
                ("allocatedSize", ctypes.c_int),
                ("canFreeData", ctypes.c_bool)]

class emxCreate_creal_T(ctypes.Structure):
    _fields_ = [("data", ctypes.POINTER(creal_T)),
                ("size", ctypes.POINTER(creal_size_T)),
                ("numDimensions", ctypes.c_int),
                ("allocatedSize", ctypes.c_int)]



    
if __name__ == '__main__':
    fusion = Sensor_fusion()
    fusion.main()
    rospy.spin()