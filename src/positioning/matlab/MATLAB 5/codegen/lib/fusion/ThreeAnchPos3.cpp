//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: ThreeAnchPos3.cpp
//
// MATLAB Coder version            : 5.6
// C/C++ source code generated on  : 17-Oct-2023 17:24:31
//

// Include Files
#include "ThreeAnchPos3.h"
#include "inv.h"
#include "rt_nonfinite.h"
#include <cmath>

// Function Definitions
//
// Arguments    : const double xa[4]
//                const double ya[4]
//                const double dist[4]
//                double Pos[2]
//                double Prob[4]
// Return Type  : void
//
void ThreeAnchPos3(const double xa[4], const double ya[4], const double dist[4],
                   double Pos[2], double Prob[4])
{
  double b_y[16];
  double x[16];
  double A[6];
  double y_tmp[6];
  double a[4];
  double b_y_tmp[4];
  double y[3];
  double d;
  double d1;
  double d2;
  double d3;
  double x_tmp;
  int c_k;
  int xpageoffset;
  //  if (ya(2)-y(1))*(xa(3)-xa(1)) ~= (ya(3)-y(1))*(xa(2)-xa(1))
  //  else
  //
  //  end
  //  Pos = (A'*A)\(A'*y);
  for (c_k = 0; c_k < 3; c_k++) {
    d = xa[c_k];
    d1 = -2.0 * (d - xa[3]);
    A[c_k] = d1;
    d2 = ya[c_k];
    d3 = -2.0 * (d2 - ya[3]);
    A[c_k + 3] = d3;
    x_tmp = dist[c_k];
    y[c_k] = ((x_tmp * x_tmp - dist[3] * dist[3]) - (d * d - xa[3] * xa[3])) -
             (d2 * d2 - ya[3] * ya[3]);
    xpageoffset = c_k << 1;
    y_tmp[xpageoffset] = d1;
    y_tmp[xpageoffset + 1] = d3;
  }
  for (int i{0}; i < 2; i++) {
    d = y_tmp[i];
    d1 = y_tmp[i + 2];
    d2 = y_tmp[i + 4];
    for (c_k = 0; c_k < 2; c_k++) {
      b_y_tmp[i + (c_k << 1)] =
          (d * A[3 * c_k] + d1 * A[3 * c_k + 1]) + d2 * A[3 * c_k + 2];
    }
  }
  coder::b_inv(b_y_tmp, a);
  for (int i{0}; i < 2; i++) {
    d = 0.0;
    d1 = a[i];
    d2 = a[i + 2];
    for (c_k = 0; c_k < 3; c_k++) {
      xpageoffset = c_k << 1;
      d += (d1 * y_tmp[xpageoffset] + d2 * y_tmp[xpageoffset + 1]) * y[c_k];
    }
    Pos[i] = d;
  }
  d = xa[0] - Pos[0];
  b_y_tmp[0] = d * d;
  d = ya[0] - Pos[1];
  a[0] = d * d;
  d = xa[1] - Pos[0];
  b_y_tmp[1] = d * d;
  d = ya[1] - Pos[1];
  a[1] = d * d;
  d = xa[2] - Pos[0];
  b_y_tmp[2] = d * d;
  d = ya[2] - Pos[1];
  a[2] = d * d;
  d = xa[3] - Pos[0];
  b_y_tmp[3] = d * d;
  d = ya[3] - Pos[1];
  a[3] = d * d;
  d = dist[0] * dist[0];
  d1 = dist[1] * dist[1];
  d2 = dist[2] * dist[2];
  d3 = dist[3] * dist[3];
  for (int i{0}; i < 4; i++) {
    x_tmp = b_y_tmp[i] + a[i];
    c_k = i << 2;
    x[c_k] = x_tmp - d;
    x[c_k + 1] = x_tmp - d1;
    x[c_k + 2] = x_tmp - d2;
    x[c_k + 3] = x_tmp - d3;
  }
  for (c_k = 0; c_k < 16; c_k++) {
    b_y[c_k] = std::abs(x[c_k]);
  }
  for (c_k = 0; c_k < 4; c_k++) {
    xpageoffset = c_k << 2;
    Prob[c_k] = std::sqrt(
        (((b_y[xpageoffset] + b_y[xpageoffset + 1]) + b_y[xpageoffset + 2]) +
         b_y[xpageoffset + 3]) /
        4.0);
  }
}

//
// Arguments    : const double xa[3]
//                const double ya[3]
//                const double dist[3]
//                double Pos[2]
//                double Prob[3]
// Return Type  : void
//
void b_ThreeAnchPos3(const double xa[3], const double ya[3],
                     const double dist[3], double Pos[2], double Prob[3])
{
  double c_y[9];
  double x[9];
  double A[4];
  double b_y_tmp[4];
  double y_tmp[4];
  double b_y[3];
  double y[3];
  double b_y_idx_0_tmp;
  double c_y_idx_0_tmp;
  double d;
  double d1;
  double d2;
  double d3;
  double d4;
  double y_idx_0;
  double y_idx_0_tmp;
  double y_idx_1;
  double y_idx_1_tmp;
  //  if (ya(2)-y(1))*(xa(3)-xa(1)) ~= (ya(3)-y(1))*(xa(2)-xa(1))
  //  else
  //
  //  end
  //  Pos = (A'*A)\(A'*y);
  A[0] = -2.0 * (xa[0] - xa[2]);
  A[2] = -2.0 * (ya[0] - ya[2]);
  y_idx_0_tmp = dist[2] * dist[2];
  y_idx_1 = xa[2] * xa[2];
  b_y_idx_0_tmp = ya[2] * ya[2];
  c_y_idx_0_tmp = dist[0] * dist[0];
  y_idx_0 = ((c_y_idx_0_tmp - y_idx_0_tmp) - (xa[0] * xa[0] - y_idx_1)) -
            (ya[0] * ya[0] - b_y_idx_0_tmp);
  y_tmp[0] = A[0];
  y_tmp[1] = A[2];
  A[1] = -2.0 * (xa[1] - xa[2]);
  A[3] = -2.0 * (ya[1] - ya[2]);
  y_idx_1_tmp = dist[1] * dist[1];
  y_idx_1 = ((y_idx_1_tmp - y_idx_0_tmp) - (xa[1] * xa[1] - y_idx_1)) -
            (ya[1] * ya[1] - b_y_idx_0_tmp);
  y_tmp[2] = A[1];
  y_tmp[3] = A[3];
  b_y_idx_0_tmp = A[0];
  d = A[1];
  d1 = A[2];
  d2 = A[3];
  for (int c_k{0}; c_k < 2; c_k++) {
    d3 = y_tmp[c_k + 2];
    d4 = y_tmp[c_k];
    b_y_tmp[c_k] = d4 * b_y_idx_0_tmp + d3 * d;
    b_y_tmp[c_k + 2] = d4 * d1 + d3 * d2;
  }
  coder::b_inv(b_y_tmp, A);
  b_y_idx_0_tmp = y_tmp[0];
  d = y_tmp[1];
  d1 = y_tmp[2];
  d2 = y_tmp[3];
  for (int c_k{0}; c_k < 2; c_k++) {
    d3 = A[c_k + 2];
    d4 = A[c_k];
    Pos[c_k] =
        (d4 * b_y_idx_0_tmp + d3 * d) * y_idx_0 + (d4 * d1 + d3 * d2) * y_idx_1;
  }
  b_y_idx_0_tmp = xa[0] - Pos[0];
  y[0] = b_y_idx_0_tmp * b_y_idx_0_tmp;
  b_y_idx_0_tmp = ya[0] - Pos[1];
  b_y[0] = b_y_idx_0_tmp * b_y_idx_0_tmp;
  b_y_idx_0_tmp = xa[1] - Pos[0];
  y[1] = b_y_idx_0_tmp * b_y_idx_0_tmp;
  b_y_idx_0_tmp = ya[1] - Pos[1];
  b_y[1] = b_y_idx_0_tmp * b_y_idx_0_tmp;
  b_y_idx_0_tmp = xa[2] - Pos[0];
  y[2] = b_y_idx_0_tmp * b_y_idx_0_tmp;
  b_y_idx_0_tmp = ya[2] - Pos[1];
  b_y[2] = b_y_idx_0_tmp * b_y_idx_0_tmp;
  for (int c_k{0}; c_k < 3; c_k++) {
    y_idx_1 = y[c_k] + b_y[c_k];
    x[3 * c_k] = y_idx_1 - c_y_idx_0_tmp;
    x[3 * c_k + 1] = y_idx_1 - y_idx_1_tmp;
    x[3 * c_k + 2] = y_idx_1 - y_idx_0_tmp;
  }
  for (int c_k{0}; c_k < 9; c_k++) {
    c_y[c_k] = std::abs(x[c_k]);
  }
  for (int c_k{0}; c_k < 3; c_k++) {
    int xpageoffset;
    xpageoffset = c_k * 3;
    Prob[c_k] = std::sqrt(
        ((c_y[xpageoffset] + c_y[xpageoffset + 1]) + c_y[xpageoffset + 2]) /
        3.0);
  }
}

//
// File trailer for ThreeAnchPos3.cpp
//
// [EOF]
//
