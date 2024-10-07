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

x_points = list()
y_points = list() 
z_points = list() 

def callback(msg):
    
    viewer_x = msg.pose.position.x   # float 
    viewer_y = msg.pose.position.y   # float 
    
    x_points.append(viewer_x)
    y_points.append(viewer_y)

def draw_scatter(x_points, y_points) :
    plt.scatter(x_points, y_points)
    center = (1576, 639)
    radius = 130
    circle = plt.Circle(center, radius, color='r', fill=False)
    plt.gca().add_patch(circle) 
    plt.axis('equal')
    plt.title('Viewer_Info')
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


if __name__ == "__main__":
    
    '''
    rospy.init_node('viewer_sub_node', anonymous=True) 
    rospy.Subscriber("/view_node/viewer_pose", PoseStamped, callback)

    rospy.spin() 
    rospy.signal_shutdown('rosbag file is finished')
    with open('/home/keti/catkin_ws/src/uwb_analysis/viewer_x_trj.pickle', 'wb') as f: 
        pickle.dump(x_points, f, pickle.HIGHEST_PROTOCOL) 

    with open('/home/keti/catkin_ws/src/uwb_analysis/viewer_y_trj.pickle', 'wb') as f: 
        pickle.dump(y_points, f, pickle.HIGHEST_PROTOCOL) 
    
    with open('/home/keti/catkin_ws/src/uwb_analysis/viewer_z_trj.pickle', 'wb') as f: 
        pickle.dump(z_points, f, pickle.HIGHEST_PROTOCOL) 
    '''

    
    with open('/home/keti/catkin_ws/src/uwb_analysis/viewer_x_trj.pickle', 'rb') as rf: 
        x_points = pickle.load(rf) 
    
    with open('/home/keti/catkin_ws/src/uwb_analysis/viewer_y_trj.pickle', 'rb') as rf: 
        y_points = pickle.load(rf) 

    with open('/home/keti/catkin_ws/src/uwb_analysis/viewer_z_trj.pickle', 'rb') as rf: 
        z_points = pickle.load(rf) 


    #draw_scatter(x_points, y_points) 
    
    print(len(x_points))
    print(len(y_points))
    #print(len(z_points))

    #x_points = x_points[:int(L/2)]
    #y_points = y_points[:int(L/2)]
    #z_points = z_points[]

    plt.scatter(x_points, y_points)
    plt.title('Viewer_Info')
    plt.show()
    #center = (1576, 639)
    #radius = 130

    #draw_3D_scatter(x_points, y_points, z_points, title='viewer3D') 