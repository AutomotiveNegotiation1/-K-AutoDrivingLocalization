//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: _coder_fusion2_mex.h
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 30-Nov-2023 16:43:53
//

#ifndef _CODER_FUSION2_MEX_H
#define _CODER_FUSION2_MEX_H

// Include Files
#include "emlrt.h"
#include "mex.h"
#include "tmwtypes.h"

// Function Declarations
MEXFUNCTION_LINKAGE void mexFunction(int32_T nlhs, mxArray *plhs[],
                                     int32_T nrhs, const mxArray *prhs[]);

emlrtCTX mexFunctionCreateRootTLS();

void unsafe_EulerKalman_2_mexFunction(int32_T nlhs, mxArray *plhs[3],
                                      int32_T nrhs, const mxArray *prhs[3]);

void unsafe_IMUpos_mexFunction(int32_T nlhs, mxArray *plhs[9], int32_T nrhs,
                               const mxArray *prhs[10]);

void unsafe_PosKalman2_mexFunction(int32_T nlhs, mxArray *plhs[1], int32_T nrhs,
                                   const mxArray *prhs[4]);

void unsafe_UWBpos6_mexFunction(int32_T nlhs, mxArray *plhs[1], int32_T nrhs,
                                const mxArray *prhs[13]);

void unsafe_fusion2_mexFunction(int32_T nlhs, mxArray *plhs[6], int32_T nrhs,
                                const mxArray *prhs[20]);

#endif
//
// File trailer for _coder_fusion2_mex.h
//
// [EOF]
//
