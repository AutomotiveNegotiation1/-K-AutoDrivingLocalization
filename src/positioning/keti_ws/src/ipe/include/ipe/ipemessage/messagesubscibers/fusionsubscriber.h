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
#include "imusubscriber.h"
#include "uwbsubscriber.h"

#include <ipe/Imupos.h>
#include <ipe/Uwbpos.h>
#include <ipe/Fusion.h>
#include "packetcallback.h"
#include "ipecallback.h"
#include "ipedatapacket.h"
#include "socketmanager.h"

#include "fusion2.h"
#include "fusion2_terminate.h"
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
class FusionSubscriber {

public:
    creal_T IMUposU;
    double init_flag_;
    double state_IMU;
    double kalman_on = 1;
    double kl = 0;
    double imuNum = 0;
    bool clientAdded; // Add this flag
    bool uwbcall = false;
    bool imucall = false;
    int num_;

    // UDP Socket Variables
    ros::Publisher pub;
    SocketManager* socketManager;
    socklen_t clientSize;
    char buffer[1024];
    std::string messageToSend = "This is a test message.";
    std::string muwbNum;
    double center_x = 0;
    double center_y = 0;
    double heading = 0;
    double Acc_Vel[100] = {};
    double Acc_Pos[100] = {};

    
public:
    FusionSubscriber(ros::NodeHandle& node);
    void onUWBDataReceived(int data); 
    void onIMUDataReceived(int data); 
    void processPacketData(int num);

private:

    void sendUDPMessage(double center_x, double center_y, double heading);


};


#endif