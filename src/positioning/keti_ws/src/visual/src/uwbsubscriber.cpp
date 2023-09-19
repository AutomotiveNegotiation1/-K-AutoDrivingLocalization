#include "uwbsubscriber.h"

std::vector<std::string> RxID_data_list;
std::vector<int> RxID_list;
std::vector<double> xain_list;
std::vector<double> yain_list;
bool statusUWB = false; // or whatever default value you wish to set
std::vector<std::string> difference;
std::vector<double> xain_difference;
std::vector<double> yain_difference;
creal_T tag_pos_b[4];
creal_T prevTagPos[4];
std::string tagNum;


Operator::Operator(std::function<void(const PosDataPacket&)> callback, KapDataPacket &packet, double timestamp, PosDataPacket &pos)
    : callback_(callback)  // Initialize the callback
{
    double zt_b = 2.30;
    bool flag = false;

    double Ln = 6;
    double Lp = 4;
    double LnC = static_cast<double>(RxID_data_list.size());
    double TagNum = extractNumber(packet.frame_id)+1;
    double Nanchor = packet.id.size();
    
    for (size_t i = 0; i < Nanchor; ++i) {
        auto it = std::find(RxID_data_list.begin(), RxID_data_list.end(), packet.id[i]);
        if (it != RxID_data_list.end()) {
            packet.RxID[i] = std::distance(RxID_data_list.begin(), it) + 1;
            packet.RxDist[i] = std::sqrt(std::pow(packet.distanceFromTag[i], 2) - std::pow(packet.z[i], 2));
        }
    }

    int index = 0;
    double RxIDin[6];
    for(double val : packet.RxID) {
        RxIDin[index] = val;
        ++index;
    }

    index = 0;
    double RxDistin[6];
    for(double val : packet.RxDist) {
        RxDistin[index] = val;
        ++index;
    }

    index = 0;
    double xain[6];
    for(double val : xain_list) {
        xain[index] = val;
        pos.x.push_back(xain[index]);
        ++index;
    }

    index = 0;
    double yain[6];
    for(double val : yain_list) {
        yain[index] = val;
        pos.y.push_back(yain[index]);
        ++index;
    }


    double s_time = timestamp;

    double UWBout[21];
    double prevTagHeading;
    if (flag == false){
        prevTagHeading = 0;
        argInit_1x4_creal_T(tag_pos_b);
        argInit_1x4_creal_preT(prevTagPos);
        flag = true;
    }

    UWBpos6(Ln, Lp, LnC, TagNum, Nanchor, RxIDin, RxDistin, s_time, tag_pos_b, xain, yain, prevTagPos, prevTagHeading, UWBout);

    pos.tag_pos_est.resize(4);
    pos.tag_pos_est[0].re = UWBout[0];
    pos.tag_pos_est[1].re = UWBout[2];
    pos.tag_pos_est[2].re = UWBout[1];
    pos.tag_pos_est[3].re = UWBout[3];

    pos.tag_pos_est[0].im = UWBout[4];
    pos.tag_pos_est[1].im = UWBout[6];
    pos.tag_pos_est[2].im = UWBout[5];
    pos.tag_pos_est[3].im = UWBout[7];

    pos.tag_pos_est_aver.resize(4);
    pos.tag_pos_est_aver[0].re = UWBout[9];
    pos.tag_pos_est_aver[1].re = UWBout[10];
    pos.tag_pos_est_aver[2].re = UWBout[11];
    pos.tag_pos_est_aver[3].re = UWBout[12];
    
    pos.tag_pos_est_aver[0].im = UWBout[13];
    pos.tag_pos_est_aver[1].im = UWBout[14];
    pos.tag_pos_est_aver[2].im = UWBout[15];
    pos.tag_pos_est_aver[3].im = UWBout[16];
    pos.heading_est = UWBout[8];
    pos.headingest_a_aver_v = UWBout[17];

    callback_(pos);

    std::vector<creal_T> tag_pos_est(4);   // Each element should be an array or another container. This depends on the exact type of data.
    std::vector<creal_T> tag_pos_est_aver(4);

    tag_pos_est[0].re = UWBout[0];
    tag_pos_est[1].re = UWBout[1];
    tag_pos_est[2].re = UWBout[2];
    tag_pos_est[3].re = UWBout[3];

    tag_pos_est[0].im = UWBout[4];
    tag_pos_est[1].im = UWBout[5];
    tag_pos_est[2].im = UWBout[6];
    tag_pos_est[3].im = UWBout[7];

    ROS_INFO("tag_pos_est--> (%f,%f), (%f,%f)", tag_pos_est[0].re, tag_pos_est[0].im, tag_pos_est[1].re, tag_pos_est[1].im);
    ROS_INFO("tag_pos_est--> (%f,%f), (%f,%f)", tag_pos_est[2].re, tag_pos_est[2].im, tag_pos_est[3].re, tag_pos_est[3].im);

    // prevTagPos[0].re = UWBout[0];
    // prevTagPos[1].re = UWBout[1];
    // prevTagPos[2].re = UWBout[2];
    // prevTagPos[3].re = UWBout[3];
    
    // prevTagPos[0].im = UWBout[4];
    // prevTagPos[1].im = UWBout[5];
    // prevTagPos[2].im = UWBout[6];
    // prevTagPos[3].im = UWBout[7];
    

    double heading_est = UWBout[8];
    ROS_INFO("heading_est--> %f", heading_est);

    tag_pos_est_aver[0].re = UWBout[9];
    tag_pos_est_aver[1].re = UWBout[10];
    tag_pos_est_aver[2].re = UWBout[11];
    tag_pos_est_aver[3].re = UWBout[12];

    tag_pos_est_aver[0].im = UWBout[13];
    tag_pos_est_aver[1].im = UWBout[14];
    tag_pos_est_aver[2].im = UWBout[15];
    tag_pos_est_aver[3].im = UWBout[16];

    ROS_INFO("tag_pos_est_aver--> (%f,%f), (%f,%f)", tag_pos_est_aver[0].re, tag_pos_est_aver[0].im, tag_pos_est_aver[1].re, tag_pos_est_aver[1].im);
    ROS_INFO("tag_pos_est_aver--> (%f,%f), (%f,%f)", tag_pos_est_aver[2].re, tag_pos_est_aver[2].im, tag_pos_est_aver[3].re, tag_pos_est_aver[3].im);

    double headingest_a_aver_v = UWBout[17];
    ROS_INFO("headingest_a_aver_v--> %f", headingest_a_aver_v);
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

// Method to get name
std::string UwbSubscriber::getName() const {
    return frame_id;  // or whatever you wish to return as name
}

UwbSubscriber::UwbSubscriber(ros::NodeHandle& node, std::string uwbNum, KapCallback *kapCallback)
    :m_kapCallback(kapCallback)
{
    bool statusUWB = false;
    tagNum = uwbNum;
    int temp = std::stoi(uwbNum) + 1; // 숫자로 변환 후 1을 더함
    frame_id = std::to_string(temp); // 다시 문자열로 변환
    topic_name_stream << "/dwm1001/anchor/ttyUWB" << tagNum;

    std::string topic_name = topic_name_stream.str();

    ROS_INFO("topic_name-->%s", topic_name.c_str());
    // m_kapCallback = new KapCallback();

    sub = node.subscribe<visual::Anchor>(topic_name, 10, &UwbSubscriber::UwbSubscriber_callback, this);
}

void UwbSubscriber::UwbSubscriber_callback(const visual::Anchor::ConstPtr& msg)
{
    m_kapDataPacket = KapDataPacket(msg);
    UwbSubscriber_setRxid(msg, m_kapDataPacket);  // 정확한 함수 호출
    // Check if m_kapCallback is not a nullptr before invoking the function
    if (m_kapCallback) {
        m_kapCallback->onLiveDataAvailable(m_kapDataPacket);
    } else {
        ROS_WARN("m_kapCallback is a nullptr!");
    }
}

void UwbSubscriber::UwbSubscriber_setRxid(const visual::Anchor::ConstPtr& msg, KapDataPacket packet)
{
    std::vector<std::string> ids = msg->id;  // Initialize ids from msg
    std::vector<double> xas = msg->x;
    std::vector<double> yas = msg->y;
    if (RxID_list.empty() && xain_list.empty() && yain_list.empty())
    {
        RxID_data_list = msg->id;
        xain_list = msg->x;
        yain_list = msg->y;
        for (int i = 0; i < ids.size(); ++i)  // Initialize i and use ids
        {
            RxID_list.push_back(i);
        }
    }
    else
    {
        std::set<std::string> ids_set(ids.begin(), ids.end());
        std::set<std::string> RxID_data_set(RxID_data_list.begin(), RxID_data_list.end());

        std::set<double> xain_set(xas.begin(), xas.end());
        std::set<double> xain_data_set(xain_list.begin(), xain_list.end());

        std::set<double> yain_set(yas.begin(), yas.end());
        std::set<double> yain_data_set(yain_list.begin(), yain_list.end());

        if (ids_set != RxID_data_set && xain_set != xain_data_set && yain_set != yain_data_set) 
        {
            difference.clear();
            xain_difference.clear();
            yain_difference.clear();
            for (const auto& item : ids) 
            {
                if (RxID_data_set.find(item) == RxID_data_set.end()) 
                {
                    difference.push_back(item);
                }
            }

            for (const auto& item : xas) 
            {
                if (xain_data_set.find(item) == xain_data_set.end()) 
                {
                    xain_difference.push_back(item);
                }
            }

            for (const auto& item : yas) 
            {
                if (yain_data_set.find(item) == yain_data_set.end()) 
                {
                    yain_difference.push_back(item);
                }
            }
            
        }   
        RxID_data_list.insert(RxID_data_list.end(), difference.begin(), difference.end());
        xain_list.insert(xain_list.end(), xain_difference.begin(), xain_difference.end());
        yain_list.insert(yain_list.end(), yain_difference.begin(), yain_difference.end());
        RxID_list.clear();
        for (int i = 0; i < RxID_data_list.size(); ++i) 
        {
            RxID_list.push_back(i);
        }
    }   

} 

bool UwbSubscriber::getDataEmpty(){
    m_kapCallback->getDataEmpty();
    return m_kapCallback->getDataEmpty();
}

creal_T argInit_creal_T(int idx)
{
    creal_T result;
    std::vector<double> xt_b = {-0.525, 0.525, -0.525, 0.525};
    std::vector<double> yt_b = {0.505, 0.505, -0.505, -0.505};

    if (idx < xt_b.size()) {
        result.re = xt_b[idx];
        result.im = yt_b[idx];
    }
    return result;
}

void argInit_1x4_creal_T(creal_T result[4])
{
    /* Loop over the array to initialize each element. */
    for (int idx1 = 0; idx1 < 4; idx1++) {
        result[idx1] = argInit_creal_T(idx1);
    }
}

creal_T argInit_creal_preT(int idx)
{
    creal_T result;
    std::vector<double> xt_b = {0, 0, 0, 0};
    std::vector<double> yt_b = {0, 0, 0, 0};

    for (size_t i = 0; i < xt_b.size(); ++i) {
        result.re = xt_b[i];
        result.im = yt_b[i];
    }
    return result;
}

void argInit_1x4_creal_preT(creal_T result[4])
{
    int idx1;
    /* Loop over the array to initialize each element. */
    for (idx1 = 0; idx1 < 4; idx1++) {
        result[idx1] = argInit_creal_preT(idx1);
    }
}