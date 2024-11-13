
#include "slamsubscriber_ipe_v4_2.h"
#include "PositioningSystem_V5_1.h" // Changed 2024.06.17 (joo.hy)
//#include "PositioningSystem_V4_2.h" // Changed 2024.06.17 (joo.hy)
#include <nav_msgs/Path.h>

SlamSubscriber::SlamSubscriber(ros::NodeHandle& _node, IPECallback* _ipeCallback)
    : o_ipeCallback(_ipeCallback)
{
    setupSubscriber(_node);
    pub_slam = _node.advertise<geometry_msgs::PoseStamped>("/ipe/slam_added_pose", 10);
    pub_ipe = _node.advertise<nav_msgs::Path>("/ipe/ipe_track", 10);
    
    socketManager = SocketManager::getInstance(); // <-- Add this line to initialize the socketManager

}

SlamSubscriber::~SlamSubscriber() {
}

void SlamSubscriber::operator()(IPEDataPacket &packet, double timestamp, SensorData* data) {
    
    processPacketData(packet, timestamp, data);
}

void SlamSubscriber::registerCallback(const std::function<void(int)>& _callback) {
    callbacks.push_back(_callback);
}

std::string SlamSubscriber::getPacketFrameID() {
    return packetFrameID = m_frameId;
}

void SlamSubscriber::setupSubscriber(ros::NodeHandle& node) {
    
    topic_name_stream << "/orb_slam3/camera_pose";
    std::string topic_name = topic_name_stream.str();

    ROS_INFO("topic_name-->%s", topic_name.c_str());
    sub_slam = node.subscribe<geometry_msgs::PoseStamped>(topic_name, 10, &SlamSubscriber::_callback, this);
}


void SlamSubscriber::_callback(const geometry_msgs::PoseStamped::ConstPtr& msg) {

    m_ipeDataPacket = IPEDataPacket(msg);
    //std::cout << "SLAM test " << std::endl;
    m_ipeDataPacket.frame_id = "slam";
    if (o_ipeCallback) {
        o_ipeCallback->onLiveDataAvailable(m_ipeDataPacket);
    } else {
        ROS_WARN("m_kapCallback is a nullptr!");
    }
}

void SlamSubscriber::sendUDPMessage(double center_x, double center_y, double heading, std::string in_out) {
    std::ostringstream oss;

    oss << in_out<<","<<center_x << "," << center_y << "," << heading;
    std::string result = oss.str();
    
    socketManager->broadcastUDPMessage(result);
}

std::string determine_Indoor(double pose_x, double pose_y, double pose_z) {
    
    if (pose_y > 82){
        if (pose_z> 0.8) {
            if(pose_x>19 && pose_y >93){
                //std::cout << "RAMP!" << pose_x << " " <<pose_y << std::endl;  
                std::cout << "OUTDOOR!" << std::endl; 
                return "X";
            }
            else{
                //std::cout << "OUTDOOR! "<< pose_x <<" "<< pose_y << std::endl; 
                std::cout << "OUTDOOR! "<< std::endl; 
                return "X";
            }
        
        }
        else{
            //std::cout <<"Indoor!" << pose_x <<" "<< pose_y << std::endl;
            std::cout <<"INDOOR!" << std::endl;
            return "O";
        }
    }  
    else{
        if (pose_z > 0.8) { 
            std::cout << "OUTDOOR!" << std::endl; 
            return "X"; 
        }
        else{  
            std::cout <<"INDOOR!" <<std::endl;
            return "O"; 
        }
    } 
}


void SlamSubscriber::processPacketData(IPEDataPacket &packet, double timestamp, SensorData* newUWBData) 
{
    // 2024_06_17 (joo.hy)
    //#include "PositioningSystem_V4_2.h" // Changed 2024.06.17 (joo.hy)
    //std:: cout << "Process Slam Packet Data 1" << std:: endl; 
    
    if (packet.slam_pos_x.empty()){
        std:: cout << "packet is emtpy" << std:: endl; 
        return;
    }

    double slam_pos_x = packet.slam_pos_x.back(); 
    double slam_pos_y = packet.slam_pos_y.back();
    double slam_pos_z = packet.slam_pos_z.back();

    double slam_ort_x = packet.slam_ort_x.back();
    double slam_ort_y = packet.slam_ort_y.back();
    double slam_ort_z = packet.slam_ort_z.back();
    double slam_ort_w = packet.slam_ort_w.back();

    //std:: cout << "Process Slam Packet Data 2" << std:: endl;  
    // double PositionVector_data[8] = {};
    std::vector<double> PositionVector_data;
    int PositionVector_size[2];
                                                                                                                                                                                             
    // double PositionOut[7]; // PositioningSystem_V2_1 version (maintained before 2024.03.28, jang.sh)
    double PositionOut[10]; // PositioningSystem_V2_2 version (after 2024.03.28, jang.sh)

    PositionVector_data.push_back(packet.s_time); 
    PositionVector_data.push_back(7.0);


    if (!packet.slam_pos_x.empty()) {
        PositionVector_data.push_back(packet.slam_pos_x.back());
    } else {
        PositionVector_data.push_back(0);
    }

    if (!packet.slam_pos_y.empty()) {
        PositionVector_data.push_back(packet.slam_pos_y.back());
    } else {
        PositionVector_data.push_back(0);
    }

    if (!packet.slam_pos_z.empty()) {
        PositionVector_data.push_back(packet.slam_pos_z.back());
    } else {
        PositionVector_data.push_back(0);
    }

    
    if (!packet.slam_ort_w.empty()) {
        PositionVector_data.push_back(packet.slam_ort_w.back());
    } else {
        PositionVector_data.push_back(0);
    }
    

    if (!packet.slam_ort_x.empty()) {
        PositionVector_data.push_back(packet.slam_ort_x.back());
    } else {
        PositionVector_data.push_back(0);
    }


    if (!packet.slam_ort_y.empty()) {
        PositionVector_data.push_back(packet.slam_ort_y.back());
    } else {
        PositionVector_data.push_back(0);
    }


    if (!packet.slam_ort_z.empty()) {
        PositionVector_data.push_back(packet.slam_ort_z.back());
    } else {
        PositionVector_data.push_back(0);
    }
    /*
    if (!packet.slam_ort_w.empty()) {
        PositionVector_data.push_back(packet.slam_ort_w.back());
    } else {
        PositionVector_data.push_back(0);
    }
    */
    

    //PositioningSystem_V4_2(PositionVector_data.data(), PositionVector_size, PositionOut); 
    
    //PositioningSystem_V5_1(PositionVector_data.data(), PositionVector_size, PositionOut); 

    
    PositioningSystem_V5_1(PositionVector_data.data(), PositionVector_size, PositionOut); //  Changed 2024.06.17 (joo.hy)
    //PositioningSystem_V5_1(PositionVector_data.data(), PositionVector_size, MapParam, PositionOut);  // changed by joo(24.08.21, pkg_5.1.16 ~) 

    geometry_msgs::PoseStamped ps;
  
    ps.header.frame_id = "map";
    ps.pose.position.x = PositionOut[0];
    ps.pose.position.y = PositionOut[1];
    //ps.pose.position.z = 0;
    ps.pose.position.z = (-1)*PositionVector_data[3] ;  //packet.slam_pos_y.back()
    std::string in_out = determine_Indoor(ps.pose.position.x, ps.pose.position.y, ps.pose.position.z) ; 

    // orginal theta 
    ps.pose.orientation.x = PositionOut[3];
    pub_slam.publish(ps);

    //sendUDPMessage(PositionOut[0], PositionOut[1], PositionOut[3], in_out); //  result (joo.hy)

    nav_msgs::Path ipe_pth; 
    ipe_pth.header.frame_id = "map";
    ipe_pth.poses.push_back(ps);
    pub_ipe.publish(ipe_pth);
    
}




    