import rospy 
from geometry_msgs.msg import PoseStamped 
import numpy as np 
import math 
import matplotlib.pyplot as plt 
import numpy as np
import cv2

#global Variables  
ipe_x_now =  None 
ipe_y_now = None 

lidar_x_now = None 
lidar_y_now = None 

# save points 
lidar_x_pts = list() 
lidar_y_pts = list() 
ipe_x_pts = list()
ipe_y_pts = list()



def ipe_callback(msg) :
    seq_num = msg.header.seq
    ipe_x = msg.pose.position.x   # float 
    ipe_y = msg.pose.position.y     
    received_time = rospy.Time.now() 

    position = [ipe_x, ipe_y]

    process_msg_getTform('ipe', position)   

def lidar_callback(msg) : 

    seq_num = msg.header.seq    
    lidar_x = msg.pose.position.x 
    lidar_y = msg.pose.position.y
    received_time = rospy.Time.now() 

    position = [lidar_x, lidar_y]
    
    process_msg_getTform('lidar', position)


def process_msg_getTform(sensor_name, position): 

    if sensor_name == 'ipe':
        ipe_x = position[0] 
        ipe_y = position[1] 

        global ipe_x_now 
        global ipe_y_now 

        ipe_x_now = ipe_x 
        ipe_y_now = ipe_y 
    

    elif sensor_name == 'lidar' :
    
        lidar_x_now = position[0]
        lidar_y_now = position[1]

        #lidar_pt = [lidar_x, lidar_y]         

        if lidar_x_pts: # not empty  
                    
            if (lidar_x_pts[-1] != lidar_x_now) or (lidar_y_pts[-1] != lidar_y_now) :  

                if(ipe_x_now != None) and (ipe_y_now != None) : 

                    lidar_x_pts.append(lidar_x_now) 
                    lidar_y_pts.append(lidar_y_now)

                    ipe_x_pts.append(ipe_x_now)
                    ipe_y_pts.append(ipe_y_now)
        else: 
            if (ipe_x_now != None) and (ipe_y_now != None) :  
                lidar_x_pts.append(lidar_x_now) 
                lidar_y_pts.append(lidar_y_now)

                ipe_x_pts.append(ipe_x_now)
                ipe_y_pts.append(ipe_y_now)

def getAffineT_lidar2ipe(lidar_x_pts, lidar_y_pts, ipe_x_pts, ipe_y_pts):
   

    src_lidar = np.array([[x, lidar_y_pts[i]] for i, x in enumerate(lidar_x_pts)]) 
    dst_ipe = np.array([[x, ipe_y_pts[i]] for i, x in enumerate(ipe_x_pts)])

    print(src_lidar)
    print(dst_ipe)

    #src_lidar = np.array(list(zip(lidar_x_pts, lidar_y_pts)))
    #dst_ipe = np.array(list(zip(ipe_x_pts, ipe_y_pts))) 
    
    retval, inliers = cv2.estimateAffine2D(src_lidar, dst_ipe, 1) 
 
    return retval

def lidar2ipe_chungra(lidar_x, lidar_y, tf_mat):

    lidar_data = np.array([lidar_x, lidar_y, 1]) 
    new_lidar = np.matmul(tf_mat, lidar_data) 

    new_lidar_x, new_lidar_y = new_lidar

    return new_lidar_x, new_lidar_y 

def visualize_tf(lidar_x_pts, lidar_y_pts, ipe_x_pts, ipe_y_pts, tf_mat) :
    
    new_lidar_x_pts = list()
    new_lidar_y_pts = list() 



    for lidar_x, lidar_y in list(zip(lidar_x_pts, lidar_y_pts)) : 
        
        new_lidar_x, new_lidar_y = lidar2ipe_chungra(lidar_x, lidar_y, tf_mat)
        
        new_lidar_x_pts.append(new_lidar_x)
        new_lidar_y_pts.append(new_lidar_y) 
    
    plt.scatter(new_lidar_x_pts, new_lidar_y_pts) 
    plt.scatter(ipe_x_pts, ipe_y_pts) 
    plt.plot([new_lidar_x_pts, ipe_x_pts], [new_lidar_y_pts, ipe_y_pts])
    plt.legend(('lidar', 'ipe')) 
    plt.show() 


if __name__ == "__main__": 

    rospy.init_node('certification_node', anonymous= True) 
    
    rospy.Subscriber("/ipe/slam_added_pose", PoseStamped, ipe_callback)
    rospy.Subscriber("/kdlidar_ros_pcl/pose", PoseStamped, lidar_callback)

    rospy.spin() 
    rospy.signal_shutdown('rosbag file running is finished')
    
    print(len(lidar_x_pts))
    print(len(lidar_y_pts))

    matrix = getAffineT_lidar2ipe(lidar_x_pts, lidar_y_pts, ipe_x_pts, ipe_y_pts) 
    print(matrix)

    print(len(lidar_x_pts))
    print(len(lidar_y_pts))


    print(len(ipe_x_pts))
    print(len(ipe_y_pts))

    visualize_tf(lidar_x_pts, lidar_y_pts, ipe_x_pts, ipe_y_pts, tf_mat=matrix) 

    '''
    

    src_lidar =np.array([[-3.24, -56.46], [-2.95, -48.76], [-2.86, -40.67], [-2.77, -32.50], [-2.54, -24.57],
    [14.17, -12.15],
    [14.09, -20.55],
    [13.63, -28.45],
    [13.50, -36.23],
    [13.37, -44.53],
    [13.09, -52.90],
    [12.90, -60.69]])

    dst_ipe = np.array([[4.1, 21.97],  [4.0, 30.12],  [3.93, 38.15] ,  [3.81, 46.31], [3.83, 54.30],
    [19.97, 66.78],
    [20.07, 58.38],
    [18.94, 50.88],
    [19.81, 42.66],
    [20.19, 34.82],
    [20.03, 26.32],
    [20.04, 17.95]])


    retval, inliers = cv2.estimateAffine2D(src_lidar, dst_ipe, 1) 
    print(retval)

     src_lidar =np.array([[1.48, -48.42], [1.3, -39.67], [1.1, -32.57], [0.86, -24.15], [0.99, -16.02]])

    dst_ipe = np.array([[3.5, 29.99],  [3.55, 38.71],  [3.53, 45.77] ,  [3.53, 54.30], [3.6, 62.2]])


    
    '''