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

#ifndef IPEDATAPACKER_H
#define IPEDATAPACKER_H

#include <vector>
#include <ros/time.h>

//#include <ipe/Anchor.h> 
#include <ipe_v4_2/Anchor.h> 

#include <sensor_msgs/Imu.h>
#include <iomanip>
#include "geometry_msgs/PoseStamped.h"

struct IPEDataPacket
{
    double s_time;
    int seq;
    std::string frame_id;
    std::vector<std::string> id;
    std::vector<double> x;
    std::vector<double> y;
    std::vector<double> z;
    std::vector<double> distanceFromTag;
    std::vector<double> RxID;
    std::vector<double> RxDist;

    std::vector<double> linear_x;
    std::vector<double> linear_y;
    std::vector<double> linear_z;

    std::vector<double> angular_x;
    std::vector<double> angular_y;
    std::vector<double> angular_z;

    std::vector<double> slam_pos_x;
    std::vector<double> slam_pos_y;
    std::vector<double> slam_pos_z;

    std::vector<double> slam_ort_x; 
    std::vector<double> slam_ort_y; 
    std::vector<double> slam_ort_z; 
    std::vector<double> slam_ort_w; 


     // 기본 생성자
    IPEDataPacket() = default;
    /*
    IPEDataPacket(const sensor_msgs::Imu::ConstPtr& msg) {
        // frame_id = msg->header.frame_id;
        s_time = convertToDouble(msg->header.stamp);
        
        // Assigning the values directly
        linear_x.push_back(msg->linear_acceleration.x);
        linear_y.push_back(msg->linear_acceleration.y);
        linear_z.push_back(msg->linear_acceleration.z);

        angular_x.push_back(msg->angular_velocity.x);
        angular_y.push_back(msg->angular_velocity.y);
        angular_z.push_back(msg->angular_velocity.z);
        //std:: cout << "IMU data is push_ back" << std:: endl;
    };
    */

    IPEDataPacket(const ipe_v4_2::Anchor::ConstPtr& msg) : 
        RxID(6, 0.0),     // 6개의 0.0으로 RxID 벡터 초기화
        RxDist(6, 0.0)   // 6개의 0.0으로 RxDist 벡터 초기화
    {
        seq = msg->header.seq;
        s_time = convertToDouble(msg->header.stamp);
        // std::cout<<"s_time:"<<s_time<<std::endl;
        id.assign(msg->id.begin(), msg->id.end());
        // for (const auto& val : id) {
        //     std::cout << val << " ";
        // }
        x.assign(msg->x.begin(), msg->x.end());
        y.assign(msg->y.begin(), msg->y.end());
        z.assign(msg->z.begin(), msg->z.end());
        distanceFromTag.assign(msg->distanceFromTag.begin(), msg->distanceFromTag.end());
        //std:: cout << "UWB data is push_ back" << std:: endl;
    };

    // hy.joo (2024.06.05) 
    IPEDataPacket(const geometry_msgs::PoseStamped::ConstPtr& msg) {
        //frame_id = msg->header.frame_id;
        s_time = convertToDouble(msg->header.stamp);
        
        // Assigning the values directly
  
        slam_pos_x.push_back(msg->pose.position.x);
        slam_pos_y.push_back(msg->pose.position.y);
        slam_pos_z.push_back(msg->pose.position.z);

        slam_ort_x.push_back(msg->pose.orientation.x);
        slam_ort_y.push_back(msg->pose.orientation.y);
        slam_ort_z.push_back(msg->pose.orientation.z);
        slam_ort_w.push_back(msg->pose.orientation.w);
        
    };

    double convertToDouble(const ros::Time& time) {
        // std::cout << std::fixed << std::setprecision(9); // 소수점 아래 9자리까지 표시
        // std::cout << "sec: " << static_cast<double>(time.sec) << std::endl;
        // std::cout << "nsec: " << static_cast<double>(time.nsec) << std::endl;
        double combinedTime = static_cast<double>(time.sec) + static_cast<double>(time.nsec) / 1e9;
        // std::cout << "sec + nsec: " << combinedTime << std::endl;
        return combinedTime;
    };

    bool empty(std::string frame_id) const {
        if (frame_id.empty()){
            return frame_id.empty() ||
                id.empty() ||
                x.empty() ||
                y.empty() ||
                z.empty() ||
                distanceFromTag.empty() ||
                RxID.empty() ||
                RxDist.empty(); //||
                
        }
        else if(frame_id == "imu"){
            return linear_x.empty() ||
                linear_y.empty() ||
                linear_z.empty() ||
                angular_x.empty() ||
                angular_y.empty() ||
                angular_z.empty();
        }

        //hy.joo (24.06.05) 
        else if(frame_id == "slam"){
            return slam_pos_x.empty() ||
                slam_pos_y.empty() ||
                slam_pos_z.empty() || 
                slam_ort_x.empty() ||
                slam_ort_y.empty() ||
                slam_ort_z.empty() ||
                slam_ort_w.empty();
        }

        else{
            return id.empty() ||
                x.empty() ||
                y.empty() ||
                z.empty() ||
                distanceFromTag.empty() ||
                RxID.empty() ||
                RxDist.empty();
        }
    };

    friend bool operator==(const IPEDataPacket& lhs, const IPEDataPacket& rhs) {
        return lhs.frame_id == rhs.frame_id &&
               lhs.id == rhs.id &&
               lhs.x == rhs.x &&
               lhs.y == rhs.y &&
               lhs.z == rhs.z &&
               lhs.distanceFromTag == rhs.distanceFromTag &&
               lhs.RxID == rhs.RxID &&
               lhs.RxDist == rhs.RxDist &&
               lhs.linear_x == rhs.linear_x &&
               lhs.linear_y == rhs.linear_y &&
               lhs.linear_z == rhs.linear_z &&
               lhs.angular_x == rhs.angular_x &&
               lhs.angular_y == rhs.angular_y &&
               lhs.angular_z == rhs.angular_z;
        // 필요한 경우 다른 멤버에 대한 조건을 추가할 수 있습니다.
    }
};

#endif