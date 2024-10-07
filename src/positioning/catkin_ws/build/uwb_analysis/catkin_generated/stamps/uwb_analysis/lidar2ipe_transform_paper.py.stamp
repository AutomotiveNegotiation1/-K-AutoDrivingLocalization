#! /usr/bin/env python
import rospy 
#from localizer_dwm1001.msg import Anchor 
#from uwb_analysis.anc_info_reader import get_uwb_dict
#from functools import partial 
from geometry_msgs.msg import PoseStamped
from nav_msgs.msg import Path

import matplotlib.pyplot as plt 
import numpy 
import pickle
import math
from mpl_toolkits.mplot3d import Axes3D
import cv2 
import numpy as np 

slam_x_pts = list()
slam_y_pts = list()
slam_z_pts = list()

lidar_x_pts = list() 
lidar_y_pts = list() 
lidar_z_pts = list() 

ipe_x_pts = list() 
ipe_y_pts = list()
ipe_z_pts = list() 

uwb_x_pts = list() 
uwb_y_pts = list()

def save_data_as_pickle(): 

    rospy.init_node('ipe_lidar_sub_node', anonymous=True) 
    rospy.Subscriber("/ipe/ipe_track", Path, ipe_callback)
    rospy.Subscriber("/kdlidar_ros_pcl/pose", PoseStamped, lidar_callback)
    rospy.Subscriber("/ipe/uwb_only_pose", PoseStamped, uwb_callback)
    rospy.spin() 
    rospy.signal_shutdown('rosbag file is finished')

    
    # save data as pickle 
    with open('/home/keti/catkin_ws/src/uwb_analysis/ipe_x_trj_global_test1.pickle', 'wb') as f: 
        pickle.dump(ipe_x_pts, f, pickle.HIGHEST_PROTOCOL) 

    with open('/home/keti/catkin_ws/src/uwb_analysis/ipe_y_trj_global_test1.pickle', 'wb') as f: 
        pickle.dump(ipe_y_pts, f, pickle.HIGHEST_PROTOCOL) 
    
    with open('/home/keti/catkin_ws/src/uwb_analysis/ipe_z_trj_global_test1.pickle', 'wb') as f: 
        pickle.dump(ipe_z_pts, f, pickle.HIGHEST_PROTOCOL) 

    with open('/home/keti/catkin_ws/src/uwb_analysis/lidar_x_trj_global_test1.pickle', 'wb') as f: 
        pickle.dump(lidar_x_pts, f, pickle.HIGHEST_PROTOCOL) 

    with open('/home/keti/catkin_ws/src/uwb_analysis/lidar_y_trj_global_test1.pickle', 'wb') as f: 
        pickle.dump(lidar_y_pts, f, pickle.HIGHEST_PROTOCOL) 

    with open('/home/keti/catkin_ws/src/uwb_analysis/lidar_z_trj_global_test1.pickle', 'wb') as f: 
        pickle.dump(lidar_z_pts, f, pickle.HIGHEST_PROTOCOL) 

    with open('/home/keti/catkin_ws/src/uwb_analysis/uwb_x_trj_global_test1.pickle', 'wb') as f: 
        pickle.dump(uwb_x_pts, f, pickle.HIGHEST_PROTOCOL)

    with open('/home/keti/catkin_ws/src/uwb_analysis/uwb_y_trj_global_test1.pickle', 'wb') as f: 
        pickle.dump(uwb_y_pts, f, pickle.HIGHEST_PROTOCOL) 
    
    
def load_data_from_pickle():
    with open('/home/keti/catkin_ws/src/uwb_analysis/ipe_x_trj_global_test1.pickle', 'rb') as rf: 
        ipe_x_pts = pickle.load(rf) 

    with open('/home/keti/catkin_ws/src/uwb_analysis/ipe_y_trj_global_test1.pickle', 'rb') as rf: 
        ipe_y_pts = pickle.load(rf) 

    with open('/home/keti/catkin_ws/src/uwb_analysis/ipe_z_trj_global_test1.pickle', 'rb') as rf: 
        ipe_z_pts = pickle.load(rf) 

    with open('/home/keti/catkin_ws/src/uwb_analysis/lidar_x_trj_global_test1.pickle', 'rb') as rf: 
        lidar_x_pts = pickle.load(rf) 
    
    with open('/home/keti/catkin_ws/src/uwb_analysis/lidar_y_trj_global_test1.pickle', 'rb') as rf: 
        lidar_y_pts = pickle.load(rf) 

    with open('/home/keti/catkin_ws/src/uwb_analysis/lidar_z_trj_global_test1.pickle', 'rb') as rf: 
        lidar_z_pts = pickle.load(rf) 

    with open('/home/keti/catkin_ws/src/uwb_analysis/uwb_x_trj_global_test1.pickle', 'rb') as rf: 
        uwb_x_pts = pickle.load(rf) 
    

    with open('/home/keti/catkin_ws/src/uwb_analysis/uwb_y_trj_global_test1.pickle', 'rb') as rf: 
        uwb_y_pts = pickle.load(rf) 
    
    
    return [ipe_x_pts, ipe_y_pts, ipe_z_pts], [lidar_x_pts, lidar_y_pts, lidar_z_pts], [uwb_x_pts, uwb_y_pts]


def get_lidar2ipe_data():

    lidar_data = np.array([[x, lidar_y_pts[i]] for i, x in enumerate(lidar_x_pts)]) 
    ipe_data = np.array([[x, ipe_y_pts[i]] for i, x in enumerate(ipe_x_pts)]) 

    #L = 100
    #lidar_data = lidar_data[:L]
    #ipe_data = ipe_data[:L]

    return lidar_data, ipe_data 

def ipe_callback(msg):
    
    ipe_x = msg.poses[0].pose.position.x   # float 
    ipe_y = msg.poses[0].pose.position.y   # float 
    ipe_z = msg.poses[0].pose.position.z   # float 

    ipe_x_pts.append(ipe_x) 
    ipe_y_pts.append(ipe_y)
    ipe_z_pts.append(ipe_z) 

    print(msg.header.stamp)

def slam_callback(msg):
    
    slam_x = msg.pose.position.x   # float 
    slam_y = msg.pose.position.y   # float 
    slam_z = msg.pose.position.z   # float 

    slam_x_pts.append(slam_x) 
    slam_y_pts.append(slam_y)
    slam_z_pts.append(slam_z) 

def lidar_callback(msg): 
    
    lidar_x = msg.pose.position.x 
    lidar_y = msg.pose.position.y
    lidar_z = msg.pose.position.z 

    lidar_x_pts.append(lidar_x) 
    lidar_y_pts.append(lidar_y) 
    lidar_z_pts.append(lidar_z) 

def uwb_callback(msg):
    uwb_x = msg.pose.position.x 
    uwb_y = msg.pose.position.y

    uwb_x_pts.append(uwb_x) 
    uwb_y_pts.append(uwb_y) 



def draw_scatter(x_points, y_points, title) :
    plt.scatter(x_points, y_points)
    
    plt.xlabel('x')
    plt.ylabel('y')
    plt.axis('equal')
    plt.title(f'{title}')
    plt.show()

def draw_3D_scatter(x_points, y_points, z_points, title) :
    fig = plt.figure()
    ax = fig.gca(projection='3d')
    
    ax.scatter(x_points, y_points, z_points)
  
    #plt.axis('equal')

    plt.xlabel('x')
    plt.ylabel('y')
    plt.title(f'{title}')
    plt.show()


def getAffineT_lidar2ipe():
    import numpy as np
    import cv2

    src_lidar = np.array([
        [-101.74, 26.25],
        [-81.87, -5.77], 
        [-88.12, -10.02],
        [-94.10, -15.63],
        [-115.63, 19.23],
       
    ])

    dst_ipe = np.array([
        [3.44, 18.74],
        [41.5, 18.4],
        [41.72, 9.44],
        [41.8, 2.71],
        [3.37, 3.92], 
        
    ])

    #retval, Rt, inliers = cv2.estimateAffine3D(src_lidar, dst_ipe) 
    
    retval, inliers = cv2.estimateAffine2D(src_lidar,dst_ipe, 1) 
    return retval
    #return Rt 

def draw_figure_1():

    # Figure 1
    plt.scatter(ipe_x_pts, ipe_y_pts)
    plt.xlabel('x')
    plt.ylabel('y')
    #plt.axis('equal')
    plt.title(f'Tracking Results of Indoor Positioning System')
   
#---------------------------
    #lidar_data, ipe_data = get_lidar2ipe_data()
    #Rt = getAffineT_lidar2ipe(lidar_data, ipe_data) 
    Rt = getAffineT_lidar2ipe() 
    Rt[0][2] += 0.4
    print(Rt)
    import numpy as np

    ipe_tf_x_pts = list()
    ipe_tf_y_pts = list()
    ipe_tf_z_pts = list()

    for i, pt in enumerate(lidar_x_pts):
        x_lidar = pt 
        y_lidar = lidar_y_pts[i] 
        z_lidar = lidar_z_pts[i]

        lidar_data = np.array([x_lidar, y_lidar,1])
        
        new_ipe = np.matmul(Rt, lidar_data)
        
        ipe_tf_x_pts.append(new_ipe[0])
        ipe_tf_y_pts.append(new_ipe[1])

    plt.scatter(ipe_tf_x_pts, ipe_tf_y_pts)

    plt.legend(('Proposed Method', 'Lidar'))
    plt.show()

def draw_figure_2():

    # Figure 2
    plt.scatter(ipe_x_pts, ipe_y_pts)
    plt.xlabel('x')
    plt.ylabel('y')
    #plt.axis('equal')
    plt.title(f'Proposed Method VS UWB Only Used')

    plt.scatter(uwb_x_pts, uwb_y_pts) 
    plt.legend(('Proposed Method', 'UWB'))
    plt.show() 
    
if __name__ == "__main__":
    
    #save_data_as_pickle() 
    ipe_pts, lidar_pts, uwb_pts = load_data_from_pickle()

    ipe_x_pts, ipe_y_pts, ipe_z_pts = ipe_pts 
    lidar_x_pts, lidar_y_pts, lidar_z_pts = lidar_pts 
    uwb_x_pts , uwb_y_pts = uwb_pts

    draw_scatter(lidar_x_pts, lidar_y_pts, 'lidar_xy')
    draw_3D_scatter(lidar_x_pts, lidar_y_pts, lidar_z_pts, 'lidar_xyz')
        
    draw_scatter(ipe_x_pts, ipe_y_pts, 'ipe_xy')
    draw_3D_scatter(ipe_x_pts, ipe_y_pts, ipe_z_pts, 'ipe_xyz')

    draw_scatter(uwb_x_pts, uwb_y_pts, 'uwb_xy')

    draw_figure_1()
    draw_figure_2() 

