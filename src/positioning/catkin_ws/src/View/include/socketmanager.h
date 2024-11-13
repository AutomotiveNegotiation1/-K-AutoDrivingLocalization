#ifndef SOCKETMANAGER_H
#define SOCKETMANAGER_H

#include <iostream>
#include <ros/ros.h>
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
    std::string kanavi_ip;
    int kanavi_port;
    
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
