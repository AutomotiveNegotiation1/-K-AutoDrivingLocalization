/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: IMUpos.h
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 20-Sep-2023 17:05:14
 */

#ifndef IMUPOS_H
#define IMUPOS_H

/* Include Files */
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
extern void IMUpos(const double IMUacc_c[3], const double IMUgyro_c[3],
                   double s_time, double b_acc_o[3], double b_gyro[3],
                   double mode, double *kf_psi, double *gyro_psi,
                   double cent_pos_est[3], double cent_vel_est[3],
                   double *state_o, double *acc_b_phi, double *acc_b_theta);

void IMUpos_init(void);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for IMUpos.h
 *
 * [EOF]
 */
