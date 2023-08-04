/*
 * _coder_TestTotal_api.c
 *
 * Code generation for function 'TestTotal'
 *
 */

/* Include files */
#include "_coder_TestTotal_api.h"
#include "_coder_TestTotal_mex.h"

/* Variable Definitions */
emlrtCTX emlrtRootTLSGlobal = NULL;

emlrtContext emlrtContextGlobal = {
    true,                                                 /* bFirstTime */
    false,                                                /* bInitialized */
    131627U,                                              /* fVersionInfo */
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
static const mxArray *b_emlrt_marshallOut(const real_T u)
{
  const mxArray *m;
  const mxArray *y;
  y = NULL;
  m = emlrtCreateDoubleScalar(u);
  emlrtAssign(&y, m);
  return y;
}

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

void TestTotal_terminate(void)
{
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
}

/* End of code generation (_coder_TestTotal_api.c) */
