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

#ifndef MAINPOSITIONING_H
#define MAINPOSITIONING_H

#include <QThread>

#include <ros/ros.h>
#include <rosbag/bag.h>
#include <rosbag/view.h>
#include <visual/Anchor.h>

#include <vector>
#include <map>
#include <chrono>

#include "kapcallback.h"
#include "uwbsubscriber.h"
#include "UWBpos6_terminate.h"
#include "kapdatapacket.h"



struct UwbSubscriber;

class MainPositioning : public QThread {
    Q_OBJECT

protected:
    void registerSubcribers(ros::NodeHandle &nh_);
    void registerCallback(UwbSubscriber *cb);
    double convertToDouble(const ros::Time& time);
    void spinFor();
    void run();

signals:
    void newPositionData(const PosDataPacket& data);

private:
    ros::NodeHandle &nh_;
    std::list<UwbSubscriber *> m_callbacks;
    KapCallback m_kapCallback;
    KapDataPacket m_kapDatapacket; 
    PosDataPacket pos;

//2023.09.15
private:
    std::function<void(const PosDataPacket&)> callback_;  // Store the callback

public:
    MainPositioning(ros::NodeHandle &nh) : nh_(nh) {}
    ~MainPositioning();
};

#endif // MAINPOSITIONING_H