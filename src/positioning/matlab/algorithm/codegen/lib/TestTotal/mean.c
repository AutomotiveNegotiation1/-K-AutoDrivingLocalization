/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: mean.c
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 01-Aug-2023 15:24:45
 */

/* Include Files */
#include "mean.h"
#include "TestTotal_data.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Arguments    : const creal_T x[10]
 * Return Type  : creal_T
 */
creal_T b_mean(const creal_T x[10])
{
  creal_T y;
  double x_im;
  double x_re;
  int k;
  x_re = x[0].re;
  x_im = x[0].im;
  for (k = 0; k < 9; k++) {
    x_re += x[k + 1].re;
    x_im += x[k + 1].im;
  }
  if (x_im == 0.0) {
    y.re = x_re / 10.0;
    y.im = 0.0;
  } else if (x_re == 0.0) {
    y.re = 0.0;
    y.im = x_im / 10.0;
  } else {
    y.re = x_re / 10.0;
    y.im = x_im / 10.0;
  }
  return y;
}

/*
 * Arguments    : const double x[10]
 * Return Type  : double
 */
double c_mean(const double x[10])
{
  double y;
  int k;
  y = x[0];
  for (k = 0; k < 9; k++) {
    y += x[k + 1];
  }
  y /= 10.0;
  return y;
}

/*
 * Arguments    : const creal_T x[4]
 * Return Type  : creal_T
 */
creal_T mean(const creal_T x[4])
{
  creal_T y;
  double x_im;
  double x_re;
  x_re = ((x[0].re + x[1].re) + x[2].re) + x[3].re;
  x_im = ((x[0].im + x[1].im) + x[2].im) + x[3].im;
  if (x_im == 0.0) {
    y.re = x_re / 4.0;
    y.im = 0.0;
  } else if (x_re == 0.0) {
    y.re = 0.0;
    y.im = x_im / 4.0;
  } else {
    y.re = x_re / 4.0;
    y.im = x_im / 4.0;
  }
  return y;
}

/*
 * File trailer for mean.c
 *
 * [EOF]
 */
