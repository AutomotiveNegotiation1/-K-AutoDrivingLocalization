/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: mean.c
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 05-Sep-2023 20:11:41
 */

/* Include Files */
#include "mean.h"
#include "UWBpos6_data.h"
#include "UWBpos6_types.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Arguments    : const emxArray_creal_T *x
 * Return Type  : creal_T
 */
creal_T b_mean(const emxArray_creal_T *x)
{
  const creal_T *x_data;
  creal_T y;
  double x_im;
  double x_re;
  int ib;
  int k;
  x_data = x->data;
  if (x->size[1] == 0) {
    x_re = 0.0;
    x_im = 0.0;
  } else {
    int firstBlockLength;
    int lastBlockLength;
    int nblocks;
    if (x->size[1] <= 1024) {
      firstBlockLength = x->size[1];
      lastBlockLength = 0;
      nblocks = 1;
    } else {
      firstBlockLength = 1024;
      nblocks = (int)((unsigned int)x->size[1] >> 10);
      lastBlockLength = x->size[1] - (nblocks << 10);
      if (lastBlockLength > 0) {
        nblocks++;
      } else {
        lastBlockLength = 1024;
      }
    }
    x_re = x_data[0].re;
    x_im = x_data[0].im;
    for (k = 2; k <= firstBlockLength; k++) {
      x_re += x_data[k - 1].re;
      x_im += x_data[k - 1].im;
    }
    for (ib = 2; ib <= nblocks; ib++) {
      double bsum_im;
      double bsum_re;
      int hi;
      firstBlockLength = (ib - 1) << 10;
      bsum_re = x_data[firstBlockLength].re;
      bsum_im = x_data[firstBlockLength].im;
      if (ib == nblocks) {
        hi = lastBlockLength;
      } else {
        hi = 1024;
      }
      for (k = 2; k <= hi; k++) {
        int bsum_re_tmp;
        bsum_re_tmp = (firstBlockLength + k) - 1;
        bsum_re += x_data[bsum_re_tmp].re;
        bsum_im += x_data[bsum_re_tmp].im;
      }
      x_re += bsum_re;
      x_im += bsum_im;
    }
  }
  if (x_im == 0.0) {
    y.re = x_re / (double)x->size[1];
    y.im = 0.0;
  } else if (x_re == 0.0) {
    y.re = 0.0;
    y.im = x_im / (double)x->size[1];
  } else {
    y.re = x_re / (double)x->size[1];
    y.im = x_im / (double)x->size[1];
  }
  return y;
}

/*
 * Arguments    : const creal_T x[3]
 * Return Type  : creal_T
 */
creal_T mean(const creal_T x[3])
{
  creal_T y;
  double x_im;
  double x_re;
  x_re = (x[0].re + x[1].re) + x[2].re;
  x_im = (x[0].im + x[1].im) + x[2].im;
  if (x_im == 0.0) {
    y.re = x_re / 3.0;
    y.im = 0.0;
  } else if (x_re == 0.0) {
    y.re = 0.0;
    y.im = x_im / 3.0;
  } else {
    y.re = x_re / 3.0;
    y.im = x_im / 3.0;
  }
  return y;
}

/*
 * File trailer for mean.c
 *
 * [EOF]
 */
