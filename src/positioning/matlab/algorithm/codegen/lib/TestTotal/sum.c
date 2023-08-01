/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: sum.c
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 01-Aug-2023 15:24:45
 */

/* Include Files */
#include "sum.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Arguments    : const double x[24]
 *                double y[6]
 * Return Type  : void
 */
void sum(const double x[24], double y[6])
{
  int k;
  int xj;
  for (xj = 0; xj < 6; xj++) {
    y[xj] = x[xj];
  }
  for (k = 0; k < 3; k++) {
    int xoffset;
    xoffset = (k + 1) * 6;
    for (xj = 0; xj < 6; xj++) {
      y[xj] += x[xoffset + xj];
    }
  }
}

/*
 * File trailer for sum.c
 *
 * [EOF]
 */
