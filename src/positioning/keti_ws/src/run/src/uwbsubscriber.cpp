// uwbsubscriber.cpp

#include "mainwindow.h"
#include "uwbsubscriber.h"


std::vector<std::string> RxID_data_list;
std::vector<int> RxID_list;
bool statusUWB = false; // or whatever default value you wish to set
std::vector<std::string> difference;
creal_T tag_pos_b[4];
creal_T prevTagPos[4];
std::string tagNum;


Operator::Operator(MainWindow& win, KapDataPacket &packet, ros::Time timestamp)
    : window(win)
{
    std::stringstream ss;
    std::stringstream ss2;
    bool flag = false;
    
    UwbSubscriber_setRxid(packet);  // 정확한 함수 호출

    // packet.x와 packet.y의 실제 길이에 따라 배열의 크기를 정의
    size_t numPoints = packet.id.size(); // 만약 id의 크기가 x와 y와 동일하다면, 그렇지 않으면 적절한 필드를 사용하여 정의하십시오.
    double xain[numPoints];
    double yain[numPoints];

    for (size_t i = 0; i < numPoints; ++i) {
        auto it = std::find(RxID_data_list.begin(), RxID_data_list.end(), packet.id[i]);
        if (it != RxID_data_list.end()) {
            packet.RxID[i] = std::distance(RxID_data_list.begin(), it) + 1;
            packet.RxDist[i] = std::sqrt(std::pow(packet.distanceFromTag[i], 2) - std::pow(packet.z[i], 2));
            ss2 << packet.distanceFromTag[i] << " ";
            ss << packet.RxDist[i] << " ";
        }
        
        // 배열에 값을 할당
        xain[i] = packet.x[i];
        yain[i] = packet.y[i];
    }
    double LnC = RxID_data_list.size();
    double TagNum = extractNumber(packet.frame_id);
    double Nanchor = packet.id.size();
    const double RxIDin[6] = {packet.RxID[0], packet.RxID[1], packet.RxID[2], packet.RxID[3], packet.RxID[4], packet.RxID[5]};
    const double RxDistin[6] = {packet.RxDist[0], packet.RxDist[1], packet.RxDist[2], packet.RxDist[3], packet.RxDist[4], packet.RxDist[5]};
    double s_time = timestamp.toSec();
    double UWBout[21];
    double prevTagHeading;
    if (flag == false){
        prevTagHeading = 0;
        argInit_1x4_creal_T(tag_pos_b);
        argInit_1x4_creal_preT(prevTagPos);
        flag = true;
    }

    UWBpos6(Ln, Lp, LnC, TagNum, Nanchor, RxIDin, RxDistin, s_time, tag_pos_b, xain, yain, prevTagPos, prevTagHeading, UWBout);

    std::vector<creal_T> tag_pos_est(4);   // Each element should be an array or another container. This depends on the exact type of data.
    std::vector<creal_T> tag_pos_est_aver(4);

    std::array<std::complex<double>, 4> tag_positions;
    std::array<double, 4> x_values;
    std::array<double, 4> y_values;
    
    for (int i = 0; i < 4; i++) {
        tag_positions[i].real(UWBout[i]);
        tag_positions[i].imag(UWBout[i + 4]);
        
        x_values[i] = packet.x[i];
        y_values[i] = packet.y[i];
    }
    
    double heading_est = UWBout[8];

    // window.updateGraph(tag_positions, heading_est, x_values, y_values);
    // tag_pos_est[0].re = UWBout[0];
        // tag_pos_est[1].re = UWBout[1];
        // tag_pos_est[2].re = UWBout[2];
        // tag_pos_est[3].re = UWBout[3];

        // tag_pos_est[0].im = UWBout[4];
        // tag_pos_est[1].im = UWBout[5];
        // tag_pos_est[2].im = UWBout[6];
        // tag_pos_est[3].im = UWBout[7];

        // ROS_INFO("tag_pos_est--> (%f,%f), (%f,%f)", tag_pos_est[0].re, tag_pos_est[0].im, tag_pos_est[1].re, tag_pos_est[1].im);
        // ROS_INFO("tag_pos_est--> (%f,%f), (%f,%f)", tag_pos_est[2].re, tag_pos_est[2].im, tag_pos_est[3].re, tag_pos_est[3].im);

        // prevTagPos[0].re = UWBout[0];
        // prevTagPos[1].re = UWBout[1];
        // prevTagPos[2].re = UWBout[2];
        // prevTagPos[3].re = UWBout[3];
        
        // prevTagPos[0].im = UWBout[4];
        // prevTagPos[1].im = UWBout[5];
        // prevTagPos[2].im = UWBout[6];
        // prevTagPos[3].im = UWBout[7];
        

        // double heading_est = UWBout[8];
        // ROS_INFO("heading_est--> %f", heading_est);

        // tag_pos_est_aver[0].re = UWBout[9];
        // tag_pos_est_aver[1].re = UWBout[10];
        // tag_pos_est_aver[2].re = UWBout[11];
        // tag_pos_est_aver[3].re = UWBout[12];

        // tag_pos_est_aver[0].im = UWBout[13];
        // tag_pos_est_aver[1].im = UWBout[14];
        // tag_pos_est_aver[2].im = UWBout[15];
        // tag_pos_est_aver[3].im = UWBout[16];

        // ROS_INFO("tag_pos_est_aver--> (%f,%f), (%f,%f)", tag_pos_est_aver[0].re, tag_pos_est_aver[0].im, tag_pos_est_aver[1].re, tag_pos_est_aver[1].im);
        // ROS_INFO("tag_pos_est_aver--> (%f,%f), (%f,%f)", tag_pos_est_aver[2].re, tag_pos_est_aver[2].im, tag_pos_est_aver[3].re, tag_pos_est_aver[3].im);

        // double headingest_a_aver_v = UWBout[17];
        // ROS_INFO("headingest_a_aver_v--> %f", headingest_a_aver_v);
}

double Operator::extractNumber(const std::string& input) {
    std::string numberStr;
    
    // Remove non-numeric characters
    for (char c : input) {
        if (std::isdigit(c)) {
            numberStr += c;
        }
    }

    // Convert the numeric string to double
    return std::stod(numberStr);
}

void Operator::UwbSubscriber_setRxid(const KapDataPacket &packet)
{
    std::vector<std::string> ids = packet.id;  // Initialize ids from msg
    if (RxID_list.empty())
    {
        RxID_data_list = packet.id;
        for (int i = 0; i < ids.size(); ++i)  // Initialize i and use ids
        {
            RxID_list.push_back(i);
        }
    }
    else
    {
        std::set<std::string> ids_set(ids.begin(), ids.end());
        std::set<std::string> RxID_data_set(RxID_data_list.begin(), RxID_data_list.end());

        if (ids_set != RxID_data_set) 
        {
            difference.clear();
            for (const auto& item : ids) 
            {
                if (RxID_data_set.find(item) == RxID_data_set.end()) 
                {
                    difference.push_back(item);
                }
            }
        }   
        RxID_data_list.insert(RxID_data_list.end(), difference.begin(), difference.end());
        RxID_list.clear();
        for (int i = 0; i < RxID_data_list.size(); ++i) 
        {
            RxID_list.push_back(i);
        }
    }   
} 


// Method to get name
std::string UwbSubscriber::getName() const {
    return frame_id;  // or whatever you wish to return as name
}

UwbSubscriber::UwbSubscriber(ros::NodeHandle& node, std::string uwbNum, KapCallback* kapCallback)
    : m_kapCallback(kapCallback)
{
    bool statusUWB = false;
    tagNum = uwbNum;
    int temp = std::stoi(uwbNum) + 1; // 숫자로 변환 후 1을 더함
    frame_id = std::to_string(temp); // 다시 문자열로 변환
    topic_name_stream << "/dwm1001/anchor/ttyUWB" << tagNum;

    std::string topic_name = topic_name_stream.str();

    ROS_INFO("topic_name-->%s", topic_name.c_str());

    sub = node.subscribe<visual::Anchor>(topic_name, 10, &UwbSubscriber::UwbSubscriber_callback, this);
}

void UwbSubscriber::UwbSubscriber_callback(const visual::Anchor::ConstPtr& msg)
{
    KapDataPacket packet;

    for (size_t i = 0; i < 6; ++i) {
        packet.x.push_back(0);
        packet.y.push_back(0);
        packet.z.push_back(0);
        packet.RxID.push_back(0);
        packet.RxDist.push_back(0);
    }

    packet.stamp.sec = msg->header.stamp.sec;
    packet.stamp.nsec = msg->header.stamp.nsec;
    packet.frame_id = frame_id;
    packet.id = msg->id;
    for (int i = 0; i < packet.id.size(); i++)
    {
        packet.x[i] = msg->x[i];
        packet.y[i] = msg->y[i];
        packet.z[i] = msg->z[i];    
    }
    packet.distanceFromTag = msg->distanceFromTag;

    // Check if m_kapCallback is not a nullptr before invoking the function
    if (m_kapCallback)
    {
        m_kapCallback->onLiveDataAvailable(packet);
    }
    else
    {
        ROS_WARN("m_kapCallback is a nullptr!");
    }
}

bool UwbSubscriber::getDataEmpty(){
    m_kapCallback->getDataEmpty();
    return m_kapCallback->getDataEmpty();
}

creal_T argInit_creal_T(int idx)
{
    static const std::vector<double> xt_b = {-0.525, 0.525, -0.525, 0.525};
    static const std::vector<double> yt_b = {0.505, 0.505, -0.505, -0.505};

    creal_T result = {0, 0};  // Default initialization

    if (idx >= 0 && idx < xt_b.size()) {
        result.re = xt_b[idx];
        result.im = yt_b[idx];
    }
    return result;
}

void argInit_1x4_creal_T(creal_T result[4])
{
    for (int idx1 = 0; idx1 < 4; idx1++) {
        result[idx1] = argInit_creal_T(idx1);
    }
}

creal_T argInit_creal_preT(int idx)
{
    static const std::vector<double> xt_b = {0,0,0,0};
    static const std::vector<double> yt_b = {0,0,0,0};

    creal_T result = {0, 0};  // Default initialization

    if (idx >= 0 && idx < xt_b.size()) {
        result.re = xt_b[idx];
        result.im = yt_b[idx];
    }
    return result;
}

void argInit_1x4_creal_preT(creal_T result[4])
{
    for (int idx1 = 0; idx1 < 4; idx1++) {
        result[idx1] = argInit_creal_preT(idx1);
    }
}