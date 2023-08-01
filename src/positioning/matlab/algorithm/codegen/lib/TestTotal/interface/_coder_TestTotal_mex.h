/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: _coder_TestTotal_mex.h
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 01-Aug-2023 16:36:28
 */

#ifndef _CODER_TESTTOTAL_MEX_H
#define _CODER_TESTTOTAL_MEX_H

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

void unsafe_TestTotal_mexFunction(int32_T nlhs, mxArray *plhs[4], int32_T nrhs);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for _coder_TestTotal_mex.h
 *
 * [EOF]
 */
