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

std::vector<double> xain_list;
std::vector<double> yain_list;
std::vector<double> xain_difference;
std::vector<double> yain_difference;
std::vector<std::string> RxID_data_list;
std::vector<int> RxID_list;
std::vector<std::string> difference;

// creal_T tag_pos_b[4];
// creal_T prevTagPos[4];
// creal_T tag_pos_est[4];
// creal_T tag_pos_est_aver[4];
// creal_T tag_center_vel_est;
// std::string tagNum;
// double UWBErrSum;
// double heading_est;
// double headingest_a_aver_v;
// double init_flag;
// double Nanchor;
// double zt_b;
// double prevTagHeading;

inline creal_T argInit_creal_T(int idx)
{
    creal_T result;
    std::vector<double> xt_b = {-0.525, 0.525, -0.525, 0.525};
    std::vector<double> yt_b = {0.505, 0.505, -0.505, -0.505};

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

UwbSubscriber::UwbSubscriber(ros::NodeHandle& node, const std::string& m_uwbNum, IPECallback* ipeCallback) 
: m_ipeCallback(ipeCallback), uwbNum(m_uwbNum) {  // 여기에서 uwbNum을 초기화
    setupSubscriber(node, uwbNum);
}

UwbSubscriber::~UwbSubscriber() {
}

void UwbSubscriber::operator()(IPEDataPacket &packet, double timestamp) {
    processPacketData(packet, timestamp);
}

void UwbSubscriber::registerCallback(const std::function<void(double, std::string&)>& callback) {
    callbacks.push_back(callback);
}

void UwbSubscriber::sendEvent(double data, std::string& uwbNum) {
    for (const auto& callback : callbacks) {
        callback(data, uwbNum);
    }
}

std::string UwbSubscriber::getPacketFrameID() {
    return packetFrameID = frame_id;
}

void UwbSubscriber::setupSubscriber(ros::NodeHandle& node, const std::string& uwbNum) {
    std::cout << "UWB test setup starting..." << std::endl;
    int temp = std::stoi(uwbNum) + 1;
    frame_id = "tag" + std::to_string(temp);
    topic_name_stream << "/dwm1001/anchor/ttyUWB" << uwbNum;

    std::string topic_name = topic_name_stream.str();

    ROS_INFO("topic_name-->%s", topic_name.c_str());

    pub = node.advertise<ipe::Uwbpos>("/UWB", 10);
    sub = node.subscribe<ipe::Anchor>(topic_name, 10, &UwbSubscriber::_callback, this);
    subFusion = node.subscribe<ipe::Fusion>("/Fusion", 10, &UwbSubscriber::_callback_Fusion, this);

}

void UwbSubscriber::_callback(const ipe::Anchor::ConstPtr& msg) {
    m_ipeDataPacket = IPEDataPacket(msg);
    m_ipeDataPacket.frame_id = frame_id;
    _setRxid(msg);
    
    if (m_ipeCallback) {
        m_ipeCallback->onLiveDataAvailable(m_ipeDataPacket);
    } else {
        ROS_WARN("m_kapCallback is a nullptr!");
    }
}

void UwbSubscriber::_callback_Fusion(const ipe::Fusion::ConstPtr& msg) {
    if (!msg){
        prevTagHeading = msg->prevTagHeading;
        init_flag = msg->init_flag;
        for(int i=0; i<4;i++){
            msg->prevTagPos[i];
            prevTagPos[i].re = msg->prevTagPos[i];
            prevTagPos[i].im = msg->prevTagPos[i+1];
        }
    }
}

void UwbSubscriber::_setRxid(const ipe::Anchor::ConstPtr& msg) {
    std::vector<std::string> newIds = msg->id;
    std::vector<double> newXs = msg->x;
    std::vector<double> newYs = msg->y;

    // Initial population
    if (RxID_list.empty() && xain_list.empty() && yain_list.empty()) {
        RxID_data_list = newIds;
        xain_list = newXs;
        yain_list = newYs;
        for (int i = 0; i < newIds.size(); ++i) {
            RxID_list.push_back(i);
        }
        return;
    }

    // Convert existing data lists to sets for easier checking
    std::set<std::string> existingIdSet(RxID_data_list.begin(), RxID_data_list.end());
    std::set<double> existingXSet(xain_list.begin(), xain_list.end());
    std::set<double> existingYSet(yain_list.begin(), yain_list.end());

    // Convert new data to sets
    std::set<std::string> newIdSet(newIds.begin(), newIds.end());
    std::set<double> newXSet(newXs.begin(), newXs.end());
    std::set<double> newYSet(newYs.begin(), newYs.end());

    if (existingIdSet != newIdSet || existingXSet != newXSet || existingYSet != newYSet) {
        for (const auto& item : newIds) {
            if (existingIdSet.find(item) == existingIdSet.end()) {
                RxID_data_list.push_back(item);
            }
        }

        for (const auto& item : newXs) {
            if (existingXSet.find(item) == existingXSet.end()) {
                xain_list.push_back(item);
            }
        }

        for (const auto& item : newYs) {
            if (existingYSet.find(item) == existingYSet.end()) {
                yain_list.push_back(item);
            }
        }
    }

    RxID_list.clear();
    for (int i = 0; i < RxID_data_list.size(); ++i) {
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

void UwbSubscriber::processPacketData(IPEDataPacket &packet, double timestamp) 
{
    PosDataPacket pos;
    double Ln = 6;
    double Lp = 4;
    double LnC = static_cast<double>(RxID_data_list.size());
    double TagNum = extractNumber(packet.frame_id) + 1;
    Nanchor = packet.id.size();

    // Process the Nanchor data in a single loop
    zt_b = 1.53;
    for (size_t i = 0; i < Nanchor; ++i) {
        auto it = std::find(RxID_data_list.begin(), RxID_data_list.end(), packet.id[i]);
        if (it != RxID_data_list.end()) {
            packet.RxID[i] = std::distance(RxID_data_list.begin(), it) + 1;
            packet.RxDist[i] = std::sqrt(std::pow(packet.distanceFromTag[i], 2) - std::pow(packet.z[i]-zt_b, 2));
        }

        // Filling in the data for arrays
    }

    double s_time = timestamp;

    double UWBout[21];
    static bool flag = false;
    if (!flag) {
        prevTagHeading = 0;
        argInit_1x4_creal_T(tag_pos_b);
        argInit_1x4_creal_preT(prevTagPos);
        flag = true;
    }

    bool isEmpty = true;

    if (Nanchor > 1) {
        for(int i=0; i < (int)Nanchor; i++){
            if(packet.RxDist.data()[i] == 0){
                isEmpty = false;
                break;
            }
        }

        if(isEmpty){
            UWBpos6(Ln, Lp, LnC, TagNum, Nanchor, packet.RxID.data(), packet.RxDist.data(), s_time, tag_pos_b, xain_list.data(), yain_list.data(), prevTagPos, prevTagHeading, UWBout);
        }
    }
    ipe::Uwbpos uwb_pos_msg;
    uwb_pos_msg.header.stamp = ros::Time::now();
    uwb_pos_msg.header.frame_id = "UWBPos";

    // // Assigning UWBout data to pos
    for (int i = 0; i < 4; i++) {
        tag_pos_est[i].re = UWBout[i];
        uwb_pos_msg.tag_pos_est.push_back(tag_pos_est[i].re);
        tag_pos_est[i].im = UWBout[i + 4];
        uwb_pos_msg.tag_pos_est.push_back(tag_pos_est[i].im);
        
        uwb_pos_msg.tag_pos_b.push_back(tag_pos_b[i].re);
        uwb_pos_msg.tag_pos_b.push_back(tag_pos_b[i].im);

        tag_pos_est_aver[i].re = UWBout[i + 9];
        tag_pos_est_aver[i].im = UWBout[i + 13];
    }

    heading_est = UWBout[8];
    headingest_a_aver_v = UWBout[17];

    for (int i = 0; i < 4; i++) {
        pos.tag_pos_est[i] = prevTagPos[i];
        pos.tag_pos_est_aver[i] = tag_pos_est_aver[i];


    }

    pos.heading_est = prevTagHeading;
    pos.headingest_a_aver_v = headingest_a_aver_v;

    for (size_t i = 0; i < yain_list.size(); ++i) {
        pos.x[i] = xain_list[i];
        pos.y[i] = yain_list[i];
    }

    tag_center_vel_est.re = UWBout[18];
    tag_center_vel_est.im = UWBout[19];

    UWBErrSum = UWBout[20];
    uwb_pos_msg.UWBErrSum = UWBErrSum;
    uwb_pos_msg.heading_est = heading_est;
    uwb_pos_msg.Nanchor = Nanchor;
    uwb_pos_msg.zt_b = zt_b;

    if (heading_est != 0 && init_flag == 0) {
        init_flag = 1;
    } else if (heading_est != 0 && init_flag ==1) {
        init_flag = 1;
    } else if (init_flag == 2) {
        init_flag = 3;
    } else if (init_flag == 3) {
        init_flag = 3;
    } else {
        init_flag = 0;
    }
    uwb_pos_msg.init_flag = init_flag;
    
    uwb_pos_msg.tag_center_vel_est.push_back(tag_center_vel_est.re);
    uwb_pos_msg.tag_center_vel_est.push_back(tag_center_vel_est.im);

    pub.publish(uwb_pos_msg);

    sendEvent(init_flag, uwbNum);
}