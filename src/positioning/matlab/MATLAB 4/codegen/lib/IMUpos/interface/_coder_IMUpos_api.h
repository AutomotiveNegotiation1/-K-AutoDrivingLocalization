/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: _coder_IMUpos_api.h
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 20-Sep-2023 17:05:14
 */

#ifndef _CODER_IMUPOS_API_H
#define _CODER_IMUPOS_API_H

/* Include Files */
#include "emlrt.h"
#include "tmwtypes.h"
#include <string.h>

/* Variable Declarations */
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void IMUpos(real_T IMUacc_c[3], real_T IMUgyro_c[3], real_T s_time,
            real_T b_acc_o[3], real_T b_gyro[3], real_T mode, real_T *kf_psi,
            real_T *gyro_psi, real_T cent_pos_est[3], real_T cent_vel_est[3],
            real_T *state_o, real_T *acc_b_phi, real_T *acc_b_theta);

void IMUpos_api(const mxArray *const prhs[10], int32_T nlhs,
                const mxArray *plhs[9]);

void IMUpos_atexit(void);

void IMUpos_initialize(void);

void IMUpos_terminate(void);

void IMUpos_xil_shutdown(void);

void IMUpos_xil_terminate(void);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for _coder_IMUpos_api.h
 *
 * [EOF]
 */
