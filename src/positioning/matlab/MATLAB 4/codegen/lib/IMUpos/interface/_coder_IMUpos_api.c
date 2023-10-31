/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: _coder_IMUpos_api.c
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 20-Sep-2023 17:05:14
 */

/* Include Files */
#include "_coder_IMUpos_api.h"
#include "_coder_IMUpos_mex.h"

/* Variable Definitions */
emlrtCTX emlrtRootTLSGlobal = NULL;

emlrtContext emlrtContextGlobal = {
    true,                                                 /* bFirstTime */
    false,                                                /* bInitialized */
    131642U,                                              /* fVersionInfo */
    NULL,                                                 /* fErrorFunction */
    "IMUpos",                                             /* fFunctionName */
    NULL,                                                 /* fRTCallStack */
    false,                                                /* bDebugMode */
    {2045744189U, 2170104910U, 2743257031U, 4284093946U}, /* fSigWrd */
    NULL                                                  /* fSigMem */
};

/* Function Declarations */
static real_T (*b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                   const emlrtMsgIdentifier *parentId))[3];

static const mxArray *b_emlrt_marshallOut(const real_T u);

static real_T c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *s_time,
                                 const char_T *identifier);

static void c_emlrt_marshallOut(const real_T u[3], const mxArray *y);

static real_T d_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                 const emlrtMsgIdentifier *parentId);

static real_T (*e_emlrt_marshallIn(const emlrtStack *sp,
                                   const mxArray *cent_pos_est,
                                   const char_T *identifier))[3];

static real_T (*emlrt_marshallIn(const emlrtStack *sp, const mxArray *IMUacc_c,
                                 const char_T *identifier))[3];

static void emlrt_marshallOut(const real_T u[3], const mxArray *y);

static real_T (*f_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                   const emlrtMsgIdentifier *parentId))[3];

static real_T (*g_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                   const emlrtMsgIdentifier *msgId))[3];

static real_T h_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                 const emlrtMsgIdentifier *msgId);

static real_T (*i_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                   const emlrtMsgIdentifier *msgId))[3];

/* Function Definitions */
/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 * Return Type  : real_T (*)[3]
 */
static real_T (*b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                   const emlrtMsgIdentifier *parentId))[3]
{
  real_T(*y)[3];
  y = g_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

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
 *                const mxArray *s_time
 *                const char_T *identifier
 * Return Type  : real_T
 */
static real_T c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *s_time,
                                 const char_T *identifier)
{
  emlrtMsgIdentifier thisId;
  real_T y;
  thisId.fIdentifier = (const char_T *)identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  y = d_emlrt_marshallIn(sp, emlrtAlias(s_time), &thisId);
  emlrtDestroyArray(&s_time);
  return y;
}

/*
 * Arguments    : const real_T u[3]
 *                const mxArray *y
 * Return Type  : void
 */
static void c_emlrt_marshallOut(const real_T u[3], const mxArray *y)
{
  static const int32_T iv[2] = {1, 3};
  emlrtMxSetData((mxArray *)y, (void *)&u[0]);
  emlrtSetDimensions((mxArray *)y, &iv[0], 2);
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 * Return Type  : real_T
 */
static real_T d_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                 const emlrtMsgIdentifier *parentId)
{
  real_T y;
  y = h_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *cent_pos_est
 *                const char_T *identifier
 * Return Type  : real_T (*)[3]
 */
static real_T (*e_emlrt_marshallIn(const emlrtStack *sp,
                                   const mxArray *cent_pos_est,
                                   const char_T *identifier))[3]
{
  emlrtMsgIdentifier thisId;
  real_T(*y)[3];
  thisId.fIdentifier = (const char_T *)identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  y = f_emlrt_marshallIn(sp, emlrtAlias(cent_pos_est), &thisId);
  emlrtDestroyArray(&cent_pos_est);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *IMUacc_c
 *                const char_T *identifier
 * Return Type  : real_T (*)[3]
 */
static real_T (*emlrt_marshallIn(const emlrtStack *sp, const mxArray *IMUacc_c,
                                 const char_T *identifier))[3]
{
  emlrtMsgIdentifier thisId;
  real_T(*y)[3];
  thisId.fIdentifier = (const char_T *)identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  y = b_emlrt_marshallIn(sp, emlrtAlias(IMUacc_c), &thisId);
  emlrtDestroyArray(&IMUacc_c);
  return y;
}

/*
 * Arguments    : const real_T u[3]
 *                const mxArray *y
 * Return Type  : void
 */
static void emlrt_marshallOut(const real_T u[3], const mxArray *y)
{
  static const int32_T i = 3;
  emlrtMxSetData((mxArray *)y, (void *)&u[0]);
  emlrtSetDimensions((mxArray *)y, &i, 1);
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 * Return Type  : real_T (*)[3]
 */
static real_T (*f_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                   const emlrtMsgIdentifier *parentId))[3]
{
  real_T(*y)[3];
  y = i_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 * Return Type  : real_T (*)[3]
 */
static real_T (*g_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                   const emlrtMsgIdentifier *msgId))[3]
{
  static const int32_T dims[2] = {1, 3};
  real_T(*ret)[3];
  int32_T iv[2];
  boolean_T bv[2] = {false, false};
  emlrtCheckVsBuiltInR2012b((emlrtConstCTX)sp, msgId, src, "double", false, 2U,
                            (const void *)&dims[0], &bv[0], &iv[0]);
  ret = (real_T(*)[3])emlrtMxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 * Return Type  : real_T
 */
static real_T h_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                 const emlrtMsgIdentifier *msgId)
{
  static const int32_T dims = 0;
  real_T ret;
  emlrtCheckBuiltInR2012b((emlrtConstCTX)sp, msgId, src, "double", false, 0U,
                          (const void *)&dims);
  ret = *(real_T *)emlrtMxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 * Return Type  : real_T (*)[3]
 */
static real_T (*i_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                   const emlrtMsgIdentifier *msgId))[3]
{
  static const int32_T dims = 3;
  real_T(*ret)[3];
  int32_T i;
  boolean_T b = false;
  emlrtCheckVsBuiltInR2012b((emlrtConstCTX)sp, msgId, src, "double", false, 1U,
                            (const void *)&dims, &b, &i);
  ret = (real_T(*)[3])emlrtMxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}

/*
 * Arguments    : const mxArray * const prhs[10]
 *                int32_T nlhs
 *                const mxArray *plhs[9]
 * Return Type  : void
 */
void IMUpos_api(const mxArray *const prhs[10], int32_T nlhs,
                const mxArray *plhs[9])
{
  emlrtStack st = {
      NULL, /* site */
      NULL, /* tls */
      NULL  /* prev */
  };
  const mxArray *prhs_copy_idx_3;
  const mxArray *prhs_copy_idx_4;
  const mxArray *prhs_copy_idx_8;
  const mxArray *prhs_copy_idx_9;
  real_T(*IMUacc_c)[3];
  real_T(*IMUgyro_c)[3];
  real_T(*b_acc_o)[3];
  real_T(*b_gyro)[3];
  real_T(*cent_pos_est)[3];
  real_T(*cent_vel_est)[3];
  real_T acc_b_phi;
  real_T acc_b_theta;
  real_T gyro_psi;
  real_T kf_psi;
  real_T mode;
  real_T s_time;
  real_T state_o;
  st.tls = emlrtRootTLSGlobal;
  prhs_copy_idx_3 = emlrtProtectR2012b(prhs[3], 3, true, -1);
  prhs_copy_idx_4 = emlrtProtectR2012b(prhs[4], 4, true, -1);
  prhs_copy_idx_8 = emlrtProtectR2012b(prhs[8], 8, true, -1);
  prhs_copy_idx_9 = emlrtProtectR2012b(prhs[9], 9, true, -1);
  /* Marshall function inputs */
  IMUacc_c = emlrt_marshallIn(&st, emlrtAlias(prhs[0]), "IMUacc_c");
  IMUgyro_c = emlrt_marshallIn(&st, emlrtAlias(prhs[1]), "IMUgyro_c");
  s_time = c_emlrt_marshallIn(&st, emlrtAliasP(prhs[2]), "s_time");
  b_acc_o = emlrt_marshallIn(&st, emlrtAlias(prhs_copy_idx_3), "b_acc_o");
  b_gyro = emlrt_marshallIn(&st, emlrtAlias(prhs_copy_idx_4), "b_gyro");
  mode = c_emlrt_marshallIn(&st, emlrtAliasP(prhs[5]), "mode");
  kf_psi = c_emlrt_marshallIn(&st, emlrtAliasP(prhs[6]), "kf_psi");
  gyro_psi = c_emlrt_marshallIn(&st, emlrtAliasP(prhs[7]), "gyro_psi");
  cent_pos_est =
      e_emlrt_marshallIn(&st, emlrtAlias(prhs_copy_idx_8), "cent_pos_est");
  cent_vel_est =
      e_emlrt_marshallIn(&st, emlrtAlias(prhs_copy_idx_9), "cent_vel_est");
  /* Invoke the target function */
  IMUpos(*IMUacc_c, *IMUgyro_c, s_time, *b_acc_o, *b_gyro, mode, &kf_psi,
         &gyro_psi, *cent_pos_est, *cent_vel_est, &state_o, &acc_b_phi,
         &acc_b_theta);
  /* Marshall function outputs */
  emlrt_marshallOut(*cent_pos_est, prhs_copy_idx_8);
  plhs[0] = prhs_copy_idx_8;
  if (nlhs > 1) {
    emlrt_marshallOut(*cent_vel_est, prhs_copy_idx_9);
    plhs[1] = prhs_copy_idx_9;
  }
  if (nlhs > 2) {
    plhs[2] = b_emlrt_marshallOut(kf_psi);
  }
  if (nlhs > 3) {
    plhs[3] = b_emlrt_marshallOut(gyro_psi);
  }
  if (nlhs > 4) {
    c_emlrt_marshallOut(*b_acc_o, prhs_copy_idx_3);
    plhs[4] = prhs_copy_idx_3;
  }
  if (nlhs > 5) {
    c_emlrt_marshallOut(*b_gyro, prhs_copy_idx_4);
    plhs[5] = prhs_copy_idx_4;
  }
  if (nlhs > 6) {
    plhs[6] = b_emlrt_marshallOut(state_o);
  }
  if (nlhs > 7) {
    plhs[7] = b_emlrt_marshallOut(acc_b_phi);
  }
  if (nlhs > 8) {
    plhs[8] = b_emlrt_marshallOut(acc_b_theta);
  }
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void IMUpos_atexit(void)
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
  IMUpos_xil_terminate();
  IMUpos_xil_shutdown();
  emlrtExitTimeCleanup(&emlrtContextGlobal);
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void IMUpos_initialize(void)
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
void IMUpos_terminate(void)
{
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
}

/*
 * File trailer for _coder_IMUpos_api.c
 *
 * [EOF]
 */
