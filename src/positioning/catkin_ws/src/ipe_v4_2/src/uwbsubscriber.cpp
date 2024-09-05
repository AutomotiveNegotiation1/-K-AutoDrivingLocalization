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
#include "uwbsubscriber.h"
//#include "PositioningSystem_V4_2.h" 
#include "PositioningSystem_V5_1.h" 


std::vector<double> xain_list;
std::vector<double> yain_list;
std::vector<double> zain_list;
std::vector<double> xain_difference;
std::vector<double> yain_difference;
double UWBErrSum;
std::vector<std::string> RxID_data_list;
std::vector<int> RxID_list;
bool statusUWB;
std::vector<std::string> difference;
creal_T tag_pos_b[4];
creal_T prevTagPos[4];
std::string tagNum;
creal_T tag_pos_est[4];
creal_T tag_pos_est_aver[4];
creal_T tag_center_vel_est;
double heading_est;
double headingest_a_aver_v;
double init_flag;
double Nanchor;
double zt_b;
double prevTagHeading;


inline creal_T argInit_creal_T(int idx)
{
    creal_T result;
    // std::vector<double> xt_b = {-0.525, 0.525, -0.525, 0.525};
    //std::vector<double> xt_b = {-0.525, 0.525, 0.525, -0.525};
    //std::vector<double> yt_b = {0.505, 0.505, -0.505, -0.505};
	// carnival
    std::vector<double> xt_b = {-0.595, 0.595, -0.595, 0.595};
    std::vector<double> yt_b = {0.74, 0.74, -0.74, -0.74};
	

    if (idx < xt_b.size()) {
        result.re = xt_b[idx];
        result.im = yt_b[idx];
    }
    return result;
};

inline void argInit_1x4_creal_T(creal_T result[4])
{
    /* Loop over the array to initialize each element. */
    for (int idx1 = 0; idx1 < 4; idx1++) {
        result[idx1] = argInit_creal_T(idx1);
    }
};

inline creal_T argInit_creal_preT(int idx)
{
    creal_T result;
    std::vector<double> xt_b = {0, 0, 0, 0};
    std::vector<double> yt_b = {0, 0, 0, 0};

    for (size_t i = 0; i < xt_b.size(); ++i) {
        result.re = xt_b[i];
        result.im = yt_b[i];
    }
    return result;
};

inline void argInit_1x4_creal_preT(creal_T result[4])
{
    int idx1;
    /* Loop over the array to initialize each element. */
    for (idx1 = 0; idx1 < 4; idx1++) {
        result[idx1] = argInit_creal_preT(idx1);
    }
};

UwbSubscriber::UwbSubscriber(ros::NodeHandle& _node, const std::string& _uwbNum, IPECallback* _ipeCallback)
: node(_node), o_ipeCallback(_ipeCallback), m_uwbNum(_uwbNum){
    // socketManager = SocketManager::getInstance(); // <-- Add this line to initialize the socketManager
    prevTagHeading = 0;
    argInit_1x4_creal_T(tag_pos_b);
    argInit_1x4_creal_preT(prevTagPos);

    pub_uwb_pose = _node.advertise<geometry_msgs::PoseStamped>("/ipe/uwb_only_pose", 10);
    pub_uwb_path = _node.advertise<nav_msgs::Path>("/ipe/uwb_path", 10);

}

UwbSubscriber::~UwbSubscriber() {
}

void UwbSubscriber::operator()(IPEDataPacket &packet, double timestamp, SensorData* data) {
    //std::cout << "Uwb operator" <<std::endl;
    processPacketData(packet, timestamp, data);

}

void UwbSubscriber::registerCallback(const std::function<void(int)>& _callback) {
    callbacks.push_back(_callback);
}

std::string UwbSubscriber::getPacketFrameID() {
    return packetFrameID = m_frameId;
}

void UwbSubscriber::setupSubscriber(const std::string& uwbNum) {
    std::cout << "UWB test setup starting..." << std::endl;
    int temp = std::stoi(uwbNum) + 1;
    m_frameId = "tag" + std::to_string(temp);
    std::ostringstream topic_name_stream;
    topic_name_stream << "/dwm1001/anchor/ttyUWB" << uwbNum;

    std::string topic_name = topic_name_stream.str();

    ROS_INFO("topic_name-->%s", topic_name.c_str());

    r_sh_UWB = node.subscribe<ipe_v4_2::Anchor>(topic_name, 10, &UwbSubscriber::_callback, this);

}

void UwbSubscriber::_callback(const ipe_v4_2::Anchor::ConstPtr& msg) {
    m_ipeDataPacket = IPEDataPacket(msg);
    m_ipeDataPacket.frame_id = m_frameId;
    _setRxid(msg);
    
    if (o_ipeCallback) {
        o_ipeCallback->onLiveDataAvailable(m_ipeDataPacket);
    } else {
        ROS_WARN("m_kapCallback is a nullptr!");
    }
}
void UwbSubscriber::_setRxid(const ipe_v4_2::Anchor::ConstPtr& msg) {
    std::vector<std::string> newIds = msg->id;
    std::vector<double> newXs = msg->x;
    std::vector<double> newYs = msg->y;
    std::vector<double> newZs = msg->z;

    // Check for new IDs and update RxID_data_list
    for (const auto& newId : newIds) {
        if (std::find(RxID_data_list.begin(), RxID_data_list.end(), newId) == RxID_data_list.end()) {
            RxID_data_list.push_back(newId);
            // Add default values for new IDs to xain and yain lists
            xain_list.push_back(0.0);
            yain_list.push_back(0.0);
            zain_list.push_back(0.0);
        }
    }

    // Update xain_list and yain_list based on newIds
    for (size_t i = 0; i < newIds.size(); ++i) {
        auto idIndex = std::find(RxID_data_list.begin(), RxID_data_list.end(), newIds[i]) - RxID_data_list.begin();
        if (idIndex < RxID_data_list.size()) {
            xain_list[idIndex] = newXs[i];
            yain_list[idIndex] = newYs[i];
            zain_list[idIndex] = newZs[i];
        }
    }

    // Update RxID_list
    RxID_list.clear();
    for (size_t i = 0; i < RxID_data_list.size(); ++i) {
        RxID_list.push_back(i);
    }
}


double UwbSubscriber::extractNumber(const std::string& input) {
    double value = 0.0;
    for (char c : input) {
        if (std::isdigit(c)) {
            value = value * 10 + (c - '0');
        }
    }
    return value;
}

void UwbSubscriber::sendUDPMessage(double center_x, double center_y, double heading) {
    std::ostringstream oss;
    oss << center_x << "," << center_y << "," << heading;
    std::string result = oss.str();
    socketManager->broadcastUDPMessage(result);
}


void UwbSubscriber::processPacketData(IPEDataPacket &packet, double timestamp, SensorData* newUWBData) 
{
    double Ln = 23;
    double Lp = 4;
    double LnC = static_cast<double>(RxID_data_list.size());
    double TagNum = extractNumber(packet.frame_id);
    Nanchor = packet.id.size();
    
    double PositionVector_data[150] = {};
    int PositionVector_size[2];
    
    // double PositionOut[7]; // PositioningSystem_V2_1 version (maintained before 2024.03.28, jang.sh)
    double PositionOut[10]; // PositioningSystem_V2_2 version (after 2024.03.28, jang.sh)
    
    PositionVector_data[0] = packet.s_time;
    PositionVector_data[1] = TagNum;
    PositionVector_data[2] = Ln;
    PositionVector_data[3] = LnC;
    PositionVector_data[4] = Nanchor;

    // Process the Nanchor data in a single loop
    //zt_b = 1.53;
    //carnival
    // zt_b = 2.3;
    zt_b = 0.4;
    int index = 5 + (int)Ln;
    for (size_t i = 0; i < Nanchor; ++i) {
        auto it = std::find(RxID_data_list.begin(), RxID_data_list.end(), packet.id[i]);
        if (it != RxID_data_list.end()) {
            PositionVector_data[5 + i] = std::distance(RxID_data_list.begin(), it) + 1;         // 5 6 7 8
        }
        PositionVector_data[index + i] = packet.distanceFromTag[i];                             // 13,14,15,16
    }



    index = (int)Ln + 9;
    for (size_t i = 0; i < LnC; ++i) {
        PositionVector_data[index + i] = xain_list[i];
        PositionVector_data[index + i + (int)LnC] = yain_list[i];
        PositionVector_data[index + i + (int)LnC * 2] = zain_list[i];
    }

    
    index = (int)Ln + 3 * (int)LnC + 9;
    for (size_t i = 0; i < 4; ++i) {
        PositionVector_data[index + i] = tag_pos_b[i].re;
        PositionVector_data[index + i + 4] = tag_pos_b[i].im;
        PositionVector_data[index + i + 8] = zt_b;
    }

    bool isEmpty = true;
    
    index = 5 + (int)Ln;
    if (Nanchor > 1) {
        for(int i=0; i < (int)Nanchor; i++){
            if(PositionVector_data[index + i] == 0){
                isEmpty = false;
                break;
            }
        }

        if(isEmpty){
          
            //PositioningSystem_V4_2(PositionVector_data, PositionVector_size, PositionOut); //  Changed 2024.06.17 (joo.hy)  
            PositioningSystem_V5_1(PositionVector_data, PositionVector_size, PositionOut); //  Changed 2024.06.17 (joo.hy)
            //PositioningSystem_V5_1(PositionVector_data, PositionVector_size, MapParam, PositionOut);  // changed by joo(24.08.21, pkg_5.1.16 ~) 


            // UWB position after EKF_SLAM_UWB (Changed 08.09 (joo.hy))
            geometry_msgs::PoseStamped uwb_ps;

            uwb_ps.header.frame_id = "map";
            uwb_ps.pose.position.x = PositionOut[7];
            uwb_ps.pose.position.y = PositionOut[8];
            uwb_ps.pose.position.z = 0;

            uwb_ps.pose.orientation.w = PositionOut[9];
            uwb_ps.pose.orientation.x = 0;
            uwb_ps.pose.orientation.y = 0;
            uwb_ps.pose.orientation.z = 1;

            pub_uwb_pose.publish(uwb_ps);

            nav_msgs::Path uwb_pth; 
            uwb_pth.header.frame_id = "map";
            uwb_pth.poses.push_back(uwb_ps);
            
            pub_uwb_path.publish(uwb_pth);


        } else {
            
        }
    }
}
