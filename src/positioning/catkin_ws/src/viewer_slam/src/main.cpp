#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <math.h>
#include <stdio.h> 
#include <cmath>
#include <vector>
#include <queue>
#include "parking_map.h"
#include "main.h"
#include "ros/ros.h"
#include "geometry_msgs/PoseStamped.h"
#include "std_msgs/String.h"

#include "socketmanager.h"
#include <thread> 
#include <chrono>

#define SERVER_ADDR "221.140.137.186"
#define SERVER_PORT 51000

using namespace std; 

int server_sock;

typedef struct euler_point { 
    double roll; 
    double pitch; 
    double yaw; 
} ;

typedef struct _MESSAGE_TIMER
{
    int type;       // 1: send server, 2: state change
    char *msg;
    int size;
    ros::Time timer;

}MESSAGE_TIMER;

queue<MESSAGE_TIMER> timers;

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
    // hy.joo (24.10.15)
    double slam_pos_y = msg->pose.position.y;     

    struct euler_point ep = euler_from_quaternion(slam_x, slam_z, -slam_y, -slam_w);
    double pitch = ep.yaw;
    
    set_slam_orientation(pitch);
    set_slam_position(slam_pos_x, (-1)*slam_pos_z) ;
    
    double in_out = determine_indoor(slam_pos_x, (-1)*slam_pos_z, (-1)*slam_pos_y);  // in_out == -1 : indoor & in_out == 1 : outdoor 

    //set_kanavi_msg(in_out, slam_pos_x, (-1)*slam_pos_z); 
    //set_kanavi_msg(in_out, vt_uwb_x, vt_uwb_y); 
    
}

void inoutCallback(const std_msgs::String::ConstPtr& msg)
{
   std::string received_msg = msg->data.c_str();
   // std::cout<<received_msg<< std::endl;

    if (received_msg == "indoor"){
    ai_inout = -1;
    }
    
    else if (received_msg == "ramp"){
    ai_inout = 0; 
    }
    
    else if (received_msg == "outdoor"){
    ai_inout = 1;
    } 
    
}
  




void *spinfor(void *data)
{
    ros::NodeHandle nh;
    ros::Subscriber slam_sub;  
    ros::Subscriber cctv_sub;  
    
    slam_sub = nh.subscribe<geometry_msgs::PoseStamped>("/orb_slam3/camera_pose", 10, slamCallback);

    ros::Publisher viewer_pub = nh.advertise<geometry_msgs::PoseStamped>("/viewer_slam_node/viewer_pose", 3);
    //ros::Publisher kanavi_pub = nh.advertise<geometry_msgs::PoseStamped>("/viewer_slam_node/kanavi_msg", 10); //in_out , x, y ,theta : Move tho sendtoKanavi thread 
    
    // add (24.10.15, hy.joo) -> move to kanavi_sendto 
    //SocketManager* socketManager = SocketManager::getInstance(); // <-- Add this line to initialize the socketManager

    //add (24.10.31. hy.joo)
    ros::Subscriber inout_sub; 
    inout_sub = nh.subscribe<std_msgs::String>("inout_deepl/pred_label", 10, inoutCallback);

    while (nh.ok()) {
        ros::spinOnce();

        //viewer INFO publish 
        geometry_msgs::PoseStamped viewer_pose;
        viewer_pose.header.frame_id = "map";
        viewer_pose.pose.position.x = vt_uwb_x;
        //viewer_pose.pose.position.x = uwb_x;
        viewer_pose.pose.position.y = vt_uwb_y;
        //viewer_pose.pose.position.y = uwb_y;
        viewer_pose.pose.position.z = kanavi_inout;
        viewer_pose.pose.orientation.x = vt_heading;

        viewer_pub.publish(viewer_pose);


        if(timers.size() > 0) {
            MESSAGE_TIMER timer = timers.front();
            ros::Time now = ros::Time::now();
            // std::cout <<timers.size() << "type : " << timer.type << "msg : " << timer.msg << std::endl;
            if(timer.type == 1) {
                if(now > timer.timer) {
                    send_message_to_server(timer.msg, timer.size);
                    timers.pop();
                }
            } else if(timer.type == 2) {
                if(now > timer.timer && strcmp(timer.msg, "MANEUVER") == 0) {
                    set_nego_state(NEGO_MANEUVER);
                    timers.pop();
                }
            }
            
        }
        /*
        //kanavi MSG publish 
        geometry_msgs::PoseStamped kanavi_msg;
        kanavi_msg.header.frame_id = "map";
        kanavi_msg.pose.position.x = kanavi_x;
        kanavi_msg.pose.position.y = kanavi_y;
        kanavi_msg.pose.position.z = kanavi_inout;
        kanavi_msg.pose.orientation.x = kanavi_heading; 

        kanavi_pub.publish(kanavi_msg);
        */

        /*
        std::ostringstream oss;
        std::string inout_ox; 
        if (kanavi_inout == -1){ //indoor 
            inout_ox = 'O';
        }
        else if (kanavi_inout == 1){ //outdoor 
            inout_ox = 'X';
        }
        
        oss << inout_ox <<","<< kanavi_x << "," << kanavi_y << "," << kanavi_heading;
        std::string result = oss.str();
        socketManager->broadcastUDPMessage(result);
        */
        
    }
}
void set_send_message(char *buff, int size, int timeout)
{
    ros::Time time = ros::Time::now();
    time.sec += timeout;
    MESSAGE_TIMER timer;
    timer.type = 1;
    timer.msg = (char *)malloc(sizeof(char) * size);
    memcpy(timer.msg, buff, size);
    timer.size = size;
    timer.timer = time;
    timers.push(timer);
}
void set_change_state(char *buff, int size, int timeout)
{
    ros::Time time = ros::Time::now();
    time.sec += timeout;
    MESSAGE_TIMER timer;
    timer.type = 2;
    timer.msg = (char *)malloc(sizeof(char) * size);
    memcpy(timer.msg, buff, size);
    timer.size = size;
    timer.timer = time;
    timers.push(timer);
}
void send_message_to_server(char *buff, int size)
{
    write(server_sock, buff, size);
}

void *rx_cctv(void *data)
{
    int rx_leng, i;
    double cctv_y, cctv_x;
    char buff[1024];
    server_sock = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_adr;

    ros::NodeHandle nh;
    ros::Publisher cctv_pub = nh.advertise<geometry_msgs::PoseStamped>("/viewer_slam_node/cctv_pose", 10);

    memset(&server_adr, 0, sizeof(server_adr));
    server_adr.sin_family = AF_INET;
    server_adr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
    server_adr.sin_port = htons(SERVER_PORT);

    if(connect(server_sock, (struct sockaddr*)&server_adr, sizeof(server_adr)) == -1) {
        printf("connect() error\n");
    }

    sprintf(buff, "%s", "carnival");
    send_message_to_server(buff, 8);
    // write(server_sock, buff, 30);
    read(server_sock, &buff, sizeof(buff));

    while(nh.ok()) {
        rx_leng = read(server_sock, buff, sizeof(buff) - 1);
        buff[rx_leng] = '\0';
        // std::cout << buff << std::endl;
        string str1(buff);
        // std::cout << str1 << std::endl;
        
        // if(strncmp(buff, "IONIC_GO", 8) == 0) {
        if(str1.find("IONIC_GO") < rx_leng) {
            set_nego_state(NEGO_NEGOTIATION); 
            continue;
        } else if(str1.find("CARNIVAL_LEFT") < rx_leng) {
            set_nego_state(NEGO_ACTION_CARNIVAL);
            continue;
        } else if(str1.find("FINISH_NEGO") < rx_leng) {
            set_nego_state(NEGO_END);
            // set_nego_button_text("주행협상 시작");
            continue;
        }


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
        new_pose.pose.position.x = cctv_y;
        new_pose.pose.position.y = cctv_x;
        new_pose.pose.position.z = 0;

        cctv_pub.publish(new_pose);
        set_cctv(cctv_x, cctv_y);
    }
}
//Add 24.10.23 (send to Kanavi Message Thread)
void *sendto_kanavi(void *data)
{
    // Moved @ (24.10.23, hy.joo)
    SocketManager* socketManager = SocketManager::getInstance(); // <-- Add this line to initialize the socketManager

    while(true) {
 
        //kanavi MSG publish 

        std::ostringstream oss;
        std::string inout_ox; 
        
        if (kanavi_inout == -1){ //indoor 
            inout_ox = 'O';
        }
        else if (kanavi_inout == 1){ //outdoor 
            inout_ox = 'X';
        }
        
        oss << inout_ox <<","<< vt_uwb_x << "," << vt_uwb_y << "," << vt_heading;
        std::string result = oss.str();
        std::cout << "kanavi msg: " << result << std::endl;
        socketManager->broadcastUDPMessage(result);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(200)); 
        
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

    //float vt_uwb_x = -1;
    //float vt_uwb_y = -1; 

    //double kanavi_x = -1; 
    //double kanavi_y = -1; 
    //double kanavi_heading = -1;
    
    double kanavi_inout = -1; 
    double ai_inout = -2;
    
    //double ai_inout_previous = -2;
    //double ai_inout_current = -2;


    if(site.compare("robo") == 0) {
        set_rosparam(ROBO, use_traffic_light, use_ipe, use_lidar, use_cctv, ramp_match);
    } else if(site.compare("global") == 0) {
        set_rosparam(GLOBAL, use_traffic_light, use_ipe, use_lidar, use_cctv, ramp_match);
    }
    
    map_init(argc, argv);
    
    return 0;
}
