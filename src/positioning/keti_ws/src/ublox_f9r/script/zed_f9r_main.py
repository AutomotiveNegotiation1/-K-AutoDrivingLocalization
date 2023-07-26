#!/usr/bin/env python3.8
""" For more info on the documentation go to https://www.u-blox.com/sites/default/files/ZED-F9R-01B_Datasheet_UBX-19054459.pdf
"""

from zed_f9r_systemDefinitions import SYS_DEFS

__author__     = SYS_DEFS.AUTHOR
__version__    = SYS_DEFS.VERSION
__maintainer__ = SYS_DEFS.MAINTAINER
__email__      = SYS_DEFS.EMAIL
__status__     = SYS_DEFS.STATUS


import rospy, time, serial, os, struct
import numpy as np
from zed_f9r_apiCommands            import ZED_F9R_API_MESSAGES
from dynamic_reconfigure.server     import Server
# from localizer_zed_f9r.cfg          import ZED_F9R_Tune_SerialConfig
from sensor_msgs.msg import Imu
from localizer_zed_f9r.msg          import GNSS
from localizer_zed_f9r.srv         import Acc
from localizer_zed_f9r.srv         import Gyro


class imu_data:
    gyro = [0, 0, 0]
    accel = [0, 0, 0]
    timeTag = 0
    gTemp = 0
    idx = 0

class gnss_data:
    iTow = 0
    date = [0, 0, 0, 0, 0, 0]
    fixType = 0
    pos_llh = [0, 0, 0]
    hMSL = 0
    gSpeed = 0
    heading = 0
    pDop = 0
    headVeh = 0
    vel_ned = [0, 0, 0]
    numSV = 0
    
class zed_f9r_localizer:
    
    def __init__(self):
        # initialize the node
        rospy.init_node('Localizer_ZED_F9R', anonymous=False)

        # allow serial port to be detected by user
        # os.popen("sudo chmod 777 /dev/ttyACM0", "w")

        # initialize ros rate 100hz
        self.rate = rospy.Rate(100)

        self.serialReadLine = ""
        # For dynamic configuration
        self.dynamicConfig_OPEN_PORT = {"open_port": False}
        self.dynamicConfig_CLOSE_PORT = {"close_port": False}
        self.dynamicConfig_SERIAL_PORT = {"serial_port": ""}

        # initialize serial port connections
        self.serialPortZED_F9R = serial.Serial(

            port       = str('/dev/' + rospy.get_param('~port')),
            baudrate   = int(rospy.get_param('~baud_rate')),
            parity     = SYS_DEFS.parity,
            stopbits   = SYS_DEFS.stopbits,
            bytesize   = SYS_DEFS.bytesize
        )
        
        # initialize the variation
        self.imuData = imu_data()
        self.gnssData = gnss_data()
        
    
    def main(self):
        """
        Initialize port and zed-f9r api

        :param:

        :returns: none

        """
        
        global serialReadLine

        #TODO implemnt functionality dynamic configuration
        #updateDynamicConfiguration_SERIALPORT()
        # close the serial port in case the previous run didn't closed it properly
        self.serialPortZED_F9R.close()
        # sleep for one sec
        time.sleep(1)
        # open serial port
        self.serialPortZED_F9R.open()

        # check if the serial port is opened
        if(self.serialPortZED_F9R.isOpen()):
            rospy.loginfo("Port opened: "+ str(self.serialPortZED_F9R.name) )
            # start sending commands to the board so we can initialize the board
            rospy.loginfo("Reading ZED-F9R data frame")
        else:
            rospy.loginfo("Can't open port: "+ str(self.serialPortZED_F9R.name))

        try:
            
            while not rospy.is_shutdown():
                # just read everything from serial port
                serialReadLine = self.serialPortZED_F9R.read()

                try:
                    self.pubblishCoordinatesIntoTopics(self.serialPortZED_F9R)

                except IndexError:
                    rospy.loginfo("Found index error in the network array!DO SOMETHING!")



        except KeyboardInterrupt:
            rospy.loginfo("Quitting ZED-F9R Shell Mode and closing port, allow 1 second for UWB recovery")

        finally:
            rospy.loginfo("Quitting, and sending reset command to dev board")
            # serialPortZED_F9R.reset_input_buffer()
            self.rate.sleep()
            if "reset" in serialReadLine:
                rospy.loginfo("succesfully closed ")
                self.serialPortZED_F9R.close()
        
    def splitByRawData(self, serialPortZED_F9R):
        """
        Split network data such us DATA Frame of IMU, by Hexa data

        :param dataFromUSB:  Array from serial port containing all informations, ACC xyz and GYRO xyz

        :returns: arrayFromUSBFormatted

        """
                # init variable
        Buf = bytes(0)

        Buf = Buf + serialPortZED_F9R.read()
        
        return Buf

    def pubblishCoordinatesIntoTopics(self, serialPortZED_F9R):
        """
        Publish anchors and tag in topics using Tag and Anchor Object

        :param networkDataArray:  Array from serial port containing all informations, tag xyz and anchor xyz

        :returns: none

        """
        ClassID = bytes(2)
        pSize = bytes(2)
        payload = []
        Buf = bytes(0)
        Buf = Buf + serialPortZED_F9R.read(2)
        
        if(len(Buf) >= 1):
            if self.checkHeader(Buf):
                Buf = Buf + serialPortZED_F9R.read(4)
                ClassID = Buf[2:4]
                #print("classID:", classID)
                pSize = struct.unpack('<H', Buf[4:6])[0]

                Buf = Buf + serialPortZED_F9R.read(pSize+2)

                if self.checkSum(Buf):
                    payload = Buf[6:6+pSize]
                    # rospy.loginfo("payload :" + str(payload))
                    self.ublox_msg_decoding(ClassID, payload)
                    if (ClassID == ZED_F9R_API_MESSAGES.UBX_ESF_RAW):
                        #print("[IMU] data: ", imuData.idx, "time tag: ", imuData.timeTag, "gyro: ", imuData.gyro, "accel: ", imuData.accel,"\n\n\n")
                        pub_imu = rospy.Publisher('/zed_f9r/imu', Imu, queue_size=100)
                        imuMsg = Imu()
                        imuMsg.header.stamp = rospy.Time.now()
                        imuMsg.header.frame_id = 'zed_f9r'
                        imuMsg.angular_velocity.x = self.imuData.gyro[0]
                        imuMsg.angular_velocity.y = self.imuData.gyro[1]
                        imuMsg.angular_velocity.z = self.imuData.gyro[2]
                        imuMsg.linear_acceleration.x = self.imuData.accel[0]
                        imuMsg.linear_acceleration.y = self.imuData.accel[1]
                        imuMsg.linear_acceleration.z = self.imuData.accel[2]
                        
                        # rospy.loginfo("imuMsg :" + str(imuMsg))
                        pub_imu.publish(imuMsg)

                    elif (ClassID == ZED_F9R_API_MESSAGES.UBX_NAV_PVT):
                        pub_gnss = rospy.Publisher('/zed_f9r/gnss_pvt', GNSS, queue_size=10, latch=True)
                        gnssMsg = GNSS()
                        gnssMsg.header.stamp = rospy.Time.now()
                        gnssMsg.iTOW = self.gnssData.iTow
                        gnssMsg.year = self.gnssData.date[0]
                        gnssMsg.month = self.gnssData.date[1]
                        gnssMsg.day = self.gnssData.date[2]
                        gnssMsg.hour = self.gnssData.date[3]
                        gnssMsg.min = self.gnssData.date[4]
                        gnssMsg.sec = self.gnssData.date[5]
                        gnssMsg.fixType = self.gnssData.fixtype
                        gnssMsg.numSV = self.gnssData.numSV
                        gnssMsg.velN = self.gnssData.vel_ned[0]
                        gnssMsg.velE = self.gnssData.vel_ned[1]
                        gnssMsg.velD = self.gnssData.vel_ned[2]
                        gnssMsg.lon = self.gnssData.pos_llh[0]
                        gnssMsg.lat = self.gnssData.pos_llh[1]
                        gnssMsg.height = self.gnssData.pos_llh[2]
                        gnssMsg.hMSL = self.gnssData.hMSL
                        gnssMsg.gSpeed = self.gnssData.gSpeed
                        gnssMsg.heading = self.gnssData.heading
                        gnssMsg.pDOP = self.gnssData.pDop
                        gnssMsg.headVeh = self.gnssData.headVeh
                        pub_gnss.publish(gnssMsg)
                        

                Buf = bytes(0)
            else:
                Buf = bytes(0)             
                
    def scale_int2float(self, x, scaler):
        y = (x & 0x7fffff) - (x & 0x800000)
        return float(y) * scaler

    def wire2tick(self, x):
        y = (x & 0x7fffff) - (x & 0x800000)
        return y

    def ubx_esf_raw(self, raw):
        tmpData = [0, 0, 0, 0, 0, 0, 0, 0, 0]
        # timeTag = struct.unpack('<I', raw[0:4])[0]
        for idx in range(0, 7):
            dat = struct.unpack('I', raw[4+idx*8:8+idx*8])[0]
            sensorType = dat >> 24 & 0x1f
            sTtag = struct.unpack('I', raw[8:12])[0]
            #print(idx, sensorType, sTtag)
            #print("sensor time tag: ", sTtag, "\n")
            stag_prev = sTtag;

            #print("diff time tag: ", sTtag-stag_prev, "\n")

            if sensorType in [5, 12, 13, 14, 16, 17, 18]:
                if (sensorType == 5):       # z-axis gyroscope angular rate
                    val = self.scale_int2float(dat & 0xffffff, 2 ** -12)
                    tmpData[3] = val
                # elif (sensorType == 10):    # single tick (speed tick)
                #     val = wire2tick(dat & 0xffffff)
                # elif (sensorType == 11):    # speed
                #     val = scale_int2float(dat & 0xffffff, 1e-3)
                elif (sensorType == 12):    # gyroscope temperature
                    val = self.scale_int2float(dat & 0xffffff, 1e-2)
                    tmpData[4] = val
                elif (sensorType == 13):    # y-axis gyroscope angular rate
                    val = self.scale_int2float(dat & 0xffffff, 2 ** -12)
                    tmpData[2] = val
                elif (sensorType == 14):    # x-axis gyroscope angular rate
                    val = self.scale_int2float(dat & 0xffffff, 2 ** -12)
                    tmpData[1] = val
                    tmpData[0] = sTtag
                elif (sensorType == 16):    # x-axis accelerometer-specific force
                    val = self.scale_int2float(dat & 0xffffff, 2 ** -10)
                    tmpData[6] = val
                    tmpData[5] = tmpData[5]+1
                elif (sensorType == 17):    # y-axis accelerometer-specific force
                    val = self.scale_int2float(dat & 0xffffff, 2 ** -10)
                    tmpData[7] = val
                elif (sensorType == 18):    # z-axis accelerometer-specific force
                    val = self.scale_int2float(dat & 0xffffff, 2 ** -10)
                    tmpData[8] = val
            else:
                tmpData[1:5] = [0, 0, 0, 0]
                tmpData[6:9] = [0, 0, 0]

        return tmpData
    
    def ubx_nav_pvt(self, raw):
        global prev_time
        iTow = struct.unpack('<I', raw[0:4])[0]
        year = struct.unpack('<H', raw[4:6])[0]         # [y]
        month = np.uint8(raw[6])                        # [month]
        day = np.uint8(raw[7])                          # [d]
        hour = np.uint8(raw[8])                         # [h]
        minuts = np.uint8(raw[9])                       # [min]
        sec = np.uint8(raw[10])                         # [s]
        valid = hex(raw[11])
        tAcc = struct.unpack('<I', raw[12:16])[0]       # [ns]
        nano = struct.unpack('<i', raw[16:20])[0]       # [ns]
        fixType = np.uint8(raw[20])                     # 0-no fix, 1-DR, 2-2D fix, 3-3D fix, 4-GNSS+DR, 5-time only fix
        flags = hex(raw[21])
        flags2 = hex(raw[22])
        numSV = np.uint8(raw[23])
        lon = struct.unpack('<i', raw[24:28])[0]
        lat = struct.unpack('<i', raw[28:32])[0]
        height = struct.unpack('<i', raw[32:36])[0]
        hMSL = struct.unpack('<i', raw[36:40])[0]
        hAcc = struct.unpack('<I', raw[40:44])[0] /1000         # [mm] --> [m]
        vAcc = struct.unpack('<I', raw[44:48])[0] /1000         # [mm] --> [m]
        velN = struct.unpack('<i', raw[48:52])[0]
        velE = struct.unpack('<i', raw[52:56])[0]
        velD = struct.unpack('<i', raw[56:60])[0]
        gSpeed = struct.unpack('<i', raw[60:64])[0]
        headMot = struct.unpack('<i', raw[64:68])[0]
        sAcc = struct.unpack('<I', raw[68:72])[0] /1000         # [mm/s] --> [m/s]
        headAcc = struct.unpack('<I', raw[72:76])[0] * 1e-5     # [deg]
        pDOP =struct.unpack('<H', raw[76:78])[0]
        flags3 = hex(raw[78])
        headVeh = struct.unpack('<i', raw[84:88])[0]
        magDec = struct.unpack('<h', raw[88:90])[0] * 1e-2      # [deg]
        magAcc = struct.unpack('<H', raw[90:92])[0] * 1e-2      # [deg]

        str_pvt = '1 ' + str(time.perf_counter()) +' '+ str(iTow) +' '+ str(hour)+' '+ str(min)+' '+ str(sec) +' '+ str(nano) +' '+ str(fixType) +' '+ str(numSV) +' '+ str(lat) +' '+ str(lon) +' '+ str(height) +' '+ str(velN) +' '+ str(velE) +' '+ str(velD) +' '+ str(gSpeed) + '\n'
        # print('1', time.perf_counter(), iTow, year, month, day, hour, min, sec, valid, tAcc, nano, fixType, flags, flags2, numSV, lon, lat, height, hMSL, hAcc, vAcc, velN, velE, velD, gSpeed, headMot, sAcc, headAcc, pDOP, flags3, headVeh, magDec, magAcc)

        self.gnssData.iTow = iTow
        self.gnssData.date = [year, month, day, hour, minuts, sec]
        self.gnssData.fixtype = fixType
        self.gnssData.vel_ned = [velN, velE, velD]
        self.gnssData.hMSL = hMSL
        self.gnssData.gSpeed = gSpeed
        self.gnssData.heading = headMot
        self.gnssData.pDop = pDOP
        self.gnssData.headVeh = headVeh
        self.gnssData.numSV = numSV
        self.gnssData.pos_llh = [lon, lat, height]
        return True


                
    def ublox_msg_decoding(self, ClassID, payload):   
        if (ClassID == ZED_F9R_API_MESSAGES.UBX_ESF_RAW):
            esf_meas = self.ubx_esf_raw(payload)
            # print('0', time.perf_counter(), esf_meas[0], esf_meas[1], esf_meas[2], esf_meas[3], esf_meas[4], esf_meas[5], esf_meas[6], esf_meas[7], esf_meas[8])
            str_raw = '0 ' + str(time.perf_counter()) +' '+ str(esf_meas[0]) +' '+ str(esf_meas[1]) +' '+ str(esf_meas[2]) +' '+ str(esf_meas[3]) +' '+ str(esf_meas[4]) +' '+ str(esf_meas[5]) +' '+ str(esf_meas[6]) +' '+ str(esf_meas[7]) +' '+ str(esf_meas[8]) +'\n'

            self.imuData.timeTag = esf_meas[0]
            self.imuData.gyro = [esf_meas[1], esf_meas[2], esf_meas[3]]
            self.imuData.gTemp = esf_meas[4]
            self.imuData.idx = esf_meas[5]
            self.imuData.accel = [esf_meas[6], esf_meas[7], esf_meas[8]]

            #if (esf_meas[5] % 1000 == 0):
            #    print(str_raw)


        elif (ClassID == ZED_F9R_API_MESSAGES.UBX_NAV_PVT):
            self.ubx_nav_pvt(payload)

        return True

                
    def checkHeader(self, Buf):
        #print("checkHeader\n");
        return Buf == ZED_F9R_API_MESSAGES.CHECK_HEADER

    def checkSum(self, Buf):
        chk_A = 0
        chk_B = 0

        for i in range(2, len(Buf)-2):
            # print(rarostopic echo /ublox/imuw[i], end=', ')
            chk_A = chk_A + np.uint8(Buf[i])
            chk_B = chk_B + chk_A

        if (np.uint8(chk_A) == Buf[-2]) and (np.uint8(chk_B) == Buf[-1]):
            return True
        else:
            return False
    
    def updateDynamicConfiguration_SERIALPORT(self):

        """
        Update dynamic configuration of ROS

        :param:

        :returns: none

        """

        global dynamicConfig_SERIAL_PORT

        # intialize dynamic configuration
        dynamicConfigServer = Server(ZED_F9R_Tune_SerialConfig, self.callbackDynamicConfig)
        # set close port to true
        self.dynamicConfig_CLOSE_PORT.update({"close_port": True})
        # set the open port to false
        self.dynamicConfig_OPEN_PORT.update({"open_port" : False})
        # update the server
        dynamicConfigServer.update_configuration(self.dynamicConfig_OPEN_PORT)
        dynamicConfigServer.update_configuration(self.dynamicConfig_CLOSE_PORT)
        # update the server with opened port
        self.dynamicConfig_CLOSE_PORT.update({"close_port": False})
        # update the server with close port
        self.dynamicConfig_OPEN_PORT.update({"open_port": True})
        # update name of serial port in dynamic configuration
        dynamicConfig_SERIAL_PORT = {"serial_port": str(self.serialPortZED_F9R.name)}
        # now update server configuration
        dynamicConfigServer.update_configuration(self.dynamicConfig_OPEN_PORT)
        dynamicConfigServer.update_configuration(self.dynamicConfig_OPEN_PORT)
        dynamicConfigServer.update_configuration(self.dynamicConfig_CLOSE_PORT)
        dynamicConfigServer.update_configuration(self.dynamicConfig_SERIAL_PORT)


    def callbackDynamicConfig(self, config, level):
        """
        Map each button from dynamic configuration gui with specific action

        :param config:  array contains value of the gui

        :returns: config

        """
        global serialReadLine
        #rospy.loginfo("""Reconfigure Request: {ZED_F9R_network_info}, {open_port},\
        #      {serial_port}, {close_port}""".format(**config))

        if config["quit_zed_f9r_api"]:
            rospy.loginfo("Not implement it yet")
            config["quit_zed_f9r_api"] = False

        if config["close_port"]:
            rospy.loginfo("Close port not implement it yet")
            config["close_port"] = False

        if config["exit"]:
            rospy.loginfo("Not implement it yet")
            config["exit"] = False

        return config

def start():
    zed_f9r = zed_f9r_localizer()
    zed_f9r.main()


if __name__ == '__main__':
    try:
        start()
        rospy.spin()
    except rospy.ROSInterruptException:
        pass