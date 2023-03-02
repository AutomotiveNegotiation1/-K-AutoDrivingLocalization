#!/usr/bin/env python3

import rospy, time, serial, os, sys, random
import argparse

from geometry_msgs.msg  import Pose
from geometry_msgs.msg  import PoseStamped
from std_msgs.msg       import Float64
from decawave_uwb.msg import uwb_anchor

from dwm1001_apiCommands import DWM1001_API_COMMANDS

class dwm1001_localizer:
       
    def __init__(self, serial_instance) :
        """
        Initialize the node, open serial port
        """

        # Init node
        rospy.init_node('DWM1001_Active_{}'.format(random.randint(0,100000)), anonymous=False)

        # Get port and tag name
        self.dwm_port = serial_instance.port
        self.tag_name = self.dwm_port.split("/")[2]
        self.use_network = False
        self.network = ""
        self.verbose = False
        
        # Set a ROS rate
        self.rate = rospy.Rate(1)
        
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

        # close the serial port in case the previous run didn't closed it properly
        self.serialPortDWM1001.close()
        # sleep for one sec
        time.sleep(1)
        # open serial port
        self.serialPortDWM1001.open()

        # check if the serial port is opened
        if(self.serialPortDWM1001.isOpen()):
            rospy.loginfo("Port opened: "+ str(self.serialPortDWM1001.name) )
            # start sending commands to the board so we can initialize the board
            self.initializeDWM1001API()
            # give some time to DWM1001 to wake up
            time.sleep(2)
            # send command lec, so we can get positions is CSV format
            self.serialPortDWM1001.write(DWM1001_API_COMMANDS.LEC)
            self.serialPortDWM1001.write(DWM1001_API_COMMANDS.SINGLE_ENTER)
            rospy.loginfo("Reading DWM1001 coordinates")
        else:
            rospy.loginfo("Can't open port: "+ str(self.serialPortDWM1001.name))

        try:

            while not rospy.is_shutdown():
                # just read everything from serial port
                serialReadLine = self.serialPortDWM1001.read_until().decode('utf-8')

                try:
                    self.publishTagPositions(serialReadLine)

                except IndexError:
                    rospy.loginfo("Found index error in the network array! DO SOMETHING!")



        except KeyboardInterrupt:
            rospy.loginfo("Quitting DWM1001 Shell Mode and closing port, allow 1 second for UWB recovery")
            self.serialPortDWM1001.write(DWM1001_API_COMMANDS.RESET)
            self.serialPortDWM1001.write(DWM1001_API_COMMANDS.SINGLE_ENTER)

        finally:
            rospy.loginfo("Quitting, and sending reset command to dev board")
            # self.serialPortDWM1001.reset_input_buffer()
            self.serialPortDWM1001.write(DWM1001_API_COMMANDS.RESET)
            self.serialPortDWM1001.write(DWM1001_API_COMMANDS.SINGLE_ENTER)
            self.rate.sleep()
            serialReadLine = self.serialPortDWM1001.read_until()
            if "reset" in serialReadLine:
                rospy.loginfo("succesfully closed ")
                self.serialPortDWM1001.close()


    def publishTagPositions(self, serialData):
        """
        Publish anchors and tag in topics using Tag and Anchor Object
        :param networkDataArray:  Array from serial port containing all informations, tag xyz and anchor xyz
        :returns: none
        """

        arrayData = [x.strip() for x in serialData.strip().split(',')]

        # If getting a tag position
        if "DIST" in arrayData[0] :

            # The number of elements should be 2 + 6*NUMBER_OF_ANCHORS + 5 (TAG POS)
            number_of_anchors = int((len(arrayData) - 7)/6)

            for i in range(number_of_anchors) :

                node_id = arrayData[2+6*i]
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
                        '/to/anchor/' + node_id +
                        "/distance", 
                        Float64, 
                        queue_size=100
                    )
                try :
                    p = PoseStamped()
                    p.header.stamp = rospy.Time.now()
                    p.pose.position.x = float(arrayData[4+6*i])
                    p.pose.position.y = float(arrayData[5+6*i])
                    p.pose.position.z = float(arrayData[6+6*i])
                    p.pose.orientation.x = 0.0
                    p.pose.orientation.y = 0.0
                    p.pose.orientation.z = 0.0
                    p.pose.orientation.w = 1.0
                    self.topics[node_id].publish(p)
                except :
                    pass
                try :
                    dist = float(arrayData[7+6*i])
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
            if "POS" in arrayData[-5] :

                # Topic is now a tag with same name as node_id
                first_time = False
                if self.tag_name not in self.topics :
                    first_time = True
                    self.topics[self.tag_name] = rospy.Publisher('/dwm1001/tag/'+self.tag_name+"/position", PoseStamped, queue_size=100)
                p = PoseStamped()
                p.header.stamp = rospy.Time.now()  
                p.pose.position.x = float(arrayData[-4])
                p.pose.position.y = float(arrayData[-3])
                p.pose.position.z = float(arrayData[-2])
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

    def initializeDWM1001API(self):
        """
        Initialize dwm1001 api, by sending sending bytes
        :param:
        :returns: none
        """
        # reset incase previuos run didn't close properly
        self.serialPortDWM1001.write(DWM1001_API_COMMANDS.RESET)
        # send ENTER two times in order to access api
        self.serialPortDWM1001.write(DWM1001_API_COMMANDS.SINGLE_ENTER)
        # sleep for half a second
        time.sleep(0.5)
        self.serialPortDWM1001.write(DWM1001_API_COMMANDS.SINGLE_ENTER)
        # sleep for half second
        time.sleep(0.5)
        # send a third one - just in case
        self.serialPortDWM1001.write(DWM1001_API_COMMANDS.SINGLE_ENTER)





if __name__ == '__main__':  
    parser = argparse.ArgumentParser()
    parser.add_argument("--port", type=str, help="Target device serial port")
    args = parser.parse_args()
    print("www")
    try:
        dwm1001 = dwm1001_localizer(args)
        dwm1001.main()
        #rospy.spin()
    except rospy.ROSInterruptException:
        pass
