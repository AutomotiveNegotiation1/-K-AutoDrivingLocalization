/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: _coder_EulerKalman_2_api.c
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 22-Sep-2023 10:38:00
 */

/* Include Files */
#include "_coder_EulerKalman_2_api.h"
#include "_coder_EulerKalman_2_mex.h"

/* Variable Definitions */
emlrtCTX emlrtRootTLSGlobal = NULL;

emlrtContext emlrtContextGlobal = {
    true,                                                 /* bFirstTime */
    false,                                                /* bInitialized */
    131642U,                                              /* fVersionInfo */
    NULL,                                                 /* fErrorFunction */
    "EulerKalman_2",                                      /* fFunctionName */
    NULL,                                                 /* fRTCallStack */
    false,                                                /* bDebugMode */
    {2045744189U, 2170104910U, 2743257031U, 4284093946U}, /* fSigWrd */
    NULL                                                  /* fSigMem */
};

/* Function Declarations */
static real_T *ab_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                   const emlrtMsgIdentifier *parentId,
                                   int32_T *y_size);

static real_T (*b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                   const emlrtMsgIdentifier *parentId))[4];

static const mxArray *b_emlrt_marshallOut(const real_T u[4]);

static real_T *bb_emlrt_marshallIn(const emlrtStack *sp, const mxArray *RxID,
                                   const char_T *identifier, int32_T y_size[2]);

static real_T (*c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *A,
                                   const char_T *identifier))[16];

static void c_emlrt_marshallOut(const real_T u[3], const mxArray *y);

static real_T *cb_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                   const emlrtMsgIdentifier *parentId,
                                   int32_T y_size[2]);

static real_T (*d_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                   const emlrtMsgIdentifier *parentId))[16];

static const mxArray *d_emlrt_marshallOut(const creal_T u);

static real_T (*db_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                    const emlrtMsgIdentifier *msgId))[4];

static real_T e_emlrt_marshallIn(const emlrtStack *sp, const mxArray *phi,
                                 const char_T *identifier);

static void e_emlrt_marshallOut(const real_T u[12], const mxArray *y);

static real_T (*eb_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                    const emlrtMsgIdentifier *msgId))[16];

static real_T (*emlrt_marshallIn(const emlrtStack *sp, const mxArray *x,
                                 const char_T *identifier))[4];

static const mxArray *emlrt_marshallOut(const real_T u);

static real_T f_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                 const emlrtMsgIdentifier *parentId);

static real_T fb_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                  const emlrtMsgIdentifier *msgId);

static creal_T g_emlrt_marshallIn(const emlrtStack *sp,
                                  const mxArray *tag_center_vel_est,
                                  const char_T *identifier);

static creal_T gb_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                   const emlrtMsgIdentifier *msgId);

static creal_T h_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                  const emlrtMsgIdentifier *parentId);

static real_T (*hb_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                    const emlrtMsgIdentifier *msgId))[3];

static real_T (*i_emlrt_marshallIn(const emlrtStack *sp, const mxArray *b_acc_o,
                                   const char_T *identifier))[3];

static real_T (*ib_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                    const emlrtMsgIdentifier *msgId))[5];

static real_T (*j_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                   const emlrtMsgIdentifier *parentId))[3];

static real_T (*jb_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                    const emlrtMsgIdentifier *msgId))[3];

static real_T (*k_emlrt_marshallIn(const emlrtStack *sp, const mxArray *k0,
                                   const char_T *identifier))[5];

static void kb_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                const emlrtMsgIdentifier *msgId,
                                creal_T ret[4]);

static real_T (*l_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                   const emlrtMsgIdentifier *parentId))[5];

static real_T (*lb_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                    const emlrtMsgIdentifier *msgId))[12];

static real_T (*m_emlrt_marshallIn(const emlrtStack *sp,
                                   const mxArray *cent_pos_est,
                                   const char_T *identifier))[3];

static real_T (*mb_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                    const emlrtMsgIdentifier *msgId))[144];

static real_T (*n_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                   const emlrtMsgIdentifier *parentId))[3];

static real_T (*nb_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                    const emlrtMsgIdentifier *msgId))[9];

static void o_emlrt_marshallIn(const emlrtStack *sp, const mxArray *tag_pos_est,
                               const char_T *identifier, creal_T y[4]);

static void ob_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                const emlrtMsgIdentifier *msgId,
                                creal_T ret[6]);

static void p_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                               const emlrtMsgIdentifier *parentId,
                               creal_T y[4]);

static real_T *pb_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                   const emlrtMsgIdentifier *msgId,
                                   int32_T *ret_size);

static real_T (*q_emlrt_marshallIn(const emlrtStack *sp, const mxArray *x,
                                   const char_T *identifier))[12];

static real_T *qb_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                   const emlrtMsgIdentifier *msgId,
                                   int32_T ret_size[2]);

static real_T (*r_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                   const emlrtMsgIdentifier *parentId))[12];

static real_T (*s_emlrt_marshallIn(const emlrtStack *sp, const mxArray *A,
                                   const char_T *identifier))[144];

static real_T (*t_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                   const emlrtMsgIdentifier *parentId))[144];

static real_T (*u_emlrt_marshallIn(const emlrtStack *sp, const mxArray *R,
                                   const char_T *identifier))[9];

static real_T (*v_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                   const emlrtMsgIdentifier *parentId))[9];

static void w_emlrt_marshallIn(const emlrtStack *sp, const mxArray *anchor_pos,
                               const char_T *identifier, creal_T y[6]);

static void x_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                               const emlrtMsgIdentifier *parentId,
                               creal_T y[6]);

static real_T *y_emlrt_marshallIn(const emlrtStack *sp, const mxArray *RxDist,
                                  const char_T *identifier, int32_T *y_size);

/* Function Definitions */
/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 *                int32_T *y_size
 * Return Type  : real_T *
 */
static real_T *ab_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                   const emlrtMsgIdentifier *parentId,
                                   int32_T *y_size)
{
  real_T *y_data;
  int32_T i;
  y_data = pb_emlrt_marshallIn(sp, emlrtAlias(u), parentId, &i);
  *y_size = i;
  emlrtDestroyArray(&u);
  return y_data;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 * Return Type  : real_T (*)[4]
 */
static real_T (*b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                   const emlrtMsgIdentifier *parentId))[4]
{
  real_T(*y)[4];
  y = db_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

/*
 * Arguments    : const real_T u[4]
 * Return Type  : const mxArray *
 */
static const mxArray *b_emlrt_marshallOut(const real_T u[4])
{
  static const int32_T i = 0;
  static const int32_T i1 = 4;
  const mxArray *m;
  const mxArray *y;
  y = NULL;
  m = emlrtCreateNumericArray(1, (const void *)&i, mxDOUBLE_CLASS, mxREAL);
  emlrtMxSetData((mxArray *)m, (void *)&u[0]);
  emlrtSetDimensions((mxArray *)m, &i1, 1);
  emlrtAssign(&y, m);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *RxID
 *                const char_T *identifier
 *                int32_T y_size[2]
 * Return Type  : real_T *
 */
static real_T *bb_emlrt_marshallIn(const emlrtStack *sp, const mxArray *RxID,
                                   const char_T *identifier, int32_T y_size[2])
{
  emlrtMsgIdentifier thisId;
  real_T *y_data;
  thisId.fIdentifier = (const char_T *)identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  y_data = cb_emlrt_marshallIn(sp, emlrtAlias(RxID), &thisId, y_size);
  emlrtDestroyArray(&RxID);
  return y_data;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *A
 *                const char_T *identifier
 * Return Type  : real_T (*)[16]
 */
static real_T (*c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *A,
                                   const char_T *identifier))[16]
{
  emlrtMsgIdentifier thisId;
  real_T(*y)[16];
  thisId.fIdentifier = (const char_T *)identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  y = d_emlrt_marshallIn(sp, emlrtAlias(A), &thisId);
  emlrtDestroyArray(&A);
  return y;
}

/*
 * Arguments    : const real_T u[3]
 *                const mxArray *y
 * Return Type  : void
 */
static void c_emlrt_marshallOut(const real_T u[3], const mxArray *y)
{
  static const int32_T i = 3;
  emlrtMxSetData((mxArray *)y, (void *)&u[0]);
  emlrtSetDimensions((mxArray *)y, &i, 1);
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 *                int32_T y_size[2]
 * Return Type  : real_T *
 */
static real_T *cb_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                   const emlrtMsgIdentifier *parentId,
                                   int32_T y_size[2])
{
  real_T *y_data;
  y_data = qb_emlrt_marshallIn(sp, emlrtAlias(u), parentId, y_size);
  emlrtDestroyArray(&u);
  return y_data;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 * Return Type  : real_T (*)[16]
 */
static real_T (*d_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                   const emlrtMsgIdentifier *parentId))[16]
{
  real_T(*y)[16];
  y = eb_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

/*
 * Arguments    : const creal_T u
 * Return Type  : const mxArray *
 */
static const mxArray *d_emlrt_marshallOut(const creal_T u)
{
  const mxArray *m;
  const mxArray *y;
  creal_T *r;
  y = NULL;
  m = emlrtCreateNumericMatrix(1, 1, mxDOUBLE_CLASS, mxCOMPLEX);
  r = (creal_T *)emlrtMxGetData(m);
  *r = u;
  emlrtFreeImagIfZero(m);
  emlrtAssign(&y, m);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 * Return Type  : real_T (*)[4]
 */
static real_T (*db_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                    const emlrtMsgIdentifier *msgId))[4]
{
  static const int32_T dims = 4;
  real_T(*ret)[4];
  int32_T i;
  boolean_T b = false;
  emlrtCheckVsBuiltInR2012b((emlrtConstCTX)sp, msgId, src, "double", false, 1U,
                            (const void *)&dims, &b, &i);
  ret = (real_T(*)[4])emlrtMxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *phi
 *                const char_T *identifier
 * Return Type  : real_T
 */
static real_T e_emlrt_marshallIn(const emlrtStack *sp, const mxArray *phi,
                                 const char_T *identifier)
{
  emlrtMsgIdentifier thisId;
  real_T y;
  thisId.fIdentifier = (const char_T *)identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  y = f_emlrt_marshallIn(sp, emlrtAlias(phi), &thisId);
  emlrtDestroyArray(&phi);
  return y;
}

/*
 * Arguments    : const real_T u[12]
 *                const mxArray *y
 * Return Type  : void
 */
static void e_emlrt_marshallOut(const real_T u[12], const mxArray *y)
{
  static const int32_T i = 12;
  emlrtMxSetData((mxArray *)y, (void *)&u[0]);
  emlrtSetDimensions((mxArray *)y, &i, 1);
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 * Return Type  : real_T (*)[16]
 */
static real_T (*eb_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                    const emlrtMsgIdentifier *msgId))[16]
{
  static const int32_T dims[2] = {4, 4};
  real_T(*ret)[16];
  int32_T iv[2];
  boolean_T bv[2] = {false, false};
  emlrtCheckVsBuiltInR2012b((emlrtConstCTX)sp, msgId, src, "double", false, 2U,
                            (const void *)&dims[0], &bv[0], &iv[0]);
  ret = (real_T(*)[16])emlrtMxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *x
 *                const char_T *identifier
 * Return Type  : real_T (*)[4]
 */
static real_T (*emlrt_marshallIn(const emlrtStack *sp, const mxArray *x,
                                 const char_T *identifier))[4]
{
  emlrtMsgIdentifier thisId;
  real_T(*y)[4];
  thisId.fIdentifier = (const char_T *)identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  y = b_emlrt_marshallIn(sp, emlrtAlias(x), &thisId);
  emlrtDestroyArray(&x);
  return y;
}

/*
 * Arguments    : const real_T u
 * Return Type  : const mxArray *
 */
static const mxArray *emlrt_marshallOut(const real_T u)
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
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 * Return Type  : real_T
 */
static real_T f_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                 const emlrtMsgIdentifier *parentId)
{
  real_T y;
  y = fb_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 * Return Type  : real_T
 */
static real_T fb_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
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
 *                const mxArray *tag_center_vel_est
 *                const char_T *identifier
 * Return Type  : creal_T
 */
static creal_T g_emlrt_marshallIn(const emlrtStack *sp,
                                  const mxArray *tag_center_vel_est,
                                  const char_T *identifier)
{
  emlrtMsgIdentifier thisId;
  creal_T y;
  thisId.fIdentifier = (const char_T *)identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  y = h_emlrt_marshallIn(sp, emlrtAlias(tag_center_vel_est), &thisId);
  emlrtDestroyArray(&tag_center_vel_est);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 * Return Type  : creal_T
 */
static creal_T gb_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                   const emlrtMsgIdentifier *msgId)
{
  static const int32_T dims = 0;
  creal_T ret;
  emlrtCheckBuiltInR2012b((emlrtConstCTX)sp, msgId, src, "double", true, 0U,
                          (const void *)&dims);
  emlrtImportArrayR2015b((emlrtConstCTX)sp, src, &ret, 8, true);
  emlrtDestroyArray(&src);
  return ret;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 * Return Type  : creal_T
 */
static creal_T h_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                  const emlrtMsgIdentifier *parentId)
{
  creal_T y;
  y = gb_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 * Return Type  : real_T (*)[3]
 */
static real_T (*hb_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
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
 *                const mxArray *b_acc_o
 *                const char_T *identifier
 * Return Type  : real_T (*)[3]
 */
static real_T (*i_emlrt_marshallIn(const emlrtStack *sp, const mxArray *b_acc_o,
                                   const char_T *identifier))[3]
{
  emlrtMsgIdentifier thisId;
  real_T(*y)[3];
  thisId.fIdentifier = (const char_T *)identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  y = j_emlrt_marshallIn(sp, emlrtAlias(b_acc_o), &thisId);
  emlrtDestroyArray(&b_acc_o);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 * Return Type  : real_T (*)[5]
 */
static real_T (*ib_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                    const emlrtMsgIdentifier *msgId))[5]
{
  static const int32_T dims[2] = {1, 5};
  real_T(*ret)[5];
  int32_T iv[2];
  boolean_T bv[2] = {false, false};
  emlrtCheckVsBuiltInR2012b((emlrtConstCTX)sp, msgId, src, "double", false, 2U,
                            (const void *)&dims[0], &bv[0], &iv[0]);
  ret = (real_T(*)[5])emlrtMxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 * Return Type  : real_T (*)[3]
 */
static real_T (*j_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                   const emlrtMsgIdentifier *parentId))[3]
{
  real_T(*y)[3];
  y = hb_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 * Return Type  : real_T (*)[3]
 */
static real_T (*jb_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
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
 * Arguments    : const emlrtStack *sp
 *                const mxArray *k0
 *                const char_T *identifier
 * Return Type  : real_T (*)[5]
 */
static real_T (*k_emlrt_marshallIn(const emlrtStack *sp, const mxArray *k0,
                                   const char_T *identifier))[5]
{
  emlrtMsgIdentifier thisId;
  real_T(*y)[5];
  thisId.fIdentifier = (const char_T *)identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  y = l_emlrt_marshallIn(sp, emlrtAlias(k0), &thisId);
  emlrtDestroyArray(&k0);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 *                creal_T ret[4]
 * Return Type  : void
 */
static void kb_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                const emlrtMsgIdentifier *msgId, creal_T ret[4])
{
  static const int32_T dims[2] = {1, 4};
  emlrtCheckBuiltInR2012b((emlrtConstCTX)sp, msgId, src, "double", true, 2U,
                          (const void *)&dims[0]);
  emlrtImportArrayR2015b((emlrtConstCTX)sp, src, &ret[0], 8, true);
  emlrtDestroyArray(&src);
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 * Return Type  : real_T (*)[5]
 */
static real_T (*l_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                   const emlrtMsgIdentifier *parentId))[5]
{
  real_T(*y)[5];
  y = ib_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 * Return Type  : real_T (*)[12]
 */
static real_T (*lb_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                    const emlrtMsgIdentifier *msgId))[12]
{
  static const int32_T dims = 12;
  real_T(*ret)[12];
  int32_T i;
  boolean_T b = false;
  emlrtCheckVsBuiltInR2012b((emlrtConstCTX)sp, msgId, src, "double", false, 1U,
                            (const void *)&dims, &b, &i);
  ret = (real_T(*)[12])emlrtMxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *cent_pos_est
 *                const char_T *identifier
 * Return Type  : real_T (*)[3]
 */
static real_T (*m_emlrt_marshallIn(const emlrtStack *sp,
                                   const mxArray *cent_pos_est,
                                   const char_T *identifier))[3]
{
  emlrtMsgIdentifier thisId;
  real_T(*y)[3];
  thisId.fIdentifier = (const char_T *)identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  y = n_emlrt_marshallIn(sp, emlrtAlias(cent_pos_est), &thisId);
  emlrtDestroyArray(&cent_pos_est);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 * Return Type  : real_T (*)[144]
 */
static real_T (*mb_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                    const emlrtMsgIdentifier *msgId))[144]
{
  static const int32_T dims[2] = {12, 12};
  real_T(*ret)[144];
  int32_T iv[2];
  boolean_T bv[2] = {false, false};
  emlrtCheckVsBuiltInR2012b((emlrtConstCTX)sp, msgId, src, "double", false, 2U,
                            (const void *)&dims[0], &bv[0], &iv[0]);
  ret = (real_T(*)[144])emlrtMxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 * Return Type  : real_T (*)[3]
 */
static real_T (*n_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                   const emlrtMsgIdentifier *parentId))[3]
{
  real_T(*y)[3];
  y = jb_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 * Return Type  : real_T (*)[9]
 */
static real_T (*nb_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                    const emlrtMsgIdentifier *msgId))[9]
{
  static const int32_T dims[2] = {3, 3};
  real_T(*ret)[9];
  int32_T iv[2];
  boolean_T bv[2] = {false, false};
  emlrtCheckVsBuiltInR2012b((emlrtConstCTX)sp, msgId, src, "double", false, 2U,
                            (const void *)&dims[0], &bv[0], &iv[0]);
  ret = (real_T(*)[9])emlrtMxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *tag_pos_est
 *                const char_T *identifier
 *                creal_T y[4]
 * Return Type  : void
 */
static void o_emlrt_marshallIn(const emlrtStack *sp, const mxArray *tag_pos_est,
                               const char_T *identifier, creal_T y[4])
{
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = (const char_T *)identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  p_emlrt_marshallIn(sp, emlrtAlias(tag_pos_est), &thisId, y);
  emlrtDestroyArray(&tag_pos_est);
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 *                creal_T ret[6]
 * Return Type  : void
 */
static void ob_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                const emlrtMsgIdentifier *msgId, creal_T ret[6])
{
  static const int32_T dims[2] = {1, 6};
  emlrtCheckBuiltInR2012b((emlrtConstCTX)sp, msgId, src, "double", true, 2U,
                          (const void *)&dims[0]);
  emlrtImportArrayR2015b((emlrtConstCTX)sp, src, &ret[0], 8, true);
  emlrtDestroyArray(&src);
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 *                creal_T y[4]
 * Return Type  : void
 */
static void p_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                               const emlrtMsgIdentifier *parentId, creal_T y[4])
{
  kb_emlrt_marshallIn(sp, emlrtAlias(u), parentId, y);
  emlrtDestroyArray(&u);
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 *                int32_T *ret_size
 * Return Type  : real_T *
 */
static real_T *pb_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                   const emlrtMsgIdentifier *msgId,
                                   int32_T *ret_size)
{
  static const int32_T dims = 4;
  real_T *ret_data;
  boolean_T b = true;
  emlrtCheckVsBuiltInR2012b((emlrtConstCTX)sp, msgId, src, "double", false, 1U,
                            (const void *)&dims, &b, ret_size);
  ret_data = (real_T *)emlrtMxGetData(src);
  emlrtDestroyArray(&src);
  return ret_data;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *x
 *                const char_T *identifier
 * Return Type  : real_T (*)[12]
 */
static real_T (*q_emlrt_marshallIn(const emlrtStack *sp, const mxArray *x,
                                   const char_T *identifier))[12]
{
  emlrtMsgIdentifier thisId;
  real_T(*y)[12];
  thisId.fIdentifier = (const char_T *)identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  y = r_emlrt_marshallIn(sp, emlrtAlias(x), &thisId);
  emlrtDestroyArray(&x);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 *                int32_T ret_size[2]
 * Return Type  : real_T *
 */
static real_T *qb_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                   const emlrtMsgIdentifier *msgId,
                                   int32_T ret_size[2])
{
  static const int32_T dims[2] = {1, 4};
  real_T *ret_data;
  int32_T iv[2];
  boolean_T bv[2] = {false, true};
  emlrtCheckVsBuiltInR2012b((emlrtConstCTX)sp, msgId, src, "double", false, 2U,
                            (const void *)&dims[0], &bv[0], &iv[0]);
  ret_size[0] = iv[0];
  ret_size[1] = iv[1];
  ret_data = (real_T *)emlrtMxGetData(src);
  emlrtDestroyArray(&src);
  return ret_data;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 * Return Type  : real_T (*)[12]
 */
static real_T (*r_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                   const emlrtMsgIdentifier *parentId))[12]
{
  real_T(*y)[12];
  y = lb_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *A
 *                const char_T *identifier
 * Return Type  : real_T (*)[144]
 */
static real_T (*s_emlrt_marshallIn(const emlrtStack *sp, const mxArray *A,
                                   const char_T *identifier))[144]
{
  emlrtMsgIdentifier thisId;
  real_T(*y)[144];
  thisId.fIdentifier = (const char_T *)identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  y = t_emlrt_marshallIn(sp, emlrtAlias(A), &thisId);
  emlrtDestroyArray(&A);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 * Return Type  : real_T (*)[144]
 */
static real_T (*t_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                   const emlrtMsgIdentifier *parentId))[144]
{
  real_T(*y)[144];
  y = mb_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *R
 *                const char_T *identifier
 * Return Type  : real_T (*)[9]
 */
static real_T (*u_emlrt_marshallIn(const emlrtStack *sp, const mxArray *R,
                                   const char_T *identifier))[9]
{
  emlrtMsgIdentifier thisId;
  real_T(*y)[9];
  thisId.fIdentifier = (const char_T *)identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  y = v_emlrt_marshallIn(sp, emlrtAlias(R), &thisId);
  emlrtDestroyArray(&R);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 * Return Type  : real_T (*)[9]
 */
static real_T (*v_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                   const emlrtMsgIdentifier *parentId))[9]
{
  real_T(*y)[9];
  y = nb_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *anchor_pos
 *                const char_T *identifier
 *                creal_T y[6]
 * Return Type  : void
 */
static void w_emlrt_marshallIn(const emlrtStack *sp, const mxArray *anchor_pos,
                               const char_T *identifier, creal_T y[6])
{
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = (const char_T *)identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  x_emlrt_marshallIn(sp, emlrtAlias(anchor_pos), &thisId, y);
  emlrtDestroyArray(&anchor_pos);
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 *                creal_T y[6]
 * Return Type  : void
 */
static void x_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                               const emlrtMsgIdentifier *parentId, creal_T y[6])
{
  ob_emlrt_marshallIn(sp, emlrtAlias(u), parentId, y);
  emlrtDestroyArray(&u);
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *RxDist
 *                const char_T *identifier
 *                int32_T *y_size
 * Return Type  : real_T *
 */
static real_T *y_emlrt_marshallIn(const emlrtStack *sp, const mxArray *RxDist,
                                  const char_T *identifier, int32_T *y_size)
{
  emlrtMsgIdentifier thisId;
  real_T *y_data;
  int32_T i;
  thisId.fIdentifier = (const char_T *)identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  y_data = ab_emlrt_marshallIn(sp, emlrtAlias(RxDist), &thisId, &i);
  *y_size = i;
  emlrtDestroyArray(&RxDist);
  return y_data;
}

/*
 * Arguments    : const mxArray * const prhs[3]
 *                int32_T nlhs
 *                const mxArray *plhs[3]
 * Return Type  : void
 */
void EulerKalman_2_api(const mxArray *const prhs[3], int32_T nlhs,
                       const mxArray *plhs[3])
{
  emlrtStack st = {
      NULL, /* site */
      NULL, /* tls */
      NULL  /* prev */
  };
  const mxArray *prhs_copy_idx_0;
  real_T(*A)[16];
  real_T(*x)[4];
  real_T(*z)[4];
  real_T phi;
  real_T psi;
  real_T theta;
  st.tls = emlrtRootTLSGlobal;
  prhs_copy_idx_0 = emlrtProtectR2012b(prhs[0], 0, false, -1);
  /* Marshall function inputs */
  x = emlrt_marshallIn(&st, emlrtAlias(prhs_copy_idx_0), "x");
  A = c_emlrt_marshallIn(&st, emlrtAlias(prhs[1]), "A");
  z = emlrt_marshallIn(&st, emlrtAlias(prhs[2]), "z");
  /* Invoke the target function */
  EulerKalman_2(*x, *A, *z, &phi, &theta, &psi);
  /* Marshall function outputs */
  plhs[0] = emlrt_marshallOut(phi);
  if (nlhs > 1) {
    plhs[1] = emlrt_marshallOut(theta);
  }
  if (nlhs > 2) {
    plhs[2] = emlrt_marshallOut(psi);
  }
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void EulerKalman_2_atexit(void)
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
  EulerKalman_2_xil_terminate();
  EulerKalman_2_xil_shutdown();
  emlrtExitTimeCleanup(&emlrtContextGlobal);
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void EulerKalman_2_initialize(void)
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
void EulerKalman_2_terminate(void)
{
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
}

/*
 * Arguments    : const mxArray * const prhs[3]
 *                const mxArray **plhs
 * Return Type  : void
 */
void EulerToQuaternion_api(const mxArray *const prhs[3], const mxArray **plhs)
{
  emlrtStack st = {
      NULL, /* site */
      NULL, /* tls */
      NULL  /* prev */
  };
  real_T(*z)[4];
  real_T phi;
  real_T psi;
  real_T theta;
  st.tls = emlrtRootTLSGlobal;
  z = (real_T(*)[4])mxMalloc(sizeof(real_T[4]));
  /* Marshall function inputs */
  phi = e_emlrt_marshallIn(&st, emlrtAliasP(prhs[0]), "phi");
  theta = e_emlrt_marshallIn(&st, emlrtAliasP(prhs[1]), "theta");
  psi = e_emlrt_marshallIn(&st, emlrtAliasP(prhs[2]), "psi");
  /* Invoke the target function */
  EulerToQuaternion(phi, theta, psi, *z);
  /* Marshall function outputs */
  *plhs = b_emlrt_marshallOut(*z);
}

/*
 * Arguments    : const mxArray * const prhs[4]
 *                const mxArray **plhs
 * Return Type  : void
 */
void PosKalman2_api(const mxArray *const prhs[4], const mxArray **plhs)
{
  emlrtStack st = {
      NULL, /* site */
      NULL, /* tls */
      NULL  /* prev */
  };
  const mxArray *prhs_copy_idx_0;
  real_T(*A)[144];
  real_T(*x)[12];
  real_T(*R)[9];
  real_T(*z)[3];
  st.tls = emlrtRootTLSGlobal;
  prhs_copy_idx_0 = emlrtProtectR2012b(prhs[0], 0, true, -1);
  /* Marshall function inputs */
  x = q_emlrt_marshallIn(&st, emlrtAlias(prhs_copy_idx_0), "x");
  A = s_emlrt_marshallIn(&st, emlrtAlias(prhs[1]), "A");
  z = m_emlrt_marshallIn(&st, emlrtAlias(prhs[2]), "z");
  R = u_emlrt_marshallIn(&st, emlrtAlias(prhs[3]), "R");
  /* Invoke the target function */
  PosKalman2(*x, *A, *z, *R);
  /* Marshall function outputs */
  e_emlrt_marshallOut(*x, prhs_copy_idx_0);
  *plhs = prhs_copy_idx_0;
}

/*
 * Arguments    : const mxArray * const prhs[5]
 *                const mxArray **plhs
 * Return Type  : void
 */
void ReliCalc_api(const mxArray *const prhs[5], const mxArray **plhs)
{
  emlrtStack st = {
      NULL, /* site */
      NULL, /* tls */
      NULL  /* prev */
  };
  creal_T anchor_pos[6];
  creal_T tag_pos_est[4];
  real_T(*RxDist_data)[4];
  real_T(*RxID_data)[4];
  real_T PP;
  int32_T RxID_size[2];
  int32_T RxDist_size;
  st.tls = emlrtRootTLSGlobal;
  /* Marshall function inputs */
  o_emlrt_marshallIn(&st, emlrtAliasP(prhs[0]), "tag_pos_est", tag_pos_est);
  w_emlrt_marshallIn(&st, emlrtAliasP(prhs[1]), "anchor_pos", anchor_pos);
  *(real_T **)&RxDist_data =
      y_emlrt_marshallIn(&st, emlrtAlias(prhs[2]), "RxDist", &RxDist_size);
  *(real_T **)&RxID_data =
      bb_emlrt_marshallIn(&st, emlrtAlias(prhs[3]), "RxID", RxID_size);
  PP = e_emlrt_marshallIn(&st, emlrtAliasP(prhs[4]), "PP");
  /* Invoke the target function */
  PP = ReliCalc(tag_pos_est, anchor_pos, *RxDist_data,
                *(int32_T(*)[1]) & RxDist_size, *RxID_data, RxID_size, PP);
  /* Marshall function outputs */
  *plhs = emlrt_marshallOut(PP);
}

/*
 * Arguments    : const mxArray * const prhs[16]
 *                int32_T nlhs
 *                const mxArray *plhs[6]
 * Return Type  : void
 */
void fusion_api(const mxArray *const prhs[16], int32_T nlhs,
                const mxArray *plhs[6])
{
  emlrtStack st = {
      NULL, /* site */
      NULL, /* tls */
      NULL  /* prev */
  };
  const mxArray *prhs_copy_idx_10;
  const mxArray *prhs_copy_idx_11;
  creal_T tag_pos_est[4];
  creal_T IMUposU;
  creal_T tag_center_vel_est;
  real_T(*k0)[5];
  real_T(*b_acc_o)[3];
  real_T(*cent_pos_est)[3];
  real_T(*cent_vel_est)[3];
  real_T Nanchor;
  real_T UWBErrSum;
  real_T acc_b_phi;
  real_T acc_b_theta;
  real_T heading_est;
  real_T init_flag;
  real_T kalman_on;
  real_T kf_psi;
  real_T state_IMU;
  real_T zt_b;
  st.tls = emlrtRootTLSGlobal;
  prhs_copy_idx_10 = emlrtProtectR2012b(prhs[10], 10, true, -1);
  prhs_copy_idx_11 = emlrtProtectR2012b(prhs[11], 11, true, -1);
  /* Marshall function inputs */
  tag_center_vel_est =
      g_emlrt_marshallIn(&st, emlrtAliasP(prhs[0]), "tag_center_vel_est");
  state_IMU = e_emlrt_marshallIn(&st, emlrtAliasP(prhs[1]), "state_IMU");
  Nanchor = e_emlrt_marshallIn(&st, emlrtAliasP(prhs[2]), "Nanchor");
  b_acc_o = i_emlrt_marshallIn(&st, emlrtAlias(prhs[3]), "b_acc_o");
  acc_b_theta = e_emlrt_marshallIn(&st, emlrtAliasP(prhs[4]), "acc_b_theta");
  acc_b_phi = e_emlrt_marshallIn(&st, emlrtAliasP(prhs[5]), "acc_b_phi");
  UWBErrSum = e_emlrt_marshallIn(&st, emlrtAliasP(prhs[6]), "UWBErrSum");
  init_flag = e_emlrt_marshallIn(&st, emlrtAliasP(prhs[7]), "init_flag");
  kalman_on = e_emlrt_marshallIn(&st, emlrtAliasP(prhs[8]), "kalman_on");
  k0 = k_emlrt_marshallIn(&st, emlrtAlias(prhs[9]), "k0");
  cent_pos_est =
      m_emlrt_marshallIn(&st, emlrtAlias(prhs_copy_idx_10), "cent_pos_est");
  cent_vel_est =
      m_emlrt_marshallIn(&st, emlrtAlias(prhs_copy_idx_11), "cent_vel_est");
  kf_psi = e_emlrt_marshallIn(&st, emlrtAliasP(prhs[12]), "kf_psi");
  o_emlrt_marshallIn(&st, emlrtAliasP(prhs[13]), "tag_pos_est", tag_pos_est);
  heading_est = e_emlrt_marshallIn(&st, emlrtAliasP(prhs[14]), "heading_est");
  zt_b = e_emlrt_marshallIn(&st, emlrtAliasP(prhs[15]), "zt_b");
  /* Invoke the target function */
  IMUposU =
      fusion(&tag_center_vel_est, state_IMU, Nanchor, *b_acc_o, acc_b_theta,
             &acc_b_phi, UWBErrSum, init_flag, kalman_on, *k0, *cent_pos_est,
             *cent_vel_est, &kf_psi, tag_pos_est, heading_est, zt_b);
  /* Marshall function outputs */
  c_emlrt_marshallOut(*cent_pos_est, prhs_copy_idx_10);
  plhs[0] = prhs_copy_idx_10;
  if (nlhs > 1) {
    c_emlrt_marshallOut(*cent_vel_est, prhs_copy_idx_11);
    plhs[1] = prhs_copy_idx_11;
  }
  if (nlhs > 2) {
    plhs[2] = emlrt_marshallOut(kf_psi);
  }
  if (nlhs > 3) {
    plhs[3] = d_emlrt_marshallOut(IMUposU);
  }
  if (nlhs > 4) {
    plhs[4] = emlrt_marshallOut(acc_b_phi);
  }
  if (nlhs > 5) {
    plhs[5] = d_emlrt_marshallOut(tag_center_vel_est);
  }
}

/*
 * File trailer for _coder_EulerKalman_2_api.c
 *
 * [EOF]
 */
