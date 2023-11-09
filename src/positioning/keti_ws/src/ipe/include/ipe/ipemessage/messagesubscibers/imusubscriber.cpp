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
double mode = 0;
double kf_psi = 0;
double gyro_psi = 0;
double cent_pos_est[3] = {};
double cent_vel_est[3] = {};
double state_o;
double acc_b_phi;
double acc_b_theta;

static void argInit_1x3_real_T(double result[3]) {
    for (int idx1{0}; idx1 < 3; idx1++) {
        result[idx1] = 0.0;
    }
}

ImuSubscriber::ImuSubscriber(ros::NodeHandle& node, IPECallback* ipeCallback)
    : m_ipeCallback(ipeCallback) {
    setupSubscriber(node);
}

ImuSubscriber::~ImuSubscriber() {}

void ImuSubscriber::operator()(IPEDataPacket &packet, double timestamp) {
    processPacketData(packet, timestamp);
}

void ImuSubscriber::registerCallback(const std::function<void(double, std::string&)>& callback) {
    callbacks.push_back(callback);
}

void ImuSubscriber::sendEvent(double data) {
    for (const auto& callback : callbacks) {
        std::string st = "IMU";
        callback(data, st);
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

    // pub = node.advertise<ipe::Imupos>("/IMU", 10);
    sub = node.subscribe<sensor_msgs::Imu>(topic_name, 10, &ImuSubscriber::_callback, this);
    // subFusion = node.subscribe<ipe::Fusion>("/Fusion", 10, &ImuSubscriber::_callback_Fusion, this);

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

// void ImuSubscriber::_callback_Fusion(const ipe::Fusion::ConstPtr& msg) {
//     if (!msg){
//         kf_psi = msg->kf_psi;
//         gyro_psi = msg->gyro_psi;
//         acc_b_phi = msg->acc_b_phi;
//         for(int i=0; i<3;i++){
//             cent_pos_est[i] = msg->cent_pos_est[i];
//             cent_vel_est[i] = msg->cent_vel_est[i];
//         }
//     }
// }


void ImuSubscriber::processPacketData(IPEDataPacket &packet, double timestamp) {
    double IMUacc_c[3] = {};
    if (!packet.linear_x.empty()) {
        IMUacc_c[0] = packet.linear_x.back();
    }
    if (!packet.linear_y.empty()) {
        IMUacc_c[1] = packet.linear_y.back();
    }
    if (!packet.linear_z.empty()) {
        IMUacc_c[2] = packet.linear_z.back();
    }

    double IMUgyro_c[3] = {};
    if (!packet.angular_x.empty()) {
        IMUgyro_c[0] = packet.angular_x.back();
    }
    if (!packet.angular_y.empty()) {
        IMUgyro_c[1] = packet.angular_y.back();
    }
    if (!packet.angular_z.empty()) {
        IMUgyro_c[2] = packet.angular_z.back();
    }

    double s_time = packet.s_time;
    

    // argInit_1x3_real_T(cent_pos_est);
    b_gyro[0] = b_acc_o[0];
    b_gyro[1] = b_acc_o[1];
    b_gyro[2] = b_acc_o[2];

    IMUpos(IMUacc_c, IMUgyro_c, s_time, b_acc_o, b_gyro, mode, &kf_psi,
        &gyro_psi, cent_pos_est, cent_vel_est, &state_o, &acc_b_phi,
        &acc_b_theta);

// Arguments    : const double IMUacc_c[3]
//                const double IMUgyro_c[3]
//                double s_time
//                double b_acc_o[3]
//                double b_gyro[3]
//                double mode
//                double *kf_psi
//                double *gyro_psi
//                double cent_pos_est[3]
//                double cent_vel_est[3]
//                double *state_o
//                double *acc_b_phi
//                double *acc_b_theta
    UWBpos6(Ln_tmp, Ln_tmp, Ln_tmp, Ln_tmp, Ln_tmp, RxIDin_tmp, RxIDin_tmp,
          Ln_tmp, tag_pos_b_tmp, xain_tmp, xain_tmp, tag_pos_b_tmp, Ln_tmp,
          UWBout);

    

    // b_gyro[0] = b_gyro[0];
    // b_gyro[1] = b_gyro[1];
    // b_gyro[2] = b_gyro[2];

    // ipe::Imupos imu_pos_msg;
    // imu_pos_msg.header.stamp = ros::Time::now();
    // imu_pos_msg.header.frame_id = "IMUPos";

    // imu_pos_msg.state_o = state_o;
    // imu_pos_msg.b_acc_o.push_back(b_acc_o[0]);
    // imu_pos_msg.b_acc_o.push_back(b_acc_o[1]);
    // imu_pos_msg.b_acc_o.push_back(b_acc_o[2]);

    // imu_pos_msg.b_acc_o.push_back(b_gyro[0]);
    // imu_pos_msg.b_acc_o.push_back(b_gyro[1]);
    // imu_pos_msg.b_acc_o.push_back(b_gyro[2]);

    // imu_pos_msg.acc_b_theta = acc_b_theta;
    // imu_pos_msg.acc_b_phi = acc_b_phi;

    // imu_pos_msg.gyro_psi = gyro_psi;

    // imu_pos_msg.cent_pos_est.push_back(cent_pos_est[0]);
    // imu_pos_msg.cent_pos_est.push_back(cent_pos_est[1]);
    // imu_pos_msg.cent_pos_est.push_back(cent_pos_est[2]);

    // imu_pos_msg.cent_vel_est.push_back(cent_vel_est[0]);
    // imu_pos_msg.cent_vel_est.push_back(cent_vel_est[1]);
    // imu_pos_msg.cent_vel_est.push_back(cent_vel_est[2]);

    // imu_pos_msg.kf_psi = kf_psi;

    // pub.publish(imu_pos_msg);

    sendEvent(state_o);
    
}
