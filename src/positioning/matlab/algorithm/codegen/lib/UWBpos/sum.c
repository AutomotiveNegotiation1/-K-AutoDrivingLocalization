/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: sum.c
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 01-Aug-2023 14:35:53
 */

/* Include Files */
#include "sum.h"
#include "UWBpos_emxutil.h"
#include "UWBpos_types.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Arguments    : const emxArray_real_T *x
 *                emxArray_real_T *y
 * Return Type  : void
 */
void sum(const emxArray_real_T *x, emxArray_real_T *y)
{
  emxArray_real_T *bsum;
  const double *x_data;
  double *bsum_data;
  double *y_data;
  int hi;
  int ib;
  int k;
  int xj;
  x_data = x->data;
  if ((x->size[0] == 0) || (x->size[1] == 0)) {
    int firstBlockLength;
    hi = y->size[0];
    y->size[0] = x->size[0];
    emxEnsureCapacity_real_T(y, hi);
    y_data = y->data;
    firstBlockLength = x->size[0];
    for (hi = 0; hi < firstBlockLength; hi++) {
      y_data[hi] = 0.0;
    }
  } else {
    int bvstride;
    int firstBlockLength;
    int lastBlockLength;
    int nblocks;
    int vstride;
    int xoffset;
    vstride = x->size[0] - 1;
    bvstride = x->size[0] << 10;
    hi = y->size[0];
    y->size[0] = x->size[0];
    emxEnsureCapacity_real_T(y, hi);
    y_data = y->data;
    emxInit_real_T(&bsum, 1);
    hi = bsum->size[0];
    bsum->size[0] = x->size[0];
    emxEnsureCapacity_real_T(bsum, hi);
    bsum_data = bsum->data;
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
    for (xj = 0; xj <= vstride; xj++) {
      y_data[xj] = x_data[xj];
      bsum_data[xj] = 0.0;
    }
    for (k = 2; k <= firstBlockLength; k++) {
      xoffset = (k - 1) * (vstride + 1);
      for (xj = 0; xj <= vstride; xj++) {
        y_data[xj] += x_data[xoffset + xj];
      }
    }
    for (ib = 2; ib <= nblocks; ib++) {
      firstBlockLength = (ib - 1) * bvstride;
      for (xj = 0; xj <= vstride; xj++) {
        bsum_data[xj] = x_data[firstBlockLength + xj];
      }
      if (ib == nblocks) {
        hi = lastBlockLength;
      } else {
        hi = 1024;
      }
      for (k = 2; k <= hi; k++) {
        xoffset = firstBlockLength + (k - 1) * (vstride + 1);
        for (xj = 0; xj <= vstride; xj++) {
          bsum_data[xj] += x_data[xoffset + xj];
        }
      }
      for (xj = 0; xj <= vstride; xj++) {
        y_data[xj] += bsum_data[xj];
      }
    }
    emxFree_real_T(&bsum);
  }
}

/*
 * File trailer for sum.c
 *
 * [EOF]
 */
