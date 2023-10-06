/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: _coder_UWBpos6_api.h
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 05-Sep-2023 20:11:41
 */

#ifndef _CODER_UWBPOS6_API_H
#define _CODER_UWBPOS6_API_H

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
void UWBpos6(real_T Ln, real_T Lp, real_T LnC, real_T TagNum, real_T Nanchor,
             real_T RxIDin[6], real_T RxDistin[6], real_T s_time,
             creal_T tag_pos_b[4], real_T xain[6], real_T yain[6],
             creal_T prevTagPos[4], real_T prevTagHeading, real_T UWBout[21]);

void UWBpos6_api(const mxArray *const prhs[13], const mxArray **plhs);

void UWBpos6_atexit(void);

void UWBpos6_initialize(void);

void UWBpos6_terminate(void);

void UWBpos6_xil_shutdown(void);

void UWBpos6_xil_terminate(void);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for _coder_UWBpos6_api.h
 *
 * [EOF]
 */
