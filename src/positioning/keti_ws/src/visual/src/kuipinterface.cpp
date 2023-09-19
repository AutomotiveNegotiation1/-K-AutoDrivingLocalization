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

#include "kuipinterface.h"

kuipInterface::~kuipInterface()
{
  ROS_INFO("Cleaning up ...");
  
  // m_callbacks의 각 요소에 대한 메모리 할당 반환
  for(auto &cb : m_callbacks) {
      delete cb;
  }
  m_callbacks.clear();  // 옵션: 리스트를 비운다
  
  UWBpos6_terminate();
}

void kuipInterface::registerSubcribers(ros::NodeHandle &node) {
    bool should_publish;
    std::list<double> totalTag;
    if (ros::param::get("/visual_node/Tag_on", should_publish) && should_publish) {
        if (ros::param::get("/visual_node/sub_UWB0", should_publish) && should_publish) {
            registerCallback(new UwbSubscriber(node, "0", &m_kuipCallback));
        }
        if (ros::param::get("/visual_node/sub_UWB1", should_publish) && should_publish) {
            registerCallback(new UwbSubscriber(node, "1", &m_kuipCallback));
        }
        if (ros::param::get("/visual_node/sub_UWB2", should_publish) && should_publish) {
            registerCallback(new UwbSubscriber(node, "2", &m_kuipCallback));
        }
        if (ros::param::get("/visual_node/sub_UWB3", should_publish) && should_publish) {
            registerCallback(new UwbSubscriber(node, "3", &m_kuipCallback));
        }
    }

    if (ros::param::get("/visual_node/IMU_on", should_publish) && should_publish) {
        registerCallback(new ImuSubscriber(node, &m_kuipCallback));
    }
}

void kuipInterface::registerCallback(PacketCallback *cb)  // Make sure PacketCallback is defined
{
    m_callbacks.push_back(cb);
}

void kuipInterface::spinFor()
{
	RosKapDataPacket rosPacket = m_kuipCallback.next();

	if (!rosPacket.second.empty())
	{
        for (auto &cb : m_callbacks)
		{
			cb->operator()(rosPacket.second, rosPacket.first);
            // ImuSubscriber* imuSub = dynamic_cast<ImuSubscriber*>(cb);
            // if (imuSub && !init) {
            //     // cb는 ImuSubscriber입니다. 이 안에서 원하는 작업을 수행하십시오.
            //     bool connected = QObject::connect(imuSub, &ImuSubscriber::newPositionData, &mw_, &MainWindow::onNewPositionData);
            //     init = true;
            // }
		}

	}
}

// void KpInterface::spinFor()
// {
//     RosKapDataPacket earliestPacket;
//     UwbSubscriber* correspondingSubscriber = nullptr;  // <-- Add this line to remember the subscriber
//     ROS_INFO("Start spinFor...");
    
//     for (auto &cb : m_callbacks)
//     {
//         if (!cb->getDataEmpty())
//         {
//             RosKapDataPacket rosPacket = cb->m_kapCallback->next();
            
//             // 첫 번째 패킷 또는 이전에 발견된 패킷보다 더 이른 패킷을 찾는 경우
//             double rostimeValue = convertToDouble(rosPacket.first)/1e9;
//             double earliesttimeValue = convertToDouble(earliestPacket.first)/1e9;
//             if (earliestPacket.second.empty() || rostimeValue < earliesttimeValue)
//             {
//                 earliestPacket = rosPacket;
//                 correspondingSubscriber = cb;  // <-- Update the subscriber
//             }
//         }
//     }

//     // 최소 타임스탬프 패킷을 찾은 경우 Operator 함수에 전달하고 pop
//     if (!earliestPacket.second.empty())
//     {
//         // Operator op(mainWindow_, earliestPacket.second, earliestPacket.first, pos); // MainWindow 참조 추가
//         Operator op(
//             [this](const PosDataPacket& pos) {
//                 ROS_INFO("Emitting newPositionData.");

//                 // Use QMetaObject::invokeMethod to safely emit the signal from the main thread.
//                 QMetaObject::invokeMethod(this, [this, pos](){
//                     emit newPositionData(pos);
//                 }, Qt::QueuedConnection);

//             }, 
//             earliestPacket.second, 
//             convertToDouble(earliestPacket.first)/1e9, 
//             pos
//         );

        
//         // TODO: pop 함수를 호출하여 earliestPacket 삭제
//         if(correspondingSubscriber) {
//             correspondingSubscriber->m_kapCallback->pop(earliestPacket);
//         }
//     }
// }

// 2023.09.14
void kuipInterface::run()
{
    try {
        registerSubcribers(nh_);
    } catch (const std::exception& e) {
        ROS_ERROR("%s", e.what());
    }

    rosbag::Bag bag;
    try {
        bag.open("/home/umaps/rosbag/[zed_f9r]2023-08-31-18-00-04_long_stop.bag", rosbag::bagmode::Read);
    } catch (rosbag::BagException& e) {
        ROS_ERROR("Error opening bag file: %s", e.what());
    }

    std::vector<std::string> topics = {
        "/dwm1001/anchor/ttyUWB0",
        "/dwm1001/anchor/ttyUWB1",
        "/dwm1001/anchor/ttyUWB2",
        "/dwm1001/anchor/ttyUWB3"
    };

    std::map<std::string, ros::Publisher> publishers;

    for (const auto& topic : topics) {
        publishers[topic] = nh_.advertise<visual::Anchor>(topic, 10);
    }

    rosbag::View view(bag);

    for (const rosbag::MessageInstance& message : view) {
        if (publishers.find(message.getTopic()) != publishers.end()) {
            visual::Anchor::ConstPtr uwb_data = message.instantiate<visual::Anchor>();
            if (uwb_data != NULL) {
                publishers[message.getTopic()].publish(uwb_data);
                // std::this_thread::sleep_for(std::chrono::milliseconds(10));
                spinFor();  // I assume you meant to use std::chrono::milliseconds
                ros::spinOnce();
            }
        }
        
    }
 
    bag.close();
    ROS_INFO("Finish...");
}


//2023.09.15
// void kuipInterface::run()
// {

//     std::ifstream file("/home/umaps/-K-AutoDrivingLocalization/src/positioning/keti_ws/src/visual/src/processed_data.csv");
//     std::string line;

//     std::list<double> Ln,Lp,LnC,TagNum,Nanchor,s_time,prevTagHeading,RxIDin,RxDistin,xain,yain,fprevTagPos,ftag_pos_b;
//     int Lnsize, Lpsize, Lncsize, TagNumsize, Nanchorsize, headsize, Rxidsize, Rxdistsize, xainsize, yainsize, tagpossize, tag_b_size, s_timesize;

//     int k = 0;
//     bool flag = false;
//     creal_T tag_pos_b[4];
//     creal_T prevTagPos[4];

//     while (std::getline(file, line)) {
//         std::istringstream iss(line);
//         std::string value;

//         if (k != 0 && k % 8 == 0) {
//             // Introduce a 1-second delay
//             std::this_thread::sleep_for(std::chrono::milliseconds(10));
//             Lnsize = Ln.size();
//             Lpsize = Lp.size();
//             Lncsize = LnC.size();
//             TagNumsize = TagNum.size();
//             Nanchorsize = Nanchor.size();
//             headsize = prevTagHeading.size();
//             Rxidsize = RxIDin.size();
//             Rxdistsize = RxDistin.size();
//             xainsize = xain.size();
//             yainsize = yain.size();
//             tagpossize = fprevTagPos.size();
//             tag_b_size = ftag_pos_b.size();
//             s_timesize = s_time.size();

//             double aLn;
//             double aLp;
//             double aLnC;
//             double aTagNum;
//             double aNanchor;
//             double aprevTagHeading;
//             double aRxIDin[6];
//             double aRxDistin[6];
//             double axain[6];
//             double ayain[6];
//             // double aprevTagPos = new double[tagpossize];
//             // double atag_pos_b = new double[tag_b_size];
//             double as_time;

//             int index = 0;
//             for(double val : Ln) {
//                 aLn = val;
//                 ++index;
//             }

//             index = 0;
//             for(double val : s_time) {
//                 as_time = val;
//                 ++index;
//             }

//             index = 0;
//             for(double val : Lp) {
//                 aLp = val;
//                 ++index;
//             }

//             index = 0;
//             for(double val : LnC) {
//                 aLnC = val;
//                 ++index;
//             }
            
//             index = 0;
//             for(double val : TagNum) {
//                 aTagNum = val;
//                 ++index;
//             }

//             index = 0;
//             for(double val : Nanchor) {
//                 aNanchor = val;
//                 ++index;
//             }

//             index = 0;
//             for(double val : prevTagHeading) {
//                 aprevTagHeading = val;
//                 ++index;
//             }

//             index = 0;
//             for(double val : RxIDin) {
//                 aRxIDin[index] = val;
//                 ++index;
//             }

//             index = 0;
//             for(double val : RxDistin) {
//                 aRxDistin[index] = val;
//                 ++index;
//             }

//             index = 0;
//             for(double val : xain) {
//                 axain[index] = val;
//                 pos.x.push_back(axain[index]);
//                 // pos.x.push_back(axain[index]);
//                 ++index;
//             }

//             index = 0;
//             for(double val : yain) {
//                 ayain[index] = val;
//                 pos.y.push_back(ayain[index]);
//                 // pos.x.push_back(ayain[index]);
//                 ++index;
//             }

//             if (flag == false){
//                 argInit_1x4_creal_T(tag_pos_b);
//                 argInit_1x4_creal_preT(prevTagPos);
//                 flag = true;
//             }
//             double UWBout[21];

//             UWBpos6(aLn, aLp, aLnC, aTagNum, aNanchor, aRxIDin, aRxDistin, as_time, tag_pos_b, axain, ayain, prevTagPos, 0, UWBout);

//             pos.tag_pos_est.resize(4);
//             pos.tag_pos_est[0].re = UWBout[0];
//             pos.tag_pos_est[1].re = UWBout[2];
//             pos.tag_pos_est[2].re = UWBout[1];
//             pos.tag_pos_est[3].re = UWBout[3];

//             pos.tag_pos_est[0].im = UWBout[4];
//             pos.tag_pos_est[1].im = UWBout[6];
//             pos.tag_pos_est[2].im = UWBout[5];
//             pos.tag_pos_est[3].im = UWBout[7];

//             pos.tag_pos_est_aver.resize(4);
//             pos.tag_pos_est_aver[0].re = UWBout[9];
//             pos.tag_pos_est_aver[1].re = UWBout[10];
//             pos.tag_pos_est_aver[2].re = UWBout[11];
//             pos.tag_pos_est_aver[3].re = UWBout[12];
            
//             pos.tag_pos_est_aver[0].im = UWBout[13];
//             pos.tag_pos_est_aver[1].im = UWBout[14];
//             pos.tag_pos_est_aver[2].im = UWBout[15];
//             pos.tag_pos_est_aver[3].im = UWBout[16];
//             pos.heading_est = UWBout[8];
//             pos.headingest_a_aver_v = UWBout[17];

//             QMetaObject::invokeMethod(this, [this]() {
//                 emit newPositionData(this->pos);
//             }, Qt::QueuedConnection);

//             std::vector<creal_T> tag_pos_est(4);   // Each element should be an array or another container. This depends on the exact type of data.
//             std::vector<creal_T> tag_pos_est_aver(4);

//             tag_pos_est[0].re = UWBout[0];
//             tag_pos_est[1].re = UWBout[1];
//             tag_pos_est[2].re = UWBout[2];
//             tag_pos_est[3].re = UWBout[3];

//             tag_pos_est[0].im = UWBout[4];
//             tag_pos_est[1].im = UWBout[5];
//             tag_pos_est[2].im = UWBout[6];
//             tag_pos_est[3].im = UWBout[7];

//             ROS_INFO("tag_pos_est--> (%f,%f), (%f,%f)", tag_pos_est[0].re, tag_pos_est[0].im, tag_pos_est[1].re, tag_pos_est[1].im);
//             ROS_INFO("tag_pos_est--> (%f,%f), (%f,%f)", tag_pos_est[2].re, tag_pos_est[2].im, tag_pos_est[3].re, tag_pos_est[3].im);


//             double heading_est = UWBout[8];
//             ROS_INFO("heading_est--> %f", heading_est);

//             tag_pos_est_aver[0].re = UWBout[9];
//             tag_pos_est_aver[1].re = UWBout[10];
//             tag_pos_est_aver[2].re = UWBout[11];
//             tag_pos_est_aver[3].re = UWBout[12];

//             tag_pos_est_aver[0].im = UWBout[13];
//             tag_pos_est_aver[1].im = UWBout[14];
//             tag_pos_est_aver[2].im = UWBout[15];
//             tag_pos_est_aver[3].im = UWBout[16];

//             ROS_INFO("tag_pos_est_aver--> (%f,%f), (%f,%f)", tag_pos_est_aver[0].re, tag_pos_est_aver[0].im, tag_pos_est_aver[1].re, tag_pos_est_aver[1].im);
//             ROS_INFO("tag_pos_est_aver--> (%f,%f), (%f,%f)", tag_pos_est_aver[2].re, tag_pos_est_aver[2].im, tag_pos_est_aver[3].re, tag_pos_est_aver[3].im);

//             double headingest_a_aver_v = UWBout[17];
//             ROS_INFO("headingest_a_aver_v--> %f", headingest_a_aver_v);

//             Ln.clear();
//             Lp.clear();
//             LnC.clear();
//             TagNum.clear();
//             Nanchor.clear();
//             s_time.clear();
//             prevTagHeading.clear();
            
//             RxIDin.clear();
//             RxDistin.clear();
//             xain.clear();
//             yain.clear();
//             fprevTagPos.clear();
//             ftag_pos_b.clear();
//             pos.x.clear();
//             pos.y.clear();
//         }

        
//         std::getline(iss, value, ',');
//         if (value != "NaN"){
//             Ln.push_back(std::stod(value));
//         }

//         std::getline(iss, value, ',');
//         if (value != "NaN"){
//             Lp.push_back(std::stod(value));
//         }

//         std::getline(iss, value, ',');
//         if (value != "NaN"){
//             LnC.push_back(std::stod(value));
//         }

//         std::getline(iss, value, ',');
//         if (value != "NaN"){
//             TagNum.push_back(std::stod(value));
//         }

//         std::getline(iss, value, ',');
//         if (value != "NaN"){
//             Nanchor.push_back(std::stod(value));
//         }

//         std::getline(iss, value, ',');
//         if (value != "NaN"){
//             RxIDin.push_back(std::stod(value));
//         }

//         std::getline(iss, value, ',');
//         if (value != "NaN"){
//             RxDistin.push_back(std::stod(value));
//         }

//         std::getline(iss, value, ',');
//         if (value != "NaN"){
//             s_time.push_back(std::stod(value));
//         }

//         std::getline(iss, value, ',');
//         if (value != "NaN"){
//             ftag_pos_b.push_back(std::stod(value));
//         }


//         std::getline(iss, value, ',');
//         if (value != "NaN"){
//             xain.push_back(std::stod(value));
//         }

//         std::getline(iss, value, ',');
//         if (value != "NaN"){
//             yain.push_back(std::stod(value));
//         }

//         std::getline(iss, value, ',');
//         if (value != "NaN"){
//             fprevTagPos.push_back(std::stod(value));
//         }

//         std::getline(iss, value, ',');
//         if (value != "NaN"){
//             prevTagHeading.push_back(std::stod(value));
//         }
        

//         k++;
//     }

//     ROS_INFO("Finish...");
// }

