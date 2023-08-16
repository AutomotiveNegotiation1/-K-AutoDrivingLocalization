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
        dll_path = os.path.abspath("./libuwb2.so")
        self.c_func = CDLL(dll_path)
          
        self.UWB = dict()
        
        self.RxID_list = None
        self.RxID_data_list = None
        
        self.statusUWB = None
        
        self.Lp = 4
        self.Ln = 4
        self.zt_b = 1.53
        
        self.tag_pos_est = np.zeros((2,4))
        self.tag_pos_est_aver = np.zeros((2,4))
        self.heading_est = 0
        self.heading_a_aver_v = 0
        
        # ROS 노드 구독
        # rospy.Subscriber("/zed_f9r/imu", Imu, self.ImuCallback)
        rospy.Subscriber("/dwm1001/anchor/ttyUWB0", Anchor, self.Anchorcallback0)
        rospy.Subscriber("/dwm1001/anchor/ttyUWB1", Anchor, self.Anchorcallback1)
        rospy.Subscriber("/dwm1001/anchor/ttyUWB2", Anchor, self.Anchorcallback2)
        rospy.Subscriber("/dwm1001/anchor/ttyUWB3", Anchor, self.Anchorcallback3)
        
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
            self.statusUWB = False
            
        if len(msg.id) > 1 and self.statusUWB == False:
            self.create_RxID(msg.id)
            self.UWB['TaID'] = num
            self.UWB['AnID'] = np.array(msg.id)
            self.UWB['xain'] = np.array(msg.x)
            self.UWB['yain'] = np.array(msg.y)
            self.UWB['zain'] = np.array(msg.z)
            self.UWB['time'] = msg.header.stamp.secs
            self.UWB['ntime'] = msg.header.stamp.nsecs
            self.UWB['RXID'] = [0, 0, 0, 0, 0, 0]
            self.UWB['RXDist'] = [0, 0, 0, 0, 0, 0]
            
            RxID = []
            for i, item in enumerate(msg.id):
                if item in self.RxID_data_list:
                    self.UWB['RXID'][i] = self.RxID_data_list.index(item)+1
                    # self.UWB['RXDist'][i] = msg.distanceFromTag[i]
                
            
            # # Compute the new RXDist values using the scalar self.zt_b
            # self.UWB['RXDist'] = np.sqrt(np.array(self.UWB['RXDist'])**2 - (np.array(msg.z[0]) - self.zt_b)**2).real

            #         # RxDist = real(sqrt(RxDistOrig.^2-(za(1)-zt_b)^2));
            
            # Create a mapping from msg.id to the corresponding values
            id_mapping = {item: (distance, x, y, z) for item, distance, x, y, z in zip(msg.id, msg.distanceFromTag, msg.x, msg.y, msg.z)}

            # Create new arrays for the values, filling them in based on the order defined in self.RxID_data_list
            RXDist_new, xain_new, yain_new, zain_new = [], [], [], []
            for item in self.RxID_data_list:
                distance, x, y, z = id_mapping.get(item, (0, 0, 0, 0))  # Default values if item not found
                RXDist_new.append(distance)
                xain_new.append(x)
                yain_new.append(y)
                zain_new.append(z)

            # Update the self.UWB dictionary
            self.UWB['RXDist'] = RXDist_new
            self.UWB['xain'] = xain_new
            self.UWB['yain'] = yain_new
            self.UWB['zain'] = zain_new

            # Compute the new RXDist values using the scalar self.zt_b
            self.UWB['RXDist'] = np.sqrt(np.array(self.UWB['RXDist'])**2 - (np.array(zain_new[0]) - self.zt_b)**2).real

            
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
        # Define the argument types for UWBpos2
        UWBout = emxArray_real_T()
        tag_pos_b = (creal_T * 4)()
#         xt_b = [-0.525 0.525 -0.525 0.525];
# yt_b = [0.505 0.505 -0.505 -0.505];
# zt_b = 1.53;
        xt_b = np.array([-0.525, 0.525, -0.525, 0.525])
        yt_b = np.array([0.505, 0.505, -0.505, -0.505])
        tag_b = xt_b + 1j*yt_b
        
        for i in range(4):
            tag_pos_b[i].real = np.real(tag_b[i])
            tag_pos_b[i].imag = np.imag(tag_b[i])
        
        Ln = ctypes.c_double(self.Ln)
        Lp = ctypes.c_double(self.Lp)
        LnC = ctypes.c_double(len(self.RxID_data_list))
        TagNum = ctypes.c_double(self.UWB['TaID']+1)
        Nanchor = ctypes.c_double(len(self.UWB['AnID']))
        print("self.UWB['RXID']", self.UWB['RXID'])
        print("self.UWB['RXDist']", self.UWB['RXDist'])
        RxIDin = (ctypes.c_double * 6)(*[float(x) for x in self.UWB['RXID']])
        RxDistin = (ctypes.c_double * 6)(*[float(x) for x in self.UWB['RXDist']])
        s_time = ctypes.c_double(self.UWB['time']+self.UWB['ntime']/1e9)
        xain = (ctypes.c_double * 6)(*[float(x) for x in self.UWB['xain']])
        yain = (ctypes.c_double * 6)(*[float(x) for x in self.UWB['yain']])

        # Define the argument types for UWBpos2
        self.c_func.UWBpos2.argtypes = [
            ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double,
            ctypes.c_double * 6, ctypes.c_double * 6, ctypes.c_double,
            creal_T * 4, ctypes.c_double * 6, ctypes.c_double * 6,
            ctypes.POINTER(emxArray_real_T)
        ]

        # Assuming UWBout is an output, you might need to initialize its properties based on how it's used in the C function
        UWBout.data = None  # or appropriate initialization
        UWBout.size = (ctypes.c_int * 2)()  # or appropriate initialization
        UWBout.size[0] = 1
        UWBout.size[1] = 18
        UWBout.allocatedSize = 0
        UWBout.numDimensions = 2
        UWBout.canFreeData = False

        # Call the function with the arguments
        self.c_func.UWBpos2(Ln, Lp, LnC, TagNum, Nanchor, RxIDin, RxDistin, s_time, tag_pos_b, xain, yain, ctypes.byref(UWBout))
        
        # Get the dimensions of the UWBout array
        rows = UWBout.size[0]
        cols = UWBout.size[1]

        # Convert the pointer to data into a Python list
        data_list = [UWBout.data[i] for i in range(rows * cols)]

        # Convert the list into a NumPy array and reshape it to the correct dimensions
        data_array = np.array(data_list).reshape((rows, cols))

        # Print the UWBout data
        print("UWBout:")
        self.tag_pos_est[0] = data_array[0][0:4]
        self.tag_pos_est[1] = data_array[0][4:8]
        print("tag_pos_est-->", self.tag_pos_est)
        self.heading_est = data_array[0][8]
        print("heading_est-->", self.heading_est)
        self.tag_pos_est_aver[0] = data_array[0][9:13]
        self.tag_pos_est_aver[1] = data_array[0][13:17]
        print("tag_pos_est_aver-->", self.tag_pos_est_aver)
        self.headingest_a_aver_v = data_array[0][17]
        print("headingest_a_aver_v-->", self.headingest_a_aver_v)
        


        self.figure()
        
    def figure(self):
        plt.clf()  # Clear the figure
        plt.axes().set_aspect('equal')
        plt.plot(self.UWB['xain'], self.UWB['yain'], 'bo')
        xa = [0.00, 0.00, 22.80, 7.80, 7.80, 8.15, 8.15, 24.50]
        ya = [15.40, 0.00, -3.90, 0.00, 10.10, 10.45, -0.35, 21.70] 
        plt.plot(xa, ya, 'g*')

        plt.plot(self.tag_pos_est[0][0], self.tag_pos_est[1][0], 'ro')
        plt.plot(self.tag_pos_est[0][1], self.tag_pos_est[1][1], 'r*')
        plt.plot(self.tag_pos_est[0][2], self.tag_pos_est[1][2], 'rv')
        plt.plot(self.tag_pos_est[0][3], self.tag_pos_est[1][3], 'r^')
        
        Xt_c_e = np.mean(self.tag_pos_est[0])
        Yt_c_e = np.mean(self.tag_pos_est[1])

        plt.quiver(Xt_c_e, Yt_c_e, np.cos(self.heading_est), np.sin(self.heading_est), color='b', linewidth=1, headwidth=6)
        plt.pause(0.001)
        
    
    def main(self):
        plt.figure(1)
        self.s_time = []
        while not rospy.is_shutdown():

            
            if self.statusUWB == True:
                self.UWB_Positioning()
                self.statusUWB = False
        plt.show()      
                # self.c_func.main_UWBpos()
        
        
class creal_T(ctypes.Structure):
    _fields_ = [
        ('real', ctypes.c_double),
        ('imag', ctypes.c_double)
    ]

class emxArray_creal_T(ctypes.Structure):
    _fields_ = [
        ('data', ctypes.POINTER(creal_T)),
        ('size', ctypes.POINTER(ctypes.c_int *2)),
        ('allocatedSize', ctypes.c_int),
        ('numDimensions', ctypes.c_int),
        ('canFreeData', ctypes.c_bool)
    ]

class emxArray_real_T(ctypes.Structure):
    _fields_ = [
        ('data', ctypes.POINTER(ctypes.c_double)),
        ('size', ctypes.POINTER(ctypes.c_int)),
        ('allocatedSize', ctypes.c_int),
        ('numDimensions', ctypes.c_int),
        ('canFreeData', ctypes.c_bool)
    ]


    
if __name__ == '__main__':
    fusion = Sensor_fusion()
    fusion.main()
    rospy.spin()