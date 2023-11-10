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

#include "UWBpos6.h"
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

// Declare Initialization Functions
inline creal_T argInit_creal_T(int idx);
inline void argInit_1x4_creal_T(creal_T result[4]);
inline creal_T argInit_creal_preT(int idx);
inline void argInit_1x4_creal_preT(creal_T result[4]);



class UwbSubscriber : public PacketCallback {
private:
    // Member Variables
    ros::Subscriber sub;
    ros::Subscriber subFusion;
    ros::Publisher pub;
    std::string frame_id;
    std::ostringstream topic_name_stream;
    IPECallback* m_ipeCallback;
    IPEDataPacket m_ipeDataPacket;
    std::vector<std::function<void(double, std::string&)>> callbacks;
    std::string uwbNum;

    // creal_T tag_pos_b[4];
    // creal_T prevTagPos[4];
    // creal_T tag_pos_est[4];
    // creal_T tag_pos_est_aver[4];
    // creal_T tag_center_vel_est;
    // std::string tagNum;
    // double UWBErrSum;
    // double heading_est;
    // double headingest_a_aver_v;
    // double init_flag;
    // double Nanchor;
    // double zt_b;
    // double prevTagHeading;
        
    // Constants
    const double Ln = 6.0;
    const double Lp = 4.0;

    // Private Member Functions
    void setupSubscriber(ros::NodeHandle& node, const std::string& uwbNum);
    void _callback(const ipe::Anchor::ConstPtr& msg);
    void _callback_Fusion(const ipe::Fusion::ConstPtr& msg);
    void _setRxid(const ipe::Anchor::ConstPtr& msg);
    double extractNumber(const std::string& input);
    void processPacketData(IPEDataPacket &packet, double timestamp);
    // void sendUDPMessage(double center_x, double center_y, double heading);

public:
    // Constructor
    UwbSubscriber(ros::NodeHandle& node, const std::string& uwbNum, IPECallback* ipeCallback);
    ~UwbSubscriber();
    
    // Operator Overload
    void operator()(IPEDataPacket &packet, double timestamp);
    
    // Public Member Functions
    void registerCallback(const std::function<void(double, std::string&)>& callback);
    void sendEvent(double data, std::string& uwbNum);
    std::string getPacketFrameID();
};

#endif // UWBSUBSCRIBER_H

