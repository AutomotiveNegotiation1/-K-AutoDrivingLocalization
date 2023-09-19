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

#include <QObject>
#include <sstream>
#include <set>
#include <ros/ros.h>
#include <sensor_msgs/Imu.h>

#include <functional>

#include "packetcallback.h"
#include "kuipcallback.h"
#include "kuipdatapacket.h"
#include "posdatapacket.h"

struct ImuSubscriber : public QObject, public PacketCallback {
    Q_OBJECT

private:
    ros::Subscriber sub;
    std::string frame_id = "imu";
    std::ostringstream topic_name_stream;
    KuipCallback* m_kapCallback;
    KuipDataPacket m_kapDataPacket;
    std::vector<std::function<void(PosDataPacket)>> callbacks;

    // Constants
    const double Ln = 6.0;
    const double Lp = 4.0;

signals:
    void newPositionData(const PosDataPacket& data);

public:
    ImuSubscriber(ros::NodeHandle& node, KuipCallback* kapCallback) 
        : m_kapCallback(kapCallback) {
        setupSubscriber(node);
    }
    virtual ~ImuSubscriber() {}


    void operator()(KuipDataPacket &packet, double timestamp) {
        processPacketData(packet, timestamp);
    }

    void registerCallback(const std::function<void(PosDataPacket)>& callback) {
        callbacks.push_back(callback);
    }

    void sendEvent(PosDataPacket data) {
        for (const auto& callback : callbacks) {
            callback(data);
        }
    }

    void onUWBDataReceived(PosDataPacket data) {
        // std::cout << "IMU received UWB data: " << data << std::endl;
        // IMU는 UWB 데이터를 받으면 자신의 이벤트도 발생시킬 수 있습니다.
        // sendEvent(data + 10);  // 예제로, UWB 데이터에 10을 더해 전송한다고 가정
    }

private:
    void setupSubscriber(ros::NodeHandle& node) {
        topic_name_stream << "/zed_f9r/imu";

        std::string topic_name = topic_name_stream.str();

        ROS_INFO("topic_name-->%s", topic_name.c_str());

        sub = node.subscribe<sensor_msgs::Imu>(topic_name, 10, &ImuSubscriber::_callback, this);
    }

    void _callback(const sensor_msgs::Imu::ConstPtr& msg) {
        m_kapDataPacket = KuipDataPacket(msg);
        
        if (m_kapCallback) {
            m_kapCallback->onLiveDataAvailable(m_kapDataPacket);
        } else {
            ROS_WARN("m_kapCallback is a nullptr!");
        }
    }

    void processPacketData(KuipDataPacket &packet, double timestamp) 
    {

    }
};


#endif
