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

#include <ros/ros.h>
#include <rosbag/bag.h>
#include <rosbag/view.h>
#include "mainwindow.h"

#include <run/Anchor.h>

int main(int argc, char** argv) {
    ros::init(argc, argv, "rosbag_republisher_node");
    ros::NodeHandle nh;

    MainWindow *ketiPs = new MainWindow();

    try {
        ketiPs->registerSubcribers(nh);
    } catch (const std::exception& e) {
        ROS_ERROR("%s", e.what());
        delete ketiPs;
        return 1;  // 프로그램을 오류 코드와 함께 종료합니다.
    }

    rosbag::Bag bag;
    try {
        bag.open("/home/umaps/rosbag/[xsens]2023-08-31-17-42-15_fast.bag", rosbag::bagmode::Read);
    } catch (rosbag::BagException& e) {
        ROS_ERROR("Error opening bag file: %s", e.what());
        return 1;
    }

    std::vector<std::string> topics = {
        "/dwm1001/anchor/ttyUWB0",
        "/dwm1001/anchor/ttyUWB1",
        "/dwm1001/anchor/ttyUWB2",
        "/dwm1001/anchor/ttyUWB3"
    };

    std::map<std::string, ros::Publisher> publishers;

    for (const auto& topic : topics) {
        publishers[topic] = nh.advertise<run::Anchor>(topic, 10);
    }

    rosbag::View view(bag);

    for (const rosbag::MessageInstance& message : view) {
        if (publishers.find(message.getTopic()) != publishers.end()) {
            run::Anchor::ConstPtr uwb_data = message.instantiate<run::Anchor>();
            if (uwb_data != NULL) {
                publishers[message.getTopic()].publish(uwb_data);
                ketiPs->spinFor();  // I assume you meant to use std::chrono::milliseconds
                ros::spinOnce();
            }
        }
        
    }
 
    bag.close();
    ROS_INFO("Finish...");
    delete ketiPs;
    return 0;
}
