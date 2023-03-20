import numpy as np

class ExtendedKalmanFilter:
    def __init__(self):
        self.x = np.zeros(6)  # State vector [x, y, z, vx, vy, vz]
        self.P = np.eye(6) * 1e-3  # State covariance matrix
        self.F = np.eye(6)  # State transition matrix
        self.Q = np.eye(6) * 1e-3  # Process noise covariance matrix
        self.R = np.array([[0.01]])  # Measurement noise covariance matrix
        self.H = np.zeros((1, 6))  # Measurement matrix

    def predict(self, dt, acceleration, angular_velocity, orientation):
        # Update state transition matrix F based on dt
        # self.F[:3, 3:] = np.eye(3) * dt
        self.F= np.array([
            [1, 0, 0, dt, 0, 0],
            [0, 1, 0, 0, dt, 0],
            [0, 0, 1, 0, 0, dt],
            [0, 0, 0, 1, 0, 0],
            [0, 0, 0, 0, 1, 0],
            [0, 0, 0, 0, 0, 1]
        ])

        # Update state and covariance using the process model
        self.x = self.motion_model(self.x, acceleration, dt, orientation)
        self.P = self.F @ self.P @ self.F.T + self.Q

    def update(self, anchor_position, distance):
        # Calculate expected distance
        expected_distance = np.linalg.norm(self.x[:3] - anchor_position)

        # Calculate the measurement matrix H
        self.H[0, :3] = (self.x[:3] - anchor_position) / expected_distance

        # Calculate Kalman gain
        S = self.H @ self.P @ self.H.T + self.R
        K = self.P @ self.H.T @ np.linalg.inv(S)

        # Update state and covariance
        y = np.array([distance - expected_distance])  # Wrap the value in a 1D numpy array
        self.x = self.x + K @ y
        self.P = (np.eye(6) - K @ self.H) @ self.P
        
    def quaternion_to_rotation_matrix(self, q):
        """
        Convert a quaternion to a 3x3 rotation matrix.
        """
        q0, q1, q2, q3 = q
        R = np.array([[1 - 2 * (q2**2 + q3**2), 2 * (q1 * q2 - q0 * q3), 2 * (q1 * q3 + q0 * q2)],
                      [2 * (q1 * q2 + q0 * q3), 1 - 2 * (q1**2 + q3**2), 2 * (q2 * q3 - q0 * q1)],
                      [2 * (q1 * q3 - q0 * q2), 2 * (q2 * q3 + q0 * q1), 1 - 2 * (q1**2 + q2**2)]])
        return R

    def motion_model(self, x, acceleration, dt, orientation):
        orientation_matrix = self.quaternion_to_rotation_matrix(orientation)

        # position = x[:3] + x[3:] * dt + 0.5 * orientation_matrix @ acceleration * dt**2
        # velocity = x[3:] + orientation_matrix @ acceleration * dt
        velocity = x[3:] + orientation_matrix @ acceleration * dt
        position = x[:3] + velocity * dt
        return np.hstack((position, velocity))