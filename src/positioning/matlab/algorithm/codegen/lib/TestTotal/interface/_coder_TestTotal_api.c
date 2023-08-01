/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: _coder_TestTotal_api.c
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 01-Aug-2023 15:24:45
 */

/* Include Files */
#include "_coder_TestTotal_api.h"
#include "_coder_TestTotal_mex.h"

/* Variable Definitions */
emlrtCTX emlrtRootTLSGlobal = NULL;

emlrtContext emlrtContextGlobal = {
    true,                                                 /* bFirstTime */
    false,                                                /* bInitialized */
    131642U,                                              /* fVersionInfo */
    NULL,                                                 /* fErrorFunction */
    "TestTotal",                                          /* fFunctionName */
    NULL,                                                 /* fRTCallStack */
    false,                                                /* bDebugMode */
    {2045744189U, 2170104910U, 2743257031U, 4284093946U}, /* fSigWrd */
    NULL                                                  /* fSigMem */
};

/* Function Declarations */
static const mxArray *b_emlrt_marshallOut(const real_T u);

static const mxArray *emlrt_marshallOut(const emlrtStack *sp,
                                        const creal_T u[4]);

/* Function Definitions */
/*
 * Arguments    : const real_T u
 * Return Type  : const mxArray *
 */
static const mxArray *b_emlrt_marshallOut(const real_T u)
{
  const mxArray *m;
  const mxArray *y;
  y = NULL;
  m = emlrtCreateDoubleScalar(u);
  emlrtAssign(&y, m);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const creal_T u[4]
 * Return Type  : const mxArray *
 */
static const mxArray *emlrt_marshallOut(const emlrtStack *sp,
                                        const creal_T u[4])
{
  static const int32_T iv[2] = {1, 4};
  const mxArray *m;
  const mxArray *y;
  y = NULL;
  m = emlrtCreateNumericArray(2, (const void *)&iv[0], mxDOUBLE_CLASS,
                              mxCOMPLEX);
  emlrtExportNumericArrayR2013b((emlrtConstCTX)sp, m, (const void *)&u[0], 8);
  emlrtAssign(&y, m);
  return y;
}

/*
 * Arguments    : int32_T nlhs
 *                const mxArray *plhs[4]
 * Return Type  : void
 */
void TestTotal_api(int32_T nlhs, const mxArray *plhs[4])
{
  emlrtStack st = {
      NULL, /* site */
      NULL, /* tls */
      NULL  /* prev */
  };
  creal_T tag_pos_est[4];
  creal_T tag_pos_est_aver[4];
  real_T heading_est;
  real_T headingest_a_aver_v;
  st.tls = emlrtRootTLSGlobal;
  /* Invoke the target function */
  TestTotal(tag_pos_est, &heading_est, tag_pos_est_aver, &headingest_a_aver_v);
  /* Marshall function outputs */
  plhs[0] = emlrt_marshallOut(&st, tag_pos_est);
  if (nlhs > 1) {
    plhs[1] = b_emlrt_marshallOut(heading_est);
  }
  if (nlhs > 2) {
    plhs[2] = emlrt_marshallOut(&st, tag_pos_est_aver);
  }
  if (nlhs > 3) {
    plhs[3] = b_emlrt_marshallOut(headingest_a_aver_v);
  }
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void TestTotal_atexit(void)
{
  emlrtStack st = {
      NULL, /* site */
      NULL, /* tls */
      NULL  /* prev */
  };
  mexFunctionCreateRootTLS();
  st.tls = emlrtRootTLSGlobal;
  emlrtEnterRtStackR2012b(&st);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
  TestTotal_xil_terminate();
  TestTotal_xil_shutdown();
  emlrtExitTimeCleanup(&emlrtContextGlobal);
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void TestTotal_initialize(void)
{
  emlrtStack st = {
      NULL, /* site */
      NULL, /* tls */
      NULL  /* prev */
  };
  mexFunctionCreateRootTLS();
  st.tls = emlrtRootTLSGlobal;
  emlrtClearAllocCountR2012b(&st, false, 0U, NULL);
  emlrtEnterRtStackR2012b(&st);
  emlrtFirstTimeR2012b(emlrtRootTLSGlobal);
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void TestTotal_terminate(void)
{
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
}

/*
 * File trailer for _coder_TestTotal_api.c
 *
 * [EOF]
 */
