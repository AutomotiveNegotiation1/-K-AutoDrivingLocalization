#!/usr/bin/env python3.8

import rospy
from serial.tools import list_ports
import argparse
import yaml
from utils.srv import deviceInfo, deviceInfoResponse

class ketis_utils:
    
    def __init__(self):
        with open('/home/keti/keti_uwb_ws/src/utils/config/ketis_params.yaml', encoding='utf-8') as f:
            self._cfg = yaml.load(f, Loader=yaml.FullLoader)
        #self.mode = self._cfg['mode']
        self.mode = None
        self.uwbInfo = self._cfg['UWB']
        self.imuInfo = self._cfg['IMU']
        self.uwbNum = []
        self.imuNum = []
        
    def run(self):
        rospy.init_node('info_server', anonymous=False)
        server = rospy.Service('device_info', deviceInfo, self.set_mode)      
        rospy.spin()  
    
    def set_mode(self, msg):
        self.mode = msg.mode
        dev = self.get_device_parsing()
        return deviceInfoResponse(dev)
        
    def get_device_parsing(self):
        uart = list_ports.comports()
        devicePort = []
        for port, desc, hwid in sorted(uart):
            desc = desc.split(" ")[0]
            if(port is not None):
                hwid_split = hwid.split(" ")
                for info in hwid_split:
                    if "=" in info:
                        VID_PID = info.split("=")[1]
                        if self.mode == 'UWB':
                            if VID_PID == self.uwbInfo['VID_PID']:
                                devicePort.append(port)
                                # pass
                        if self.mode == 'IMU':
                            if VID_PID == self.imuInfo['VID_PID']:
                                # print(port)
                                devicePort.append(port)
                                # pass
        return devicePort
    
if __name__ == '__main__':
    utils = ketis_utils()
    utils.run()
