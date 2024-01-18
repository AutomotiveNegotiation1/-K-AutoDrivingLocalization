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
#ifndef IMUSUBSCRIBER_H
#define IMUSUBSCRIBER_H

#include <sstream>
#include <set>
#include <ros/ros.h>
#include <sensor_msgs/Imu.h>
#include <functional>
#include "packetcallback.h"
#include "ipecallback.h"
#include "ipedatapacket.h"
#include "uwbsubscriber.h"

#include "socketmanager.h"

#include "IMUpos.h"
#include "rt_nonfinite.h"
#include "sensordata.h"

// Declare Global Variables
extern double b_acc_o[3];
extern double b_gyro[3];
extern double mode;
extern double kf_psi;
extern double gyro_psi;
extern double cent_pos_est[3];
extern double cent_vel_est[3];
extern double state_o;
extern double acc_b_phi;
extern double acc_b_theta;
extern double signalIMU;
extern double imuNum;

class ImuSubscriber : public PacketCallback {
private:
    ros::Publisher pub;
    ros::Subscriber sub;
    ros::Subscriber subFusion;
    std::string frame_id = "imu";
    std::ostringstream topic_name_stream;

    SocketManager* socketManager;

    IPECallback* m_ipeCallback;
    IPEDataPacket m_ipeDataPacket;
    std::vector<std::function<void(int)>> callbacks;

    const double Ln = 6.0;
    const double Lp = 4.0;

public:
    ImuSubscriber(ros::NodeHandle& node, IPECallback* ipeCallback);
    ~ImuSubscriber();
    void operator()(IPEDataPacket &packet, double timestamp, SensorData* data);
    void registerCallback(const std::function<void(int)>& callback);
    void sendEvent(int data);
    std::string getPacketFrameID();

    void sendUDPMessage(double center_x, double center_y, double heading);

private:
    void setupSubscriber(ros::NodeHandle& node);
    void _callback(const sensor_msgs::Imu::ConstPtr& msg);
    void processPacketData(IPEDataPacket &packet, double timestamp,  SensorData* data);
};

#endif // IMUSUBSCRIBER_H
