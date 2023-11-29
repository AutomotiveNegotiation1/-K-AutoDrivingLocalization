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

#include "ipecallback.h"

IPECallback::IPECallback(size_t maxBufferSize)
    : m_maxBufferSize(maxBufferSize)
{
}

IPECallback::~IPECallback() throw()
{
}

RosKapDataPacket IPECallback::next()
{
    RosKapDataPacket packet;
    if (m_buffer.empty()) 
    {
        return RosKapDataPacket();
    }
    packet = m_buffer.front();
    m_buffer.pop_front();
    return packet;
}

void IPECallback::pop()
{
    if (!m_buffer.empty()) 
    {
        m_buffer.pop_front();
    }
}

bool IPECallback::getDataEmpty() 
{
    if (m_buffer.empty()) 
    {
        return true;
    }

    RosKapDataPacket packet = m_buffer.front();
    if (packet.second.id.empty()) 
    {
        return true;
    } 
    else 
    {
        return false;
    }
}

// void IPECallback::onLiveDataAvailable(IPEDataPacket packet)
// {
//     // if (m_buffer.size() == m_maxBufferSize)
//     // {
//     //     m_buffer.pop_front();
//     // }

//     m_buffer.push_back(RosKapDataPacket(packet.s_time, packet));
// }

void IPECallback::onLiveDataAvailable(IPEDataPacket packet) {
    RosKapDataPacket newPacket(packet.s_time, packet);

    // 덱이 비어있거나, 새 패킷이 가장 최신일 경우 뒤에 추가한다.
    if (m_buffer.empty() || newPacket.first >= m_buffer.back().first) {
        m_buffer.push_back(newPacket);
    } else {
        // 새 패킷이 가장 오래된 것보다 이전 데이터일 경우 앞에 삽입한다.
        if (newPacket.first <= m_buffer.front().first) {
            m_buffer.push_front(newPacket);
        } else {
            // 중간에 삽입해야 할 경우, 삽입 위치를 찾는다.
            for (auto it = m_buffer.begin(); it != m_buffer.end(); ++it) {
                // 삽입 위치를 찾으면 반복을 중단한다.
                if (newPacket.first < it->first) {
                    m_buffer.insert(it, newPacket);
                    break;
                }
            }
        }
    }

    // 버퍼 사이즈를 확인하고, 필요하면 가장 오래된 데이터를 제거한다.
    while (m_buffer.size() > m_maxBufferSize) {
        m_buffer.pop_front();
    }
}
