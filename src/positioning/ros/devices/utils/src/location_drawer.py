#!/usr/bin/env python3.8
# coding=utf-8
#
# * Origincal Code
# * https://github.com/PulkitRustagi/UWB-Localization/blob/main/src/location_drawer.py
# * modified by PulkitRustagi
#

import tkinter

import matplotlib
matplotlib.use('TkAgg')

from dash import Dash, html, dcc


import time

import streamlit as st

import rospy
from nav_msgs.msg import Odometry
from geometry_msgs.msg  import PoseStamped
from liveplotter import LivePlotter
from geometry_msgs.msg import Point
import numpy as np

import sys
from utils.srv import deviceInfo
import threading

class PositionPlotter(object):
    """
    A class to help plot the position of Odometry data
    Args:
        object (_type_): _description_
    """

    def __init__(self, AN_infos=None):
        """
        Setup the PositionPlotter class
        Args:
            vehicle_name (str, optional): The name of the vehicle. Defaults to 'ketiCar'.
            position_links (_type_, optional): The Subscriver topic to plot in the window. If it is None then the topics are searched
            all topics ending with "uwb/ranging" (UKF position estimate), "uwb/filtered" (uwb positioning),
            "ground_truth/state" (ground truth state). Defaults to None.
        """           
        self.AN_infos = AN_infos
        self.start_s = None
        
        if self.AN_infos is None:
            self.AN_infos = []

            topic_names = rospy.get_published_topics()

            for i, (topic, _) in enumerate(topic_names):
                topic_list = '/dwm1001/anchor/'
                if topic_list in topic:
                    self.AN_infos.append(topic)

            rospy.loginfo(self.AN_infos)

        # self.live_plotter = LivePlotter(alpha=0.5, window_name="Location Drawer", time_removal=750, font_size='x-small')
        self.live_plotter = LivePlotter(algorithm='nomal', alpha=0.5, window_name="Location Drawer")
        # self.live_plotter.ax.set_aspect("equal")        
        self.AN_subscribers = dict()
        self.topic_list = '/dwm1001/anchor/'
        
        
        for AN_info in self.AN_infos:
            # print(position_link)
            self.AN_subscribers[AN_info] = rospy.Subscriber(AN_info, PoseStamped,
                                                               self.create_position_subscriber_func(AN_info.split('/')[3]))
        
        rospy.wait_for_service('device_info')
        devices = rospy.ServiceProxy('device_info', deviceInfo)
        self.TA_list = devices('UWB')
        self.TA_subscribers = dict()
    
        for dev in self.TA_list.TagID:
            self.TA_subscribers[dev] = rospy.Subscriber('/dwm1001/tag/{}/position'.format(dev.split('/')[2]), PoseStamped,
                                                        self.create_vehicle_position_subscriver_func(dev.split('/')[2]))
            
        app = Dash(__name__)

    def create_position_subscriber_func(self, name):
        """
        Helper factory function to create an PoseStamped and live plotter data adder
        Args:
            name (_type_): Name of the topic name
        Returns:
            _type_: return the custom pose addition function
        """
        def add_pose(msg):
            """
            Extracts the position x, y values from the PoseStamped msg and adds it to the live plotter
            Args:
                msg (None): The PoseStamped pose to process
            """
            # self.update_AN_position(msg.header.frame_id)
            x = msg.pose.position.x
            y = msg.pose.position.y

            self.live_plotter.add_data_point('anchor', name, x, y)

        return add_pose

    def create_vehicle_position_subscriver_func(self, name):
        """
        Deprecated
        Added vehicle position
        Args:
            msg (_type_): The PoseStamped pose
        """
        def add_pose(msg):
            tag_x = msg.pose.position.x
            tag_y = msg.pose.position.y
            self.live_plotter.add_data_point('tag', name, tag_x, tag_y)
        
        return add_pose
    
    # def update_AN_position(self, AN):        
    #     topic_names = rospy.get_published_topics()
    #     AN_list = [topic for (topic, _) in topic_names if self.topic_list in topic] 
    #     for AN in AN_list:
    #         rospy.wait_for_message(AN, )
    #     if AN == '':
            
        
    #     if len(AN_index) is 0:
    #         self.AN_infos.remove(AN_index)
    #         self.AN_subscribers[AN_index].unregister()
            
    #     else:
    #         pass
        
        
    #     if AN is self.AN_infos:
    #         AN_index = [topic for (topic, _) in topic_names if self.topic_list in topic if AN in topic] 
            
    #     print(":",len(AN_list), ":", len(self.AN_infos))
    #     set1, set2 = set(self.AN_infos), set(AN_list)
    #     if AN_index not in self.AN_infos:
    #         if len(AN_list) == len(self.AN_infos):
    #             self.AN_infos.remove(AN_index)
    #             self.AN_subscribers[AN_index].unregister()
    #             pass
    #         if len(self.AN_infos) < len(AN_list):
    #             pass
        
    #     if len(self.AN_infos) < len(AN_list):
    #         AN_info = set2 - set1
    #         self.AN_infos.append(AN_info)
    #         self.AN_subscribers[AN_info] = rospy.Subscriber(AN_info, PoseStamped,
    #                                                     self.create_position_subscriber_func(AN_info.split('/')[3]))
        
    #     if len(self.AN_infos) > len(AN_index):
    #         AN_info = set1 - set2
    #         self.AN_infos.remove(AN_info)
    #         self.AN_subscribers[AN_info].unregister()

        

    def run(self):
        """
        Run the plotter
        """
        # st.title("test")
        fig = self.live_plotter.show()

        # st.pyplot(fig)

if __name__ == '__main__':
    rospy.init_node("location_drawer_node")

    #myargv = rospy.myargv(argv=sys.argv)[1:]
    myargv=[]
    if len(myargv) == 0:
        myargv = None
        
    data_plotter = PositionPlotter(AN_infos=myargv)
    # position_links = []
    # data_plotter = PositionPlotter()
    data_plotter.run()
    rospy.spin()