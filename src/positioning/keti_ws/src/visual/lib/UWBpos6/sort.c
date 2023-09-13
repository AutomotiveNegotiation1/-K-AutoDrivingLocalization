/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: sort.c
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 05-Sep-2023 20:11:41
 */

/* Include Files */
#include "sort.h"
#include "UWBpos6_emxutil.h"
#include "UWBpos6_types.h"
#include "rt_nonfinite.h"
#include "sortIdx.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : double x[16]
 *                int idx[16]
 * Return Type  : void
 */
void b_sort(double x[16], int idx[16])
{
  double xwork[16];
  double x4[4];
  int iwork[16];
  int i1;
  int i3;
  int ib;
  int k;
  int nNaNs;
  int quartetOffset;
  signed char idx4[4];
  x4[0] = 0.0;
  idx4[0] = 0;
  x4[1] = 0.0;
  idx4[1] = 0;
  x4[2] = 0.0;
  idx4[2] = 0;
  x4[3] = 0.0;
  idx4[3] = 0;
  memset(&idx[0], 0, 16U * sizeof(int));
  memset(&xwork[0], 0, 16U * sizeof(double));
  nNaNs = 0;
  ib = 0;
  for (k = 0; k < 16; k++) {
    if (rtIsNaN(x[k])) {
      idx[15 - nNaNs] = k + 1;
      xwork[15 - nNaNs] = x[k];
      nNaNs++;
    } else {
      ib++;
      idx4[ib - 1] = (signed char)(k + 1);
      x4[ib - 1] = x[k];
      if (ib == 4) {
        double d;
        double d1;
        int i4;
        signed char b_i1;
        signed char b_i3;
        signed char i;
        signed char i2;
        quartetOffset = k - nNaNs;
        if (x4[0] <= x4[1]) {
          i1 = 1;
          ib = 2;
        } else {
          i1 = 2;
          ib = 1;
        }
        if (x4[2] <= x4[3]) {
          i3 = 3;
          i4 = 4;
        } else {
          i3 = 4;
          i4 = 3;
        }
        d = x4[i1 - 1];
        d1 = x4[i3 - 1];
        if (d <= d1) {
          d = x4[ib - 1];
          if (d <= d1) {
            i = (signed char)i1;
            b_i1 = (signed char)ib;
            i2 = (signed char)i3;
            b_i3 = (signed char)i4;
          } else if (d <= x4[i4 - 1]) {
            i = (signed char)i1;
            b_i1 = (signed char)i3;
            i2 = (signed char)ib;
            b_i3 = (signed char)i4;
          } else {
            i = (signed char)i1;
            b_i1 = (signed char)i3;
            i2 = (signed char)i4;
            b_i3 = (signed char)ib;
          }
        } else {
          d1 = x4[i4 - 1];
          if (d <= d1) {
            if (x4[ib - 1] <= d1) {
              i = (signed char)i3;
              b_i1 = (signed char)i1;
              i2 = (signed char)ib;
              b_i3 = (signed char)i4;
            } else {
              i = (signed char)i3;
              b_i1 = (signed char)i1;
              i2 = (signed char)i4;
              b_i3 = (signed char)ib;
            }
          } else {
            i = (signed char)i3;
            b_i1 = (signed char)i4;
            i2 = (signed char)i1;
            b_i3 = (signed char)ib;
          }
        }
        idx[quartetOffset - 3] = idx4[i - 1];
        idx[quartetOffset - 2] = idx4[b_i1 - 1];
        idx[quartetOffset - 1] = idx4[i2 - 1];
        idx[quartetOffset] = idx4[b_i3 - 1];
        x[quartetOffset - 3] = x4[i - 1];
        x[quartetOffset - 2] = x4[b_i1 - 1];
        x[quartetOffset - 1] = x4[i2 - 1];
        x[quartetOffset] = x4[b_i3 - 1];
        ib = 0;
      }
    }
  }
  if (ib > 0) {
    signed char perm[4];
    perm[1] = 0;
    perm[2] = 0;
    perm[3] = 0;
    if (ib == 1) {
      perm[0] = 1;
    } else if (ib == 2) {
      if (x4[0] <= x4[1]) {
        perm[0] = 1;
        perm[1] = 2;
      } else {
        perm[0] = 2;
        perm[1] = 1;
      }
    } else if (x4[0] <= x4[1]) {
      if (x4[1] <= x4[2]) {
        perm[0] = 1;
        perm[1] = 2;
        perm[2] = 3;
      } else if (x4[0] <= x4[2]) {
        perm[0] = 1;
        perm[1] = 3;
        perm[2] = 2;
      } else {
        perm[0] = 3;
        perm[1] = 1;
        perm[2] = 2;
      }
    } else if (x4[0] <= x4[2]) {
      perm[0] = 2;
      perm[1] = 1;
      perm[2] = 3;
    } else if (x4[1] <= x4[2]) {
      perm[0] = 2;
      perm[1] = 3;
      perm[2] = 1;
    } else {
      perm[0] = 3;
      perm[1] = 2;
      perm[2] = 1;
    }
    quartetOffset = (unsigned char)ib;
    for (k = 0; k < quartetOffset; k++) {
      i1 = perm[k] - 1;
      i3 = ((k - nNaNs) - ib) + 16;
      idx[i3] = idx4[i1];
      x[i3] = x4[i1];
    }
  }
  ib = (nNaNs >> 1) + 16;
  for (k = 0; k <= ib - 17; k++) {
    i1 = (k - nNaNs) + 16;
    quartetOffset = idx[i1];
    idx[i1] = idx[15 - k];
    idx[15 - k] = quartetOffset;
    x[i1] = xwork[15 - k];
    x[15 - k] = xwork[i1];
  }
  if ((nNaNs & 1) != 0) {
    quartetOffset = ib - nNaNs;
    x[quartetOffset] = xwork[quartetOffset];
  }
  if (16 - nNaNs > 1) {
    memset(&iwork[0], 0, 16U * sizeof(int));
    i3 = (16 - nNaNs) >> 2;
    quartetOffset = 4;
    while (i3 > 1) {
      if ((i3 & 1) != 0) {
        i3--;
        ib = quartetOffset * i3;
        i1 = 16 - (nNaNs + ib);
        if (i1 > quartetOffset) {
          b_merge(idx, x, ib, quartetOffset, i1 - quartetOffset, iwork, xwork);
        }
      }
      ib = quartetOffset << 1;
      i3 >>= 1;
      for (k = 0; k < i3; k++) {
        b_merge(idx, x, k * ib, quartetOffset, quartetOffset, iwork, xwork);
      }
      quartetOffset = ib;
    }
    if (16 - nNaNs > quartetOffset) {
      b_merge(idx, x, 0, quartetOffset, 16 - (nNaNs + quartetOffset), iwork,
              xwork);
    }
  }
}

/*
 * Arguments    : emxArray_real_T *x
 *                emxArray_int32_T *idx
 * Return Type  : void
 */
void sort(emxArray_real_T *x, emxArray_int32_T *idx)
{
  emxArray_int32_T *b_iwork;
  emxArray_int32_T *iidx;
  emxArray_int32_T *iwork;
  emxArray_real_T *b_xwork;
  emxArray_real_T *vwork;
  emxArray_real_T *xwork;
  double *vwork_data;
  double *x_data;
  double *xwork_data;
  int b;
  int b_b;
  int dim;
  int i;
  int i1;
  int j;
  int k;
  int vlen;
  int vstride;
  int *idx_data;
  int *iidx_data;
  int *iwork_data;
  x_data = x->data;
  dim = 0;
  if (x->size[0] != 1) {
    dim = -1;
  }
  if (dim + 2 <= 1) {
    i = x->size[0];
  } else {
    i = 1;
  }
  vlen = i - 1;
  emxInit_real_T(&vwork, 1);
  i1 = vwork->size[0];
  vwork->size[0] = i;
  emxEnsureCapacity_real_T(vwork, i1);
  vwork_data = vwork->data;
  i = idx->size[0];
  idx->size[0] = x->size[0];
  emxEnsureCapacity_int32_T(idx, i);
  idx_data = idx->data;
  vstride = 1;
  for (k = 0; k <= dim; k++) {
    vstride *= x->size[0];
  }
  emxInit_int32_T(&iidx);
  emxInit_int32_T(&iwork);
  emxInit_real_T(&xwork, 1);
  emxInit_int32_T(&b_iwork);
  emxInit_real_T(&b_xwork, 1);
  for (j = 0; j < vstride; j++) {
    for (k = 0; k <= vlen; k++) {
      vwork_data[k] = x_data[j + k * vstride];
    }
    i = iidx->size[0];
    iidx->size[0] = vwork->size[0];
    emxEnsureCapacity_int32_T(iidx, i);
    iidx_data = iidx->data;
    dim = vwork->size[0];
    for (i = 0; i < dim; i++) {
      iidx_data[i] = 0;
    }
    if (vwork->size[0] != 0) {
      double x4[4];
      int idx4[4];
      int bLen2;
      int i2;
      int i3;
      int i4;
      int iidx_tmp;
      int n;
      int nNonNaN;
      n = vwork->size[0];
      x4[0] = 0.0;
      idx4[0] = 0;
      x4[1] = 0.0;
      idx4[1] = 0;
      x4[2] = 0.0;
      idx4[2] = 0;
      x4[3] = 0.0;
      idx4[3] = 0;
      i = iwork->size[0];
      iwork->size[0] = vwork->size[0];
      emxEnsureCapacity_int32_T(iwork, i);
      iwork_data = iwork->data;
      dim = vwork->size[0];
      for (i = 0; i < dim; i++) {
        iwork_data[i] = 0;
      }
      i = xwork->size[0];
      xwork->size[0] = vwork->size[0];
      emxEnsureCapacity_real_T(xwork, i);
      xwork_data = xwork->data;
      dim = vwork->size[0];
      for (i = 0; i < dim; i++) {
        xwork_data[i] = 0.0;
      }
      bLen2 = 0;
      dim = 0;
      for (k = 0; k < n; k++) {
        if (rtIsNaN(vwork_data[k])) {
          iidx_tmp = (n - bLen2) - 1;
          iidx_data[iidx_tmp] = k + 1;
          xwork_data[iidx_tmp] = vwork_data[k];
          bLen2++;
        } else {
          dim++;
          idx4[dim - 1] = k + 1;
          x4[dim - 1] = vwork_data[k];
          if (dim == 4) {
            double d;
            double d1;
            signed char b_i1;
            signed char b_i2;
            signed char b_i3;
            signed char b_i4;
            dim = k - bLen2;
            if (x4[0] <= x4[1]) {
              i1 = 1;
              i2 = 2;
            } else {
              i1 = 2;
              i2 = 1;
            }
            if (x4[2] <= x4[3]) {
              i3 = 3;
              i4 = 4;
            } else {
              i3 = 4;
              i4 = 3;
            }
            d = x4[i1 - 1];
            d1 = x4[i3 - 1];
            if (d <= d1) {
              d = x4[i2 - 1];
              if (d <= d1) {
                b_i1 = (signed char)i1;
                b_i2 = (signed char)i2;
                b_i3 = (signed char)i3;
                b_i4 = (signed char)i4;
              } else if (d <= x4[i4 - 1]) {
                b_i1 = (signed char)i1;
                b_i2 = (signed char)i3;
                b_i3 = (signed char)i2;
                b_i4 = (signed char)i4;
              } else {
                b_i1 = (signed char)i1;
                b_i2 = (signed char)i3;
                b_i3 = (signed char)i4;
                b_i4 = (signed char)i2;
              }
            } else {
              d1 = x4[i4 - 1];
              if (d <= d1) {
                if (x4[i2 - 1] <= d1) {
                  b_i1 = (signed char)i3;
                  b_i2 = (signed char)i1;
                  b_i3 = (signed char)i2;
                  b_i4 = (signed char)i4;
                } else {
                  b_i1 = (signed char)i3;
                  b_i2 = (signed char)i1;
                  b_i3 = (signed char)i4;
                  b_i4 = (signed char)i2;
                }
              } else {
                b_i1 = (signed char)i3;
                b_i2 = (signed char)i4;
                b_i3 = (signed char)i1;
                b_i4 = (signed char)i2;
              }
            }
            iidx_data[dim - 3] = idx4[b_i1 - 1];
            iidx_data[dim - 2] = idx4[b_i2 - 1];
            iidx_data[dim - 1] = idx4[b_i3 - 1];
            iidx_data[dim] = idx4[b_i4 - 1];
            vwork_data[dim - 3] = x4[b_i1 - 1];
            vwork_data[dim - 2] = x4[b_i2 - 1];
            vwork_data[dim - 1] = x4[b_i3 - 1];
            vwork_data[dim] = x4[b_i4 - 1];
            dim = 0;
          }
        }
      }
      i3 = vwork->size[0] - bLen2;
      if (dim > 0) {
        signed char perm[4];
        perm[1] = 0;
        perm[2] = 0;
        perm[3] = 0;
        if (dim == 1) {
          perm[0] = 1;
        } else if (dim == 2) {
          if (x4[0] <= x4[1]) {
            perm[0] = 1;
            perm[1] = 2;
          } else {
            perm[0] = 2;
            perm[1] = 1;
          }
        } else if (x4[0] <= x4[1]) {
          if (x4[1] <= x4[2]) {
            perm[0] = 1;
            perm[1] = 2;
            perm[2] = 3;
          } else if (x4[0] <= x4[2]) {
            perm[0] = 1;
            perm[1] = 3;
            perm[2] = 2;
          } else {
            perm[0] = 3;
            perm[1] = 1;
            perm[2] = 2;
          }
        } else if (x4[0] <= x4[2]) {
          perm[0] = 2;
          perm[1] = 1;
          perm[2] = 3;
        } else if (x4[1] <= x4[2]) {
          perm[0] = 2;
          perm[1] = 3;
          perm[2] = 1;
        } else {
          perm[0] = 3;
          perm[1] = 2;
          perm[2] = 1;
        }
        i = (unsigned char)dim;
        for (k = 0; k < i; k++) {
          iidx_tmp = perm[k] - 1;
          i1 = (i3 - dim) + k;
          iidx_data[i1] = idx4[iidx_tmp];
          vwork_data[i1] = x4[iidx_tmp];
        }
      }
      dim = bLen2 >> 1;
      for (k = 0; k < dim; k++) {
        i1 = i3 + k;
        i2 = iidx_data[i1];
        iidx_tmp = (n - k) - 1;
        iidx_data[i1] = iidx_data[iidx_tmp];
        iidx_data[iidx_tmp] = i2;
        vwork_data[i1] = xwork_data[iidx_tmp];
        vwork_data[iidx_tmp] = xwork_data[i1];
      }
      if ((bLen2 & 1) != 0) {
        i1 = i3 + dim;
        vwork_data[i1] = xwork_data[i1];
      }
      nNonNaN = vwork->size[0] - bLen2;
      i1 = 2;
      if (nNonNaN > 1) {
        if (vwork->size[0] >= 256) {
          int nBlocks;
          nBlocks = nNonNaN >> 8;
          if (nBlocks > 0) {
            for (b = 0; b < nBlocks; b++) {
              double c_xwork[256];
              int c_iwork[256];
              i4 = (b << 8) - 1;
              for (b_b = 0; b_b < 6; b_b++) {
                n = 1 << (b_b + 2);
                bLen2 = n << 1;
                i = 256 >> (b_b + 3);
                for (k = 0; k < i; k++) {
                  i2 = (i4 + k * bLen2) + 1;
                  for (i1 = 0; i1 < bLen2; i1++) {
                    dim = i2 + i1;
                    c_iwork[i1] = iidx_data[dim];
                    c_xwork[i1] = vwork_data[dim];
                  }
                  i3 = 0;
                  i1 = n;
                  dim = i2 - 1;
                  int exitg1;
                  do {
                    exitg1 = 0;
                    dim++;
                    if (c_xwork[i3] <= c_xwork[i1]) {
                      iidx_data[dim] = c_iwork[i3];
                      vwork_data[dim] = c_xwork[i3];
                      if (i3 + 1 < n) {
                        i3++;
                      } else {
                        exitg1 = 1;
                      }
                    } else {
                      iidx_data[dim] = c_iwork[i1];
                      vwork_data[dim] = c_xwork[i1];
                      if (i1 + 1 < bLen2) {
                        i1++;
                      } else {
                        dim -= i3;
                        for (i1 = i3 + 1; i1 <= n; i1++) {
                          iidx_tmp = dim + i1;
                          iidx_data[iidx_tmp] = c_iwork[i1 - 1];
                          vwork_data[iidx_tmp] = c_xwork[i1 - 1];
                        }
                        exitg1 = 1;
                      }
                    }
                  } while (exitg1 == 0);
                }
              }
            }
            i1 = nBlocks << 8;
            dim = nNonNaN - i1;
            if (dim > 0) {
              merge_block(iidx, vwork, i1, dim, 2, iwork, xwork);
              xwork_data = xwork->data;
              iwork_data = iwork->data;
            }
            i1 = 8;
          }
        }
        dim = iwork->size[0];
        i = b_iwork->size[0];
        b_iwork->size[0] = iwork->size[0];
        emxEnsureCapacity_int32_T(b_iwork, i);
        iidx_data = b_iwork->data;
        for (i = 0; i < dim; i++) {
          iidx_data[i] = iwork_data[i];
        }
        i = b_xwork->size[0];
        b_xwork->size[0] = xwork->size[0];
        emxEnsureCapacity_real_T(b_xwork, i);
        vwork_data = b_xwork->data;
        dim = xwork->size[0];
        for (i = 0; i < dim; i++) {
          vwork_data[i] = xwork_data[i];
        }
        merge_block(iidx, vwork, 0, nNonNaN, i1, b_iwork, b_xwork);
        vwork_data = vwork->data;
        iidx_data = iidx->data;
      }
    }
    for (k = 0; k <= vlen; k++) {
      i = j + k * vstride;
      x_data[i] = vwork_data[k];
      idx_data[i] = iidx_data[k];
    }
  }
  emxFree_real_T(&b_xwork);
  emxFree_int32_T(&b_iwork);
  emxFree_real_T(&xwork);
  emxFree_int32_T(&iwork);
  emxFree_int32_T(&iidx);
  emxFree_real_T(&vwork);
}

/*
 * File trailer for sort.c
 *
 * [EOF]
 */
