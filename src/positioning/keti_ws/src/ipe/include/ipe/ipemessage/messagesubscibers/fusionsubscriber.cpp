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

#include "fusionsubscriber.h"

FusionSubscriber::FusionSubscriber(ros::NodeHandle& node) {
    std::cout << "FUSION test setup starting..." << std::endl;
    socketManager = SocketManager::getInstance(); // <-- Add this line to initialize the socketManager
    // socketManager->addClient("192.168.4.178", 54000);
}

void FusionSubscriber::sendUDPMessage(double center_x, double center_y, double heading) {
    std::ostringstream oss;
    oss << center_x << "," << center_y << "," << heading;
    std::string result = oss.str();
    socketManager->broadcastUDPMessage(result);
}


void FusionSubscriber::onUWBDataReceived(int data){
    std::cout << "UWB data received" << data << std::endl;
    sendUDPMessage(cent_pos_est[0], cent_pos_est[1], -kf_psi);
}

void FusionSubscriber::onIMUDataReceived(int data) {
    std::cout << "IMU data received" << data << std::endl;
    processPacketData(data);
}

void FusionSubscriber::processPacketData(int num) {
    // // ipe::Fusion fusion_msg;
    // // fusion_msg.header.stamp = ros::Time::now();
    // // fusion_msg.header.frame_id = "Fusions";
    // num_ = num;

    // if (num_ == 1) {
    //     sendUDPMessage(cent_pos_est[0], cent_pos_est[1], -kf_psi);
    //     // fusion_msg.cent_pos_est.push_back(cent_pos_est[0]);
    //     // fusion_msg.cent_pos_est.push_back(cent_pos_est[1]);
    //     // fusion_msg.cent_pos_est.push_back(cent_pos_est[2]);
    //     // fusion_msg.kf_psi = kf_psi;
    //     imucall = false;
    // }
    // else if (num_ == 2) {


    //     IMUposU = fusion2(kl, imuNum, tag_pos_est, tag_center_vel_est, cent_pos_est,
    //             cent_vel_est, b_acc_o, acc_b_phi, Acc_Pos, Acc_Vel, kalman_on, init_flag_, UWBErrSum, &kf_psi,
    //             &gyro_psi, heading_est, acc_b_theta, 2, Nanchor, state_o);

        
        
    //     uwbcall = false;

    //     // ROS_INFO("TagPos : (%f,%f)",cent_pos_est[0], cent_pos_est[1]);
    //     // ROS_INFO("Heading : (%f)", prevTagHeading);

    //     sendUDPMessage(IMUposU.re, IMUposU.im, -kf_psi);

        
    //     if (num_ == 1 && init_flag == 1){
    //         init_flag = 2;
    //     }
    //     std::complex<real_T> j(0, 1); // 복소수 단위

    //     // creal_T cent_pos_est_;
    //     creal_T current_tag_pos_b[4];
    //     for (int i = 0; i < 4; ++i) {
    //         cent_pos_est[0] = IMUposU.re;
    //         cent_pos_est[1] = IMUposU.im;
    //         current_tag_pos_b[i].re = tag_pos_b[i].re;
    //         current_tag_pos_b[i].im = tag_pos_b[i].im;
                    
    //         std::complex<double> cent_pos_est_c(IMUposU.re, IMUposU.im);
    //         std::complex<double> current_tag_pos_b_c(current_tag_pos_b[i].re, current_tag_pos_b[i].im);
    //         std::complex<double> TagPos = cent_pos_est_c + std::exp(j * (-kf_psi)) * (current_tag_pos_b_c + 0.4 * j);
    //         creal_T TagPos_;
    //         TagPos_.re = std::real(TagPos);
    //         TagPos_.im = std::imag(TagPos);
    //         prevTagPos[i].re = TagPos_.re;
    //         prevTagPos[i].im = TagPos_.im;
    //     }
        
    //     prevTagHeading = -kf_psi;
    //     kf_psi = kf_psi;
        
    //     // fusion_msg.cent_pos_est.push_back(cent_pos_est[0]);
    //     // fusion_msg.cent_pos_est.push_back(cent_pos_est[1]);
    //     // fusion_msg.cent_pos_est.push_back(cent_pos_est[2]);
        
    //     // fusion_msg.cent_vel_est.push_back(cent_vel_est[0]);
    //     // fusion_msg.cent_vel_est.push_back(cent_vel_est[1]);
    //     // fusion_msg.cent_vel_est.push_back(cent_vel_est[2]);

    //     // fusion_msg.acc_b_phi = acc_b_phi;
    //     // fusion_msg.prevTagHeading = prevTagHeading;
    //     // fusion_msg.init_flag = init_flag;
    //     // fusion_msg.gyro_psi = gyro_psi;
        
    // // pub.publish(fusion_msg);
}
