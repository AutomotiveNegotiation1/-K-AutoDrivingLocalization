#ifndef __PARKING_MAP_H__
#define __PARKING_MAP_H__

#include "main.h"


#ifdef __cplusplus
extern "C" {
#endif

    int map_init(int argc, char **argv);
    //void set_uwb(double _x, double _y, double _angle);
    //void set_lidar(double _x, double _y, double _angle);
    void set_slam_orientation(double pitch); 
    void set_slam_position(double _x, double _y); 
    //void set_kanavi_msg(double in_out, double _x, double _y);
    void set_cctv_tl(bool _on);
    void set_cctv(double _x, double _y);
    void set_rosparam(SITE _site, bool _use_traffic_light, bool _use_ipe, bool _use_lidar, bool _use_cctv, bool _ramp_match);
    double determine_indoor(double pose_x, double pose_y, double pose_z);
    void set_carnival_tl(bool _on);
    void set_nego_state(int state);
    void set_nego_label(char *text);
    void set_nego_label2(char *text); 
    void set_nego_button_text(char *text);

    typedef enum _NEGO_STATE
    {
        NEGO_INIT,
        NEGO_WAIT_CARNIVAL,
        NEGO_WAIT_IONIC,
        NEGO_WAIT,
        NEGO_MANEUVER,
        NEGO_NEGOTIATION,
        NEGO_ACTION_IONIC,
        NEGO_ACTION_CARNIVAL,
        NEGO_FINISH,
        NEGO_END
    }NEGO_STATE;

#ifdef __cplusplus
}
#endif


#endif  // __PARKING_MAP_H__