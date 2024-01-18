//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: fusion2.cpp
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 30-Nov-2023 16:43:53
//

// Include Files
#include "fusion2.h"
#include "EulerKalman_2.h"
#include "PosKalman2.h"
#include "fusion2_data.h"
#include "fusion2_initialize.h"
#include "fusion2_rtwutil.h"
#include "mod.h"
#include "rotationVectorToMatrix.h"
#include "rt_nonfinite.h"
#include <algorithm>
#include <cmath>
#include <cstring>

// Variable Definitions
static double UWB_LS_Pos[60];

static double UWB_M_Pos[60];

static double UWB_M_Vel[60];

static double kf_theta;

static double kf_phi;

// Function Definitions
//
// Arguments    : double kl
//                const double k0[5]
//                const creal_T tag_pos_est[4]
//                const creal_T tag_center_vel_est
//                double cent_pos_est[3]
//                double cent_vel_est[3]
//                double b_acc_o[3]
//                double acc_b_phi
//                const double b_Acc_Pos[60]
//                const double b_Acc_Vel[60]
//                double kalman_on
//                double init_flag
//                double UWBErrSum
//                double *kf_psi
//                double *gyro_psi
//                double heading_est
//                double acc_b_theta
//                double IMUSel
//                double Nanchor
//                double state_IMU
// Return Type  : creal_T
//
creal_T fusion2(double kl, const double k0, const creal_T tag_pos_est[4],
                const creal_T tag_center_vel_est, double cent_pos_est[3],
                double cent_vel_est[3], double b_acc_o[3], double acc_b_phi,
                bool kalman_on, double init_flag, double UWBErrSum, double *kf_psi,
                double *gyro_psi, double heading_est, double acc_b_theta,
                double IMUSel, double Nanchor, double state_IMU)
{
  static const double dv2[36]{
      0.01, 0.0, 0.0,  0.0, 0.0,  0.0, 0.0, 0.01, 0.0, 0.0,  0.0, 0.0,
      0.0,  0.0, 0.01, 0.0, 0.0,  0.0, 0.0, 0.0,  0.0, 0.01, 0.0, 0.0,
      0.0,  0.0, 0.0,  0.0, 0.01, 0.0, 0.0, 0.0,  0.0, 0.0,  0.0, 0.01};
  static const double dv3[36]{0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.1, 0.0,
                              0.0, 0.0, 0.0, 0.0, 0.0, 0.1, 0.0, 0.0, 0.0,
                              0.0, 0.0, 0.0, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0,
                              0.0, 0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.1};
  static const double dv1[16]{1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0,
                              0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0};
  static const signed char b_iv[36]{1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
                                    0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0,
                                    0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1};
  creal_T IMUposU;
  double b_R[36];
  double cosPhi;
  double sinTheta;
  if (!isInitialized_fusion2) {
    fusion2_initialize();
  }
  cosPhi = (9.0 * tag_pos_est[2].re + tag_pos_est[0].re) +
           (9.0 * tag_pos_est[3].re + tag_pos_est[1].re);
  sinTheta = (9.0 * tag_pos_est[2].im + tag_pos_est[0].im) +
             (9.0 * tag_pos_est[3].im + tag_pos_est[1].im);
  if (sinTheta == 0.0) {
    IMUposU.re = cosPhi / 20.0;
    IMUposU.im = 0.0;
  } else if (cosPhi == 0.0) {
    IMUposU.re = 0.0;
    IMUposU.im = sinTheta / 20.0;
  } else {
    IMUposU.re = cosPhi / 20.0;
    IMUposU.im = sinTheta / 20.0;
  }
  if (k0 > 50.0) {
    int UWB_LS_Pos_tmp;
    int UWB_M_Pos_tmp;
    int i;
    //  INS/UWB
    for (i = 0; i < 19; i++) {
      UWB_LS_Pos_tmp = 3 * (i + 1);
      UWB_LS_Pos[3 * i] = UWB_LS_Pos[UWB_LS_Pos_tmp];
      UWB_LS_Pos[3 * i + 1] = UWB_LS_Pos[UWB_LS_Pos_tmp + 1];
      UWB_LS_Pos[3 * i + 2] = UWB_LS_Pos[UWB_LS_Pos_tmp + 2];
    }
    UWB_LS_Pos[57] = IMUposU.re;
    UWB_LS_Pos[58] = IMUposU.im;
    UWB_LS_Pos[59] = 1.53;
    if ((kalman_on == true) && (init_flag > 2.0) && (UWBErrSum < 0.5)) {
      double c_cosPhi[4];
      double d_cosPhi[4];
      double b_cosPhi;
      double b_cosPhi_tmp;
      double b_cosPsi;
      double b_cosTheta;
      double b_sinPhi;
      double b_sinPsi;
      double b_sinTheta;
      double cosPhi_tmp;
      double cosPsi;
      double cosTheta;
      double sinPhi;
      double sinPsi;
      cosPhi = coder::b_mod(-heading_est);
      sinTheta = std::round(std::abs(cosPhi - *kf_psi) / 6.2831853071795862);
      if (sinTheta < 2.147483648E+9) {
        i = static_cast<int>(sinTheta);
      } else if (sinTheta >= 2.147483648E+9) {
        i = MAX_int32_T;
      } else {
        i = 0;
      }
      if (coder::b_mod(-heading_est) - *kf_psi > 3.1415926535897931) {
        cosPhi -= 6.2831853071795862 * static_cast<double>(i);
      } else {
        cosPhi += 6.2831853071795862 * static_cast<double>(i);
      }
      sinPhi = std::sin(acc_b_phi / 2.0);
      b_cosPhi = std::cos(acc_b_phi / 2.0);
      b_sinTheta = std::sin(acc_b_theta / 2.0);
      cosTheta = std::cos(acc_b_theta / 2.0);
      sinPsi = std::sin(cosPhi / 2.0);
      cosPsi = std::cos(cosPhi / 2.0);
      b_sinPhi = std::sin(kf_phi / 2.0);
      cosPhi = std::cos(kf_phi / 2.0);
      sinTheta = std::sin(kf_theta / 2.0);
      b_cosTheta = std::cos(kf_theta / 2.0);
      b_sinPsi = std::sin(*kf_psi / 2.0);
      b_cosPsi = std::cos(*kf_psi / 2.0);
      cosPhi_tmp = cosPhi * b_cosTheta;
      b_cosPhi_tmp = b_sinPhi * sinTheta;
      c_cosPhi[0] = cosPhi_tmp * b_cosPsi + b_cosPhi_tmp * b_sinPsi;
      sinTheta *= cosPhi;
      cosPhi = b_sinPhi * b_cosTheta;
      c_cosPhi[1] = cosPhi * b_cosPsi - sinTheta * b_sinPsi;
      c_cosPhi[2] = sinTheta * b_cosPsi + cosPhi * b_sinPsi;
      c_cosPhi[3] = cosPhi_tmp * b_sinPsi - b_cosPhi_tmp * b_cosPsi;
      cosPhi_tmp = b_cosPhi * cosTheta;
      b_cosPhi_tmp = sinPhi * b_sinTheta;
      d_cosPhi[0] = cosPhi_tmp * cosPsi + b_cosPhi_tmp * sinPsi;
      sinTheta = b_cosPhi * b_sinTheta;
      cosPhi = sinPhi * cosTheta;
      d_cosPhi[1] = cosPhi * cosPsi - sinTheta * sinPsi;
      d_cosPhi[2] = sinTheta * cosPsi + cosPhi * sinPsi;
      d_cosPhi[3] = cosPhi_tmp * sinPsi - b_cosPhi_tmp * cosPsi;
      EulerKalman_2(c_cosPhi, dv1, d_cosPhi, &kf_phi, &kf_theta, kf_psi);
      *gyro_psi = *kf_psi;
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
      //                      UWB_M_Pos(:,end) = (Tm+Td);
      //                      UWB_M_Pos(:,end) = (Ta);
      UWB_M_Pos[57] = IMUposU.re;
      UWB_M_Pos[58] = IMUposU.im;
      UWB_M_Pos[59] = 0.0;
      UWB_M_Vel[57] = tag_center_vel_est.re;
      UWB_M_Vel[58] = tag_center_vel_est.im;
      UWB_M_Vel[59] = 0.0;
      //                      UWB_M_Vel(:,end) =
      //                      (UWB_M_Pos(:,end)-UWB_M_Pos(:,end-9))/10;
      //                      tag_center_vel_est
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
      UWB_M_Pos[59] = 1.53;
      UWB_M_Vel[57] = 0.0;
      UWB_M_Vel[58] = 0.0;
      UWB_M_Vel[59] = 0.0;
    }
    if ((kalman_on == true) && (init_flag > 2.0) && (UWBErrSum < 1.0)) {
      double A[144];
      double x[12];
      double Cb2n[9];
      double a_tmp[9];
      double b_dv[3];
      int A_tmp;
      // x = [cent_pos_est;cent_vel_est(1:2);0;TEMP;TEMP_bias];
      x[9] = b_acc_o[0];
      x[10] = b_acc_o[1];
      x[11] = b_acc_o[2];
      // Cb2n = [0 1 0;1 0 0;0 0 1]*rotationVectorToMatrix([0;0;kf_psi])';
      b_dv[0] = kf_phi;
      b_dv[1] = kf_theta;
      b_dv[2] = *kf_psi;
      coder::rotationVectorToMatrix(b_dv, a_tmp);
      for (i = 0; i < 3; i++) {
        signed char i1;
        signed char i2;
        signed char i3;
        x[i] = cent_pos_est[i];
        x[i + 3] = cent_vel_est[i];
        x[i + 6] = 0.0;
        i1 = iv1[i];
        i2 = iv1[i + 3];
        i3 = iv1[i + 6];
        for (UWB_LS_Pos_tmp = 0; UWB_LS_Pos_tmp < 3; UWB_LS_Pos_tmp++) {
          Cb2n[i + 3 * UWB_LS_Pos_tmp] =
              (static_cast<double>(i1) * a_tmp[UWB_LS_Pos_tmp] +
               static_cast<double>(i2) * a_tmp[UWB_LS_Pos_tmp + 3]) +
              static_cast<double>(i3) * a_tmp[UWB_LS_Pos_tmp + 6];
        }
      }
      for (i = 0; i < 144; i++) {
        A[i] = iv[i];
      }
      for (i = 0; i < 3; i++) {
        UWB_M_Pos_tmp = 12 * (i + 3);
        A[UWB_M_Pos_tmp] = dv[3 * i];
        A_tmp = 12 * (i + 6);
        A[A_tmp] = Cb2n[3 * i] * 0.0001 / 2.0;
        UWB_LS_Pos_tmp = 3 * i + 1;
        A[UWB_M_Pos_tmp + 1] = dv[UWB_LS_Pos_tmp];
        A[A_tmp + 1] = Cb2n[UWB_LS_Pos_tmp] * 0.0001 / 2.0;
        UWB_LS_Pos_tmp = 3 * i + 2;
        A[UWB_M_Pos_tmp + 2] = dv[UWB_LS_Pos_tmp];
        A[A_tmp + 2] = Cb2n[UWB_LS_Pos_tmp] * 0.0001 / 2.0;
      }
      for (i = 0; i < 9; i++) {
        a_tmp[i] = -Cb2n[i];
      }
      for (i = 0; i < 3; i++) {
        sinTheta = a_tmp[3 * i];
        UWB_M_Pos_tmp = 12 * (i + 9);
        A[UWB_M_Pos_tmp] = sinTheta * 0.0001 / 2.0;
        A_tmp = 12 * (i + 6);
        A[A_tmp + 3] = Cb2n[3 * i] * 0.01;
        A[UWB_M_Pos_tmp + 3] = sinTheta * 0.01;
        UWB_LS_Pos_tmp = 3 * i + 1;
        sinTheta = a_tmp[UWB_LS_Pos_tmp];
        A[UWB_M_Pos_tmp + 1] = sinTheta * 0.0001 / 2.0;
        A[A_tmp + 4] = Cb2n[UWB_LS_Pos_tmp] * 0.01;
        A[UWB_M_Pos_tmp + 4] = sinTheta * 0.01;
        UWB_LS_Pos_tmp = 3 * i + 2;
        sinTheta = a_tmp[UWB_LS_Pos_tmp];
        A[UWB_M_Pos_tmp + 2] = sinTheta * 0.0001 / 2.0;
        A[A_tmp + 5] = Cb2n[UWB_LS_Pos_tmp] * 0.01;
        A[UWB_M_Pos_tmp + 5] = sinTheta * 0.01;
      }
      if (rt_hypotd_snf(UWB_M_Pos[57] - cent_pos_est[0],
                        UWB_M_Pos[58] - cent_pos_est[1]) < 2.0) {
        if (IMUSel == 1.0) {
          for (i = 0; i < 36; i++) {
            b_R[i] = b_iv[i];
          }
        } else {
          std::copy(&dv3[0], &dv3[36], &b_R[0]);
        }
        for (i = 0; i < 3; i++) {
          a_tmp[3 * i] = b_R[6 * i];
          a_tmp[3 * i + 1] = b_R[6 * i + 1];
          a_tmp[3 * i + 2] = b_R[6 * i + 2];
        }
        PosKalman2(x, A, *(double(*)[3]) & UWB_M_Pos[57], a_tmp);
      } else {
        if (IMUSel == 1.0) {
          std::copy(&dv3[0], &dv3[36], &b_R[0]);
        } else {
          std::copy(&dv2[0], &dv2[36], &b_R[0]);
        }
        for (i = 0; i < 3; i++) {
          a_tmp[3 * i] = b_R[6 * i];
          a_tmp[3 * i + 1] = b_R[6 * i + 1];
          a_tmp[3 * i + 2] = b_R[6 * i + 2];
        }
        PosKalman2(x, A, *(double(*)[3]) & UWB_M_Pos[57], a_tmp);
        //                          z = [UWB_M_Pos(:,end);UWB_M_Vel(:,end)];
        //                          x_n = PosKalman(x,A,z,R);
        x[0] = UWB_M_Pos[57];
        x[3] = UWB_M_Vel[57];
        x[1] = UWB_M_Pos[58];
        x[4] = UWB_M_Vel[58];
        x[2] = UWB_M_Pos[59];
        x[5] = UWB_M_Vel[59];
      }
      if (Nanchor > 1.0) {
        if (state_IMU == 2.0) {
          b_acc_o[0] = x[9];
          b_acc_o[1] = x[10];
          b_acc_o[2] = x[11];
        }
        cent_pos_est[0] = x[0];
        cent_vel_est[0] = x[3];
        cent_pos_est[1] = x[1];
        cent_vel_est[1] = x[4];
        cent_pos_est[2] = x[2];
        cent_vel_est[2] = x[5];
      }
    }
  }
  return IMUposU;
}

//
// Arguments    : void
// Return Type  : void
//
void fusion2_init()
{
  kf_theta = 0.0;
  kf_phi = 0.0;
  std::memset(&UWB_LS_Pos[0], 0, 60U * sizeof(double));
  std::memset(&UWB_M_Pos[0], 0, 60U * sizeof(double));
  std::memset(&UWB_M_Vel[0], 0, 60U * sizeof(double));
}

//
// File trailer for fusion2.cpp
//
// [EOF]
//
