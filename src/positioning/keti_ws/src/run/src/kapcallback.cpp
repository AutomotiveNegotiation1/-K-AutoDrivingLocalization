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

#include "kapcallback.h"

KapCallback::KapCallback(size_t maxBufferSize)
    : m_maxBufferSize(maxBufferSize)
{
}

KapCallback::~KapCallback() throw()
{
}

RosKapDataPacket KapCallback::next(const std::chrono::milliseconds &timeout)
{
    RosKapDataPacket packet;
    std::unique_lock<std::mutex> lock(m_mutex);

    if (m_condition.wait_for(lock, timeout, [&] { return !m_buffer.empty(); }))
    {
        packet = m_buffer.front();
        // m_buffer.pop_front();
    }

    return packet;
}

void KapCallback::pop(const RosKapDataPacket &targetPacket)
{
    std::unique_lock<std::mutex> lock(m_mutex);

    auto it = std::find(m_buffer.begin(), m_buffer.end(), targetPacket);
    if (it != m_buffer.end())
    {
        m_buffer.erase(it);
    }
}

bool KapCallback::getDataEmpty() {
    std::unique_lock<std::mutex> lock(m_mutex);

    if (m_buffer.empty()) {
        return true;
    }

    RosKapDataPacket packet = m_buffer.front();

    // Assuming KapDataPacket has a member or method that helps determine if it's "empty"
    // Adjust the next line accordingly
    if (packet.second.id.empty()) { 
        return true;
    } else {
        return false;
    }
}

void KapCallback::onLiveDataAvailable(KapDataPacket packet)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    ros::Time now = ros::Time::now();

    if (m_buffer.size() == m_maxBufferSize)
    {
        m_buffer.pop_front();
    }

    m_buffer.push_back(RosKapDataPacket(now, packet));

    lock.unlock();
    m_condition.notify_one();
}
