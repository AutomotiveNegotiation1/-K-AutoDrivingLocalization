//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: mean.cpp
//
// MATLAB Coder version            : 5.6
// C/C++ source code generated on  : 17-Oct-2023 17:24:31
//

// Include Files
#include "mean.h"
#include "fusion_data.h"
#include "rt_nonfinite.h"
#include "coder_array.h"

// Function Definitions
//
// Arguments    : const ::coder::array<creal_T, 2U> &x
// Return Type  : creal_T
//
namespace coder {
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
      nblocks = static_cast<int>(static_cast<unsigned int>(x.size(1)) >> 10);
      lastBlockLength = x.size(1) - (nblocks << 10);
      if (lastBlockLength > 0) {
        nblocks++;
      } else {
        lastBlockLength = 1024;
      }
    }
    x_re = x[0].re;
    x_im = x[0].im;
    for (int c_k{2}; c_k <= firstBlockLength; c_k++) {
      x_re += x[c_k - 1].re;
      x_im += x[c_k - 1].im;
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
      for (int c_k{2}; c_k <= hi; c_k++) {
        int bsum_re_tmp;
        bsum_re_tmp = (firstBlockLength + c_k) - 1;
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
