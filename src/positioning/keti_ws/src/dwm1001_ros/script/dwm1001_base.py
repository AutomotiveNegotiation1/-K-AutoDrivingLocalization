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
import rospy
import yaml
from serial.tools import list_ports

class dwm1001_DataLoader:
    def __init__(self):
        rospy.init_node("start_dmw", anonymous=True)
        self.data = rospy.get_param("/localizer_dwm1001/serial_data")
        f = open(self.data) # 나중에 반환(close() ) 코드 추가
        self._cfg = yaml.load_all(f, Loader=yaml.FullLoader)


    def main(self):
        print("www")
        print(self.data)

        serial_connect = list_ports.comports()
        for i, (port, desc, hwid) in enumerate(sorted(serial_connect)):
            print(port)

        # tag1_data = [value['Tag'][0] for value in self._cfg]
        # tag2_data = [value['Tag'][1] for value in self._cfg]
        # tag3_data = [value['Tag'][2] for value in self._cfg]
        # tag4_data = [value['Tag'][3] for value in self._cfg]


def start():
    dwm1001 = dwm1001_DataLoader()
    dwm1001.main()

if __name__ == "__main__":
    try:
        start()
        rospy.spin()
    except rospy.ROSInterruptException:
        pass
