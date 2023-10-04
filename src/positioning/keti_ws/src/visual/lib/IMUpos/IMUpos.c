/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: IMUpos.c
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 04-Oct-2023 14:13:20
 */

/* Include Files */
#include "IMUpos.h"
#include "IMUpos_data.h"
#include "IMUpos_initialize.h"
#include "rt_nonfinite.h"
#include "rt_defines.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Variable Definitions */
static double k;

static double IMUacc[60];

static double IMUgyro[60];

static double Acc_Acc[60];

static double Acc_Vel[60];

static double Acc_Pos[60];

static double gyro_theta;

static double state;

static double gyro_phi;

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
 * mode : 2 -> F9R,  1 -> Xens
 *
 * Arguments    : const double IMUacc_c[3]
 *                const double IMUgyro_c[3]
 *                double s_time
 *                double b_acc_o[3]
 *                double b_gyro[3]
 *                double mode
 *                double *kf_psi
 *                double *gyro_psi
 *                double cent_pos_est[3]
 *                double cent_vel_est[3]
 *                double *state_o
 *                double *acc_b_phi
 *                double *acc_b_theta
 * Return Type  : void
 */
void IMUpos(const double IMUacc_c[3], const double IMUgyro_c[3], double s_time,
            double b_acc_o[3], double b_gyro[3], double mode, double *kf_psi,
            double *gyro_psi, double cent_pos_est[3], double cent_vel_est[3],
            double *state_o, double *acc_b_phi, double *acc_b_theta)
{
  static const double dv[3] = {0.0, 0.0, 9.8};
  static const signed char d_a[9] = {0, 1, 0, 1, 0, 0, 0, 0, 1};
  double b_AA[16];
  double a[9];
  double u[3];
  double dt_gyro;
  int b_i;
  int b_k;
  int i;
  int i1;
  int j;
  int jA;
  int jp1j;
  int kAcol;
  (void)s_time;
  if (!isInitialized_IMUpos) {
    IMUpos_initialize();
  }
  if (mode == 1.0) {
    dt_gyro = 0.01;
  } else {
    dt_gyro = 0.03;
  }
  /*  Acc_Steady Acc_PQR Acc_Acc_a kf_phi kf_theta %gyro_psi  */
  if (k < 900.0) {
    k++;
  }
  for (i = 0; i < 3; i++) {
    for (i1 = 0; i1 < 19; i1++) {
      jA = i1 + 20 * i;
      IMUgyro[jA] = IMUgyro[jA + 1];
      IMUacc[jA] = IMUacc[jA + 1];
    }
    jA = 20 * i + 19;
    IMUgyro[jA] = IMUgyro_c[i];
    IMUacc[jA] = IMUacc_c[i];
    Acc_Vel[i + 57] = cent_vel_est[i];
    Acc_Pos[i + 57] = cent_pos_est[i];
  }
  /*  dt_gyro = s_time - s_time_prev; */
  if (k > 300.0) {
    double AA[16];
    double K[16];
    double Pp[16];
    double b_Pp[16];
    double b_u[9];
    double c_cosPhi[4];
    double qua_kf[4];
    double xp[4];
    double acc_b[3];
    double absxk;
    double alpha;
    double b_a;
    double cosPhi;
    double cosPsi_tmp;
    double cosTheta;
    double p;
    double q;
    double r;
    double scale;
    double sinPhi;
    double sinPsi_tmp;
    double sinTheta;
    double theta;
    double unnamed_idx_2_tmp;
    signed char b_p[4];
    signed char ipiv[4];
    state = 2.0;
    if (mode == 1.0) {
      p = IMUgyro[19] - b_gyro[0];
      q = IMUgyro[39] - b_gyro[1];
      r = IMUgyro[59] - b_gyro[2];
    } else {
      p = (IMUgyro[19] - b_gyro[0]) * 3.1415926535897931 / 180.0;
      q = (IMUgyro[39] - b_gyro[1]) * 3.1415926535897931 / 180.0;
      r = (IMUgyro[59] - b_gyro[2]) * 3.1415926535897931 / 180.0;
    }
    /* %% for test */
    /*      used_phi = kf_phi; */
    /*      used_theta = kf_theta; */
    /*      used_psi = kf_psi; */
    acc_b[0] = IMUacc[19] - b_acc_o[0];
    Acc_Pos[57] = cent_pos_est[0];
    Acc_Vel[57] = cent_vel_est[0];
    acc_b[1] = IMUacc[39] - b_acc_o[1];
    Acc_Pos[58] = cent_pos_est[1];
    Acc_Vel[58] = cent_vel_est[1];
    acc_b[2] = IMUacc[59] - b_acc_o[2];
    Acc_Pos[59] = cent_pos_est[2];
    Acc_Vel[59] = cent_vel_est[2];
    for (i = 0; i < 19; i++) {
      jA = 3 * (i + 1);
      Acc_Pos[3 * i] = Acc_Pos[jA];
      Acc_Vel[3 * i] = Acc_Vel[jA];
      Acc_Acc[3 * i] = Acc_Acc[jA];
      kAcol = 3 * i + 1;
      Acc_Pos[kAcol] = Acc_Pos[jA + 1];
      Acc_Vel[kAcol] = Acc_Vel[jA + 1];
      Acc_Acc[kAcol] = Acc_Acc[jA + 1];
      kAcol = 3 * i + 2;
      Acc_Pos[kAcol] = Acc_Pos[jA + 2];
      Acc_Vel[kAcol] = Acc_Vel[jA + 2];
      Acc_Acc[kAcol] = Acc_Acc[jA + 2];
    }
    /* %%%%%%%%%%%%%%%% For Test %%%%%%%%%%%%%%%%%% */
    /*      Acc_Acc_a(:,1:end-1) = Acc_Acc_a(:,2:end); */
    /*      Acc_PQR(:,1:end-1) = Acc_PQR(:,2:end); */
    /*      Acc_Steady(:,1:end-1) = Acc_Steady(:,2:end); */
    /*   */
    /*              %         vel_t =
     * rotationVectorToMatrix([used_phi;used_theta;used_psi])*Acc_Vel(:,i-1); */
    /*              %         Acc_PQR(:,i) = 1*([0 vel_t(3) -vel_t(2);-vel_t(3)
     * 0 0;vel_t(2) 0 0]*[p;q;r]); */
    /*              %         Acc_Steady(:,i) =
     * gravity*[sin(used_theta_next);-cos(used_theta_next)*sin(used_phi_next);-cos(used_theta_next)*cos(used_phi_next)];
     */
    /*              %         Acc_Acc(:,i) =
     * rotationVectorToMatrix([used_phi;used_theta;used_psi])'*(acc_b(:,i)-Acc_PQR(:,i)-Acc_Steady(:,i));
     */
    /*              vel_t = [0 1 0;1 0 0;0 0
     * 1]*rotationVectorToMatrix([used_phi;used_theta;used_psi])*Acc_Vel(:,end-1);
     */
    /*              Acc_PQR(:,end) = [0 1 0;1 0 0;0 0 1]*([0 vel_t(3)
     * -vel_t(2);-vel_t(3) 0 0;vel_t(2) 0 0]*[p;q;r]); */
    /*  %             Acc_PQR(:,end) = [0;0;0]; */
    /*              Acc_Steady(:,end) = [0 1 0;1 0 0;0 0
     * 1]*gravity*[sin(used_theta);-cos(used_theta)*sin(used_phi);-cos(used_theta)*cos(used_phi)];
     */
    /*              Acc_Acc_a(:,end) = [0 1 0;1 0 0;0 0
     * 1]*rotationVectorToMatrix([used_phi;used_theta;used_psi])'*(acc_b-Acc_PQR(:,end)-Acc_Steady(:,end));
     */
    /* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
    /* if mode == 1 */
    u[2] = *kf_psi;
    scale = 3.3121686421112381E-170;
    absxk = fabs(u[2]);
    if (absxk > 3.3121686421112381E-170) {
      theta = 1.0;
      scale = absxk;
    } else {
      absxk /= 3.3121686421112381E-170;
      theta = absxk * absxk;
    }
    theta = scale * sqrt(theta);
    if (theta < 1.0E-6) {
      memset(&a[0], 0, 9U * sizeof(double));
      a[0] = 1.0;
      a[4] = 1.0;
      a[8] = 1.0;
    } else {
      signed char c_a[9];
      u[0] = 0.0 / theta;
      u[1] = 0.0 / theta;
      u[2] /= theta;
      alpha = cos(theta);
      b_a = sin(theta);
      for (i = 0; i < 9; i++) {
        c_a[i] = 0;
      }
      a[0] = b_a * 0.0;
      a[3] = b_a * -u[2];
      absxk = b_a * u[1];
      a[6] = absxk;
      a[1] = b_a * u[2];
      a[4] = b_a * 0.0;
      scale = b_a * -u[0];
      a[7] = scale;
      a[2] = scale;
      a[5] = absxk;
      a[8] = b_a * 0.0;
      for (b_k = 0; b_k < 3; b_k++) {
        c_a[b_k + 3 * b_k] = 1;
        absxk = u[0] * u[b_k];
        b_u[3 * b_k] = absxk;
        b_u[3 * b_k + 1] = absxk;
        b_u[3 * b_k + 2] = u[2] * u[b_k];
      }
      for (i = 0; i < 9; i++) {
        a[i] = ((double)c_a[i] * alpha + a[i]) + (1.0 - alpha) * b_u[i];
      }
    }
    u[0] = acc_b[0];
    u[1] = acc_b[1];
    u[2] = 0.0;
    for (i = 0; i < 3; i++) {
      signed char i2;
      signed char i3;
      signed char i4;
      unnamed_idx_2_tmp = 0.0;
      i2 = d_a[i];
      i3 = d_a[i + 3];
      i4 = d_a[i + 6];
      for (i1 = 0; i1 < 3; i1++) {
        unnamed_idx_2_tmp +=
            (((double)i2 * a[3 * i1] + (double)i3 * a[3 * i1 + 1]) +
             (double)i4 * a[3 * i1 + 2]) *
            u[i1];
      }
      Acc_Acc[i + 57] = unnamed_idx_2_tmp;
    }
    /*      else */
    /*          Acc_Acc(:,end) = [0 1 0;1 0 0;0 0
     * 1]*rotationVectorToMatrix([0;0;used_psi])'*([acc_b(1);acc_b(2);0]); */
    /*      end */
    absxk = tan(gyro_theta);
    scale = sin(gyro_phi);
    theta = cos(gyro_phi);
    alpha = cos(gyro_theta);
    b_u[0] = dt_gyro;
    b_u[3] = dt_gyro * (scale * absxk);
    b_u[6] = dt_gyro * (theta * absxk);
    b_u[1] = 0.0;
    b_u[4] = dt_gyro * theta;
    b_u[7] = dt_gyro * -scale;
    b_u[2] = 0.0;
    b_u[5] = dt_gyro * (scale / alpha);
    b_u[8] = dt_gyro * (theta / alpha);
    for (i = 0; i < 3; i++) {
      u[i] = (b_u[i] * p + b_u[i + 3] * q) + b_u[i + 6] * r;
    }
    double b_cosPhi;
    double b_cosTheta;
    double b_sinPhi;
    double b_sinTheta;
    absxk = *gyro_psi + u[2];
    gyro_phi += u[0];
    gyro_theta += u[1];
    *gyro_psi = absxk;
    /*      [gyro_phi,gyro_theta,gyro_psi] =
     * EulerGyroUpdate(p,q,r,dt_acc,gyro_phi,gyro_theta,gyro_psi); */
    *acc_b_phi = atan(acc_b[1] / acc_b[2]);
    *acc_b_theta =
        atan(acc_b[0] / sqrt(acc_b[1] * acc_b[1] + acc_b[2] * acc_b[2]));
    sinPhi = sin(*acc_b_phi / 2.0);
    cosPhi = cos(*acc_b_phi / 2.0);
    sinTheta = sin(*acc_b_theta / 2.0);
    cosTheta = cos(*acc_b_theta / 2.0);
    sinPsi_tmp = sin(absxk / 2.0);
    cosPsi_tmp = cos(absxk / 2.0);
    b_sinPhi = sin(gyro_phi / 2.0);
    b_cosPhi = cos(gyro_phi / 2.0);
    b_sinTheta = sin(gyro_theta / 2.0);
    b_cosTheta = cos(gyro_theta / 2.0);
    b_a = dt_gyro / 2.0;
    alpha = b_a * -p;
    unnamed_idx_2_tmp = b_a * -q;
    dt_gyro = b_a * -r;
    theta = b_a * p;
    scale = b_a * r;
    absxk = b_a * q;
    AA[0] = 1.0;
    AA[1] = theta;
    AA[2] = absxk;
    AA[3] = scale;
    AA[4] = alpha;
    AA[5] = 1.0;
    AA[6] = dt_gyro;
    AA[7] = absxk;
    AA[8] = unnamed_idx_2_tmp;
    AA[9] = scale;
    AA[10] = 1.0;
    AA[11] = alpha;
    AA[12] = dt_gyro;
    AA[13] = unnamed_idx_2_tmp;
    AA[14] = theta;
    AA[15] = 1.0;
    /*  % */
    theta = b_cosPhi * b_cosTheta;
    alpha = b_sinPhi * b_sinTheta;
    c_cosPhi[0] = theta * cosPsi_tmp + alpha * sinPsi_tmp;
    scale = b_cosPhi * b_sinTheta;
    absxk = b_sinPhi * b_cosTheta;
    c_cosPhi[1] = absxk * cosPsi_tmp - scale * sinPsi_tmp;
    c_cosPhi[2] = scale * cosPsi_tmp + absxk * sinPsi_tmp;
    c_cosPhi[3] = theta * sinPsi_tmp - alpha * cosPsi_tmp;
    for (i = 0; i < 4; i++) {
      unnamed_idx_2_tmp = 0.0;
      for (i1 = 0; i1 < 4; i1++) {
        jA = i1 << 2;
        kAcol = i + jA;
        unnamed_idx_2_tmp += AA[kAcol] * c_cosPhi[i1];
        b_AA[kAcol] =
            ((AA[i] * P[jA] + AA[i + 4] * P[jA + 1]) + AA[i + 8] * P[jA + 2]) +
            AA[i + 12] * P[jA + 3];
      }
      xp[i] = unnamed_idx_2_tmp;
      unnamed_idx_2_tmp = b_AA[i];
      alpha = b_AA[i + 4];
      scale = b_AA[i + 8];
      theta = b_AA[i + 12];
      for (i1 = 0; i1 < 4; i1++) {
        jA = i + (i1 << 2);
        Pp[jA] = (((unnamed_idx_2_tmp * AA[i1] + alpha * AA[i1 + 4]) +
                   scale * AA[i1 + 8]) +
                  theta * AA[i1 + 12]) +
                 Q[jA];
      }
    }
    for (i = 0; i < 4; i++) {
      for (i1 = 0; i1 < 4; i1++) {
        jp1j = i1 << 2;
        jA = i + jp1j;
        AA[i1 + (i << 2)] = H[jA];
        b_AA[jA] = ((H[i] * Pp[jp1j] + H[i + 4] * Pp[jp1j + 1]) +
                    H[i + 8] * Pp[jp1j + 2]) +
                   H[i + 12] * Pp[jp1j + 3];
      }
    }
    for (i = 0; i < 4; i++) {
      unnamed_idx_2_tmp = b_AA[i];
      alpha = b_AA[i + 4];
      scale = b_AA[i + 8];
      theta = b_AA[i + 12];
      for (i1 = 0; i1 < 4; i1++) {
        kAcol = i1 << 2;
        jA = i + kAcol;
        K[jA] = (((unnamed_idx_2_tmp * AA[kAcol] + alpha * AA[kAcol + 1]) +
                  scale * AA[kAcol + 2]) +
                 theta * AA[kAcol + 3]) +
                R[jA];
      }
    }
    memset(&b_AA[0], 0, 16U * sizeof(double));
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
      absxk = fabs(K[b_tmp]);
      for (b_k = 2; b_k <= jA; b_k++) {
        scale = fabs(K[(b_tmp + b_k) - 1]);
        if (scale > absxk) {
          kAcol = b_k - 1;
          absxk = scale;
        }
      }
      if (K[b_tmp + kAcol] != 0.0) {
        if (kAcol != 0) {
          jA = j + kAcol;
          ipiv[j] = (signed char)(jA + 1);
          absxk = K[j];
          K[j] = K[jA];
          K[jA] = absxk;
          absxk = K[j + 4];
          K[j + 4] = K[jA + 4];
          K[jA + 4] = absxk;
          absxk = K[j + 8];
          K[j + 8] = K[jA + 8];
          K[jA + 8] = absxk;
          absxk = K[j + 12];
          K[j + 12] = K[jA + 12];
          K[jA + 12] = absxk;
        }
        i = (b_tmp - j) + 4;
        for (b_i = jp1j; b_i <= i; b_i++) {
          K[b_i - 1] /= K[b_tmp];
        }
      }
      jA = b_tmp;
      for (kAcol = 0; kAcol <= mmj_tmp; kAcol++) {
        absxk = K[(b_tmp + (kAcol << 2)) + 4];
        if (absxk != 0.0) {
          i = jA + 6;
          i1 = (jA - j) + 8;
          for (jp1j = i; jp1j <= i1; jp1j++) {
            K[jp1j - 1] += K[((b_tmp + jp1j) - jA) - 5] * -absxk;
          }
        }
        jA += 4;
      }
    }
    b_p[0] = 1;
    b_p[1] = 2;
    b_p[2] = 3;
    b_p[3] = 4;
    if (ipiv[0] > 1) {
      jA = b_p[ipiv[0] - 1];
      b_p[ipiv[0] - 1] = 1;
      b_p[0] = (signed char)jA;
    }
    if (ipiv[1] > 2) {
      jA = b_p[ipiv[1] - 1];
      b_p[ipiv[1] - 1] = b_p[1];
      b_p[1] = (signed char)jA;
    }
    if (ipiv[2] > 3) {
      jA = b_p[ipiv[2] - 1];
      b_p[ipiv[2] - 1] = b_p[2];
      b_p[2] = (signed char)jA;
    }
    for (b_k = 0; b_k < 4; b_k++) {
      jp1j = (b_p[b_k] - 1) << 2;
      b_AA[b_k + jp1j] = 1.0;
      for (j = b_k + 1; j < 5; j++) {
        i = (j + jp1j) - 1;
        if (b_AA[i] != 0.0) {
          i1 = j + 1;
          for (b_i = i1; b_i < 5; b_i++) {
            jA = (b_i + jp1j) - 1;
            b_AA[jA] -= b_AA[i] * K[(b_i + ((j - 1) << 2)) - 1];
          }
        }
      }
    }
    for (j = 0; j < 4; j++) {
      jA = j << 2;
      for (b_k = 3; b_k >= 0; b_k--) {
        kAcol = b_k << 2;
        i = b_k + jA;
        unnamed_idx_2_tmp = b_AA[i];
        if (unnamed_idx_2_tmp != 0.0) {
          b_AA[i] = unnamed_idx_2_tmp / K[b_k + kAcol];
          for (b_i = 0; b_i < b_k; b_i++) {
            jp1j = b_i + jA;
            b_AA[jp1j] -= b_AA[i] * K[b_i + kAcol];
          }
        }
      }
      unnamed_idx_2_tmp = Pp[j];
      alpha = Pp[j + 4];
      scale = Pp[j + 8];
      theta = Pp[j + 12];
      for (i = 0; i < 4; i++) {
        i1 = i << 2;
        b_Pp[j + i1] = ((unnamed_idx_2_tmp * AA[i1] + alpha * AA[i1 + 1]) +
                        scale * AA[i1 + 2]) +
                       theta * AA[i1 + 3];
      }
    }
    theta = cosPhi * cosTheta;
    alpha = sinPhi * sinTheta;
    c_cosPhi[0] = theta * cosPsi_tmp + alpha * sinPsi_tmp;
    scale = cosPhi * sinTheta;
    absxk = sinPhi * cosTheta;
    c_cosPhi[1] = absxk * cosPsi_tmp - scale * sinPsi_tmp;
    c_cosPhi[2] = scale * cosPsi_tmp + absxk * sinPsi_tmp;
    c_cosPhi[3] = theta * sinPsi_tmp - alpha * cosPsi_tmp;
    for (i = 0; i < 4; i++) {
      unnamed_idx_2_tmp = b_Pp[i];
      alpha = b_Pp[i + 4];
      scale = b_Pp[i + 8];
      theta = b_Pp[i + 12];
      absxk = 0.0;
      for (i1 = 0; i1 < 4; i1++) {
        kAcol = i1 << 2;
        jA = i + kAcol;
        K[jA] = ((unnamed_idx_2_tmp * b_AA[kAcol] + alpha * b_AA[kAcol + 1]) +
                 scale * b_AA[kAcol + 2]) +
                theta * b_AA[kAcol + 3];
        absxk += H[jA] * xp[i1];
      }
      c_cosPhi[i] -= absxk;
    }
    for (i = 0; i < 4; i++) {
      unnamed_idx_2_tmp = 0.0;
      for (i1 = 0; i1 < 4; i1++) {
        kAcol = i1 << 2;
        jA = i + kAcol;
        unnamed_idx_2_tmp += K[jA] * c_cosPhi[i1];
        b_AA[jA] = ((K[i] * H[kAcol] + K[i + 4] * H[kAcol + 1]) +
                    K[i + 8] * H[kAcol + 2]) +
                   K[i + 12] * H[kAcol + 3];
      }
      qua_kf[i] = xp[i] + unnamed_idx_2_tmp;
      unnamed_idx_2_tmp = b_AA[i];
      alpha = b_AA[i + 4];
      scale = b_AA[i + 8];
      theta = b_AA[i + 12];
      for (i1 = 0; i1 < 4; i1++) {
        kAcol = i1 << 2;
        jA = i + kAcol;
        P[jA] =
            Pp[jA] - (((unnamed_idx_2_tmp * Pp[kAcol] + alpha * Pp[kAcol + 1]) +
                       scale * Pp[kAcol + 2]) +
                      theta * Pp[kAcol + 3]);
      }
    }
    *kf_psi = rt_atan2d_snf(
        2.0 * (qua_kf[1] * qua_kf[2] + qua_kf[0] * qua_kf[3]),
        1.0 - 2.0 * (qua_kf[2] * qua_kf[2] + qua_kf[3] * qua_kf[3]));
    Acc_Vel[57] = Acc_Vel[54] + Acc_Acc[57] * 0.01;
    Acc_Pos[57] =
        (Acc_Pos[54] + Acc_Vel[54] * 0.01) + Acc_Acc[57] * 0.0001 / 2.0;
    Acc_Vel[58] = Acc_Vel[55] + Acc_Acc[58] * 0.01;
    Acc_Pos[58] =
        (Acc_Pos[55] + Acc_Vel[55] * 0.01) + Acc_Acc[58] * 0.0001 / 2.0;
    Acc_Vel[59] = Acc_Vel[56] + Acc_Acc[59] * 0.01;
    Acc_Pos[59] =
        (Acc_Pos[56] + Acc_Vel[56] * 0.01) + Acc_Acc[59] * 0.0001 / 2.0;
  } else if (k == 300.0) {
    double acc_b[3];
    for (kAcol = 0; kAcol < 3; kAcol++) {
      double alpha;
      double unnamed_idx_2_tmp;
      jA = kAcol * 20;
      unnamed_idx_2_tmp = IMUgyro[jA];
      alpha = IMUacc[jA];
      for (b_k = 0; b_k < 19; b_k++) {
        i = (jA + b_k) + 1;
        unnamed_idx_2_tmp += IMUgyro[i];
        alpha += IMUacc[i];
      }
      b_gyro[kAcol] = unnamed_idx_2_tmp / 20.0;
      unnamed_idx_2_tmp = alpha / 20.0 + dv[kAcol];
      b_acc_o[kAcol] = unnamed_idx_2_tmp;
      acc_b[kAcol] = IMUacc[20 * kAcol + 19] - unnamed_idx_2_tmp;
    }
    *acc_b_phi = atan(acc_b[1] / acc_b[2]);
    *acc_b_theta =
        atan(acc_b[0] / sqrt(acc_b[1] * acc_b[1] + acc_b[2] * acc_b[2]));
    gyro_phi = *acc_b_phi;
    gyro_theta = *acc_b_theta;
    *gyro_psi = *kf_psi;
    state = 1.0;
  } else {
    for (i = 0; i < 19; i++) {
      jA = 3 * (i + 1);
      Acc_Vel[3 * i] = Acc_Vel[jA];
      Acc_Acc[3 * i] = Acc_Acc[jA];
      Acc_Pos[3 * i] = Acc_Pos[jA];
      kAcol = 3 * i + 1;
      Acc_Vel[kAcol] = Acc_Vel[jA + 1];
      Acc_Acc[kAcol] = Acc_Acc[jA + 1];
      Acc_Pos[kAcol] = Acc_Pos[jA + 1];
      kAcol = 3 * i + 2;
      Acc_Vel[kAcol] = Acc_Vel[jA + 2];
      Acc_Acc[kAcol] = Acc_Acc[jA + 2];
      Acc_Pos[kAcol] = Acc_Pos[jA + 2];
    }
    Acc_Acc[57] = 0.0;
    Acc_Acc[58] = 0.0;
    Acc_Acc[59] = 0.0;
    Acc_Vel[57] = cent_vel_est[0];
    Acc_Vel[58] = cent_vel_est[1];
    Acc_Vel[59] = cent_vel_est[2];
    Acc_Pos[57] = cent_pos_est[0];
    Acc_Pos[58] = cent_pos_est[1];
    Acc_Pos[59] = cent_pos_est[2];
    *acc_b_phi = atan(IMUacc[39] / IMUacc[59]);
    *acc_b_theta = atan(
        IMUacc[19] / sqrt(IMUacc[39] * IMUacc[39] + IMUacc[59] * IMUacc[59]));
    gyro_phi = gyro_phi * (k - 1.0) / k + *acc_b_phi / k;
    gyro_theta = gyro_theta * (k - 1.0) / k + *acc_b_theta / k;
    *gyro_psi = *kf_psi;
    /*              b_gyro = b_gyro*(k0(5)-1)/k0(5)+IMUgyro(end,:)/k0(5); */
  }
  cent_pos_est[0] = Acc_Pos[57];
  cent_vel_est[0] = Acc_Vel[57];
  cent_pos_est[1] = Acc_Pos[58];
  cent_vel_est[1] = Acc_Vel[58];
  cent_pos_est[2] = Acc_Pos[59];
  cent_vel_est[2] = Acc_Vel[59];
  *state_o = state;
}

/*
 * mode : 2 -> F9R,  1 -> Xens
 *
 * Arguments    : void
 * Return Type  : void
 */
void IMUpos_init(void)
{
  k = 0.0;
  state = 0.0;
  /*  0 : init, */
  memset(&IMUacc[0], 0, 60U * sizeof(double));
  memset(&IMUgyro[0], 0, 60U * sizeof(double));
  memset(&Acc_Acc[0], 0, 60U * sizeof(double));
  memset(&Acc_Vel[0], 0, 60U * sizeof(double));
  memset(&Acc_Pos[0], 0, 60U * sizeof(double));
  /* %%%% For Test %%%%%% */
  /*      Acc_Steady = zeros(3,2*Lv); */
  /*      Acc_PQR = zeros(3,2*Lv); */
  /*      Acc_Acc_a = zeros(3,2*Lv ); */
  /*      kf_phi = 0; */
  /*      kf_theta = 0; */
  /* %%%%%%%%%%%%%%%%%%%%%% */
  gyro_phi = 0.0;
  gyro_theta = 0.0;
  /*      gyro_psi = 0; */
}

/*
 * File trailer for IMUpos.c
 *
 * [EOF]
 */
