/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: fusion.c
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 22-Sep-2023 09:43:03
 */

/* Include Files */
#include "fusion.h"
#include "PosKalman2.h"
#include "fusion_data.h"
#include "fusion_initialize.h"
#include "mod.h"
#include "rt_nonfinite.h"
#include "rt_defines.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Variable Definitions */
static double k;

static double Acc_Vel[60];

static double Acc_Pos[60];

static double IMUacc[60];

static double UWB_M_Pos[60];

static double UWB_M_Vel[60];

static double kf_theta;

static double kf_phi;

static double kl;

static double UWB_LS_Pos[60];

/* Function Declarations */
static double rt_atan2d_snf(double u0, double u1);

static double rt_hypotd_snf(double u0, double u1);

static double rt_roundd_snf(double u);

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
 * Arguments    : double u0
 *                double u1
 * Return Type  : double
 */
static double rt_hypotd_snf(double u0, double u1)
{
  double a;
  double b;
  double y;
  a = fabs(u0);
  b = fabs(u1);
  if (a < b) {
    a /= b;
    y = b * sqrt(a * a + 1.0);
  } else if (a > b) {
    b /= a;
    y = a * sqrt(b * b + 1.0);
  } else if (rtIsNaN(b)) {
    y = rtNaN;
  } else {
    y = a * 1.4142135623730951;
  }
  return y;
}

/*
 * Arguments    : double u
 * Return Type  : double
 */
static double rt_roundd_snf(double u)
{
  double y;
  if (fabs(u) < 4.503599627370496E+15) {
    if (u >= 0.5) {
      y = floor(u + 0.5);
    } else if (u > -0.5) {
      y = u * 0.0;
    } else {
      y = ceil(u - 0.5);
    }
  } else {
    y = u;
  }
  return y;
}

/*
 * Arguments    : const creal_T *tag_center_vel_est
 *                double state_IMU
 *                double Nanchor
 *                const double b_acc_o[3]
 *                double acc_b_theta
 *                const double *acc_b_phi
 *                double UWBErrSum
 *                double init_flag
 *                double kalman_on
 *                const double k0[5]
 *                double cent_pos_est[3]
 *                double cent_vel_est[3]
 *                double *kf_psi
 *                const creal_T tag_pos_est[4]
 *                double heading_est
 *                double zt_b
 * Return Type  : creal_T
 */
creal_T fusion(const creal_T *tag_center_vel_est, double state_IMU,
               double Nanchor, const double b_acc_o[3], double acc_b_theta,
               const double *acc_b_phi, double UWBErrSum, double init_flag,
               double kalman_on, double k0, double cent_pos_est[3],
               double cent_vel_est[3], double *kf_psi,
               const creal_T tag_pos_est[4], double heading_est, double zt_b)
{
  static const double dv[9] = {0.01, 0.0, 0.0, 0.0, 0.01, 0.0, 0.0, 0.0, 0.01};
  static const double dv1[9] = {0.1, 0.0, 0.0, 0.0, 0.1, 0.0, 0.0, 0.0, 0.1};
  static const signed char b_a[9] = {0, 1, 0, 1, 0, 0, 0, 0, 1};
  creal_T IMUposU;
  double A[16];
  double a[9];
  double u[3];
  double cosTheta;
  double sinTheta;
  int b_i;
  int b_k;
  int i;
  int i1;
  int j;
  int jp1j;
  int kAcol;
  (void)state_IMU;
  if (!isInitialized_fusion) {
    fusion_initialize();
  }
  /*  Initialization */
  if (k < 1000.0) {
    k++;
  }
  k++;
  sinTheta = (9.0 * tag_pos_est[2].re + tag_pos_est[0].re) +
             (9.0 * tag_pos_est[3].re + tag_pos_est[1].re);
  cosTheta = (9.0 * tag_pos_est[2].im + tag_pos_est[0].im) +
             (9.0 * tag_pos_est[3].im + tag_pos_est[1].im);
  if (cosTheta == 0.0) {
    IMUposU.re = sinTheta / 20.0;
    IMUposU.im = 0.0;
  } else if (sinTheta == 0.0) {
    IMUposU.re = 0.0;
    IMUposU.im = cosTheta / 20.0;
  } else {
    IMUposU.re = sinTheta / 20.0;
    IMUposU.im = cosTheta / 20.0;
  }
  if (k0 > 50.0) {
    double cosPhi;
    double cosPsi;
    int jA;
    /*  INS/UWB */
    for (i = 0; i < 19; i++) {
      jA = 3 * (i + 1);
      UWB_LS_Pos[3 * i] = UWB_LS_Pos[jA];
      UWB_LS_Pos[3 * i + 1] = UWB_LS_Pos[jA + 1];
      UWB_LS_Pos[3 * i + 2] = UWB_LS_Pos[jA + 2];
    }
    UWB_LS_Pos[57] = IMUposU.re;
    UWB_LS_Pos[58] = IMUposU.im;
    UWB_LS_Pos[59] = zt_b;
    /*  */
    /*                  if (RxIDTot(k,PP) >= 3) && (Reliability>2) &&
     * (kalman_on) */
    if ((kalman_on == 1.0) && (init_flag > 2.0) && (UWBErrSum < 0.5)) {
      double K[16];
      double Pp[16];
      double b_Pp[16];
      double y_tmp[16];
      double c_cosPhi[4];
      double qua_kf[4];
      double xp[4];
      double b_cosPhi;
      double b_cosPhi_tmp;
      double b_cosPsi;
      double b_cosTheta;
      double b_sinPhi;
      double b_sinPsi;
      double b_sinTheta;
      double cosPhi_tmp;
      double sinPhi;
      double sinPsi;
      signed char ipiv[4];
      signed char p[4];
      /* && (Nanchor>2) && (Reliability >2) */
      sinTheta = b_mod(-heading_est);
      cosPsi = sinTheta - *kf_psi;
      cosPhi_tmp = rt_roundd_snf(fabs(cosPsi) / 6.2831853071795862);
      if (cosPhi_tmp < 2.147483648E+9) {
        i = (int)cosPhi_tmp;
      } else if (cosPhi_tmp >= 2.147483648E+9) {
        i = MAX_int32_T;
      } else {
        i = 0;
      }
      if (cosPsi > 3.1415926535897931) {
        cosPhi = sinTheta - 6.2831853071795862 * (double)i;
      } else {
        cosPhi = sinTheta + 6.2831853071795862 * (double)i;
      }
      sinPhi = sin(*acc_b_phi / 2.0);
      b_cosPhi = cos(*acc_b_phi / 2.0);
      b_sinTheta = sin(acc_b_theta / 2.0);
      b_cosTheta = cos(acc_b_theta / 2.0);
      sinPsi = sin(cosPhi / 2.0);
      b_cosPsi = cos(cosPhi / 2.0);
      b_sinPhi = sin(kf_phi / 2.0);
      cosPhi = cos(kf_phi / 2.0);
      sinTheta = sin(kf_theta / 2.0);
      cosTheta = cos(kf_theta / 2.0);
      b_sinPsi = sin(*kf_psi / 2.0);
      cosPsi = cos(*kf_psi / 2.0);
      cosPhi_tmp = cosPhi * cosTheta;
      b_cosPhi_tmp = b_sinPhi * sinTheta;
      c_cosPhi[0] = cosPhi_tmp * cosPsi + b_cosPhi_tmp * b_sinPsi;
      sinTheta *= cosPhi;
      cosPhi = b_sinPhi * cosTheta;
      c_cosPhi[1] = cosPhi * cosPsi - sinTheta * b_sinPsi;
      c_cosPhi[2] = sinTheta * cosPsi + cosPhi * b_sinPsi;
      c_cosPhi[3] = cosPhi_tmp * b_sinPsi - b_cosPhi_tmp * cosPsi;
      for (i = 0; i < 4; i++) {
        cosPsi = 0.0;
        for (i1 = 0; i1 < 4; i1++) {
          jA = i1 << 2;
          kAcol = i + jA;
          cosPsi += (double)iv[kAcol] * c_cosPhi[i1];
          A[kAcol] = (((double)iv[i] * P[jA] + (double)iv[i + 4] * P[jA + 1]) +
                      (double)iv[i + 8] * P[jA + 2]) +
                     (double)iv[i + 12] * P[jA + 3];
        }
        xp[i] = cosPsi;
        cosPsi = A[i];
        cosPhi_tmp = A[i + 4];
        cosPhi = A[i + 8];
        cosTheta = A[i + 12];
        for (i1 = 0; i1 < 4; i1++) {
          kAcol = i1 << 2;
          jA = i + kAcol;
          Pp[jA] = (((cosPsi * (double)iv[kAcol] +
                      cosPhi_tmp * (double)iv[kAcol + 1]) +
                     cosPhi * (double)iv[kAcol + 2]) +
                    cosTheta * (double)iv[kAcol + 3]) +
                   Q[jA];
        }
      }
      for (i = 0; i < 4; i++) {
        for (i1 = 0; i1 < 4; i1++) {
          jA = i1 << 2;
          kAcol = i + jA;
          y_tmp[i1 + (i << 2)] = H[kAcol];
          A[kAcol] = ((H[i] * Pp[jA] + H[i + 4] * Pp[jA + 1]) +
                      H[i + 8] * Pp[jA + 2]) +
                     H[i + 12] * Pp[jA + 3];
        }
      }
      for (i = 0; i < 4; i++) {
        cosPsi = A[i];
        cosPhi_tmp = A[i + 4];
        cosPhi = A[i + 8];
        cosTheta = A[i + 12];
        for (i1 = 0; i1 < 4; i1++) {
          kAcol = i1 << 2;
          jA = i + kAcol;
          K[jA] = (((cosPsi * y_tmp[kAcol] + cosPhi_tmp * y_tmp[kAcol + 1]) +
                    cosPhi * y_tmp[kAcol + 2]) +
                   cosTheta * y_tmp[kAcol + 3]) +
                  R[jA];
        }
      }
      memset(&A[0], 0, 16U * sizeof(double));
      ipiv[0] = 1;
      ipiv[1] = 2;
      ipiv[2] = 3;
      for (j = 0; j < 3; j++) {
        int b_tmp;
        int mmj_tmp;
        mmj_tmp = 2 - j;
        b_tmp = j * 5;
        jp1j = b_tmp + 2;
        jA = 4 - j;
        kAcol = 0;
        sinTheta = fabs(K[b_tmp]);
        for (b_k = 2; b_k <= jA; b_k++) {
          cosTheta = fabs(K[(b_tmp + b_k) - 1]);
          if (cosTheta > sinTheta) {
            kAcol = b_k - 1;
            sinTheta = cosTheta;
          }
        }
        if (K[b_tmp + kAcol] != 0.0) {
          if (kAcol != 0) {
            jA = j + kAcol;
            ipiv[j] = (signed char)(jA + 1);
            sinTheta = K[j];
            K[j] = K[jA];
            K[jA] = sinTheta;
            sinTheta = K[j + 4];
            K[j + 4] = K[jA + 4];
            K[jA + 4] = sinTheta;
            sinTheta = K[j + 8];
            K[j + 8] = K[jA + 8];
            K[jA + 8] = sinTheta;
            sinTheta = K[j + 12];
            K[j + 12] = K[jA + 12];
            K[jA + 12] = sinTheta;
          }
          i = (b_tmp - j) + 4;
          for (b_i = jp1j; b_i <= i; b_i++) {
            K[b_i - 1] /= K[b_tmp];
          }
        }
        jA = b_tmp;
        for (kAcol = 0; kAcol <= mmj_tmp; kAcol++) {
          cosTheta = K[(b_tmp + (kAcol << 2)) + 4];
          if (cosTheta != 0.0) {
            i = jA + 6;
            i1 = (jA - j) + 8;
            for (jp1j = i; jp1j <= i1; jp1j++) {
              K[jp1j - 1] += K[((b_tmp + jp1j) - jA) - 5] * -cosTheta;
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
        jp1j = (p[b_k] - 1) << 2;
        A[b_k + jp1j] = 1.0;
        for (j = b_k + 1; j < 5; j++) {
          i = (j + jp1j) - 1;
          if (A[i] != 0.0) {
            i1 = j + 1;
            for (b_i = i1; b_i < 5; b_i++) {
              kAcol = (b_i + jp1j) - 1;
              A[kAcol] -= A[i] * K[(b_i + ((j - 1) << 2)) - 1];
            }
          }
        }
      }
      for (j = 0; j < 4; j++) {
        jA = j << 2;
        for (b_k = 3; b_k >= 0; b_k--) {
          kAcol = b_k << 2;
          i = b_k + jA;
          cosPsi = A[i];
          if (cosPsi != 0.0) {
            A[i] = cosPsi / K[b_k + kAcol];
            for (b_i = 0; b_i < b_k; b_i++) {
              jp1j = b_i + jA;
              A[jp1j] -= A[i] * K[b_i + kAcol];
            }
          }
        }
        cosPsi = Pp[j];
        cosPhi_tmp = Pp[j + 4];
        cosPhi = Pp[j + 8];
        cosTheta = Pp[j + 12];
        for (i = 0; i < 4; i++) {
          i1 = i << 2;
          b_Pp[j + i1] = ((cosPsi * y_tmp[i1] + cosPhi_tmp * y_tmp[i1 + 1]) +
                          cosPhi * y_tmp[i1 + 2]) +
                         cosTheta * y_tmp[i1 + 3];
        }
      }
      cosPhi_tmp = b_cosPhi * b_cosTheta;
      b_cosPhi_tmp = sinPhi * b_sinTheta;
      c_cosPhi[0] = cosPhi_tmp * b_cosPsi + b_cosPhi_tmp * sinPsi;
      sinTheta = b_cosPhi * b_sinTheta;
      cosPhi = sinPhi * b_cosTheta;
      c_cosPhi[1] = cosPhi * b_cosPsi - sinTheta * sinPsi;
      c_cosPhi[2] = sinTheta * b_cosPsi + cosPhi * sinPsi;
      c_cosPhi[3] = cosPhi_tmp * sinPsi - b_cosPhi_tmp * b_cosPsi;
      for (i = 0; i < 4; i++) {
        cosPsi = b_Pp[i];
        cosPhi_tmp = b_Pp[i + 4];
        cosPhi = b_Pp[i + 8];
        cosTheta = b_Pp[i + 12];
        sinTheta = 0.0;
        for (i1 = 0; i1 < 4; i1++) {
          kAcol = i1 << 2;
          jA = i + kAcol;
          K[jA] = ((cosPsi * A[kAcol] + cosPhi_tmp * A[kAcol + 1]) +
                   cosPhi * A[kAcol + 2]) +
                  cosTheta * A[kAcol + 3];
          sinTheta += H[jA] * xp[i1];
        }
        c_cosPhi[i] -= sinTheta;
      }
      for (i = 0; i < 4; i++) {
        cosPsi = 0.0;
        for (i1 = 0; i1 < 4; i1++) {
          kAcol = i1 << 2;
          jA = i + kAcol;
          cosPsi += K[jA] * c_cosPhi[i1];
          A[jA] = ((K[i] * H[kAcol] + K[i + 4] * H[kAcol + 1]) +
                   K[i + 8] * H[kAcol + 2]) +
                  K[i + 12] * H[kAcol + 3];
        }
        qua_kf[i] = xp[i] + cosPsi;
        cosPsi = A[i];
        cosPhi_tmp = A[i + 4];
        cosPhi = A[i + 8];
        cosTheta = A[i + 12];
        for (i1 = 0; i1 < 4; i1++) {
          kAcol = i1 << 2;
          jA = i + kAcol;
          P[jA] = Pp[jA] - (((cosPsi * Pp[kAcol] + cosPhi_tmp * Pp[kAcol + 1]) +
                             cosPhi * Pp[kAcol + 2]) +
                            cosTheta * Pp[kAcol + 3]);
        }
      }
      sinTheta = qua_kf[2] * qua_kf[2];
      kf_phi =
          rt_atan2d_snf(2.0 * (qua_kf[2] * qua_kf[3] + qua_kf[0] * qua_kf[1]),
                        1.0 - 2.0 * (qua_kf[1] * qua_kf[1] + sinTheta));
      kf_theta = -asin(2.0 * (qua_kf[1] * qua_kf[3] - qua_kf[0] * qua_kf[2]));
      *kf_psi =
          rt_atan2d_snf(2.0 * (qua_kf[1] * qua_kf[2] + qua_kf[0] * qua_kf[3]),
                        1.0 - 2.0 * (sinTheta + qua_kf[3] * qua_kf[3]));
    }
    if (kl > 20.0) {
      for (i = 0; i < 19; i++) {
        jA = 3 * (i + 1);
        UWB_M_Pos[3 * i] = UWB_M_Pos[jA];
        UWB_M_Vel[3 * i] = UWB_M_Vel[jA];
        kAcol = 3 * i + 1;
        UWB_M_Pos[kAcol] = UWB_M_Pos[jA + 1];
        UWB_M_Vel[kAcol] = UWB_M_Vel[jA + 1];
        kAcol = 3 * i + 2;
        UWB_M_Pos[kAcol] = UWB_M_Pos[jA + 2];
        UWB_M_Vel[kAcol] = UWB_M_Vel[jA + 2];
      }
      /*                      UWB_M_Pos(:,end) = (Tm+Td); */
      /*                      UWB_M_Pos(:,end) = (Ta); */
      UWB_M_Pos[57] = IMUposU.re;
      UWB_M_Pos[58] = IMUposU.im;
      UWB_M_Pos[59] = 0.0;
      UWB_M_Vel[57] = tag_center_vel_est->re;
      UWB_M_Vel[58] = tag_center_vel_est->im;
      UWB_M_Vel[59] = 0.0;
      /*                      UWB_M_Vel(:,end) =
       * (UWB_M_Pos(:,end)-UWB_M_Pos(:,end-9))/10; */
      /*                      tag_center_vel_est */
    } else {
      for (i = 0; i < 19; i++) {
        jA = 3 * (i + 1);
        UWB_M_Pos[3 * i] = UWB_M_Pos[jA];
        UWB_M_Vel[3 * i] = UWB_M_Vel[jA];
        kAcol = 3 * i + 1;
        UWB_M_Pos[kAcol] = UWB_M_Pos[jA + 1];
        UWB_M_Vel[kAcol] = UWB_M_Vel[jA + 1];
        kAcol = 3 * i + 2;
        UWB_M_Pos[kAcol] = UWB_M_Pos[jA + 2];
        UWB_M_Vel[kAcol] = UWB_M_Vel[jA + 2];
      }
      UWB_M_Pos[57] = IMUposU.re;
      UWB_M_Pos[58] = IMUposU.im;
      UWB_M_Pos[59] = zt_b;
      UWB_M_Vel[57] = 0.0;
      UWB_M_Vel[58] = 0.0;
      UWB_M_Vel[59] = 0.0;
    }
    if ((kalman_on == 1.0) && (init_flag > 2.0) && (UWBErrSum < 1.0)) {
      double b_A[144];
      double x[12];
      double Cb2n[9];
      /* x = [cent_pos_est;cent_vel_est(1:2);0;TEMP;TEMP_bias]; */
      x[6] = IMUacc[19];
      x[7] = IMUacc[39];
      x[8] = 0.0;
      x[9] = b_acc_o[0];
      x[10] = b_acc_o[1];
      x[11] = b_acc_o[2];
      /* Cb2n = [0 1 0;1 0 0;0 0 1]*rotationVectorToMatrix([0;0;kf_psi])'; */
      u[2] = *kf_psi;
      cosTheta = 3.3121686421112381E-170;
      x[0] = cent_pos_est[0];
      x[3] = cent_vel_est[0];
      sinTheta = fabs(kf_phi);
      if (sinTheta > 3.3121686421112381E-170) {
        cosPsi = 1.0;
        cosTheta = sinTheta;
      } else {
        cosPhi = sinTheta / 3.3121686421112381E-170;
        cosPsi = cosPhi * cosPhi;
      }
      x[1] = cent_pos_est[1];
      x[4] = cent_vel_est[1];
      sinTheta = fabs(kf_theta);
      if (sinTheta > cosTheta) {
        cosPhi = cosTheta / sinTheta;
        cosPsi = cosPsi * cosPhi * cosPhi + 1.0;
        cosTheta = sinTheta;
      } else {
        cosPhi = sinTheta / cosTheta;
        cosPsi += cosPhi * cosPhi;
      }
      x[2] = cent_pos_est[2];
      x[5] = cent_vel_est[2];
      sinTheta = fabs(u[2]);
      if (sinTheta > cosTheta) {
        cosPhi = cosTheta / sinTheta;
        cosPsi = cosPsi * cosPhi * cosPhi + 1.0;
        cosTheta = sinTheta;
      } else {
        cosPhi = sinTheta / cosTheta;
        cosPsi += cosPhi * cosPhi;
      }
      cosPsi = cosTheta * sqrt(cosPsi);
      if (cosPsi < 1.0E-6) {
        memset(&a[0], 0, 9U * sizeof(double));
        a[0] = 1.0;
        a[4] = 1.0;
        a[8] = 1.0;
      } else {
        double b_u[9];
        u[0] = kf_phi / cosPsi;
        u[1] = kf_theta / cosPsi;
        u[2] /= cosPsi;
        cosTheta = cos(cosPsi);
        sinTheta = sin(cosPsi);
        memset(&a[0], 0, 9U * sizeof(double));
        Cb2n[0] = sinTheta * 0.0;
        Cb2n[3] = sinTheta * -u[2];
        Cb2n[6] = sinTheta * u[1];
        Cb2n[1] = sinTheta * u[2];
        Cb2n[4] = sinTheta * 0.0;
        Cb2n[7] = sinTheta * -u[0];
        Cb2n[2] = sinTheta * -u[1];
        Cb2n[5] = sinTheta * u[0];
        Cb2n[8] = sinTheta * 0.0;
        for (b_k = 0; b_k < 3; b_k++) {
          a[b_k + 3 * b_k] = 1.0;
          b_u[3 * b_k] = u[0] * u[b_k];
          b_u[3 * b_k + 1] = u[1] * u[b_k];
          b_u[3 * b_k + 2] = u[2] * u[b_k];
        }
        for (i = 0; i < 9; i++) {
          a[i] = (a[i] * cosTheta + Cb2n[i]) + (1.0 - cosTheta) * b_u[i];
        }
      }
      for (i = 0; i < 3; i++) {
        signed char i2;
        signed char i3;
        signed char i4;
        i2 = b_a[i];
        i3 = b_a[i + 3];
        i4 = b_a[i + 6];
        for (i1 = 0; i1 < 3; i1++) {
          Cb2n[i + 3 * i1] =
              ((double)i2 * a[3 * i1] + (double)i3 * a[3 * i1 + 1]) +
              (double)i4 * a[3 * i1 + 2];
        }
      }
      for (i = 0; i < 144; i++) {
        b_A[i] = iv1[i];
      }
      for (i = 0; i < 3; i++) {
        jp1j = 12 * (i + 3);
        b_A[jp1j] = dv[3 * i];
        kAcol = 12 * (i + 6);
        b_A[kAcol] = Cb2n[3 * i] * 0.0001 / 2.0;
        jA = 3 * i + 1;
        b_A[jp1j + 1] = dv[jA];
        b_A[kAcol + 1] = Cb2n[jA] * 0.0001 / 2.0;
        jA = 3 * i + 2;
        b_A[jp1j + 2] = dv[jA];
        b_A[kAcol + 2] = Cb2n[jA] * 0.0001 / 2.0;
      }
      for (i = 0; i < 9; i++) {
        a[i] = -Cb2n[i];
      }
      for (i = 0; i < 3; i++) {
        cosPsi = a[3 * i];
        jp1j = 12 * (i + 9);
        b_A[jp1j] = cosPsi * 0.0001 / 2.0;
        kAcol = 12 * (i + 6);
        b_A[kAcol + 3] = Cb2n[3 * i] * 0.01;
        b_A[jp1j + 3] = cosPsi * 0.01;
        i1 = 3 * i + 1;
        cosPsi = a[i1];
        b_A[jp1j + 1] = cosPsi * 0.0001 / 2.0;
        b_A[kAcol + 4] = Cb2n[i1] * 0.01;
        b_A[jp1j + 4] = cosPsi * 0.01;
        i1 = 3 * i + 2;
        cosPsi = a[i1];
        b_A[jp1j + 2] = cosPsi * 0.0001 / 2.0;
        b_A[kAcol + 5] = Cb2n[i1] * 0.01;
        b_A[jp1j + 5] = cosPsi * 0.01;
      }
      if (rt_hypotd_snf(UWB_M_Pos[57] - cent_pos_est[0],
                        UWB_M_Pos[58] - cent_pos_est[1]) < 2.0) {
        PosKalman2(x, b_A, &UWB_M_Pos[57], dv1);
      } else {
        PosKalman2(x, b_A, &UWB_M_Pos[57], dv);
        /*                          z = [UWB_M_Pos(:,end);UWB_M_Vel(:,end)]; */
        /*                          x_n = PosKalman(x,A,z,R); */
        x[0] = UWB_M_Pos[57];
        x[3] = UWB_M_Vel[57];
        x[1] = UWB_M_Pos[58];
        x[4] = UWB_M_Vel[58];
        x[2] = UWB_M_Pos[59];
        x[5] = UWB_M_Vel[59];
      }
      Acc_Pos[57] = x[0];
      Acc_Vel[57] = x[3];
      Acc_Pos[58] = x[1];
      Acc_Vel[58] = x[4];
      Acc_Pos[59] = x[2];
      Acc_Vel[59] = x[5];
      if (Nanchor > 1.0) {
        cent_pos_est[0] = Acc_Pos[57];
        cent_vel_est[0] = Acc_Vel[57];
        cent_pos_est[1] = Acc_Pos[58];
        cent_vel_est[1] = Acc_Vel[58];
        cent_pos_est[2] = Acc_Pos[59];
        cent_vel_est[2] = Acc_Vel[59];
      }
    }
    kl++;
  }
  return IMUposU;
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void fusion_init(void)
{
  k = 0.0;
  kf_theta = 0.0;
  kf_phi = 0.0;
  kl = 1.0;
  memset(&Acc_Vel[0], 0, 60U * sizeof(double));
  memset(&Acc_Pos[0], 0, 60U * sizeof(double));
  memset(&IMUacc[0], 0, 60U * sizeof(double));
  memset(&UWB_M_Pos[0], 0, 60U * sizeof(double));
  memset(&UWB_M_Vel[0], 0, 60U * sizeof(double));
  memset(&UWB_LS_Pos[0], 0, 60U * sizeof(double));
}

/*
 * File trailer for fusion.c
 *
 * [EOF]
 */
