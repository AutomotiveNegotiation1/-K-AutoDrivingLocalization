#!/usr/bin/env python3.8
import os
import rospy
from utils.srv import deviceInfo

rospy.wait_for_service('device_info')
devices = rospy.ServiceProxy('device_info', deviceInfo)
devs = devices('UWB')
for dev in devs.TagID:
    test= os.system("rosrun decawave_uwb mdek1001_node.py --port {} &".format(dev))
        
        