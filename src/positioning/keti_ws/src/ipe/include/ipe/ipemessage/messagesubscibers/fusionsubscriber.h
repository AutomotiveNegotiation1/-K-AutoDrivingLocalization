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

#ifndef FUSIONSUBSCRIBER_H
#define FUSIONSUBSCRIBER_H
#include <ros/ros.h>
#include <complex>
#include <vector>
#include <ipe/Imupos.h>
#include <ipe/Uwbpos.h>
#include <ipe/Fusion.h>
#include "packetcallback.h"
#include "ipecallback.h"
#include "ipedatapacket.h"
#include "socketmanager.h"

#include "fusion.h"
#include "fusion_terminate.h"
#include "rt_nonfinite.h"


/* Function Declarations */
// inline void computePrevTagPos(real_T cent_pos_est_x, real_T cent_pos_est_y)
// {
//     std::complex<real_T> j(0, 1); // 복소수 단위

//     for (int i = 0; i < 4; ++i) {
//         std::complex<real_T> cent_pos_est(cent_pos_est_x, cent_pos_est_y);
//         std::complex<real_T> current_tag_pos_b(tag_pos_b[i].re, tag_pos_b[i].im);
        
//         std::complex<real_T> TagPos = cent_pos_est + std::exp(j * (-kf_psi)) * (current_tag_pos_b + 0.4 * j);
        
//         prevTagPos[i].re = TagPos.real();
//         prevTagPos[i].im = TagPos.imag();
//     }
// }


// /*
//  * Arguments    : void
//  * Return Type  : double
//  */
// // static double argInit_real_T(void)
// // {
// //   return 0.0;
// // }


/* Function Declarations */
struct FusionSubscriber {

private:
    IPECallback* m_ipeCallback;
    double init_flag_;
    double state_IMU;
    double kalman_on = 1;
    double imuNum = 0;
    bool clientAdded; // Add this flag
    bool uwbcall = false;
    bool imucall = false;

    // creal_T tag_center_vel_est;
    // creal_T tag_pos_est[4];
    // creal_T prevTagPos[4];
    // creal_T tag_pos_b[4];
    // double UWBErrSum;
    // double heading_est;
    // double Nanchor;
    // double init_flag;
    // double zt_b;
    // double gyro_psi;

    // double state_o;
    // double b_acc_o[3] = {};
    // double acc_b_theta;
    // double acc_b_phi;
    // double cent_pos_est[3] = {};
    // double cent_vel_est[3] = {};
    // double kf_psi;
    // double prevTagHeading;

public:
    // UDP Socket Variables
    ros::Subscriber subUWB;
    ros::Subscriber subIMU;
    ros::Publisher pub;
    SocketManager* socketManager;
    socklen_t clientSize;
    char buffer[1024];
    std::string messageToSend = "This is a test message.";
    std::string muwbNum;
    double center_x = 0;
    double center_y = 0;
    double heading = 0;

    

    FusionSubscriber(ros::NodeHandle& node) {
        std::cout << "FUSION test setup starting..." << std::endl;
        socketManager = SocketManager::getInstance(); // <-- Add this line to initialize the socketManager
        socketManager->addClient("192.168.4.178", 54000);

        // subUWB = node.subscribe<ipe::Uwbpos>("/UWB", 10, _UWB_callback, this);
        // subIMU = node.subscribe<ipe::Imupos>("/IMU", 10, _IMU_callback, this);

        // subUWB = node.subscribe<ipe::Uwbpos>("/UWB", 10, boost::bind(&FusionSubscriber::_UWB_callback, this, _1));
        // subIMU = node.subscribe<ipe::Imupos>("/IMU", 10, boost::bind(&FusionSubscriber::_IMU_callback, this, _1));

        // pub = node.advertise<ipe::Fusion>("/Fusion", 10);
    }


    void onUWBDataReceived(double data, std::string& uwbNum) {
        init_flag_ = data;
        uwbcall = true;
        processPacketData();
    }

    void onIMUDataReceived(double data) {
        state_IMU = data;
        imuNum++;
        imucall = true;
        processPacketData();
    }

private:
    // void _UWB_callback(const ipe::Uwbpos::ConstPtr& msg) {
    //     UWBErrSum = msg->UWBErrSum;
    //     heading_est = msg->heading_est;
    //     Nanchor = msg->Nanchor;
    //     init_flag = msg->init_flag;
    //     zt_b = msg->zt_b;

    //     tag_center_vel_est.re = msg->tag_center_vel_est[0];
    //     tag_center_vel_est.im = msg->tag_center_vel_est[1];

    //     for (int i = 0; i < 4; ++i) {
    //         tag_pos_est[i].re = msg->tag_pos_est[i];
    //         tag_pos_est[i].im = msg->tag_pos_est[i+1];
    //         tag_pos_b[i].re = msg->tag_pos_est[i];
    //         tag_pos_b[i].im = msg->tag_pos_est[i+1];
    //     }

    //     uwbcall = true;
    // }
    // void _IMU_callback(const ipe::Imupos::ConstPtr& msg){
    //     state_o = msg->state_o;
    //     acc_b_theta = msg->acc_b_theta;
    //     acc_b_phi = msg->acc_b_phi;
    //     kf_psi = msg->kf_psi;
    //     imuNum++;

    //     for (int i=0;i<4;i++){
    //         b_acc_o[i] = msg->b_acc_o[i];
    //         // cent_pos_est[i] = msg->cent_pos_est[i];
    //         // cent_vel_est[i] = msg->cent_vel_est[i];
    //     }
    //     imucall = true;
    // }

    void setupSubscirber(ros::NodeHandle& node){
        // source code : publish vehicle pos based on socket
    }

    void sendUDPMessage(double center_x, double center_y, double heading) {
        std::ostringstream oss;
        oss << center_x << "," << center_y << "," << heading;
        std::string result = oss.str();
        socketManager->broadcastUDPMessage(result);

    }

    void processPacketData() {
        // ipe::Fusion fusion_msg;
        // fusion_msg.header.stamp = ros::Time::now();
        // fusion_msg.header.frame_id = "Fusions";

        if (imucall == true) {
            sendUDPMessage(cent_pos_est[0], cent_pos_est[1], -kf_psi);
            // fusion_msg.cent_pos_est.push_back(cent_pos_est[0]);
            // fusion_msg.cent_pos_est.push_back(cent_pos_est[1]);
            // fusion_msg.cent_pos_est.push_back(cent_pos_est[2]);
            // fusion_msg.kf_psi = kf_psi;
            imucall = false;
        }
        else if (uwbcall == true) {
            creal_T IMUposU;

            IMUposU = fusion(&tag_center_vel_est, state_o, Nanchor, b_acc_o,
                    acc_b_theta, &acc_b_phi, UWBErrSum, init_flag,
                    kalman_on, imuNum, cent_pos_est, cent_vel_est, &kf_psi,
                    tag_pos_est, heading_est, zt_b);

            if (init_flag == 1) {
                gyro_psi = -heading_est;
                kf_psi = gyro_psi;
                cent_pos_est[0] = IMUposU.re;
                cent_pos_est[1] = IMUposU.im;
                cent_pos_est[2] = 0; 
                cent_vel_est[0] = tag_center_vel_est.re;
                cent_vel_est[1] = tag_center_vel_est.im;
                cent_vel_est[2] = 0;
            }

            if (imucall == true && init_flag == 1){
                init_flag = 2;
            }
            uwbcall = false;

            std::complex<real_T> j(0, 1); // 복소수 단위

            for (int i = 0; i < 4; ++i) {
                std::complex<real_T> cent_pos_est_(IMUposU.re, IMUposU.im);
                std::complex<real_T> current_tag_pos_b(tag_pos_b[i].re, tag_pos_b[i].im);
                
                std::complex<real_T> TagPos = cent_pos_est_ + std::exp(j * (-kf_psi)) * (current_tag_pos_b + 0.4 * j);
                double d = TagPos.real();
                // fusion_msg.prevTagPos.push_back(TagPos.real());
                // fusion_msg.prevTagPos.push_back(TagPos.imag());
            }
            prevTagHeading = -kf_psi;

            // ROS_INFO("TagPos : (%f,%f)",cent_pos_est[0], cent_pos_est[1]);
            // ROS_INFO("Heading : (%f)", prevTagHeading);

            sendUDPMessage(IMUposU.re, IMUposU.im, prevTagHeading);
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
        // pub.publish(fusion_msg);
    }

};


#endif