#ifndef __MAIN_H__
#define __MAIN_H__


#ifdef __cplusplus
extern "C" {
#endif
    void *spinfor(void *data);
    void *rx_cctv(void *data);
#ifdef __cplusplus
}
#endif


typedef enum _SITE
{
    GLOBAL,
    ROBO
}SITE;


#endif // __PARKING_MAP_H__