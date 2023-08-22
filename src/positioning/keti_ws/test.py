###############################################################################
#
# Copyright (C) 2023 - 2028 KETI, All rights reserved.
#                           (Korea Electronics Technology Institute)
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# Use of the Software is limited solely to applications:
# (a) running for Korean Government Project, or
# (b) that interact with KETI project/platform.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
# KETI BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
# WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
# OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
# Except as contained in this notice, the name of the KETI shall not be used
# in advertising or otherwise to promote the sale, use or other dealings in
# this Software without prior written authorization from KETI.
#
##############################################################################

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
