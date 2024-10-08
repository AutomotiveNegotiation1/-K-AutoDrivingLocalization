#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <math.h>
#include <stdio.h> 
#include <cmath>
#include <vector>
#include "viewer_lidar.h"
#include "viewer_lidar_main.h"
#include "ros/ros.h"
#include "geometry_msgs/PoseStamped.h"
#include "std_msgs/String.h"

#define SERVER_ADDR "221.140.137.186"
#define SERVER_PORT 51000

using namespace std; 

std::vector<std::vector<double>> rt;
void euler_from_quaternion2(double x, double y, double z, double w, double *roll, double *pitch, double *yaw) {
    double sinr_cosp, cosr_cosp, sinp, siny_cosp, cosy_cosp;

    sinr_cosp = 2 * (w * x + y * z);
    cosr_cosp = 1 - 2 * (x * x + y * y);
    *roll = atan2(sinr_cosp, cosr_cosp);

    sinp = 2 * (w * y - z * x);
    *pitch = asin(sinp);
    
    siny_cosp = 2 * (w * z + x * y);
    cosy_cosp = 1 - 2 * (y * y + z * z);
    *yaw = atan2(siny_cosp, cosy_cosp);
}


void lidarCallback(const geometry_msgs::PoseStamped::ConstPtr& msg)
{    
    double roll, pitch, yaw, pos_x, pos_y;
    euler_from_quaternion2(msg->pose.orientation.x, msg->pose.orientation.y, msg->pose.orientation.z, -1 * msg->pose.orientation.w, &roll, &pitch, &yaw);

    double new_x = msg->pose.position.x;
    double new_y = msg->pose.position.y;
    double in_x, in_y;
    in_x = rt[0][0] * new_x + rt[0][1] * new_y + rt[0][2];
    in_y = rt[1][0] * new_x + rt[1][1] * new_y + rt[1][2];
    
    set_lidar(in_x, in_y, yaw);
    
    return;
}

void *spinfor(void *data)
{
    ros::NodeHandle nh;
    ros::Subscriber lidar_sub;

    lidar_sub = nh.subscribe<geometry_msgs::PoseStamped>("/kdlidar_ros_pcl/pose", 10, lidarCallback);
    
    while (nh.ok()) {
        ros::spinOnce();
    }
}



int main(int argc, char **argv)
{
    ros::init(argc, argv, "viewer_lidar_node");

    std::string site;
    bool use_traffic_light;
    bool use_ipe;
    bool use_lidar;
    bool use_cctv;
    bool ramp_match;
    std::vector<double> rt1;
    std::vector<double> rt2;

    ros::param::get("/viewer_lidar_node/rt", rt1);
    ros::param::get("/viewer_lidar_node/rt2", rt2);

    rt.push_back(rt1);
    rt.push_back(rt2);

    set_rosparam(ROBO, false, false, true, false, false);
    
    
    map_init(argc, argv);
    
    return 0;
}