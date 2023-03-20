#!/usr/bin/env python3.8

from pyrsistent import v
import rospy
import sys
import time
import numpy as np
import serial
import argparse
import math
from sensor_msgs.msg import Imu
from pyubx2 import UBXReader, UBXMessage, GET, SET
import struct
from ublox_f9r.msg import imu_message

class ZED_F9R_IMU_localizer:

    def __init__(self, serial_port):
        
        rospy.init_node('zed_f9r_imu_publisher', anonymous=True)
        
        self.serial_port = serial_port
        self.prev_time = None
        self.post_time = None
        self.prev_roll = 0
        self.prev_pitch = 0
        self.prev_yaw = 0
        
        self.topics = None
        
        # Serial port settings
        self.serial_instance = serial.Serial(
            port = self.serial_port,
            baudrate = 115200,
            parity = serial.PARITY_ODD,
            stopbits = serial.STOPBITS_TWO,
            bytesize = serial.SEVENBITS
        )
        
        self.serial_instance.reset_input_buffer()
        
        
        
    # def enable_esf_raw(self, ser):
    #     cfg_msg = b'\xB5\x62\x06\x01\x08\x00\x10\x03\x01\x00\x00\x00\x00\x00\x1C\x8A'
    #     ser.write(cfg_msg)
    #     # msg = UBXMessage('CFG', 'CFG-MSG', SET,
    #     #              msgClass=0x10,
    #     #              msgID=0x03,
    #     #              rate=1)
    #     # ser.write(msg.serialize())
    
    # def set_message_rate(self, serial_port, msg_class, msg_id, rate):
    #     # UBX-CFG-MSG 메시지 생성
    #     cfg_msg = UBXMessage('CFG', 'CFG-MSG', SET, msgClass=msg_class, msgID=msg_id, rate=rate)
        
    #     # 시리얼 포트를 통해 메시지 전송
    #     serial_port.write(cfg_msg.serialize())

    #     # 응답 메시지를 기다린 다음 처리
    #     # 이 부분은 ACK-ACK 메시지를 받을 때까지 기다리는 로직이 필요할 수 있습니다.
    def scale_int2float(self, x, scaler):
        y = (x & 0x7fffff) - (x & 0x800000)
        return float(y) * scaler
    
    def low_pass_filter(self, data, alpha=0.8):
        # print("Data:", data)  # Add this line to debug
        filtered_data = [0] * len(data)
        filtered_data[0] = data[0]
        
        for i in range(1, len(data)):
            filtered_data[i] = alpha * data[i-1] + (1 - alpha) * data[i]
        
        return filtered_data
    
    def quaternion_from_euler(self, roll, pitch, yaw):
        cy = math.cos(yaw * 0.5)
        sy = math.sin(yaw * 0.5)
        cp = math.cos(pitch * 0.5)
        sp = math.sin(pitch * 0.5)
        cr = math.cos(roll * 0.5)
        sr = math.sin(roll * 0.5)

        qw = cy * cp * cr + sy * sp * sr
        qx = cy * cp * sr - sy * sp * cr
        qy = sy * cp * sr + cy * sp * cr
        qz = sy * cp * cr - cy * sp * sr

        return qx, qy, qz, qw
    
    def get_roll_pitch_yaw(self, acc_data, gyro_data, dt, alpha=0.98):
        # 가속도계 데이터에서 roll, pitch 계산
        roll_acc = np.arctan2(acc_data[1], acc_data[2])
        pitch_acc = np.arctan2(-acc_data[0], np.sqrt(acc_data[1]**2 + acc_data[2]**2))

        # 자이로스코프 데이터를 사용하여 오일러 각 누적
        roll_rate = gyro_data[0] * dt
        pitch_rate = gyro_data[1] * dt
        yaw_rate = gyro_data[2] * dt

        # 가속도계와 자이로스코프 데이터를 융합하기 위해 컴플리먼트 필터 사용
        roll = alpha * (self.prev_roll + roll_rate) + (1 - alpha) * roll_acc
        pitch = alpha * (self.prev_pitch + pitch_rate) + (1 - alpha) * pitch_acc
        yaw = self.prev_yaw + yaw_rate

        # 이전 오일러 각 업데이트
        self.prev_roll = roll
        self.prev_pitch = pitch
        self.prev_yaw = yaw

        return roll, pitch, yaw

    def main(self):
        if(self.serial_instance.isOpen()):
            rospy.loginfo("Port opened: "+ str(self.serial_instance.name) )
        else:
            rospy.loginfo("Can't open port: "+ str(self.serial_instance.name))
        
        try:
            # self.enable_esf_raw(self.serial_instance)
            # self.set_message_rate(self.serial_instance, 0x01, 0x07, 0)  # NAV-PVT 비활성화
            # self.set_message_rate(self.serial_instance, 0x02, 0x10, 1)  # RXM-RAW 활성화
            while not rospy.is_shutdown():
                # just read everything from serial port
                # ubx_reader = UBXReader(self.serial_instance)
                ubx_reader = UBXReader(self.serial_instance)

                try:
                    self.publishIMUPositions(ubx_reader)

                except IndexError:
                    rospy.loginfo("Found index error in the network array! DO SOMETHING!")



        except KeyboardInterrupt:
            rospy.loginfo("Quitting DWM1001 Shell Mode and closing port, allow 1 second for UWB recovery")

        finally:
            rospy.loginfo("Quitting, and sending reset command to dev board")
            # self.serial_instance.reset_input_buffer()
            # self.rate.sleep()
            serialReadLine = self.serial_instance.read_until()
            if "reset" in serialReadLine:
                rospy.loginfo("succesfully closed ")
                self.serial_instance.close()
                
    
    def publishIMUPositions(self, ubx_reader):        
        if self.prev_time is None:
            self.prev_time = time.time()
            return  # Skip the first iteration
        
        (raw_data, parsed_data) = ubx_reader.read()
        if parsed_data is not None:
            msg_id = parsed_data.identity
            if msg_id == "ESF-RAW":
                # print(parsed_data)
                # 가속도 및 자이로스코프 데이터 추출
                acc_data = [0, 0, 0]
                gyro_data = [0, 0, 0]
                # print(parsed_data)
                for idx in range(7):
                    if self.topics is None:
                        first_time = True
                        
                        self.topics = rospy.Publisher(
                            '/ublox' + 
                            '/imu/' +  
                            "/twist", 
                            imu_message, 
                            queue_size=100
                        )
                    # data_field = raw_data
                    # print(data_field)
                    # data_field = struct.unpack('I', raw_data[4+idx*8:8+idx*8])[0]
                    data_field = struct.unpack('I', raw_data[10+idx*8:14+idx*8])[0]
                    # print(data_field)                     
                    sensor_type = data_field >> 24 & 0x1F

                    if sensor_type == 16:
                        acc_data[0] = self.scale_int2float(data_field & 0xFFFFFF, 2 ** -10)
                    elif sensor_type == 17:
                        acc_data[1] = self.scale_int2float(data_field & 0xFFFFFF, 2 ** -10)
                    elif sensor_type == 18:
                        acc_data[2] = self.scale_int2float(data_field & 0xFFFFFF, 2 ** -10)
                    elif sensor_type == 14:
                        gyro_data[0] = self.scale_int2float(data_field & 0xFFFFFF, 2 ** -12)
                    elif sensor_type == 13:
                        
                        gyro_data[1] = self.scale_int2float(data_field & 0xFFFFFF, 2 ** -12)
                    elif sensor_type == 5:
                        gyro_data[2] = self.scale_int2float(data_field & 0xFFFFFF, 2 ** -12)

                self.post_time = time.time()
                dt = (self.post_time - self.prev_time)
                self.prev_time = self.post_time  # Update the previous time
                
                # print(acc_data)
                
                # 가속도계 및 자이로스코프 데이터 필터링
                filtered_acc_data = self.low_pass_filter(acc_data)
                filtered_gyro_data = self.low_pass_filter(gyro_data)
                
                roll, pitch, yaw = self.get_roll_pitch_yaw(filtered_acc_data, filtered_gyro_data, dt, alpha=0.98)
                qx, qy, qz, qw = self.quaternion_from_euler(roll, pitch, yaw)
                
                i = imu_message()
                i.header.frame_id = 'ublox_f9r'
                i.dt = dt
                i.roll = roll
                i.pitch = pitch
                i.yaw = yaw
                i.linear_x = filtered_acc_data[0]
                i.linear_y = filtered_acc_data[1]
                i.linear_z = filtered_acc_data[2]
                i.angular_x = filtered_gyro_data[0]
                i.angular_y = filtered_gyro_data[1]
                i.angular_z = filtered_gyro_data[2]
                i.quaternion_x = qx
                i.quaternion_y = qy
                i.quaternion_z = qz
                i.quaternion_w = qw
                self.topics.publish(i)
                
                
                # print("Rotate Data:",roll, pitch, yaw)
                # print("Quaternion Data:", qx, qy, qz, qw)
                # print("Acc Data:", acc_data)
                # print("Gyro Data:", gyro_data)


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("--port", type=str, help="Target device serial port")
    args = parser.parse_args()

    try:
        zed_f9r_imu_pub = ZED_F9R_IMU_localizer(args.port)
        zed_f9r_imu_pub.main()
        rospy.spin()
    except rospy.ROSInterruptException:
        pass