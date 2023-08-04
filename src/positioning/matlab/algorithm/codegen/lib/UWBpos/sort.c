/*
 * sort.c
 *
 * Code generation for function 'sort'
 *
 */

/* Include files */
#include "sort.h"
#include "rt_nonfinite.h"
#include "sortIdx.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Definitions */
void sort(double x_data[], const int *x_size)
{
  double vwork_data[4];
  double x4[4];
  double xwork_data[4];
  double d;
  double d1;
  int b_iwork_data[4];
  int iidx_data[4];
  int iwork_data[4];
  int dim;
  int i1;
  int i2;
  int i3;
  int i4;
  int j;
  int k;
  int nNaNs;
  int vlen;
  int vstride;
  int vwork_size;
  signed char idx4[4];
  signed char perm[4];
  signed char b_i1;
  signed char b_i2;
  signed char b_i3;
  signed char i;
  dim = 0;
  if (*x_size != 1) {
    dim = -1;
  }
  if (dim + 2 <= 1) {
    vwork_size = *x_size;
  } else {
    vwork_size = 1;
  }
  vlen = vwork_size - 1;
  vstride = 1;
  for (k = 0; k <= dim; k++) {
    vstride *= *x_size;
  }
  for (j = 0; j < vstride; j++) {
    for (k = 0; k <= vlen; k++) {
      vwork_data[k] = x_data[j + k * vstride];
    }
    if (vwork_size - 1 >= 0) {
      memset(&iidx_data[0], 0, (unsigned int)vwork_size * sizeof(int));
    }
    if (vwork_size != 0) {
      x4[0] = 0.0;
      idx4[0] = 0;
      x4[1] = 0.0;
      idx4[1] = 0;
      x4[2] = 0.0;
      idx4[2] = 0;
      x4[3] = 0.0;
      idx4[3] = 0;
      memset(&iwork_data[0], 0, (unsigned int)vwork_size * sizeof(int));
      memset(&xwork_data[0], 0, (unsigned int)vwork_size * sizeof(double));
      nNaNs = 0;
      dim = 0;
      for (k = 0; k < vwork_size; k++) {
        if (rtIsNaN(vwork_data[k])) {
          i3 = (vwork_size - nNaNs) - 1;
          iidx_data[i3] = k + 1;
          xwork_data[i3] = vwork_data[k];
          nNaNs++;
        } else {
          dim++;
          idx4[dim - 1] = (signed char)(k + 1);
          x4[dim - 1] = vwork_data[k];
          if (dim == 4) {
            dim = k - nNaNs;
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
                i = (signed char)i1;
                b_i1 = (signed char)i2;
                b_i2 = (signed char)i3;
                b_i3 = (signed char)i4;
              } else if (d <= x4[i4 - 1]) {
                i = (signed char)i1;
                b_i1 = (signed char)i3;
                b_i2 = (signed char)i2;
                b_i3 = (signed char)i4;
              } else {
                i = (signed char)i1;
                b_i1 = (signed char)i3;
                b_i2 = (signed char)i4;
                b_i3 = (signed char)i2;
              }
            } else {
              d1 = x4[i4 - 1];
              if (d <= d1) {
                if (x4[i2 - 1] <= d1) {
                  i = (signed char)i3;
                  b_i1 = (signed char)i1;
                  b_i2 = (signed char)i2;
                  b_i3 = (signed char)i4;
                } else {
                  i = (signed char)i3;
                  b_i1 = (signed char)i1;
                  b_i2 = (signed char)i4;
                  b_i3 = (signed char)i2;
                }
              } else {
                i = (signed char)i3;
                b_i1 = (signed char)i4;
                b_i2 = (signed char)i1;
                b_i3 = (signed char)i2;
              }
            }
            iidx_data[dim - 3] = idx4[i - 1];
            iidx_data[dim - 2] = idx4[b_i1 - 1];
            iidx_data[dim - 1] = idx4[b_i2 - 1];
            iidx_data[dim] = idx4[b_i3 - 1];
            vwork_data[dim - 3] = x4[i - 1];
            vwork_data[dim - 2] = x4[b_i1 - 1];
            vwork_data[dim - 1] = x4[b_i2 - 1];
            vwork_data[dim] = x4[b_i3 - 1];
            dim = 0;
          }
        }
      }
      i4 = vwork_size - nNaNs;
      if (dim > 0) {
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
        i1 = (unsigned char)dim;
        for (k = 0; k < i1; k++) {
          i3 = perm[k] - 1;
          i2 = (i4 - dim) + k;
          iidx_data[i2] = idx4[i3];
          vwork_data[i2] = x4[i3];
        }
      }
      dim = nNaNs >> 1;
      for (k = 0; k < dim; k++) {
        i1 = i4 + k;
        i2 = iidx_data[i1];
        i3 = (vwork_size - k) - 1;
        iidx_data[i1] = iidx_data[i3];
        iidx_data[i3] = i2;
        vwork_data[i1] = xwork_data[i3];
        vwork_data[i3] = xwork_data[i1];
      }
      if ((nNaNs & 1) != 0) {
        dim += i4;
        vwork_data[dim] = xwork_data[dim];
      }
      if (i4 > 1) {
        i3 = i4 >> 2;
        i2 = 4;
        while (i3 > 1) {
          if ((i3 & 1) != 0) {
            i3--;
            dim = i2 * i3;
            i1 = i4 - dim;
            if (i1 > i2) {
              merge(iidx_data, vwork_data, dim, i2, i1 - i2, iwork_data,
                    xwork_data);
            }
          }
          dim = i2 << 1;
          i3 >>= 1;
          for (k = 0; k < i3; k++) {
            merge(iidx_data, vwork_data, k * dim, i2, i2, iwork_data,
                  xwork_data);
          }
          i2 = dim;
        }
        if (i4 > i2) {
          if (vwork_size - 1 >= 0) {
            memcpy(&b_iwork_data[0], &iwork_data[0],
                   (unsigned int)vwork_size * sizeof(int));
            memcpy(&x4[0], &xwork_data[0],
                   (unsigned int)vwork_size * sizeof(double));
          }
          merge(iidx_data, vwork_data, 0, i2, i4 - i2, b_iwork_data, x4);
        }
      }
    }
    for (k = 0; k <= vlen; k++) {
      x_data[j + k * vstride] = vwork_data[k];
    }
  }
}

/* End of code generation (sort.c) */
