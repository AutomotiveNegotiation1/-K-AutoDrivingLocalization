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
<<<<<<< HEAD
    socketManager->addClient("192.168.4.178", 54000);
=======
    // socketManager->addClient("192.168.4.178", 54000);
>>>>>>> main
}

void FusionSubscriber::sendUDPMessage(double center_x, double center_y, double heading) {
    std::ostringstream oss;
    oss << center_x << "," << center_y << "," << heading;
    std::string result = oss.str();
    socketManager->broadcastUDPMessage(result);

}

void FusionSubscriber::processPacketData(int num) {
    // ipe::Fusion fusion_msg;
    // fusion_msg.header.stamp = ros::Time::now();
    // fusion_msg.header.frame_id = "Fusions";
    num_ = num;

    if (num_ == 1) {
        sendUDPMessage(cent_pos_est[0], cent_pos_est[1], -kf_psi);
        // fusion_msg.cent_pos_est.push_back(cent_pos_est[0]);
        // fusion_msg.cent_pos_est.push_back(cent_pos_est[1]);
        // fusion_msg.cent_pos_est.push_back(cent_pos_est[2]);
        // fusion_msg.kf_psi = kf_psi;
        imucall = false;
    }
    else if (num_ == 2) {


        IMUposU = fusion2(kl, imuNum, tag_pos_est, tag_center_vel_est, cent_pos_est,
                cent_vel_est, b_acc_o, acc_b_phi, Acc_Pos, Acc_Vel, kalman_on, init_flag, UWBErrSum, &kf_psi,
                &gyro_psi, heading_est, acc_b_theta, 2, Nanchor, state_o);

        
<<<<<<< HEAD
=======
        
>>>>>>> main
        uwbcall = false;

        // ROS_INFO("TagPos : (%f,%f)",cent_pos_est[0], cent_pos_est[1]);
        // ROS_INFO("Heading : (%f)", prevTagHeading);

        sendUDPMessage(IMUposU.re, IMUposU.im, -kf_psi);
        // fusion_msg.cent_pos_est.push_back(cent_pos_est[0]);
        // fusion_msg.cent_pos_est.push_back(cent_pos_est[1]);
        // fusion_msg.cent_pos_est.push_back(cent_pos_est[2]);
        
        // fusion_msg.cent_vel_est.push_back(cent_vel_est[0]);
        // fusion_msg.cent_vel_est.push_back(cent_vel_est[1]);
        // fusion_msg.cent_vel_est.push_back(cent_vel_est[2]);

        // fusion_msg.acc_b_phi = acc_b_phi;
        // fusion_msg.prevTagHeading = prevTagHeading;
        // fusion_msg.init_flag = init_flag;
        // fusion_msg.gyro_psi = gyro_psi;
    }
    // // pub.publish(fusion_msg);

}