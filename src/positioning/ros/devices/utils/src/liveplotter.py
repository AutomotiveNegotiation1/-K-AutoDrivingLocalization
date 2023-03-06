#!/usr/bin/env python3.8
# coding=utf-8
#
# * Origincal Code
# * https://github.com/PulkitRustagi/UWB-Localization/blob/main/src/live_plotter.py
# * modified by PulkitRustagi
#

from __future__ import print_function
import tkinter

import matplotlib
matplotlib.use('TkAgg')


import rospy
import streamlit as st
import streamlit.components.v1 as components
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import pandas as pd
from sympy import legendre, plot
from html import *
import numpy as np
from collections import deque

class LivePlotter(object):
    """
    A helper class for creating live plotting matplotlib windows

    Args:
        object (_type_): _description_
    """
    def __init__(self, dash_bool=False, update_interval=1000, alpha=1.0, window_name=None, time_removal=None, lengend_loc='best',
                 font_size='x-large'):
        """
        Setup the live plotting window

        Args:
            update_interval (int, optional): How fast the animate window updates its values. Defaults to 1000.
            alpha (float, optional): The alpha value of lines added if there are many to make things a little easier to view. Defaults to 1.0.
            window_name (_type_, optional): The matplotlib window name. Defaults to None.
            time_removal (_type_, optional): How long before you want to remove the legend, None to keep it indefinitely. Defaults to None.
            lengend_loc (str, optional): The location of the legend as defined by matplotlib parameter choies. Defaults to 'best.
            font_size (_type_, optional): The font size of the legend as defined by metplotlib parmeter choices. Defaults to None.
        """
        self.que = deque(maxlen=10)
        self.fig = plt.figure()
        self.ax = self.fig.add_subplot(1, 1, 1)
        self.fig.tight_layout()
        self.dash = dash_bool
        
        # if window_name is not None:
        #     self.fig.canvas.set_window_title(window_name)
        
        self.update_interval = update_interval
        
        self.alpha = alpha
        self.data = dict()
        self.objects = dict()
        self.ani = None
        self.time_removal = time_removal
        self.legend_loc = lengend_loc
        self.font_size = font_size
        
    def add_data_point(self, mode, object_name, x, y):
        """
        Adds data to the live plotting window

        Args:
            object_name (_type_): The line object to add the data to. If it does not exist a new object will be created otherwise the data will just be appended to
            x (_type_): The x data to add
            y (_type_): The x data to add
        """
        
        if object_name not in self.data:
            self.data[object_name] = {
                "x": [],
                "y": []
            }
            if mode == 'anchor':
                line, = self.ax.plot([], [], '^', label=object_name, alpha=self.alpha)
                
            if mode == 'tag':
                line, = self.ax.plot([], [], 'or', label=object_name, alpha=self.alpha)
            
            self.objects[object_name] = line
                    
        if (len(self.data[object_name].get("x", 'No_Key')) == 10) and (len(self.data[object_name].get("y", 'No_Key')) == 10):
             self.data[object_name]["x"].pop(0)
             self.data[object_name]["y"].pop(0)
            
        self.data[object_name]["x"].append(x)
        self.data[object_name]["y"].append(y)
        
        print(self.data[object_name]["x"])
        self.objects[object_name].set_xdata(self.data[object_name]['x'])
        self.objects[object_name].set_ydata(self.data[object_name]['y'])       
        
    def func_animate(self, i):
        """
        The animotion function called at each interval step

        Args:
            i (_type_): The current index of the animate

        Returns:
            _type_: The objects to draw
        """
        try:
            if self.time_removal is None or i < self.time_removal:
                # if self.time_removal is not None:
                #     print(i)
                self.ax.legend(loc=self.legend_loc, fontsize=self.font_size)
            else:
                self.ax.legend_loc = None
            self.ax.relim()
            self.ax.autoscale_view()
        except ValueError:
            rospy.loginfo("Error graphing")
            
        return self.objects.values
    
    def show(self):
        """
        Shows and start the live plotter func loop
        """
        self.ani = FuncAnimation(self.fig, self.func_animate, interval=self.update_interval)
        plt.show()
        
        # st.title("Embed Matplotlib animation in Streamlit")
        # components.html(self.ani.to_jshtml(), height=1000)
        
        
        
if __name__ == '__main__':
    plotter = LivePlotter()
    plotter.show()