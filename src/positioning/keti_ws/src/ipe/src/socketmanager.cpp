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
#include "socketmanager.h"

SocketManager* SocketManager::instance = nullptr;

SocketManager::SocketManager() {
    initializeSocket();
}

SocketManager* SocketManager::getInstance() {
    if (!instance) {
        instance = new SocketManager();
    }
    return instance;
}

int SocketManager::getSocket() const {
    return sock;
}

sockaddr_in SocketManager::getServerAddress() const {
    return serverAddress;
}

void SocketManager::addClient(const std::string& ip, unsigned short port) {
    sockaddr_in clientAddress;
    clientAddress.sin_family = AF_INET;
    clientAddress.sin_port = htons(port);
    if (inet_pton(AF_INET, ip.c_str(), &clientAddress.sin_addr) <= 0) {
        std::cerr << "Invalid IP address provided!" << std::endl;
        return;
    }
    clientMap[clientAddress] = true;
}

bool SocketManager::isClientExist(const std::string& ip, unsigned short port) {
    sockaddr_in clientAddress;
    clientAddress.sin_family = AF_INET;
    clientAddress.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &clientAddress.sin_addr);
    return clientMap.find(clientAddress) != clientMap.end();
}

bool SocketManager::sendUDPMessage(const std::string& message, const sockaddr_in& clientAddress) {

    ssize_t sent = sendto(sock, message.c_str(), message.size(), 0, 
                          (struct sockaddr*)&clientAddress, sizeof(clientAddress));
    if (sent == -1) {
        perror("sendto() error");
        return false;
    }
    return true;
}

bool SocketManager::broadcastUDPMessage(const std::string& message) {
    struct sockaddr_in broadcastAddr;
    memset(&broadcastAddr, 0, sizeof(broadcastAddr));
    broadcastAddr.sin_family = AF_INET;
    broadcastAddr.sin_port = htons(5000); // 브로드캐스트할 포트 지정
    // broadcastAddr.sin_addr.s_addr = htonl(INADDR_BROADCAST); // 브로드캐스트 주소 설정
    broadcastAddr.sin_addr.s_addr = inet_addr("192.168.3.242");


    int sendResult = sendto(sock, message.c_str(), message.size(), 0, 
                            (struct sockaddr *)&broadcastAddr, sizeof(broadcastAddr));
    if (sendResult == -1) {
        perror("Broadcast message failed");
        return false;
    }
    return true;
}

void SocketManager::initializeSocket() {
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        perror("Can't create socket");
        exit(1);
    }

    int opt = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt)) == -1) {
        perror("Error setting socket option SO_BROADCAST");
        close(sock);
        exit(1);
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(54000);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, (sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        perror("Can't bind to IP/port");
        exit(1);
    }
}
