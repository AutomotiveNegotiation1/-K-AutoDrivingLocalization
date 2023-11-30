//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: exp.cpp
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 30-Nov-2023 16:43:53
//

// Include Files
#include "exp.h"
#include "rt_nonfinite.h"
#include <cmath>

// Function Definitions
//
// Arguments    : creal_T *x
// Return Type  : void
//
namespace coder {
void b_exp(creal_T *x)
{
  if (x->im == 0.0) {
    x->re = std::exp(x->re);
    x->im = 0.0;
  } else if (std::isinf(x->im) && std::isinf(x->re) && (x->re < 0.0)) {
    x->re = 0.0;
    x->im = 0.0;
  } else {
    double b_r;
    double d;
    b_r = std::exp(x->re / 2.0);
    d = x->im;
    x->re = b_r * (b_r * std::cos(x->im));
    x->im = b_r * (b_r * std::sin(d));
  }
}

} // namespace coder

//
// File trailer for exp.cpp
//
// [EOF]
//
