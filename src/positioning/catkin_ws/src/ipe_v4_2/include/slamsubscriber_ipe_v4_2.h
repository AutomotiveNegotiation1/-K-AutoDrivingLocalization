
#include <sstream>
#include <set>
#include <ros/ros.h>
#include <sensor_msgs/Imu.h>
#include <functional>

#include "packetcallback.h"
#include "ipecallback.h"
#include "ipedatapacket.h"
#include "uwbsubscriber.h"
#include "socketmanager.h"
#include "sensordata.h"
#include "geometry_msgs/PoseStamped.h"
#include "std_msgs/String.h"
// #include "nav_msgs/Path.h"

// #include "IMUpos.h"
// #include "PositioningSystem_V2_1.h" // Changed 2024.03.28 (jang.sh)
// #include "PositioningSystem_V2_2.h" // Changed 2024.03.28 (jang.sh)
//#include "PositioningSystem_V2_3.h" // Changed 2024.04.01 (ahn.jw)
//#include "PositioningSystem_V4_2.h" // Changed 2024.06.17 (joo.hy)

#include "PositioningSystem_V5_1.h" // Changed 2024.06.17 (joo.hy)

#include "rt_nonfinite.h"
#include "rtwtypes.h"

//#include "map_param_chungra.h" // Changed 2024.06.17 (joo.hy)


class SlamSubscriber : public PacketCallback {
private:
    ros::Publisher pub_slam;
    ros::Publisher pub_uwb;
    ros::Publisher pub_ipe;
    
    ros::Subscriber sub_slam;
    
    //std::string frame_id = "slam";
    
    std::ostringstream topic_name_stream;
    std::string m_frameId = "slam";

    SocketManager* socketManager;

    IPECallback* o_ipeCallback;
    IPECallback* m_ipeCallback;
    IPEDataPacket m_ipeDataPacket;
    std::vector<std::function<void(int)>> callbacks;

    //const double Ln = 6.0;
    //const double Lp = 4.0;

public:
    //ros::NodeHandle &node;
    SlamSubscriber(ros::NodeHandle& node, IPECallback* ipeCallback);
    ~SlamSubscriber();
    void operator()(IPEDataPacket &packet, double timestamp, SensorData* data);
    void registerCallback(const std::function<void(int)>& callback);
    //void sendEvent(int data);
    std::string getPacketFrameID();

    void sendUDPMessage(double center_x, double center_y, double heading, std::string in_out);

private:
    void setupSubscriber(ros::NodeHandle& node);
    //void _callback_lidarpose(const geometry_msgs::PoseStamped::ConstPtr& msg);
    //void _callback(const sensor_msgs::Imu::ConstPtr& msg);
    //void _callback_control(const std_msgs::String::ConstPtr& msg);
    void _callback(const geometry_msgs::PoseStamped::ConstPtr& msg);
    void rviz_match(double new_pos[3]) ;
    void processPacketData(IPEDataPacket &packet, double timestamp,  SensorData* data);
};
