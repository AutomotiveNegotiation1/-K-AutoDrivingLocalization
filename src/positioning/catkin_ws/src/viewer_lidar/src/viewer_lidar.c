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
#include "viewer_lidar.h"
#include "viewer_lidar_main.h"
#include <math.h>

#define TRAFFICLIGHT_TEST

/*
#define CIRCLE_ENTRY_X 1550
#define CIRCLE_TOP_Y 486.25
#define CIRCLE_BOTTOM_Y 773.25
#define CIRCLE_X 1610
#define CIRCLE_Y 630
#define RADIUS 143.75
*/
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

// get object from glade
GtkBuilder *gBuilder;
GtkWidget *gApp;
GtkFixed *gFixed;
GtkDrawingArea *drawingArea;
GdkPixbuf *pixbuf;
GtkImage *gImage;


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
bool cctv_on = FALSE;

GMutex gmutex;

double uwb_x = 100.0;
double uwb_y = 100.0;
double uwb_angle = 0;
double cctv_x = 100.0;
double cctv_y = 100.0;
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

struct timeval uwb_tv, cctv_tv;

void draw_car(GdkDrawingContext *_drawingContext, double _x, double _y, double _angle, enum PAINT _paint);
void position_offset_lidar(double *x, double *y);
gint callback_timer(gpointer argv);
void rotate_point(double _angle, double *new_x, double *new_y);
gboolean manually_draw();
gboolean on_draw(GtkWidget *widget, GdkEventExpose *event, gpointer data);


gint callback_timer(gpointer argv)
{
    gint x, y;
    gtk_widget_get_pointer(gApp, &x, &y);
    gtk_widget_queue_draw(GTK_WIDGET(drawingArea));
    
    //printf("mouse position :: x = %d, y = %d\n", x, y);

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
void set_lidar(double _x, double _y, double _angle)
{
    g_mutex_lock(&gmutex);

    gettimeofday(&uwb_tv, NULL);

    uwb_y = _x;
    uwb_x = _y;
    uwb_angle  = _angle + (G_PI);
    // printf("x : %f, y : %f, angle : %f\n", _x, _y, _angle); 
    position_offset_lidar(&uwb_x, &uwb_y);
    g_mutex_unlock(&gmutex);
    
    isUpdate_uwb = TRUE;
}

void position_offset_lidar(double *x, double *y)
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
        *x *= 14.25;
        *y *= 13.8;
        *x += 190; 
        *y += 233; //317 ;
#endif        
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

    if (current_mstime - cctv_mstime >= 1000)
    {
        gtk_widget_hide(GTK_WIDGET(gImage));
        if(use_traffic_light) {
            gtk_widget_hide(GTK_WIDGET(tlgImage_green_ccw));
        
        }
        
    } else if(site == GLOBAL && !(cctv_x > 150.0 && cctv_x < 440.0)) {
        gtk_widget_hide(GTK_WIDGET(gImage));
        if(use_traffic_light) {
            gtk_widget_hide(GTK_WIDGET(tlgImage_green_ccw));
        }
    // } else if(site == ROBO && !(cctv_x > 1000.0 && cctv_x < 1550.0)) {
    } else if(site == ROBO && !(cctv_x > 785.73 && cctv_x < 1179.27)) {
        gtk_widget_hide(GTK_WIDGET(gImage));
        if(use_traffic_light) {
            gtk_widget_hide(GTK_WIDGET(tlgImage_green_ccw));
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
                draw_car(drawingContext, uwb_x, uwb_y, uwb_angle, FROM_UWB);
                if(use_traffic_light) {
                    if((uwb_x > 1270 && uwb_x < 1400) && (uwb_y > 158 && uwb_y < 500)) {
                        if(cctv_on) {
                            gtk_widget_hide(GTK_WIDGET(tlgImage_green_usd));
                            gtk_widget_show(GTK_WIDGET(tlgImage_usd));
                        } else {
                            gtk_widget_hide(GTK_WIDGET(tlgImage_usd));
                            gtk_widget_show(GTK_WIDGET(tlgImage_green_usd));
                        }
                    } else {
                        gtk_widget_hide(GTK_WIDGET(tlgImage_usd));
                        gtk_widget_hide(GTK_WIDGET(tlgImage_green_usd));
                    }
                }
                
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
    
    draw_point_on_fixed(gFixed, _x, _y);
    cairo_paint(cr);
}


static gboolean draw_circle_callback(GtkWidget *widget, cairo_t *cr, gpointer data)
{
    cairo_set_line_width(cr, 2);
    cairo_set_source_rgb(cr, 1, 0, 0);

    cairo_arc(cr, 5, 5, 2, 0, 2*G_PI);
    cairo_fill(cr);
    return FALSE;
}

void draw_point_on_fixed(GtkFixed *fixed, double x, double y)
{
    GtkWidget *drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(drawing_area, 10, 10);
    gtk_fixed_put(fixed, drawing_area, x, y);

    g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(draw_circle_callback), NULL);
    gtk_widget_show(drawing_area);
}




int map_init(int argc, char **argv)
{
    gtk_init(&argc, &argv);

    g_mutex_init(&gmutex);

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
    cst_uwb = cairo_image_surface_create_from_png("/home/lab1/Documents/git/workspace/yeta1/catkin_ws/src/view/resource/carnival2.png");
    // cst_cctv = cairo_image_surface_create_from_png("/home/keti/Documents/JW/-K-AutoDrivingLocalization/src/positioning/keti_ws/src/view/src/yellow_car.png");
    
    gApp = GTK_WIDGET(gtk_builder_get_object(gBuilder, "ID_Window"));
    g_signal_connect(gApp, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_builder_connect_signals(gBuilder, NULL);
    gFixed = GTK_FIXED(gtk_builder_get_object(gBuilder, "ID_FIXED"));
    drawingArea = GTK_DRAWING_AREA(gtk_builder_get_object(gBuilder, "ID_DRAWINGAREA"));
    
    gtk_widget_set_size_request(GTK_WIDGET(drawingArea), 200, 200);

    
    pixbuf = gdk_pixbuf_new_from_resource_at_scale("/glade/ioniq_elec_02.png", ioniq_width, ioniq_height, TRUE, NULL);
    
    pixbuf = gdk_pixbuf_rotate_simple(pixbuf, GDK_PIXBUF_ROTATE_COUNTERCLOCKWISE);
    gImage = GTK_IMAGE(gtk_image_new_from_pixbuf(pixbuf));
    gtk_fixed_put(gFixed, GTK_WIDGET(gImage), 100 - center_offset_y, 98 - center_offset_x);
    
    
    gtk_widget_show(gApp);
    gtk_widget_show(GTK_WIDGET(gImage));
    gtk_widget_show(GTK_WIDGET(drawingArea));
    gtk_widget_hide(GTK_WIDGET(gImage));

    g_thread_new("ros_thread", spinfor, NULL);

    g_timeout_add(10, callback_timer, NULL);

    gtk_main();



    return EXIT_SUCCESS;
}
