/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: fusion.h
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 22-Sep-2023 09:43:03
 */

#ifndef FUSION_H
#define FUSION_H

/* Include Files */
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
extern creal_T
fusion(const creal_T *tag_center_vel_est, double state_IMU, double Nanchor,
       const double b_acc_o[3], double acc_b_theta, const double *acc_b_phi,
       double UWBErrSum, double init_flag, double kalman_on, double k0,
       double cent_pos_est[3], double cent_vel_est[3], double *kf_psi,
       const creal_T tag_pos_est[4], double heading_est, double zt_b);

void fusion_init(void);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for fusion.h
 *
 * [EOF]
 */
