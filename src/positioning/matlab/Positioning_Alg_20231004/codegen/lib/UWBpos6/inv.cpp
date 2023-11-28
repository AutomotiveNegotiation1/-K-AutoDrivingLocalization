//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: inv.cpp
//
// MATLAB Coder version            : 5.6
// C/C++ source code generated on  : 17-Oct-2023 17:18:56
//

// Include Files
#include "inv.h"
#include "rt_nonfinite.h"
#include <cmath>

// Function Definitions
//
// Arguments    : const double x[4]
//                double y[4]
// Return Type  : void
//
namespace coder {
void inv(const double x[4], double y[4])
{
  if (std::abs(x[1]) > std::abs(x[0])) {
    double r;
    double t;
    r = x[0] / x[1];
    t = 1.0 / (r * x[3] - x[2]);
    y[0] = x[3] / x[1] * t;
    y[1] = -t;
    y[2] = -x[2] / x[1] * t;
    y[3] = r * t;
  } else {
    double r;
    double t;
    r = x[1] / x[0];
    t = 1.0 / (x[3] - r * x[2]);
    y[0] = x[3] / x[0] * t;
    y[1] = -r * t;
    y[2] = -x[2] / x[0] * t;
    y[3] = t;
  }
}

} // namespace coder

//
// File trailer for inv.cpp
//
// [EOF]
//
