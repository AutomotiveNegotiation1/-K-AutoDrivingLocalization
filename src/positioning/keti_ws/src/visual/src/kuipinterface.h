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

#ifndef KUIPINTERFACE_H
#define KUIPINTERFACE_H

#include <QThread>

#include <ros/ros.h>
#include <rosbag/bag.h>
#include <rosbag/view.h>
#include <visual/Anchor.h>

#include <vector>
#include <map>
#include <chrono>

#include "kuipcallback.h"
#include "kuipdatapacket.h"
#include "packetcallback.h"
#include "uwbsubscriber.h"
#include "imusubscriber.h"
#include "mainwindow.h"

#include "UWBpos6_terminate.h"



class PacketCallback;

class kuipInterface : public QThread {
    Q_OBJECT

protected:
    void registerSubcribers(ros::NodeHandle &nh_);
    void registerCallback(PacketCallback *cb);
    double convertToDouble(const ros::Time& time);
    void spinFor();
    void run();

signals:
    void newPositionData(const PosDataPacket& data);

private:
    bool init = false;
    ros::NodeHandle &nh_;
    MainWindow &mw_;
    std::list<PacketCallback *> m_callbacks;
    KuipCallback m_kuipCallback;
    KuipDataPacket m_kapDatapacket; 
    PosDataPacket pos;

//2023.09.15
private:
    std::function<void(const PosDataPacket&)> callback_;  // Store the callback

public:
    kuipInterface(ros::NodeHandle &nh, MainWindow &mw) : nh_(nh),mw_(mw){}
    ~kuipInterface();
};

#endif // MAINPOSITIONING_H