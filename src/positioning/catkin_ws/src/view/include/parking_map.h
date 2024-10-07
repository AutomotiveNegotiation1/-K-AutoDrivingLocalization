#ifndef __PARKING_MAP_H__
#define __PARKING_MAP_H__

#include "main.h"


#ifdef __cplusplus
extern "C" {
#endif

    int map_init(int argc, char **argv);
    void set_uwb(double _x, double _y, double _angle);
    void set_lidar(double _x, double _y, double _angle);
    void set_slam_orientation(double pitch); 
    void set_cctv(double _x, double _y);
    void set_rosparam(SITE _site, bool _use_traffic_light, bool _use_ipe, bool _use_lidar, bool _use_cctv, bool _ramp_match);
#ifdef __cplusplus
}
#endif


#endif  // __PARKING_MAP_H__