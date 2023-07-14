#!/usr/bin/env python3.8
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
        serial_connect = list_ports.comports()
        for i, (port, desc, hwid) in enumerate(sorted(serial_connect)):
            print(i)
            
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
{'Tag1': [{'tag_name': 'tag1'}, {'id': '1366:0105'}, {'serial': '000760178940'}, {'baud': 115200}],
 'Tag2': [{'tag_name': 'tag2'}, {'id': '1366:0105'}, {'serial': '000760179526'}, {'baud': 115200}],
 'Tag3': [{'tag_name': 'tag3'}, {'id': '1366:0105'}, {'serial': '000760009648'}, {'baud': 115200}],
 'Tag4': [{'tag_name': 'tag4'}, {'id': '1366:0105'}, {'serial': '000760009277'}, {'baud': 115200}]}