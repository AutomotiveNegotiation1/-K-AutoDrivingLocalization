#!/usr/bin/env python3.8

import rospy
from serial.tools import list_ports
import argparse
import yaml

class ketis_utils:
    
    def __init__(self, mode=False):
        with open('/home/keti/keti_uwb_ws/src/utils/config/ketis_params.yaml', encoding='utf-8') as f:
            self._cfg = yaml.load(f, Loader=yaml.FullLoader)
        #self.mode = self._cfg['mode']
        self.mode = mode
        self.uwbInfo = self._cfg['UWB']
        self.imuInfo = self._cfg['IMU']
        self.uwbNum = []
        self.imuNum = []
        
    def get_device_number(self):
        deviceLen = None
        self.get_device_parsing()
        if self.mode == True:
            deviceLen = len(self.uwbNum)
        else:
            deviceLen = len(self.imuNum)
        
        return deviceLen
    
    def get_device_port(self):
        deviceLen = None
        self.get_device_parsing()
        if self.mode == True:
            deviceLen = self.uwbNum
        else:
            deviceLen = self.imuNum
        
        return deviceLen
            
    
    def get_device_parsing(self):
        uart = list_ports.comports()
        
        for port, desc, hwid in sorted(uart):
            desc = desc.split(" ")[0]
            if(port is not None):
                hwid_split = hwid.split(" ")
                
                for info in hwid_split:
                    if "=" in info:
                        VID_PID = info.split("=")[1]
                        
                        if self.mode == True:
                            if VID_PID == self.uwbInfo['VID_PID']:
                                print(port)
                                self.uwbNum.append(port)
                                pass
                        else:
                            if VID_PID == self.imuInfo['VID_PID']:
                                print(port)
                                self.imuNum.append(port)
                                pass
                        
        
    
if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("--mode", action="store_true", help="Target device serial port")
    args = parser.parse_args()
    
    try:
        utils = ketis_utils(args.mode)
        utils.get_device_parsing()
    except rospy.ROSInterruptException: 
        rospy.loginfo("Error get_device_parsing()")
    finally: 
        rospy.loginfo("modify get_device_parsing()")
    
