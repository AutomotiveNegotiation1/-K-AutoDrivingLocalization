//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: IMUpos.cpp
//
// MATLAB Coder version            : 5.6
// C/C++ source code generated on  : 17-Oct-2023 17:24:31
//

// Include Files
#include "IMUpos.h"
#include "EulerKalman_2.h"
#include "fusion_data.h"
#include "fusion_initialize.h"
#include "rotationVectorToMatrix.h"
#include "rt_nonfinite.h"
#include <cmath>
#include <cstring>

// Variable Definitions
static double b_k;

static double IMUacc[60];

static double IMUgyro[60];

static double Acc_Acc[60];

static double b_Acc_Vel[60];

static double b_Acc_Pos[60];

static double gyro_theta;

static double state;

static double gyro_phi;

// Function Definitions
//
// mode : 2 -> F9R,  1 -> Xens
//
// Arguments    : const double IMUacc_c[3]
//                const double IMUgyro_c[3]
//                double s_time
//                double b_acc_o[3]
//                double b_gyro[3]
//                double mode
//                double *kf_psi
//                double *gyro_psi
//                double cent_pos_est[3]
//                double cent_vel_est[3]
//                double *state_o
//                double *acc_b_phi
//                double *acc_b_theta
// Return Type  : void
//
void IMUpos(const double IMUacc_c[3], const double IMUgyro_c[3], double s_time,
            double b_acc_o[3], double b_gyro[3], double mode, double *kf_psi,
            double *gyro_psi, double cent_pos_est[3], double cent_vel_est[3],
            double *state_o, double *acc_b_phi, double *acc_b_theta)
{
  static const double b_dv[3]{0.0, 0.0, 9.8};
  double dt_gyro;
  int Acc_Vel_tmp;
  int IMUgyro_tmp;
  int i;
  if (!isInitialized_fusion) {
    fusion_initialize();
  }
  if (mode == 1.0) {
    dt_gyro = 0.01;
  } else {
    dt_gyro = 0.03;
  }
  //  Acc_Steady Acc_PQR Acc_Acc_a kf_phi kf_theta %gyro_psi
  if (b_k < 900.0) {
    b_k++;
  }
  for (i = 0; i < 3; i++) {
    for (Acc_Vel_tmp = 0; Acc_Vel_tmp < 19; Acc_Vel_tmp++) {
      IMUgyro_tmp = Acc_Vel_tmp + 20 * i;
      IMUgyro[IMUgyro_tmp] = IMUgyro[IMUgyro_tmp + 1];
      IMUacc[IMUgyro_tmp] = IMUacc[IMUgyro_tmp + 1];
    }
    IMUgyro_tmp = 20 * i + 19;
    IMUgyro[IMUgyro_tmp] = IMUgyro_c[i];
    IMUacc[IMUgyro_tmp] = IMUacc_c[i];
    b_Acc_Vel[i + 57] = cent_vel_est[i];
    b_Acc_Pos[i + 57] = cent_pos_est[i];
  }
  //  dt_gyro = s_time - s_time_prev;
  if (b_k > 300.0) {
    double b_dt_gyro[9];
    double acc_b[3];
    double b_acc_b[3];
    double NextEuler_idx_2;
    double UpdateMat_tmp;
    double b_UpdateMat_tmp;
    double b_r;
    double cosPhi;
    double p;
    double q;
    state = 2.0;
    if (mode == 1.0) {
      p = IMUgyro[19] - b_gyro[0];
      q = IMUgyro[39] - b_gyro[1];
      b_r = IMUgyro[59] - b_gyro[2];
    } else {
      p = (IMUgyro[19] - b_gyro[0]) * 3.1415926535897931 / 180.0;
      q = (IMUgyro[39] - b_gyro[1]) * 3.1415926535897931 / 180.0;
      b_r = (IMUgyro[59] - b_gyro[2]) * 3.1415926535897931 / 180.0;
    }
    // %% for test
    //      used_phi = kf_phi;
    //      used_theta = kf_theta;
    //      used_psi = kf_psi;
    acc_b[0] = IMUacc[19] - b_acc_o[0];
    b_Acc_Pos[57] = cent_pos_est[0];
    b_Acc_Vel[57] = cent_vel_est[0];
    acc_b[1] = IMUacc[39] - b_acc_o[1];
    b_Acc_Pos[58] = cent_pos_est[1];
    b_Acc_Vel[58] = cent_vel_est[1];
    acc_b[2] = IMUacc[59] - b_acc_o[2];
    b_Acc_Pos[59] = cent_pos_est[2];
    b_Acc_Vel[59] = cent_vel_est[2];
    for (i = 0; i < 19; i++) {
      IMUgyro_tmp = 3 * (i + 1);
      b_Acc_Pos[3 * i] = b_Acc_Pos[IMUgyro_tmp];
      b_Acc_Vel[3 * i] = b_Acc_Vel[IMUgyro_tmp];
      Acc_Acc[3 * i] = Acc_Acc[IMUgyro_tmp];
      Acc_Vel_tmp = 3 * i + 1;
      b_Acc_Pos[Acc_Vel_tmp] = b_Acc_Pos[IMUgyro_tmp + 1];
      b_Acc_Vel[Acc_Vel_tmp] = b_Acc_Vel[IMUgyro_tmp + 1];
      Acc_Acc[Acc_Vel_tmp] = Acc_Acc[IMUgyro_tmp + 1];
      Acc_Vel_tmp = 3 * i + 2;
      b_Acc_Pos[Acc_Vel_tmp] = b_Acc_Pos[IMUgyro_tmp + 2];
      b_Acc_Vel[Acc_Vel_tmp] = b_Acc_Vel[IMUgyro_tmp + 2];
      Acc_Acc[Acc_Vel_tmp] = Acc_Acc[IMUgyro_tmp + 2];
    }
    // %%%%%%%%%%%%%%%% For Test %%%%%%%%%%%%%%%%%%
    //      Acc_Acc_a(:,1:end-1) = Acc_Acc_a(:,2:end);
    //      Acc_PQR(:,1:end-1) = Acc_PQR(:,2:end);
    //      Acc_Steady(:,1:end-1) = Acc_Steady(:,2:end);
    //
    //              %         vel_t =
    //              rotationVectorToMatrix([used_phi;used_theta;used_psi])*Acc_Vel(:,i-1);
    //              %         Acc_PQR(:,i) = 1*([0 vel_t(3) -vel_t(2);-vel_t(3)
    //              0 0;vel_t(2) 0 0]*[p;q;r]); %         Acc_Steady(:,i) =
    //              gravity*[sin(used_theta_next);-cos(used_theta_next)*sin(used_phi_next);-cos(used_theta_next)*cos(used_phi_next)];
    //              %         Acc_Acc(:,i) =
    //              rotationVectorToMatrix([used_phi;used_theta;used_psi])'*(acc_b(:,i)-Acc_PQR(:,i)-Acc_Steady(:,i));
    //              vel_t = [0 1 0;1 0 0;0 0
    //              1]*rotationVectorToMatrix([used_phi;used_theta;used_psi])*Acc_Vel(:,end-1);
    //              Acc_PQR(:,end) = [0 1 0;1 0 0;0 0 1]*([0 vel_t(3)
    //              -vel_t(2);-vel_t(3) 0 0;vel_t(2) 0 0]*[p;q;r]);
    //  %             Acc_PQR(:,end) = [0;0;0];
    //              Acc_Steady(:,end) = [0 1 0;1 0 0;0 0
    //              1]*gravity*[sin(used_theta);-cos(used_theta)*sin(used_phi);-cos(used_theta)*cos(used_phi)];
    //              Acc_Acc_a(:,end) = [0 1 0;1 0 0;0 0
    //              1]*rotationVectorToMatrix([used_phi;used_theta;used_psi])'*(acc_b-Acc_PQR(:,end)-Acc_Steady(:,end));
    // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    // if mode == 1
    b_acc_b[0] = 0.0;
    b_acc_b[1] = 0.0;
    b_acc_b[2] = *kf_psi;
    coder::rotationVectorToMatrix(b_acc_b, b_dt_gyro);
    b_acc_b[0] = acc_b[0];
    b_acc_b[1] = acc_b[1];
    b_acc_b[2] = 0.0;
    for (i = 0; i < 3; i++) {
      signed char i1;
      signed char i2;
      signed char i3;
      UpdateMat_tmp = 0.0;
      i1 = iv1[i];
      i2 = iv1[i + 3];
      i3 = iv1[i + 6];
      for (Acc_Vel_tmp = 0; Acc_Vel_tmp < 3; Acc_Vel_tmp++) {
        UpdateMat_tmp +=
            ((static_cast<double>(i1) * b_dt_gyro[Acc_Vel_tmp] +
              static_cast<double>(i2) * b_dt_gyro[Acc_Vel_tmp + 3]) +
             static_cast<double>(i3) * b_dt_gyro[Acc_Vel_tmp + 6]) *
            b_acc_b[Acc_Vel_tmp];
      }
      Acc_Acc[i + 57] = UpdateMat_tmp;
    }
    //      else
    //          Acc_Acc(:,end) = [0 1 0;1 0 0;0 0
    //          1]*rotationVectorToMatrix([0;0;used_psi])'*([acc_b(1);acc_b(2);0]);
    //      end
    NextEuler_idx_2 = std::tan(gyro_theta);
    UpdateMat_tmp = std::sin(gyro_phi);
    cosPhi = std::cos(gyro_phi);
    b_UpdateMat_tmp = std::cos(gyro_theta);
    b_dt_gyro[0] = dt_gyro;
    b_dt_gyro[3] = dt_gyro * (UpdateMat_tmp * NextEuler_idx_2);
    b_dt_gyro[6] = dt_gyro * (cosPhi * NextEuler_idx_2);
    b_dt_gyro[1] = 0.0;
    b_dt_gyro[4] = dt_gyro * cosPhi;
    b_dt_gyro[7] = dt_gyro * -UpdateMat_tmp;
    b_dt_gyro[2] = 0.0;
    b_dt_gyro[5] = dt_gyro * (UpdateMat_tmp / b_UpdateMat_tmp);
    b_dt_gyro[8] = dt_gyro * (cosPhi / b_UpdateMat_tmp);
    for (i = 0; i < 3; i++) {
      b_acc_b[i] =
          (b_dt_gyro[i] * p + b_dt_gyro[i + 3] * q) + b_dt_gyro[i + 6] * b_r;
    }
    double b_I[16];
    double c_cosPhi[4];
    double d_cosPhi[4];
    double a_idx_0_tmp;
    double b_cosPhi;
    double b_cosTheta;
    double b_sinPhi;
    double b_sinTheta;
    double cosPsi_tmp;
    double cosTheta;
    double sinPhi;
    double sinPsi_tmp;
    double sinTheta;
    double unnamed_idx_1_tmp;
    double unnamed_idx_2_tmp;
    double unnamed_idx_3_tmp;
    NextEuler_idx_2 = *gyro_psi + b_acc_b[2];
    gyro_phi += b_acc_b[0];
    gyro_theta += b_acc_b[1];
    *gyro_psi = NextEuler_idx_2;
    //      [gyro_phi,gyro_theta,gyro_psi] =
    //      EulerGyroUpdate(p,q,r,dt_acc,gyro_phi,gyro_theta,gyro_psi);
    *acc_b_phi = std::atan(acc_b[1] / acc_b[2]);
    *acc_b_theta = std::atan(
        acc_b[0] / std::sqrt(acc_b[1] * acc_b[1] + acc_b[2] * acc_b[2]));
    sinPhi = std::sin(*acc_b_phi / 2.0);
    b_cosPhi = std::cos(*acc_b_phi / 2.0);
    sinTheta = std::sin(*acc_b_theta / 2.0);
    cosTheta = std::cos(*acc_b_theta / 2.0);
    sinPsi_tmp = std::sin(NextEuler_idx_2 / 2.0);
    cosPsi_tmp = std::cos(NextEuler_idx_2 / 2.0);
    b_sinPhi = std::sin(gyro_phi / 2.0);
    cosPhi = std::cos(gyro_phi / 2.0);
    b_sinTheta = std::sin(gyro_theta / 2.0);
    b_cosTheta = std::cos(gyro_theta / 2.0);
    UpdateMat_tmp = dt_gyro / 2.0;
    //  %
    unnamed_idx_1_tmp = UpdateMat_tmp * -p;
    unnamed_idx_2_tmp = UpdateMat_tmp * -q;
    unnamed_idx_3_tmp = UpdateMat_tmp * -b_r;
    a_idx_0_tmp = UpdateMat_tmp * p;
    p = UpdateMat_tmp * b_r;
    b_UpdateMat_tmp = UpdateMat_tmp * q;
    NextEuler_idx_2 = cosPhi * b_cosTheta;
    dt_gyro = b_sinPhi * b_sinTheta;
    c_cosPhi[0] = NextEuler_idx_2 * cosPsi_tmp + dt_gyro * sinPsi_tmp;
    cosPhi *= b_sinTheta;
    UpdateMat_tmp = b_sinPhi * b_cosTheta;
    c_cosPhi[1] = UpdateMat_tmp * cosPsi_tmp - cosPhi * sinPsi_tmp;
    c_cosPhi[2] = cosPhi * cosPsi_tmp + UpdateMat_tmp * sinPsi_tmp;
    c_cosPhi[3] = NextEuler_idx_2 * sinPsi_tmp - dt_gyro * cosPsi_tmp;
    b_I[0] = 1.0;
    b_I[1] = a_idx_0_tmp;
    b_I[2] = b_UpdateMat_tmp;
    b_I[3] = p;
    b_I[4] = unnamed_idx_1_tmp;
    b_I[5] = 1.0;
    b_I[6] = unnamed_idx_3_tmp;
    b_I[7] = b_UpdateMat_tmp;
    b_I[8] = unnamed_idx_2_tmp;
    b_I[9] = p;
    b_I[10] = 1.0;
    b_I[11] = unnamed_idx_1_tmp;
    b_I[12] = unnamed_idx_3_tmp;
    b_I[13] = unnamed_idx_2_tmp;
    b_I[14] = a_idx_0_tmp;
    b_I[15] = 1.0;
    NextEuler_idx_2 = b_cosPhi * cosTheta;
    dt_gyro = sinPhi * sinTheta;
    d_cosPhi[0] = NextEuler_idx_2 * cosPsi_tmp + dt_gyro * sinPsi_tmp;
    cosPhi = b_cosPhi * sinTheta;
    UpdateMat_tmp = sinPhi * cosTheta;
    d_cosPhi[1] = UpdateMat_tmp * cosPsi_tmp - cosPhi * sinPsi_tmp;
    d_cosPhi[2] = cosPhi * cosPsi_tmp + UpdateMat_tmp * sinPsi_tmp;
    d_cosPhi[3] = NextEuler_idx_2 * sinPsi_tmp - dt_gyro * cosPsi_tmp;
    EulerKalman_2(c_cosPhi, b_I, d_cosPhi, UpdateMat_tmp, kf_psi);
    b_Acc_Vel[57] = b_Acc_Vel[54] + Acc_Acc[57] * 0.01;
    b_Acc_Pos[57] =
        (b_Acc_Pos[54] + b_Acc_Vel[54] * 0.01) + Acc_Acc[57] * 0.0001 / 2.0;
    b_Acc_Vel[58] = b_Acc_Vel[55] + Acc_Acc[58] * 0.01;
    b_Acc_Pos[58] =
        (b_Acc_Pos[55] + b_Acc_Vel[55] * 0.01) + Acc_Acc[58] * 0.0001 / 2.0;
    b_Acc_Vel[59] = b_Acc_Vel[56] + Acc_Acc[59] * 0.01;
    b_Acc_Pos[59] =
        (b_Acc_Pos[56] + b_Acc_Vel[56] * 0.01) + Acc_Acc[59] * 0.0001 / 2.0;
  } else if (b_k == 300.0) {
    double acc_b[3];
    for (Acc_Vel_tmp = 0; Acc_Vel_tmp < 3; Acc_Vel_tmp++) {
      double NextEuler_idx_2;
      double UpdateMat_tmp;
      IMUgyro_tmp = Acc_Vel_tmp * 20;
      UpdateMat_tmp = IMUgyro[IMUgyro_tmp];
      NextEuler_idx_2 = IMUacc[IMUgyro_tmp];
      for (int c_k{0}; c_k < 19; c_k++) {
        i = (IMUgyro_tmp + c_k) + 1;
        UpdateMat_tmp += IMUgyro[i];
        NextEuler_idx_2 += IMUacc[i];
      }
      b_gyro[Acc_Vel_tmp] = UpdateMat_tmp / 20.0;
      UpdateMat_tmp = NextEuler_idx_2 / 20.0 + b_dv[Acc_Vel_tmp];
      b_acc_o[Acc_Vel_tmp] = UpdateMat_tmp;
      acc_b[Acc_Vel_tmp] = IMUacc[20 * Acc_Vel_tmp + 19] - UpdateMat_tmp;
    }
    *acc_b_phi = std::atan(acc_b[1] / acc_b[2]);
    *acc_b_theta = std::atan(
        acc_b[0] / std::sqrt(acc_b[1] * acc_b[1] + acc_b[2] * acc_b[2]));
    gyro_phi = *acc_b_phi;
    gyro_theta = *acc_b_theta;
    *gyro_psi = *kf_psi;
    state = 1.0;
  } else {
    for (i = 0; i < 19; i++) {
      IMUgyro_tmp = 3 * (i + 1);
      b_Acc_Vel[3 * i] = b_Acc_Vel[IMUgyro_tmp];
      Acc_Acc[3 * i] = Acc_Acc[IMUgyro_tmp];
      b_Acc_Pos[3 * i] = b_Acc_Pos[IMUgyro_tmp];
      Acc_Vel_tmp = 3 * i + 1;
      b_Acc_Vel[Acc_Vel_tmp] = b_Acc_Vel[IMUgyro_tmp + 1];
      Acc_Acc[Acc_Vel_tmp] = Acc_Acc[IMUgyro_tmp + 1];
      b_Acc_Pos[Acc_Vel_tmp] = b_Acc_Pos[IMUgyro_tmp + 1];
      Acc_Vel_tmp = 3 * i + 2;
      b_Acc_Vel[Acc_Vel_tmp] = b_Acc_Vel[IMUgyro_tmp + 2];
      Acc_Acc[Acc_Vel_tmp] = Acc_Acc[IMUgyro_tmp + 2];
      b_Acc_Pos[Acc_Vel_tmp] = b_Acc_Pos[IMUgyro_tmp + 2];
    }
    Acc_Acc[57] = 0.0;
    Acc_Acc[58] = 0.0;
    Acc_Acc[59] = 0.0;
    b_Acc_Vel[57] = cent_vel_est[0];
    b_Acc_Vel[58] = cent_vel_est[1];
    b_Acc_Vel[59] = cent_vel_est[2];
    b_Acc_Pos[57] = cent_pos_est[0];
    b_Acc_Pos[58] = cent_pos_est[1];
    b_Acc_Pos[59] = cent_pos_est[2];
    *acc_b_phi = std::atan(IMUacc[39] / IMUacc[59]);
    *acc_b_theta = std::atan(IMUacc[19] / std::sqrt(IMUacc[39] * IMUacc[39] +
                                                    IMUacc[59] * IMUacc[59]));
    gyro_phi = gyro_phi * (b_k - 1.0) / b_k + *acc_b_phi / b_k;
    gyro_theta = gyro_theta * (b_k - 1.0) / b_k + *acc_b_theta / b_k;
    *gyro_psi = *kf_psi;
    //              b_gyro = b_gyro*(k0(5)-1)/k0(5)+IMUgyro(end,:)/k0(5);
  }
  cent_pos_est[0] = b_Acc_Pos[57];
  cent_vel_est[0] = b_Acc_Vel[57];
  cent_pos_est[1] = b_Acc_Pos[58];
  cent_vel_est[1] = b_Acc_Vel[58];
  cent_pos_est[2] = b_Acc_Pos[59];
  cent_vel_est[2] = b_Acc_Vel[59];
  *state_o = state;
}

//
// mode : 2 -> F9R,  1 -> Xens
//
// Arguments    : void
// Return Type  : void
//
void IMUpos_init()
{
  b_k = 0.0;
  state = 0.0;
  //  0 : init,
  std::memset(&IMUacc[0], 0, 60U * sizeof(double));
  std::memset(&IMUgyro[0], 0, 60U * sizeof(double));
  std::memset(&Acc_Acc[0], 0, 60U * sizeof(double));
  std::memset(&b_Acc_Vel[0], 0, 60U * sizeof(double));
  std::memset(&b_Acc_Pos[0], 0, 60U * sizeof(double));
  // %%%% For Test %%%%%%
  //      Acc_Steady = zeros(3,2*Lv);
  //      Acc_PQR = zeros(3,2*Lv);
  //      Acc_Acc_a = zeros(3,2*Lv );
  //      kf_phi = 0;
  //      kf_theta = 0;
  // %%%%%%%%%%%%%%%%%%%%%%
  gyro_phi = 0.0;
  gyro_theta = 0.0;
  //      gyro_psi = 0;
}

//
// File trailer for IMUpos.cpp
//
// [EOF]
//
