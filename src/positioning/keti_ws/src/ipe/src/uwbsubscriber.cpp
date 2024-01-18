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

UwbSubscriber::UwbSubscriber(ros::NodeHandle& _node, const std::string& _uwbNum, IPECallback* _ipeCallback)
: node(_node), o_ipeCallback(_ipeCallback), m_uwbNum(_uwbNum){
    socketManager = SocketManager::getInstance(); // <-- Add this line to initialize the socketManager
    prevTagHeading = 0;
    argInit_1x4_creal_T(tag_pos_b);
    argInit_1x4_creal_preT(prevTagPos);
}

UwbSubscriber::~UwbSubscriber() {
}

void UwbSubscriber::operator()(IPEDataPacket &packet, double timestamp, SensorData* data) {
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
    topic_name_stream << "/dwm1001/anchor/tag" << uwbNum;

    std::string topic_name = topic_name_stream.str();

    ROS_INFO("topic_name-->%s", topic_name.c_str());

    r_sh_UWB = node.subscribe<ipe::Anchor>(topic_name, 10, &UwbSubscriber::_callback, this);

}

void UwbSubscriber::_callback(const ipe::Anchor::ConstPtr& msg) {
    m_ipeDataPacket = IPEDataPacket(msg);
    m_ipeDataPacket.frame_id = m_frameId;
    _setRxid(msg);
    
    if (o_ipeCallback) {
        o_ipeCallback->onLiveDataAvailable(m_ipeDataPacket);
    } else {
        ROS_WARN("m_kapCallback is a nullptr!");
    }
}
void UwbSubscriber::_setRxid(const ipe::Anchor::ConstPtr& msg) {
    std::vector<std::string> newIds = msg->id;
    std::vector<double> newXs = msg->x;
    std::vector<double> newYs = msg->y;

    // Check for new IDs and update RxID_data_list
    for (const auto& newId : newIds) {
        if (std::find(RxID_data_list.begin(), RxID_data_list.end(), newId) == RxID_data_list.end()) {
            RxID_data_list.push_back(newId);
            // Add default values for new IDs to xain and yain lists
            xain_list.push_back(0.0);
            yain_list.push_back(0.0);
        }
    }

    // Update xain_list and yain_list based on newIds
    for (size_t i = 0; i < newIds.size(); ++i) {
        auto idIndex = std::find(RxID_data_list.begin(), RxID_data_list.end(), newIds[i]) - RxID_data_list.begin();
        if (idIndex < RxID_data_list.size()) {
            xain_list[idIndex] = newXs[i];
            yain_list[idIndex] = newYs[i];
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
    ROS_INFO("positioning-->%s", result.c_str());
    socketManager->broadcastUDPMessage(result);
}


void UwbSubscriber::processPacketData(IPEDataPacket &packet, double timestamp, SensorData* newUWBData) 
{
    double Ln = 6;
    double Lp = 4;
    double LnC = static_cast<double>(RxID_data_list.size());
    double TagNum = extractNumber(packet.frame_id);
    Nanchor = packet.id.size();

    ROS_INFO("---------------------------------------------------");
    ROS_INFO("Tag Number: %f", TagNum);
    ROS_INFO("Sequence: %d", packet.seq);

    std::ostringstream anchor_info;
    for (int i = 0; i < Nanchor; ++i) {
        anchor_info << "AN" << (i + 1) << ": " << packet.id[i] << " - " << packet.distanceFromTag[i];
        if (i < Nanchor - 1) {
            anchor_info << " = ";
        }
    }
    ROS_INFO("%s", anchor_info.str().c_str());

    
    std::vector<double> newXs = packet.x;
    newXs.push_back(0);
    newXs.push_back(0);
    std::vector<double> newYs = packet.y;
    newYs.push_back(0);
    newYs.push_back(0);

    // Process the Nanchor data in a single loop
    zt_b = 1.53;
    for (size_t i = 0; i < Nanchor; ++i) {
        auto it = std::find(RxID_data_list.begin(), RxID_data_list.end(), packet.id[i]);
        if (it != RxID_data_list.end()) {
            packet.RxID[i] = std::distance(RxID_data_list.begin(), it) + 1;
            
        }
        // packet.RxDist[i] = packet.distanceFromTag[i];
        packet.RxDist[i] = std::real(std::sqrt(std::pow(packet.distanceFromTag[i], 2) - std::pow(packet.z[i] - zt_b, 2)));
        // Filling in the data for arrays
    }

    double s_time = timestamp;
    double UWBout[21];

    bool isEmpty = true;

    if (Nanchor > 1) {
        for(int i=0; i < (int)Nanchor; i++){
            if(packet.RxDist.data()[i] == 0){
                isEmpty = false;
                break;
            }
        }

        if(isEmpty){
            newUWBData->Ln = Ln;
            newUWBData->Lp = Lp;
            newUWBData->LnC = LnC;
            newUWBData->PP = TagNum;
            newUWBData->Nanchor = Nanchor;
            newUWBData->RxIDUWB1 = packet.RxID[0];
            newUWBData->RxIDUWB2 = packet.RxID[1];
            newUWBData->RxIDUWB3 = packet.RxID[2];
            newUWBData->RxIDUWB4 = packet.RxID[3];
            newUWBData->RxIDUWB5 = packet.RxID[4];
            newUWBData->RxIDUWB6 = packet.RxID[5];
            newUWBData->RxDistUWB1 = packet.RxDist[0];
            newUWBData->RxDistUWB2 = packet.RxDist[1];
            newUWBData->RxDistUWB3 = packet.RxDist[2];
            newUWBData->RxDistUWB4 = packet.RxDist[3];
            newUWBData->RxDistUWB5 = packet.RxDist[4];
            newUWBData->RxDistUWB6 = packet.RxDist[5];
            newUWBData->s_time = s_time; 
            newUWBData->real_tag_pos_b1 = tag_pos_est[0].re;
            newUWBData->imag_tag_pos_b1 = tag_pos_est[0].im;
            newUWBData->real_tag_pos_b2 = tag_pos_est[1].re;
            newUWBData->imag_tag_pos_b2 = tag_pos_est[1].im;
            newUWBData->real_tag_pos_b3 = tag_pos_est[2].re;
            newUWBData->imag_tag_pos_b3 = tag_pos_est[2].im;
            newUWBData->real_tag_pos_b4 = tag_pos_est[3].re;
            newUWBData->imag_tag_pos_b4 = tag_pos_est[3].im;
            newUWBData->xain1 = xain_list[0];
            newUWBData->xain2 = xain_list[1];
            newUWBData->xain3 = xain_list[2];
            newUWBData->xain4 = xain_list[3];
            newUWBData->xain5 = xain_list[4];
            newUWBData->xain6 = xain_list[5];
            newUWBData->yain1 = yain_list[0];
            newUWBData->yain2 = yain_list[1];
            newUWBData->yain3 = yain_list[2];
            newUWBData->yain4 = yain_list[3];
            newUWBData->yain5 = yain_list[4];
            newUWBData->yain6 = yain_list[5];
            newUWBData->real_TagPos1 = prevTagPos[0].re;
            newUWBData->imag_TagPos1 = prevTagPos[0].im;
            newUWBData->real_TagPos2 = prevTagPos[1].re;
            newUWBData->imag_TagPos2 = prevTagPos[1].im;
            newUWBData->real_TagPos3 = prevTagPos[2].re;
            newUWBData->imag_TagPos3 = prevTagPos[2].im;
            newUWBData->real_TagPos4 = prevTagPos[3].re;
            newUWBData->imag_TagPos4 = prevTagPos[3].im;
            newUWBData->kf_psi = kf_psi;

            ROS_INFO("yain6: %f", yain_list[5]);
            if (RxID_data_list.size() > 5) { // Checks if there are at least 6 elements
                ROS_INFO("yain6 ID: %s", RxID_data_list[5].c_str());
            }

            
            UWBpos6(Ln, Lp, LnC, TagNum, Nanchor, packet.RxID.data(), packet.RxDist.data(), s_time, tag_pos_b, xain_list.data(), yain_list.data(), prevTagPos, -kf_psi, UWBout);
            // // Assigning UWBout data to pos
            for (int i = 0; i < 4; i++) {
                tag_pos_est[i].re = UWBout[i];
                tag_pos_est[i].im = UWBout[i + 4];

                tag_pos_est_aver[i].re = UWBout[i + 9];
                tag_pos_est_aver[i].im = UWBout[i + 13];
            }

            heading_est = UWBout[8];
            headingest_a_aver_v = UWBout[17];


            tag_center_vel_est.re = UWBout[18];
            tag_center_vel_est.im = UWBout[19];

            UWBErrSum = UWBout[20];

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
            // kf_psi = -kf_psi;

            IMUposU = fusion2(kl, imuNum, tag_pos_est, tag_center_vel_est, cent_pos_est,
                        cent_vel_est, b_acc_o, acc_b_phi, kalman_on, init_flag, UWBErrSum, &kf_psi,
                        &gyro_psi, heading_est, acc_b_theta, 2, Nanchor, state_o);
            // kf_psi = -kf_psi;
            // ROS_INFO("b_acc_o1 : %f", b_acc_o[0]);
            // ROS_INFO("b_acc_o2 : %f", b_acc_o[1]);
            // ROS_INFO("b_acc_o3 : %f", b_acc_o[2]);
            if (init_flag == 1){
                gyro_psi = -heading_est;
                kf_psi = gyro_psi;
                cent_pos_est[0] = IMUposU.re;
                cent_pos_est[1] = IMUposU.im;
                cent_pos_est[1] = 0;
                cent_vel_est[0] = tag_center_vel_est.re;
                cent_vel_est[1] = tag_center_vel_est.im;
                cent_vel_est[2] = 0;
            }

            
            if (init_flag == 1){
                init_flag = 2;
            }
            kl++;

            sendUDPMessage(cent_pos_est[0], cent_pos_est[1], kf_psi);

            ROS_INFO("---------------------------------------------------");

        }
    }
    // kl++;
    std::complex<real_T> j(0, 1); // 복소수 단위

    // creal_T cent_pos_est_;
    creal_T current_tag_pos_b[4];
    for (int i = 0; i < 4; ++i) {                    
        std::complex<double> cent_pos_est_c(IMUposU.re, IMUposU.im);
        std::complex<double> current_tag_pos_b_c(tag_pos_b[i].re, tag_pos_b[i].im);
        std::complex<double> TagPos = cent_pos_est_c + std::exp(j * (-kf_psi)) * (current_tag_pos_b_c + 0.4 * j);
        creal_T TagPos_;
        TagPos_.re = std::real(TagPos);
        TagPos_.im = std::imag(TagPos);
        prevTagPos[i].re = TagPos_.re;
        prevTagPos[i].im = TagPos_.im;
    }
    // ROS_INFO("kf_psi-->%f", state_o);




}