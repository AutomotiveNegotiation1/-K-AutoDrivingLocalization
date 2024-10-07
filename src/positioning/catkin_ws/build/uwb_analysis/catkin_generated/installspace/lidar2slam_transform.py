#!/usr/bin/env python3
import rospy 
#from localizer_dwm1001.msg import Anchor 
#from uwb_analysis.anc_info_reader import get_uwb_dict
#from functools import partial 
from geometry_msgs.msg import PoseStamped
import matplotlib.pyplot as plt 
import numpy 
import pickle
import math
from mpl_toolkits.mplot3d import Axes3D
import cv2 

slam_x_pts = list()
slam_y_pts = list()
slam_z_pts = list()

lidar_x_pts = list() 
lidar_y_pts = list() 
lidar_z_pts = list() 


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


def getAffineT():
    import numpy as np
    import cv2

    src_lidar = np.array([
        [-0.37, -3.07, -0.38],
        [12.12, -0.39, -0.38],
        [4.31, -71.9, 0.23],
        [18.3, -68.9, -0.3],
        
        [31.9, 8.67, -0.37],
        [17.7, 25.5, -0.38],
        [5.06, 11.4, -0.38],
        [-3.12, 27.35, -0.38],
        [-15.17, 12.62, 5.78]


    ])

    dst_slam = np.array([
        [0.13, -0.56, -0.28],
        [2.87, -0.06, -0.28],
        [0.54, -16.8, -0.19],
        [3.8, -16.3, -0.18],

        [7.42, 1.42, -0.28],
        [4.4, 6.13, -0.28],
        [1.67, 2.96, 1.28],
        [-0.7, 7.23, 1.26],
        [-3.24, 3.75, -0.28]
    ])

    retval, Rt, inliers = cv2.estimateAffine3D(src_lidar, dst_slam) 
    
    return Rt 

if __name__ == "__main__":
    '''   
    rospy.init_node('slam_lidar_sub_node', anonymous=True) 
    rospy.Subscriber("/orb_slam3/camera_pose", PoseStamped, slam_callback)
    rospy.Subscriber("/kdlidar_ros_pcl/pose", PoseStamped, lidar_callback)

    rospy.spin() 
    rospy.signal_shutdown('rosbag file is finished')

    
    with open('/home/keti/catkin_ws/src/uwb_analysis/slam_x_trj.pickle', 'wb') as f: 
        pickle.dump(slam_x_pts, f, pickle.HIGHEST_PROTOCOL) 

    with open('/home/keti/catkin_ws/src/uwb_analysis/slam_y_trj.pickle', 'wb') as f: 
        pickle.dump(slam_y_pts, f, pickle.HIGHEST_PROTOCOL) 
    

    with open('/home/keti/catkin_ws/src/uwb_analysis/slam_z_trj.pickle', 'wb') as f: 
        pickle.dump(slam_z_pts, f, pickle.HIGHEST_PROTOCOL) 

    with open('/home/keti/catkin_ws/src/uwb_analysis/lidar_x_trj.pickle', 'wb') as f: 
        pickle.dump(lidar_x_pts, f, pickle.HIGHEST_PROTOCOL) 


    with open('/home/keti/catkin_ws/src/uwb_analysis/lidar_y_trj.pickle', 'wb') as f: 
        pickle.dump(lidar_y_pts, f, pickle.HIGHEST_PROTOCOL) 

    with open('/home/keti/catkin_ws/src/uwb_analysis/lidar_z_trj.pickle', 'wb') as f: 
        pickle.dump(lidar_z_pts, f, pickle.HIGHEST_PROTOCOL) 
    '''


    with open('/home/keti/catkin_ws/src/uwb_analysis/slam_x_trj.pickle', 'rb') as rf: 
        slam_x_pts = pickle.load(rf) 

    with open('/home/keti/catkin_ws/src/uwb_analysis/slam_y_trj.pickle', 'rb') as rf: 
        slam_y_pts = pickle.load(rf) 

    with open('/home/keti/catkin_ws/src/uwb_analysis/slam_z_trj.pickle', 'rb') as rf: 
        slam_z_pts = pickle.load(rf) 

    with open('/home/keti/catkin_ws/src/uwb_analysis/lidar_x_trj.pickle', 'rb') as rf: 
        lidar_x_pts = pickle.load(rf) 
    
    with open('/home/keti/catkin_ws/src/uwb_analysis/lidar_y_trj.pickle', 'rb') as rf: 
        lidar_y_pts = pickle.load(rf) 

    with open('/home/keti/catkin_ws/src/uwb_analysis/lidar_z_trj.pickle', 'rb') as rf: 
        lidar_z_pts = pickle.load(rf) 


    #draw_scatter(lidar_x_pts, lidar_y_pts, 'lidar_xy')
    #draw_3D_scatter(lidar_x_pts, lidar_y_pts, lidar_z_pts, 'lidar_xyz')
    
    slam_y_pts = [(-1)*ele for ele in slam_y_pts]
    slam_z_pts = [(-1)*ele for ele in slam_z_pts]

    
    plt.scatter(slam_z_pts, slam_x_pts)
    
    plt.xlabel('x')
    plt.ylabel('y')
    plt.title('slam_xy')
    
    
    
    draw_scatter(slam_z_pts, slam_x_pts, 'slam_xy')
    #draw_3D_scatter(slam_z_pts, slam_x_pts, slam_y_pts, 'slam_xyz')

#---------------------------
    slam_tf_x_pts = list()
    slam_tf_y_pts = list()
    slam_tf_z_pts = list()


    Rt = getAffineT() 
    import numpy as np

    for i, pt in enumerate(lidar_x_pts):
        x_lidar = pt 
        y_lidar = lidar_y_pts[i] 
        z_lidar = lidar_z_pts[i]

        lidar_data = np.array([x_lidar, y_lidar, z_lidar, 1])
        slam_data = np.matmul(Rt, lidar_data)
        
        slam_tf_x_pts.append(slam_data[0])
        slam_tf_y_pts.append(slam_data[1])
        slam_tf_z_pts.append(slam_data[2])
    
    #draw_3D_scatter(slam_tf_x_pts, slam_tf_y_pts, slam_tf_z_pts, 'slam_tf_xyz') 
    #draw_scatter(slam_tf_x_pts, slam_tf_y_pts, 'slam_tf_xy') 

    plt.scatter(slam_tf_x_pts, slam_tf_y_pts)
    plt.show()
    
