/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: _coder_fusion_api.h
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 22-Sep-2023 10:33:23
 */

#ifndef _CODER_FUSION_API_H
#define _CODER_FUSION_API_H

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
void PosKalman2(real_T x[12], real_T A[144], real_T z[3], real_T R[9]);

void PosKalman2_api(const mxArray *const prhs[4], const mxArray **plhs);

real_T ReliCalc(creal_T tag_pos_est[4], creal_T anchor_pos[6],
                real_T RxDist_data[], int32_T RxDist_size[1],
                real_T RxID_data[], int32_T RxID_size[2], real_T PP);

void ReliCalc_api(const mxArray *const prhs[5], const mxArray **plhs);

creal_T fusion(creal_T *tag_center_vel_est, real_T state_IMU, real_T Nanchor,
               real_T b_acc_o[3], real_T acc_b_theta, real_T *acc_b_phi,
               real_T UWBErrSum, real_T init_flag, real_T kalman_on,
               real_T k0[5], real_T cent_pos_est[3], real_T cent_vel_est[3],
               real_T *kf_psi, creal_T tag_pos_est[4], real_T heading_est,
               real_T zt_b);

void fusion_api(const mxArray *const prhs[16], int32_T nlhs,
                const mxArray *plhs[6]);

void fusion_atexit(void);

void fusion_initialize(void);

void fusion_terminate(void);

void fusion_xil_shutdown(void);

void fusion_xil_terminate(void);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for _coder_fusion_api.h
 *
 * [EOF]
 */
