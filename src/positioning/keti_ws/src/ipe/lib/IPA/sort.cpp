//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: sort.cpp
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 30-Nov-2023 16:43:53
//

// Include Files
#include "sort.h"
#include "rt_nonfinite.h"
#include "sortIdx.h"
#include "coder_array.h"
#include <cmath>
#include <cstring>

// Function Definitions
//
// Arguments    : ::coder::array<double, 1U> &x
//                ::coder::array<int, 1U> &idx
// Return Type  : void
//
namespace coder {
namespace internal {
void sort(::coder::array<double, 1U> &x, ::coder::array<int, 1U> &idx)
{
  array<double, 1U> b_xwork;
  array<double, 1U> vwork;
  array<double, 1U> xwork;
  array<int, 1U> b_iwork;
  array<int, 1U> iidx;
  array<int, 1U> iwork;
  int dim;
  int i2;
  int vlen;
  int vstride;
  signed char perm[4];
  dim = 0;
  if (x.size(0) != 1) {
    dim = -1;
  }
  if (dim + 2 <= 1) {
    i2 = x.size(0);
  } else {
    i2 = 1;
  }
  vlen = i2 - 1;
  vwork.set_size(i2);
  idx.set_size(x.size(0));
  vstride = 1;
  for (int b_k{0}; b_k <= dim; b_k++) {
    vstride *= x.size(0);
  }
  for (int j{0}; j < vstride; j++) {
    for (int b_k{0}; b_k <= vlen; b_k++) {
      vwork[b_k] = x[j + b_k * vstride];
    }
    iidx.set_size(vwork.size(0));
    dim = vwork.size(0);
    for (i2 = 0; i2 < dim; i2++) {
      iidx[i2] = 0;
    }
    if (vwork.size(0) != 0) {
      double x4[4];
      int idx4[4];
      int bLen;
      int b_n;
      int i1;
      int i3;
      int i4;
      int iidx_tmp;
      int n;
      int nNonNaN;
      n = vwork.size(0);
      b_n = vwork.size(0);
      x4[0] = 0.0;
      idx4[0] = 0;
      x4[1] = 0.0;
      idx4[1] = 0;
      x4[2] = 0.0;
      idx4[2] = 0;
      x4[3] = 0.0;
      idx4[3] = 0;
      iwork.set_size(vwork.size(0));
      dim = vwork.size(0);
      for (i2 = 0; i2 < dim; i2++) {
        iwork[i2] = 0;
      }
      xwork.set_size(vwork.size(0));
      dim = vwork.size(0);
      for (i2 = 0; i2 < dim; i2++) {
        xwork[i2] = 0.0;
      }
      bLen = 0;
      dim = -1;
      for (int b_k{0}; b_k < b_n; b_k++) {
        if (std::isnan(vwork[b_k])) {
          iidx_tmp = (b_n - bLen) - 1;
          iidx[iidx_tmp] = b_k + 1;
          xwork[iidx_tmp] = vwork[b_k];
          bLen++;
        } else {
          dim++;
          idx4[dim] = b_k + 1;
          x4[dim] = vwork[b_k];
          if (dim + 1 == 4) {
            double d;
            double d1;
            dim = b_k - bLen;
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
                perm[0] = static_cast<signed char>(i1);
                perm[1] = static_cast<signed char>(i2);
                perm[2] = static_cast<signed char>(i3);
                perm[3] = static_cast<signed char>(i4);
              } else if (d <= x4[i4 - 1]) {
                perm[0] = static_cast<signed char>(i1);
                perm[1] = static_cast<signed char>(i3);
                perm[2] = static_cast<signed char>(i2);
                perm[3] = static_cast<signed char>(i4);
              } else {
                perm[0] = static_cast<signed char>(i1);
                perm[1] = static_cast<signed char>(i3);
                perm[2] = static_cast<signed char>(i4);
                perm[3] = static_cast<signed char>(i2);
              }
            } else {
              d1 = x4[i4 - 1];
              if (d <= d1) {
                if (x4[i2 - 1] <= d1) {
                  perm[0] = static_cast<signed char>(i3);
                  perm[1] = static_cast<signed char>(i1);
                  perm[2] = static_cast<signed char>(i2);
                  perm[3] = static_cast<signed char>(i4);
                } else {
                  perm[0] = static_cast<signed char>(i3);
                  perm[1] = static_cast<signed char>(i1);
                  perm[2] = static_cast<signed char>(i4);
                  perm[3] = static_cast<signed char>(i2);
                }
              } else {
                perm[0] = static_cast<signed char>(i3);
                perm[1] = static_cast<signed char>(i4);
                perm[2] = static_cast<signed char>(i1);
                perm[3] = static_cast<signed char>(i2);
              }
            }
            iidx[dim - 3] = idx4[perm[0] - 1];
            iidx[dim - 2] = idx4[perm[1] - 1];
            iidx[dim - 1] = idx4[perm[2] - 1];
            iidx[dim] = idx4[perm[3] - 1];
            vwork[dim - 3] = x4[perm[0] - 1];
            vwork[dim - 2] = x4[perm[1] - 1];
            vwork[dim - 1] = x4[perm[2] - 1];
            vwork[dim] = x4[perm[3] - 1];
            dim = -1;
          }
        }
      }
      i3 = (b_n - bLen) - 1;
      if (dim + 1 > 0) {
        perm[1] = 0;
        perm[2] = 0;
        perm[3] = 0;
        if (dim + 1 == 1) {
          perm[0] = 1;
        } else if (dim + 1 == 2) {
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
        for (int b_k{0}; b_k <= dim; b_k++) {
          iidx_tmp = perm[b_k] - 1;
          i1 = (i3 - dim) + b_k;
          iidx[i1] = idx4[iidx_tmp];
          vwork[i1] = x4[iidx_tmp];
        }
      }
      dim = (bLen >> 1) + 1;
      for (int b_k{0}; b_k <= dim - 2; b_k++) {
        i1 = (i3 + b_k) + 1;
        i2 = iidx[i1];
        iidx_tmp = (b_n - b_k) - 1;
        iidx[i1] = iidx[iidx_tmp];
        iidx[iidx_tmp] = i2;
        vwork[i1] = xwork[iidx_tmp];
        vwork[iidx_tmp] = xwork[i1];
      }
      if ((bLen & 1) != 0) {
        dim += i3;
        vwork[dim] = xwork[dim];
      }
      nNonNaN = n - bLen;
      i1 = 2;
      if (nNonNaN > 1) {
        if (n >= 256) {
          int nBlocks;
          nBlocks = nNonNaN >> 8;
          if (nBlocks > 0) {
            for (int b{0}; b < nBlocks; b++) {
              double c_xwork[256];
              int c_iwork[256];
              i4 = (b << 8) - 1;
              for (int b_b{0}; b_b < 6; b_b++) {
                bLen = 1 << (b_b + 2);
                b_n = bLen << 1;
                n = 256 >> (b_b + 3);
                for (int b_k{0}; b_k < n; b_k++) {
                  i2 = (i4 + b_k * b_n) + 1;
                  for (i1 = 0; i1 < b_n; i1++) {
                    dim = i2 + i1;
                    c_iwork[i1] = iidx[dim];
                    c_xwork[i1] = vwork[dim];
                  }
                  i3 = 0;
                  i1 = bLen;
                  dim = i2 - 1;
                  int exitg1;
                  do {
                    exitg1 = 0;
                    dim++;
                    if (c_xwork[i3] <= c_xwork[i1]) {
                      iidx[dim] = c_iwork[i3];
                      vwork[dim] = c_xwork[i3];
                      if (i3 + 1 < bLen) {
                        i3++;
                      } else {
                        exitg1 = 1;
                      }
                    } else {
                      iidx[dim] = c_iwork[i1];
                      vwork[dim] = c_xwork[i1];
                      if (i1 + 1 < b_n) {
                        i1++;
                      } else {
                        dim -= i3;
                        for (i1 = i3 + 1; i1 <= bLen; i1++) {
                          iidx_tmp = dim + i1;
                          iidx[iidx_tmp] = c_iwork[i1 - 1];
                          vwork[iidx_tmp] = c_xwork[i1 - 1];
                        }
                        exitg1 = 1;
                      }
                    }
                  } while (exitg1 == 0);
                }
              }
            }
            dim = nBlocks << 8;
            i1 = nNonNaN - dim;
            if (i1 > 0) {
              merge_block(iidx, vwork, dim, i1, 2, iwork, xwork);
            }
            i1 = 8;
          }
        }
        dim = iwork.size(0);
        b_iwork.set_size(iwork.size(0));
        for (i2 = 0; i2 < dim; i2++) {
          b_iwork[i2] = iwork[i2];
        }
        b_xwork.set_size(xwork.size(0));
        dim = xwork.size(0);
        for (i2 = 0; i2 < dim; i2++) {
          b_xwork[i2] = xwork[i2];
        }
        merge_block(iidx, vwork, 0, nNonNaN, i1, b_iwork, b_xwork);
      }
    }
    for (int b_k{0}; b_k <= vlen; b_k++) {
      i2 = j + b_k * vstride;
      x[i2] = vwork[b_k];
      idx[i2] = iidx[b_k];
    }
  }
}

//
// Arguments    : double x[16]
//                int idx[16]
// Return Type  : void
//
void sort(double x[16], int idx[16])
{
  double xwork[16];
  double x4[4];
  int iwork[16];
  int i1;
  int i2;
  int i3;
  int ib;
  int nNaNs;
  signed char idx4[4];
  signed char perm[4];
  x4[0] = 0.0;
  idx4[0] = 0;
  x4[1] = 0.0;
  idx4[1] = 0;
  x4[2] = 0.0;
  idx4[2] = 0;
  x4[3] = 0.0;
  idx4[3] = 0;
  std::memset(&idx[0], 0, 16U * sizeof(int));
  std::memset(&xwork[0], 0, 16U * sizeof(double));
  nNaNs = 0;
  ib = 0;
  for (int b_k{0}; b_k < 16; b_k++) {
    if (std::isnan(x[b_k])) {
      idx[15 - nNaNs] = b_k + 1;
      xwork[15 - nNaNs] = x[b_k];
      nNaNs++;
    } else {
      ib++;
      idx4[ib - 1] = static_cast<signed char>(b_k + 1);
      x4[ib - 1] = x[b_k];
      if (ib == 4) {
        double d;
        double d1;
        int i4;
        ib = b_k - nNaNs;
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
            perm[0] = static_cast<signed char>(i1);
            perm[1] = static_cast<signed char>(i2);
            perm[2] = static_cast<signed char>(i3);
            perm[3] = static_cast<signed char>(i4);
          } else if (d <= x4[i4 - 1]) {
            perm[0] = static_cast<signed char>(i1);
            perm[1] = static_cast<signed char>(i3);
            perm[2] = static_cast<signed char>(i2);
            perm[3] = static_cast<signed char>(i4);
          } else {
            perm[0] = static_cast<signed char>(i1);
            perm[1] = static_cast<signed char>(i3);
            perm[2] = static_cast<signed char>(i4);
            perm[3] = static_cast<signed char>(i2);
          }
        } else {
          d1 = x4[i4 - 1];
          if (d <= d1) {
            if (x4[i2 - 1] <= d1) {
              perm[0] = static_cast<signed char>(i3);
              perm[1] = static_cast<signed char>(i1);
              perm[2] = static_cast<signed char>(i2);
              perm[3] = static_cast<signed char>(i4);
            } else {
              perm[0] = static_cast<signed char>(i3);
              perm[1] = static_cast<signed char>(i1);
              perm[2] = static_cast<signed char>(i4);
              perm[3] = static_cast<signed char>(i2);
            }
          } else {
            perm[0] = static_cast<signed char>(i3);
            perm[1] = static_cast<signed char>(i4);
            perm[2] = static_cast<signed char>(i1);
            perm[3] = static_cast<signed char>(i2);
          }
        }
        idx[ib - 3] = idx4[perm[0] - 1];
        idx[ib - 2] = idx4[perm[1] - 1];
        idx[ib - 1] = idx4[perm[2] - 1];
        idx[ib] = idx4[perm[3] - 1];
        x[ib - 3] = x4[perm[0] - 1];
        x[ib - 2] = x4[perm[1] - 1];
        x[ib - 1] = x4[perm[2] - 1];
        x[ib] = x4[perm[3] - 1];
        ib = 0;
      }
    }
  }
  if (ib > 0) {
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
    for (int b_k{0}; b_k < ib; b_k++) {
      i2 = perm[b_k] - 1;
      i1 = ((b_k - nNaNs) - ib) + 16;
      idx[i1] = idx4[i2];
      x[i1] = x4[i2];
    }
  }
  ib = (nNaNs >> 1) + 16;
  for (int b_k{0}; b_k <= ib - 17; b_k++) {
    i2 = (b_k - nNaNs) + 16;
    i1 = idx[i2];
    idx[i2] = idx[15 - b_k];
    idx[15 - b_k] = i1;
    x[i2] = xwork[15 - b_k];
    x[15 - b_k] = xwork[i2];
  }
  if ((nNaNs & 1) != 0) {
    ib -= nNaNs;
    x[ib] = xwork[ib];
  }
  if (16 - nNaNs > 1) {
    std::memset(&iwork[0], 0, 16U * sizeof(int));
    i3 = (16 - nNaNs) >> 2;
    i1 = 4;
    while (i3 > 1) {
      if ((i3 & 1) != 0) {
        i3--;
        ib = i1 * i3;
        i2 = 16 - (nNaNs + ib);
        if (i2 > i1) {
          merge(idx, x, ib, i1, i2 - i1, iwork, xwork);
        }
      }
      ib = i1 << 1;
      i3 >>= 1;
      for (int b_k{0}; b_k < i3; b_k++) {
        merge(idx, x, b_k * ib, i1, i1, iwork, xwork);
      }
      i1 = ib;
    }
    if (16 - nNaNs > i1) {
      merge(idx, x, 0, i1, 16 - (nNaNs + i1), iwork, xwork);
    }
  }
}

} // namespace internal
} // namespace coder

//
// File trailer for sort.cpp
//
// [EOF]
//
