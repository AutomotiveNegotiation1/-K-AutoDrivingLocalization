//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: fusion2.h
//
// MATLAB Coder version            : 5.6
// C/C++ source code generated on  : 09-Nov-2023 17:48:28
//

#ifndef FUSION2_H
#define FUSION2_H

// Include Files
#include "rtwtypes.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
extern creal_T fusion2(double kl, const double k0[5],
                       const creal_T tag_pos_est[4],
                       const creal_T tag_center_vel_est, double cent_pos_est[3],
                       double cent_vel_est[3], double b_acc_o[3],
                       double acc_b_phi, const double b_Acc_Pos[60],
                       const double b_Acc_Vel[60], double kalman_on,
                       double init_flag, double UWBErrSum, double *kf_psi,
                       double *gyro_psi, double heading_est, double acc_b_theta,
                       double IMUSel, double Nanchor, double state_IMU);

void fusion2_init();

#endif
//
// File trailer for fusion2.h
//
// [EOF]
//
