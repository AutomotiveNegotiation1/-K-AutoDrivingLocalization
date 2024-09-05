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

#ifndef IPEINTERFACE_H
#define IPEINTERFACE_H

#include <ros/ros.h>
#include <rosbag/bag.h>
#include <rosbag/view.h>

//#include <ipe/Anchor.h>

#include <ipe_v4_2/Anchor.h>  //joo.hy (06/17)

#include <complex>

#include <functional>
#include <vector>
#include <map>
#include <chrono>
#include <thread>

#include "ipecallback.h"
#include "ipedatapacket.h"
#include "packetcallback.h"
#include "uwbsubscriber.h"

//#include "imusubscriber.h"

// #include "fusion2_terminate.h"
#include "sensordata.h"
#include "slamsubscriber_ipe_v4_2.h"

class PacketCallback;
struct uSensorData {
    int No;
    double Tag1;
    double Tag2;
    double Tag3;
    double Tag4;
    double IMU;
    //hy.joo(24/06/05)
    double Slam; 
};

class IPEInterface {

private:
    void run();
    void spinFor();
    void registerCallback(PacketCallback *_cb);
    void registerSubcribers(ros::NodeHandle &_node);
    void testPositioning();
    void Positioning();

private:
    int no= 1;
    int flag2 = 1;
    UwbSubscriber* o_uwb;
    //ImuSubscriber* o_imu;
    SlamSubscriber* o_slam; 

    ros::NodeHandle &r_nh;
    std::list<PacketCallback *> l_callbacks;
    IPECallback o_ipeCallback;
    bool m_test;

public:
    std::vector<uSensorData> data;  // SensorData 객체들을 저장할 벡터
    std::vector<SensorData> uwbdata;  // SensorData 객체들을 저장할 벡터
    IPEInterface(ros::NodeHandle &_nh, bool _test);
    ~IPEInterface();
};

#endif // MAINPOSITIONING_H