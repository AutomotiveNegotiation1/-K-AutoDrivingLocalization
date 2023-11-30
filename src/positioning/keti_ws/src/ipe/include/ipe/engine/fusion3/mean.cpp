//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: mean.cpp
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 30-Nov-2023 16:43:53
//

// Include Files
#include "mean.h"
#include "fusion2_data.h"
#include "rt_nonfinite.h"
#include "coder_array.h"

// Function Definitions
//
// Arguments    : const creal_T x[3]
// Return Type  : creal_T
//
namespace coder {
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

//
// Arguments    : const double x[3]
// Return Type  : double
//
double mean(const double x[3])
{
  return ((x[0] + x[1]) + x[2]) / 3.0;
}

//
// Arguments    : const ::coder::array<creal_T, 2U> &x
// Return Type  : creal_T
//
creal_T mean(const ::coder::array<creal_T, 2U> &x)
{
  creal_T y;
  double x_im;
  double x_re;
  if (x.size(1) == 0) {
    x_re = 0.0;
    x_im = 0.0;
  } else {
    int firstBlockLength;
    int lastBlockLength;
    int nblocks;
    if (x.size(1) <= 1024) {
      firstBlockLength = x.size(1);
      lastBlockLength = 0;
      nblocks = 1;
    } else {
      firstBlockLength = 1024;
      nblocks = x.size(1) / 1024;
      lastBlockLength = x.size(1) - (nblocks << 10);
      if (lastBlockLength > 0) {
        nblocks++;
      } else {
        lastBlockLength = 1024;
      }
    }
    x_re = x[0].re;
    x_im = x[0].im;
    for (int b_k{2}; b_k <= firstBlockLength; b_k++) {
      x_re += x[b_k - 1].re;
      x_im += x[b_k - 1].im;
    }
    for (int ib{2}; ib <= nblocks; ib++) {
      double bsum_im;
      double bsum_re;
      int hi;
      firstBlockLength = (ib - 1) << 10;
      bsum_re = x[firstBlockLength].re;
      bsum_im = x[firstBlockLength].im;
      if (ib == nblocks) {
        hi = lastBlockLength;
      } else {
        hi = 1024;
      }
      for (int b_k{2}; b_k <= hi; b_k++) {
        int bsum_re_tmp;
        bsum_re_tmp = (firstBlockLength + b_k) - 1;
        bsum_re += x[bsum_re_tmp].re;
        bsum_im += x[bsum_re_tmp].im;
      }
      x_re += bsum_re;
      x_im += bsum_im;
    }
  }
  if (x_im == 0.0) {
    y.re = x_re / static_cast<double>(x.size(1));
    y.im = 0.0;
  } else if (x_re == 0.0) {
    y.re = 0.0;
    y.im = x_im / static_cast<double>(x.size(1));
  } else {
    y.re = x_re / static_cast<double>(x.size(1));
    y.im = x_im / static_cast<double>(x.size(1));
  }
  return y;
}

} // namespace coder

//
// File trailer for mean.cpp
//
// [EOF]
//
