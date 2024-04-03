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
  
//   fusion2_terminate();
}


void IPEInterface::registerSubcribers(ros::NodeHandle &_node) {
    bool m_should_publish;
    o_imu = new ImuSubscriber(_node, &o_ipeCallback);
    std::list<double> totalTag;
    if (ros::param::get("/ipe_node/Tag_on", m_should_publish) && m_should_publish) {
        if (ros::param::get("/ipe_node/sub_UWB0", m_should_publish) && m_should_publish) {
            UwbSubscriber* o_uwb1 = new UwbSubscriber(_node, "0", &o_ipeCallback);
            o_uwb1->setupSubscriber("0");
            registerCallback(o_uwb1);
        }
        if (ros::param::get("/ipe_node/sub_UWB1", m_should_publish) && m_should_publish) {
            UwbSubscriber* o_uwb2 = new UwbSubscriber(_node, "1", &o_ipeCallback);
            o_uwb2->setupSubscriber("1");
            registerCallback(o_uwb2);
        }
        if (ros::param::get("/ipe_node/sub_UWB2", m_should_publish) && m_should_publish) {
            UwbSubscriber* o_uwb3 = new UwbSubscriber(_node, "2", &o_ipeCallback);
            o_uwb3->setupSubscriber("2");
            registerCallback(o_uwb3);
        }
        if (ros::param::get("/ipe_node/sub_UWB3", m_should_publish) && m_should_publish) {
            UwbSubscriber* o_uwb4 = new UwbSubscriber(_node, "3", &o_ipeCallback);
            o_uwb4->setupSubscriber("3");
            registerCallback(o_uwb4);
        }
    }

    if (ros::param::get("/ipe_node/IMU_on", m_should_publish) && m_should_publish) {
        registerCallback(o_imu);
    }
}

void IPEInterface::registerCallback(PacketCallback *_cb)  // Make sure PacketCallback is defined
{
    l_callbacks.push_back(_cb);
}

// 데이터를 CSV 파일로 저장하는 함수
void saveDataToFile(const std::vector<uSensorData>& data, const std::string& filename) {
    std::ofstream file(filename);

    // 파일 스트림에 소수점 출력 형식 설정
    file << std::fixed << std::setprecision(9);


    // 파일 헤더
    file << "No,Tag1,Tag2,Tag3,Tag4,IMU\n";

    // 데이터 작성
    for (const auto& d : data) {
        file << d.No << ",";
        file << d.Tag1 << ",";
        file << d.Tag2 << ",";
        file << d.Tag3 << ",";
        file << d.Tag4 << ",";
        file << d.IMU << "\n";
    }

    file.close();
}

void saveUWBDataToFile(const std::vector<SensorData>& data, const std::string& filename) {
    std::ofstream file(filename);

    // 파일 스트림에 소수점 출력 형식 설정
    file << std::fixed << std::setprecision(9);


    // 파일 헤더
    file << "No, Ln, Lp, LnC, PP, Nanchor, RxIDUWB1, RxIDUWB2, RxIDUWB3, RxIDUWB4, RxIDUWB5, RxIDUWB6, RxDistUWB1, RxDistUWB2, RxDistUWB3, RxDistUWB4, RxDistUWB5, RxDistUWB6, s_time, tag_pos_b1, tag_pos_b2, tag_pos_b3, tag_pos_b4, tag_pos_b5, tag_pos_b6, tag_pos_b7, tag_pos_b8, xain1, xain2, xain3, xain4, xain5, xain6, yain1, yain2, yain3, yain4, yain5, yain6, TagPos1, TagPos2, TagPos3, TagPos4, TagPos5, TagPos6, TagPos7, TagPos8, -kf_psi\n";

    // 데이터 작성
    for (const auto& d : data) {
        file << d.No << ",";
        file << d.Ln << ",";
        file << d.Lp << ",";
        file << d.LnC << ",";
        file << d.PP << ",";
        file << d.Nanchor << ",";
        file << d.RxIDUWB1 << ",";
        file << d.RxIDUWB2 << ",";
        file << d.RxIDUWB3 << ",";
        file << d.RxIDUWB4 << ",";
        file << d.RxIDUWB5 << ",";
        file << d.RxIDUWB6 << ",";
        file << d.RxDistUWB1 << ",";
        file << d.RxDistUWB2 << ",";
        file << d.RxDistUWB3 << ",";
        file << d.RxDistUWB4 << ",";
        file << d.RxDistUWB5 << ",";
        file << d.RxDistUWB6 << ",";
        file << d.s_time << ",";
        file << d.real_tag_pos_b1 << ",";
        file << d.imag_tag_pos_b1 << ",";
        file << d.real_tag_pos_b2 << ",";
        file << d.imag_tag_pos_b2 << ",";
        file << d.real_tag_pos_b3 << ",";
        file << d.imag_tag_pos_b3 << ",";
        file << d.real_tag_pos_b4 << ",";
        file << d.imag_tag_pos_b4 << ",";
        file << d.xain1 << ",";
        file << d.xain2 << ",";
        file << d.xain3 << ",";
        file << d.xain4 << ",";
        file << d.xain5 << ",";
        file << d.xain6 << ",";
        file << d.yain1 << ",";
        file << d.yain2 << ",";
        file << d.yain3 << ",";
        file << d.yain4 << ",";
        file << d.yain5 << ",";
        file << d.yain6 << ",";
        file << d.real_TagPos1 << ",";
        file << d.imag_TagPos1 << ",";
        file << d.real_TagPos2 << ",";
        file << d.imag_TagPos2 << ",";
        file << d.real_TagPos3 << ",";
        file << d.imag_TagPos3 << ",";
        file << d.real_TagPos4 << ",";
        file << d.imag_TagPos4 << ",";
        file << d.kf_psi << "\n";
    }

    file.close();
}

void saveIMUDataToFile(const std::vector<SensorData>& data, const std::string& filename) {
    std::ofstream file(filename);

    // 파일 스트림에 소수점 출력 형식 설정
    file << std::fixed << std::setprecision(9);


    // 파일 헤더
    file << "No, IMUacc_c1, IMUacc_c2, IMUacc_c3, IMUgyro_c1, IMUgyro_c2, IMUgyro_c3, s_time, b_acc_o1, b_acc_o2, b_acc_o3, b_gyro1, b_gyro2, b_gyro3, IMUSel, kf_psi, gyro_psi, cent_pos_est1, cent_pos_est2, cent_pos_est3, cent_vel_est1, cent_vel_est2, cent_vel_est3\n";

    // 데이터 작성
    for (const auto& d : data) {
        file << d.No << ",";
        file << d.IMUacc_c1 << ",";
        file << d.IMUacc_c2 << ",";
        file << d.IMUacc_c3 << ",";
        file << d.IMUgyro_c1 << ",";
        file << d.IMUgyro_c2 << ",";
        file << d.IMUgyro_c3 << ",";
        file << d.s_time << ",";
        file << d.b_acc_o1 << ",";
        file << d.b_acc_o2 << ",";
        file << d.b_acc_o3 << ",";
        file << d.b_gyro1 << ",";
        file << d.b_gyro2 << ",";
        file << d.b_gyro3 << ",";
        file << d.IMUSel << ",";
        file << d.kf_psi << ",";
        file << d.gyro_psi << ",";
        file << d.cent_pos_est1 << ",";
        file << d.cent_pos_est2 << ",";
        file << d.cent_pos_est3 << ",";
        file << d.cent_vel_est1 << ",";
        file << d.cent_vel_est2 << ",";
        file << d.cent_vel_est3 << "\n";
    }

    file.close();
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
            ImuSubscriber* imuSub = dynamic_cast<ImuSubscriber*>(cb);
            std::string frame_id = rosPacket.second.frame_id;
//            std::cout<<rosPacket.second.frame_id<<std::endl;

            
            if (rosPacket.second.frame_id == "imu"){
                if (imuSub){
                    cb->operator()(rosPacket.second, rosPacket.first, &newUWBData);
                    newData.IMU = rosPacket.first;
                    no++;
                    o_ipeCallback.pop();
                    break;
                }
            }
            else{
                if (!imuSub){
                    cb->operator()(rosPacket.second, rosPacket.first, &newUWBData);
                    if (rosPacket.second.frame_id == "tag1") {
                        newData.Tag1 = rosPacket.first;
                        //std::cout<<"Tag1---"<<std::endl;
                    } 
                    if (rosPacket.second.frame_id == "tag2") {
                        newData.Tag2 = rosPacket.first;
                        //std::cout<<"Tag2---"<<std::endl;
                    }
                    if (rosPacket.second.frame_id == "tag3") {
                        newData.Tag3 = rosPacket.first;
                        //std::cout<<"Tag3---"<<std::endl;
                    }
                    if (rosPacket.second.frame_id == "tag4") {
                        newData.Tag4 = rosPacket.first;
                        //std::cout<<"Tag4---"<<std::endl;
                    }
                    no++;
                    o_ipeCallback.pop();
                    break;
                }
            }

            
		}
        //data.push_back(newData);
        //uwbdata.push_back(newUWBData);
        // saveDataToFile(data, "/home/ubuntu/ros_delivery_workspace/-K-AutoDrivingLocalization/src/positioning/sensor_data.csv");
        //saveUWBDataToFile(uwbdata, "/home/keti/Document/JW/-K-AutoDrivingLocalization/src/positioning/uwbsensor_data.csv");
        //saveIMUDataToFile(uwbdata, "/home/keti/Document/JW/-K-AutoDrivingLocalization/src/positioning/imusensor_data.csv");

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
        // bag.open("/home/umaps/rosbag/[zed_f9r]2023-08-31-17-56-41_slow.bag", rosbag::bagmode::Read);
//        bag.open("/home/keti/Documents/JW/-K-AutoDrivingLocalization-main/src/positioning/keti_ws/2024-02-16-16-18-43.bag", rosbag::bagmode::Read);
//        bag.open("/home/keti/Documents/JW/-K-AutoDrivingLocalization-main/rosbag/2024-02-20-17-03-24.bag", rosbag::bagmode::Read);
        // bag.open("/home/keti/Documents/JW/-K-AutoDrivingLocalization-main/rosbag/2024-03-05-17-19-02.bag", rosbag::bagmode::Read);
        bag.open("/home/keti/Documents/JW/-K-AutoDrivingLocalization-main/rosbag/2024-04-01-16-48-31.bag", rosbag::bagmode::Read);
        // bag.open("/home/keti/Documents/JW/-K-AutoDrivingLocalization-main/rosbag/2024-03-21-16-55-51.bag", rosbag::bagmode::Read);
//        bag.open("/home/keti/Documents/JW/-K-AutoDrivingLocalization-main/src/positioning/matlab/Positioning_Alg_20231004/2023-12-05-18-16-11.bag", rosbag::bagmode::Read);
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

//    std::vector<std::string> topics = {
  //      "/dwm1001/anchor/tag0",
  //      "/dwm1001/anchor/tag1",
  //      "/dwm1001/anchor/tag2",
    //    "/dwm1001/anchor/tag3",
    //    "/zed_f9r/imu"
    //};

    std::map<std::string, ros::Publisher> publishers;

    for (const auto& topic : topics) {
        if (topic == "/zed_f9r/imu") {
            publishers[topic] = r_nh.advertise<sensor_msgs::Imu>(topic, 10);
        } else {
            publishers[topic] = r_nh.advertise<ipe::Anchor>(topic, 10);
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
            } 
            if (message.getTopic().find("/dwm1001/anchor/ttyUWB") == 0 && message.getTopic().back() >= '0' && message.getTopic().back() <= '3') {
                ipe::Anchor::ConstPtr uwb_data = message.instantiate<ipe::Anchor>();
                if (uwb_data != NULL) {
                    publishers[message.getTopic()].publish(uwb_data);
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
