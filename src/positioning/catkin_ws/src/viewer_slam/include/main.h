#ifndef __MAIN_H__
#define __MAIN_H__


#ifdef __cplusplus
extern "C" {
#endif
    void *spinfor(void *data);
    void *rx_cctv(void *data);
    void *sendto_kanavi(void *data);
    void set_send_message(char *buff, int size, int timeout);
    void set_change_state(char *buff, int size, int timeout);
    void send_message_to_server(char *buff, int size);
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

//extern double kanavi_x; 
//extern double kanavi_y; 
//extern double kanavi_heading; 
extern double kanavi_inout; 
extern double ai_inout; 
//extern double ai_inout_previous;
//extern double ai_inout_current;

#endif // __PARKING_MAP_H__