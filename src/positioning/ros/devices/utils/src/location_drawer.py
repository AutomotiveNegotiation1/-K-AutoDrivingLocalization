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


import streamlit as st

import rospy
from nav_msgs.msg import Odometry
from geometry_msgs.msg  import PoseStamped
from liveplotter import LivePlotter
from geometry_msgs.msg import Point
import numpy as np

import sys

class PositionPlotter(object):
    """
    A class to help plot the position of Odometry data

    Args:
        object (_type_): _description_
    """
    
    def __init__(self, vehicle_name='ketiCar', position_links=None):
        """
        Setup the PositionPlotter class

        Args:
            vehicle_name (str, optional): The name of the vehicle. Defaults to 'ketiCar'.
            position_links (_type_, optional): The Subscriver topic to plot in the window. If it is None then the topics are searched
            all topics ending with "uwb/ranging" (UKF position estimate), "uwb/filtered" (uwb positioning),
            "ground_truth/state" (ground truth state). Defaults to None.
        """
        print("2:", position_links)
        if position_links is None:
            position_links = []
            
            topic_names = rospy.get_published_topics()
            
            for (topic, _) in topic_names:
                if topic.endswith('/dwm1001/anchor/AN0/position') or topic.endswith('/dwm1001/anchor/AN1/position') or topic.endswith('/dwm1001/anchor/AN2/position'):
                    print(topic)
                    position_links.append(topic)
            
            rospy.loginfo(position_links)
            
        # self.live_plotter = LivePlotter(alpha=0.5, window_name="Location Drawer", time_removal=750, font_size='x-small')
        self.live_plotter = LivePlotter(dash_bool=True, alpha=0.5, window_name="Location Drawer")
        self.live_plotter.ax.set_aspect("equal")
        
        self.vehicle_name = vehicle_name
        
        self.vehicle_position_topic = '/dwm1001/tag/ttyACM3/position'
        
        self.vehicle_positon_sub = rospy.Subscriber(self.vehicle_position_topic, PoseStamped, self.add_vehicle_pose)
        
        self.subscribers = dict()
        
        for position_link in position_links:
            print(position_link)
            self.subscribers[position_link] = rospy.Subscriber(position_link, PoseStamped,
                                                               self.create_position_subscriber_func(position_link.split('/')[3]))
            
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
            x = msg.pose.position.x
            y = msg.pose.position.y
            
            self.live_plotter.add_data_point('anchor', name, x, y)
        
        return add_pose
    
    def add_vehicle_pose(self, msg):
        """
        Deprecated
        Added vehicle position

        Args:
            msg (_type_): The PoseStamped pose
        """
        tag_x = msg.pose.position.x
        tag_y = msg.pose.position.y
        self.live_plotter.add_data_point('tag', self.vehicle_name, tag_x, tag_y)
        
    def run(self):
        """
        Run the plotter
        """
        self.live_plotter.show()
        
if __name__ == '__main__':
    rospy.init_node("location_drawer_node")
    
    #myargv = rospy.myargv(argv=sys.argv)[1:]
    myargv=[]
    if len(myargv) == 0:
        myargv = None
    
    print("1:", myargv)
    data_plotter = PositionPlotter(position_links=myargv)
    # position_links = []
    # data_plotter = PositionPlotter()
    data_plotter.run()
    rospy.spin()

