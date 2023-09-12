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

#ifndef UWBSUBSCRIBER_H
#define UWBSUBSCRIBER_H

#include <sstream>
#include <set>

#include <ros/ros.h>
#include <unordered_map>
#include <boost/variant.hpp>
#include <complex>
#include <run/Anchor.h>
#include "packetcallback.h"
#include "kapcallback.h"

#include "rtwtypes.h"
#include "UWBpos6.h"
#include "UWBpos6_terminate.h"
#include "rt_nonfinite.h"

static std::vector<std::string> RxID_data_list;
static std::vector<int> RxID_list;
static bool statusUWB;
static std::vector<std::string> difference; // Add this line if difference is used in this file
static creal_T tag_pos_b[4];
static creal_T prevTagPos[4];
static std::string tagNum;


static creal_T argInit_creal_T(void)
{
    creal_T result;
    std::vector<double> xt_b = {-0.525, 0.525, -0.525, 0.525};
    std::vector<double> yt_b = {0.505, 0.505, -0.505, -0.505};

    for (size_t i = 0; i < xt_b.size(); ++i) {
        result.re = xt_b[i];
        result.im = yt_b[i];
    }
    return result;
};

static creal_T argInit_creal_preT(void)
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

static void argInit_1x4_creal_T(creal_T result[4])
{
    int idx1;
    /* Loop over the array to initialize each element. */
    for (idx1 = 0; idx1 < 4; idx1++) {
        result[idx1] = argInit_creal_T();
    }
};

static void argInit_1x4_creal_preT(creal_T result[4])
{
    int idx1;
    /* Loop over the array to initialize each element. */
    for (idx1 = 0; idx1 < 4; idx1++) {
        result[idx1] = argInit_creal_preT();
    }
};

struct Operator 
{
    double zt_b = 2.30;
    double Ln = 6;
    double Lp = 4;

    Operator(KapDataPacket &packet, ros::Time timestamp)
    {
        std::stringstream ss;
        std::stringstream ss2;
        
        UwbSubscriber_setRxid(packet);  // 정확한 함수 호출

        // packet.x와 packet.y의 실제 길이에 따라 배열의 크기를 정의
        size_t numPoints = packet.id.size(); // 만약 id의 크기가 x와 y와 동일하다면, 그렇지 않으면 적절한 필드를 사용하여 정의하십시오.
        double xain[numPoints];
        double yain[numPoints];

        for (size_t i = 0; i < numPoints; ++i) {
            auto it = std::find(RxID_data_list.begin(), RxID_data_list.end(), packet.id[i]);
            if (it != RxID_data_list.end()) {
                packet.RxID[i] = std::distance(RxID_data_list.begin(), it) + 1;
                packet.RxDist[i] = std::sqrt(std::pow(packet.distanceFromTag[i], 2) - std::pow(packet.z[i], 2));
                ss2 << packet.distanceFromTag[i] << " ";
                ss << packet.RxDist[i] << " ";
            }
            
            // 배열에 값을 할당
            xain[i] = packet.x[i];
            yain[i] = packet.y[i];
        }
        double LnC = RxID_data_list.size();
        double TagNum = extractNumber(packet.frame_id);
        double Nanchor = packet.id.size();
        const double RxIDin[6] = {packet.RxID[0], packet.RxID[1], packet.RxID[2], packet.RxID[3], packet.RxID[4], packet.RxID[5]};
        const double RxDistin[6] = {packet.RxDist[0], packet.RxDist[1], packet.RxDist[2], packet.RxDist[3], packet.RxDist[4], packet.RxDist[5]};
        double s_time = timestamp.toSec();
        double UWBout[21];
        double prevTagHeading = 0;
        argInit_1x4_creal_T(tag_pos_b);
        argInit_1x4_creal_preT(prevTagPos);

        UWBpos6(Ln, Lp, LnC, TagNum, Nanchor, RxIDin, RxDistin, s_time, tag_pos_b, xain, yain, prevTagPos, prevTagHeading, UWBout);

        std::vector<creal_T> tag_pos_est(4);   // Each element should be an array or another container. This depends on the exact type of data.
        std::vector<creal_T> tag_pos_est_aver(4);

        tag_pos_est[0].re = UWBout[0];
        tag_pos_est[1].re = UWBout[1];
        tag_pos_est[2].re = UWBout[2];
        tag_pos_est[3].re = UWBout[3];

        tag_pos_est[0].im = UWBout[4];
        tag_pos_est[1].im = UWBout[5];
        tag_pos_est[2].im = UWBout[6];
        tag_pos_est[3].im = UWBout[7];

        ROS_INFO("tag_pos_est--> (%f,%f), (%f,%f)", tag_pos_est[0].re, tag_pos_est[0].im, tag_pos_est[1].re, tag_pos_est[1].im);
        ROS_INFO("tag_pos_est--> (%f,%f), (%f,%f)", tag_pos_est[2].re, tag_pos_est[2].im, tag_pos_est[3].re, tag_pos_est[3].im);

        prevTagPos[0].re = UWBout[0];
        prevTagPos[1].re = UWBout[1];
        prevTagPos[2].re = UWBout[2];
        prevTagPos[3].re = UWBout[3];
        
        prevTagPos[0].im = UWBout[4];
        prevTagPos[1].im = UWBout[5];
        prevTagPos[2].im = UWBout[6];
        prevTagPos[3].im = UWBout[7];
        

        double heading_est = UWBout[8];
        ROS_INFO("heading_est--> %f", heading_est);

        tag_pos_est_aver[0].re = UWBout[9];
        tag_pos_est_aver[1].re = UWBout[10];
        tag_pos_est_aver[2].re = UWBout[11];
        tag_pos_est_aver[3].re = UWBout[12];

        tag_pos_est_aver[0].im = UWBout[13];
        tag_pos_est_aver[1].im = UWBout[14];
        tag_pos_est_aver[2].im = UWBout[15];
        tag_pos_est_aver[3].im = UWBout[16];

        ROS_INFO("tag_pos_est_aver--> (%f,%f), (%f,%f)", tag_pos_est_aver[0].re, tag_pos_est_aver[0].im, tag_pos_est_aver[1].re, tag_pos_est_aver[1].im);
        ROS_INFO("tag_pos_est_aver--> (%f,%f), (%f,%f)", tag_pos_est_aver[2].re, tag_pos_est_aver[2].im, tag_pos_est_aver[3].re, tag_pos_est_aver[3].im);

        double headingest_a_aver_v = UWBout[17];
        ROS_INFO("headingest_a_aver_v--> %f", headingest_a_aver_v);
    }

    double extractNumber(const std::string& input) {
        std::string numberStr;
        
        // Remove non-numeric characters
        for (char c : input) {
            if (std::isdigit(c)) {
                numberStr += c;
            }
        }

        // Convert the numeric string to double
        return std::stod(numberStr);
    }

    void UwbSubscriber_setRxid(const KapDataPacket &packet)
    {
        std::vector<std::string> ids = packet.id;  // Initialize ids from msg
        if (RxID_list.empty())
        {
            RxID_data_list = packet.id;
            for (int i = 0; i < ids.size(); ++i)  // Initialize i and use ids
            {
                RxID_list.push_back(i);
            }
        }
        else
        {
            std::set<std::string> ids_set(ids.begin(), ids.end());
            std::set<std::string> RxID_data_set(RxID_data_list.begin(), RxID_data_list.end());

            if (ids_set != RxID_data_set) 
            {
                difference.clear();
                for (const auto& item : ids) 
                {
                    if (RxID_data_set.find(item) == RxID_data_set.end()) 
                    {
                        difference.push_back(item);
                    }
                }
            }   
            RxID_data_list.insert(RxID_data_list.end(), difference.begin(), difference.end());
            RxID_list.clear();
            for (int i = 0; i < RxID_data_list.size(); ++i) 
            {
                RxID_list.push_back(i);
            }
        }   
    }
};

struct UwbSubscriber 
{
    ros::Subscriber sub;
    std::string frame_id = "tag";
    std::ostringstream topic_name_stream;
    KapCallback* m_kapCallback;
    
    
    // Method to get name
    std::string getName() const {
        return frame_id;  // or whatever you wish to return as name
    }

    UwbSubscriber(ros::NodeHandle& node, std::string uwbNum, KapCallback* kapCallback)
        : m_kapCallback(kapCallback)
    {
        bool statusUWB = false;
        tagNum = uwbNum;
        int temp = std::stoi(uwbNum) + 1; // 숫자로 변환 후 1을 더함
        frame_id = std::to_string(temp); // 다시 문자열로 변환
        topic_name_stream << "/dwm1001/anchor/ttyUWB" << tagNum;

        std::string topic_name = topic_name_stream.str();

        ROS_INFO("topic_name-->%s", topic_name.c_str());

        sub = node.subscribe<run::Anchor>(topic_name, 10, &UwbSubscriber::UwbSubscriber_callback, this);
    }

    void UwbSubscriber_callback(const run::Anchor::ConstPtr& msg)
    {
        KapDataPacket packet;

        for (size_t i = 0; i < 6; ++i) {
            packet.x.push_back(0);
            packet.y.push_back(0);
            packet.z.push_back(0);
            packet.RxID.push_back(0);
            packet.RxDist.push_back(0);
        }

        packet.stamp.sec = msg->header.stamp.sec;
        packet.stamp.nsec = msg->header.stamp.nsec;
        packet.frame_id = frame_id;
        packet.id = msg->id;
        for (int i = 0; i < packet.id.size(); i++)
        {
            packet.x[i] = msg->x[i];
            packet.y[i] = msg->y[i];
            packet.z[i] = msg->z[i];    
        }
        packet.distanceFromTag = msg->distanceFromTag;

        // Check if m_kapCallback is not a nullptr before invoking the function
        if (m_kapCallback)
        {
            m_kapCallback->onLiveDataAvailable(packet);
        }
        else
        {
            ROS_WARN("m_kapCallback is a nullptr!");
        }
    }

    bool getDataEmpty(){
        m_kapCallback->getDataEmpty();
        return m_kapCallback->getDataEmpty();
    }

};

#endif
