//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: _coder_fusion2_mex.cpp
//
// MATLAB Coder version            : 5.6
// C/C++ source code generated on  : 09-Nov-2023 17:48:28
//

// Include Files
#include "_coder_fusion2_mex.h"
#include "_coder_fusion2_api.h"

// Function Definitions
//
// Arguments    : int32_T nlhs
//                mxArray *plhs[]
//                int32_T nrhs
//                const mxArray *prhs[]
// Return Type  : void
//
void mexFunction(int32_T nlhs, mxArray *plhs[], int32_T nrhs,
                 const mxArray *prhs[])
{
  static const char_T *emlrtEntryPoints[3]{"fusion2", "IMUpos", "UWBpos6"};
  emlrtStack st{
      nullptr, // site
      nullptr, // tls
      nullptr  // prev
  };
  mexAtExit(&fusion2_atexit);
  // Module initialization.
  fusion2_initialize();
  st.tls = emlrtRootTLSGlobal;
  // Dispatch the entry-point.
  switch (emlrtGetEntryPointIndexR2016a(
      &st, nrhs, &prhs[0], (const char_T **)&emlrtEntryPoints[0], 3)) {
  case 0:
    unsafe_fusion2_mexFunction(nlhs, plhs, nrhs - 1, &prhs[1]);
    break;
  case 1:
    unsafe_IMUpos_mexFunction(nlhs, plhs, nrhs - 1, &prhs[1]);
    break;
  case 2:
    unsafe_UWBpos6_mexFunction(nlhs, plhs, nrhs - 1, &prhs[1]);
    break;
  }
  // Module termination.
  fusion2_terminate();
}

//
// Arguments    : void
// Return Type  : emlrtCTX
//
emlrtCTX mexFunctionCreateRootTLS()
{
  emlrtCreateRootTLSR2022a(&emlrtRootTLSGlobal, &emlrtContextGlobal, nullptr, 1,
                           nullptr, "windows-949", true);
  return emlrtRootTLSGlobal;
}

//
// Arguments    : int32_T nlhs
//                mxArray *plhs[9]
//                int32_T nrhs
//                const mxArray *prhs[10]
// Return Type  : void
//
void unsafe_IMUpos_mexFunction(int32_T nlhs, mxArray *plhs[9], int32_T nrhs,
                               const mxArray *prhs[10])
{
  emlrtStack st{
      nullptr, // site
      nullptr, // tls
      nullptr  // prev
  };
  const mxArray *b_prhs[10];
  const mxArray *outputs[9];
  int32_T i1;
  st.tls = emlrtRootTLSGlobal;
  // Check for proper number of arguments.
  if (nrhs != 10) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:WrongNumberOfInputs", 5, 12, 10, 4,
                        6, "IMUpos");
  }
  if (nlhs > 9) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:TooManyOutputArguments", 3, 4, 6,
                        "IMUpos");
  }
  // Call the function.
  for (int32_T i{0}; i < 10; i++) {
    b_prhs[i] = prhs[i];
  }
  IMUpos_api(b_prhs, nlhs, outputs);
  // Copy over outputs to the caller.
  if (nlhs < 1) {
    i1 = 1;
  } else {
    i1 = nlhs;
  }
  emlrtReturnArrays(i1, &plhs[0], &outputs[0]);
}

//
// Arguments    : int32_T nlhs
//                mxArray *plhs[1]
//                int32_T nrhs
//                const mxArray *prhs[13]
// Return Type  : void
//
void unsafe_UWBpos6_mexFunction(int32_T nlhs, mxArray *plhs[1], int32_T nrhs,
                                const mxArray *prhs[13])
{
  emlrtStack st{
      nullptr, // site
      nullptr, // tls
      nullptr  // prev
  };
  const mxArray *b_prhs[13];
  const mxArray *outputs;
  st.tls = emlrtRootTLSGlobal;
  // Check for proper number of arguments.
  if (nrhs != 13) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:WrongNumberOfInputs", 5, 12, 13, 4,
                        7, "UWBpos6");
  }
  if (nlhs > 1) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:TooManyOutputArguments", 3, 4, 7,
                        "UWBpos6");
  }
  // Call the function.
  for (int32_T i{0}; i < 13; i++) {
    b_prhs[i] = prhs[i];
  }
  UWBpos6_api(b_prhs, &outputs);
  // Copy over outputs to the caller.
  emlrtReturnArrays(1, &plhs[0], &outputs);
}

//
// Arguments    : int32_T nlhs
//                mxArray *plhs[6]
//                int32_T nrhs
//                const mxArray *prhs[20]
// Return Type  : void
//
void unsafe_fusion2_mexFunction(int32_T nlhs, mxArray *plhs[6], int32_T nrhs,
                                const mxArray *prhs[20])
{
  emlrtStack st{
      nullptr, // site
      nullptr, // tls
      nullptr  // prev
  };
  const mxArray *b_prhs[20];
  const mxArray *outputs[6];
  int32_T i1;
  st.tls = emlrtRootTLSGlobal;
  // Check for proper number of arguments.
  if (nrhs != 20) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:WrongNumberOfInputs", 5, 12, 20, 4,
                        7, "fusion2");
  }
  if (nlhs > 6) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:TooManyOutputArguments", 3, 4, 7,
                        "fusion2");
  }
  // Call the function.
  for (int32_T i{0}; i < 20; i++) {
    b_prhs[i] = prhs[i];
  }
  fusion2_api(b_prhs, nlhs, outputs);
  // Copy over outputs to the caller.
  if (nlhs < 1) {
    i1 = 1;
  } else {
    i1 = nlhs;
  }
  emlrtReturnArrays(i1, &plhs[0], &outputs[0]);
}

//
// File trailer for _coder_fusion2_mex.cpp
//
// [EOF]
//
