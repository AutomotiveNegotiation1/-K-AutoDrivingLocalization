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
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cairo.h>

#define max(a, b) a > b ? a : b
#define ROTATION_WINDOW 2

enum PAINT
{
    FROM_UWB,
    FROM_CCTV
};

struct POINT
{
    double x;
    double y;
    struct POINT *prev;
    struct POINT *next;
};

// get object from glade
GtkBuilder *gBuilder;
GtkWidget *gApp;
GtkFixed *gFixed;
GtkDrawingArea *drawingArea;
GtkWidget *gimage;
GtkWidget *gimage2;

cairo_surface_t *cst_uwb;
cairo_surface_t *cst_cctv;
GdkWindow *draw_window;
cairo_region_t *cairoRegion;
bool first = TRUE;
GdkDrawingContext *drawingContext;

pthread_t uwb_thread;
double uwb_x = 100.0;
double uwb_y = 100.0;
double uwb_angle = 0;
enum PAINT cur_paint = FROM_UWB;
bool isUpdate = FALSE;
bool isStart = FALSE;

double current_x;
double current_y;
double current_angle;
enum PAINT current_paint;

// origin => 2519, 3984
//  target_zoom
double car_width = 40.0;
double car_height = 60.0;

double area_offset;
double center_offset_x;
double center_offset_y;
int change_cnt = 0;

struct POINT *point_list = NULL;
int list_cnt = 0;
struct timeval uwb_tv, cctv_tv;

void draw_car(GdkDrawingContext *_drawingContext, double _x, double _y, double _angle, enum PAINT _paint);
void position_offset_uwb(double *x, double *y);
void position_offset_cctv(double *x, double *y);
gint callback_timer(gpointer argv);
int new_socket();
gpointer uwb_thread_run(gpointer data);
void rotate_point(double _angle, double *new_x, double *new_y);
gboolean manually_draw();
gboolean on_draw(GtkWidget *widget, GdkEventExpose *event, gpointer data);
// gboolean on_draw(GtkWidget *widget, cairo_t *cr, gpointer data);

void draw_point_on_fixed(GtkFixed *fixed, double x, double y);
static gboolean draw_circle_callback(GtkWidget *widget, cairo_t *cr, gpointer data);

GdkPixbuf *rotate_pixbuf_func(GdkPixbuf *src_pixbuf, gdouble angle)
{
    gint src_width, src_height;
    src_width = gdk_pixbuf_get_width(src_pixbuf);
    src_height = gdk_pixbuf_get_height(src_pixbuf);
    GdkPixbuf *dest_pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, TRUE, 8, src_width, src_height);

    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, src_width, src_height);
    cairo_t *cr = cairo_create(surface);
    gdk_cairo_set_source_pixbuf(cr, src_pixbuf, 0, 0);

    cairo_translate(cr, src_width / 2.0, src_height / 2.0);
    cairo_rotate(cr, angle * G_PI / 180.0);
    cairo_translate(cr, -src_width / 2.0, -src_height / 2.0);

    cairo_paint(cr);

    cairo_destroy(cr);
    cairo_surface_destroy(surface);

    return dest_pixbuf;
}


void set_car(GtkWidget *image, gdouble angle)
{
    GdkPixbuf *pixbuf = gtk_image_get_pixbuf(GTK_IMAGE(image));
    // gint src_width, src_height;
    // src_width = gdk_pixbuf_get_width(pixbuf);
    // src_height = gdk_pixbuf_get_height(pixbuf);
    // GdkPixbuf *pixbuf2 = gdk_pixbuf_new(GDK_COLORSPACE_RGB, TRUE, 8, src_width, src_height);
    // GdkPixbuf *rotate_pixbuf = gdk_pixbuf_rotate_simple(pixbuf, angle);
    GdkPixbuf *rotate_pixbuf = rotate_pixbuf_func(pixbuf, angle);
    // gtk_image_set_from_pixbuf(GTK_IMAGE(image), rotate_pixbuf);
    // GdkPixbuf *dest_pixbuf = ;
    // gdk_pixbuf_scale(pixbuf, pixbuf2, 100, 100, 50, 50, 0, 0, 1, 1, 0);
    // gtk_image_set_from_pixbuf(GTK_IMAGE(image), pixbuf2);
    


    // g_object_unref(rotate_pixbuf);
}

int main(int argc, char *argv[])
{
    point_list = (struct POINT *)malloc(sizeof(struct POINT));
    point_list->prev = point_list;
    point_list->next = point_list;
    area_offset = max(car_width, car_height);
    center_offset_x = car_width / 2.0;
    center_offset_y = car_height / 2.0;
    cst_uwb = cairo_image_surface_create_from_png("car.png");
    cst_cctv = cairo_image_surface_create_from_png("yellow_car.png");
    gtk_init(&argc, &argv);
    gBuilder = gtk_builder_new_from_resource("/glade/robo_parking_map.glade");
    //gBuilder = gtk_builder_new_from_file("parking_map.glade");

    gApp = GTK_WIDGET(gtk_builder_get_object(gBuilder, "ID_Window"));
    g_signal_connect(gApp, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_builder_connect_signals(gBuilder, NULL);
    gFixed = GTK_FIXED(gtk_builder_get_object(gBuilder, "ID_FIXED"));
    drawingArea = GTK_DRAWING_AREA(gtk_builder_get_object(gBuilder, "ID_DRAWINGAREA"));
    
    gimage = gtk_image_new_from_file("car.png");
    gimage2 = gtk_image_new_from_file("yellow_car.png");
    // gtk_container_add(GTK_CONTAINER(gFixed), gimage);
    
    // set_car(gimage, 90);
    gtk_widget_set_size_request(GTK_WIDGET(drawingArea), 200, 200);
    // gtk_widget_set_double_buffered(GTK_WIDGET(drawingArea), FALSE);
    // bool db = gtk_widget_get_double_buffered(GTK_WIDGET(drawingArea));
    // if(db) {
    //     printf("doublebuffered : true\n");
    
    // } else {

    //     printf("doublebuffered : false\n");
    // }
    
    gtk_widget_show_all(gApp);
    // gtk_widget_show(gApp);
    gtk_widget_show(GTK_WIDGET(drawingArea));
    g_thread_new("uwb_thread", uwb_thread_run, NULL);

    g_timeout_add(10, callback_timer, NULL);

    gtk_main();
    free(point_list);
    return EXIT_SUCCESS;
}

gint callback_timer(gpointer argv)
{
    gint x, y;
        // gtk_widget_queue_draw(GTK_WIDGET(drawingArea));
    // gtk_widget_get_pointer(gApp, &x, &y);
    // printf("mouse position :: x = %d, y = %d\n", x, y);
    return TRUE;
}

int new_socket()
{
    int newSock = socket(PF_INET, SOCK_DGRAM, 0);
    if (newSock == -1)
    {
        printf("Socket create fail");
    }
    return newSock;
}

gpointer uwb_thread_run(gpointer data)
{
    char buff[1024];
    int rx_leng;
    int sock = new_socket();
    struct sockaddr_in from_adr;
    socklen_t adr_sz;
    char *eptr;

    memset(&from_adr, 0, sizeof(from_adr));
    from_adr.sin_family = AF_INET;
    from_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    from_adr.sin_port = htons(5000);
    if (bind(sock, (struct sockaddr *)&from_adr, sizeof(from_adr)) == -1)
    {
        perror("bind failed");
        exit(1);
    }
    while (1)
    {
        adr_sz = sizeof(from_adr);
        rx_leng = recvfrom(sock, buff, 1024, 0, (struct sockaddr *)&from_adr, &adr_sz);
        gettimeofday(&uwb_tv, NULL);
        buff[rx_leng] = 0;
        // fprintf(f, "UWB buff : %s\n", buff);
        // printf("UWB buff : %s\n", buff);
        isStart = TRUE;

        char *p;
        int phase = 1;
        p = strtok(buff, ",");

        while (p)
        {
            if (phase == 1)
            {
                uwb_y = strtod(p, &eptr);
                phase = 2;
            }
            else if (phase == 2)
            {
                uwb_x = strtod(p, &eptr);
                phase = 3;
            }
            else if (phase == 3)
            {
                sscanf(p, "%lf", &uwb_angle);
                uwb_angle  = uwb_angle+(3.141592653589793/2);
                phase = 1;
            }
            p = strtok(NULL, ",");
        }
        position_offset_uwb(&uwb_x, &uwb_y);
        
        isUpdate = TRUE;
        gtk_widget_queue_draw(GTK_WIDGET(drawingArea));
    }

    close(sock);
    return NULL;
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

    return TRUE;
}


// gboolean on_draw(GtkWidget *widget,
//                  cairo_t *cr,
//                  gpointer data)
// {
//     gint src_width, src_height;
//     gdouble angle = 17;
//     GdkPixbuf *pixbuf = gtk_image_get_pixbuf(GTK_IMAGE(gimage));
//     src_width = gdk_pixbuf_get_width(pixbuf);
//     src_height = gdk_pixbuf_get_height(pixbuf);
//     GdkPixbuf *dest_pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, TRUE, 8, src_width, src_height);

//     // cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, src_width, src_height);
//     // cairo_t *cr = cairo_create(surface);
//     // gdk_cairo_set_source_pixbuf(cr, pixbuf, 0, 0);

//     // cairo_translate(cr, src_width / 2.0, src_height / 2.0);
//     // cairo_rotate(cr, angle * G_PI / 180.0);
//     // cairo_translate(cr, -src_width / 2.0, -src_height / 2.0);

//     // cairo_paint(cr);

//     // cairo_destroy(cr);
//     // cairo_surface_destroy(surface);
//     return TRUE;
// }

gboolean manually_draw()
{
    struct timeval current_tv;
    gettimeofday(&current_tv, NULL);
    double current_mstime, uwb_mstime, cctv_mstime;
    current_mstime = current_tv.tv_sec * 1000 + current_tv.tv_usec / 1000;
    uwb_mstime = uwb_tv.tv_sec * 1000 + uwb_tv.tv_usec / 1000;
    cctv_mstime = cctv_tv.tv_sec * 1000 + cctv_tv.tv_usec / 1000;
    drawingContext = gdk_window_begin_draw_frame(draw_window, cairoRegion);

    if (current_mstime - uwb_mstime >= 0 && current_mstime - uwb_mstime < 400)
    {
        if(uwb_x < 1855 && uwb_x > 0 && uwb_y > 0 && uwb_y < 1027) {
            draw_car(drawingContext, uwb_x, uwb_y, uwb_angle, FROM_UWB);
            // printf("draw car %f,%f,%f\n", uwb_y, uwb_x, uwb_angle);
        } else {
            printf("out of bound\n");
        }
    } else {
        printf("current_mstime : %f\n", current_mstime);
        printf("uwb_mstime : %f\n", uwb_mstime);
    }

    // gtk_widget_queue_draw(GTK_WIDGET(drawingArea));
    // say: "I'm finished drawing
    gdk_window_end_draw_frame(draw_window, drawingContext);

    return FALSE;
}

void rotate_point(double _angle, double *new_x, double *new_y)
{
    *new_x = center_offset_x * cos(_angle) - center_offset_y * sin(_angle);
    *new_y = center_offset_x * sin(_angle) + center_offset_y * cos(_angle);
}

void position_offset_uwb(double *x, double *y)
{
    *x *= 16.1;
    *y *= 16.1;
    *x += 64;
    *y += 157;
}

void draw_car(GdkDrawingContext *_drawingContext, double _x, double _y, double _angle, enum PAINT _paint)
{
    double rotate_x, rotate_y;
    int w = cairo_image_surface_get_width(cst_cctv);
    int h = cairo_image_surface_get_height(cst_cctv);
    
    
    cairo_t *cr = gdk_drawing_context_get_cairo_context(_drawingContext);
    
    gtk_fixed_move(gFixed, GTK_WIDGET(drawingArea), _x - area_offset, _y - area_offset);

    rotate_point(_angle, &rotate_x, &rotate_y);
    cairo_translate(cr, (-1 * rotate_x) + area_offset, (-1 * rotate_y) + area_offset);
    cairo_rotate(cr, _angle);
    cairo_scale(cr, car_width / w, car_height / h);
    
    cairo_set_source_surface(cr, cst_uwb, 30, 30);
    cairo_paint(cr);

    // cairo_destroy(cr);
}


