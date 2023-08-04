/*
 * _coder_UWBpos_mex.h
 *
 * Code generation for function 'UWBpos'
 *
 */

#ifndef _CODER_UWBPOS_MEX_H
#define _CODER_UWBPOS_MEX_H

/* Include files */
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

void unsafe_UWBpos_mexFunction(int32_T nlhs, mxArray *plhs[4], int32_T nrhs,
                               const mxArray *prhs[10]);

#ifdef __cplusplus
}
#endif

#endif
/* End of code generation (_coder_UWBpos_mex.h) */
