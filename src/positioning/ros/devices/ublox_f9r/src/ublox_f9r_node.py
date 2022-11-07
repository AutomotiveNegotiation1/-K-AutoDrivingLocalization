#!/usr/bin/env python3

import rospy
from sensor_msgs.msg import Imu
from ublox_f9r.msg import gnssPVT

import serial, time, struct
import numpy as np

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



tmpData = [0, 0, 0, 0, 0, 0, 0, 0, 0]
esf_meas = [0, 0, 0, 0, 0, 0, 0, 0, 0]

stag_prev = 0;

def scale_int2float(x, scaler):
    y = (x & 0x7fffff) - (x & 0x800000)
    return float(y) * scaler

def wire2tick(x):
    y = (x & 0x7fffff) - (x & 0x800000)
    return y

def ubx_esf_raw(raw):

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
                val = scale_int2float(dat & 0xffffff, 2 ** -12)
                tmpData[3] = val
            # elif (sensorType == 10):    # single tick (speed tick)
            #     val = wire2tick(dat & 0xffffff)
            # elif (sensorType == 11):    # speed
            #     val = scale_int2float(dat & 0xffffff, 1e-3)
            elif (sensorType == 12):    # gyroscope temperature
                val = scale_int2float(dat & 0xffffff, 1e-2)
                tmpData[4] = val
            elif (sensorType == 13):    # y-axis gyroscope angular rate
                val = scale_int2float(dat & 0xffffff, 2 ** -12)
                tmpData[2] = val
            elif (sensorType == 14):    # x-axis gyroscope angular rate
                val = scale_int2float(dat & 0xffffff, 2 ** -12)
                tmpData[1] = val
                tmpData[0] = sTtag
            elif (sensorType == 16):    # x-axis accelerometer-specific force
                val = scale_int2float(dat & 0xffffff, 2 ** -10)
                tmpData[6] = val
                tmpData[5] = tmpData[5]+1
            elif (sensorType == 17):    # y-axis accelerometer-specific force
                val = scale_int2float(dat & 0xffffff, 2 ** -10)
                tmpData[7] = val
            elif (sensorType == 18):    # z-axis accelerometer-specific force
                val = scale_int2float(dat & 0xffffff, 2 ** -10)
                tmpData[8] = val
        else:
            tmpData[1:5] = [0, 0, 0, 0]
            tmpData[6:9] = [0, 0, 0]

    return tmpData




def ubx_nav_pvt(raw):
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

    gnssData.iTow = iTow
    gnssData.date = [year, month, day, hour, minuts, sec]
    gnssData.fixtype = fixType
    gnssData.vel_ned = [velN, velE, velD]
    gnssData.hMSL = hMSL
    gnssData.gSpeed = gSpeed
    gnssData.heading = headMot
    gnssData.pDop = pDOP
    gnssData.headVeh = headVeh
    gnssData.numSV = numSV
    gnssData.pos_llh = [lon, lat, height]
    return True



def ublox_msg_decoding(cid, raw):

    if (cid == b'\x10\x03'):
        esf_meas = ubx_esf_raw(raw)
        # print('0', time.perf_counter(), esf_meas[0], esf_meas[1], esf_meas[2], esf_meas[3], esf_meas[4], esf_meas[5], esf_meas[6], esf_meas[7], esf_meas[8])
        str_raw = '0 ' + str(time.perf_counter()) +' '+ str(esf_meas[0]) +' '+ str(esf_meas[1]) +' '+ str(esf_meas[2]) +' '+ str(esf_meas[3]) +' '+ str(esf_meas[4]) +' '+ str(esf_meas[5]) +' '+ str(esf_meas[6]) +' '+ str(esf_meas[7]) +' '+ str(esf_meas[8]) +'\n'

        imuData.timeTag = esf_meas[0]
        imuData.gyro = [esf_meas[1], esf_meas[2], esf_meas[3]]
        imuData.gTemp = esf_meas[4]
        imuData.idx = esf_meas[5]
        imuData.accel = [esf_meas[6], esf_meas[7], esf_meas[8]]

        #if (esf_meas[5] % 1000 == 0):
        #    print(str_raw)



    elif (cid == b'\x01\x07'):
        ubx_nav_pvt(raw)

    return True


def checkHeader(raw):
    #print("checkHeader\n");
    return raw == b'\xb5\x62'

def checkSum(raw):
    chk_A = 0
    chk_B = 0

    for i in range(2, len(raw)-2):
        # print(rarostopic echo /ublox/imuw[i], end=', ')
        chk_A = chk_A + np.uint8(raw[i])
        chk_B = chk_B + chk_A

    if (np.uint8(chk_A) == raw[-2]) and (np.uint8(chk_B) == raw[-1]):
        return True
    else:
        return False


def pub_gnss_msg_set(msg, dat):
    msg.header.stamp = rospy.Time.now()
    msg.iTOW = dat.iTow
    msg.year = dat.date[0]
    msg.month = dat.date[1]
    msg.day = dat.date[2]
    msg.hour = dat.date[3]
    msg.min = dat.date[4]
    msg.sec = dat.date[5]
    msg.fixType = dat.fixtype
    msg.numSV = dat.numSV
    msg.velN = dat.vel_ned[0]
    msg.velE = dat.vel_ned[1]
    msg.velD = dat.vel_ned[2]
    msg.lon = dat.pos_llh[0]
    msg.lat = dat.pos_llh[1]
    msg.height = dat.pos_llh[2]
    msg.hMSL = dat.hMSL
    msg.gSpeed = dat.gSpeed
    msg.heading = dat.heading
    msg.pDOP = dat.pDop
    msg.headVeh = dat.headVeh

    return msg


def pub_imu_msg_set(msg, dat):
    msg.header.stamp = rospy.Time.now()
    msg.header.frame_id = 'ublox_f9r'
    msg.angular_velocity.x = dat.gyro[0]
    msg.angular_velocity.y = dat.gyro[1]
    msg.angular_velocity.z = dat.gyro[2]
    msg.linear_acceleration.x = dat.accel[0]
    msg.linear_acceleration.y = dat.accel[1]
    msg.linear_acceleration.z = dat.accel[2]

    return msg



if __name__ == '__main__':

    ## ros setting
    rospy.init_node('get_ublox_data', anonymous=True)
    pub_imu = rospy.Publisher('/ublox/imu', Imu, queue_size=10)
    pub_gnss = rospy.Publisher('/ublox/gnss_pvt', gnssPVT, queue_size=10)
    imuMsg = Imu()
    gnssMsg = gnssPVT()


    ## serial setting
    ser = serial.Serial("/dev/ttyIMU", 921600, timeout=1)
    if ser.is_open == False:
        rospy.logerr('Serial port')

    # init variable
    Buf = bytes(0)
    classID = bytes(2)
    pSize = bytes(2)
    payload = []

    imuData = imu_data()
    gnssData = gnss_data()

    while not rospy.is_shutdown():
        Buf = Buf + ser.read()

        if(len(Buf) >= 2):
            if checkHeader(Buf):
                Buf = Buf + ser.read(4)
                classID = Buf[2:4]
                #print("classID:", classID)
                pSize = struct.unpack('<H', Buf[4:6])[0]

                Buf = Buf + ser.read(pSize+2)

                if checkSum(Buf):
                    payload = Buf[6:6+pSize]
                    ublox_msg_decoding(classID,payload)
                    if (classID == b'\x10\x03'):
                        #print("[IMU] data: ", imuData.idx, "time tag: ", imuData.timeTag, "gyro: ", imuData.gyro, "accel: ", imuData.accel,"\n\n\n")
                        pub_imu.publish(pub_imu_msg_set(imuMsg, imuData))

                    elif (classID == b'\x01\x07'):
                        pub_gnss.publish(pub_gnss_msg_set(gnssMsg, gnssData))
                        #print(gnssData.iTow, gnssData.vel_ned)


                Buf = bytes(0)
            else:
                Buf = bytes(0)

        #rate.sleep()

    ser.close()





