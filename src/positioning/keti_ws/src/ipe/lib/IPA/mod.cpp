//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: mod.cpp
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 30-Nov-2023 16:43:53
//

// Include Files
#include "mod.h"
#include "rt_nonfinite.h"
#include <cmath>

// Function Definitions
//
// Arguments    : double x
// Return Type  : double
//
namespace coder {
double b_mod(double x)
{
  double b_r;
  if (std::isnan(x) || std::isinf(x)) {
    b_r = rtNaN;
  } else if (x == 0.0) {
    b_r = 0.0;
  } else {
    bool rEQ0;
    b_r = std::fmod(x, 6.2831853071795862);
    rEQ0 = (b_r == 0.0);
    if (!rEQ0) {
      double q;
      q = std::abs(x / 6.2831853071795862);
      rEQ0 = !(std::abs(q - std::floor(q + 0.5)) > 2.2204460492503131E-16 * q);
    }
    if (rEQ0) {
      b_r = 0.0;
    } else if (x < 0.0) {
      b_r += 6.2831853071795862;
    }
  }
  return b_r;
}

} // namespace coder

//
// File trailer for mod.cpp
//
// [EOF]
//
