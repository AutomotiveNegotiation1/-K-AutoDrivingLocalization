//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: combineVectorElements.cpp
//
// MATLAB Coder version            : 5.6
// C/C++ source code generated on  : 09-Nov-2023 17:48:28
//

// Include Files
#include "combineVectorElements.h"
#include "rt_nonfinite.h"
#include "coder_array.h"

// Function Definitions
//
// Arguments    : const ::coder::array<creal_T, 3U> &x
// Return Type  : creal_T
//
namespace coder {
creal_T combineVectorElements(const ::coder::array<creal_T, 3U> &x)
{
  creal_T y;
  if (x.size(2) == 0) {
    y.re = 0.0;
    y.im = 0.0;
  } else {
    int firstBlockLength;
    int lastBlockLength;
    int nblocks;
    if (x.size(2) <= 1024) {
      firstBlockLength = x.size(2);
      lastBlockLength = 0;
      nblocks = 1;
    } else {
      firstBlockLength = 1024;
      nblocks = static_cast<int>(static_cast<unsigned int>(x.size(2)) >> 10);
      lastBlockLength = x.size(2) - (nblocks << 10);
      if (lastBlockLength > 0) {
        nblocks++;
      } else {
        lastBlockLength = 1024;
      }
    }
    y = x[0];
    for (int b_k{2}; b_k <= firstBlockLength; b_k++) {
      y.re += x[b_k - 1].re;
      y.im += x[b_k - 1].im;
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
      y.re += bsum_re;
      y.im += bsum_im;
    }
  }
  return y;
}

} // namespace coder

//
// File trailer for combineVectorElements.cpp
//
// [EOF]
//
