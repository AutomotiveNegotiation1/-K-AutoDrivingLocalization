/*
 * _coder_UWBpos_mex.c
 *
 * Code generation for function 'UWBpos'
 *
 */

/* Include files */
#include "_coder_UWBpos_mex.h"
#include "_coder_UWBpos_api.h"

/* Function Definitions */
void mexFunction(int32_T nlhs, mxArray *plhs[], int32_T nrhs,
                 const mxArray *prhs[])
{
  mexAtExit(&UWBpos_atexit);
  /* Module initialization. */
  UWBpos_initialize();
  /* Dispatch the entry-point. */
  unsafe_UWBpos_mexFunction(nlhs, plhs, nrhs, prhs);
  /* Module termination. */
  UWBpos_terminate();
}

emlrtCTX mexFunctionCreateRootTLS(void)
{
  emlrtCreateRootTLSR2022a(&emlrtRootTLSGlobal, &emlrtContextGlobal, NULL, 1,
                           NULL, "windows-949", true);
  return emlrtRootTLSGlobal;
}

void unsafe_UWBpos_mexFunction(int32_T nlhs, mxArray *plhs[4], int32_T nrhs,
                               const mxArray *prhs[10])
{
  emlrtStack st = {
      NULL, /* site */
      NULL, /* tls */
      NULL  /* prev */
  };
  const mxArray *outputs[4];
  int32_T i;
  st.tls = emlrtRootTLSGlobal;
  /* Check for proper number of arguments. */
  if (nrhs != 10) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:WrongNumberOfInputs", 5, 12, 10, 4,
                        6, "UWBpos");
  }
  if (nlhs > 4) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:TooManyOutputArguments", 3, 4, 6,
                        "UWBpos");
  }
  /* Call the function. */
  UWBpos_api(prhs, nlhs, outputs);
  /* Copy over outputs to the caller. */
  if (nlhs < 1) {
    i = 1;
  } else {
    i = nlhs;
  }
  emlrtReturnArrays(i, &plhs[0], &outputs[0]);
}

/* End of code generation (_coder_UWBpos_mex.c) */
