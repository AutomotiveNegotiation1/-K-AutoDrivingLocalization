#ifndef UWBSUBSCRIBER_H
#define UWBSUBSCRIBER_H

#include <sstream>
#include <set>
#include <unordered_map>
#include <boost/variant.hpp>
#include <complex>

#include <ros/ros.h>
#include <visual/Anchor.h>

#include "packetcallback.h"
#include "kapcallback.h"
#include "kapdatapacket.h"
#include "mainwindow.h"

#include "rtwtypes.h"
#include "UWBpos6.h"
#include "UWBpos6_terminate.h"
#include "rt_nonfinite.h"
#include <functional>  // for std::function

// Assuming that these variables are used globally across multiple source files,
// if not, consider moving them into an appropriate class or namespace.
extern std::vector<std::string> RxID_data_list;
extern std::vector<int> RxID_list;
extern bool statusUWB;
extern std::vector<std::string> difference;
extern creal_T tag_pos_b[4];
extern creal_T prevTagPos[4];
extern std::string tagNum;

creal_T argInit_creal_T(int idx);
void argInit_1x4_creal_T(creal_T result[4]);
creal_T argInit_creal_preT(int idx);
void argInit_1x4_creal_preT(creal_T result[4]);

struct Operator 
{
    double zt_b = 2.30;
    double Ln = 6;
    double Lp = 4;

    Operator(std::function<void(const PosDataPacket&)> callback, KapDataPacket &packet, ros::Time timestamp, PosDataPacket &pos);  // 선언만 있어야 함
    
    // ... other members
    double extractNumber(const std::string& input);

private:
    std::function<void(const PosDataPacket&)> callback_;  // Store the callback
};


struct UwbSubscriber 
{
    ros::Subscriber sub;
    std::string frame_id = "tag";
    std::ostringstream topic_name_stream;
    KapCallback* m_kapCallback;

    // Method declarations
    std::string getName() const;
    UwbSubscriber(ros::NodeHandle& node, std::string uwbNum);
    void UwbSubscriber_callback(const visual::Anchor::ConstPtr& msg);
    void UwbSubscriber_setRxid(const visual::Anchor::ConstPtr& msg);
    bool getDataEmpty();
};

#endif
