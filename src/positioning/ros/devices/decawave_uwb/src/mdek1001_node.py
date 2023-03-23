#!/usr/bin/env python3.8

import rospy, time, serial, os, sys, random, argparse
import threading

from geometry_msgs.msg  import Pose
from geometry_msgs.msg  import PoseStamped
from std_msgs.msg       import Float64
from decawave_uwb.msg import uwb_anchor
from decawave_uwb.msg import Ranging

from dwm1001_apiCommands import DWM1001ApiCommands
            

class DWM1001DataReader:
       
    def __init__(self, serial_instance) :
        """
        Initialize the node, open serial port
        """

        # Init node
        # rospy.init_node('DWM1001_Active_{}'.format(serial_instance.split("/")[2]), anonymous=False)
        rospy.init_node('DWM1001/Data/Reader', anonymous=True)

        # Get port and tag name
        self.dwm_port = serial_instance.port
        self.tag_name = self.dwm_port.split("/")[2]
        self.use_network = False
        self.network = ""
        self.verbose = False
        
        # Set a ROS rate
        self.rate = rospy.Rate(10)
        
        # Empty dictionary to store topics being published
        self.topics = {}
        
        # Serial port settings
        self.serialPortDWM1001 = serial.Serial(
            port = self.dwm_port,
            baudrate = 115200,
            parity = serial.PARITY_ODD,
            stopbits = serial.STOPBITS_TWO,
            bytesize = serial.SEVENBITS
        )
    

    def main(self) :
        """
        Initialize port and dwm1001 api
        :param:
        :returns: none
        """
        DWM1001 = DWM1001ApiCommands(self.serialPortDWM1001, rata=10)
        DWM1001.is_Open()

        try:

            while not rospy.is_shutdown():
                # just read everything from serial port
                raw,  parsed_data= DWM1001.read()

                try:
                    self.publishTagPositions(parsed_data)

                except IndexError:
                    rospy.loginfo("Found index error in the network array! DO SOMETHING!")


        except KeyboardInterrupt:
            rospy.loginfo("Quitting DWM1001 Shell Mode and closing port, allow 1 second for UWB recovery")
            DWM1001.commands('RESET')
            DWM1001.commands('SINGLE_ENTER')

        finally:
            rospy.loginfo("Quitting, and sending reset command to dev board")
            # self.serialPortDWM1001.reset_input_buffer()
            DWM1001.commands('RESET')
            DWM1001.commands('SINGLE_ENTER')
            self.rate.sleep()
            raw,  parsed_data= DWM1001.read()
            if "reset" in raw:
                rospy.loginfo("succesfully closed ")
                DWM1001.close()


    def publishTagPositions(self, parsed_data):
        """
        Publish anchors and tag in topics using Tag and Anchor Object
        :param networkDataArray:  Array from serial port containing all informations, tag xyz and anchor xyz
        :returns: none
        """


        # If getting a tag position
        if "DIST" in parsed_data.anchor.identity.ID:

            # The number of elements should be 2 + 6*NUMBER_OF_ANCHORS + 5 (TAG POS)
            # number_of_anchors = DWM1001_API_COMMANDS.set_node_number(arrayData)
            number_of_anchors = parsed_data.anchor.ANNum

            for i in range(number_of_anchors) :

                # node_id = DWM1001_API_COMMANDS.set_node_id(arrayData, i)
                node_id = parsed_data.anchor.identity.ANserial
                
                first_time = False
                if node_id not in self.topics :
                    first_time = True
                    
                    self.topics[node_id] = rospy.Publisher(
                        '/dwm1001' + 
                        '/anchor/' + node_id + 
                        "/position", 
                        PoseStamped, 
                        queue_size=100
                    )
                    self.topics[node_id+"_dist"] = rospy.Publisher(
                        '/dwm1001' + 
                        '/tag/' + self.tag_name +
                        "/distance", 
                        Ranging, 
                        queue_size=100
                    )
                try :
                    p = PoseStamped()
                    p.header.stamp = rospy.Time.now()
                    p.header.frame_id = node_id
                    p.pose.position.x = parsed_data.anchor.pose.x[i]
                    p.pose.position.y = parsed_data.anchor.pose.y[i]
                    p.pose.position.z = parsed_data.anchor.pose.z[i]
                    p.pose.orientation.x = 0.0
                    p.pose.orientation.y = 0.0
                    p.pose.orientation.z = 0.0
                    p.pose.orientation.w = 1.0
                    self.topics[node_id].publish(p)
                except :
                    pass
                try :
                    dist = Ranging()
                    dist.header.stamp = p.header.stamp
                    dist.anchorId = node_id
                    dist.tagId = self.tag_name
                    dist.range = parsed_data.anchor.pose.distance[i]
                    # dist.range = float(arrayData[7+6*i])
                    self.topics[node_id+"_dist"].publish(dist)
                except :
                    pass

                if self.verbose or first_time :
                    rospy.loginfo("Anchor " + node_id + ": "
                                  + " x: "
                                  + str(p.pose.position.x)
                                  + " y: "
                                  + str(p.pose.position.y)
                                  + " z: "
                                  + str(p.pose.position.z))

            # Now publish the position of the tag itself
            if "POS" in parsed_data.tag.identity.ID:

                # Topic is now a tag with same name as node_id
                first_time = False
                if self.tag_name not in self.topics :
                    first_time = True
                    self.topics[self.tag_name] = rospy.Publisher('/dwm1001/tag/'+self.tag_name+"/position", PoseStamped, queue_size=100)
                p = PoseStamped()
                p.header.stamp = rospy.Time.now()  
                p.pose.position.x = parsed_data.tag.pose.x
                p.pose.position.y = parsed_data.tag.pose.y
                p.pose.position.z = parsed_data.tag.pose.z
                p.pose.orientation.x = 0.0
                p.pose.orientation.y = 0.0
                p.pose.orientation.z = 0.0
                p.pose.orientation.w = 1.0
                self.topics[self.tag_name].publish(p)

                if self.verbose or first_time :
                    rospy.loginfo("Tag " + self.tag_name + ": "
                                  + " x: "
                                  + str(p.pose.position.x)
                                  + " y: "
                                  + str(p.pose.position.y)
                                  + " z: "
                                  + str(p.pose.position.z))

if __name__ == '__main__':  
    parser = argparse.ArgumentParser()
    parser.add_argument("--port", type=str, help="Target device serial port")
    args = parser.parse_args()
    try:
        dwm1001 = DWM1001DataReader(args)
        dwm1001.main()
    except rospy.ROSInterruptException:
        pass