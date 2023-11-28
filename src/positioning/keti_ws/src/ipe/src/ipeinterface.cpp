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
  
  fusion2_terminate();
}

// void IPEInterface::registerSubcribers(ros::NodeHandle &_node) {
//     bool m_should_publish;
//     o_fusion = new FusionSubscriber(_node);
//     std::list<double> totalTag;
//     if (ros::param::get("/ipe_node/Tag_on", m_should_publish) && m_should_publish) {
//         if (ros::param::get("/ipe_node/sub_UWB0", m_should_publish) && m_should_publish) {
//             UwbSubscriber* o_uwb1 = new UwbSubscriber(_node, "0", &o_ipeCallback, o_fusion);
//             // o_uwb1->registerCallback([this](double value, std::string& value2) { o_fusion->onUWBDataReceived(value, value2); });
//             registerCallback(o_uwb1);
//         }
//         if (ros::param::get("/ipe_node/sub_UWB1", m_should_publish) && m_should_publish) {
//             UwbSubscriber* o_uwb2 = new UwbSubscriber(_node, "1", &o_ipeCallback, o_fusion);
//             // o_uwb2->registerCallback([this](double value, std::string& value2) { o_fusion->onUWBDataReceived(value, value2); });
//             registerCallback(o_uwb2);
//         }
//         if (ros::param::get("/ipe_node/sub_UWB2", m_should_publish) && m_should_publish) {
//             UwbSubscriber* o_uwb3 = new UwbSubscriber(_node, "2", &o_ipeCallback, o_fusion);
//             // o_uwb3->registerCallback([this](double value, std::string& value2) { o_fusion->onUWBDataReceived(value, value2); });
//             registerCallback(o_uwb3);
//         }
//         if (ros::param::get("/ipe_node/sub_UWB3", m_should_publish) && m_should_publish) {
//             UwbSubscriber* o_uwb4 = new UwbSubscriber(_node, "3", &o_ipeCallback, o_fusion);
//             // o_uwb4->registerCallback([this](double value, std::string& value2) { o_fusion->onUWBDataReceived(value, value2); });
//             registerCallback(o_uwb4);
//         }
//     }

//     if (ros::param::get("/ipe_node/IMU_on", m_should_publish) && m_should_publish) {
//         ImuSubscriber* o_imu = new ImuSubscriber(_node, &o_ipeCallback, o_fusion);
//         // o_imu->registerCallback([this](double value, std::string& frameID) { o_fusion->onIMUDataReceived(value); });

//         registerCallback(o_imu);
//     }
// }


void IPEInterface::registerSubcribers(ros::NodeHandle &_node) {
    bool m_should_publish;
    o_fusion = new FusionSubscriber(_node);
<<<<<<< HEAD
    o_uwb = new UwbSubscriber(_node, "0", &o_ipeCallback, o_fusion);
=======
>>>>>>> main
    o_imu = new ImuSubscriber(_node, &o_ipeCallback, o_fusion);
    std::list<double> totalTag;
    if (ros::param::get("/ipe_node/Tag_on", m_should_publish) && m_should_publish) {
        if (ros::param::get("/ipe_node/sub_UWB0", m_should_publish) && m_should_publish) {
<<<<<<< HEAD
            o_uwb->setupSubscriber("0");
            registerCallback(o_uwb);
        }
        if (ros::param::get("/ipe_node/sub_UWB1", m_should_publish) && m_should_publish) {
            o_uwb->setupSubscriber("1");
            registerCallback(o_uwb);
        }
        if (ros::param::get("/ipe_node/sub_UWB2", m_should_publish) && m_should_publish) {
            o_uwb->setupSubscriber("2");
            registerCallback(o_uwb);
        }
        if (ros::param::get("/ipe_node/sub_UWB3", m_should_publish) && m_should_publish) {
            o_uwb->setupSubscriber("3");
            registerCallback(o_uwb);
=======
            UwbSubscriber* o_uwb1 = new UwbSubscriber(_node, "0", &o_ipeCallback, o_fusion);
            o_uwb1->setupSubscriber("0");
            registerCallback(o_uwb1);
        }
        if (ros::param::get("/ipe_node/sub_UWB1", m_should_publish) && m_should_publish) {
            UwbSubscriber* o_uwb1 = new UwbSubscriber(_node, "1", &o_ipeCallback, o_fusion);
            o_uwb1->setupSubscriber("1");
            registerCallback(o_uwb1);
        }
        if (ros::param::get("/ipe_node/sub_UWB2", m_should_publish) && m_should_publish) {
            UwbSubscriber* o_uwb1 = new UwbSubscriber(_node, "2", &o_ipeCallback, o_fusion);
            o_uwb1->setupSubscriber("2");
            registerCallback(o_uwb1);
        }
        if (ros::param::get("/ipe_node/sub_UWB3", m_should_publish) && m_should_publish) {
            UwbSubscriber* o_uwb1 = new UwbSubscriber(_node, "3", &o_ipeCallback, o_fusion);
            o_uwb1->setupSubscriber("3");
            registerCallback(o_uwb1);
>>>>>>> main
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


void IPEInterface::spinFor()
{
    ImuSubscriber* imuSub;
    UwbSubscriber* uwbSub;

	RosKapDataPacket rosPacket = o_ipeCallback.next();
    std::string frame_id1 = rosPacket.second.frame_id;
    bool flag = false;
	if (!rosPacket.second.empty(rosPacket.second.frame_id))
	{
        for (auto &cb : l_callbacks)
		{
            imuSub = dynamic_cast<ImuSubscriber*>(cb);
            uwbSub = dynamic_cast<UwbSubscriber*>(cb);
            std::string frame_id = rosPacket.second.frame_id;
            if (rosPacket.second.frame_id == "imu"){
                if (imuSub){
                    cb->operator()(rosPacket.second, rosPacket.first);
<<<<<<< HEAD
=======
                    o_imu = imuSub;
>>>>>>> main
                    o_ipeCallback.pop();
                    break;
                }
            }
            else{
                if (!imuSub){
                    if (flag2 != 1){
                        uwbSub->xain_list = o_uwb->xain_list;
                        uwbSub->yain_list = o_uwb->yain_list;
                        uwbSub->RxID_data_list = o_uwb->RxID_data_list;
                        uwbSub->RxID_list = o_uwb->RxID_list;
                    }
                    flag2 = 2;
                    cb->operator()(rosPacket.second, rosPacket.first);
<<<<<<< HEAD
=======
                    o_uwb = uwbSub;
                    flag = true;
>>>>>>> main
                    o_ipeCallback.pop();
                    break;
                }
            }
		}
        
        if (o_fusion->init_flag == 1) {
<<<<<<< HEAD
            o_imu->gyro_psi = -o_fusion->heading_est;
=======
            o_imu->gyro_psi = -o_fusion->heading_est; 
>>>>>>> main
            o_imu->kf_psi = o_fusion->gyro_psi;
            o_imu->cent_pos_est[0] = o_fusion->IMUposU.re;
            o_imu->cent_pos_est[1] = o_fusion->IMUposU.im;
            o_imu->cent_pos_est[2] = 0; 
            o_imu->cent_vel_est[0] = o_fusion->tag_center_vel_est.re;
            o_imu->cent_vel_est[1] = o_fusion->tag_center_vel_est.im;
            o_imu->cent_vel_est[2] = 0;
        }

<<<<<<< HEAD
        if (o_fusion->num_ == 1 && o_fusion->init_flag == 1){
            o_uwb->init_flag = 2;
        }
        std::complex<real_T> j(0, 1); // 복소수 단위

        for (int i = 0; i < 4; ++i) {
            std::complex<real_T> cent_pos_est_(o_fusion->IMUposU.re, o_fusion->IMUposU.im);
            std::complex<real_T> current_tag_pos_b(o_fusion->tag_pos_b[i].re, o_fusion->tag_pos_b[i].im);
                    
            std::complex<real_T> TagPos = cent_pos_est_ + std::exp(j * (-o_fusion->kf_psi)) * (current_tag_pos_b + 0.4 * j);
            o_uwb->prevTagPos[i].re = 0;
            o_uwb->prevTagPos[i].im = 0;


        }
        o_uwb->prevTagHeading = -o_fusion->kf_psi;
=======
        std::copy(std::begin(o_fusion->b_acc_o), std::end(o_fusion->b_acc_o), std::begin(o_imu->b_acc_o));
        std::copy(std::begin(o_fusion->cent_pos_est), std::end(o_fusion->cent_pos_est), std::begin(o_imu->cent_pos_est));
        std::copy(std::begin(o_fusion->cent_vel_est), std::end(o_fusion->cent_vel_est), std::begin(o_imu->cent_vel_est));
        o_imu->kf_psi = o_fusion->kf_psi;
        o_imu->gyro_psi = o_fusion->gyro_psi;
        // o_imu->acc_b_phi = o_fusion->acc_b_phi;
        // o_imu->acc_b_theta = o_fusion->acc_b_theta;

        if (flag){
            if (o_fusion->num_ == 1 && o_fusion->init_flag == 1){
                o_uwb->init_flag = 2;
            }
            std::complex<real_T> j(0, 1); // 복소수 단위

            // creal_T cent_pos_est_;
            creal_T current_tag_pos_b[4];
            for (int i = 0; i < 4; ++i) {
                o_imu->cent_pos_est[0] = o_fusion->IMUposU.re;
                o_imu->cent_pos_est[1] = o_fusion->IMUposU.im;
                current_tag_pos_b[i].re = o_fusion->tag_pos_b[i].re;
                current_tag_pos_b[i].im = o_fusion->tag_pos_b[i].im;
                        
                std::complex<double> cent_pos_est_c(o_fusion->IMUposU.re, o_fusion->IMUposU.im);
                std::complex<double> current_tag_pos_b_c(current_tag_pos_b[i].re, current_tag_pos_b[i].im);
                std::complex<double> TagPos = cent_pos_est_c + std::exp(j * (-o_fusion->kf_psi)) * (current_tag_pos_b_c + 0.4 * j);
                creal_T TagPos_;
                TagPos_.re = std::real(TagPos);
                TagPos_.im = std::imag(TagPos);
                o_uwb->prevTagPos[i].re = TagPos_.re;
                o_uwb->prevTagPos[i].im = TagPos_.im;
            }
            
            o_uwb->prevTagHeading = -o_fusion->kf_psi;
            o_imu->kf_psi = o_fusion->kf_psi;
        }
>>>>>>> main

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
<<<<<<< HEAD
        bag.open("/home/umaps/rosbag/[zed_f9r]2023-08-31-17-56-41_slow.bag", rosbag::bagmode::Read);
=======
        // bag.open("/home/umaps/rosbag/[zed_f9r]2023-08-31-17-56-41_slow.bag", rosbag::bagmode::Read);
        bag.open("/home/umaps/rosbag/test/20231116_fast_inline_2023-11-16-10-34-58.bag", rosbag::bagmode::Read);
>>>>>>> main
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
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            spinFor();  // Assuming that 'spinFor()' is some kind of sleep/delay function. If not, please clarify.
            ros::spinOnce();
        }
    }

    bag.close();
    ROS_INFO("Finish...");
}