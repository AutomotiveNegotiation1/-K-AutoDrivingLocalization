#include <QApplication>
#include <QTimer>
#include <ros/ros.h>
#include <rosbag/bag.h>
#include <rosbag/view.h>
#include "mainwindow.h"
#include <visual/Anchor.h>

void processRosbagChunk(rosbag::View &view, MainWindow &KETIApp, std::map<std::string, ros::Publisher> &publishers) {
    static auto viewIterator = view.begin();

    if (viewIterator != view.end()) {
        const rosbag::MessageInstance &message = *viewIterator;

        if (publishers.find(message.getTopic()) != publishers.end()) {
            visual::Anchor::ConstPtr uwb_data = message.instantiate<visual::Anchor>();
            if (uwb_data) {
                publishers[message.getTopic()].publish(uwb_data);
                KETIApp.updateGraph();
                KETIApp.spinFor();
                ros::spinOnce();
            }
        }

        viewIterator++;  // Move to the next message
    }
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "rosbag_republisher_node");
    ros::NodeHandle nh;

    QApplication app(argc, argv);
    MainWindow KETIApp;
    KETIApp.show();

    try {
        KETIApp.registerSubcribers(nh);
    } catch (const std::exception& e) {
        ROS_ERROR("%s", e.what());
        return 1;  
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
        publishers[topic] = nh.advertise<visual::Anchor>(topic, 10);
    }

    rosbag::View view(bag);
    
    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&]() {
        processRosbagChunk(view, KETIApp, publishers);
    });
    timer.start(1);  // Process one rosbag message every 100 milliseconds.

    return app.exec();  // Enter the Qt event loop
}
