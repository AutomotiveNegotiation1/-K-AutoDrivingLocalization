/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: combineVectorElements.c
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 05-Sep-2023 20:11:41
 */

/* Include Files */
#include "combineVectorElements.h"
#include "UWBpos6_types.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Arguments    : const emxArray_creal_T *x
 * Return Type  : creal_T
 */
creal_T combineVectorElements(const emxArray_creal_T *x)
{
  const creal_T *x_data;
  creal_T y;
  int ib;
  int k;
  x_data = x->data;
  if (x->size[2] == 0) {
    y.re = 0.0;
    y.im = 0.0;
  } else {
    int firstBlockLength;
    int lastBlockLength;
    int nblocks;
    if (x->size[2] <= 1024) {
      firstBlockLength = x->size[2];
      lastBlockLength = 0;
      nblocks = 1;
    } else {
      firstBlockLength = 1024;
      nblocks = (int)((unsigned int)x->size[2] >> 10);
      lastBlockLength = x->size[2] - (nblocks << 10);
      if (lastBlockLength > 0) {
        nblocks++;
      } else {
        lastBlockLength = 1024;
      }
    }
    y = x_data[0];
    for (k = 2; k <= firstBlockLength; k++) {
      y.re += x_data[k - 1].re;
      y.im += x_data[k - 1].im;
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
      y.re += bsum_re;
      y.im += bsum_im;
    }
  }
  return y;
}

/*
 * File trailer for combineVectorElements.c
 *
 * [EOF]
 */
