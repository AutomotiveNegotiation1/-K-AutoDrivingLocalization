/******************************************************************************
*
* Copyright (C) 2023 - 2028 KETI, All rights reserved.
*                           (Korea Electronics Technology Institute)
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running for Korean Government Project, or
* (b) that interact with KETI project/platform.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* KETI BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the KETI shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from KETI.
*
******************************************************************************/

#include "imusubscriber.h"

double b_acc_o[3] = {};
double b_gyro[3] = {};
double mode = 2;
double kf_psi = 0;
double gyro_psi = 0;
double cent_pos_est[3] = {};
double cent_vel_est[3] = {};
double state_o;
double acc_b_phi;
double acc_b_theta;
double signalIMU = 0;
double imuNum = 0;



static void argInit_1x3_real_T(double result[3]) {
    for (int idx1{0}; idx1 < 3; idx1++) {
        result[idx1] = 0.0;
    }
}

ImuSubscriber::ImuSubscriber(ros::NodeHandle& _node, IPECallback* _ipeCallback)
    : m_ipeCallback(_ipeCallback){
    setupSubscriber(_node);
    socketManager = SocketManager::getInstance(); // <-- Add this line to initialize the socketManager
}


ImuSubscriber::~ImuSubscriber() {}

void ImuSubscriber::operator()(IPEDataPacket &packet, double timestamp, SensorData* data) {
    processPacketData(packet, timestamp, data);
}

void ImuSubscriber::registerCallback(const std::function<void(int)>& callback) {
    callbacks.push_back(callback);
}

void ImuSubscriber::sendEvent(int data) {
    for (const auto& callback : callbacks) {
        callback(data);
    }
}

std::string ImuSubscriber::getPacketFrameID() {
    return frame_id;
}

void ImuSubscriber::setupSubscriber(ros::NodeHandle& node) {
    std::cout << "IMU test setup starting..." << std::endl;
    topic_name_stream << "/zed_f9r/imu";
    std::string topic_name = topic_name_stream.str();
    ROS_INFO("topic_name-->%s", topic_name.c_str());

    sub = node.subscribe<sensor_msgs::Imu>(topic_name, 10, &ImuSubscriber::_callback, this);

}

void ImuSubscriber::_callback(const sensor_msgs::Imu::ConstPtr& msg) {
    m_ipeDataPacket = IPEDataPacket(msg);
    m_ipeDataPacket.frame_id = "imu";
    if (m_ipeCallback) {
        m_ipeCallback->onLiveDataAvailable(m_ipeDataPacket);
    } else {
        ROS_WARN("m_kapCallback is a nullptr!");
    }
}

void ImuSubscriber::sendUDPMessage(double center_x, double center_y, double heading) {
    std::ostringstream oss;
    oss << center_x << "," << center_y << "," << heading;
    std::string result = oss.str();
    ROS_INFO("positioning-->%s", result.c_str());
    socketManager->broadcastUDPMessage(result);
}


void ImuSubscriber::processPacketData(IPEDataPacket &packet, double timestamp,  SensorData* data) {
    // double PositionVector_data[8] = {};
    std::vector<double> PositionVector_data;
    int PositionVector_size[2];
    
    // double PositionOut[7]; // PositioningSystem_V2_1 version (maintained before 2024.03.28, jang.sh)
    double PositionOut[10]; // PositioningSystem_V2_2 version (after 2024.03.28, jang.sh)

    double acc_y = packet.linear_y.back();
    double acc_z = packet.linear_z.back();
    double gyro_y = packet.angular_y.back();
    double gyro_z = packet.angular_z.back();
    if(acc_z > 0) {
        acc_y *= -1;
        acc_z *= -1;
        gyro_y *= -1;
        gyro_z *= -1;
    }

    PositionVector_data.push_back(packet.s_time);
    PositionVector_data.push_back(5.0);
    if (!packet.linear_x.empty()) {
        PositionVector_data.push_back(packet.linear_x.back());
    } else {
        PositionVector_data.push_back(0);
    }
    if (!packet.linear_y.empty()) {
        PositionVector_data.push_back(acc_y);
    } else {
        PositionVector_data.push_back(0);
    }
    if (!packet.linear_z.empty()) {
        PositionVector_data.push_back(acc_z);
    } else {
        PositionVector_data.push_back(0);
    }

    if (!packet.angular_x.empty()) {
        PositionVector_data.push_back(packet.angular_x.back());
    } else {
        PositionVector_data.push_back(0);
    }
    if (!packet.angular_y.empty()) {
        PositionVector_data.push_back(gyro_y);
    } else {
        PositionVector_data.push_back(0);
    }
    if (!packet.angular_z.empty()) {
        PositionVector_data.push_back(gyro_z);
    } else {
        PositionVector_data.push_back(0);
    }

    // PositioningSystem_V2_1(PositionVector_data.data(), PositionVector_size, PositionOut); // before 2024.03.28 (written by jang.sh)
    // PositioningSystem_V2_2(PositionVector_data.data(), PositionVector_size, PositionOut); // before 2024.03.28 (written by jang.sh)
    PositioningSystem_V2_3(PositionVector_data.data(), PositionVector_size, PositionOut); //  Changed 2024.04.01 (ahn.jw)
    sendUDPMessage(PositionOut[0], PositionOut[1], PositionOut[3]); // Kalman result (jang.sh)
    // sendUDPMessage(PositionOut[7], PositionOut[8], PositionOut[9]); // UWB result (jang.sh)


}
