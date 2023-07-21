import rospy
import numpy as np
import plotly.graph_objects as go
import streamlit as st
from geometry_msgs.msg import Quaternion

class Plotting:
    def __init__(self):
        rospy.init_node("fusion_plot_node")
        rospy.Subscriber("/fusion/positioning", Quaternion, self.poscallback)
        self.xt_b = np.array([-0.09, 0.09, -0.09, 0.09])
        self.yt_b = np.array([0.12, 0.12, -0.12, -0.12])
        self.tag_pos_b = self.xt_b + 1j * self.yt_b
        self.x = 0
        self.y = 0
        self.heading = 0

        # Plotly Graph Object
        self.fig = go.Figure()
        self.plot = st.plotly_chart(self.fig)

    def poscallback(self, msg):
        self.x = msg.x
        self.y = msg.y
        self.heading = msg.w

        # Update Plot
        self.update_plot()

    def update_plot(self):
        tag_pos_est = np.array([self.tag_pos_b * np.exp(1j * self.heading) + self.x + 1j * self.y])
        tag_e = np.array([[np.real(tag), np.imag(tag)] for tag in tag_pos_est]).T

        self.fig = go.Figure()

        for i in range(4):
            self.fig.add_trace(go.Scatter(x=[tag_e[i][0]], y=[tag_e[i][1]], mode='markers'))
        
        self.plot.plotly_chart(self.fig)

if __name__ == '__main__':
    fusion = Plotting()
    rospy.spin()
