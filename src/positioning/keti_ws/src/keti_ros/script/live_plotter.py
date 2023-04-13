#!/usr/bin/env python3.8
from __future__ import print_function

import numpy as np
import tkinter as tk
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure
import matplotlib
matplotlib.use('TkAgg')

# from keti_fusion import Sensor_fusion


class LivePlotter(object):
    """
    A helper class for creating live plotting matplotlib windows
    """

    def __init__(self, master, sensor_fusion):
        self.master = master
        self.sensor_fusion = sensor_fusion
        self.fig = Figure()

        self.ax1 = self.fig.add_subplot(121)  # 첫 번째 서브플롯
        # self.ax2 = self.fig.add_subplot(132)  # 두 번째 서브플롯
        self.ax3 = self.fig.add_subplot(122)  # 세 번째 서브플롯
        
        self.ax1.set_title('1st UWB pos graph')
        # self.ax2.set_title('2nd IMU pos graph')
        self.ax3.set_title('3rd Fusion pos graph')
        
        self.ax1.grid(True)
        # self.ax2.grid(True)
        self.ax3.grid(True)
        
        x_ticks_ax1 = np.arange(-1, 3, 0.1)
        y_ticks_ax1 = np.arange(-2, 7, 0.1)
        self.ax1.set_xticks(x_ticks_ax1)
        self.ax1.set_yticks(y_ticks_ax1)  

        # x_ticks_ax2 = np.arange(-1, 3, 0.1)
        # y_ticks_ax2 = np.arange(-2, 7, 0.1)
        # self.ax2.set_xticks(x_ticks_ax2)
        # self.ax2.set_yticks(y_ticks_ax2)
        
        x_ticks_ax3 = np.arange(-1, 3, 0.1)
        y_ticks_ax3 = np.arange(-2, 7, 0.1)
        self.ax3.set_xticks(x_ticks_ax3)
        self.ax3.set_yticks(y_ticks_ax3)      

        self.line1_o, = self.ax1.plot([], [], 'o')
        # self.line2_o, = self.ax2.plot([], [], 'o')
        self.line3_o, = self.ax3.plot([], [], 'o')
        self.line1_up, = self.ax1.plot([], [], '^')
        # self.line2_up, = self.ax2.plot([], [], '^')
        self.line3_up, = self.ax3.plot([], [], '^')
        self.line1_gt, = self.ax1.plot(0.38, 4.0, 'r>')
        # self.line2_gt, = self.ax2.plot(0.38, 4.0, 'r>')
        self.line3_gt, = self.ax3.plot(0.38, 4.0, 'r>')

        self.canvas = FigureCanvasTkAgg(self.fig, master=self.master)
        self.canvas.get_tk_widget().pack(side=tk.TOP, fill=tk.BOTH, expand=True)
        
    def update_color(self, id):
        self.color = None
        if id == 'DWM1001_tag1':
            self.color = 'tab:green'
        if id == 'DWM1001_tag2':
            self.color = 'tab:purple'
        if id == 'DWM1001_tag3':
            self.color = 'tab:pink'
        if id == 'DWM1001_tag4':
            self.color = 'tab:cyan'
        print(self.color)
        return self.color
            
            
        
        
        
    def update_plot(self):
        id_1, x_data_1, y_data_1 = self.sensor_fusion.get_anchor_data()
        id_2, x_data_2, y_data_2 = self.sensor_fusion.get_uwb_data()
#        id_3, x_data_3, y_data_3 = self.sensor_fusion.get_imu_data()
        id_4, x_data_4, y_data_4 = self.sensor_fusion.get_fusion_data()
        self.line1_o.set_data(x_data_2, y_data_2)
        self.line1_o.set_color(self.update_color(id_2))
#        self.line2_o.set_data(x_data_3, y_data_3)
        # self.line1_o.set_label(id_2)
        self.line3_o.set_data(x_data_4, y_data_4)
        # self.line1_o.set_label(id_2)
        
        self.line1_up.set_data(x_data_1, y_data_1)
#        self.line2_up.set_data(x_data_1, y_data_1)
        self.line3_up.set_data(x_data_1, y_data_1)

        self.ax1.relim()
        self.ax1.autoscale_view()
#        self.ax2.relim()
#        self.ax2.autoscale_view()
        self.ax3.relim()
        self.ax3.autoscale_view()

        self.canvas.draw()
        self.master.after(100, self.update_plot)
