/******************************************************************************
*
* Copyright (C) 2023 - 2028 KETI, All rights reserved.
*                           (Korea Electronics Technology Institute)
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running for Korean Government Project, or
* (b) that interact with KETI project/platform.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* KETI BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the KETI shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from KETI.
*
******************************************************************************/

#ifndef TESTSTRUCTURE_H
#define TESTSTRUCTURE_H

// #include <ros/ros.h>
// #include "kapcallback.h"
#include "rtwtypes.h"

static creal_T argInit_creal_T1(void)
{
    creal_T result;
    std::vector<double> xt_b = {-0.525, 0.525, -0.525, 0.525};
    std::vector<double> yt_b = {0.505, 0.505, -0.505, -0.505};

    // Using the last value for this example, but this is likely not what you want.
    result.re = xt_b.back();
    result.im = yt_b.back();
    return result;
};

static creal_T argInit_creal_T2(void)
{
    creal_T result;
    result.re = 0;
    result.im = 0;
    return result;
};

static void argInit_1x4_creal_T1(creal_T result[4])
{
    for (int idx1 = 0; idx1 < 4; idx1++) {
        result[idx1] = argInit_creal_T1();
    }
};

static void argInit_1x4_creal_T2(creal_T result[4])
{
    for (int idx1 = 0; idx1 < 4; idx1++) {
        result[idx1] = argInit_creal_T2();
    }
};

struct tag_1_structure
{
    creal_T tag_pos_b1[4];
    creal_T prevTagPos1[4];

    double Ln = 6;
    double Lp = 4;
    double LnC = 4;
    double TagNum = 1;
    double Nanchor = 4;  // Added missing semicolon
    const double RxIDin[6] = {2,1,3,4,0,0};
    const double RxDistin[6] = {5.2236,5.0618,7.8021,12.2057,0,0};
    double s_time = 1.6934e+09;
    double xain[6] = {-0.0100,7.8800,7.8400,0,0,0};
    double yain[6] = {-0.3000,-0.3000,10.5000,15.1000,0,0};
    double prevTagHeading = 0;

    tag_1_structure() {
    argInit_1x4_creal_T1(tag_pos_b1);
    argInit_1x4_creal_T2(prevTagPos1);

    // Assigning specific imaginary values to prevTagPos1
    double im_values[4] = {0.9050, 0.9050, -0.1050, -0.1050};
    for (int i = 0; i < 4; ++i) {
        prevTagPos1[i].im = im_values[i];
    }
}


};

struct tag_2_structure
{
    creal_T tag_pos_b2[4];
    creal_T prevTagPos2[4];

    double Ln = 6;
    double Lp = 4;
    double LnC = 4;
    double TagNum = 2;
    double Nanchor = 4;  // Added missing semicolon
    const double RxIDin[6] = {1,2,3,4,0,0};
    const double RxDistin[6] = {4.2813,5.9000,8.3144,11.8049,0,0};
    double s_time = 1.6934e+09;
    double xain[6] = {-0.0100,7.8800,7.8400,0,0,0};
    double yain[6] = {-0.3000,-0.3000,10.5000,15.1000,0,0};
    double prevTagHeading = 0;

    tag_2_structure() {
        argInit_1x4_creal_T1(tag_pos_b2);
        argInit_1x4_creal_T2(prevTagPos2);
    }
};

// ... rest of the code ...

#endif
