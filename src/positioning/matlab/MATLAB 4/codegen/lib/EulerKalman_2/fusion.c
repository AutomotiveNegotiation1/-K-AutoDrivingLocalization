/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: fusion.c
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 22-Sep-2023 10:38:00
 */

/* Include Files */
#include "fusion.h"
#include "EulerKalman_2.h"
#include "EulerKalman_2_data.h"
#include "EulerKalman_2_initialize.h"
#include "EulerKalman_2_rtwutil.h"
#include "EulerToQuaternion.h"
#include "PosKalman2.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Variable Definitions */
static double k;

static double Acc_Vel[60];

static double Acc_Pos[60];

static double UWB_M_Pos[60];

static double UWB_M_Vel[60];

static double kf_theta;

static double kf_phi;

static double kl;

static double UWB_LS_Pos[60];

/* Function Declarations */
static double rt_roundd_snf(double u);

/* Function Definitions */
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
               double kalman_on, const double k0[5], double cent_pos_est[3],
               double cent_vel_est[3], double *kf_psi,
               const creal_T tag_pos_est[4], double heading_est, double zt_b)
{
  static const double dv[16] = {1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0,
                                0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0};
  static const double dv1[9] = {0.01, 0.0, 0.0, 0.0, 0.01, 0.0, 0.0, 0.0, 0.01};
  static const double dv2[9] = {0.1, 0.0, 0.0, 0.0, 0.1, 0.0, 0.0, 0.0, 0.1};
  static const signed char b_a[9] = {0, 1, 0, 1, 0, 0, 0, 0, 1};
  creal_T IMUposU;
  double a[9];
  double u[3];
  double q;
  double scale;
  int UWB_LS_Pos_tmp;
  int i;
  (void)state_IMU;
  if (!isInitialized_EulerKalman_2) {
    EulerKalman_2_initialize();
  }
  /*  Initialization */
  if (k < 1000.0) {
    k++;
  }
  k++;
  q = (9.0 * tag_pos_est[2].re + tag_pos_est[0].re) +
      (9.0 * tag_pos_est[3].re + tag_pos_est[1].re);
  scale = (9.0 * tag_pos_est[2].im + tag_pos_est[0].im) +
          (9.0 * tag_pos_est[3].im + tag_pos_est[1].im);
  if (scale == 0.0) {
    IMUposU.re = q / 20.0;
    IMUposU.im = 0.0;
  } else if (q == 0.0) {
    IMUposU.re = 0.0;
    IMUposU.im = scale / 20.0;
  } else {
    IMUposU.re = q / 20.0;
    IMUposU.im = scale / 20.0;
  }
  if (k0[4] > 50.0) {
    double t;
    int UWB_M_Pos_tmp;
    /*  INS/UWB */
    for (i = 0; i < 19; i++) {
      UWB_LS_Pos_tmp = 3 * (i + 1);
      UWB_LS_Pos[3 * i] = UWB_LS_Pos[UWB_LS_Pos_tmp];
      UWB_LS_Pos[3 * i + 1] = UWB_LS_Pos[UWB_LS_Pos_tmp + 1];
      UWB_LS_Pos[3 * i + 2] = UWB_LS_Pos[UWB_LS_Pos_tmp + 2];
    }
    UWB_LS_Pos[57] = IMUposU.re;
    UWB_LS_Pos[58] = IMUposU.im;
    UWB_LS_Pos[59] = zt_b;
    /*  */
    /*                  if (RxIDTot(k,PP) >= 3) && (Reliability>2) &&
     * (kalman_on) */
    if ((kalman_on == 1.0) && (init_flag > 2.0) && (UWBErrSum < 0.5)) {
      double qua_acc[4];
      double qua_kf[4];
      /* && (Nanchor>2) && (Reliability >2) */
      if (rtIsNaN(-heading_est) || rtIsInf(-heading_est)) {
        t = rtNaN;
      } else if (-heading_est == 0.0) {
        t = 0.0;
      } else {
        bool rEQ0;
        t = fmod(-heading_est, 6.2831853071795862);
        rEQ0 = (t == 0.0);
        if (!rEQ0) {
          q = fabs(-heading_est / 6.2831853071795862);
          rEQ0 = !(fabs(q - floor(q + 0.5)) > 2.2204460492503131E-16 * q);
        }
        if (rEQ0) {
          t = 0.0;
        } else if (-heading_est < 0.0) {
          t += 6.2831853071795862;
        }
      }
      scale = t - *kf_psi;
      q = rt_roundd_snf(fabs(scale) / 6.2831853071795862);
      if (q < 2.147483648E+9) {
        i = (int)q;
      } else if (q >= 2.147483648E+9) {
        i = MAX_int32_T;
      } else {
        i = 0;
      }
      if (scale > 3.1415926535897931) {
        q = t - 6.2831853071795862 * (double)i;
      } else {
        q = t + 6.2831853071795862 * (double)i;
      }
      EulerToQuaternion(*acc_b_phi, acc_b_theta, q, qua_acc);
      EulerToQuaternion(kf_phi, kf_theta, *kf_psi, qua_kf);
      EulerKalman_2(qua_kf, dv, qua_acc, &kf_phi, &kf_theta, kf_psi);
    }
    if (kl > 20.0) {
      for (i = 0; i < 19; i++) {
        UWB_LS_Pos_tmp = 3 * (i + 1);
        UWB_M_Pos[3 * i] = UWB_M_Pos[UWB_LS_Pos_tmp];
        UWB_M_Vel[3 * i] = UWB_M_Vel[UWB_LS_Pos_tmp];
        UWB_M_Pos_tmp = 3 * i + 1;
        UWB_M_Pos[UWB_M_Pos_tmp] = UWB_M_Pos[UWB_LS_Pos_tmp + 1];
        UWB_M_Vel[UWB_M_Pos_tmp] = UWB_M_Vel[UWB_LS_Pos_tmp + 1];
        UWB_M_Pos_tmp = 3 * i + 2;
        UWB_M_Pos[UWB_M_Pos_tmp] = UWB_M_Pos[UWB_LS_Pos_tmp + 2];
        UWB_M_Vel[UWB_M_Pos_tmp] = UWB_M_Vel[UWB_LS_Pos_tmp + 2];
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
        UWB_LS_Pos_tmp = 3 * (i + 1);
        UWB_M_Pos[3 * i] = UWB_M_Pos[UWB_LS_Pos_tmp];
        UWB_M_Vel[3 * i] = UWB_M_Vel[UWB_LS_Pos_tmp];
        UWB_M_Pos_tmp = 3 * i + 1;
        UWB_M_Pos[UWB_M_Pos_tmp] = UWB_M_Pos[UWB_LS_Pos_tmp + 1];
        UWB_M_Vel[UWB_M_Pos_tmp] = UWB_M_Vel[UWB_LS_Pos_tmp + 1];
        UWB_M_Pos_tmp = 3 * i + 2;
        UWB_M_Pos[UWB_M_Pos_tmp] = UWB_M_Pos[UWB_LS_Pos_tmp + 2];
        UWB_M_Vel[UWB_M_Pos_tmp] = UWB_M_Vel[UWB_LS_Pos_tmp + 2];
      }
      UWB_M_Pos[57] = IMUposU.re;
      UWB_M_Pos[58] = IMUposU.im;
      UWB_M_Pos[59] = zt_b;
      UWB_M_Vel[57] = 0.0;
      UWB_M_Vel[58] = 0.0;
      UWB_M_Vel[59] = 0.0;
    }
    if ((kalman_on == 1.0) && (init_flag > 2.0) && (UWBErrSum < 1.0)) {
      double A[144];
      double x[12];
      double Cb2n[9];
      double theta;
      int A_tmp;
      /* x = [cent_pos_est;cent_vel_est(1:2);0;TEMP;TEMP_bias]; */
      x[9] = b_acc_o[0];
      x[10] = b_acc_o[1];
      x[11] = b_acc_o[2];
      /* Cb2n = [0 1 0;1 0 0;0 0 1]*rotationVectorToMatrix([0;0;kf_psi])'; */
      u[2] = *kf_psi;
      scale = 3.3121686421112381E-170;
      x[0] = cent_pos_est[0];
      x[3] = cent_vel_est[0];
      x[6] = 0.0;
      q = fabs(kf_phi);
      if (q > 3.3121686421112381E-170) {
        theta = 1.0;
        scale = q;
      } else {
        t = q / 3.3121686421112381E-170;
        theta = t * t;
      }
      x[1] = cent_pos_est[1];
      x[4] = cent_vel_est[1];
      x[7] = 0.0;
      q = fabs(kf_theta);
      if (q > scale) {
        t = scale / q;
        theta = theta * t * t + 1.0;
        scale = q;
      } else {
        t = q / scale;
        theta += t * t;
      }
      x[2] = cent_pos_est[2];
      x[5] = cent_vel_est[2];
      x[8] = 0.0;
      q = fabs(u[2]);
      if (q > scale) {
        t = scale / q;
        theta = theta * t * t + 1.0;
        scale = q;
      } else {
        t = q / scale;
        theta += t * t;
      }
      theta = scale * sqrt(theta);
      if (theta < 1.0E-6) {
        memset(&a[0], 0, 9U * sizeof(double));
        a[0] = 1.0;
        a[4] = 1.0;
        a[8] = 1.0;
      } else {
        double b_u[9];
        u[0] = kf_phi / theta;
        u[1] = kf_theta / theta;
        u[2] /= theta;
        q = cos(theta);
        scale = sin(theta);
        memset(&a[0], 0, 9U * sizeof(double));
        Cb2n[0] = scale * 0.0;
        Cb2n[3] = scale * -u[2];
        Cb2n[6] = scale * u[1];
        Cb2n[1] = scale * u[2];
        Cb2n[4] = scale * 0.0;
        Cb2n[7] = scale * -u[0];
        Cb2n[2] = scale * -u[1];
        Cb2n[5] = scale * u[0];
        Cb2n[8] = scale * 0.0;
        for (UWB_LS_Pos_tmp = 0; UWB_LS_Pos_tmp < 3; UWB_LS_Pos_tmp++) {
          a[UWB_LS_Pos_tmp + 3 * UWB_LS_Pos_tmp] = 1.0;
          b_u[3 * UWB_LS_Pos_tmp] = u[0] * u[UWB_LS_Pos_tmp];
          b_u[3 * UWB_LS_Pos_tmp + 1] = u[1] * u[UWB_LS_Pos_tmp];
          b_u[3 * UWB_LS_Pos_tmp + 2] = u[2] * u[UWB_LS_Pos_tmp];
        }
        for (i = 0; i < 9; i++) {
          a[i] = (a[i] * q + Cb2n[i]) + (1.0 - q) * b_u[i];
        }
      }
      for (i = 0; i < 3; i++) {
        signed char i1;
        signed char i2;
        signed char i3;
        i1 = b_a[i];
        i2 = b_a[i + 3];
        i3 = b_a[i + 6];
        for (UWB_LS_Pos_tmp = 0; UWB_LS_Pos_tmp < 3; UWB_LS_Pos_tmp++) {
          Cb2n[i + 3 * UWB_LS_Pos_tmp] =
              ((double)i1 * a[3 * UWB_LS_Pos_tmp] +
               (double)i2 * a[3 * UWB_LS_Pos_tmp + 1]) +
              (double)i3 * a[3 * UWB_LS_Pos_tmp + 2];
        }
      }
      for (i = 0; i < 144; i++) {
        A[i] = iv[i];
      }
      for (i = 0; i < 3; i++) {
        UWB_M_Pos_tmp = 12 * (i + 3);
        A[UWB_M_Pos_tmp] = dv1[3 * i];
        A_tmp = 12 * (i + 6);
        A[A_tmp] = Cb2n[3 * i] * 0.0001 / 2.0;
        UWB_LS_Pos_tmp = 3 * i + 1;
        A[UWB_M_Pos_tmp + 1] = dv1[UWB_LS_Pos_tmp];
        A[A_tmp + 1] = Cb2n[UWB_LS_Pos_tmp] * 0.0001 / 2.0;
        UWB_LS_Pos_tmp = 3 * i + 2;
        A[UWB_M_Pos_tmp + 2] = dv1[UWB_LS_Pos_tmp];
        A[A_tmp + 2] = Cb2n[UWB_LS_Pos_tmp] * 0.0001 / 2.0;
      }
      for (i = 0; i < 9; i++) {
        a[i] = -Cb2n[i];
      }
      for (i = 0; i < 3; i++) {
        scale = a[3 * i];
        UWB_M_Pos_tmp = 12 * (i + 9);
        A[UWB_M_Pos_tmp] = scale * 0.0001 / 2.0;
        A_tmp = 12 * (i + 6);
        A[A_tmp + 3] = Cb2n[3 * i] * 0.01;
        A[UWB_M_Pos_tmp + 3] = scale * 0.01;
        UWB_LS_Pos_tmp = 3 * i + 1;
        scale = a[UWB_LS_Pos_tmp];
        A[UWB_M_Pos_tmp + 1] = scale * 0.0001 / 2.0;
        A[A_tmp + 4] = Cb2n[UWB_LS_Pos_tmp] * 0.01;
        A[UWB_M_Pos_tmp + 4] = scale * 0.01;
        UWB_LS_Pos_tmp = 3 * i + 2;
        scale = a[UWB_LS_Pos_tmp];
        A[UWB_M_Pos_tmp + 2] = scale * 0.0001 / 2.0;
        A[A_tmp + 5] = Cb2n[UWB_LS_Pos_tmp] * 0.01;
        A[UWB_M_Pos_tmp + 5] = scale * 0.01;
      }
      if (rt_hypotd_snf(UWB_M_Pos[57] - cent_pos_est[0],
                        UWB_M_Pos[58] - cent_pos_est[1]) < 2.0) {
        PosKalman2(x, A, &UWB_M_Pos[57], dv2);
      } else {
        PosKalman2(x, A, &UWB_M_Pos[57], dv1);
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
  memset(&UWB_M_Pos[0], 0, 60U * sizeof(double));
  memset(&UWB_M_Vel[0], 0, 60U * sizeof(double));
  memset(&UWB_LS_Pos[0], 0, 60U * sizeof(double));
}

/*
 * File trailer for fusion.c
 *
 * [EOF]
 */
