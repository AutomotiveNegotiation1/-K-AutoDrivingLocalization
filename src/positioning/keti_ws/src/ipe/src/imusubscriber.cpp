/******************************************************************************
*
* Copyright (C) 2023 - 2028 KETI, All rights reserved.
*                           (Korea Electronics Technology Institute)
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running for Korean Government Project, or
* (b) that interact with KETI project/platform.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* KETI BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the KETI shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from KETI.
*
******************************************************************************/

#include "imusubscriber.h"

double b_acc_o[3] = {};
double b_gyro[3] = {};
double mode = 2;
double kf_psi = 0;
double gyro_psi = 0;
double cent_pos_est[3] = {};
double cent_vel_est[3] = {};
double state_o;
double acc_b_phi;
double acc_b_theta;
double signalIMU = 0;
double imuNum = 0;

double lidar_x, lidar_y, lidar_z;
double lidar_q[4];

double PositionOut[10];

double start_x = 32.8000087333;
double start_y = -4.3550085624;
double end_x = 36.0328132588;
double end_y = -4.7361490070;
double start_x2 = 0.5715455412;
double start_y2 = -0.4677960276;
double end_x2 = 3.9188332557;
double end_y2 = 0.0377253293;
double diff_x = end_x - start_x;
double diff_y = end_y - start_y;
double diff_x2 = end_x2 - start_x2;
double diff_y2 = end_y2 - start_y2;
// (diff_x + diff_yi) / (diff_x2 + diff_y2i)
double result_x = ((diff_x * diff_x2) + (diff_y * diff_y2)) / (pow(diff_x2, 2) + pow(diff_y2, 2));
double result_y = ((diff_y * diff_x2) - (diff_x * diff_y2)) / (pow(diff_x2, 2) + pow(diff_y2, 2));
double abs_result = sqrt(pow(result_x, 2) + pow(result_y, 2));
double norm_result_x = result_x / abs(result_x);
double norm_result_y = result_y / abs(result_y);

static void argInit_1x3_real_T(double result[3]) {
    for (int idx1{0}; idx1 < 3; idx1++) {
        result[idx1] = 0.0;
    }
}

static void euler_from_quaternion(double x, double y, double z, double w, double *roll, double *pitch, double *yaw) {
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

static void euler_to_quaternion(double *x, double *y, double *z, double *w, double _roll, double _pitch, double _yaw) {
    double cy,sy,cp,sp,cr,sr;

    cy = cos(_yaw * 0.5);
    sy = sin(_yaw * 0.5);
    cp = cos(_pitch * 0.5);
    sp = sin(_pitch * 0.5);
    cr = cos(_roll * 0.5);
    sr = sin(_roll * 0.5);
    *w = -1 * (cy * cp * cr + sy * sp * sr);
    *x = cy * cp * sr - sy * sp * cr;
    *y = sy * cp * sr + cy * sp * cr;
    *z = sy * cp * cr - cy * sp * sr;
}

static void quatmultiply(double *q1, double *q2, double *result) {
    result[0] = q1[0] * q2[0] - q1[1] * q2[1] - q1[2] * q2[2] - q1[3] * q2[3];
    result[1] = q1[0] * q2[1] + q2[0] * q1[1] + q1[2] * q2[3] - q1[3] * q2[2];
    result[2] = q1[0] * q2[2] + q2[0] * q1[2] + q1[3] * q2[1] - q1[1] * q2[3];
    result[3] = q1[0] * q2[3] + q2[0] * q1[3] + q1[1] * q2[2] - q1[2] * q2[1];
}

static void quatinv(double *q)
{
    double norm = q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3];
    double inv_norm = 1.0 / norm;

    q[0] = q[0] * inv_norm;
    q[1] = -1 * q[1] * inv_norm;
    q[2] = -1 * q[2] * inv_norm;
    q[3] = -1 * q[3] * inv_norm;
}


static void calc_rotate(double *_x, double *_y)
{
    double __x = *_x - start_x;
    double __y = *_y - start_y;
    double rotated_x = __x / norm_result_x;
    double rotated_y = __y / norm_result_y;
    *_x = rotated_x + start_x2;
    *_y = rotated_y + start_y2;
}
static void rotate_pose(double *a, double *b, double angle)
{
    double v1 = *a;
    double v2 = *b;
    *a = v1 * cos(angle) - v2 * sin(angle);
    *b = v1 * sin(angle) + v2 * cos(angle);
}


void ImuSubscriber::rviz_match()
{
    double input_x = PositionOut[0];
    double input_y = -1 * PositionOut[1];

    calc_rotate(&input_x, &input_y);
    geometry_msgs::PoseStamped ps;

    euler_to_quaternion(&ps.pose.orientation.x, &ps.pose.orientation.y, &ps.pose.orientation.z, &ps.pose.orientation.w, 0, 0 ,PositionOut[3] - 1.57);
    ps.header.frame_id = "map";
    ps.pose.position.x = input_x;
    ps.pose.position.y = input_y;
    ps.pose.position.z = 0;

    pub.publish(ps);
    
}

ImuSubscriber::ImuSubscriber(ros::NodeHandle& _node, IPECallback* _ipeCallback)
    : m_ipeCallback(_ipeCallback){
    setupSubscriber(_node);
    pub = _node.advertise<geometry_msgs::PoseStamped>("/ipe/pose", 10);
    
    socketManager = SocketManager::getInstance(); // <-- Add this line to initialize the socketManager
}


ImuSubscriber::~ImuSubscriber() {}

void ImuSubscriber::operator()(IPEDataPacket &packet, double timestamp, SensorData* data) {
    processPacketData(packet, timestamp, data);
}

void ImuSubscriber::registerCallback(const std::function<void(int)>& callback) {
    callbacks.push_back(callback);
}

void ImuSubscriber::sendEvent(int data) {
    for (const auto& callback : callbacks) {
        callback(data);
    }
}

std::string ImuSubscriber::getPacketFrameID() {
    return frame_id;
}

void ImuSubscriber::setupSubscriber(ros::NodeHandle& node) {
    std::cout << "IMU test setup starting..." << std::endl;
    topic_name_stream << "/zed_f9r/imu";
    std::string topic_name = topic_name_stream.str();
    ROS_INFO("topic_name-->%s", topic_name.c_str());

    sub = node.subscribe<sensor_msgs::Imu>(topic_name, 10, &ImuSubscriber::_callback, this);
    subFusion = node.subscribe<geometry_msgs::PoseStamped>("/kdlidar_ros_pcl/pose", 1, &ImuSubscriber::_callback_lidarpose, this);

}

void ImuSubscriber::_callback_lidarpose(const geometry_msgs::PoseStamped::ConstPtr& msg) {
    
    lidar_q[0] = msg->pose.orientation.w;
    lidar_q[1] = msg->pose.orientation.x;
    lidar_q[2] = msg->pose.orientation.y;
    lidar_q[3] = msg->pose.orientation.z;
    // euler_from_quaternion(msg->pose.orientation.x, msg->pose.orientation.y, msg->pose.orientation.z, msg->pose.orientation.w, &lidar_roll, &lidar_pitch, &lidar_yaw);
    lidar_x = msg->pose.position.x;
    lidar_y = msg->pose.position.y;
    lidar_z = msg->pose.position.z;    
}

void ImuSubscriber::_callback(const sensor_msgs::Imu::ConstPtr& msg) {
    m_ipeDataPacket = IPEDataPacket(msg);
    m_ipeDataPacket.frame_id = "imu";
    if (m_ipeCallback) {
        m_ipeCallback->onLiveDataAvailable(m_ipeDataPacket);
    } else {
        ROS_WARN("m_kapCallback is a nullptr!");
    }
}

void ImuSubscriber::sendUDPMessage(double center_x, double center_y, double heading) {
    std::ostringstream oss;
    oss << center_x << "," << center_y << "," << heading;
    std::string result = oss.str();
    // ROS_INFO("positioning-->%s", result.c_str());
    socketManager->broadcastUDPMessage(result);
}


void ImuSubscriber::processPacketData(IPEDataPacket &packet, double timestamp,  SensorData* data) {
    // double PositionVector_data[8] = {};
    std::vector<double> PositionVector_data;
    int PositionVector_size[2];
    
    // double PositionOut[7]; // PositioningSystem_V2_1 version (maintained before 2024.03.28, jang.sh)
    // double PositionOut[10]; // PositioningSystem_V2_2 version (after 2024.03.28, jang.sh)

    double acc_y = packet.linear_y.back();
    double acc_z = packet.linear_z.back();
    double gyro_y = packet.angular_y.back();
    double gyro_z = packet.angular_z.back();
    if(acc_z > 0) {
        acc_y *= -1;
        acc_z *= -1;
        gyro_y *= -1;
        gyro_z *= -1;
    }

    PositionVector_data.push_back(packet.s_time);
    PositionVector_data.push_back(5.0);
    if (!packet.linear_x.empty()) {
        PositionVector_data.push_back(packet.linear_x.back());
    } else {
        PositionVector_data.push_back(0);
    }
    if (!packet.linear_y.empty()) {
        PositionVector_data.push_back(acc_y);
    } else {
        PositionVector_data.push_back(0);
    }
    if (!packet.linear_z.empty()) {
        PositionVector_data.push_back(acc_z);
    } else {
        PositionVector_data.push_back(0);
    }

    if (!packet.angular_x.empty()) {
        PositionVector_data.push_back(packet.angular_x.back());
    } else {
        PositionVector_data.push_back(0);
    }
    if (!packet.angular_y.empty()) {
        PositionVector_data.push_back(gyro_y);
    } else {
        PositionVector_data.push_back(0);
    }
    if (!packet.angular_z.empty()) {
        PositionVector_data.push_back(gyro_z);
    } else {
        PositionVector_data.push_back(0);
    }
    
    // PositioningSystem_V2_1(PositionVector_data.data(), PositionVector_size, PositionOut); // before 2024.03.28 (written by jang.sh)
    // PositioningSystem_V2_2(PositionVector_data.data(), PositionVector_size, PositionOut); // before 2024.03.28 (written by jang.sh)
    PositioningSystem_V2_3(PositionVector_data.data(), PositionVector_size, PositionOut); //  Changed 2024.04.01 (ahn.jw)



    // Send Result
    sendUDPMessage(PositionOut[0], PositionOut[1], PositionOut[3]); // Kalman result (jang.sh)
    // sendUDPMessage(PositionOut[7], PositionOut[8], PositionOut[9]); // UWB result (jang.sh)
    // matching_func1();
    // matching_func2();
    rviz_match();
    
    
}
