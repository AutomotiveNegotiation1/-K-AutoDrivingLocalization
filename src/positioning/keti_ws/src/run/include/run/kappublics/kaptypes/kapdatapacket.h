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

#ifndef KAPDATAPACKET_H
#define KAPDATAPACKET_H

#include <vector>
#include <ros/time.h>
#include <run/Anchor.h>

struct KapDataPacket
{
    std::string frame_id;
    std::vector<std::string> id;
    std::vector<double> x;
    std::vector<double> y;
    std::vector<double> z;
    std::vector<double> distanceFromTag;
    std::vector<double> RxID;
    std::vector<double> RxDist;

     // 기본 생성자
    KapDataPacket() = default;

    KapDataPacket(const run::Anchor::ConstPtr& msg) : 
        RxID(6, 0.0),     // 6개의 0.0으로 RxID 벡터 초기화
        RxDist(6, 0.0)   // 6개의 0.0으로 RxDist 벡터 초기화
    {
        frame_id = msg->header.frame_id;
        
        id.assign(msg->id.begin(), msg->id.end());
        for (const auto& val : id) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
        x.assign(msg->x.begin(), msg->x.end());
        y.assign(msg->y.begin(), msg->y.end());
        z.assign(msg->z.begin(), msg->z.end());
        distanceFromTag.assign(msg->distanceFromTag.begin(), msg->distanceFromTag.end());
        // RxID와 RxDist는 이미 초기화됩니다.
    }

    bool empty() const {
        return frame_id.empty() ||
               id.empty() ||
               x.empty() ||
               y.empty() ||
               z.empty() ||
               distanceFromTag.empty() ||
               RxID.empty() ||
               RxDist.empty();
    }

    friend bool operator==(const KapDataPacket& lhs, const KapDataPacket& rhs) {
        return lhs.frame_id == rhs.frame_id &&
               lhs.id == rhs.id &&
               lhs.x == rhs.x &&
               lhs.y == rhs.y &&
               lhs.z == rhs.z &&
               lhs.distanceFromTag == rhs.distanceFromTag &&
               lhs.RxID == rhs.RxID &&
               lhs.RxDist == rhs.RxDist;
        // 필요한 경우 다른 멤버에 대한 조건을 추가할 수 있습니다.
    }
};

#endif
