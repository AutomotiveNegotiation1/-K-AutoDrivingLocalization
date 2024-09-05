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
import matplotlib.pyplot as plt
from localizer_dwm1001.msg import Anchor, Tag
from geometry_msgs.msg import TransformStamped
from math import acos, atan2, pi, cos, sin
from matplotlib import animation
import cmath
from collections import deque

rospy.init_node('localization_demo')

# Define a class to handle each subscriber
class MySubscriber(object):
    def __init__(self, topic, msg_type):
        self.data_queue = deque(maxlen=10)
        self.sub = rospy.Subscriber(topic, msg_type, self.callback)

    def callback(self, data):
        self.data_queue.append(data)

    def receive(self, timeout=None):
        if len(self.data_queue) > 0:
            return self.data_queue.popleft()
        else:
            return None

# Publishers and subscribers
subAnchor1 = MySubscriber('/dwm1001/anchor/tag1', Anchor)
subAnchor2 = MySubscriber('/dwm1001/anchor/tag2', Anchor)
subAnchor3 = MySubscriber('/dwm1001/anchor/tag3', Anchor)
subAnchor4 = MySubscriber('/dwm1001/anchor/tag4', Anchor)

subTag1 = MySubscriber('/dwm1001/tag1', Tag)
subTag2 = MySubscriber('/dwm1001/tag2', Tag)
subTag3 = MySubscriber('/dwm1001/tag3', Tag)
subTag4 = MySubscriber('/dwm1001/tag4', Tag)

subAnchor = [subAnchor1, subAnchor2, subAnchor3, subAnchor4]
subTag = [subTag1, subTag2, subTag3, subTag4]

# Initialize plot
# fig, (ax1, ax2, ax3) = plt.subplots(1, 3)
fig, (ax1, ax2, ax3) = plt.subplots(1, 3, figsize=(15, 5))

h = ax1.plot([], [], 'r', linewidth=3, marker='o', markersize=3)
ax1.set_aspect('equal')

# Anchor info
xa = [0, 1.67, 0, 1.67]
ya = [4.22, 4.22, 0, 0]
anchorId = ['991B', '439D', '9B8F', '89A4']

gt = [0.3, 3.86]

a = 3.405
b = 1.485
c = 3.367

A_rad = acos((a ** 2 + b ** 2 - c ** 2) / (2 * a * b))  # A in radians
A_deg = np.rad2deg(A_rad)  # Convert A from radians to degrees

A_deg = 0
# xt_b = [-0.5, 0.5, -0.5, 0.5]
# yt_b = [0.5, 0.5, -0.5, -0.5]

xt_b = [0.14, 0.14, -0.14, -0.14]
yt_b = [0.14, -0.14, 0.14, -0.14]

xt_b_center = sum(xt_b)
yt_b_center = sum(yt_b)
angles_from_heading = np.arctan2(yt_b, xt_b)

# gt = [0.287, 3.823]
tag_pos_b = np.array(xt_b) + 1j * np.array(yt_b)

# Variables for storing all estimated Xt_e and Yt_e
Xt_e_all = []
Yt_e_all = []
# Before the main loop
dist = np.zeros((len(anchorId), len(subTag)))

# Main loop
for i in range(100):
    print(i)
    plt.figure(1)
    xt = []
    yt = []
    tagId = []

    for p in range(len(subTag)):
        msgAnchor = subAnchor[p].receive(1)
        msgTag = subTag[p].receive(1)

        # tag information
        xt.append(msgTag.x)
        yt.append(msgTag.y)
        tagId.append(msgTag.header.frame_id)

        # reorder according to anchorId
        for n in range(len(anchorId)):
            index = [i for i, x in enumerate(msgAnchor.id) if x == anchorId[n]]
            if index: # if there is a match
                dist[n][p] = msgAnchor.distanceFromTag[index[0]]

    plt.xlim([-1, 3])
    plt.ylim([-1, 5])
    plt.plot(xa, ya, marker='^', linestyle='', color='b')
    text_pos = [[i, j+0.1] for i, j in zip(xa, ya)]
    for pos, txt in zip(text_pos, anchorId):
        plt.text(pos[0], pos[1], txt)

    plt.text(np.mean(xa), 4.7, 'ljw')
    plt.text(np.mean(xa), 0.1, 'jsh')

    Li = 1
    A = np.zeros((len(anchorId) * (len(anchorId)-1) * 16, len(anchorId) * 2))
    Y = np.zeros((len(anchorId) * (len(anchorId)-1) * 16, 1))

    # Generate Matrix
    for n in range(len(anchorId)):
        for m in range(len(anchorId)):
            if n != m:
                for p in range(4):
                    for q in range(4):
                        if p != q:
                            A[Li-1, 2*p:2*p+2] = [2*(xa[n]-xa[m]), 2*(ya[n]-ya[m])]
                            A[Li-1, 2*q:2*q+2] = [2*(xa[n]-xa[m]), 2*(ya[n]-ya[m])]
                            Y[Li-1, 0] = dist[n][p]**2 + dist[n][q]**2 - dist[m][p]**2 - dist[m][q]**2 - 2*xa[n]**2 + 2*xa[m]**2 - 2*ya[n]**2 + 2*ya[m]**2
                            Li += 1

    # Calculate Tag position
    Res = np.linalg.inv(A.T @ A) @ A.T @ Y
    Xt_e = -Res[::2]
    Yt_e = -Res[1::2]

    # Estimated Center Position
    Xt_c_e = np.mean(Xt_e)
    Yt_c_e = np.mean(Yt_e)

    # Estimated Heading
    tag_pos_est = Xt_e + 1j*Yt_e
    tag_arrow_est = np.sum((tag_pos_est-Xt_c_e-1j*Yt_c_e) * np.exp(-1j*angles_from_heading))
    heading_est = cmath.phase(tag_arrow_est)
    tag_pos_g = tag_pos_b * np.exp(1j*(heading_est))+Xt_c_e+1j*Yt_c_e

    Xt_e_all.append(msgTag.x)
    Xt_e_all.append(msgTag.y)

    # If i is a multiple of 10, calculate the mean
    if i % 10 == 0:
        Xt_e_mean = np.mean(Xt_e_all)  # Calculate the mean of each row of Xt_e_all
        Yt_e_mean = np.mean(Yt_e_all)  # Calculate the mean of each row of Yt_e_all

        # Estimated Center Position
        Xt_c_e_mean = np.mean(Xt_e_mean)
        Yt_c_e_mean = np.mean(Yt_e_mean)
        ##############################

        xt_c_e_mean_error = gt - np.array([Xt_c_e_mean, Yt_c_e_mean])

        tag_pos_est_mean = Xt_e_mean + 1j*Yt_e_mean
        tag_arrow_est_mean = np.sum((tag_pos_est_mean-Xt_c_e_mean-1j*Yt_c_e_mean) * np.exp(-1j*angles_from_heading))
        heading_est_mean = atan2(tag_arrow_est_mean.imag, tag_arrow_est_mean.real)
        tag_pos_g_mean = tag_pos_b*np.exp(1j*(heading_est_mean))+Xt_c_e_mean+1j*Yt_c_e_mean

        xtL_mean = tag_pos_g_mean.real
        ytL_mean = tag_pos_g_mean.imag

        ax3.set_xlim([-1, 3])
        ax3.set_ylim([-1, 5])
        ax3.plot(xa, ya, '^b')

        # plot(ax3, 0.3, 3.86, 'ro')
        tag = [xt[0], yt[0]]
        anchor = [xa[0], ya[0]]
        # plot([xt[0], xa[0]], [yt[0], ya[0]], 'm-')

        # text_pos = [[i, j+0.1] for i, j in zip(xa, ya)]
        for pos, txt in zip(text_pos, anchorId):
            ax3.text(pos[0], pos[1], txt)

        ax3.text(np.mean(xa), 4.7, '이재원 연구원')
        ax3.text(np.mean(xa), 0.1, '장성현 책임')

        head = -A_deg*pi/180

        ax3.quiver(Xt_c_e_mean, Yt_c_e_mean, np.cos(heading_est_mean), np.sin(heading_est_mean), color='g')
        ax3.quiver(Xt_c_e_mean, Yt_c_e_mean, np.cos(head), np.sin(head), color='r')

        ax3.plot(xtL_mean[0], ytL_mean[0], '*m')
        ax3.plot(xtL_mean[1], ytL_mean[1], 'oc')
        ax3.plot(xtL_mean[2], ytL_mean[2], '^r')
        ax3.plot(xtL_mean[3], ytL_mean[3], 'ob')


    # Plotting on ax1
    ax1.plot(xa, ya, marker='^', linestyle='', color='b')
    ax1.plot(Xt_e[0], Yt_e[0], marker='o', linestyle='', color='m')
    ax1.plot(Xt_e[1], Yt_e[1], marker='^', linestyle='', color='c')
    ax1.plot(Xt_e[2], Yt_e[2], marker='>', linestyle='', color='y')
    ax1.plot(Xt_e[3], Yt_e[3], marker='*', linestyle='', color='k')
    ax1.plot(Xt_c_e, Yt_c_e, marker='x', linestyle='', color='r')
    ax1.arrow(Xt_c_e, Yt_c_e, 0.5 * cos(heading_est), 0.5*sin(heading_est), head_width=0.05, color='r')
    ax1.set_xlim([-1, 3])
    ax1.set_ylim([-1, 5])

    # Plotting on ax2
    ax2.plot(xa, ya, marker='^', linestyle='', color='b')
    ax2.plot(xt[0], yt[0], marker='o', linestyle='', color='m')
    ax2.plot(xt[1], yt[1], marker='^', linestyle='', color='c')
    ax2.plot(xt[2], yt[2], marker='>', linestyle='', color='y')
    ax2.plot(xt[3], yt[3], marker='*', linestyle='', color='k')
    ax2.plot(np.mean(xt), np.mean(yt), marker='x', linestyle='', color='g')
    # ax2.arrow(np.mean(xt), np.mean(yt), 0.5*cos(heading), 0.5*sin(heading), head_width=0.05, color='g')
    ax2.set_xlim([-1, 3])
    ax2.set_ylim([-1, 5])

    # Plotting on ax3
    ax3.plot(xa, ya, marker='^', linestyle='', color='b')
    ax3.plot(np.mean(Xt_e_all), np.mean(Yt_e_all), marker='x', linestyle='', color='r')
    # ax3.arrow(np.mean(Xt_e_all), np.mean(Yt_e_all), 0.5*cos(np.mean(heading_est_all)), 0.5*sin(np.mean(heading_est_all)), head_width=0.05, color='r')
    ax3.set_xlim([-1, 3])
    ax3.set_ylim([-1, 5])

    plt.pause(0.01)


