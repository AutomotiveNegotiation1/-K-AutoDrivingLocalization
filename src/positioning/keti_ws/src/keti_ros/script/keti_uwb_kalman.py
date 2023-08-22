#!/usr/bin/env python3.8
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

# coding=utf-8
#
# * Origincal Code
# * https://github.com/PulkitRustagi/UWB-Localization/blob/main/src/efk/kalman_filter.py
# * modified by PulkitRustagi
#
from __future__ import print_function

import numpy as np
from numpy.linalg import inv

class UWBKalmanFilter(object):
    def __init__(self, x, A, z, R):
        self.threshold = 100
        self.x = x
        self.P = 1 * np.eye(12)
        self.P[0:3,3:6] = 1 * np.eye(3)
        self.P[0:6,6:12] = 10 * np.ones((6,6))
        self.P[9:12, 9:12] = 100 * np.eye(3)
        self.A = A
        self.H = np.zeros((6,12))
        self.H[0:6, 0:6] = np.eye(6)
        self.R = R
        self.Q = 1e-4 * np.eye(12)
        self.I = None
        self.S = None
        self.K = None
        self.z = z


    def predict(self):

        self.xp = np.dot(self.A, self.x)
        # print("self.xp", self.xp.shape)
        # print("self.xp", self.xp)
        self.Pp = np.dot(np.dot(self.A, self.P), self.A.T) + self.Q
        # print("self.Pp", self.Pp.shape)
        # print("self.Pp", self.Pp)
        self.H = self.H.astype(np.float64)
        self.R = self.R.astype(np.float64)
        self.Pp = self.Pp.astype(np.float64)

    def update(self):
        self.K = np.dot(np.dot(self.Pp, self.H.T), inv(np.dot(np.dot(self.H, self.Pp), self.H.T) + self.R))
        # print("self.x", self.x.shape)
        # print("self.x", self.x)
        # print("self.z :", self.z.shape)
        # print("self.z :", self.z)
        # print("np.dot(self.H, self.xp) :", (np.dot(self.H, self.xp)).shape)
        # print("np.dot(self.H, self.xp) :", np.dot(self.H, self.xp))
        # print("result :", (self.z - np.dot(self.H, self.xp)).shape )
        # print("result :", self.xp + np.dot(self.K, (self.z - np.dot(self.H, self.xp))) )
        # print("result :", (self.xp + np.dot(self.K, (self.z - np.dot(self.H, self.xp)))).shape)
        self.x = self.xp + np.dot(self.K, (self.z - np.dot(self.H, self.xp)))
        self.P = self.Pp - np.dot(np.dot(self.K, self.H), self.Pp)

        return self.x


    def result(self):
        phi = np.arctan2(2 * (self.x[2] * self.x[3] + self.x[0] * self.x[1]), 1 - 2 * (self.x[1] ** 2 + self.x[2] ** 2))
        theta = -np.arcsin(2 * (self.x[1] * self.x[3] - self.x[0] * self.x[2]))
        psi = np.arctan2(2 * (self.x[1] * self.x[2] + self.x[0] * self.x[3]), 1 - 2 * (self.x[2] ** 2 + self.x[3] ** 2))

        return phi, theta, psi