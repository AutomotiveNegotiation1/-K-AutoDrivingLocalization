#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "parking_map.h"
#include "main.h"
#include "ros/ros.h"
#include "geometry_msgs/PoseStamped.h"
#include "std_msgs/String.h"

#define SERVER_ADDR "221.140.137.186"
#define SERVER_PORT 51000

void uwbCallback(const geometry_msgs::PoseStamped::ConstPtr& msg)
{
    set_uwb(msg->pose.position.x, msg->pose.position.y, msg->pose.orientation.x);

    return;
}

void lidarCallback(const geometry_msgs::PoseStamped::ConstPtr& msg)
{
    set_lidar(msg->pose.position.x, msg->pose.position.y, msg->pose.orientation.x);

    return;
}

void *spinfor(void *data)
{
    ros::NodeHandle nh;
    bool use_lidar;
    bool use_ipe;
    ros::Subscriber ipe_sub;
    ros::Subscriber lidar_sub;

    ros::param::get("/view_node/use_lidar", use_lidar);
    ros::param::get("/view_node/use_ipe", use_ipe);
    if(use_ipe) {
        ipe_sub = nh.subscribe<geometry_msgs::PoseStamped>("/ipe/slam_added_pose", 10, uwbCallback);
    }
    if(use_lidar) {
        lidar_sub = nh.subscribe<geometry_msgs::PoseStamped>("/kdlidar_ros_pcl/pose", 10, lidarCallback);
    } 
    

    while (nh.ok()) {
        ros::spinOnce();
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



    if(site.compare("robo") == 0) {
        set_rosparam(ROBO, use_traffic_light, use_ipe, use_lidar, use_cctv, ramp_match);
    } else if(site.compare("global") == 0) {
        set_rosparam(GLOBAL, use_traffic_light, use_ipe, use_lidar, use_cctv, ramp_match);
    }
    
    map_init(argc, argv);
    
    return 0;
}