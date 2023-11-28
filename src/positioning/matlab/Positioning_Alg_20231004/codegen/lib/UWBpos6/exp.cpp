//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: exp.cpp
//
// MATLAB Coder version            : 5.6
// C/C++ source code generated on  : 17-Oct-2023 17:18:56
//

// Include Files
#include "exp.h"
#include "rt_nonfinite.h"
#include <cmath>

// Function Definitions
//
// Arguments    : creal_T &x
// Return Type  : void
//
namespace coder {
void b_exp(creal_T &x)
{
  if (x.re == 0.0) {
    double d;
    d = x.im;
    x.re = std::cos(d);
    x.im = std::sin(d);
  } else if (x.im == 0.0) {
    x.re = std::exp(x.re);
    x.im = 0.0;
  } else if (std::isinf(x.im) && std::isinf(x.re) && (x.re < 0.0)) {
    x.re = 0.0;
    x.im = 0.0;
  } else {
    double d;
    double r;
    r = std::exp(x.re / 2.0);
    d = x.im;
    x.re = r * (r * std::cos(d));
    x.im = r * (r * std::sin(d));
  }
}

} // namespace coder

//
// File trailer for exp.cpp
//
// [EOF]
//
