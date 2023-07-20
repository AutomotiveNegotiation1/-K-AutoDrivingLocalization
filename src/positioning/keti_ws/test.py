import rospy
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
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

        self.fig, self.ax = plt.subplots()
        self.ax.set_aspect('equal')
        self.circles = []
        self.quiver = None

        self.animation = FuncAnimation(self.fig, self.update, init_func=self.init_plot, blit=True)

    def init_plot(self):
        self.ax.clear()
        self.ax.set_xlim(-10, 10)
        self.ax.set_ylim(-10, 10)
        self.circles = []
        self.quiver = self.ax.quiver(0, 0, 0, 0, color='r', scale=1, scale_units='xy', angles='xy')
        return self.circles + [self.quiver]

    def poscallback(self, msg):
        self.x = msg.x
        self.y = msg.y
        self.heading = msg.w

    def update(self, frame):
        tag_pos_est = np.array([self.tag_pos_b * np.exp(1j * self.heading) + self.x + 1j * self.y])
        tag_e = np.array([[np.real(tag), np.imag(tag)] for tag in tag_pos_est]).T

        self.circles = []
        self.circles.append(self.ax.plot(tag_e[0][0], tag_e[0][1], 'ro')[0])
        self.circles.append(self.ax.plot(tag_e[1][0], tag_e[1][1], 'r*')[0])
        self.circles.append(self.ax.plot(tag_e[2][0], tag_e[2][1], 'rv')[0])
        self.circles.append(self.ax.plot(tag_e[3][0], tag_e[3][1], 'r^')[0])

        self.quiver.set_offsets([self.x, self.y])
        self.quiver.set_UVC(np.cos(self.heading), np.sin(self.heading))

        return self.circles + [self.quiver]

    def run(self):
        plt.show()

if __name__ == '__main__':
    fusion = Plotting()
    fusion.run()
    rospy.spin()
