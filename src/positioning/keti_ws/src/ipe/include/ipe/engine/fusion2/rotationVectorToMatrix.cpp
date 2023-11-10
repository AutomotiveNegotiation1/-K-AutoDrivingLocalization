//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: rotationVectorToMatrix.cpp
//
// MATLAB Coder version            : 5.6
// C/C++ source code generated on  : 09-Nov-2023 17:48:28
//

// Include Files
#include "rotationVectorToMatrix.h"
#include "rt_nonfinite.h"
#include <cmath>
#include <cstring>

// Function Definitions
//
// Arguments    : const double rotationVector[3]
//                double rotationMatrix[9]
// Return Type  : void
//
namespace coder {
void rotationVectorToMatrix(const double rotationVector[3],
                            double rotationMatrix[9])
{
  double a[9];
  double absxk;
  double scale;
  double t;
  double theta;
  scale = 3.3121686421112381E-170;
  absxk = std::abs(rotationVector[0]);
  if (absxk > 3.3121686421112381E-170) {
    theta = 1.0;
    scale = absxk;
  } else {
    t = absxk / 3.3121686421112381E-170;
    theta = t * t;
  }
  absxk = std::abs(rotationVector[1]);
  if (absxk > scale) {
    t = scale / absxk;
    theta = theta * t * t + 1.0;
    scale = absxk;
  } else {
    t = absxk / scale;
    theta += t * t;
  }
  absxk = std::abs(rotationVector[2]);
  if (absxk > scale) {
    t = scale / absxk;
    theta = theta * t * t + 1.0;
    scale = absxk;
  } else {
    t = absxk / scale;
    theta += t * t;
  }
  theta = scale * std::sqrt(theta);
  if (theta < 1.0E-6) {
    std::memset(&a[0], 0, 9U * sizeof(double));
    a[0] = 1.0;
    a[4] = 1.0;
    a[8] = 1.0;
  } else {
    double b_u[9];
    double u[3];
    signed char b_a[9];
    u[0] = rotationVector[0] / theta;
    u[1] = rotationVector[1] / theta;
    u[2] = rotationVector[2] / theta;
    absxk = std::cos(theta);
    scale = std::sin(theta);
    for (int b_k{0}; b_k < 9; b_k++) {
      b_a[b_k] = 0;
    }
    a[0] = scale * 0.0;
    a[3] = scale * -u[2];
    a[6] = scale * u[1];
    a[1] = scale * u[2];
    a[4] = scale * 0.0;
    a[7] = scale * -u[0];
    a[2] = scale * -u[1];
    a[5] = scale * u[0];
    a[8] = scale * 0.0;
    for (int b_k{0}; b_k < 3; b_k++) {
      b_a[b_k + 3 * b_k] = 1;
      b_u[3 * b_k] = u[0] * u[b_k];
      b_u[3 * b_k + 1] = u[1] * u[b_k];
      b_u[3 * b_k + 2] = u[2] * u[b_k];
    }
    for (int b_k{0}; b_k < 9; b_k++) {
      a[b_k] = (static_cast<double>(b_a[b_k]) * absxk + a[b_k]) +
               (1.0 - absxk) * b_u[b_k];
    }
  }
  for (int b_k{0}; b_k < 3; b_k++) {
    rotationMatrix[3 * b_k] = a[b_k];
    rotationMatrix[3 * b_k + 1] = a[b_k + 3];
    rotationMatrix[3 * b_k + 2] = a[b_k + 6];
  }
}

} // namespace coder

//
// File trailer for rotationVectorToMatrix.cpp
//
// [EOF]
//
