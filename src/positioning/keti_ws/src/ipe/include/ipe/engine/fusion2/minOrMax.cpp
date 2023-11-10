//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: minOrMax.cpp
//
// MATLAB Coder version            : 5.6
// C/C++ source code generated on  : 09-Nov-2023 17:48:28
//

// Include Files
#include "minOrMax.h"
#include "rt_nonfinite.h"
#include <cmath>

// Function Definitions
//
// Arguments    : const double x[3]
//                int &idx
// Return Type  : double
//
namespace coder {
namespace internal {
double minimum(const double x[3], int &idx)
{
  double ex;
  int b_k;
  if (!std::isnan(x[0])) {
    idx = 1;
  } else {
    bool exitg1;
    idx = 0;
    b_k = 2;
    exitg1 = false;
    while ((!exitg1) && (b_k < 4)) {
      if (!std::isnan(x[b_k - 1])) {
        idx = b_k;
        exitg1 = true;
      } else {
        b_k++;
      }
    }
  }
  if (idx == 0) {
    ex = x[0];
    idx = 1;
  } else {
    int i;
    ex = x[idx - 1];
    i = idx + 1;
    for (b_k = i; b_k < 4; b_k++) {
      double d;
      d = x[b_k - 1];
      if (ex > d) {
        ex = d;
        idx = b_k;
      }
    }
  }
  return ex;
}

} // namespace internal
} // namespace coder

//
// File trailer for minOrMax.cpp
//
// [EOF]
//
