// socketmanager.h
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
#ifndef SOCKETMANAGER_H
#define SOCKETMANAGER_H

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <map>
#include <cstdlib>

// sockaddr_in의 비교 함수
struct sockaddr_in_compare {
    bool operator()(const sockaddr_in& a, const sockaddr_in& b) const {
        if (a.sin_addr.s_addr != b.sin_addr.s_addr) {
            return a.sin_addr.s_addr < b.sin_addr.s_addr;
        }
        return a.sin_port < b.sin_port;
    }
};

class SocketManager {
private:
    int sock;
    sockaddr_in serverAddress;
    std::map<sockaddr_in, bool, sockaddr_in_compare> clientMap; // 비교 함수 사용
    static SocketManager* instance;

    SocketManager();

public:
    static SocketManager* getInstance();
    int getSocket() const;
    sockaddr_in getServerAddress() const;
    void addClient(const std::string& ip, unsigned short port);
    bool isClientExist(const std::string& ip, unsigned short port);
    void initializeSocket();
    bool sendUDPMessage(const std::string& message, const sockaddr_in& clientAddress);
    bool broadcastUDPMessage(const std::string& message);
    // ... 기타 socket 관련 메소드 ...
};

#endif // SOCKETMANAGER_H
