#ifndef __VIEWER_LIDAR_MAIN_H__
#define __VIEWER_LIDAR_MAIN_H__


#ifdef __cplusplus
extern "C" {
#endif
    void *spinfor(void *data);
#ifdef __cplusplus
}
#endif


typedef enum _SITE
{
    GLOBAL,
    ROBO
}SITE;

extern float vt_uwb_x; 
extern float vt_uwb_y; 

#endif // __VIEWER_LIDAR_MAIN_H__