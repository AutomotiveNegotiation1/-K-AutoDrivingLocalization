#!/usr/bin/env python3.8
import os
import rospy
from utils.srv import deviceInfo

rospy.wait_for_service('device_info')
devices = rospy.ServiceProxy('device_info', deviceInfo)
devs = devices('IMU')
for dev in devs.TagID:
    print(dev)
    test= os.system("rosrun ublox_f9r ublox_f9r_node.py --port {} &".format(dev))
        
        