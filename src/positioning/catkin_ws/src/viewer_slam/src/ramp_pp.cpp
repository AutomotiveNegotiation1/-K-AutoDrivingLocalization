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
#include <math.h>
#include <vector>
#include <fstream>
#include <iostream>
#include "ramp_pp.h"

using namespace std;

bool bf = true;
vector<pair<double, double>> circle_data;
int data_size = 0;
double last_diff_y = 0;
double before_x = 0;
double before_y = 0;


void geofencing(double *x, double *y)
{
    double center_x = 1527;
    double center_y = 647;
    double diff_x = *x - center_x;
    double diff_y = *y - center_y;
    double tan = diff_y/diff_x;
    double distancesquare = pow(diff_x, 2) + pow(diff_y, 2);
    if(13225 > distancesquare) {
        double x_square = 13225 / (pow(tan, 2) + 1);
        if(diff_x >= 0) {
            *x = sqrt(x_square) + center_x;
            *y = sqrt(x_square) * tan + center_y;
        } else {
            *x = center_x - sqrt(x_square);
            *y = center_y - sqrt(x_square) * tan;
        }
        
    }

}

void post_process(double *_x, double *_y, double *_angle)
{
    geofencing(_x, _y);
}
