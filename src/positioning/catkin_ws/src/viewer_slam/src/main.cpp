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

    return ret; 
}



void slamCallback(const geometry_msgs::PoseStamped::ConstPtr& msg)
{
    double slam_x = msg->pose.orientation.x;
    double slam_y = msg->pose.orientation.y;
    double slam_z = msg->pose.orientation.z;
    double slam_w = msg->pose.orientation.w;

    double slam_pos_x = msg->pose.position.x; 
    double slam_pos_z = msg->pose.position.z; 
    

    struct euler_point ep = euler_from_quaternion(slam_x, slam_z, -slam_y, -slam_w);
    double pitch = ep.yaw;
    
    set_slam_orientation(pitch);
    set_slam_position(slam_pos_x, (-1)*slam_pos_z) ;
    //printf("r: %f p:%f y:%f\n", ep.roll, pitch, ep.yaw);
    return;
}


void *spinfor(void *data)
{
    ros::NodeHandle nh;
    ros::Subscriber slam_sub;  
    
    slam_sub = nh.subscribe<geometry_msgs::PoseStamped>("/orb_slam3/camera_pose", 10, slamCallback);

    ros::Publisher viewer_pub = nh.advertise<geometry_msgs::PoseStamped>("/viewer_slam_node/viewer_pose", 10);

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
    ros::Publisher cctv_pub = nh.advertise<geometry_msgs::PoseStamped>("/viewer_slam_node/cctv_pose", 10);

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
    ros::init(argc, argv, "viewer_slam_node");

    std::string site;
    bool use_traffic_light;
    bool use_ipe;
    bool use_lidar;
    bool use_cctv;
    bool ramp_match;
    
    ros::param::get("/viewer_slam_node/site", site);
    ros::param::get("/viewer_slam_node/use_traffic_light", use_traffic_light);
    ros::param::get("/viewer_slam_node/use_ipe", use_ipe);
    ros::param::get("/viewer_slam_node/use_lidar", use_lidar);
    ros::param::get("/viewer_slam_node/use_cctv", use_cctv);
    ros::param::get("/viewer_slam_node/ramp_match", ramp_match);

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