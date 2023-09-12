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

#include "mainwindow.h"

MainWindow::MainWindow()
{
    ROS_INFO("Create Positioning object...");
}

MainWindow::~MainWindow()
{
    ROS_INFO("Cleaning up ...");
}
void MainWindow::spinFor(std::chrono::milliseconds timeout)
{
    RosKapDataPacket* earliestPacket = nullptr;
    UwbSubscriber* correspondingSubscriber = nullptr;  // <-- Add this line to remember the subscriber
    ROS_INFO("Start spinFor...");
    
    for (auto &cb : m_callbacks)
    {
        if (!cb->getDataEmpty())
        {
            RosKapDataPacket rosPacket = cb->m_kapCallback->next(timeout);
            
            // 첫 번째 패킷 또는 이전에 발견된 패킷보다 더 이른 패킷을 찾는 경우
            if (!earliestPacket || rosPacket.second.stamp < earliestPacket->second.stamp)
            {
                earliestPacket = &rosPacket;
                correspondingSubscriber = cb;  // <-- Update the subscriber
            }
        }
    }

    // 최소 타임스탬프 패킷을 찾은 경우 Operator 함수에 전달하고 pop
    if (earliestPacket)
    {
        Operator(earliestPacket->second, earliestPacket->first);
        
        // TODO: pop 함수를 호출하여 earliestPacket 삭제
        if(correspondingSubscriber) {
            correspondingSubscriber->m_kapCallback->pop(*earliestPacket);
        }
    }
}

void MainWindow::registerSubcribers(ros::NodeHandle &node) {
    bool should_publish;

    if (ros::param::get("/run_node/sub_UWB0", should_publish) && should_publish) {
        registerCallback(new UwbSubscriber(node, "0", &m_kapCallback));
    }
    if (ros::param::get("/run_node/sub_UWB1", should_publish) && should_publish) {
        registerCallback(new UwbSubscriber(node, "1", &m_kapCallback));
    }
    if (ros::param::get("/run_node/sub_UWB2", should_publish) && should_publish) {
        registerCallback(new UwbSubscriber(node, "2", &m_kapCallback));
    }
    if (ros::param::get("/run_node/sub_UWB3", should_publish) && should_publish) {
        registerCallback(new UwbSubscriber(node, "3", &m_kapCallback));
    }
    // ... (repeat for other UWB nodes)
}

void MainWindow::registerCallback(UwbSubscriber *cb)  // Make sure PacketCallback is defined
{
    m_callbacks.push_back(cb);

    std::string callbacks_info = "Registered Callbacks: ";
    for (const auto& callback : m_callbacks) {
        callbacks_info += "[" + callback->getName() + "] ";  // Assuming you have a `getName()` function in your UwbSubscriber class
    }
    ROS_INFO("%s", callbacks_info.c_str());
}

bool MainWindow::handleError(std::string error)
{
    ROS_ERROR("%s", error.c_str());
    return false;
}
