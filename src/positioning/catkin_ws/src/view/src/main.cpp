#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <math.h>
#include <stdio.h> 
#include <cmath>
#include "parking_map.h"
#include "main.h"
#include "ros/ros.h"
#include "geometry_msgs/PoseStamped.h"
#include "std_msgs/String.h"

#define SERVER_ADDR "221.140.137.186"
#define SERVER_PORT 51000

using namespace std; 




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

void uwbCallback(const geometry_msgs::PoseStamped::ConstPtr& msg)
{
  
    set_uwb(msg->pose.position.x, msg->pose.position.y, msg->pose.orientation.x);
    //std::cout << "vt_uwb_x: " << vt_uwb_x << std::endl; 
    //std::cout << "vt_uwb_y: " << vt_uwb_y << std::endl; 

    return;
}

void lidarCallback(const geometry_msgs::PoseStamped::ConstPtr& msg)
{    double roll, pitch, yaw, pos_x, pos_y;
    euler_from_quaternion2(msg->pose.orientation.x, msg->pose.orientation.y, msg->pose.orientation.z, -1 * msg->pose.orientation.w, &roll, &pitch, &yaw);

    // tf2::Quaternion new_ori(msg->pose.orientation.x, msg->pose.orientation.y, msg->pose.orientation.z, msg->pose.orientation.w);
    // tf2::Matrix3x3 m(new_ori);
    double offset_angle = 0.03;//-0.018
    double offset_x = (78.815) * cos(offset_angle) - (-0.12913) * sin(offset_angle);
    double offset_y = (78.815) * sin(offset_angle) + (-0.12913) * cos(offset_angle);

    double new_x = (msg->pose.position.y) * cos(offset_angle) - (msg->pose.position.x) * sin(offset_angle);
    double new_y = (msg->pose.position.y) * sin(offset_angle) + (msg->pose.position.x) * cos(offset_angle);
    
    set_lidar(new_y + offset_y, new_x + offset_x, yaw);


    return;
}


typedef struct euler_point { 
    double roll; 
    double pitch; 
    double yaw; 
} ;

struct euler_point euler_from_quaternion(double x, double y, double z, double w) {
    double sinr_cosp, cosr_cosp, sinp, siny_cosp, cosy_cosp;

    sinr_cosp = 2 * (w * x + y * z);
    cosr_cosp = 1 - 2 * (x * x + y * y);
    double roll = atan2(sinr_cosp, cosr_cosp);

    sinp = 2 * (w * y - z * x);
    double pitch = asin(sinp);
    
    siny_cosp = 2 * (w * z + x * y);
    cosy_cosp = 1 - 2 * (y * y + z * z);
    double yaw = atan2(siny_cosp, cosy_cosp);
    struct euler_point ret; 
    ret.roll = roll;
    ret.pitch = pitch; 
    ret.yaw = yaw;

    //printf("r: %f p:%f y:%f\n", roll, pitch, yaw );

    return ret; 
}


struct euler_point euler_from_quaternion_modified(double x, double y, double z, double w) {
    double sinr_cosp, cosr_cosp, sinp, cosp, siny_cosp, cosy_cosp;

    sinr_cosp = 2 * (w * x + y * z);
    cosr_cosp = 1 - 2 * (x * x + y * y);
    double roll = atan2(sinr_cosp, cosr_cosp);

    sinp = sqrt(1 + 2 * ( w * y - x * z));
    cosp = sqrt(1 - 2 * ( w * y - x * z)); 
    double pitch = 2 * atan2(sinp, cosp) - 3.141592 / 2;
    
    // sinp = 2 * (w * y - z * x);
    // double pitch = asin(sinp);
    
    siny_cosp = 2 * (w * z + x * y);
    cosy_cosp = 1 - 2 * (y * y + z * z);

    double yaw = atan2(siny_cosp, cosy_cosp);
    struct euler_point ret; 
    ret.roll = roll;
    ret.pitch = pitch; 
    ret.yaw = yaw;

    //printf("r: %f p:%f y:%f\n", roll, pitch, yaw );

    return ret; 
}


void slamCallback(const geometry_msgs::PoseStamped::ConstPtr& msg)
{
    double slam_x = msg->pose.orientation.x;
    double slam_y = msg->pose.orientation.y;
    double slam_z = msg->pose.orientation.z;
    double slam_w = msg->pose.orientation.w;
    

    struct euler_point ep = euler_from_quaternion(slam_x, slam_z, -slam_y, -slam_w);
    double pitch = ep.yaw;
    
    set_slam_orientation(pitch);
    //printf("r: %f p:%f y:%f\n", ep.roll, pitch, ep.yaw);
    return;
}


void *spinfor(void *data)
{
    ros::NodeHandle nh;
    bool use_lidar;
    bool use_ipe;
    ros::Subscriber ipe_sub;
    ros::Subscriber lidar_sub;
    ros::Subscriber slam_sub; 

    ros::param::get("/view_node/use_lidar", use_lidar);
    ros::param::get("/view_node/use_ipe", use_ipe);
    if(use_ipe) {
        ipe_sub = nh.subscribe<geometry_msgs::PoseStamped>("/ipe/slam_added_pose", 10, uwbCallback);
        slam_sub = nh.subscribe<geometry_msgs::PoseStamped>("/orb_slam3/camera_pose", 10, slamCallback);

    }
    
    if(use_lidar) {
        lidar_sub = nh.subscribe<geometry_msgs::PoseStamped>("/kdlidar_ros_pcl/pose", 10, lidarCallback);
    } 

    
    ros::Publisher viewer_pub = nh.advertise<geometry_msgs::PoseStamped>("/view_node/viewer_pose", 10);

    while (nh.ok()) {
        ros::spinOnce();

        geometry_msgs::PoseStamped viewer_pose;
        viewer_pose.header.frame_id = "map";
        viewer_pose.pose.position.x = vt_uwb_x;
        viewer_pose.pose.position.y = vt_uwb_y;
        viewer_pose.pose.position.z = 0;

        viewer_pub.publish(viewer_pose);
    }
}

void *rx_cctv(void *data)
{
    int rx_leng, i;
    double cctv_y, cctv_x;
    char buff[1024];
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_adr;

    ros::NodeHandle nh;
    ros::Publisher cctv_pub = nh.advertise<geometry_msgs::PoseStamped>("/view_node/cctv_pose", 10);

    memset(&server_adr, 0, sizeof(server_adr));
    server_adr.sin_family = AF_INET;
    server_adr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
    server_adr.sin_port = htons(SERVER_PORT);

    if(connect(sock, (struct sockaddr*)&server_adr, sizeof(server_adr)) == -1) {
        printf("connect() error\n");
    }

    sprintf(buff, "%s", "subscribe");
    write(sock, buff, 30);
    read(sock, &buff, sizeof(buff));

    while(nh.ok()) {
        rx_leng = read(sock, buff, sizeof(buff) - 1);
        buff[rx_leng] = '\0';

        for(i = 0; i < rx_leng - 3; i++) {
            if(buff[i] == 0x05 && buff[i + 1] == 0x05 && buff[i + 2] == 0x05 && buff[i + 3] == 0x05) {
                buff[i] = '\0';
            }
        }
        char *p;
        int phase = 1;
        char *eptr;
        p = strtok(buff, ",");
        while(p)
        {
            if(phase == 1) {
                cctv_y = strtod(p, &eptr);
                phase = 2;
            }
            else if(phase == 2) {
                cctv_x = strtod(p, &eptr);
                phase = 3;
            }
            else if(phase == 3) {
                strtod(p, &eptr);
                phase = 1;
            }
            p = strtok(NULL, ",");
        }

        geometry_msgs::PoseStamped new_pose;
        new_pose.header.frame_id = "map";
        new_pose.pose.position.x = cctv_x;
        new_pose.pose.position.y = cctv_y;
        new_pose.pose.position.z = 0;

        cctv_pub.publish(new_pose);
        set_cctv(cctv_x, cctv_y);
    }
}


int main(int argc, char **argv)
{
    ros::init(argc, argv, "view_node");

    std::string site;
    bool use_traffic_light;
    bool use_ipe;
    bool use_lidar;
    bool use_cctv;
    bool ramp_match;
    
    ros::param::get("/view_node/site", site);
    ros::param::get("/view_node/use_traffic_light", use_traffic_light);
    ros::param::get("/view_node/use_ipe", use_ipe);
    ros::param::get("/view_node/use_lidar", use_lidar);
    ros::param::get("/view_node/use_cctv", use_cctv);
    ros::param::get("/view_node/ramp_match", ramp_match);

    float vt_uwb_x = -1;
    float vt_uwb_y = -1; 

    if(site.compare("robo") == 0) {
        set_rosparam(ROBO, use_traffic_light, use_ipe, use_lidar, use_cctv, ramp_match);
    } else if(site.compare("global") == 0) {
        set_rosparam(GLOBAL, use_traffic_light, use_ipe, use_lidar, use_cctv, ramp_match);
    }
    
    map_init(argc, argv);
    
    return 0;
}