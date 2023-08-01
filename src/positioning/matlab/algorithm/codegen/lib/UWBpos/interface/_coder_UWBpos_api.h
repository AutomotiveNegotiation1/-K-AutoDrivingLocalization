/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: _coder_UWBpos_api.h
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 01-Aug-2023 14:00:18
 */

#ifndef _CODER_UWBPOS_API_H
#define _CODER_UWBPOS_API_H

/* Include Files */
#include "emlrt.h"
#include "tmwtypes.h"
#include <string.h>

/* Type Definitions */
#ifndef typedef_emxArray_creal_T
#define typedef_emxArray_creal_T
typedef struct {
  creal_T *data;
  int32_T *size;
  int32_T allocatedSize;
  int32_T numDimensions;
  boolean_T canFreeData;
} emxArray_creal_T;
#endif /* typedef_emxArray_creal_T */

/* Variable Declarations */
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void UWBpos(real_T Ln, real_T Lp, real_T TagNum, real_T Nanchor,
            real_T RxID_data[], int32_T RxID_size[2], real_T RxDist_data[],
            int32_T RxDist_size[1], real_T s_time, creal_T tag_pos_b[4],
            real_T xa[6], real_T ya[6], emxArray_creal_T *tag_pos_est,
            real_T *heading_est, emxArray_creal_T *tag_pos_est_aver,
            real_T *headingest_a_aver_v);

void UWBpos_api(const mxArray *const prhs[10], int32_T nlhs,
                const mxArray *plhs[4]);

void UWBpos_atexit(void);

void UWBpos_initialize(void);

void UWBpos_terminate(void);

void UWBpos_xil_shutdown(void);

void UWBpos_xil_terminate(void);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for _coder_UWBpos_api.h
 *
 * [EOF]
 */
