#ifndef __MAIN_H__
#define __MAIN_H__


#ifdef __cplusplus
extern "C" {
#endif
    void *spinfor(void *data);
    void *rx_cctv(void *data);
    void *sendto_kanavi(void *data);
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
extern float vt_heading;
extern double kanavi_inout; 

#endif // __PARKING_MAP_H__