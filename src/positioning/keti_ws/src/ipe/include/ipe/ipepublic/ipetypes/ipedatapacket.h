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
#include <ipe/Anchor.h>
#include <sensor_msgs/Imu.h>
#include "rtwtypes.h"
#include "rt_nonfinite.h"

struct IPEDataPacket
{
    double s_time;
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

     // 기본 생성자
    IPEDataPacket() = default;

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
    };

    IPEDataPacket(const ipe::Anchor::ConstPtr& msg) : 
        RxID(6, 0.0),     // 6개의 0.0으로 RxID 벡터 초기화
        RxDist(6, 0.0)   // 6개의 0.0으로 RxDist 벡터 초기화
    {
        s_time = convertToDouble(msg->header.stamp);
        id.assign(msg->id.begin(), msg->id.end());
        // for (const auto& val : id) {
        //     std::cout << val << " ";
        // }
        std::cout << std::endl;
        x.assign(msg->x.begin(), msg->x.end());
        y.assign(msg->y.begin(), msg->y.end());
        z.assign(msg->z.begin(), msg->z.end());
        distanceFromTag.assign(msg->distanceFromTag.begin(), msg->distanceFromTag.end());
    };

    double convertToDouble(const ros::Time& time) {
        return static_cast<double>(time.sec) + static_cast<double>(time.nsec) / 1e9;
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