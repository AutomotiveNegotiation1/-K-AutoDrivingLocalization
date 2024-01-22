//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: _coder_fusion2_api.h
//
// MATLAB Coder version            : 5.6
// C/C++ source code generated on  : 09-Nov-2023 17:48:28
//

#ifndef _CODER_FUSION2_API_H
#define _CODER_FUSION2_API_H

// Include Files
#include "emlrt.h"
#include "tmwtypes.h"
#include <algorithm>
#include <cstring>

// Variable Declarations
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

// Function Declarations
void IMUpos(real_T IMUacc_c[3], real_T IMUgyro_c[3], real_T s_time,
            real_T b_acc_o[3], real_T b_gyro[3], real_T mode, real_T *kf_psi,
            real_T *gyro_psi, real_T cent_pos_est[3], real_T cent_vel_est[3],
            real_T *state_o, real_T *acc_b_phi, real_T *acc_b_theta);

void IMUpos_api(const mxArray *const prhs[10], int32_T nlhs,
                const mxArray *plhs[9]);

void UWBpos6(real_T Ln, real_T Lp, real_T LnC, real_T TagNum, real_T Nanchor,
             real_T RxIDin[6], real_T RxDistin[6], real_T s_time,
             creal_T tag_pos_b[4], real_T xain[6], real_T yain[6],
             creal_T prevTagPos[4], real_T prevTagHeading, real_T UWBout[21]);

void UWBpos6_api(const mxArray *const prhs[13], const mxArray **plhs);

creal_T fusion2(real_T kl, real_T k0[5], creal_T tag_pos_est[4],
                creal_T tag_center_vel_est, real_T cent_pos_est[3],
                real_T cent_vel_est[3], real_T b_acc_o[3], real_T acc_b_phi,
                real_T Acc_Pos[60], real_T Acc_Vel[60], real_T kalman_on,
                real_T init_flag, real_T UWBErrSum, real_T *kf_psi,
                real_T *gyro_psi, real_T heading_est, real_T acc_b_theta,
                real_T IMUSel, real_T Nanchor, real_T state_IMU);

void fusion2_api(const mxArray *const prhs[20], int32_T nlhs,
                 const mxArray *plhs[6]);

void fusion2_atexit();

void fusion2_initialize();

void fusion2_terminate();

void fusion2_xil_shutdown();

void fusion2_xil_terminate();

#endif
//
// File trailer for _coder_fusion2_api.h
//
// [EOF]
//
