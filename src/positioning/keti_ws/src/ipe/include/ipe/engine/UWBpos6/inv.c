/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: inv.c
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 05-Sep-2023 20:11:41
 */

/* Include Files */
#include "inv.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Definitions */
/*
 * Arguments    : const double x[4]
 *                double y[4]
 * Return Type  : void
 */
void inv(const double x[4], double y[4])
{
  if (fabs(x[1]) > fabs(x[0])) {
    double b_r;
    double t;
    b_r = x[0] / x[1];
    t = 1.0 / (b_r * x[3] - x[2]);
    y[0] = x[3] / x[1] * t;
    y[1] = -t;
    y[2] = -x[2] / x[1] * t;
    y[3] = b_r * t;
  } else {
    double b_r;
    double t;
    b_r = x[1] / x[0];
    t = 1.0 / (x[3] - b_r * x[2]);
    y[0] = x[3] / x[0] * t;
    y[1] = -b_r * t;
    y[2] = -x[2] / x[0] * t;
    y[3] = t;
  }
}

/*
 * File trailer for inv.c
 *
 * [EOF]
 */
