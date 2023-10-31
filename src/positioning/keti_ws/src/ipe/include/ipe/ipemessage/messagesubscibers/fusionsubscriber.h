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
#include "packetcallback.h"
#include "ipecallback.h"
#include "ipedatapacket.h"
#include "socketmanager.h"

#include "fusion.h"
#include "fusion_terminate.h"
#include "rt_nonfinite.h"


/* Function Declarations */
inline void computePrevTagPos(real_T cent_pos_est_x, real_T cent_pos_est_y)
{
    std::complex<real_T> j(0, 1); // 복소수 단위

    for (int i = 0; i < 4; ++i) {
        std::complex<real_T> cent_pos_est(cent_pos_est_x, cent_pos_est_y);
        std::complex<real_T> current_tag_pos_b(tag_pos_b[i].re, tag_pos_b[i].im);
        
        std::complex<real_T> TagPos = cent_pos_est + std::exp(j * (-kf_psi)) * (current_tag_pos_b + 0.4 * j);
        
        prevTagPos[i].re = TagPos.real();
        prevTagPos[i].im = TagPos.imag();
    }
}


/*
 * Arguments    : void
 * Return Type  : double
 */
// static double argInit_real_T(void)
// {
//   return 0.0;
// }


/* Function Declarations */
struct FusionSubscriber {

private:
    IPECallback* m_ipeCallback;
    double init_flag_;
    double state_IMU;
    double kalman_on = 1;
    double imuNum = 0;
    bool clientAdded; // Add this flag

public:
    // UDP Socket Variables
    SocketManager* socketManager;
    socklen_t clientSize;
    char buffer[1024];
    std::string messageToSend = "This is a test message.";
    std::string muwbNum;
    double center_x = 0;
    double center_y = 0;
    double heading = 0;

    FusionSubscriber() {
        socketManager = SocketManager::getInstance(); // <-- Add this line to initialize the socketManager
        socketManager->addClient("192.168.3.198", 54000);
    }


    void onUWBDataReceived(double data, std::string& uwbNum) {
        init_flag_ = data;
        processPacketData();
    }

    void onIMUDataReceived(double data) {
        state_IMU = data;
        imuNum++;
        processPacketData();
    }

private:
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
        /*
        * Arguments    : double k
        *                double state_IMU
        *                double Nanchor
        *                const double b_acc_o[3]
        *                double acc_b_theta
        *                const double *acc_b_phi
        *                const emxArray_real_T *UWBErrSum
        *                double init_flag
        *                double kalman_on
        *                const double k0[5]
        *                double cent_pos_est[3]
        *                double cent_vel_est[3]
        *                double *kf_psi
        *                const creal_T tag_pos_est[4]
        *                double heading_est
        *                double zt_b
        * Return Type  : creal_T
        */
        creal_T IMUposU;

        // result = fusion(&tag_center_vel_est, state_IMU, Nanchor, b_acc_o,
        //                 acc_b_theta, &acc_b_phi, UWBErrSum, init_flag,
        //                 kalman_on, imuNum, cent_pos_est, cent_vel_est, &kf_psi,
        //                 tag_pos_est, heading_est, zt_b);

        IMUposU = fusion(&tag_center_vel_est, state_IMU, Nanchor, b_acc_o,
                   acc_b_theta, &acc_b_phi, UWBErrSum, init_flag,
                   kalman_on, imuNum, cent_pos_est, cent_vel_est, &kf_psi,
                   tag_pos_est, heading_est, zt_b);

        if (init_flag == 1){
            gyro_psi = -heading_est;
            kf_psi = gyro_psi;
            cent_pos_est[0] = IMUposU.re;
            cent_pos_est[1] = IMUposU.im;
            cent_pos_est[1] = 0;
            cent_vel_est[0] = tag_center_vel_est.re;
            cent_vel_est[1] = tag_center_vel_est.im;
            cent_vel_est[2] = 0;
        }

        if (signalIMU == 1 && init_flag == 1){
            init_flag = 2;
        }
        signalIMU = 0;
        computePrevTagPos(IMUposU.re, IMUposU.im);
        prevTagHeading = -kf_psi;

        ROS_INFO("TagPos : (%f,%f)",cent_pos_est[0], cent_pos_est[1]);
        ROS_INFO("Heading : (%f)", prevTagHeading);

        sendUDPMessage(cent_pos_est[0], cent_pos_est[1], prevTagHeading);
    }

};


#endif