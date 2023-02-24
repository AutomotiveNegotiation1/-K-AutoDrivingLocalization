#! /usr/bin/env python3
import time
import rospy
import numpy as np
import json
import serial
from serial.tools import list_ports

from ublox_f9r.msg import gnssPVT
from sensor_msgs.msg import Imu
from decawave_uwb.msg import uwb_anchor

def listToString(self, str_list):
    """
    _For List to String_

    Args:
        str_list (_list_): sring list

    Returns:
        _str_: list to sting convet
    """
    result = ""
    for s in str_list:
        result += s
    return result.strip()

def listIntToString(self, str_list):
    """
    _For List int to String_

    Args:
        str_list (_type_): _description_

    Returns:
        _type_: _description_
    """
    result = ""
    for s in str_list:
        result += str(s) + " "
    return result.strip()

def DWM1001_init(self, ser):
    """
    DWM1001 shell command init

    Args:
        ser (_byte_): serial read data instance
    """
    ser.write(b'\x0D\x0D')
    while(1):
        raw = ser.readline()
        rawData = raw.split(b'>')
        if rawData[0] == b'dwm':
            print("end")
            break
    raw = ser.readline()
    time.sleep(1)


def pub_uwb_msg_set(self, name, uwbMsg, anchorList, dat):
    """
    For publish message settings

    Args:
        name (_str_): _description_
        uwbMsg (_type_): _description_
        anchorList (_type_): _description_
        dat (_type_): _description_

    Returns:
        _type_: _description_
    """
    uwbMsg.header.stamp = rospy.Time.now()
    uwbMsg.header.frame_id = 'MDEK1001'
    
    numMeas = int(dat[1])
    uwbMsg.ActiveNum = numMeas
    uwbMsg.TagID = name

    uwbMsg.Range = [np.nan, np.nan, np.nan, np.nan, np.nan, np.nan, np.nan, np.nan]

    for i in range(0, numMeas):
        anchor = dat[6*i+3]
        anchor = anchor.decode()
        dist = float(dat[6*i+7])
        idx = anchorList.index(anchor)
        uwbMsg.Range[idx] = dist


    if dat[-5].decode() == 'POS':
        uwbMsg.Pos.position.x = float(dat[-4])
        uwbMsg.Pos.position.y = float(dat[-3])
        uwbMsg.Pos.position.z = float(dat[-2])
        
    print(uwbMsg)

    return uwbMsg

def get_json_parser(config):
    """_summary_

    Args:
        config (_type_): _description_
    Returns:
        data (_dict_):
    """
    with open(config, 'r', encoding='utf-8') as file:
        config = json.load(file)
        
    return config
        


def _get_device_parser(config):
    """_summary_

    Args:
        config (_dict_): config file

    Returns:
        _dict_: Connected hardware infomation of serial port 
    """
    dev = {}
    devices = list_ports.comports()
    dev_num = 0
    for port, desc, hwid in sorted(devices):
        if(port is not None):
            V_PID = hwid.split('PID=')[1]
            if 'SER=' in V_PID:
                V_PID = V_PID.split(' SER=')[0]
            for device in config:
                if(V_PID ==config[device]['conf']['V_PID']):
                    dev[device] = {}
                    connect_ID = device + str(dev_num+1)
                    dev[connect_ID] = device.device
                    dev_num += 1
                    print("uart> device with serial {} found at {}".format(connect_ID, dev[connect_ID]))
    
    return dev

def get_device_parser(config):
    """_summary_

    Args:
        config (_type_): _description_
    """    
    uart = list_ports.comports()
    dictt = {}
    uwbNum = []    
    imuNum = []    
    mode = {"J-Link":"UWB", "u-blox":"IMU"}
    for port, desc, hwid in sorted(uart):
        desc = desc.split(" ")[0]
        conf = {}
        if(port is not None):
            conf['port'] = port
            info = hwid.split(" ")
            for data in info:
                # print(data)
                if "=" in data:
                    key = data.split("=")
                    conf[key[0]] = key[1]
        if(conf["VID:PID"] == config[mode[desc]]["conf"]["VID:PID"]):
            if desc == 'u-blox':
                imuNum.append(conf)
            if desc == 'J-Link':
                uwbNum.append(conf)
        
    dictt["UWB"] = uwbNum
    dictt["IMU"] = imuNum
    
    return dictt

def connect_device(config):
    """_summary_

    Args:
        serial (_Json_): _description_
    """
    serialInstance = []
    config = get_json_parser(config)
    dev = get_device_parser(config)

    for device in dev:
        nodeName = config[device]['conf']['name']
        pubName = config[device]['conf']['P_name']
        conf = dev[device]
        serialInstance = None

        rospy.init_node('get_{}_data'.format(nodeName), anonymous=True)
        if device == 'IMU':
            pubImu = rospy.Publisher('/{}}/{}'.format(nodeName, pubName[0]), Imu, queue_size=10)
            pubGnss = rospy.Publisher('/{}}/{}'.format(nodeName, pubName[1]), gnssPVT, queue_size=10)
            # imuMsg = Imu()
            # gnssMsg = gnssPVT()

        else:
            pub_uwb = rospy.Publisher('/{}/{}'.format(nodeName, pubName[1]), uwb_anchor, queue_size=10)
            # uwbMsg = uwb_anchor() 
        
        for i, con in enumerate(conf[:]):   
            ## serial setting
            serialInstance.append(serial.Serial(con['port'], con['baud'], timeout=1))
            if serialInstance[i].is_open == False:
                rospy.logerr('Serial port')
                
    return serialInstance, 
    
    