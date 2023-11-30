//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: PosKalman2.cpp
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 30-Nov-2023 16:43:53
//

// Include Files
#include "PosKalman2.h"
#include "fusion2_data.h"
#include "fusion2_initialize.h"
#include "rt_nonfinite.h"
#include <algorithm>
#include <cmath>
#include <cstring>

// Variable Definitions
static double b_H[36];

static double b_Q[144];

static double b_P[144];

// Function Definitions
//
// Arguments    : double x[12]
//                const double A[144]
//                const double z[3]
//                const double b_R[9]
// Return Type  : void
//
void PosKalman2(double x[12], const double A[144], const double z[3],
                const double b_R[9])
{
  double Pp[144];
  double b_A[144];
  double K[36];
  double b_Pp[36];
  double xp[12];
  double b_x[9];
  double c_x[9];
  double b_z[3];
  double absx11;
  double absx21;
  double absx31;
  int itmp;
  int p1;
  int p2;
  int p3;
  if (!isInitialized_fusion2) {
    fusion2_initialize();
  }
  //  12x1,
  for (p3 = 0; p3 < 12; p3++) {
    xp[p3] = 0.0;
    for (itmp = 0; itmp < 12; itmp++) {
      p1 = p3 + 12 * itmp;
      xp[p3] += A[p1] * x[itmp];
      absx11 = 0.0;
      for (p2 = 0; p2 < 12; p2++) {
        absx11 += A[p3 + 12 * p2] * b_P[p2 + 12 * itmp];
      }
      b_A[p1] = absx11;
    }
    for (itmp = 0; itmp < 12; itmp++) {
      absx11 = 0.0;
      for (p2 = 0; p2 < 12; p2++) {
        absx11 += b_A[p3 + 12 * p2] * A[itmp + 12 * p2];
      }
      p1 = p3 + 12 * itmp;
      Pp[p1] = absx11 + b_Q[p1];
    }
  }
  for (p3 = 0; p3 < 3; p3++) {
    for (itmp = 0; itmp < 12; itmp++) {
      p1 = p3 + 3 * itmp;
      K[itmp + 12 * p3] = b_H[p1];
      absx11 = 0.0;
      for (p2 = 0; p2 < 12; p2++) {
        absx11 += b_H[p3 + 3 * p2] * Pp[p2 + 12 * itmp];
      }
      b_Pp[p1] = absx11;
    }
  }
  for (p3 = 0; p3 < 3; p3++) {
    for (itmp = 0; itmp < 3; itmp++) {
      absx11 = 0.0;
      for (p2 = 0; p2 < 12; p2++) {
        absx11 += b_Pp[p3 + 3 * p2] * K[p2 + 12 * itmp];
      }
      p1 = p3 + 3 * itmp;
      b_x[p1] = absx11 + b_R[p1];
    }
  }
  std::copy(&b_x[0], &b_x[9], &c_x[0]);
  p1 = 0;
  p2 = 3;
  p3 = 6;
  absx11 = std::abs(b_x[0]);
  absx21 = std::abs(b_x[1]);
  absx31 = std::abs(b_x[2]);
  if ((absx21 > absx11) && (absx21 > absx31)) {
    p1 = 3;
    p2 = 0;
    c_x[0] = b_x[1];
    c_x[1] = b_x[0];
    c_x[3] = b_x[4];
    c_x[4] = b_x[3];
    c_x[6] = b_x[7];
    c_x[7] = b_x[6];
  } else if (absx31 > absx11) {
    p1 = 6;
    p3 = 0;
    c_x[0] = b_x[2];
    c_x[2] = b_x[0];
    c_x[3] = b_x[5];
    c_x[5] = b_x[3];
    c_x[6] = b_x[8];
    c_x[8] = b_x[6];
  }
  c_x[1] /= c_x[0];
  c_x[2] /= c_x[0];
  c_x[4] -= c_x[1] * c_x[3];
  c_x[5] -= c_x[2] * c_x[3];
  c_x[7] -= c_x[1] * c_x[6];
  c_x[8] -= c_x[2] * c_x[6];
  if (std::abs(c_x[5]) > std::abs(c_x[4])) {
    itmp = p2;
    p2 = p3;
    p3 = itmp;
    absx11 = c_x[1];
    c_x[1] = c_x[2];
    c_x[2] = absx11;
    absx11 = c_x[4];
    c_x[4] = c_x[5];
    c_x[5] = absx11;
    absx11 = c_x[7];
    c_x[7] = c_x[8];
    c_x[8] = absx11;
  }
  c_x[5] /= c_x[4];
  c_x[8] -= c_x[5] * c_x[7];
  absx11 = (c_x[1] * c_x[5] - c_x[2]) / c_x[8];
  absx21 = -(c_x[1] + c_x[7] * absx11) / c_x[4];
  b_x[p1] = ((1.0 - c_x[3] * absx21) - c_x[6] * absx11) / c_x[0];
  b_x[p1 + 1] = absx21;
  b_x[p1 + 2] = absx11;
  absx11 = -c_x[5] / c_x[8];
  absx21 = (1.0 - c_x[7] * absx11) / c_x[4];
  b_x[p2] = -(c_x[3] * absx21 + c_x[6] * absx11) / c_x[0];
  b_x[p2 + 1] = absx21;
  b_x[p2 + 2] = absx11;
  absx11 = 1.0 / c_x[8];
  absx21 = -c_x[7] * absx11 / c_x[4];
  b_x[p3] = -(c_x[3] * absx21 + c_x[6] * absx11) / c_x[0];
  b_x[p3 + 1] = absx21;
  b_x[p3 + 2] = absx11;
  for (p3 = 0; p3 < 12; p3++) {
    for (itmp = 0; itmp < 3; itmp++) {
      absx11 = 0.0;
      for (p2 = 0; p2 < 12; p2++) {
        absx11 += Pp[p3 + 12 * p2] * K[p2 + 12 * itmp];
      }
      b_Pp[p3 + 12 * itmp] = absx11;
    }
  }
  for (p3 = 0; p3 < 12; p3++) {
    absx11 = b_Pp[p3];
    absx21 = b_Pp[p3 + 12];
    absx31 = b_Pp[p3 + 24];
    for (itmp = 0; itmp < 3; itmp++) {
      K[p3 + 12 * itmp] =
          (absx11 * b_x[3 * itmp] + absx21 * b_x[3 * itmp + 1]) +
          absx31 * b_x[3 * itmp + 2];
    }
  }
  for (p3 = 0; p3 < 3; p3++) {
    absx11 = 0.0;
    for (itmp = 0; itmp < 12; itmp++) {
      absx11 += b_H[p3 + 3 * itmp] * xp[itmp];
    }
    b_z[p3] = z[p3] - absx11;
  }
  for (p3 = 0; p3 < 12; p3++) {
    absx11 = K[p3 + 12];
    absx21 = K[p3 + 24];
    x[p3] = xp[p3] + ((K[p3] * b_z[0] + absx11 * b_z[1]) + absx21 * b_z[2]);
    for (itmp = 0; itmp < 12; itmp++) {
      b_A[p3 + 12 * itmp] =
          (K[p3] * b_H[3 * itmp] + absx11 * b_H[3 * itmp + 1]) +
          absx21 * b_H[3 * itmp + 2];
    }
    for (itmp = 0; itmp < 12; itmp++) {
      absx11 = 0.0;
      for (p2 = 0; p2 < 12; p2++) {
        absx11 += b_A[p3 + 12 * p2] * Pp[p2 + 12 * itmp];
      }
      p1 = p3 + 12 * itmp;
      b_P[p1] = Pp[p1] - absx11;
    }
  }
}

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
