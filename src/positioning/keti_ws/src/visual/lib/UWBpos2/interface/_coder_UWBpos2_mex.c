/*
 * _coder_UWBpos2_mex.c
 *
 * Code generation for function 'UWBpos2'
 *
 */

/* Include files */
#include "_coder_UWBpos2_mex.h"
#include "_coder_UWBpos2_api.h"

/* Function Definitions */
void mexFunction(int32_T nlhs, mxArray *plhs[], int32_T nrhs,
                 const mxArray *prhs[])
{
  mexAtExit(&UWBpos2_atexit);
  /* Module initialization. */
  UWBpos2_initialize();
  /* Dispatch the entry-point. */
  unsafe_UWBpos2_mexFunction(nlhs, plhs, nrhs, prhs);
  /* Module termination. */
  UWBpos2_terminate();
}

emlrtCTX mexFunctionCreateRootTLS(void)
{
  emlrtCreateRootTLSR2022a(&emlrtRootTLSGlobal, &emlrtContextGlobal, NULL, 1,
                           NULL, "windows-949", true);
  return emlrtRootTLSGlobal;
}

void unsafe_UWBpos2_mexFunction(int32_T nlhs, mxArray *plhs[1], int32_T nrhs,
                                const mxArray *prhs[11])
{
  emlrtStack st = {
      NULL, /* site */
      NULL, /* tls */
      NULL  /* prev */
  };
  const mxArray *outputs;
  st.tls = emlrtRootTLSGlobal;
  /* Check for proper number of arguments. */
  if (nrhs != 11) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:WrongNumberOfInputs", 5, 12, 11, 4,
                        7, "UWBpos2");
  }
  if (nlhs > 1) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:TooManyOutputArguments", 3, 4, 7,
                        "UWBpos2");
  }
  /* Call the function. */
  UWBpos2_api(prhs, &outputs);
  /* Copy over outputs to the caller. */
  emlrtReturnArrays(1, &plhs[0], &outputs);
}

/* End of code generation (_coder_UWBpos2_mex.c) */
