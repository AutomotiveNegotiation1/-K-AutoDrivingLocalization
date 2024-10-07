import rospy 
from geometry_msgs.msg import PoseStamped 
import numpy as np 
import math 
import matplotlib.pyplot as plt 

lidar_time = None 
ipe_time = None 

lidar_position = None
ipe_position = None 

# save points 
lidar_x_pts = list() 
lidar_y_pts = list() 
ipe_x_pts = list()
ipe_y_pts = list()


# Errors 
total_error = 0
total_error_mean = 0
total_x_error = 0 
total_y_error = 0 

# save points for Plot 
plt_lidar = list()
plt_ipe1 = list()
plt_ipe2 = list()

def ipe_callback(msg) :

    ipe_x = msg.pose.position.x   # float 
    ipe_y = msg.pose.position.y 
    
    received_time = rospy.Time.now() 

    position = [ipe_x, ipe_y]
    
    #save_all_points('ipe', position) 
    process_msg_minDist('ipe', position)
    # process_msg_minTime('ipe', received_time, position) 
    

def lidar_callback(msg) : 
    
    lidar_x = msg.pose.position.x 
    lidar_y = msg.pose.position.y
    received_time = rospy.Time.now() 
    lidar_x, lidar_y = lidar2ipe_chugra(lidar_x, lidar_y)
    position = [lidar_x, lidar_y]
    
    process_msg_minDist('lidar', position)

def lidar2ipe_chugra(lidar_x, lidar_y):
    
    '''
    tf_mat = np.array([
    [ 1.00530427e+00, 5.81048522e-02, 4.65577398e+00],
    [-9.16979796e-02, 1.00543656e+00,  7.92377870e+01]])
    
    
    tf_mat = np.array([ 
    [1.05784540e+00, 6.09003875e-02, 4.80116345e+00],
    [6.89235261e-01, 1.04901579e+00, 8.09124642e+01]])
    
    tf_mat = np.array([
        [ 1.18662706e-01, -1.14563905e-02,  3.80976146e+00],
        [ 2.04436602e+00,  9.69749012e-01,  8.34054706e+01]])
    
    tf_mat = np.array([
    [ -0.16840653, 0.94657628, -22.9825818],
    [ -0.968661, -0.30603554, 63.72679125]])    
    
    tf_mat = np.array([
    [ 1.03573539e+00, 1.84865905e-02, 2.91058301e+00],
    [ 5.83632499e-01, 1.00289443e+00, 7.79847752e+01]])    
    '''
    tf_mat = np.array([ 
    [1.86051095e-01,  5.55207250e-03,  3.50737336e+00],
    [-6.30867491e-01,  9.85020253e-01,  7.86011347e+01]])


    lidar_data = np.array([lidar_x, lidar_y, 1]) 
    new_lidar = np.matmul(tf_mat, lidar_data) 

    new_lidar_x, new_lidar_y = new_lidar
    return new_lidar_x, new_lidar_y 

def save_all_points(sensor_name, position): 
    if sensor_name == 'ipe':
        ipe_x_pts.append(position[0])
        ipe_y_pts.append(position[1])

    elif sensor_name == 'lidar': 
        lidar_x_pts.append(position[0])
        lidar_y_pts.append(position[1]) 

    
def get_closest_ipepair(fixed_pt):  
    lidar_x = fixed_pt[0] 
    lidar_y = fixed_pt[1]
    
    ipe_x_pts, ipe_y_pts

    ipe_reordered= [[ipe_x, ipe_y_pts[i]] for i, ipe_x in enumerate(ipe_x_pts)]
    min_dist = 100000000
    min_x = min_second_x = -1
    min_y = min_second_y = -1 
    min_second_dist = min_dist

    for x, y in ipe_reordered:
       
        dist = math.sqrt(((x-fixed_pt[0])**2) + ((y-fixed_pt[1])**2))  
        if min_dist > dist:

            min_second_x = min_x   
            min_second_y = min_y
            min_second_dist = min_dist
            
            min_dist = dist             
            min_x = x 
            min_y = y 

        
    return [min_x, min_y, min_dist], [min_second_x, min_second_y, min_second_dist]


    
def process_msg_minDist(sensor_name, position) : 

    #if position[0] < 4.0: 
        save_all_points(sensor_name, position)  # save points (Lidar & IPE)

        if sensor_name == 'lidar':  
        
            lidar_x = position[0] 
            lidar_y = position[1] 

            lidar_pt = [lidar_x, lidar_y] 
            
            tup1, tup2 = get_closest_ipepair(lidar_pt)  
            
            # set error as min_distance using only one point 
            min_dist_raw = tup1[2] 
            #if min_dist_raw < 0.1: 
            print(f'Error[minDist] of IPE: {min_dist_raw}m\n')
            global total_error 

            total_error += min_dist_raw 

            plt_lidar.append(lidar_pt) 
            plt_ipe1.append([tup1[0], tup1[1]]) 
            '''
            # set error as min_distance using 2 points 
            
            a, b,c = get_line_equation(tup1, tup2)
            min_dist = dist_line2point(a,b,c, lidar_pt[0], lidar_pt[1])
            flag = False 
            
                   if flag and (min_dist < 0.1 and min_dist != -1) :
            
                print(f'Error of IPE: {min_dist}m')
                ipe_1 =[tup1[0], tup1[1]]
                ipe_2 = [tup2[0], tup2[1]]

                plt_lidar.append(lidar_pt)
                plt_ipe1.append(ipe_1)
                plt_ipe2.append(ipe_2)
            else: 
                print(f'Error of IPE: {min_dist}m')
            '''
               
def plot_minDist_1pt(plt_lidar, plt_ipe1): 

    L = len(plt_lidar) 
    error = total_error / L  
    print(f'Mean of Errors:{error}m') 

    for i, lidar_pt in enumerate(plt_lidar): 

        if (plt_ipe1[i][0] <0 and plt_ipe1[i][1] <0): 
            continue 

        plt.scatter(lidar_pt[0], lidar_pt[1], c = 'b') 
        #plt.scatter(plt_ipe1[i][0], plt_ipe1[i][1])

        plt.plot([lidar_pt[0], plt_ipe1[i][0]], [lidar_pt[1], plt_ipe1[i][1]], marker = 'o')
    plt.show() 

def plot_minDist_2pts(plt_lidar, plt_ipe1, plt_ipe2): 

    for i, lidar_pt in enumerate(plt_lidar): 

        if (plt_ipe1[i][0] <0 and plt_ipe1[i][1] <0) or (plt_ipe2[i][0] <0 and plt_ipe2[i][1]<0): 
            continue 

        plt.scatter(lidar_pt[0], lidar_pt[1], c = 'b') 
        #plt.scatter(plt_ipe1[i][0], plt_ipe1[i][1])

        plt.plot([lidar_pt[0], plt_ipe1[i][0]], [lidar_pt[1], plt_ipe1[i][1]], marker = 'o')
        plt.plot([lidar_pt[0], plt_ipe2[i][0]], [lidar_pt[1], plt_ipe2[i][1]], marker = 'x')

    #plt.axis('equal')    
    plt.show() 

def dist_line2point(a,b,c, fixed_x, fixed_y) :
    if a==0 and b==0 : 
        return -1 
    else:     
        dist = abs(a*fixed_x + b*fixed_y + c) / math.sqrt(a**2 + b**2) 
    return dist 

def get_line_equation(tup1, tup2) : 
    x1 = tup1[0]
    y1 = tup1[1]

    x2 = tup2[0]
    y2 = tup2[1] 

    if y1 == y2: 
        return (0,0,0)
    else:
        m = (x1-x2) / (y1-y2) 
        
        return (m, -1, (-1)*m*x1 + y1)



def process_msg_minTime(sensor_name, msg_time, position) :

    if sensor_name == 'ipe':
        global ipe_time
        global ipe_position

        ipe_time = msg_time 
        ipe_position = position

    
    elif sensor_name =='lidar':
        global lidar_time
        global lidar_position

        lidar_time = msg_time 
        lidar_position = position 


    if (ipe_time is not None) and (lidar_time is not None) : 
        
        time_diff = abs(ipe_time - lidar_time).to_sec()
        if time_diff< 0.001 and ipe_position[0]<4.0 :

            ipe_x_pts.append(ipe_position[0])
            ipe_y_pts.append(ipe_position[1])
            lidar_x_pts.append(lidar_position[0])
            lidar_y_pts.append(lidar_position[1])

            error = math.sqrt((lidar_position[0] - ipe_position[0])**2 + (lidar_position[1] - ipe_position[1]) ** 2 )
            #if error <0.1: 
            print(f'error: {error}')
            print(f'ipe_x: {ipe_position[0]} / lidar_x: {lidar_position[0]} / time_diff: {time_diff}')
            print(f'ipe_y: {ipe_position[1]} / lidar_y: {lidar_position[1]} / time_diff: {time_diff}')

            lidar_time = None 
            ipe_time = None 

def draw_trajectory():

    print(f'lidar len: {len(lidar_x_pts)}')
    print(f'IPE len: {len(ipe_x_pts)}')

    global total_error_mean 
    global total_x_error 
    global total_y_error
            
    total_error_mean /= len(lidar_x_pts)
    total_x_error /= len(lidar_x_pts)
    total_y_error /= len(lidar_y_pts)

    print(f'total_error_mean: {total_error_mean}')
    print(f'total_x_error: {total_x_error}')
    print(f'total_y_error: {total_y_error}')

    plt.scatter(lidar_x_pts, lidar_y_pts) 
    plt.title('Trajectory: Lidar VS IPE')

    plt.scatter(ipe_x_pts, ipe_y_pts) 
    plt.legend(('Lidar', 'IPE'))
    plt.axis('equal')
    plt.show() 
    

if __name__ == "__main__": 

    rospy.init_node('certification_node', anonymous= True) 
    
    rospy.Subscriber("/ipe/slam_added_pose", PoseStamped, ipe_callback)
    rospy.Subscriber("/kdlidar_ros_pcl/pose", PoseStamped, lidar_callback)

    rospy.spin() 
    rospy.signal_shutdown('rosbag file running is finished')
    
    plot_minDist_1pt(plt_lidar, plt_ipe1) 
    #plot_minDist_2pts(plt_lidar, plt_ipe1, plt_ipe2)  
    #draw_trajectory()
    #matrix = getAffineT_lidar2ipe(lidar_x_pts, lidar_y_pts, ipe_x_pts, ipe_y_pts)