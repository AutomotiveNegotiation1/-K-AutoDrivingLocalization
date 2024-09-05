#! /usr/bin/env python
import rospy 
from localizer_dwm1001.msg import Anchor 
from uwb_analysis.anc_info_reader import get_uwb_dict
from functools import partial 
import matplotlib.pyplot as plt 
import numpy 

points = list()
timestamp = 0 
UWB_DICT = get_uwb_dict() 

def callback(msg, tag_num):
    
    global timestamp 
    timestamp += 1 

    id_list = msg.id  # string
    x_list = msg.x   # float 
    y_list = msg.y   # float 
    z_list = msg.z   # float 
    distanceFromTag_list = msg.distanceFromTag

    tmp_str = str(len(id_list))+ ' ' 
    for id_ in id_list: 
        tmp_str += str(id_) + ' '
        id_num = UWB_DICT[str(id_)][0] 
        
        pt = (timestamp, str(id_), str(id_num))
        global points 
        points.append(pt) 

    rospy.loginfo(f"TAG {tag_num}: There are {tmp_str} Activated Anchors!")

def draw_scatter(points) :

    xs = [pt[0] for pt in points]
    ys_idname = [pt[1] for pt in points]
    ys_idnum = [pt[2] for pt in points]
    plt.scatter(xs, ys_idnum)
    plt.title('Tag3')
    plt.show()


if __name__ == "__main__":

    
    rospy.init_node('uwb_sub_node', anonymous=True) 

    #rospy.Subscriber("/dwm1001/anchor/ttyUWB0", Anchor, partial(callback, tag_num="0"))
    #rospy.Subscriber("/dwm1001/anchor/ttyUWB1", Anchor, partial(callback, tag_num="1"))
    #rospy.Subscriber("/dwm1001/anchor/ttyUWB2", Anchor, partial(callback, tag_num="2"))
    rospy.Subscriber("/dwm1001/anchor/ttyUWB3", Anchor, partial(callback, tag_num="3"))

    rospy.spin() 
    rospy.signal_shutdown('rosbag file is finished')
    draw_scatter(points) 

     