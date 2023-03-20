#!/usr/bin/env python3.8
# coding=utf-8
#
# * Origincal Code
# * https://github.com/PulkitRustagi/UWB-Localization/blob/main/src/efk_uwb_localization.py
# * modified by PulkitRustagi

import numpy as np
import rospy
# import tf
from decawave_uwb.msg import Ranging
from nav_msgs.msg import Odometry
from visualization_msgs.msg import MarkerArray
from utils.srv import deviceInfo
from geometry_msgs.msg  import PoseStamped
from liveplotter import LivePlotter

from src.utils.src.ekf.fusion_kf import FusionEKF


class UWBLocalization(object):
    """
    A deprecated localization node using an EKF which estimates the position of the left and right tags seperately
    """

    def __init__(self, AN_infos=None):
        """
        Setups the EKF localization of the tags
        """
        
        
        self.AN_infos = AN_infos
        
        if self.AN_infos is None:
            self.AN_infos = []

            topic_names = rospy.get_published_topics()

            for i, (topic, _) in enumerate(topic_names):
                topic_list = '/dwm1001/anchor/'
                if topic_list in topic:
                    self.AN_infos.append(topic)
                    

            rospy.loginfo(self.AN_infos)
            
        self.live_plotter = LivePlotter(algorithm='ekf', alpha=0.5, window_name="Location Drawer")
        # self.live_plotter.ax.set_aspect("equal")        
        
        self.anchor_poses = dict()
        self.AN_subscribers = dict()
        for AN_info in self.AN_infos:
            # print(position_link)
            self.AN_subscribers[AN_info] = rospy.Subscriber(AN_info, PoseStamped,
                                                            self.create_anchor_subscriber_func(AN_info.split('/')[3]))

        rospy.wait_for_service('device_info')
        devices = rospy.ServiceProxy('device_info', deviceInfo)
        self.TA_list = devices('UWB')
        self.kalman_filter_tag = dict()
        self.TA_subscribers = dict()
        # self.kalman_filter = FusionEKF()
        for dev in self.TA_list.TagID:
            self.kalman_filter_tag[dev] = FusionEKF()
            self.TA_subscribers[dev] = rospy.Subscriber('/dwm1001/tag/{}/distance'.format(dev.split('/')[2]), Ranging, 
                                                        self.create_tag_subscriber_func(dev))
        
    def create_tag_subscriber_func(self, name):
        
        def add_ranging(msg):
            if msg.anchorId in self.anchor_poses:
                anchor_pose = self.anchor_poses[msg.anchorId]
                anchor_distance = msg.range
                
                for dev in self.TA_list.TagID:
                    self.kalman_filter_tag[dev].process_measurement(anchor_pose, anchor_distance)
                    tag_x = self.kalman_filter_tag[dev].kalman_filter.x[0]
                    tag_y = self.kalman_filter_tag[dev].kalman_filter.x[1]
                    self.live_plotter.add_data_point('tag', name, tag_x, tag_y)
                    
        return add_ranging

    def create_anchor_subscriber_func(self, name):
        
        def add_anchors(msg):
            # type: (MarkerArray) -> None
            """
            Function that handles the MarkerArray of anchor positions and updates the anchor pose dict
            @param msg: the MarkerArray topic message
            """
            x = msg.pose.position.x
            y = msg.pose.position.y
            z = msg.pose.position.z
            self.anchor_poses[name] = np.array([x, y, z])
            
            self.live_plotter.add_data_point('anchor', name, x, y)

        return add_anchors

    def run(self):
        """
        The step function that publishes the position information of the tags
        @return:
        """
        fig = self.live_plotter.show()
        # rate = rospy.Rate(60)

        # while not rospy.is_shutdown():
        #     self.pose.pose.pose.position.x = self.kalman_filter_tag_0.kalman_filter.x[0]
        #     self.pose.pose.pose.position.y = self.kalman_filter_tag_0.kalman_filter.x[1]
        #     self.pose.pose.pose.position.z = self.kalman_filter_tag_0.kalman_filter.x[2]

        #     self.pose.twist.twist.linear.x = self.kalman_filter_tag_0.kalman_filter.x[3]
        #     self.pose.twist.twist.linear.y = self.kalman_filter_tag_0.kalman_filter.x[4]
        #     self.pose.twist.twist.linear.z = self.kalman_filter_tag_0.kalman_filter.x[5]

        #     # r = self.change_pose_ref(self.pose, '/right_tag')

        #     # if r:
        #     self.estimated_pose_tag_0.publish(self.pose)

        #     self.pose.pose.pose.position.x = self.kalman_filter_tag_1.kalman_filter.x[0]
        #     self.pose.pose.pose.position.y = self.kalman_filter_tag_1.kalman_filter.x[1]
        #     self.pose.pose.pose.position.z = self.kalman_filter_tag_1.kalman_filter.x[2]

        #     self.pose.twist.twist.linear.x = self.kalman_filter_tag_1.kalman_filter.x[3]
        #     self.pose.twist.twist.linear.y = self.kalman_filter_tag_1.kalman_filter.x[4]
        #     self.pose.twist.twist.linear.z = self.kalman_filter_tag_1.kalman_filter.x[5]

        #     # r = self.change_pose_ref(self.pose, '/left_tag')

        #     # if r:
        #     self.estimated_pose_tag_1.publish(self.pose)

        #     rate.sleep()


if __name__ == "__main__":
    rospy.init_node("ekf_uwb_localization")

    myargv=[]
    if len(myargv) == 0:
        myargv = None
        
    loc = UWBLocalization(AN_infos=myargv)
    loc.run()

    rospy.spin()

