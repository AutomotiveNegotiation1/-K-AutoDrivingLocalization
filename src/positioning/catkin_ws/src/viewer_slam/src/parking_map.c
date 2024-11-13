#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <time.h>
#include <pwd.h>
#include <pthread.h>
#include <gtk/gtk.h>
#include <gtk/gtkx.h>
#include <cairo.h>
#include "parking_map.h"
#include "main.h"
#include "ramp_pp.h"
#include <math.h>

#define TRAFFICLIGHT_TEST

#define CIRCLE_ENTRY_X 1474
#define CIRCLE_TOP_Y 447
#define CIRCLE_BOTTOM_Y 773.25
#define CIRCLE_X 1610
#define CIRCLE_Y 630
#define RADIUS 143.75

#define map_y_a 252
#define map_y_z 894
#define map_y_leng map_y_z - map_y_a // 642
#define map_x_a 71
#define map_x_z 1399
#define map_x_leng map_x_z - map_x_a // 1328

#define origin_map_y_a 65 
#define origin_map_y_z 957
#define origin_map_y_leng origin_map_y_z - origin_map_y_a // 892
#define origin_map_x_a 68
#define origin_map_x_z 1852
#define origin_map_x_leng origin_map_x_z - origin_map_x_a // 1784

#define max(a, b) a > b ? a : b
#define ADAPTIVE_SCALE

enum PAINT
{
    FROM_UWB,
    FROM_CCTV
};

typedef enum _RAMP_STATE
{
    NOT_IN_RAMP,
    IN_RAMP_TOP,
    IN_RAMP_CIRCLE,
    IN_RAMP_BOTTOM,
    RAMP_FINISH
}RAMP_STATE;

// Get ROS Param
SITE site;
bool use_traffic_light;
bool use_ipe;
bool use_lidar;
bool use_cctv;
bool ramp_match;

RAMP_STATE ramp_state = NOT_IN_RAMP;
NEGO_STATE nego_state = NEGO_INIT;
// get object from glade
GtkBuilder *gBuilder;
GtkWidget *gApp;
GtkFixed *gFixed;
GtkDrawingArea *drawingArea;
GdkPixbuf *pixbuf;
GtkImage *gImage;
GdkPixbuf *carnivalPixbuf;
GdkPixbuf *cctvPixbuf;
GtkImage *carA;
GtkImage *carB;
GtkImage *cctvImage;
GtkWidget *carALabel;
GtkWidget *carBLabel;

GtkWidget *gNegoButton;
GtkWidget *gNegoLabel;
GtkWidget *gNegoLabel2;
GtkWidget *gNegoBox;

// traffic light
GtkImage *tlgImage_usd;
GtkImage *tlgImage_green_usd;
GtkImage *tlgImage_ccw;
GtkImage *tlgImage_green_ccw;

cairo_surface_t *cst_uwb;
cairo_surface_t *cst_cctv;
GdkWindow *draw_window;
cairo_region_t *cairoRegion;
bool first = TRUE;
bool carnival_on = FALSE;
bool cctv_on = FALSE;

GMutex gmutex;

double uwb_x = 100.0;
double uwb_y = 100.0;

double slam_x = 100.0;
double slam_y = 100.0; 

double uwb_angle = 0;
double before_cctv_x = 1000000.0;
double cctv_x = 0.0;
double cctv_y = 0.0;
double cctv_angle = 270;
bool isUpdate_uwb = FALSE;
bool isUpdate_cctv = FALSE;

// origin => 2519, 3984
//  target_zoom
// double car_width = 40.0;
// double car_height = 60.0;
// double carnival_width = 52; // 155
// double carnival_height = 107; // 323
double carnival_width = 25.83; // 155
double carnival_height = 53.83; // 323
double carnival2_width = 78.25; // 626
double carnival2_height = 62.625; // 501
double car_width = 29.77;
double car_height = 43.18;
double ioniq_width = 117.375;
double ioniq_height = 93.9375;

double area_offset;
double center_offset_x;
double center_offset_y;
double carnival_center_offset_x;
double carnival_center_offset_y;

struct timeval cctv_tv, uwb_tv;

void draw_car(GdkDrawingContext *_drawingContext, double _x, double _y, double _angle, enum PAINT _paint);
void position_offset_lidar(double *x, double *y);
void position_offset_uwb(double *x, double *y);
void position_offset_cctv(double *x, double *y);
gint callback_timer(gpointer argv);
void rotate_point(double _angle, double *new_x, double *new_y);
gboolean manually_draw();
gboolean on_draw(GtkWidget *widget, GdkEventExpose *event, gpointer data);
void draw_rect_on_fixed(GtkFixed *fixed, double x, double y);

float vt_uwb_x; 
float vt_uwb_y;
float vt_heading; 

//double kanavi_x; 
//double kanavi_y;
//double kanavi_heading;
double kanavi_inout;
double ai_inout;

struct RAMP_PP_DATA{
    float new_x; 
    float new_y; 
}; 


struct RAMP_PP_DATA ramp_post_process(float x, float y){

    float ramp_center_x = 1576; 
    float ramp_center_y = 639; 
    float ramp_radius = 125 ; 
    float new_x = -1; 
    float new_y = -1; 
    float theta = atan((x-ramp_center_x) / (y-ramp_center_y)); 

    float diff_x = x - ramp_center_x;
    float diff_y = y - ramp_center_y;


    if (diff_x >0 && diff_y<0){
        //printf("First PLANE\n");
        //printf("theta: %f\n", theta); 
        theta = (-1)*theta - (3.1416/2);
    }

    else if (diff_x >0 && diff_y>0){
        //printf("Second PLANE\n");
        //printf("theta: %f\n", theta); 
        theta = (3.1416/2) + ((-1)*theta) ;
    }

    else if (diff_x <0 && diff_y>0){
        //printf("Third PLANE\n");
        //printf("theta: %f\n", theta); 
        theta = (3.1416/2) + ((-1)*theta) ;
    }
    
    new_x = ramp_radius * cos(theta) + ramp_center_x; 
    new_y = ramp_radius * sin(theta) + ramp_center_y;
    
    struct RAMP_PP_DATA ramp_pp_data; 
    ramp_pp_data.new_x = new_x; 
    ramp_pp_data.new_y = new_y; 


    return ramp_pp_data ;
}


void trafficlight_init()
{
    if(!use_traffic_light) {
        printf("traffic_light false\n");
        return;
    }
    
    // GdkPixbuf *tlpixbuf = gdk_pixbuf_new_from_resource_at_scale("/glade/draw_tl_red.png", 86, 22, TRUE, NULL);
    // GdkPixbuf *tlpixbuf_green = gdk_pixbuf_new_from_resource_at_scale("/glade/draw_tl_up.png", 86, 22, TRUE, NULL);
    // GdkPixbuf *tlpixbuf_left = gdk_pixbuf_new_from_resource_at_scale("/glade/draw_tl_left.png", 86, 22, TRUE, NULL);
    GdkPixbuf *tlpixbuf = gdk_pixbuf_new_from_resource_at_scale("/glade/draw_tl_red.png", 101, 31, TRUE, NULL);
    GdkPixbuf *tlpixbuf_green = gdk_pixbuf_new_from_resource_at_scale("/glade/draw_tl_up.png", 101, 31, TRUE, NULL);
    GdkPixbuf *tlpixbuf_left = gdk_pixbuf_new_from_resource_at_scale("/glade/draw_tl_left.png", 101, 31, TRUE, NULL);
    // GdkPixbuf *tlpixbuf = gdk_pixbuf_new_from_resource_at_scale("/glade/draw_tl_red.png", 151, 46, TRUE, NULL);
    // GdkPixbuf *tlpixbuf_green = gdk_pixbuf_new_from_resource_at_scale("/glade/draw_tl_up.png", 151, 46, TRUE, NULL);
    // GdkPixbuf *tlpixbuf_left = gdk_pixbuf_new_from_resource_at_scale("/glade/draw_tl_left.png", 151, 46, TRUE, NULL);
    
    GdkPixbuf *tlpixbuf_usd = gdk_pixbuf_rotate_simple(tlpixbuf, GDK_PIXBUF_ROTATE_UPSIDEDOWN);
    GdkPixbuf *tlpixbuf_green_usd = gdk_pixbuf_rotate_simple(tlpixbuf_left, GDK_PIXBUF_ROTATE_UPSIDEDOWN);
    GdkPixbuf *tlpixbuf_ccw = gdk_pixbuf_rotate_simple(tlpixbuf, GDK_PIXBUF_ROTATE_COUNTERCLOCKWISE);
    GdkPixbuf *tlpixbuf_green_ccw = gdk_pixbuf_rotate_simple(tlpixbuf_green, GDK_PIXBUF_ROTATE_COUNTERCLOCKWISE);


    tlgImage_usd = gtk_image_new_from_pixbuf(tlpixbuf_usd);
    tlgImage_ccw = gtk_image_new_from_pixbuf(tlpixbuf_ccw);
    tlgImage_green_usd = gtk_image_new_from_pixbuf(tlpixbuf_green_usd);
    tlgImage_green_ccw = gtk_image_new_from_pixbuf(tlpixbuf_green_ccw);


    // gtk_fixed_put(gFixed, tlgImage_usd, 1264, 589);
    // gtk_fixed_put(gFixed, tlgImage_green_usd, 1264, 589);
    // gtk_fixed_put(gFixed, tlgImage_ccw, 1170, 464);
    // gtk_fixed_put(gFixed, tlgImage_green_ccw, 1170, 464);
    gtk_fixed_put(gFixed, tlgImage_usd, 1248, 585);
    gtk_fixed_put(gFixed, tlgImage_green_usd, 1248, 585);
    gtk_fixed_put(gFixed, tlgImage_ccw, 1153, 450);
    gtk_fixed_put(gFixed, tlgImage_green_ccw, 1153, 450);
    // gtk_fixed_put(gFixed, tlgImage_usd, 1230, 618);
    // gtk_fixed_put(gFixed, tlgImage_green_usd, 1230, 618);
    // gtk_fixed_put(gFixed, tlgImage_ccw, 1153, 426);
    // gtk_fixed_put(gFixed, tlgImage_green_ccw, 1153, 426);
}


gint callback_timer(gpointer argv)
{
    gint x, y;
    gtk_widget_get_pointer(gApp, &x, &y);
    gtk_widget_queue_draw(GTK_WIDGET(drawingArea));
    
    // printf("mouse position :: x = %d, y = %d\n", x, y);

    return TRUE;
}

void set_rosparam(SITE _site, bool _use_traffic_light, bool _use_ipe, bool _use_lidar, bool _use_cctv, bool _ramp_match)
{
    site = _site;
    use_traffic_light = _use_traffic_light;
    use_ipe = _use_ipe;
    use_lidar = _use_lidar;
    use_cctv = _use_cctv;
    ramp_match = _ramp_match;
}

#define CIRCLE_ENTRY_X 1550
#define CIRCLE_TOP_Y 486.25
#define CIRCLE_BOTTOM_Y 773.25
#define CIRCLE_X 1610
#define CIRCLE_Y 630
#define RADIUS 143.75

typedef struct euler_point { 
    double roll; 
    double pitch; 
    double yaw; 
} ;

struct euler_point euler_from_quaternion(double x, double y, double z, double w) {
    double sinr_cosp, cosr_cosp, sinp, siny_cosp, cosy_cosp;

    sinr_cosp = 2 * (w * x + y * z);
    cosr_cosp = 1 - 2 * (x * x + y * y);
    double roll = atan2(sinr_cosp, cosr_cosp);

    sinp = 2 * (w * y - z * x);
    double pitch = asin(sinp);
    
    siny_cosp = 2 * (w * z + x * y);
    cosy_cosp = 1 - 2 * (y * y + z * z);
    double yaw = atan2(siny_cosp, cosy_cosp);
    struct euler_point ret; 
    ret.roll = roll;
    ret.pitch = pitch; 
    ret.yaw = yaw; 
    return ret; 
}

void set_slam_orientation(double pitch) 
{

    g_mutex_lock(&gmutex);

    gettimeofday(&uwb_tv, NULL);

    uwb_angle  = pitch - 3.1415926535; 

    g_mutex_unlock(&gmutex);
    vt_heading = uwb_angle;

    isUpdate_uwb = TRUE;

    return; 
}

void set_slam_position(double _x, double _y) {
    g_mutex_lock(&gmutex);

    gettimeofday(&uwb_tv, NULL);

    uwb_x = _x;
    uwb_y = _y;

    position_offset_slam(&uwb_x, &uwb_y);
    g_mutex_unlock(&gmutex);

    vt_uwb_x = uwb_x; 
    vt_uwb_y = uwb_y; 
    if(use_traffic_light) {
        if((uwb_x > 1270 && uwb_x < 1400) && (uwb_y > 158 && uwb_y < 500)) {
            if(nego_state == NEGO_INIT) {
                set_nego_state(NEGO_WAIT_CARNIVAL);
            } else if(nego_state == NEGO_WAIT_IONIC) {
                set_nego_state(NEGO_WAIT);
            }
        // if((uwb_x > 978.92 && uwb_x < 1071.94) && (uwb_y > 317.71 && uwb_y < 516.36)) {
            if(cctv_on) {
                gtk_widget_hide(GTK_WIDGET(tlgImage_green_usd));
                gtk_widget_show(GTK_WIDGET(tlgImage_usd));
            } else if(carnival_on) {
                gtk_widget_hide(GTK_WIDGET(tlgImage_usd));
                gtk_widget_show(GTK_WIDGET(tlgImage_green_usd));
            } else {
                gtk_widget_hide(GTK_WIDGET(tlgImage_green_usd));
                gtk_widget_show(GTK_WIDGET(tlgImage_usd));
            }
        } else {
            if(nego_state == NEGO_ACTION_CARNIVAL) {
                set_nego_state(NEGO_FINISH);
            }
            carnival_on = false;
            gtk_widget_hide(GTK_WIDGET(tlgImage_usd));
            gtk_widget_hide(GTK_WIDGET(tlgImage_green_usd));
        }
    }
    isUpdate_uwb = TRUE; 
    return;
}

/*
void set_kanavi_msg(double in_out, double _x, double _y)
{    
    //[[ 0.09177623  4.40955783  3.63392082]
    //[ 4.23448731 -0.2350489  78.45529541]]
    /* SLAM 2 IPE Parmeters */
    /*
    double a = 0.09177623;
    double b = 4.40955783; 
    double c = 4.23448731;
    double d = -0.2350489 ; 

    double e = 3.63392082;
    double f = 78.45529541; 
    
  
    kanavi_inout = in_out;
    kanavi_x = a*(_x) + b*(_y) + e;  
    kanavi_y = c*(_x) + d*(_y) + f; 
    kanavi_heading = uwb_angle; 
    

    kanavi_inout = in_out; 
    kanavi_x = _x ; 
    kanavi_y = _y ; 
    kanavi_heading = uwb_angle;
    return;
} 
*/


double determine_indoor(double pose_x, double pose_y, double pose_z) {
  
    if (pose_z> 1.0) { // slam says it's outdoor
        //printf("%f : OUTDOOR!\n", pose_z); 
        
        kanavi_inout = 1.0 ; 
        
        return 1.0; 
        }
    else{ // slam says it's indoor 
        //printf("%f : INDOOR!\n", pose_z); 

        if (ai_inout == 1.0){ //ai says it's outdoor or ramp (24.11.05)
            kanavi_inout = 1.0; 
            return 1.0;
        }

        else if (ai_inout == 0.0){ //ai says it's ramp (24.11.05)

            if (vt_uwb_x > 1400 && vt_uwb_x <1855 && vt_uwb_y > 440 && vt_uwb_y<856){
                kanavi_inout = -1.0;
                return -1.0;
            }
            else{
                kanavi_inout = 1.0;  
                return 1.0;
            }
        }
        /*
        else if (vt_uwb_x > 200 && vt_uwb_x <1855 && vt_uwb_y > 440 && vt_uwb_y<856){
            
            kanavi_inout = -1.0; 
            return -1.0;
        }
        */
        
        else if (ai_inout == -1.0){ //ai says it's indoor
            kanavi_inout = -1.0;
            return -1.0 ;    
        }
        
        return -1.0; 
    }
}
double get_dist(double _ax, double _ay, double _bx, double _by)
{
    return sqrt(pow(_ax - _bx, 2) + pow(_ay - _by, 2));
}

void set_carnival_tl(bool _on) {
    carnival_on = _on;
}

void set_cctv_tl(bool _on) {
    cctv_on = _on;
}

void set_cctv(double _x, double _y)
{
    g_mutex_lock(&gmutex);

    gettimeofday(&cctv_tv, NULL);

    cctv_y = _y;
    cctv_x = _x;
    if(site == GLOBAL) {
        cctv_x += 2.55;
    }
    position_offset_cctv(&cctv_x, &cctv_y);
    
    isUpdate_cctv = TRUE;
    if(before_cctv_x < cctv_x) {
        cctv_x = before_cctv_x;
    }
    if(site == GLOBAL) {
        if(cctv_x > 150 && cctv_x < 440)
        {
            gtk_widget_show(GTK_WIDGET(gImage));
            gtk_fixed_move(gFixed, GTK_WIDGET(gImage), cctv_x - center_offset_y, 80 - center_offset_x);
        }
    } else if(site == ROBO) {
        if(nego_state == NEGO_INIT) {
            set_nego_state(NEGO_WAIT_IONIC);
        } else if(nego_state == NEGO_WAIT_CARNIVAL) {
            set_nego_state(NEGO_WAIT);
        }
        if(NEGO_INIT < nego_state && nego_state < NEGO_ACTION_CARNIVAL) {
            
            if(cctv_x > 1200.0 && cctv_x < 1550.0) {
                if(cctv_on) {
                    gtk_widget_hide(GTK_WIDGET(tlgImage_ccw));  
                    gtk_widget_show(GTK_WIDGET(tlgImage_green_ccw));
                } else {
                    gtk_widget_hide(GTK_WIDGET(tlgImage_green_ccw)); 
                    gtk_widget_show(GTK_WIDGET(tlgImage_ccw));
                }      
            } else {
                if(nego_state == NEGO_NEGOTIATION) {
                    printf("set_state : action_ionic\n");
                    set_nego_state(NEGO_ACTION_IONIC);
                    // send_message_to_server("ionic_pass", 10);
                }
                gtk_widget_hide(GTK_WIDGET(tlgImage_ccw));  
                gtk_widget_hide(GTK_WIDGET(tlgImage_green_ccw));  
            }
            gtk_widget_show(GTK_WIDGET(gImage));
            gtk_fixed_move(gFixed, GTK_WIDGET(gImage), cctv_x , 480 - center_offset_x);
        } else {
            gtk_widget_hide(GTK_WIDGET(gImage));
        }
    }

    before_cctv_x = cctv_x;
    g_mutex_unlock(&gmutex);
    cctv_angle = uwb_angle;
    isUpdate_uwb = TRUE;
}

void position_offset_slam(double *x, double *y){
    if(site == ROBO) {
        
        double a = 5.98594548e+01;
        double b = -4.00834940e-01;
        double c = 3.82208820e-01;
        double d = 6.16637281e+01;

        double e = 1.27574055e+03;
        //double f = 2.73099360e+02; 
        //double f = 2.83099360e+02; 
        double f = 2.78099360e+02; 

        double viewer_x = a*(*x) + b*(*y) + e ;
        double viewer_y = c*(*x) + d*(*y) + f ;
        /*
        tf_mat = 
            [[ 5.98594548e+01 -4.00834940e-01  1.27574055e+03]
        [ 3.82208820e-01  6.16637281e+01  2.73099360e+02]]
        */
        /*
        [[ 5.98594548e+01 -4.00834940e-01  1.27574055e+03]
        [ 3.82208820e-01  6.16637281e+01  2.83099360e+02]]
        */
        *x = viewer_x ; 
        *y = viewer_y ; 

    } 

}



gboolean on_draw(GtkWidget *widget,
                 GdkEventExpose *event,
                 gpointer data)
{
    if (first)
    {
        draw_window = gtk_widget_get_window(widget);
        cairoRegion = cairo_region_create();
        first = FALSE;
    }
    manually_draw();

    return FALSE;
}

gboolean manually_draw()
{

    g_mutex_lock(&gmutex);
    struct timeval current_tv;
    gettimeofday(&current_tv, NULL);
    double current_mstime, uwb_mstime, cctv_mstime;
    current_mstime = current_tv.tv_sec * 1000 + current_tv.tv_usec / 1000;
    uwb_mstime = uwb_tv.tv_sec * 1000 + uwb_tv.tv_usec / 1000;
    cctv_mstime = cctv_tv.tv_sec * 1000 + cctv_tv.tv_usec / 1000;
    
    GdkDrawingContext *drawingContext = gdk_window_begin_draw_frame(draw_window, cairoRegion);

    if (current_mstime - cctv_mstime >= 1200)
    {
        gtk_widget_hide(GTK_WIDGET(gImage));
        if(use_traffic_light) {
            gtk_widget_hide(GTK_WIDGET(tlgImage_green_ccw));
            gtk_widget_hide(GTK_WIDGET(tlgImage_ccw));
        
        }
        
    } else if(site == GLOBAL && !(cctv_x > 150.0 && cctv_x < 440.0)) {
        gtk_widget_hide(GTK_WIDGET(gImage));
        if(use_traffic_light) {
            gtk_widget_hide(GTK_WIDGET(tlgImage_green_ccw));
        }
    // } else if(site == ROBO && !(cctv_x > 1000.0 && cctv_x < 1550.0)) {
    } else if(site == ROBO && !(cctv_x > 785.73 && cctv_x < 1179.27)) {
        // gtk_widget_hide(GTK_WIDGET(gImage));
        if(use_traffic_light) {
            // gtk_widget_hide(GTK_WIDGET(tlgImage_green_ccw));
        }
    }

    if (current_mstime - uwb_mstime >= 0 && current_mstime - uwb_mstime < 1000)
    {
        if(site == GLOBAL) {
            if(uwb_x < 1450 && uwb_x > 0 && uwb_y > 0 && uwb_y < 1027) {
                draw_car(drawingContext, uwb_x, uwb_y, uwb_angle, FROM_UWB);
            }
        } else if(site == ROBO) {
            if(uwb_x < 1855 && uwb_x > 0 && uwb_y > 0 && uwb_y < 1027) {
            // if(uwb_x < 1397.5 && uwb_x > 70 && uwb_y > 205.15 && uwb_y < 936.8) {
                draw_car(drawingContext, uwb_x, uwb_y, uwb_angle, FROM_UWB);
                
                
            }
        }
    } 

    // say: "I'm finished drawing
    gdk_window_end_draw_frame(draw_window, drawingContext);
    
    g_mutex_unlock(&gmutex);

    return FALSE;
}

void rotate_point(double _angle, double *new_x, double *new_y)
{
    *new_x = carnival_center_offset_x * cos(_angle) - carnival_center_offset_y * sin(_angle);
    *new_y = carnival_center_offset_x * sin(_angle) + carnival_center_offset_y * cos(_angle);
}


void draw_car(GdkDrawingContext *_drawingContext, double _x, double _y, double _angle, enum PAINT _paint)
{
    double rotate_x, rotate_y;
    int w = cairo_image_surface_get_width(cst_uwb);
    int h = cairo_image_surface_get_height(cst_uwb);
    
    cairo_t *cr = gdk_drawing_context_get_cairo_context(_drawingContext);
    gtk_fixed_move(gFixed, GTK_WIDGET(drawingArea), _x - area_offset, _y - area_offset);

    rotate_point(_angle, &rotate_x, &rotate_y);
    cairo_translate(cr, (-1 * rotate_x) + area_offset, (-1 * rotate_y) + area_offset);
    cairo_rotate(cr, _angle);
    cairo_scale(cr, carnival2_width / w, carnival2_height / h);

    if(_paint == FROM_UWB) {
        cairo_set_source_surface(cr, cst_uwb, 30, 30);
    }
    
    isUpdate_uwb = FALSE;
    isUpdate_cctv = FALSE;
    
    cairo_paint(cr);
}

void position_offset_cctv(double *x, double *y)
{
    if(site == GLOBAL) {
        *x *= 20;
        *y *= 20;
        *x += 98;
    } else if(site == ROBO) {
#ifdef ADAPTIVE_SCALE
        if(*x <= 8.1 && *x >= 0) {
            *x *= 13.7;
            *x += 185;
        } else if(*x > 8.1 && *x <= 24.3) {
            *x -= 8.1;
            *x *= 13.95;
            *x += 296;
        } else if(*x > 24.3 && *x <= 32.4) {
            *x -= 24.3;
            *x *= 13.82;
            *x += 522;
        } else if(*x > 32.4 && *x <= 48.6) {
            *x -= 32.4;
            *x *= 13.95;
            *x += 634;
        } else if(*x > 48.6 && *x <= 56.7) {
            *x -= 48.6;
            *x *= 13.82;
            *x += 860;
        } else if(*x > 56.7 && *x <= 64.8) {
            *x -= 56.7;
            *x *= 13.95;
            *x += 972;
        } else if(*x > 64.8 && *x <= 75.6) {
            *x -= 64.8;
            *x *= 13.61;
            *x += 1085;
        } else if(*x > 75.6 && *x <= 83.7) {
            *x -= 75.6;
            *x *= 14.44;
            *x += 1232;
        } else if(*x > 83.7 && *x <= 89.1) {
            *x -= 83.7;
            *x *= 12.96;
            *x += 1349;
        } else if(*x > 89.1 && *x <= 111.25) {
            *x -= 89.1;
            *x *= 14.45;
            *x += 1419;
        } else {
            *x *= 13.88;
            *x += 185;
        }




        if(*y <= 7.5 && *y >= 0) {
            *y *= 12.93;
            *y += 233;
        } else if(*y > 7.5 && *y <= 15.6) {
            *y -= 7.5;
            *y *= 14.94;
            *y += 330;
        } else if(*y > 15.6 && *y <= 22.6) {
            *y -= 15.6;
            *y *= 14.66;
            *y += 451;
        } else if(*y > 22.6 && *y <= 27.7) {
            *y -= 22.6;
            *y *= 12.94;
            *y += 553;
        } else if(*y > 27.7 && *y <= 35.8) {
            *y -= 27.7;
            *y *= 13.95;
            *y += 619;
        } else if(*y > 35.8 && *y <= 43.9) {
            *y -= 35.8;
            *y *= 14.07;
            *y += 731;
        } else {
            *y *= 13.79;
            *y += 233;
        }
#else
        *x *= 11.52;
        *y *= 11.47;
        *x += 116;
        *y += 317;
        // *x *= 16.1;
        // *y *= 16.1;
        // *x += 64;
        // *y += 8;
        #endif
    }

}



int map_init(int argc, char **argv)
{
    gtk_init(&argc, &argv);

    g_mutex_init(&gmutex);

    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(cssProvider, "/home/keti/catkin_ws/src/viewer_slam/src/style.css", NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);


    if(site == ROBO) {
        gBuilder = gtk_builder_new_from_resource("/glade/robo_parking_map.glade");
    } else if(site == GLOBAL) {
        gBuilder = gtk_builder_new_from_resource("/glade/parking_map.glade");
    }

    area_offset = max(carnival2_width, carnival2_height);
    carnival_center_offset_x = carnival2_width / 2.0;
    carnival_center_offset_y = carnival2_height / 2.0;
    center_offset_x = ioniq_width / 2.0;
    center_offset_y = ioniq_height / 2.0;
    cst_uwb = cairo_image_surface_create_from_png("/home/keti/catkin_ws/src/viewer_slam/resource/carnival2.png");
    
    gApp = GTK_WIDGET(gtk_builder_get_object(gBuilder, "ID_Window"));
    g_signal_connect(gApp, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_builder_connect_signals(gBuilder, NULL);
    gFixed = GTK_FIXED(gtk_builder_get_object(gBuilder, "ID_FIXED"));
    drawingArea = GTK_DRAWING_AREA(gtk_builder_get_object(gBuilder, "ID_DRAWINGAREA"));
    
    gtk_widget_set_size_request(GTK_WIDGET(drawingArea), 200, 200);

    trafficlight_init();
    set_nego();
    cctvPixbuf = gdk_pixbuf_new_from_resource_at_scale("/glade/cctv.png", carnival2_width, carnival2_height, TRUE, NULL);
    carnivalPixbuf = gdk_pixbuf_new_from_resource_at_scale("/glade/carnival2.png", carnival2_width, carnival2_height, TRUE, NULL);
    pixbuf = gdk_pixbuf_new_from_resource_at_scale("/glade/ioniq_elec_02.png", ioniq_width, ioniq_height, TRUE, NULL);
    carA = GTK_IMAGE(gtk_image_new_from_pixbuf(carnivalPixbuf));
    carB = GTK_IMAGE(gtk_image_new_from_pixbuf(pixbuf));
    cctvImage = GTK_IMAGE(gtk_image_new_from_pixbuf(cctvPixbuf));
    pixbuf = gdk_pixbuf_rotate_simple(pixbuf, GDK_PIXBUF_ROTATE_COUNTERCLOCKWISE);
    gImage = GTK_IMAGE(gtk_image_new_from_pixbuf(pixbuf));
    gtk_fixed_put(gFixed, GTK_WIDGET(gImage), 100 - center_offset_y, 98 - center_offset_x);
    gtk_fixed_put(gFixed, GTK_WIDGET(carA), 780, 50);
    gtk_fixed_put(gFixed, GTK_WIDGET(carB), 960, 37);
    gtk_fixed_put(gFixed, GTK_WIDGET(cctvImage), 950, 470);
    
    carALabel = gtk_label_new("A 차량:      ");
    gtk_fixed_put(gFixed, carALabel, 700, 50);
    gtk_widget_show(carALabel);

    carBLabel = gtk_label_new("B 차량:      "); 
    gtk_fixed_put(gFixed, carBLabel, 900, 50);
    gtk_widget_show(carBLabel);


    draw_rect_on_fixed(gFixed, 1150, 390);
    gtk_widget_show(gApp);
    gtk_widget_show(GTK_WIDGET(gImage));
    gtk_widget_show(GTK_WIDGET(cctvImage));
    gtk_widget_show(GTK_WIDGET(carA));
    gtk_widget_show(GTK_WIDGET(carB));
    gtk_widget_show(GTK_WIDGET(drawingArea));
    gtk_widget_hide(GTK_WIDGET(gImage));

    g_thread_new("ros_thread", spinfor, NULL);
    g_thread_new("kanavi_thread", sendto_kanavi, NULL);
    if(use_cctv) {
        g_thread_new("get_cctv", rx_cctv, NULL);
    }
    g_timeout_add(10, callback_timer, NULL);

    gtk_main();



    return EXIT_SUCCESS;
}


//----------------------------------------------------------------------------------------------------------

static gboolean draw_rect_callback(GtkWidget *widget, cairo_t *cr, gpointer data)
{
    cairo_set_line_width(cr, 10);
    cairo_set_source_rgb(cr, 1, 0, 0);

    double dashes[] = {15.0, 15.0};
    cairo_set_dash(cr, dashes, 2, 0);

    cairo_rectangle(cr, 0, 0, 275, 225);
    cairo_stroke(cr);
    // cairo_fill(cr);
    return FALSE;

}

void draw_rect_on_fixed(GtkFixed *fixed, double x, double y)
{
    GtkWidget *drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(drawing_area, 275, 225);
    gtk_fixed_put(fixed, drawing_area, x, y);
    g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(draw_rect_callback), NULL);
    gtk_widget_show(drawing_area);
}

static gboolean btn_click_callback(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
    
    return FALSE;

}

void set_nego()
{
    if(!use_traffic_light) {
        printf("traffic_light false\n");
        return;
    }

    gNegoBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_vexpand(gNegoBox, true);

    gNegoLabel = gtk_label_new("정지");
    gtk_widget_show(gNegoLabel);
    gtk_widget_hide(gNegoLabel);

    gNegoLabel2 = gtk_label_new("정지2");
    // GtkStyleContext *context = gtk_widget_get_style_context(gNegoLabel2);
    // gtk_style_context_add_class(context, "label2");
    gtk_widget_show(gNegoLabel2);
    gtk_widget_hide(gNegoLabel2);

    // gNegoButton = gtk_button_new_with_label("주행협상 시작");

    // g_signal_connect(G_OBJECT(gNegoButton), "clicked", G_CALLBACK(btn_click_callback), NULL);
    // gtk_widget_show(gNegoButton);


    gtk_container_add(GTK_CONTAINER(gNegoBox), gNegoLabel);
    gtk_container_add(GTK_CONTAINER(gNegoBox), gNegoLabel2);
    // gtk_container_add(GTK_CONTAINER(gNegoBox), gNegoButton);
    gtk_widget_set_size_request(GTK_WIDGET(gNegoBox), 350, 200);
    gtk_fixed_put(gFixed, gNegoBox, 1460, 280);
    // gtk_container_set_border_width(GTK_CONTAINER(gNegoBox), 100);
    gtk_widget_show(gNegoBox);


}
void set_nego_state(int state) 
{
    nego_state = state;
    if(nego_state == NEGO_WAIT_CARNIVAL) {
        set_nego_label("A 차량 정지");
    } else if(nego_state == NEGO_WAIT_IONIC) {
        set_nego_label2("B 차량 정지");
    } else if(nego_state == NEGO_WAIT) {
        set_nego_label("A 차량 주행의도(좌회전) 전송");
        set_nego_label2("B 차량 주행의도(직진) 전송");
        set_change_state("MANEUVER", 8, 1);
    } else if(nego_state == NEGO_MANEUVER) {
        set_nego_label("주행협상 중");
        set_nego_label2("");
    } else if(nego_state == NEGO_NEGOTIATION) {
        set_nego_label("A 차량 주행협상 결과: 정지");
        set_nego_label2("B 차량 주행협상 결과: 직진");
        set_cctv_tl(true);
    } else if(nego_state == NEGO_ACTION_IONIC) {
        set_send_message("ionic_pass", 10, 2);
    } else if(nego_state == NEGO_ACTION_CARNIVAL) {
        set_nego_label("A 차량 주행협상 결과: 좌회전");
        set_nego_label2("");
        set_carnival_tl(true);
        set_cctv_tl(false);
    } else if(nego_state == NEGO_FINISH) {
        set_send_message("carnival_pass", 13, 2);
    } else if(nego_state == NEGO_END) {
        set_nego_label("서비스 종료");
        before_cctv_x = 1000000.0;
    }
} 
void set_nego_label(char *text) 
{
    gtk_label_set_text(gNegoLabel, text);
    gtk_widget_show(gNegoLabel);
} 
void set_nego_label2(char *text) 
{
    if(strcmp(text, "") == 0) {
        gtk_widget_hide(gNegoLabel2);
    } else {
        gtk_label_set_text(gNegoLabel2, text);
        gtk_widget_show(gNegoLabel2);
    }
} 
void set_nego_button_text(char *text) {
    gtk_button_set_label(GTK_BUTTON(gNegoButton), text);
}