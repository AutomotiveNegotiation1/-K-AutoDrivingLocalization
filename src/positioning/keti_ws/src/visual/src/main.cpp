#include <QApplication>
#include <QTimer>
#include <ros/ros.h>
#include "mainwindow.h"
#include "mainpositioning.h"

int main(int argc, char** argv) {
    ros::init(argc, argv, "rosbag_republisher_node");
    ros::NodeHandle nh;

    QApplication app(argc, argv);
    MainWindow KETIApp;
    KETIApp.show();

    // ROS 처리를 위한 스레드 시작
    MainPositioning rosThread(nh);

    bool connected = QObject::connect(&rosThread, &MainPositioning::newPositionData,
                                  &KETIApp, &MainWindow::onNewPositionData);
    if (connected) {
        ROS_INFO("Signal and slot connected successfully.");
    } else {
        ROS_WARN("Signal and slot connection failed.");
    }
    
    rosThread.start();

    // Qt 이벤트 루프 진입
    return app.exec();
}
