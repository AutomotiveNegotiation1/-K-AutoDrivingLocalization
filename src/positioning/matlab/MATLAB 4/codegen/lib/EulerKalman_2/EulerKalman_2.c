/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: EulerKalman_2.c
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 22-Sep-2023 10:38:00
 */

/* Include Files */
#include "EulerKalman_2.h"
#include "EulerKalman_2_data.h"
#include "EulerKalman_2_initialize.h"
#include "rt_nonfinite.h"
#include "rt_defines.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Variable Definitions */
static double H[16];

static double Q[16];

static double R[16];

static double P[16];

/* Function Declarations */
static double rt_atan2d_snf(double u0, double u1);

/* Function Definitions */
/*
 * Arguments    : double u0
 *                double u1
 * Return Type  : double
 */
static double rt_atan2d_snf(double u0, double u1)
{
  double y;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = rtNaN;
  } else if (rtIsInf(u0) && rtIsInf(u1)) {
    int i;
    int i1;
    if (u0 > 0.0) {
      i = 1;
    } else {
      i = -1;
    }
    if (u1 > 0.0) {
      i1 = 1;
    } else {
      i1 = -1;
    }
    y = atan2(i, i1);
  } else if (u1 == 0.0) {
    if (u0 > 0.0) {
      y = RT_PI / 2.0;
    } else if (u0 < 0.0) {
      y = -(RT_PI / 2.0);
    } else {
      y = 0.0;
    }
  } else {
    y = atan2(u0, u1);
  }
  return y;
}

/*
 * Arguments    : double x[4]
 *                const double A[16]
 *                const double z[4]
 *                double *phi
 *                double *theta
 *                double *psi
 * Return Type  : void
 */
void EulerKalman_2(double x[4], const double A[16], const double z[4],
                   double *phi, double *theta, double *psi)
{
  double K[16];
  double Pp[16];
  double b[16];
  double b_A[16];
  double y_tmp[16];
  double b_z[4];
  double xp[4];
  double d;
  double d1;
  double s;
  double smax;
  int b_i;
  int b_k;
  int b_tmp;
  int i;
  int i1;
  int j;
  int jA;
  int jp1j;
  int kAcol;
  signed char ipiv[4];
  signed char p[4];
  if (!isInitialized_EulerKalman_2) {
    EulerKalman_2_initialize();
  }
  for (i = 0; i < 4; i++) {
    smax = 0.0;
    for (i1 = 0; i1 < 4; i1++) {
      jA = i1 << 2;
      kAcol = i + jA;
      smax += A[kAcol] * x[i1];
      b_A[kAcol] =
          ((A[i] * P[jA] + A[i + 4] * P[jA + 1]) + A[i + 8] * P[jA + 2]) +
          A[i + 12] * P[jA + 3];
    }
    xp[i] = smax;
    smax = b_A[i];
    s = b_A[i + 4];
    d = b_A[i + 8];
    d1 = b_A[i + 12];
    for (i1 = 0; i1 < 4; i1++) {
      jA = i + (i1 << 2);
      Pp[jA] =
          (((smax * A[i1] + s * A[i1 + 4]) + d * A[i1 + 8]) + d1 * A[i1 + 12]) +
          Q[jA];
      y_tmp[i1 + (i << 2)] = H[jA];
    }
  }
  memset(&b[0], 0, 16U * sizeof(double));
  for (i = 0; i < 4; i++) {
    smax = H[i];
    s = H[i + 4];
    d = H[i + 8];
    d1 = H[i + 12];
    for (i1 = 0; i1 < 4; i1++) {
      kAcol = i1 << 2;
      b_A[i + kAcol] =
          ((smax * Pp[kAcol] + s * Pp[kAcol + 1]) + d * Pp[kAcol + 2]) +
          d1 * Pp[kAcol + 3];
    }
    smax = b_A[i];
    s = b_A[i + 4];
    d = b_A[i + 8];
    d1 = b_A[i + 12];
    for (i1 = 0; i1 < 4; i1++) {
      kAcol = i1 << 2;
      jA = i + kAcol;
      K[jA] = (((smax * y_tmp[kAcol] + s * y_tmp[kAcol + 1]) +
                d * y_tmp[kAcol + 2]) +
               d1 * y_tmp[kAcol + 3]) +
              R[jA];
    }
    ipiv[i] = (signed char)(i + 1);
  }
  for (j = 0; j < 3; j++) {
    int mmj_tmp;
    mmj_tmp = 2 - j;
    b_tmp = j * 5;
    jp1j = b_tmp + 2;
    jA = 4 - j;
    kAcol = 0;
    smax = fabs(K[b_tmp]);
    for (b_k = 2; b_k <= jA; b_k++) {
      s = fabs(K[(b_tmp + b_k) - 1]);
      if (s > smax) {
        kAcol = b_k - 1;
        smax = s;
      }
    }
    if (K[b_tmp + kAcol] != 0.0) {
      if (kAcol != 0) {
        jA = j + kAcol;
        ipiv[j] = (signed char)(jA + 1);
        smax = K[j];
        K[j] = K[jA];
        K[jA] = smax;
        smax = K[j + 4];
        K[j + 4] = K[jA + 4];
        K[jA + 4] = smax;
        smax = K[j + 8];
        K[j + 8] = K[jA + 8];
        K[jA + 8] = smax;
        smax = K[j + 12];
        K[j + 12] = K[jA + 12];
        K[jA + 12] = smax;
      }
      i = (b_tmp - j) + 4;
      for (b_i = jp1j; b_i <= i; b_i++) {
        K[b_i - 1] /= K[b_tmp];
      }
    }
    jA = b_tmp;
    for (kAcol = 0; kAcol <= mmj_tmp; kAcol++) {
      smax = K[(b_tmp + (kAcol << 2)) + 4];
      if (smax != 0.0) {
        i = jA + 6;
        i1 = (jA - j) + 8;
        for (jp1j = i; jp1j <= i1; jp1j++) {
          K[jp1j - 1] += K[((b_tmp + jp1j) - jA) - 5] * -smax;
        }
      }
      jA += 4;
    }
  }
  p[0] = 1;
  p[1] = 2;
  p[2] = 3;
  p[3] = 4;
  if (ipiv[0] > 1) {
    jA = p[ipiv[0] - 1];
    p[ipiv[0] - 1] = 1;
    p[0] = (signed char)jA;
  }
  if (ipiv[1] > 2) {
    jA = p[ipiv[1] - 1];
    p[ipiv[1] - 1] = p[1];
    p[1] = (signed char)jA;
  }
  if (ipiv[2] > 3) {
    jA = p[ipiv[2] - 1];
    p[ipiv[2] - 1] = p[2];
    p[2] = (signed char)jA;
  }
  for (b_k = 0; b_k < 4; b_k++) {
    b_tmp = (p[b_k] - 1) << 2;
    b[b_k + b_tmp] = 1.0;
    for (j = b_k + 1; j < 5; j++) {
      i = (j + b_tmp) - 1;
      if (b[i] != 0.0) {
        i1 = j + 1;
        for (b_i = i1; b_i < 5; b_i++) {
          jA = (b_i + b_tmp) - 1;
          b[jA] -= b[i] * K[(b_i + ((j - 1) << 2)) - 1];
        }
      }
    }
  }
  for (j = 0; j < 4; j++) {
    jA = j << 2;
    for (b_k = 3; b_k >= 0; b_k--) {
      kAcol = b_k << 2;
      i = b_k + jA;
      smax = b[i];
      if (smax != 0.0) {
        b[i] = smax / K[b_k + kAcol];
        for (b_i = 0; b_i < b_k; b_i++) {
          b_tmp = b_i + jA;
          b[b_tmp] -= b[i] * K[b_i + kAcol];
        }
      }
    }
    smax = Pp[j];
    s = Pp[j + 4];
    d = Pp[j + 8];
    d1 = Pp[j + 12];
    for (i = 0; i < 4; i++) {
      i1 = i << 2;
      b_A[j + i1] =
          ((smax * y_tmp[i1] + s * y_tmp[i1 + 1]) + d * y_tmp[i1 + 2]) +
          d1 * y_tmp[i1 + 3];
    }
  }
  for (i = 0; i < 4; i++) {
    double d2;
    smax = 0.0;
    s = b_A[i];
    d = b_A[i + 4];
    d1 = b_A[i + 8];
    d2 = b_A[i + 12];
    for (i1 = 0; i1 < 4; i1++) {
      kAcol = i1 << 2;
      jA = i + kAcol;
      K[jA] = ((s * b[kAcol] + d * b[kAcol + 1]) + d1 * b[kAcol + 2]) +
              d2 * b[kAcol + 3];
      smax += H[jA] * xp[i1];
    }
    b_z[i] = z[i] - smax;
  }
  for (i = 0; i < 4; i++) {
    smax = 0.0;
    for (i1 = 0; i1 < 4; i1++) {
      kAcol = i1 << 2;
      jA = i + kAcol;
      smax += K[jA] * b_z[i1];
      b_A[jA] = ((K[i] * H[kAcol] + K[i + 4] * H[kAcol + 1]) +
                 K[i + 8] * H[kAcol + 2]) +
                K[i + 12] * H[kAcol + 3];
    }
    x[i] = xp[i] + smax;
    smax = b_A[i];
    s = b_A[i + 4];
    d = b_A[i + 8];
    d1 = b_A[i + 12];
    for (i1 = 0; i1 < 4; i1++) {
      kAcol = i1 << 2;
      jA = i + kAcol;
      P[jA] = Pp[jA] -
              (((smax * Pp[kAcol] + s * Pp[kAcol + 1]) + d * Pp[kAcol + 2]) +
               d1 * Pp[kAcol + 3]);
    }
  }
  smax = x[2] * x[2];
  *phi = rt_atan2d_snf(2.0 * (x[2] * x[3] + x[0] * x[1]),
                       1.0 - 2.0 * (x[1] * x[1] + smax));
  *theta = -asin(2.0 * (x[1] * x[3] - x[0] * x[2]));
  *psi = rt_atan2d_snf(2.0 * (x[1] * x[2] + x[0] * x[3]),
                       1.0 - 2.0 * (smax + x[3] * x[3]));
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void EulerKalman_2_init(void)
{
  static const signed char b_iv[16] = {1, 0, 0, 0, 0, 1, 0, 0,
                                       0, 0, 1, 0, 0, 0, 0, 1};
  static const signed char iv1[16] = {100, 0, 0,   0, 0, 100, 0, 0,
                                      0,   0, 100, 0, 0, 0,   0, 100};
  int i;
  /*      x = [1 0 0 0]'; */
  for (i = 0; i < 16; i++) {
    int i1;
    i1 = b_iv[i];
    Q[i] = i1;
    R[i] = iv1[i];
    H[i] = 0.0;
    P[i] = i1;
  }
  H[0] = 1.0;
  H[5] = 1.0;
  H[10] = 1.0;
  H[15] = 1.0;
}

/*
 * File trailer for EulerKalman_2.c
 *
 * [EOF]
 */
