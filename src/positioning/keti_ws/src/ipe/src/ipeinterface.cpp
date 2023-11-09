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

#include "ipeinterface.h"

IPEInterface::~IPEInterface()
{
  ROS_INFO("Cleaning up ...");
  
  // m_callbacks의 각 요소에 대한 메모리 할당 반환
  for(auto &cb : m_callbacks) {
      delete cb;
  }
  m_callbacks.clear();  // 옵션: 리스트를 비운다
  
  fusion_terminate();
}

void IPEInterface::registerSubcribers(ros::NodeHandle &node) {
    bool should_publish;
    fusion = new FusionSubscriber(node);
    std::list<double> totalTag;
    if (ros::param::get("/ipe_node/Tag_on", should_publish) && should_publish) {
        if (ros::param::get("/ipe_node/sub_UWB0", should_publish) && should_publish) {
            UwbSubscriber* uwb1 = new UwbSubscriber(node, "0", &m_ipeCallback);
            uwb1->registerCallback([this](double value, std::string& value2) { fusion->onUWBDataReceived(value, value2); });
            registerCallback(uwb1);
        }
        if (ros::param::get("/ipe_node/sub_UWB1", should_publish) && should_publish) {
            UwbSubscriber* uwb2 = new UwbSubscriber(node, "1", &m_ipeCallback);
            uwb2->registerCallback([this](double value, std::string& value2) { fusion->onUWBDataReceived(value, value2); });
            registerCallback(uwb2);
        }
        if (ros::param::get("/ipe_node/sub_UWB2", should_publish) && should_publish) {
            UwbSubscriber* uwb3 = new UwbSubscriber(node, "2", &m_ipeCallback);
            uwb3->registerCallback([this](double value, std::string& value2) { fusion->onUWBDataReceived(value, value2); });
            registerCallback(uwb3);
        }
        if (ros::param::get("/ipe_node/sub_UWB3", should_publish) && should_publish) {
            UwbSubscriber* uwb4 = new UwbSubscriber(node, "3", &m_ipeCallback);
            uwb4->registerCallback([this](double value, std::string& value2) { fusion->onUWBDataReceived(value, value2); });
            registerCallback(uwb4);
        }
    }

    if (ros::param::get("/ipe_node/IMU_on", should_publish) && should_publish) {
        ImuSubscriber* imu = new ImuSubscriber(node, &m_ipeCallback);
        imu->registerCallback([this](double value, std::string& frameID) { fusion->onIMUDataReceived(value); });

        registerCallback(imu);
    }
}

void IPEInterface::registerCallback(PacketCallback *cb)  // Make sure PacketCallback is defined
{
    m_callbacks.push_back(cb);
}


void IPEInterface::spinFor()
{
	RosKapDataPacket rosPacket = m_ipeCallback.next();
    std::string frame_id1 = rosPacket.second.frame_id;
	if (!rosPacket.second.empty(rosPacket.second.frame_id))
	{
        for (auto &cb : m_callbacks)
		{
            ImuSubscriber* imuSub = dynamic_cast<ImuSubscriber*>(cb);
            std::string frame_id = rosPacket.second.frame_id;
            if (rosPacket.second.frame_id == "imu"){
                if (imuSub){
                    cb->operator()(rosPacket.second, rosPacket.first);
                    m_ipeCallback.pop();
                    break;
                }
            }
            else{
                if (!imuSub){
                    cb->operator()(rosPacket.second, rosPacket.first);
                    m_ipeCallback.pop();
                    break;
                }
            }
		}

	}
}


// 2023.10.05
// void IPEInterface::run()
// {
//     try {
//         registerSubcribers(nh_);
//     } catch (const std::exception& e) {
//         ROS_ERROR("%s", e.what());
//     }

//     while (ros::ok()) {
//         spinFor();
//         ros::spinOnce();
//     }
// }

// 2023.09.14
void IPEInterface::run()
{
    try {
        registerSubcribers(nh_);
    } catch (const std::exception& e) {
        ROS_ERROR("%s", e.what());
    }

    rosbag::Bag bag;
    try {
        bag.open("/home/keti/rosbag/[zed_f9r]2023-08-31-17-58-37_fast.bag", rosbag::bagmode::Read);
    } catch (rosbag::BagException& e) {
        ROS_ERROR("Error opening bag file: %s", e.what());
    }

    std::vector<std::string> topics = {
        "/dwm1001/anchor/ttyUWB0",
        "/dwm1001/anchor/ttyUWB1",
        "/dwm1001/anchor/ttyUWB2",
        "/dwm1001/anchor/ttyUWB3",
        "/zed_f9r/imu"
    };

    std::map<std::string, ros::Publisher> publishers;

    for (const auto& topic : topics) {
        if (topic == "/zed_f9r/imu") {
            publishers[topic] = nh_.advertise<sensor_msgs::Imu>(topic, 10);
        } else {
            publishers[topic] = nh_.advertise<ipe::Anchor>(topic, 10);
        }
    }

    rosbag::View view(bag);

    for (const rosbag::MessageInstance& message : view) {
        if (publishers.find(message.getTopic()) != publishers.end()) {
            if (message.getTopic() == "/zed_f9r/imu") {
                sensor_msgs::Imu::ConstPtr imu_data = message.instantiate<sensor_msgs::Imu>();
                if (imu_data != NULL) {
                    publishers[message.getTopic()].publish(imu_data);
                }
            } else {
                ipe::Anchor::ConstPtr uwb_data = message.instantiate<ipe::Anchor>();
                if (uwb_data != NULL) {
                    publishers[message.getTopic()].publish(uwb_data);
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            spinFor();  // Assuming that 'spinFor()' is some kind of sleep/delay function. If not, please clarify.
            ros::spinOnce();
        }
    }

    bag.close();
    ROS_INFO("Finish...");
}