#!/usr/bin/env python3.8
# coding=utf-8
#
# * Origincal Code
# * https://github.com/PulkitRustagi/UWB-Localization/blob/main/src/efk/fusion_ekf.py
# * modified by PulkitRustagi
import time

import numpy as np

from ekf.ekf_kalman_filter import ExtendedKalmanFilter


class FusionEKF:
    def __init__(self):
        self.ekf = ExtendedKalmanFilter()

        self.uwb_noise_var = 0.01  # UWB measurement noise variance
        self.min_imu_data = 50  # Minimum number of IMU data points for noise estimation

        self.imu_data_buffer = []

    def estimate_process_noise(self, imu_data):
        self.imu_data_buffer.append(imu_data)

        if len(self.imu_data_buffer) >= self.min_imu_data:
            acc_data = np.array([data['acceleration'] for data in self.imu_data_buffer])
            gyro_data = np.array([data['angular_velocity'] for data in self.imu_data_buffer])

            acc_noise_var = np.var(acc_data, axis=0)
            gyro_noise_var = np.var(gyro_data, axis=0)

            self.ekf.Q[:3, :3] = np.diag(acc_noise_var)
            self.ekf.Q[3:6, 3:6] = np.diag(gyro_noise_var)

            self.imu_data_buffer = []

    def process_imu_data(self, imu_data):
        dt = imu_data['dt']
        acceleration = imu_data['acceleration']
        angular_velocity = imu_data['angular_velocity']
        orientation = imu_data['orientation']

        self.estimate_process_noise(imu_data)

        self.ekf.predict(dt, acceleration, angular_velocity, orientation)

    def process_uwb_data(self, uwb_data):
        anchor_position = uwb_data['anchor_position']
        distance = uwb_data['distance']

        # Update the measurement covariance matrix
        self.ekf.R = np.array([[self.uwb_noise_var]])

        self.ekf.update(anchor_position, distance)

    def get_state(self):
        return self.ekf.x

    def get_covariance(self):
        return self.ekf.P