#! /usr/bin/env python
import rospy 
#from localizer_dwm1001.msg import Anchor 
#from uwb_analysis.anc_info_reader import get_uwb_dict
#from functools import partial 
from geometry_msgs.msg import PoseStamped
import matplotlib.pyplot as plt 
import numpy 
import pickle
import math

x_points = list()
y_points = list() 

new_x_points = list() 
new_y_points = list() 

new_x_else = list() 
new_y_else = list() 

new_2_x_points = list() 
new_2_y_points = list() 

new_3_x_points = list() 
new_3_y_points = list() 


test_theta = list() 

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

def post_process(x,y):

    point = (x, y)
    center = (1576, 639)
    radius = 130 

    theta = math.atan((point[0]-center[0])/ (point[1]-center[1]))  
    
    if (point[0] -center[0]) > 0  and (point[1] - center[1] >0) :  
        new_x = radius * math.cos(theta) + center[0] 
        new_y = radius * math.sin(theta) + center[1] 

        new_x_points.append(new_x)
        new_y_points.append(new_y) 
            
    
    elif (point[0] -center[0]) > 0  and (point[1] - center[1] < 0) : 
        if theta > -1.29 : 

            new_x = radius * math.cos(theta) + center[0] 
            new_y = radius * math.sin(theta) + center[1] 

            new_x_points.append(new_x)
            new_y_points.append(new_y) 
            
    
    elif (point[0] -center[0]) < 0  and (point[1] - center[1] > 0) : 
        
        theta= (math.pi/ 2) + (theta)*(-1)
    
        new_x = radius * math.cos(theta) + center[0] 
        new_y = radius * math.sin(theta) + center[1] 

        new_x_points.append(new_x)
        new_y_points.append(new_y) 
        
        #test_theta.append(theta) 


def post_process_2(x,y):
    point = (x, y)
    center = (1534, 593)
    radius = 176 
    theta = math.atan((point[0]-center[0])/ (point[1]-center[1]))  

    if (point[0] -center[0]) < 0  and (point[1] - center[1] > 0) : 
        
        theta= (math.pi/ 2) + (theta)*(-1)
    
        new_x = radius * math.cos(theta) + center[0] 
        new_y = radius * math.sin(theta) + center[1] 

        new_2_x_points.append(new_x)
        new_2_y_points.append(new_y) 

def post_process_3(x,y):
    # determine Indoor / Outdoor 
    new_3_x_points.append(x) 
    new_3_y_points.append(y) 
    

if __name__ == "__main__":
    
    """
    rospy.init_node('viewer_sub_node', anonymous=True) 
    rospy.Subscriber("/view_node/viewer_pose", PoseStamped, callback)

    rospy.spin() 
    rospy.signal_shutdown('rosbag file is finished')
    with open('./uwb_analysis/x_trj.pickle', 'wb') as f: 
        pickle.dump(x_points, f, pickle.HIGHEST_PROTOCOL) 

    with open('./uwb_analysis/y_trj.pickle', 'wb') as f: 
        pickle.dump(y_points, f, pickle.HIGHEST_PROTOCOL) 
    """

    with open('/home/keti/catkin_ws/src/uwb_analysis/x_trj.pickle', 'rb') as rf: 
        x_points = pickle.load(rf) 
    
    with open('/home/keti/catkin_ws/src/uwb_analysis/y_trj.pickle', 'rb') as rf: 
        y_points = pickle.load(rf) 


    #draw_scatter(x_points, y_points) 
    
    L = len(x_points)
    #x_points = x_points[:int(L/3)]
    #y_points = y_points[:int(L/3)]


    for i, x_pt in enumerate(x_points): 
        y_pt = y_points[i] 

        #if (x_pt > 1514 and y_pt > 513) : 
        if (x_pt > 1522 and y_pt > 513) : 
            post_process(x_pt, y_pt) 

        elif(x_pt> 1415 and x_pt<1530): 
            if(y_pt>716 and y_pt<762):
                post_process_2(x_pt, y_pt)

        if (x_pt >1370 and y_pt<628) : 
            post_process_3(x_pt, y_pt) 
            
        #else : 
        new_x_else.append(x_pt) 
        new_y_else.append(y_pt) 
            

    plt.scatter(new_x_else, new_y_else)
    #center = (1576, 639)
    #radius = 130

    center = (1534, 593)
    radius = 176
    circle = plt.Circle(center, radius, color='r', fill=False)
    plt.gca().add_patch(circle) 
    plt.axis('equal')


    plt.scatter(new_x_points, new_y_points)
    #center = (1576, 639)
    #radius = 130
    #circle = plt.Circle(center, radius, color='r', fill=False)
    #plt.gca().add_patch(circle) 
    plt.axis('equal')

    plt.scatter(new_2_x_points, new_2_y_points)

    plt.scatter(new_3_x_points, new_3_y_points)
    
    plt.show()
    '''
    theta_x = [x for x in range(len(test_theta))]
    plt.scatter(theta_x, test_theta) 
    plt.show()
    '''
