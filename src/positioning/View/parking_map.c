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

cairo_surface_t *cst_uwb;
cairo_surface_t *cst_cctv;
GdkWindow *draw_window;
cairo_region_t *cairoRegion;
bool first = TRUE;

pthread_t uwb_thread;
pthread_t cctv_thread;
double uwb_x = 100.0;
double uwb_y = 100.0;
double uwb_angle = 0;
double cctv_x = 100.0;
double cctv_y = 100.0;
double cctv_angle = 270;
enum PAINT cur_paint = FROM_CCTV;
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

int valid_cctv_zone(double x, double y);
void draw_car(GdkDrawingContext *_drawingContext, double _x, double _y, double _angle, enum PAINT _paint);
int calculate_diff(double before_x, double before_y, double after_x, double after_y);
void add_point(double x, double y);
void set_mode(double x, double y);
void position_offset_uwb(double *x, double *y);
void position_offset_cctv(double *x, double *y);
gint callback_timer(gpointer argv);
int new_socket();
gpointer uwb_thread_run(gpointer data);
gpointer cctv_thread_run(gpointer data);
void rotate_point(double _angle, double *new_x, double *new_y);
gboolean manually_draw();
gboolean on_draw(GtkWidget *widget, GdkEventExpose *event, gpointer data);
FILE *f;

void draw_point_on_fixed(GtkFixed *fixed, double x, double y);
static gboolean draw_circle_callback(GtkWidget *widget, cairo_t *cr, gpointer data);

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
    gBuilder = gtk_builder_new_from_resource("/glade/parking_map.glade");
    //gBuilder = gtk_builder_new_from_file("parking_map.glade");

    gApp = GTK_WIDGET(gtk_builder_get_object(gBuilder, "ID_Window"));
    g_signal_connect(gApp, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_builder_connect_signals(gBuilder, NULL);
    gFixed = GTK_FIXED(gtk_builder_get_object(gBuilder, "ID_FIXED"));
    drawingArea = GTK_DRAWING_AREA(gtk_builder_get_object(gBuilder, "ID_DRAWINGAREA"));
    
    time_t t;
    char fname[256];  // 버퍼 크기 증가
    time(&t);
    snprintf(fname, sizeof(fname), "log/log_%ld.txt", t);  // sprintf 대신 snprintf 사용
    f = fopen(fname, "w+");
    if (!f)
    {
        fprintf(stderr, "fopen error\n");
        return EXIT_FAILURE;  // 파일 열기 실패 처리
    }

    gtk_widget_show(gApp);
    gtk_widget_show(GTK_WIDGET(drawingArea));
    g_thread_new("uwb_thread", uwb_thread_run, NULL);
    //g_thread_new("cctv_thread", cctv_thread_run, NULL);

    // g_timeout_add(1000, callback_timer, NULL);

    gtk_main();
    fclose(f);
    free(point_list);
    return EXIT_SUCCESS;
}

gint callback_timer(gpointer argv)
{
    gint x, y;
    gtk_widget_get_pointer(gApp, &x, &y);
    printf("mouse position :: x = %d, y = %d\n", x, y);

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
        printf("UWB buff : %s\n", buff);
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
        set_mode(uwb_x, uwb_y);

        if (cur_paint == FROM_UWB)
        {
            isUpdate = TRUE;
        }
        //        gtk_fixed_move(gFixed, GTK_WIDGET(drawingArea), x - area_offset, y - area_offset);
    }

    close(sock);
    return NULL;
}

gpointer cctv_thread_run(gpointer data)
{
    char buff[1024];
    int rx_leng;
    int sock = new_socket();
    struct sockaddr_in from_adr;
    socklen_t adr_sz;
    double timestamp;
    char *eptr;

    memset(&from_adr, 0, sizeof(from_adr));
    from_adr.sin_family = AF_INET;
    from_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    from_adr.sin_port = htons(5001);
    if (bind(sock, (struct sockaddr *)&from_adr, sizeof(from_adr)) == -1)
    {
        perror("bind failed");
        exit(1);
    }
    while (1)
    {
        adr_sz = sizeof(from_adr);
        rx_leng = recvfrom(sock, buff, 1024, 0, (struct sockaddr *)&from_adr, &adr_sz);
        buff[rx_leng] = 0;
        // fprintf(f, "CCTV buff : %s\n", buff);
        printf("CCTV buff : %s\n", buff);
        isStart = TRUE;
        char *p;
        int phase = 1;
        p = strtok(buff, ",");
        while (p)
        {
            if (phase == 1)
            {
                cctv_y = strtod(p, &eptr);
                phase = 2;
            }
            else if (phase == 2)
            {
                cctv_x = strtod(p, &eptr);
                phase = 3;
            }
            else if (phase == 3)
            {
                timestamp = strtod(p, &eptr);
                // cctv_angle = atoi(p);
                phase = 1;
            }
            p = strtok(NULL, ",");
        }
        position_offset_cctv(&cctv_x, &cctv_y);
        if (valid_cctv_zone(cctv_x, cctv_y) == 1)
        {
            gettimeofday(&cctv_tv, NULL);
            cur_paint = FROM_CCTV;
            isUpdate = TRUE;
        }
        // printf("x : %f, y : %f\n", cctv_x, cctv_y);

        if (list_cnt >= ROTATION_WINDOW)
        {
            double diff_x = cctv_x - point_list->next->x;
            double diff_y = point_list->next->y - cctv_y;
            double _x = diff_x / diff_y;
            cctv_angle = atan(diff_x / diff_y);
            if (diff_y < 0 && diff_x < 0)
            {
                cctv_angle += G_PI;
            }
            else if (diff_y < 0 && diff_x > 0)
            {
                cctv_angle += G_PI;
            }
            else if (diff_x == 0.0 && diff_y < 0)
            {
                cctv_angle += G_PI;
            }
            printf("diff_x : %f, diff_y : %f, _x : %f, cctv_angle : %f\n", diff_x, diff_y, _x, cctv_angle);
        }

        // gtk_fixed_move(gFixed, GTK_WIDGET(drawingArea), x - area_offset, y - area_offset);
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

    return FALSE;
}

gboolean manually_draw()
{
    struct timeval current_tv;
    gettimeofday(&current_tv, NULL);
    double current_mstime, uwb_mstime, cctv_mstime;
    current_mstime = current_tv.tv_sec * 1000 + current_tv.tv_usec / 1000;
    uwb_mstime = uwb_tv.tv_sec * 1000 + uwb_tv.tv_usec / 1000;
    cctv_mstime = cctv_tv.tv_sec * 1000 + cctv_tv.tv_usec / 1000;

    GdkDrawingContext *drawingContext = gdk_window_begin_draw_frame(draw_window, cairoRegion);

    {
        // if (current_mstime - cctv_mstime > 0 && current_mstime - cctv_mstime < 400)
        // {
        //     printf("cctv paint\n");
        //     if (cctv_x > 1450 || cctv_y < 0 || cctv_y > 1027)
        //     {
        //         printf("out of bound\n");
        //     }
        //     else
        //     {
        //         int result = calculate_diff(current_x, current_y, cctv_x, cctv_y);
        //         if (result == 0)
        //         {
        //             draw_car(drawingContext, cctv_x, cctv_y, cctv_angle, FROM_CCTV);
        //         } else {
        //             draw_car(drawingContext, current_x, current_y, current_angle, current_paint);
        //         }
                
        //     }
        // }
        if (current_mstime - uwb_mstime > 0 && current_mstime - uwb_mstime < 400)
        {
            if (uwb_x > 1450 || uwb_y < 0 || uwb_y > 1027)
            {
//                printf("out of bound\n");
            }
            else
            {
                int result = calculate_diff(current_x, current_y, uwb_x, uwb_y);
                if (result == 0)
                {
                    draw_car(drawingContext, uwb_x, uwb_y, uwb_angle, FROM_UWB);
                    printf("draw car %f,%f,%f\n", uwb_y, uwb_x, uwb_angle);
                } else {
                    draw_car(drawingContext, current_x, current_y, current_angle, current_paint);
                }
            }
        }
        else
        {
            // printf("out of time\n");
            //            gtk_widget_set_size_request(GTK_WIDGET(drawingArea), 0, 0);
        }

        gtk_widget_queue_draw(GTK_WIDGET(drawingArea));

        // say: "I'm finished drawing
        gdk_window_end_draw_frame(draw_window, drawingContext);
    }

    return FALSE;
}

void rotate_point(double _angle, double *new_x, double *new_y)
{
    *new_x = center_offset_x * cos(_angle) - center_offset_y * sin(_angle);
    *new_y = center_offset_x * sin(_angle) + center_offset_y * cos(_angle);
}


// void rotate_point(double _angle, double *new_x, double *new_y)
// {
//     double temp_x = *new_x;
//     double temp_y = *new_y;

//     *new_x = temp_x * cos(_angle) + temp_y * sin(_angle);
//     *new_y = -temp_x * sin(_angle) + temp_y * cos(_angle);
// }

void position_offset_uwb(double *x, double *y)
{
    *x *= 20;
    *y *= 20;
    // *x += 375;
    // *y += 8;
    *x += 98;
    // *y += 0;
}

void position_offset_cctv(double *x, double *y)
{
    // 기둥 20픽셀 : 100cm, 약 140픽셀 : 705cm 1픽셀 당 약 5cm
    // 122픽셀 -> 약 610cm -> 608cm
    // 픽셀의 수 : 그리드 수 = n'픽셀 : n칸
    // *x = *x * pixel_x / measurement_x;
    //    *y = *y * pixel_y / measurement_y;
    // (0,0) offset
    *x *= 20;
    *y *= 20;

    *x += 375;
    *y += 8;
}

void set_mode(double x, double y)
{
    //
    /* **********************************************
        CCTV ZONE : 380,0 => 1100,140
    ************************************************/
    // if (y < 140.0 && x > 500.0 && x < 1100.0)
    // {
    //     cur_paint = FROM_CCTV;
    //     // change_cnt = 10;
    // }
    // else
    // {
    //     cur_paint = FROM_UWB;
    //     // change_cnt = 10;
    // }
    cur_paint = FROM_UWB;
}
int valid_cctv_zone(double x, double y)
{
    //
    /* **********************************************
        CCTV ZONE : 380,0 => 1100,140
    ************************************************/
    if (y < 140.0 && x > 500.0 && x < 1100.0)
    {
        cur_paint = FROM_CCTV;
        return 1;
    }
    return 0;
}

void add_point(double x, double y)
{
    struct POINT *new_point = (struct POINT *)malloc(sizeof(struct POINT));
    if (new_point == NULL) {
        fprintf(stderr, "Memory allocation failed for new point\n");
        return;  // 메모리 할당 실패 처리
    }
    new_point->x = x;
    new_point->y = y;

    if (list_cnt >= ROTATION_WINDOW)
    {
        struct POINT *remove_point = point_list->next;
        point_list->next = point_list->next->next;
        point_list->next->prev = point_list;
        free(remove_point);
        list_cnt--;
    }
    new_point->prev = point_list->prev;
    new_point->next = point_list;
    point_list->prev->next = new_point;
    point_list->prev = new_point;
    list_cnt++;
}

void add_anchor(int x, int y)
{
    GtkImage *gImage2 = GTK_IMAGE(gtk_image_new());
    gtk_image_set_from_icon_name(gImage2, "gtk-media-record", GTK_ICON_SIZE_BUTTON);
    gtk_fixed_put(gFixed, GTK_WIDGET(gImage2), x, y);
    gtk_widget_show(GTK_WIDGET(gImage2));
}

int calculate_diff(double before_x, double before_y, double after_x, double after_y)
{
    // double result = pow(after_x - before_x, 2) + pow(after_y - before_y, 2);
    // if (result < 25)
    // {
    //     return -1;
    // }
    // else
    // {
    //     return 0;
    // }
    return 0;
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
    if (!isStart)
    {
        gtk_widget_set_size_request(GTK_WIDGET(drawingArea), 0, 0);
    }
    else
    {
        gtk_widget_set_size_request(GTK_WIDGET(drawingArea), 200, 200);
    }
    if(_paint == FROM_UWB) {
        cairo_set_source_surface(cr, cst_uwb, 30, 30);
    } else if(_paint == FROM_CCTV) {
        cairo_set_source_surface(cr, cst_cctv, 30, 30);
    }
    if (isUpdate)
    {
        //add_point(_x, _y);
        isUpdate = FALSE;
        current_x = _x;
        current_y = _y;

        current_angle = _angle;
        current_paint = _paint;
    }
    //draw_point_on_fixed(gFixed, current_x, current_y);
    cairo_paint(cr);
}

void draw_point_on_fixed(GtkFixed *fixed, double x, double y)
{
    GtkWidget *drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(drawing_area, 10, 10); // 원의 크기 설정
    gtk_fixed_put(fixed, drawing_area, x, y);

    // 원 그리기에 대한 콜백 함수 연결
    g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(draw_circle_callback), NULL);
    gtk_widget_show(drawing_area);
}

static gboolean draw_circle_callback(GtkWidget *widget, cairo_t *cr, gpointer data)
{
    cairo_set_line_width(cr, 2);
    cairo_set_source_rgb(cr, 1, 0, 0); // 적색 원

    // 원 그리기 (중앙 위치, 반지름 5)
    cairo_arc(cr, 5, 5, 2, 0, 2 * G_PI);
    cairo_fill(cr);

    return FALSE;
}

