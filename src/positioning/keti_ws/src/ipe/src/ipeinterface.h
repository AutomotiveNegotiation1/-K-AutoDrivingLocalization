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
#include <ipe/Anchor.h>
#include <functional>
#include <vector>
#include <map>
#include <chrono>
#include <thread>

#include "ipecallback.h"
#include "ipedatapacket.h"

#include "packetcallback.h"
#include "uwbsubscriber.h"
#include "imusubscriber.h"
#include "fusionsubscriber.h"

#include "fusion_terminate.h"



class PacketCallback;

class IPEInterface {

protected:
    void registerSubcribers(ros::NodeHandle &nh_);
    void registerCallback(PacketCallback *cb);
    double convertToDouble(const ros::Time& time);
    void spinFor();


private:
    int uwbInit = 0;
    int imuInit = 0;
    FusionSubscriber* fusion;
    ros::NodeHandle &nh_;
    std::list<PacketCallback *> m_callbacks;
    IPECallback m_ipeCallback;
    IPEDataPacket m_ipeDatapacket; 
    PosDataPacket pos;

//2023.09.15
private:
    std::function<void(const PosDataPacket&)> callback_;  // Store the callback

public:
    void run();
    IPEInterface(ros::NodeHandle &nh) : nh_(nh){}
    ~IPEInterface();
};

#endif // MAINPOSITIONING_H