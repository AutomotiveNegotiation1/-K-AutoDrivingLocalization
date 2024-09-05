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

#include <fstream>
#include <vector>
#include <limits>
#include <iomanip> 

IPEInterface::IPEInterface(ros::NodeHandle &_nh, bool _test) : r_nh(_nh), m_test(_test) {
    run();
}

IPEInterface::~IPEInterface()
{
  ROS_INFO("Cleaning up ...");
  
  // m_callbacks의 각 요소에 대한 메모리 할당 반환
  for(auto &cb : l_callbacks) {
      delete cb;
  }
  l_callbacks.clear();  // 옵션: 리스트를 비운다

}


void IPEInterface::registerSubcribers(ros::NodeHandle &_node) {
    bool m_should_publish;
    //o_imu = new ImuSubscriber(_node, &o_ipeCallback);

    std::list<double> totalTag;
    if (ros::param::get("/ipe_v4_2_node/Tag_on", m_should_publish) && m_should_publish) {
        
        if (ros::param::get("/ipe_v4_2_node/sub_UWB0", m_should_publish) && m_should_publish) {
            UwbSubscriber* o_uwb1 = new UwbSubscriber(_node, "0", &o_ipeCallback);
            o_uwb1->setupSubscriber("0");
            registerCallback(o_uwb1);
        }
        if (ros::param::get("/ipe_v4_2_node/sub_UWB1", m_should_publish) && m_should_publish) {
            UwbSubscriber* o_uwb2 = new UwbSubscriber(_node, "1", &o_ipeCallback);
            o_uwb2->setupSubscriber("1");
            registerCallback(o_uwb2);
        }
        if (ros::param::get("/ipe_v4_2_node/sub_UWB2", m_should_publish) && m_should_publish) {
            UwbSubscriber* o_uwb3 = new UwbSubscriber(_node, "2", &o_ipeCallback);
            o_uwb3->setupSubscriber("2");
            registerCallback(o_uwb3);
        }
        if (ros::param::get("/ipe_v4_2_node/sub_UWB3", m_should_publish) && m_should_publish) {
            UwbSubscriber* o_uwb4 = new UwbSubscriber(_node, "3", &o_ipeCallback);
            o_uwb4->setupSubscriber("3");
            registerCallback(o_uwb4);
        }
    }
  
    // 2024.06.04 (hy.joo)
    
    
    o_slam = new SlamSubscriber(_node, &o_ipeCallback); 
    if (ros::param::get("/ipe_v4_2_node/Slam_on", m_should_publish) && m_should_publish){
        registerCallback(o_slam);    
    }
    
}

void IPEInterface::registerCallback(PacketCallback *_cb)  // Make sure PacketCallback is defined
{
    l_callbacks.push_back(_cb);
}


void IPEInterface::spinFor()
{
	RosKapDataPacket rosPacket = o_ipeCallback.next();
    std::string frame_id1 = rosPacket.second.frame_id;
    
    uSensorData newData;
    SensorData newUWBData;

    newData.No = no;
    newUWBData.No = no;

    if (!rosPacket.second.empty(rosPacket.second.frame_id))
    {
        for (auto &cb : l_callbacks)
		{
            //std::string frame_id = rosPacket.second.frame_id;
            /*
            if (rosPacket.second.frame_id == "imu"){
                ImuSubscriber* imuSub = dynamic_cast<ImuSubscriber*>(cb);
                if (imuSub){
                    cb->operator()(rosPacket.second, rosPacket.first, &newUWBData);
                    newData.IMU = rosPacket.first;
                    no++;
                    o_ipeCallback.pop();
                    break;
                }
            }
            */

            //hy.joo (24.06.05)
            if(rosPacket.second.frame_id == "slam"){
                SlamSubscriber* slamSub = dynamic_cast<SlamSubscriber*>(cb);
                
                if(slamSub){
                    
                    cb->operator()(rosPacket.second, rosPacket.first, &newUWBData); 
                    newData.Slam = rosPacket.first; 
                    no++; 
                    o_ipeCallback.pop(); 
                    break; 
                }
            }

            else{
                UwbSubscriber* uwbSub = dynamic_cast<UwbSubscriber*>(cb);
                
                if(uwbSub){
                    
                    cb->operator()(rosPacket.second, rosPacket.first, &newUWBData); 
                    if (rosPacket.second.frame_id == "tag1") {
                        newData.Tag1 = rosPacket.first;
                        
                    } 
                    if (rosPacket.second.frame_id == "tag2") {
                        newData.Tag2 = rosPacket.first;
                        
                    }
                    if (rosPacket.second.frame_id == "tag3") {
                        newData.Tag3 = rosPacket.first;
                        
                    }
                    if (rosPacket.second.frame_id == "tag4") {
                        newData.Tag4 = rosPacket.first;
                        
                    }
                    no++;
                    o_ipeCallback.pop();
                    break;
                }
            }
		}
        
	} 
    
}

void IPEInterface::run(){
    if (m_test == true){
        testPositioning();
    }
    else{
        Positioning();
    }
}

void IPEInterface::Positioning()
{
    try {
        registerSubcribers(r_nh);
    } catch (const std::exception& e) {
        ROS_ERROR("%s", e.what());
    }

    while (ros::ok()) {
        spinFor();
        ros::spinOnce();
    }
}

// 2023.09.14
void IPEInterface::testPositioning()
{
    
    try {
        registerSubcribers(r_nh);
    } catch (const std::exception& e) {
        ROS_ERROR("%s", e.what());
    }

    rosbag::Bag bag;
    try {
        std::string bagfile_path; 
        //bag.open("/media/keti/xavier_SSD/Datasets/bagfiles/global_0614_track8_noImage.bag", rosbag::bagmode::Read);    
        ros::param::get("/ipe_v4_2_node/bagfile_path", bagfile_path); 
        bag.open(bagfile_path, rosbag::bagmode::Read);    

    } catch (rosbag::BagException& e) {
        ROS_ERROR("Error opening bag file: %s", e.what());
    }

    std::vector<std::string> topics = {
        "/dwm1001/anchor/ttyUWB0",
        "/dwm1001/anchor/ttyUWB1",
        "/dwm1001/anchor/ttyUWB2",
        "/dwm1001/anchor/ttyUWB3",
        //"/zed_f9r/imu", 
        "/orb_slam3/camera_pose",
    };

    std::map<std::string, ros::Publisher> publishers;

    for (const auto& topic : topics) {
        /*
        if (topic == "/zed_f9r/imu") {
            publishers[topic] = r_nh.advertise<sensor_msgs::Imu>(topic, 10);
        } 
        */
        //hy.joo (24/06/05)
        if(topic == "/orb_slam3/camera_pose"){
            publishers[topic] = r_nh.advertise<geometry_msgs::PoseStamped>(topic, 10);
        }

        else {
            publishers[topic] = r_nh.advertise<ipe_v4_2::Anchor>(topic, 10);
        }
    }

    rosbag::View view(bag);

    for (const rosbag::MessageInstance& message : view) {
        if (publishers.find(message.getTopic()) != publishers.end()) {
            /*
            if (message.getTopic() == "/zed_f9r/imu") {
                sensor_msgs::Imu::ConstPtr imu_data = message.instantiate<sensor_msgs::Imu>();
                if (imu_data != NULL) {
                    publishers[message.getTopic()].publish(imu_data);
                }
            } 
            */
            if (message.getTopic().find("/dwm1001/anchor/ttyUWB") == 0 && message.getTopic().back() >= '0' && message.getTopic().back() <= '3') {
                ipe_v4_2::Anchor::ConstPtr uwb_data = message.instantiate<ipe_v4_2::Anchor>();
                if (uwb_data != NULL) {
                    publishers[message.getTopic()].publish(uwb_data);
                }
            }

            if (message.getTopic() == "/orb_slam3/camera_pose") {
                geometry_msgs::PoseStamped::ConstPtr slam_pose_data = message.instantiate<geometry_msgs::PoseStamped>();
                if (slam_pose_data != NULL) {
             
                    publishers[message.getTopic()].publish(slam_pose_data);
                }
            } 

            std::this_thread::sleep_for(std::chrono::milliseconds(3));
            spinFor();  // Assuming that 'spinFor()' is some kind of sleep/delay function. If not, please clarify.
            ros::spinOnce();
        }
    }

    bag.close();
    ROS_INFO("Finish...");
}
