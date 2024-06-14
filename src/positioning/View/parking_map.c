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
#define ROTATION_WINDOW 20

enum PAINT
{
    FROM_UWB,
    FROM_CCTV
};

struct POINT
{
    double x;
    double y;
    struct timeval time;
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
bool abnormal = FALSE;
bool initializing = TRUE;
int initial_cnt = 1;

pthread_t uwb_thread;
pthread_t cctv_thread;
GMutex gmutex;

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

void draw_car(GdkDrawingContext *_drawingContext, double _x, double _y, double _angle, enum PAINT _paint);
double calculate_diff(double before_x, double before_y, double after_x, double after_y);
void add_point(double x, double y);
void set_mode(double _uwb_x, double _uwb_y, double _cctv_x, double _cctv_y);
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
    
    gtk_widget_set_size_request(GTK_WIDGET(drawingArea), 200, 200);
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
    g_mutex_init(&gmutex);

    gtk_widget_show(gApp);
    gtk_widget_show(GTK_WIDGET(drawingArea));
    g_thread_new("uwb_thread", uwb_thread_run, NULL);
    g_thread_new("cctv_thread", cctv_thread_run, NULL);

    g_timeout_add(10, callback_timer, NULL);

    gtk_main();
    fclose(f);
    free(point_list);
    return EXIT_SUCCESS;
}

gint callback_timer(gpointer argv)
{
    gint x, y;
    gtk_widget_get_pointer(gApp, &x, &y);
    gtk_widget_queue_draw(GTK_WIDGET(drawingArea));
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

        g_mutex_lock(&gmutex);

        gettimeofday(&uwb_tv, NULL);

        buff[rx_leng] = 0;
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
        g_mutex_unlock(&gmutex);
        
        // isUpdate = TRUE;
        // gtk_widget_queue_draw(GTK_WIDGET(drawingArea));
    }

    close(sock);
    return NULL;
}

void position_offset_uwb(double *x, double *y)
{
    *x *= 20;
    *y *= 20;
    // *x += 375;
    // *y += 8;
    *x += 98;
    // *y += 0;
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
        
        g_mutex_lock(&gmutex);
        gettimeofday(&cctv_tv, NULL);
        buff[rx_leng] = 0;
        // fprintf(f, "CCTV buff : %s\n", buff);
        // printf("CCTV buff : %s\n", buff);
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
        isUpdate = TRUE;

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
        } else {
            cctv_angle = uwb_angle;
            printf("cctv_angle : %f\n", cctv_angle);
        }
        g_mutex_unlock(&gmutex);
        
        // gtk_widget_queue_draw(GTK_WIDGET(drawingArea));
        // gtk_fixed_move(gFixed, GTK_WIDGET(drawingArea), x - area_offset, y - area_offset);
    }

    close(sock);
    return NULL;
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

    // *x += 375;
    // *y += 8;
    *x += 98;
    // *y += 0;
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
    // printf("start currnet_mstime : %f\n", current_mstime);
    GdkDrawingContext *drawingContext = gdk_window_begin_draw_frame(draw_window, cairoRegion);

    set_mode(uwb_x, uwb_y, cctv_x, cctv_y);

    if (cur_paint == FROM_UWB && current_mstime - uwb_mstime >= 0 && current_mstime - uwb_mstime < 400)
    {
        if (uwb_x > 0 && uwb_x < 1450 && uwb_y > 0 && uwb_y < 1027) {
            if(abnormal) {
                draw_car(drawingContext, current_x, current_y, current_angle, current_paint);
            } else {
                draw_car(drawingContext, uwb_x, uwb_y, uwb_angle, cur_paint);
            }
        } else {
            initial_cnt++;
        }
    } 
    else if (cur_paint == FROM_CCTV && current_mstime - cctv_mstime >= 0 && current_mstime - cctv_mstime < 400)
    {
        if (cctv_x > 0 && cctv_x < 1450 && cctv_y > 0 && cctv_y < 1027) {
            if(abnormal) {
                draw_car(drawingContext, current_x, current_y, current_angle, current_paint);
            } else {
                draw_car(drawingContext, cctv_x, cctv_y, cctv_angle, cur_paint);
            }
        } else {
            initial_cnt++;
        }
    }

    // say: "I'm finished drawing
    gdk_window_end_draw_frame(draw_window, drawingContext);
    
    g_mutex_unlock(&gmutex);

    return FALSE;
}

void rotate_point(double _angle, double *new_x, double *new_y)
{
    *new_x = center_offset_x * cos(_angle) - center_offset_y * sin(_angle);
    *new_y = center_offset_x * sin(_angle) + center_offset_y * cos(_angle);
}


void set_mode(double _uwb_x, double _uwb_y, double _cctv_x, double _cctv_y)
{
    double diff;
    // 1안 : 특정 영역에서만 CCTV 좌표 출력
    /* **********************************************
        CCTV ZONE : 380,0 => 1100,140
    ************************************************/
    if (_uwb_y < 130.0 && _uwb_x > 98.0 && _uwb_x < 558.0)
    {
        cur_paint = FROM_CCTV;
    }
    else
    {
        cur_paint = FROM_UWB;
    }
    // 2안 : 특정 영역 + 기존 좌표와의 거리 비교
    if(cur_paint == FROM_CCTV) {
        diff = calculate_diff(current_x, current_y, _cctv_x, _cctv_y);
    } else if(cur_paint == FROM_UWB) {
        diff = calculate_diff(current_x, current_y, _uwb_x, _uwb_y);
    }
    
    if(initializing) {
        if(initial_cnt > 60) {
            printf("finisg init\n");
            initializing = FALSE;
            initial_cnt = 0;
        }
        if(diff > 100) {
            initial_cnt = 0;
        } else {
            initial_cnt++;
        }
        abnormal = FALSE;
    } else {
        if(diff > 100) { 
            initial_cnt++;
            abnormal = TRUE;
        } else {
            abnormal = FALSE;
            initial_cnt = 0;
        }
        if(initial_cnt > 180) {
            printf("initializing\n");
            initializing = TRUE;
            initial_cnt = 0;
        } 
    }
    // 3안 : 기존 좌표 기반 
    //
    // if(debug_cn % 100 == 0) {
    //     if(cur_paint == FROM_CCTV) {
    //         cur_paint = FROM_UWB;
    //     } else if(cur_paint == FROM_UWB) {
    //         cur_paint = FROM_CCTV;
    //     }
    // }
    

}
void remove_all_points()
{
    struct POINT *remove_point;
    while(point_list->next != point_list) {
        remove_point = point_list->next;
        point_list->next = remove_point->next;
        point_list->next->prev = point_list;
        free(remove_point);
        list_cnt--;
    }
}

void add_point(double x, double y)
{
    double new_mstime = cctv_tv.tv_sec * 1000 + cctv_tv.tv_usec / 1000;
    double last_mstime;
    struct POINT *new_point = (struct POINT *)malloc(sizeof(struct POINT));
    if (new_point == NULL) {
        fprintf(stderr, "Memory allocation failed for new point\n");
        return;  // 메모리 할당 실패 처리
    }
    new_point->x = x;
    new_point->y = y;
    new_point->time = cctv_tv;

    last_mstime = point_list->prev->time.tv_sec * 1000 + point_list->prev->time.tv_usec / 1000;
    if(new_mstime - last_mstime > 1000) {
        remove_all_points();
    }

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



double calculate_diff(double before_x, double before_y, double after_x, double after_y)
{
    double result = pow(after_x - before_x, 2) + pow(after_y - before_y, 2);
    
    return result;
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

    if(_paint == FROM_UWB) {
        cairo_set_source_surface(cr, cst_uwb, 30, 30);
    } else if(_paint == FROM_CCTV) {
        cairo_set_source_surface(cr, cst_cctv, 30, 30);
        if (isUpdate && !abnormal)
        {
            add_point(_x, _y);
            isUpdate = FALSE;
        }
    }
    
    current_x = _x;
    current_y = _y;
    current_angle = _angle;
    current_paint = _paint;
    //draw_point_on_fixed(gFixed, current_x, current_y);
    cairo_paint(cr);
}



