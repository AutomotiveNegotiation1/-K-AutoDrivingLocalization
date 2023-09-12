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
#include <map>
#include <ros/time.h>

struct KapDataPacket
{
    ros::Time stamp;
    std::string frame_id;
    std::vector<std::string> id;
    std::vector<double> x;
    std::vector<double> y;
    std::vector<double> z;
    std::vector<double> distanceFromTag;
    std::vector<double> RxID;
    std::vector<double> RxDist;

    KapDataPacket(){} // constructor that initializes d to nullptr

    bool empty() const {
        return stamp.isZero() ||                // ros::Time의 isZero() 함수는 시간이 0인지 확인합니다.
            frame_id.empty() ||
            id.empty() ||
            x.empty() ||
            y.empty() ||
            z.empty() ||
            distanceFromTag.empty() ||
            RxID.empty() ||
            RxDist.empty();
    }

    friend bool operator==(const KapDataPacket& lhs, const KapDataPacket& rhs) {
        return lhs.stamp == rhs.stamp &&
               lhs.frame_id == rhs.frame_id &&
               lhs.id == rhs.id &&
               lhs.x == rhs.x &&
               lhs.y == rhs.y &&
               lhs.z == rhs.z &&
               lhs.distanceFromTag == rhs.distanceFromTag &&
               lhs.RxID == rhs.RxID &&
               lhs.RxDist == rhs.RxDist;
        // You can add conditions for other members if necessary.
    }
};

#endif
