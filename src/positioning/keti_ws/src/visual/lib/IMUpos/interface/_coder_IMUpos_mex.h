/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: _coder_IMUpos_mex.h
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 04-Oct-2023 14:13:20
 */

#ifndef _CODER_IMUPOS_MEX_H
#define _CODER_IMUPOS_MEX_H

/* Include Files */
#include "emlrt.h"
#include "mex.h"
#include "tmwtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
MEXFUNCTION_LINKAGE void mexFunction(int32_T nlhs, mxArray *plhs[],
                                     int32_T nrhs, const mxArray *prhs[]);

emlrtCTX mexFunctionCreateRootTLS(void);

void unsafe_IMUpos_mexFunction(int32_T nlhs, mxArray *plhs[9], int32_T nrhs,
                               const mxArray *prhs[10]);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for _coder_IMUpos_mex.h
 *
 * [EOF]
 */
