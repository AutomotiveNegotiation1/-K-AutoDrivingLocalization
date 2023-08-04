/*
 * _coder_TestTotal_mex.c
 *
 * Code generation for function 'TestTotal'
 *
 */

/* Include files */
#include "_coder_TestTotal_mex.h"
#include "_coder_TestTotal_api.h"

/* Function Definitions */
void mexFunction(int32_T nlhs, mxArray *plhs[], int32_T nrhs,
                 const mxArray *prhs[])
{
  (void)prhs;
  mexAtExit(&TestTotal_atexit);
  /* Module initialization. */
  TestTotal_initialize();
  /* Dispatch the entry-point. */
  unsafe_TestTotal_mexFunction(nlhs, plhs, nrhs);
  /* Module termination. */
  TestTotal_terminate();
}

emlrtCTX mexFunctionCreateRootTLS(void)
{
  emlrtCreateRootTLSR2022a(&emlrtRootTLSGlobal, &emlrtContextGlobal, NULL, 1,
                           NULL, "windows-949", true);
  return emlrtRootTLSGlobal;
}

void unsafe_TestTotal_mexFunction(int32_T nlhs, mxArray *plhs[4], int32_T nrhs)
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
  if (nrhs != 0) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:WrongNumberOfInputs", 5, 12, 0, 4,
                        9, "TestTotal");
  }
  if (nlhs > 4) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:TooManyOutputArguments", 3, 4, 9,
                        "TestTotal");
  }
  /* Call the function. */
  TestTotal_api(nlhs, outputs);
  /* Copy over outputs to the caller. */
  if (nlhs < 1) {
    i = 1;
  } else {
    i = nlhs;
  }
  emlrtReturnArrays(i, &plhs[0], &outputs[0]);
}

/* End of code generation (_coder_TestTotal_mex.c) */
