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

#ifndef UWBSUBSCRIBER_H
#define UWBSUBSCRIBER_H

#include <sstream>
#include <set>
#include <unordered_map>
#include <vector>
#include <functional>
#include <ros/ros.h>
#include <ipe/Anchor.h>
#include <ipe/Uwbpos.h>
#include <ipe/Fusion.h>
#include <sstream>

#include "packetcallback.h"
#include "ipecallback.h"
#include "ipedatapacket.h"
#include "posdatapacket.h"
#include "socketmanager.h"

#include "imusubscriber.h"

#include "UWBpos6.h"
#include "rt_nonfinite.h"

#include "fusion2.h"
#include "fusion2_terminate.h"
#include "rt_nonfinite.h"

// Declare Global Variables
extern std::vector<double> xain_list;
extern std::vector<double> yain_list;
extern std::vector<double> xain_difference;
extern std::vector<double> yain_difference;
extern double UWBErrSum;
extern std::vector<std::string> RxID_data_list;
extern std::vector<int> RxID_list;
extern bool statusUWB;
extern std::vector<std::string> difference;
extern creal_T tag_pos_b[4];
extern creal_T prevTagPos[4];
extern std::string tagNum;
extern creal_T tag_pos_est[4];
extern creal_T tag_pos_est_aver[4];
extern creal_T tag_center_vel_est;
extern double heading_est;
extern double headingest_a_aver_v;
extern double init_flag;
extern double Nanchor;
extern double zt_b;
extern double prevTagHeading;
extern double kl;

// Declare Initialization Functions
inline void computePrevTagPos(real_T cent_pos_est_x, real_T cent_pos_est_y);
inline creal_T argInit_creal_T(int idx);
inline void argInit_1x4_creal_T(creal_T result[4]);
inline creal_T argInit_creal_preT(int idx);
inline void argInit_1x4_creal_preT(creal_T result[4]);

class FusionSubscriber;

class UwbSubscriber : public PacketCallback {
private:
    // Member Variables
    ros::Publisher pub;
    ros::NodeHandle &node;
    ros::Subscriber r_sh_UWB;
    IPECallback* o_ipeCallback;
    IPEDataPacket m_ipeDataPacket;
    std::string m_uwbNum;
    std::string m_frameId;
    std::vector<std::function<void(int)>> callbacks;
        
    // Constants
    const double Ln = 6.0;
    const double Lp = 4.0;
    double kf_psi_1 = 0;

    SocketManager* socketManager;
    double center_x = 0;
    double center_y = 0;
    double heading = 0;
    double Acc_Vel[60] = {};
    double Acc_Pos[60] = {};
    double kalman_on = 1;

    creal_T IMUposU;
    
private:
    // Private Member Functions
    void _callback(const ipe::Anchor::ConstPtr& msg);
    void _callback_Fusion(const ipe::Fusion::ConstPtr& msg);
    void _setRxid(const ipe::Anchor::ConstPtr& msg);
    double extractNumber(const std::string& input);
    void processPacketData(IPEDataPacket &packet, double timestamp, FusionSubscriber* _fusionSubscriber);
    // void sendUDPMessage(double center_x, double center_y, double heading);

    void sendUDPMessage(double center_x, double center_y, double heading);

public:
    // Constructor
    UwbSubscriber(ros::NodeHandle& _node, const std::string& _uwbNum, IPECallback* _ipeCallback);
    ~UwbSubscriber();

    void setupSubscriber(const std::string& uwbNum);
    void operator()(IPEDataPacket &_packet, double _timestamp, FusionSubscriber* _fusionSubscriber);
    
};

#endif // UWBSUBSCRIBER_H