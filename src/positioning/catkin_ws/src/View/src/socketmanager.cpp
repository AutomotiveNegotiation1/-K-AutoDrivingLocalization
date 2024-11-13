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

    ros::param::get("/viewer_slam_node/kanavi_ip", kanavi_ip); 
    ros::param::get("/viewer_slam_node/kanavi_port", kanavi_port); 

    memset(&broadcastAddr, 0, sizeof(broadcastAddr));
    broadcastAddr.sin_family = AF_INET;
    broadcastAddr.sin_port = htons(kanavi_port);
    broadcastAddr.sin_addr.s_addr = inet_addr(kanavi_ip.c_str());


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