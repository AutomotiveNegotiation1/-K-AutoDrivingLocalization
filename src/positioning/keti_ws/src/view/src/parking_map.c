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
#define TRAFFICLIGHT_TEST

#define CIRCLE_ENTRY_X 1550
#define CIRCLE_TOP_Y 486.25
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
void position_offset_uwb(double *x, double *y);
void position_offset_cctv(double *x, double *y);
gint callback_timer(gpointer argv);
void rotate_point(double _angle, double *new_x, double *new_y);
gboolean manually_draw();
gboolean on_draw(GtkWidget *widget, GdkEventExpose *event, gpointer data);


void trafficlight_init()
{
    if(!use_traffic_light) {
        printf("traffic_light false\n");
        return;
    }
    
    GdkPixbuf *tlpixbuf = gdk_pixbuf_new_from_resource_at_scale("/glade/draw_tl_red.png", 72, 18, TRUE, NULL);
    GdkPixbuf *tlpixbuf_green = gdk_pixbuf_new_from_resource_at_scale("/glade/draw_tl_up.png", 72, 18, TRUE, NULL);
    GdkPixbuf *tlpixbuf_left = gdk_pixbuf_new_from_resource_at_scale("/glade/draw_tl_left.png", 72, 18, TRUE, NULL);
    // GdkPixbuf *tlpixbuf = gdk_pixbuf_new_from_resource_at_scale("/glade/draw_tl_red.png", 100, 25, TRUE, NULL);
    // GdkPixbuf *tlpixbuf_green = gdk_pixbuf_new_from_resource_at_scale("/glade/draw_tl_up.png", 100, 25, TRUE, NULL);
    // GdkPixbuf *tlpixbuf_left = gdk_pixbuf_new_from_resource_at_scale("/glade/draw_tl_left.png", 100, 25, TRUE, NULL);
    
    GdkPixbuf *tlpixbuf_usd = gdk_pixbuf_rotate_simple(tlpixbuf, GDK_PIXBUF_ROTATE_UPSIDEDOWN);
    GdkPixbuf *tlpixbuf_green_usd = gdk_pixbuf_rotate_simple(tlpixbuf_left, GDK_PIXBUF_ROTATE_UPSIDEDOWN);
    GdkPixbuf *tlpixbuf_ccw = gdk_pixbuf_rotate_simple(tlpixbuf, GDK_PIXBUF_ROTATE_COUNTERCLOCKWISE);
    GdkPixbuf *tlpixbuf_green_ccw = gdk_pixbuf_rotate_simple(tlpixbuf_green, GDK_PIXBUF_ROTATE_COUNTERCLOCKWISE);


    tlgImage_usd = gtk_image_new_from_pixbuf(tlpixbuf_usd);
    tlgImage_ccw = gtk_image_new_from_pixbuf(tlpixbuf_ccw);
    tlgImage_green_usd = gtk_image_new_from_pixbuf(tlpixbuf_green_usd);
    tlgImage_green_ccw = gtk_image_new_from_pixbuf(tlpixbuf_green_ccw);


    gtk_fixed_put(gFixed, tlgImage_usd, 1007, 604);
    gtk_fixed_put(gFixed, tlgImage_green_usd, 1007, 604);
    gtk_fixed_put(gFixed, tlgImage_ccw, 936, 510);
    gtk_fixed_put(gFixed, tlgImage_green_ccw, 936, 510);
    // gtk_fixed_put(gFixed, tlgImage_usd, 1350 - 50, 570 - 10);
    // gtk_fixed_put(gFixed, tlgImage_green_usd, 1350 - 50, 570 - 10);
    // gtk_fixed_put(gFixed, tlgImage_ccw, 1210, 420);
    // gtk_fixed_put(gFixed, tlgImage_green_ccw, 1210, 420);
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
void set_lidar(double _x, double _y, double _angle)
{
    g_mutex_lock(&gmutex);

    gettimeofday(&uwb_tv, NULL);

    uwb_y = _x;
    uwb_x = _y;
    uwb_angle  = _angle+(3.141592653589793/2);

    position_offset_lidar(&uwb_x, &uwb_y);
    g_mutex_unlock(&gmutex);
    
    isUpdate_uwb = TRUE;
}
#define CIRCLE_ENTRY_X 1550
#define CIRCLE_TOP_Y 486.25
#define CIRCLE_BOTTOM_Y 773.25
#define CIRCLE_X 1610
#define CIRCLE_Y 630
#define RADIUS 143.75
void set_uwb(double _x, double _y, double _angle)
{
    g_mutex_lock(&gmutex);

    gettimeofday(&uwb_tv, NULL);

    uwb_y = _x;
    uwb_x = _y;
    uwb_angle  = _angle+(G_PI/2);

    position_offset_uwb(&uwb_x, &uwb_y);
    g_mutex_unlock(&gmutex);


    if(ramp_match) {
        double angle = fmod(uwb_angle, G_PI * 2);
        // printf("angle : %f\n", angle);
        if(ramp_state == IN_RAMP_TOP) {
            uwb_y = CIRCLE_TOP_Y;
            // printf("before entry -> uwb_x : %f\n", uwb_x);
            // if(angle < G_PI / 2.0) {
            if(uwb_x < CIRCLE_X) {
            } else {
                ramp_state = IN_RAMP_CIRCLE;
            }
        } else if(ramp_state == IN_RAMP_CIRCLE) {
            if(angle >= (G_PI * 3.0 / 2.0)) {
                ramp_state = IN_RAMP_BOTTOM;
                uwb_y = CIRCLE_BOTTOM_Y;
            } else {
                double theta = G_PI - angle;
                uwb_x = cos(theta) * RADIUS + CIRCLE_X;
                uwb_y = CIRCLE_Y - sin(theta) * RADIUS;
                printf("in circle -> uwb_x : %f, theta : %f\n", uwb_x, theta);
            }
        } else if(ramp_state == IN_RAMP_BOTTOM) {
            uwb_y = CIRCLE_BOTTOM_Y;
            // printf("after entry -> uwb_x : %f\n", uwb_x);
            if(uwb_x > CIRCLE_ENTRY_X) {
            } else {
                // ramp_state = RAMP_FINISH;
            }
        } else if(ramp_state == NOT_IN_RAMP) {
            if(uwb_y > 400 & uwb_y < 530 & uwb_x > 1545.0 & uwb_x < 1550.0) {
                if(angle < 0) {
                    angle += 3.141592653589793;
                }
                if(angle > 1.0 && angle < 2.0) {
                    ramp_state = IN_RAMP_TOP;
                }

            }
        }


    }
    isUpdate_uwb = TRUE;
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
    
    
#ifdef TRAFFICLIGHT_TEST
    cctv_x += 107;
    // cctv_x += 150;
#endif
    if(site == GLOBAL) {
        if(cctv_x > 150 && cctv_x < 440)
        {
            gtk_widget_show(GTK_WIDGET(gImage));
            gtk_fixed_move(gFixed, GTK_WIDGET(gImage), cctv_x - center_offset_y, 80 - center_offset_x);
        }
    } else if(site == ROBO) {
        // printf("cctv : %f\n", cctv_x);
        // if(cctv_x > 1000.0 && cctv_x < 1550.0) {
        if(cctv_x > 785.73 && cctv_x < 1179.27) {
            // if(cctv_x < 1240.0) {
            if(cctv_x < 957.46) {
                gtk_widget_hide(GTK_WIDGET(tlgImage_green_ccw));
                cctv_on = FALSE;
            } else {
                gtk_widget_show(GTK_WIDGET(tlgImage_green_ccw));
                cctv_on = TRUE;
            }
            gtk_widget_show(GTK_WIDGET(gImage));
            // gtk_fixed_move(gFixed, GTK_WIDGET(gImage), cctv_x - center_offset_x, 470 - center_offset_y);
            gtk_fixed_move(gFixed, GTK_WIDGET(gImage), cctv_x - center_offset_x, 540 - center_offset_y);
        }
    }

    g_mutex_unlock(&gmutex);
    cctv_angle = uwb_angle;
    isUpdate_uwb = TRUE;
}
void position_offset_lidar(double *x, double *y)
{
    if(site == GLOBAL) {
        *x *= 20;
        *y *= 20;
        // TODO
    } else if(site == ROBO) {
        *x *= 16.1;
        *y *= 16.1;
        // TODO
    }
}
void position_offset_uwb(double *x, double *y)
{
    if(site == GLOBAL) {
        *x *= 20;
        *y *= 20;
        *x += 98;
    } else if(site == ROBO) {
        *x *= 11.52;
        *y *= 11.47;
        *x += 116;
        *y += 317;
        // *x *= 16.1;
        // *y *= 16.1;
        // *x += 64;
        // *y += 157;
    }
}

void position_offset_cctv(double *x, double *y)
{
    if(site == GLOBAL) {
        *x *= 20;
        *y *= 20;
        *x += 98;
    } else if(site == ROBO) {
        *x *= 11.52;
        *y *= 11.47;
        *x += 116;
        *y += 317;
        // *x *= 16.1;
        // *y *= 16.1;
        // *x += 64;
        // *y += 8;
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
            // if(uwb_x < 1855 && uwb_x > 0 && uwb_y > 0 && uwb_y < 1027) {
            if(uwb_x < 1397.5 && uwb_x > 70 && uwb_y > 205.15 && uwb_y < 936.8) {
                draw_car(drawingContext, uwb_x, uwb_y, uwb_angle, FROM_UWB);
                // if((uwb_x > 1270 && uwb_x < 1400) && (uwb_y > 158 && uwb_y < 500)) {
                if((uwb_x > 978.92 && uwb_x < 1071.94) && (uwb_y > 317.71 && uwb_y < 516.36)) {
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
    cst_uwb = cairo_image_surface_create_from_png("/home/keti/Documents/git/workspace/yeta1/catkin_ws/src/view/src/carnival2.png");
    // cst_cctv = cairo_image_surface_create_from_png("/home/keti/Documents/JW/-K-AutoDrivingLocalization/src/positioning/keti_ws/src/view/src/yellow_car.png");
    
    gApp = GTK_WIDGET(gtk_builder_get_object(gBuilder, "ID_Window"));
    g_signal_connect(gApp, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_builder_connect_signals(gBuilder, NULL);
    gFixed = GTK_FIXED(gtk_builder_get_object(gBuilder, "ID_FIXED"));
    drawingArea = GTK_DRAWING_AREA(gtk_builder_get_object(gBuilder, "ID_DRAWINGAREA"));
    
    gtk_widget_set_size_request(GTK_WIDGET(drawingArea), 200, 200);

    trafficlight_init();
    
    pixbuf = gdk_pixbuf_new_from_resource_at_scale("/glade/ioniq_elec_02.png", ioniq_width, ioniq_height, TRUE, NULL);
    
    pixbuf = gdk_pixbuf_rotate_simple(pixbuf, GDK_PIXBUF_ROTATE_COUNTERCLOCKWISE);
    gImage = GTK_IMAGE(gtk_image_new_from_pixbuf(pixbuf));
    gtk_fixed_put(gFixed, GTK_WIDGET(gImage), 100 - center_offset_y, 98 - center_offset_x);
    
    
    gtk_widget_show(gApp);
    gtk_widget_show(GTK_WIDGET(gImage));
    gtk_widget_show(GTK_WIDGET(drawingArea));
    gtk_widget_hide(GTK_WIDGET(gImage));

    g_thread_new("ros_thread", spinfor, NULL);
    if(use_cctv) {
        g_thread_new("get_cctv", rx_cctv, NULL);
    }
    g_timeout_add(10, callback_timer, NULL);

    gtk_main();



    return EXIT_SUCCESS;
}