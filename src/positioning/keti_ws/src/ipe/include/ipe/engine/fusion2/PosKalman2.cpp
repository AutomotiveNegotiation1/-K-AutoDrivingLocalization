//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: PosKalman2.cpp
//
// MATLAB Coder version            : 5.6
// C/C++ source code generated on  : 09-Nov-2023 17:48:28
//

// Include Files
#include "PosKalman2.h"
#include "fusion2_data.h"
#include "rt_nonfinite.h"
#include <algorithm>
#include <cstring>

// Function Definitions
//
// Arguments    : void
// Return Type  : void
//
void PosKalman2_init()
{
  static const double b_dv[144]{
      0.01, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0,  0.01, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0,  0.0,  0.01, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0,  0.0,  0.0,  0.01, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0,  0.0,  0.0,  0.0,  0.01, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0,  0.0,  0.0,  0.0,  0.0,  0.01, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
      0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.01, 0.0,  0.0,  0.0,  0.0,  0.0,
      0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.01, 0.0,  0.0,  0.0,  0.0,
      0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.01, 0.0,  0.0,  0.0,
      0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.01, 0.0,  0.0,
      0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.01, 0.0,
      0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.01};
  static const signed char y[9]{1, 0, 0, 0, 1, 0, 0, 0, 1};
  int Q_tmp;
  std::memset(&b_H[0], 0, 36U * sizeof(double));
  //      H(2,2) = 1; H(3:6,3:6) = eye(4);
  for (int i{0}; i < 3; i++) {
    b_H[3 * i] = 0.0;
    b_H[3 * i + 1] = 0.0;
    b_H[3 * i + 2] = 0.0;
  }
  b_H[0] = 1.0;
  b_H[4] = 1.0;
  b_H[8] = 1.0;
  std::copy(&b_dv[0], &b_dv[144], &b_Q[0]);
  for (int i{0}; i < 3; i++) {
    Q_tmp = 12 * (i + 9);
    b_Q[Q_tmp + 9] = dv[3 * i];
    b_Q[Q_tmp + 10] = dv[3 * i + 1];
    b_Q[Q_tmp + 11] = dv[3 * i + 2];
  }
  // Q(10:12,10:12)=1e-5*eye(3);
  for (int i{0}; i < 144; i++) {
    b_P[i] = iv[i];
  }
  for (int i{0}; i < 3; i++) {
    Q_tmp = 12 * (i + 3);
    b_P[Q_tmp] = y[3 * i];
    b_P[Q_tmp + 1] = y[3 * i + 1];
    b_P[Q_tmp + 2] = y[3 * i + 2];
  }
  for (int i{0}; i < 6; i++) {
    for (Q_tmp = 0; Q_tmp < 6; Q_tmp++) {
      b_P[Q_tmp + 12 * (i + 6)] = 1.0;
    }
  }
  for (int i{0}; i < 3; i++) {
    Q_tmp = 12 * (i + 9);
    b_P[Q_tmp + 9] = y[3 * i];
    b_P[Q_tmp + 10] = y[3 * i + 1];
    b_P[Q_tmp + 11] = y[3 * i + 2];
  }
}

//
// File trailer for PosKalman2.cpp
//
// [EOF]
//
