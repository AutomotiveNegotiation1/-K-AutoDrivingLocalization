//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: _coder_fusion2_api.cpp
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 30-Nov-2023 16:43:53
//

// Include Files
#include "_coder_fusion2_api.h"
#include "_coder_fusion2_mex.h"

// Variable Definitions
emlrtCTX emlrtRootTLSGlobal{nullptr};

emlrtContext emlrtContextGlobal{
    true,                                                 // bFirstTime
    false,                                                // bInitialized
    131626U,                                              // fVersionInfo
    nullptr,                                              // fErrorFunction
    "fusion2",                                            // fFunctionName
    nullptr,                                              // fRTCallStack
    false,                                                // bDebugMode
    {2045744189U, 2170104910U, 2743257031U, 4284093946U}, // fSigWrd
    nullptr                                               // fSigMem
};

// Function Declarations
static real_T (*ab_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                    const emlrtMsgIdentifier *msgId))[6];

static real_T (*b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *A,
                                   const char_T *identifier))[16];

static real_T (*b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                   const emlrtMsgIdentifier *parentId))[16];

static void b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                               const emlrtMsgIdentifier *msgId, creal_T ret[4]);

static void b_emlrt_marshallOut(const real_T u[3], const mxArray *y);

static real_T c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                 const emlrtMsgIdentifier *parentId);

static real_T c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *kl,
                                 const char_T *identifier);

static void c_emlrt_marshallOut(const real_T u[12], const mxArray *y);

static real_T (*d_emlrt_marshallIn(const emlrtStack *sp, const mxArray *k0,
                                   const char_T *identifier))[5];

static real_T (*d_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                   const emlrtMsgIdentifier *parentId))[5];

static creal_T e_emlrt_marshallIn(const emlrtStack *sp,
                                  const mxArray *tag_center_vel_est,
                                  const char_T *identifier);

static creal_T e_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                  const emlrtMsgIdentifier *parentId);

static real_T (*emlrt_marshallIn(const emlrtStack *sp, const mxArray *x,
                                 const char_T *identifier))[4];

static real_T (*emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                 const emlrtMsgIdentifier *parentId))[4];

static void emlrt_marshallIn(const emlrtStack *sp, const mxArray *tag_pos_est,
                             const char_T *identifier, creal_T y[4]);

static void emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                             const emlrtMsgIdentifier *parentId, creal_T y[4]);

static const mxArray *emlrt_marshallOut(const creal_T u);

static const mxArray *emlrt_marshallOut(const real_T u[21]);

static void emlrt_marshallOut(const real_T u[3], const mxArray *y);

static const mxArray *emlrt_marshallOut(const real_T u);

static real_T (*f_emlrt_marshallIn(const emlrtStack *sp,
                                   const mxArray *cent_pos_est,
                                   const char_T *identifier))[3];

static real_T (*f_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                   const emlrtMsgIdentifier *parentId))[3];

static real_T (*g_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                   const emlrtMsgIdentifier *parentId))[3];

static real_T (*g_emlrt_marshallIn(const emlrtStack *sp, const mxArray *b_acc_o,
                                   const char_T *identifier))[3];

static real_T (*h_emlrt_marshallIn(const emlrtStack *sp, const mxArray *Acc_Pos,
                                   const char_T *identifier))[60];

static real_T (*h_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                   const emlrtMsgIdentifier *parentId))[60];

static real_T (*i_emlrt_marshallIn(const emlrtStack *sp, const mxArray *x,
                                   const char_T *identifier))[12];

static real_T (*i_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                   const emlrtMsgIdentifier *parentId))[12];

static real_T (*j_emlrt_marshallIn(const emlrtStack *sp, const mxArray *A,
                                   const char_T *identifier))[144];

static real_T (*j_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                   const emlrtMsgIdentifier *parentId))[144];

static real_T (*k_emlrt_marshallIn(const emlrtStack *sp, const mxArray *R,
                                   const char_T *identifier))[9];

static real_T (*k_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                   const emlrtMsgIdentifier *parentId))[9];

static real_T (*l_emlrt_marshallIn(const emlrtStack *sp, const mxArray *RxIDin,
                                   const char_T *identifier))[6];

static real_T (*l_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                   const emlrtMsgIdentifier *parentId))[6];

static real_T (*m_emlrt_marshallIn(const emlrtStack *sp, const mxArray *xain,
                                   const char_T *identifier))[6];

static real_T (*m_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                   const emlrtMsgIdentifier *parentId))[6];

static real_T (*n_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                   const emlrtMsgIdentifier *msgId))[4];

static real_T (*o_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                   const emlrtMsgIdentifier *msgId))[16];

static real_T p_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                 const emlrtMsgIdentifier *msgId);

static real_T (*q_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                   const emlrtMsgIdentifier *msgId))[5];

static creal_T r_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                  const emlrtMsgIdentifier *msgId);

static real_T (*s_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                   const emlrtMsgIdentifier *msgId))[3];

static real_T (*t_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                   const emlrtMsgIdentifier *msgId))[3];

static real_T (*u_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                   const emlrtMsgIdentifier *msgId))[60];

static real_T (*v_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                   const emlrtMsgIdentifier *msgId))[12];

static real_T (*w_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                   const emlrtMsgIdentifier *msgId))[144];

static real_T (*x_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                   const emlrtMsgIdentifier *msgId))[9];

static real_T (*y_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                   const emlrtMsgIdentifier *msgId))[6];

// Function Definitions
//
// Arguments    : const emlrtStack *sp
//                const mxArray *src
//                const emlrtMsgIdentifier *msgId
// Return Type  : real_T (*)[6]
//
static real_T (*ab_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                    const emlrtMsgIdentifier *msgId))[6]
{
  static const int32_T dims[2]{1, 6};
  real_T(*ret)[6];
  emlrtCheckBuiltInR2012b((emlrtCTX)sp, msgId, src, (const char_T *)"double",
                          false, 2U, (void *)&dims[0]);
  ret = (real_T(*)[6])emlrtMxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}

//
// Arguments    : const emlrtStack *sp
//                const mxArray *u
//                const emlrtMsgIdentifier *parentId
// Return Type  : real_T (*)[16]
//
static real_T (*b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                   const emlrtMsgIdentifier *parentId))[16]
{
  real_T(*y)[16];
  y = o_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

//
// Arguments    : const emlrtStack *sp
//                const mxArray *src
//                const emlrtMsgIdentifier *msgId
//                creal_T ret[4]
// Return Type  : void
//
static void b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                               const emlrtMsgIdentifier *msgId, creal_T ret[4])
{
  static const int32_T dims[2]{1, 4};
  emlrtCheckBuiltInR2012b((emlrtCTX)sp, msgId, src, (const char_T *)"double",
                          true, 2U, (void *)&dims[0]);
  emlrtImportArrayR2015b((emlrtCTX)sp, src, &ret[0], 8, true);
  emlrtDestroyArray(&src);
}

//
// Arguments    : const emlrtStack *sp
//                const mxArray *A
//                const char_T *identifier
// Return Type  : real_T (*)[16]
//
static real_T (*b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *A,
                                   const char_T *identifier))[16]
{
  emlrtMsgIdentifier thisId;
  real_T(*y)[16];
  thisId.fIdentifier = const_cast<const char_T *>(identifier);
  thisId.fParent = nullptr;
  thisId.bParentIsCell = false;
  y = b_emlrt_marshallIn(sp, emlrtAlias(A), &thisId);
  emlrtDestroyArray(&A);
  return y;
}

//
// Arguments    : const real_T u[3]
//                const mxArray *y
// Return Type  : void
//
static void b_emlrt_marshallOut(const real_T u[3], const mxArray *y)
{
  static const int32_T i{3};
  emlrtMxSetData((mxArray *)y, (void *)&u[0]);
  emlrtSetDimensions((mxArray *)y, &i, 1);
}

//
// Arguments    : const emlrtStack *sp
//                const mxArray *u
//                const emlrtMsgIdentifier *parentId
// Return Type  : real_T
//
static real_T c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                 const emlrtMsgIdentifier *parentId)
{
  real_T y;
  y = p_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

//
// Arguments    : const emlrtStack *sp
//                const mxArray *kl
//                const char_T *identifier
// Return Type  : real_T
//
static real_T c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *kl,
                                 const char_T *identifier)
{
  emlrtMsgIdentifier thisId;
  real_T y;
  thisId.fIdentifier = const_cast<const char_T *>(identifier);
  thisId.fParent = nullptr;
  thisId.bParentIsCell = false;
  y = c_emlrt_marshallIn(sp, emlrtAlias(kl), &thisId);
  emlrtDestroyArray(&kl);
  return y;
}

//
// Arguments    : const real_T u[12]
//                const mxArray *y
// Return Type  : void
//
static void c_emlrt_marshallOut(const real_T u[12], const mxArray *y)
{
  static const int32_T i{12};
  emlrtMxSetData((mxArray *)y, (void *)&u[0]);
  emlrtSetDimensions((mxArray *)y, &i, 1);
}

//
// Arguments    : const emlrtStack *sp
//                const mxArray *u
//                const emlrtMsgIdentifier *parentId
// Return Type  : real_T (*)[5]
//
static real_T (*d_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                   const emlrtMsgIdentifier *parentId))[5]
{
  real_T(*y)[5];
  y = q_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

//
// Arguments    : const emlrtStack *sp
//                const mxArray *k0
//                const char_T *identifier
// Return Type  : real_T (*)[5]
//
static real_T (*d_emlrt_marshallIn(const emlrtStack *sp, const mxArray *k0,
                                   const char_T *identifier))[5]
{
  emlrtMsgIdentifier thisId;
  real_T(*y)[5];
  thisId.fIdentifier = const_cast<const char_T *>(identifier);
  thisId.fParent = nullptr;
  thisId.bParentIsCell = false;
  y = d_emlrt_marshallIn(sp, emlrtAlias(k0), &thisId);
  emlrtDestroyArray(&k0);
  return y;
}

//
// Arguments    : const emlrtStack *sp
//                const mxArray *tag_center_vel_est
//                const char_T *identifier
// Return Type  : creal_T
//
static creal_T e_emlrt_marshallIn(const emlrtStack *sp,
                                  const mxArray *tag_center_vel_est,
                                  const char_T *identifier)
{
  emlrtMsgIdentifier thisId;
  creal_T y;
  thisId.fIdentifier = const_cast<const char_T *>(identifier);
  thisId.fParent = nullptr;
  thisId.bParentIsCell = false;
  y = e_emlrt_marshallIn(sp, emlrtAlias(tag_center_vel_est), &thisId);
  emlrtDestroyArray(&tag_center_vel_est);
  return y;
}

//
// Arguments    : const emlrtStack *sp
//                const mxArray *u
//                const emlrtMsgIdentifier *parentId
// Return Type  : creal_T
//
static creal_T e_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                  const emlrtMsgIdentifier *parentId)
{
  creal_T y;
  y = r_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

//
// Arguments    : const emlrtStack *sp
//                const mxArray *tag_pos_est
//                const char_T *identifier
//                creal_T y[4]
// Return Type  : void
//
static void emlrt_marshallIn(const emlrtStack *sp, const mxArray *tag_pos_est,
                             const char_T *identifier, creal_T y[4])
{
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = const_cast<const char_T *>(identifier);
  thisId.fParent = nullptr;
  thisId.bParentIsCell = false;
  emlrt_marshallIn(sp, emlrtAlias(tag_pos_est), &thisId, y);
  emlrtDestroyArray(&tag_pos_est);
}

//
// Arguments    : const emlrtStack *sp
//                const mxArray *u
//                const emlrtMsgIdentifier *parentId
//                creal_T y[4]
// Return Type  : void
//
static void emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                             const emlrtMsgIdentifier *parentId, creal_T y[4])
{
  b_emlrt_marshallIn(sp, emlrtAlias(u), parentId, y);
  emlrtDestroyArray(&u);
}

//
// Arguments    : const emlrtStack *sp
//                const mxArray *u
//                const emlrtMsgIdentifier *parentId
// Return Type  : real_T (*)[4]
//
static real_T (*emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                 const emlrtMsgIdentifier *parentId))[4]
{
  real_T(*y)[4];
  y = n_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

//
// Arguments    : const emlrtStack *sp
//                const mxArray *x
//                const char_T *identifier
// Return Type  : real_T (*)[4]
//
static real_T (*emlrt_marshallIn(const emlrtStack *sp, const mxArray *x,
                                 const char_T *identifier))[4]
{
  emlrtMsgIdentifier thisId;
  real_T(*y)[4];
  thisId.fIdentifier = const_cast<const char_T *>(identifier);
  thisId.fParent = nullptr;
  thisId.bParentIsCell = false;
  y = emlrt_marshallIn(sp, emlrtAlias(x), &thisId);
  emlrtDestroyArray(&x);
  return y;
}

//
// Arguments    : const creal_T u
// Return Type  : const mxArray *
//
static const mxArray *emlrt_marshallOut(const creal_T u)
{
  const mxArray *m;
  const mxArray *y;
  creal_T *r;
  y = nullptr;
  m = emlrtCreateNumericMatrix(1, 1, mxDOUBLE_CLASS, mxCOMPLEX);
  r = static_cast<creal_T *>(emlrtMxGetData(m));
  *r = u;
  emlrtFreeImagIfZero(m);
  emlrtAssign(&y, m);
  return y;
}

//
// Arguments    : const real_T u[3]
//                const mxArray *y
// Return Type  : void
//
static void emlrt_marshallOut(const real_T u[3], const mxArray *y)
{
  static const int32_T iv[2]{1, 3};
  emlrtMxSetData((mxArray *)y, (void *)&u[0]);
  emlrtSetDimensions((mxArray *)y, &iv[0], 2);
}

//
// Arguments    : const real_T u
// Return Type  : const mxArray *
//
static const mxArray *emlrt_marshallOut(const real_T u)
{
  const mxArray *m;
  const mxArray *y;
  y = nullptr;
  m = emlrtCreateDoubleScalar(u);
  emlrtAssign(&y, m);
  return y;
}

//
// Arguments    : const real_T u[21]
// Return Type  : const mxArray *
//
static const mxArray *emlrt_marshallOut(const real_T u[21])
{
  static const int32_T iv[2]{0, 0};
  static const int32_T iv1[2]{1, 21};
  const mxArray *m;
  const mxArray *y;
  y = nullptr;
  m = emlrtCreateNumericArray(2, (const void *)&iv[0], mxDOUBLE_CLASS, mxREAL);
  emlrtMxSetData((mxArray *)m, (void *)&u[0]);
  emlrtSetDimensions((mxArray *)m, &iv1[0], 2);
  emlrtAssign(&y, m);
  return y;
}

//
// Arguments    : const emlrtStack *sp
//                const mxArray *u
//                const emlrtMsgIdentifier *parentId
// Return Type  : real_T (*)[3]
//
static real_T (*f_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                   const emlrtMsgIdentifier *parentId))[3]
{
  real_T(*y)[3];
  y = s_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

//
// Arguments    : const emlrtStack *sp
//                const mxArray *cent_pos_est
//                const char_T *identifier
// Return Type  : real_T (*)[3]
//
static real_T (*f_emlrt_marshallIn(const emlrtStack *sp,
                                   const mxArray *cent_pos_est,
                                   const char_T *identifier))[3]
{
  emlrtMsgIdentifier thisId;
  real_T(*y)[3];
  thisId.fIdentifier = const_cast<const char_T *>(identifier);
  thisId.fParent = nullptr;
  thisId.bParentIsCell = false;
  y = f_emlrt_marshallIn(sp, emlrtAlias(cent_pos_est), &thisId);
  emlrtDestroyArray(&cent_pos_est);
  return y;
}

//
// Arguments    : const emlrtStack *sp
//                const mxArray *b_acc_o
//                const char_T *identifier
// Return Type  : real_T (*)[3]
//
static real_T (*g_emlrt_marshallIn(const emlrtStack *sp, const mxArray *b_acc_o,
                                   const char_T *identifier))[3]
{
  emlrtMsgIdentifier thisId;
  real_T(*y)[3];
  thisId.fIdentifier = const_cast<const char_T *>(identifier);
  thisId.fParent = nullptr;
  thisId.bParentIsCell = false;
  y = g_emlrt_marshallIn(sp, emlrtAlias(b_acc_o), &thisId);
  emlrtDestroyArray(&b_acc_o);
  return y;
}

//
// Arguments    : const emlrtStack *sp
//                const mxArray *u
//                const emlrtMsgIdentifier *parentId
// Return Type  : real_T (*)[3]
//
static real_T (*g_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                   const emlrtMsgIdentifier *parentId))[3]
{
  real_T(*y)[3];
  y = t_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

//
// Arguments    : const emlrtStack *sp
//                const mxArray *Acc_Pos
//                const char_T *identifier
// Return Type  : real_T (*)[60]
//
static real_T (*h_emlrt_marshallIn(const emlrtStack *sp, const mxArray *Acc_Pos,
                                   const char_T *identifier))[60]
{
  emlrtMsgIdentifier thisId;
  real_T(*y)[60];
  thisId.fIdentifier = const_cast<const char_T *>(identifier);
  thisId.fParent = nullptr;
  thisId.bParentIsCell = false;
  y = h_emlrt_marshallIn(sp, emlrtAlias(Acc_Pos), &thisId);
  emlrtDestroyArray(&Acc_Pos);
  return y;
}

//
// Arguments    : const emlrtStack *sp
//                const mxArray *u
//                const emlrtMsgIdentifier *parentId
// Return Type  : real_T (*)[60]
//
static real_T (*h_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                   const emlrtMsgIdentifier *parentId))[60]
{
  real_T(*y)[60];
  y = u_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

//
// Arguments    : const emlrtStack *sp
//                const mxArray *u
//                const emlrtMsgIdentifier *parentId
// Return Type  : real_T (*)[12]
//
static real_T (*i_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                   const emlrtMsgIdentifier *parentId))[12]
{
  real_T(*y)[12];
  y = v_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

//
// Arguments    : const emlrtStack *sp
//                const mxArray *x
//                const char_T *identifier
// Return Type  : real_T (*)[12]
//
static real_T (*i_emlrt_marshallIn(const emlrtStack *sp, const mxArray *x,
                                   const char_T *identifier))[12]
{
  emlrtMsgIdentifier thisId;
  real_T(*y)[12];
  thisId.fIdentifier = const_cast<const char_T *>(identifier);
  thisId.fParent = nullptr;
  thisId.bParentIsCell = false;
  y = i_emlrt_marshallIn(sp, emlrtAlias(x), &thisId);
  emlrtDestroyArray(&x);
  return y;
}

//
// Arguments    : const emlrtStack *sp
//                const mxArray *u
//                const emlrtMsgIdentifier *parentId
// Return Type  : real_T (*)[144]
//
static real_T (*j_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                   const emlrtMsgIdentifier *parentId))[144]
{
  real_T(*y)[144];
  y = w_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

//
// Arguments    : const emlrtStack *sp
//                const mxArray *A
//                const char_T *identifier
// Return Type  : real_T (*)[144]
//
static real_T (*j_emlrt_marshallIn(const emlrtStack *sp, const mxArray *A,
                                   const char_T *identifier))[144]
{
  emlrtMsgIdentifier thisId;
  real_T(*y)[144];
  thisId.fIdentifier = const_cast<const char_T *>(identifier);
  thisId.fParent = nullptr;
  thisId.bParentIsCell = false;
  y = j_emlrt_marshallIn(sp, emlrtAlias(A), &thisId);
  emlrtDestroyArray(&A);
  return y;
}

//
// Arguments    : const emlrtStack *sp
//                const mxArray *u
//                const emlrtMsgIdentifier *parentId
// Return Type  : real_T (*)[9]
//
static real_T (*k_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                   const emlrtMsgIdentifier *parentId))[9]
{
  real_T(*y)[9];
  y = x_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

//
// Arguments    : const emlrtStack *sp
//                const mxArray *R
//                const char_T *identifier
// Return Type  : real_T (*)[9]
//
static real_T (*k_emlrt_marshallIn(const emlrtStack *sp, const mxArray *R,
                                   const char_T *identifier))[9]
{
  emlrtMsgIdentifier thisId;
  real_T(*y)[9];
  thisId.fIdentifier = const_cast<const char_T *>(identifier);
  thisId.fParent = nullptr;
  thisId.bParentIsCell = false;
  y = k_emlrt_marshallIn(sp, emlrtAlias(R), &thisId);
  emlrtDestroyArray(&R);
  return y;
}

//
// Arguments    : const emlrtStack *sp
//                const mxArray *RxIDin
//                const char_T *identifier
// Return Type  : real_T (*)[6]
//
static real_T (*l_emlrt_marshallIn(const emlrtStack *sp, const mxArray *RxIDin,
                                   const char_T *identifier))[6]
{
  emlrtMsgIdentifier thisId;
  real_T(*y)[6];
  thisId.fIdentifier = const_cast<const char_T *>(identifier);
  thisId.fParent = nullptr;
  thisId.bParentIsCell = false;
  y = l_emlrt_marshallIn(sp, emlrtAlias(RxIDin), &thisId);
  emlrtDestroyArray(&RxIDin);
  return y;
}

//
// Arguments    : const emlrtStack *sp
//                const mxArray *u
//                const emlrtMsgIdentifier *parentId
// Return Type  : real_T (*)[6]
//
static real_T (*l_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                   const emlrtMsgIdentifier *parentId))[6]
{
  real_T(*y)[6];
  y = y_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

//
// Arguments    : const emlrtStack *sp
//                const mxArray *xain
//                const char_T *identifier
// Return Type  : real_T (*)[6]
//
static real_T (*m_emlrt_marshallIn(const emlrtStack *sp, const mxArray *xain,
                                   const char_T *identifier))[6]
{
  emlrtMsgIdentifier thisId;
  real_T(*y)[6];
  thisId.fIdentifier = const_cast<const char_T *>(identifier);
  thisId.fParent = nullptr;
  thisId.bParentIsCell = false;
  y = m_emlrt_marshallIn(sp, emlrtAlias(xain), &thisId);
  emlrtDestroyArray(&xain);
  return y;
}

//
// Arguments    : const emlrtStack *sp
//                const mxArray *u
//                const emlrtMsgIdentifier *parentId
// Return Type  : real_T (*)[6]
//
static real_T (*m_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                   const emlrtMsgIdentifier *parentId))[6]
{
  real_T(*y)[6];
  y = ab_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

//
// Arguments    : const emlrtStack *sp
//                const mxArray *src
//                const emlrtMsgIdentifier *msgId
// Return Type  : real_T (*)[4]
//
static real_T (*n_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                   const emlrtMsgIdentifier *msgId))[4]
{
  static const int32_T dims{4};
  real_T(*ret)[4];
  emlrtCheckBuiltInR2012b((emlrtCTX)sp, msgId, src, (const char_T *)"double",
                          false, 1U, (void *)&dims);
  ret = (real_T(*)[4])emlrtMxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}

//
// Arguments    : const emlrtStack *sp
//                const mxArray *src
//                const emlrtMsgIdentifier *msgId
// Return Type  : real_T (*)[16]
//
static real_T (*o_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                   const emlrtMsgIdentifier *msgId))[16]
{
  static const int32_T dims[2]{4, 4};
  real_T(*ret)[16];
  emlrtCheckBuiltInR2012b((emlrtCTX)sp, msgId, src, (const char_T *)"double",
                          false, 2U, (void *)&dims[0]);
  ret = (real_T(*)[16])emlrtMxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}

//
// Arguments    : const emlrtStack *sp
//                const mxArray *src
//                const emlrtMsgIdentifier *msgId
// Return Type  : real_T
//
static real_T p_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                 const emlrtMsgIdentifier *msgId)
{
  static const int32_T dims{0};
  real_T ret;
  emlrtCheckBuiltInR2012b((emlrtCTX)sp, msgId, src, (const char_T *)"double",
                          false, 0U, (void *)&dims);
  ret = *static_cast<real_T *>(emlrtMxGetData(src));
  emlrtDestroyArray(&src);
  return ret;
}

//
// Arguments    : const emlrtStack *sp
//                const mxArray *src
//                const emlrtMsgIdentifier *msgId
// Return Type  : real_T (*)[5]
//
static real_T (*q_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                   const emlrtMsgIdentifier *msgId))[5]
{
  static const int32_T dims[2]{1, 5};
  real_T(*ret)[5];
  emlrtCheckBuiltInR2012b((emlrtCTX)sp, msgId, src, (const char_T *)"double",
                          false, 2U, (void *)&dims[0]);
  ret = (real_T(*)[5])emlrtMxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}

//
// Arguments    : const emlrtStack *sp
//                const mxArray *src
//                const emlrtMsgIdentifier *msgId
// Return Type  : creal_T
//
static creal_T r_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                  const emlrtMsgIdentifier *msgId)
{
  static const int32_T dims{0};
  creal_T ret;
  emlrtCheckBuiltInR2012b((emlrtCTX)sp, msgId, src, (const char_T *)"double",
                          true, 0U, (void *)&dims);
  emlrtImportArrayR2015b((emlrtCTX)sp, src, &ret, 8, true);
  emlrtDestroyArray(&src);
  return ret;
}

//
// Arguments    : const emlrtStack *sp
//                const mxArray *src
//                const emlrtMsgIdentifier *msgId
// Return Type  : real_T (*)[3]
//
static real_T (*s_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                   const emlrtMsgIdentifier *msgId))[3]
{
  static const int32_T dims{3};
  real_T(*ret)[3];
  emlrtCheckBuiltInR2012b((emlrtCTX)sp, msgId, src, (const char_T *)"double",
                          false, 1U, (void *)&dims);
  ret = (real_T(*)[3])emlrtMxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}

//
// Arguments    : const emlrtStack *sp
//                const mxArray *src
//                const emlrtMsgIdentifier *msgId
// Return Type  : real_T (*)[3]
//
static real_T (*t_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                   const emlrtMsgIdentifier *msgId))[3]
{
  static const int32_T dims[2]{1, 3};
  real_T(*ret)[3];
  emlrtCheckBuiltInR2012b((emlrtCTX)sp, msgId, src, (const char_T *)"double",
                          false, 2U, (void *)&dims[0]);
  ret = (real_T(*)[3])emlrtMxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}

//
// Arguments    : const emlrtStack *sp
//                const mxArray *src
//                const emlrtMsgIdentifier *msgId
// Return Type  : real_T (*)[60]
//
static real_T (*u_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                   const emlrtMsgIdentifier *msgId))[60]
{
  static const int32_T dims[2]{3, 20};
  real_T(*ret)[60];
  emlrtCheckBuiltInR2012b((emlrtCTX)sp, msgId, src, (const char_T *)"double",
                          false, 2U, (void *)&dims[0]);
  ret = (real_T(*)[60])emlrtMxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}

//
// Arguments    : const emlrtStack *sp
//                const mxArray *src
//                const emlrtMsgIdentifier *msgId
// Return Type  : real_T (*)[12]
//
static real_T (*v_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                   const emlrtMsgIdentifier *msgId))[12]
{
  static const int32_T dims{12};
  real_T(*ret)[12];
  emlrtCheckBuiltInR2012b((emlrtCTX)sp, msgId, src, (const char_T *)"double",
                          false, 1U, (void *)&dims);
  ret = (real_T(*)[12])emlrtMxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}

//
// Arguments    : const emlrtStack *sp
//                const mxArray *src
//                const emlrtMsgIdentifier *msgId
// Return Type  : real_T (*)[144]
//
static real_T (*w_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                   const emlrtMsgIdentifier *msgId))[144]
{
  static const int32_T dims[2]{12, 12};
  real_T(*ret)[144];
  emlrtCheckBuiltInR2012b((emlrtCTX)sp, msgId, src, (const char_T *)"double",
                          false, 2U, (void *)&dims[0]);
  ret = (real_T(*)[144])emlrtMxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}

//
// Arguments    : const emlrtStack *sp
//                const mxArray *src
//                const emlrtMsgIdentifier *msgId
// Return Type  : real_T (*)[9]
//
static real_T (*x_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                   const emlrtMsgIdentifier *msgId))[9]
{
  static const int32_T dims[2]{3, 3};
  real_T(*ret)[9];
  emlrtCheckBuiltInR2012b((emlrtCTX)sp, msgId, src, (const char_T *)"double",
                          false, 2U, (void *)&dims[0]);
  ret = (real_T(*)[9])emlrtMxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}

//
// Arguments    : const emlrtStack *sp
//                const mxArray *src
//                const emlrtMsgIdentifier *msgId
// Return Type  : real_T (*)[6]
//
static real_T (*y_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                   const emlrtMsgIdentifier *msgId))[6]
{
  static const int32_T dims{6};
  real_T(*ret)[6];
  emlrtCheckBuiltInR2012b((emlrtCTX)sp, msgId, src, (const char_T *)"double",
                          false, 1U, (void *)&dims);
  ret = (real_T(*)[6])emlrtMxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}

//
// Arguments    : const mxArray * const prhs[3]
//                int32_T nlhs
//                const mxArray *plhs[3]
// Return Type  : void
//
void EulerKalman_2_api(const mxArray *const prhs[3], int32_T nlhs,
                       const mxArray *plhs[3])
{
  emlrtStack st{
      nullptr, // site
      nullptr, // tls
      nullptr  // prev
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
  // Marshall function inputs
  x = emlrt_marshallIn(&st, emlrtAlias(prhs_copy_idx_0), "x");
  A = b_emlrt_marshallIn(&st, emlrtAlias(prhs[1]), "A");
  z = emlrt_marshallIn(&st, emlrtAlias(prhs[2]), "z");
  // Invoke the target function
  EulerKalman_2(*x, *A, *z, &phi, &theta, &psi);
  // Marshall function outputs
  plhs[0] = emlrt_marshallOut(phi);
  if (nlhs > 1) {
    plhs[1] = emlrt_marshallOut(theta);
  }
  if (nlhs > 2) {
    plhs[2] = emlrt_marshallOut(psi);
  }
}

//
// Arguments    : const mxArray * const prhs[10]
//                int32_T nlhs
//                const mxArray *plhs[9]
// Return Type  : void
//
void IMUpos_api(const mxArray *const prhs[10], int32_T nlhs,
                const mxArray *plhs[9])
{
  emlrtStack st{
      nullptr, // site
      nullptr, // tls
      nullptr  // prev
  };
  const mxArray *prhs_copy_idx_0;
  const mxArray *prhs_copy_idx_1;
  const mxArray *prhs_copy_idx_2;
  const mxArray *prhs_copy_idx_3;
  const mxArray *prhs_copy_idx_4;
  const mxArray *prhs_copy_idx_5;
  const mxArray *prhs_copy_idx_6;
  const mxArray *prhs_copy_idx_7;
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
  prhs_copy_idx_0 = prhs[0];
  prhs_copy_idx_1 = prhs[1];
  prhs_copy_idx_2 = prhs[2];
  prhs_copy_idx_3 = emlrtProtectR2012b(prhs[3], 3, true, -1);
  prhs_copy_idx_4 = emlrtProtectR2012b(prhs[4], 4, true, -1);
  prhs_copy_idx_5 = prhs[5];
  prhs_copy_idx_6 = prhs[6];
  prhs_copy_idx_7 = prhs[7];
  prhs_copy_idx_8 = emlrtProtectR2012b(prhs[8], 8, true, -1);
  prhs_copy_idx_9 = emlrtProtectR2012b(prhs[9], 9, true, -1);
  // Marshall function inputs
  IMUacc_c = g_emlrt_marshallIn(&st, emlrtAlias(prhs_copy_idx_0), "IMUacc_c");
  IMUgyro_c = g_emlrt_marshallIn(&st, emlrtAlias(prhs_copy_idx_1), "IMUgyro_c");
  s_time = c_emlrt_marshallIn(&st, emlrtAliasP(prhs_copy_idx_2), "s_time");
  b_acc_o = g_emlrt_marshallIn(&st, emlrtAlias(prhs_copy_idx_3), "b_acc_o");
  b_gyro = g_emlrt_marshallIn(&st, emlrtAlias(prhs_copy_idx_4), "b_gyro");
  mode = c_emlrt_marshallIn(&st, emlrtAliasP(prhs_copy_idx_5), "mode");
  kf_psi = c_emlrt_marshallIn(&st, emlrtAliasP(prhs_copy_idx_6), "kf_psi");
  gyro_psi = c_emlrt_marshallIn(&st, emlrtAliasP(prhs_copy_idx_7), "gyro_psi");
  cent_pos_est =
      f_emlrt_marshallIn(&st, emlrtAlias(prhs_copy_idx_8), "cent_pos_est");
  cent_vel_est =
      f_emlrt_marshallIn(&st, emlrtAlias(prhs_copy_idx_9), "cent_vel_est");
  // Invoke the target function
  IMUpos(*IMUacc_c, *IMUgyro_c, s_time, *b_acc_o, *b_gyro, mode, &kf_psi,
         &gyro_psi, *cent_pos_est, *cent_vel_est, &state_o, &acc_b_phi,
         &acc_b_theta);
  // Marshall function outputs
  b_emlrt_marshallOut(*cent_pos_est, prhs_copy_idx_8);
  plhs[0] = prhs_copy_idx_8;
  if (nlhs > 1) {
    b_emlrt_marshallOut(*cent_vel_est, prhs_copy_idx_9);
    plhs[1] = prhs_copy_idx_9;
  }
  if (nlhs > 2) {
    plhs[2] = emlrt_marshallOut(kf_psi);
  }
  if (nlhs > 3) {
    plhs[3] = emlrt_marshallOut(gyro_psi);
  }
  if (nlhs > 4) {
    emlrt_marshallOut(*b_acc_o, prhs_copy_idx_3);
    plhs[4] = prhs_copy_idx_3;
  }
  if (nlhs > 5) {
    emlrt_marshallOut(*b_gyro, prhs_copy_idx_4);
    plhs[5] = prhs_copy_idx_4;
  }
  if (nlhs > 6) {
    plhs[6] = emlrt_marshallOut(state_o);
  }
  if (nlhs > 7) {
    plhs[7] = emlrt_marshallOut(acc_b_phi);
  }
  if (nlhs > 8) {
    plhs[8] = emlrt_marshallOut(acc_b_theta);
  }
}

//
// Arguments    : const mxArray * const prhs[4]
//                const mxArray **plhs
// Return Type  : void
//
void PosKalman2_api(const mxArray *const prhs[4], const mxArray **plhs)
{
  emlrtStack st{
      nullptr, // site
      nullptr, // tls
      nullptr  // prev
  };
  const mxArray *prhs_copy_idx_0;
  real_T(*A)[144];
  real_T(*x)[12];
  real_T(*R)[9];
  real_T(*z)[3];
  st.tls = emlrtRootTLSGlobal;
  prhs_copy_idx_0 = emlrtProtectR2012b(prhs[0], 0, true, -1);
  // Marshall function inputs
  x = i_emlrt_marshallIn(&st, emlrtAlias(prhs_copy_idx_0), "x");
  A = j_emlrt_marshallIn(&st, emlrtAlias(prhs[1]), "A");
  z = f_emlrt_marshallIn(&st, emlrtAlias(prhs[2]), "z");
  R = k_emlrt_marshallIn(&st, emlrtAlias(prhs[3]), "R");
  // Invoke the target function
  PosKalman2(*x, *A, *z, *R);
  // Marshall function outputs
  c_emlrt_marshallOut(*x, prhs_copy_idx_0);
  *plhs = prhs_copy_idx_0;
}

//
// Arguments    : const mxArray * const prhs[13]
//                const mxArray **plhs
// Return Type  : void
//
void UWBpos6_api(const mxArray *const prhs[13], const mxArray **plhs)
{
  emlrtStack st{
      nullptr, // site
      nullptr, // tls
      nullptr  // prev
  };
  creal_T prevTagPos[4];
  creal_T tag_pos_b[4];
  real_T(*UWBout)[21];
  real_T(*RxDistin)[6];
  real_T(*RxIDin)[6];
  real_T(*xain)[6];
  real_T(*yain)[6];
  real_T Ln;
  real_T LnC;
  real_T Lp;
  real_T Nanchor;
  real_T TagNum;
  real_T prevTagHeading;
  real_T s_time;
  st.tls = emlrtRootTLSGlobal;
  UWBout = (real_T(*)[21])mxMalloc(sizeof(real_T[21]));
  // Marshall function inputs
  Ln = c_emlrt_marshallIn(&st, emlrtAliasP(prhs[0]), "Ln");
  Lp = c_emlrt_marshallIn(&st, emlrtAliasP(prhs[1]), "Lp");
  LnC = c_emlrt_marshallIn(&st, emlrtAliasP(prhs[2]), "LnC");
  TagNum = c_emlrt_marshallIn(&st, emlrtAliasP(prhs[3]), "TagNum");
  Nanchor = c_emlrt_marshallIn(&st, emlrtAliasP(prhs[4]), "Nanchor");
  RxIDin = l_emlrt_marshallIn(&st, emlrtAlias(prhs[5]), "RxIDin");
  RxDistin = l_emlrt_marshallIn(&st, emlrtAlias(prhs[6]), "RxDistin");
  s_time = c_emlrt_marshallIn(&st, emlrtAliasP(prhs[7]), "s_time");
  emlrt_marshallIn(&st, emlrtAliasP(prhs[8]), "tag_pos_b", tag_pos_b);
  xain = m_emlrt_marshallIn(&st, emlrtAlias(prhs[9]), "xain");
  yain = m_emlrt_marshallIn(&st, emlrtAlias(prhs[10]), "yain");
  emlrt_marshallIn(&st, emlrtAliasP(prhs[11]), "prevTagPos", prevTagPos);
  prevTagHeading =
      c_emlrt_marshallIn(&st, emlrtAliasP(prhs[12]), "prevTagHeading");
  // Invoke the target function
  UWBpos6(Ln, Lp, LnC, TagNum, Nanchor, *RxIDin, *RxDistin, s_time, tag_pos_b,
          *xain, *yain, prevTagPos, prevTagHeading, *UWBout);
  // Marshall function outputs
  *plhs = emlrt_marshallOut(*UWBout);
}

//
// Arguments    : const mxArray * const prhs[20]
//                int32_T nlhs
//                const mxArray *plhs[6]
// Return Type  : void
//
void fusion2_api(const mxArray *const prhs[20], int32_T nlhs,
                 const mxArray *plhs[6])
{
  emlrtStack st{
      nullptr, // site
      nullptr, // tls
      nullptr  // prev
  };
  const mxArray *prhs_copy_idx_0;
  const mxArray *prhs_copy_idx_1;
  const mxArray *prhs_copy_idx_2;
  const mxArray *prhs_copy_idx_3;
  const mxArray *prhs_copy_idx_4;
  const mxArray *prhs_copy_idx_5;
  const mxArray *prhs_copy_idx_6;
  const mxArray *prhs_copy_idx_7;
  const mxArray *prhs_copy_idx_8;
  const mxArray *prhs_copy_idx_9;
  creal_T tag_pos_est[4];
  creal_T tag_center_vel_est;
  real_T(*Acc_Pos)[60];
  real_T(*Acc_Vel)[60];
  real_T(*k0)[5];
  real_T(*b_acc_o)[3];
  real_T(*cent_pos_est)[3];
  real_T(*cent_vel_est)[3];
  real_T IMUSel;
  real_T Nanchor;
  real_T UWBErrSum;
  real_T acc_b_phi;
  real_T acc_b_theta;
  real_T gyro_psi;
  real_T heading_est;
  real_T init_flag;
  real_T kalman_on;
  real_T kf_psi;
  real_T kl;
  real_T state_IMU;
  st.tls = emlrtRootTLSGlobal;
  prhs_copy_idx_0 = prhs[0];
  prhs_copy_idx_1 = prhs[1];
  prhs_copy_idx_2 = prhs[2];
  prhs_copy_idx_3 = prhs[3];
  prhs_copy_idx_4 = emlrtProtectR2012b(prhs[4], 4, true, -1);
  prhs_copy_idx_5 = emlrtProtectR2012b(prhs[5], 5, true, -1);
  prhs_copy_idx_6 = emlrtProtectR2012b(prhs[6], 6, true, -1);
  prhs_copy_idx_7 = prhs[7];
  prhs_copy_idx_8 = emlrtProtectR2012b(prhs[8], 8, false, -1);
  prhs_copy_idx_9 = emlrtProtectR2012b(prhs[9], 9, false, -1);
  // Marshall function inputs
  kl = c_emlrt_marshallIn(&st, emlrtAliasP(prhs_copy_idx_0), "kl");
  k0 = d_emlrt_marshallIn(&st, emlrtAlias(prhs_copy_idx_1), "k0");
  emlrt_marshallIn(&st, emlrtAliasP(prhs_copy_idx_2), "tag_pos_est",
                   tag_pos_est);
  tag_center_vel_est = e_emlrt_marshallIn(&st, emlrtAliasP(prhs_copy_idx_3),
                                          "tag_center_vel_est");
  cent_pos_est =
      f_emlrt_marshallIn(&st, emlrtAlias(prhs_copy_idx_4), "cent_pos_est");
  cent_vel_est =
      f_emlrt_marshallIn(&st, emlrtAlias(prhs_copy_idx_5), "cent_vel_est");
  b_acc_o = g_emlrt_marshallIn(&st, emlrtAlias(prhs_copy_idx_6), "b_acc_o");
  acc_b_phi =
      c_emlrt_marshallIn(&st, emlrtAliasP(prhs_copy_idx_7), "acc_b_phi");
  Acc_Pos = h_emlrt_marshallIn(&st, emlrtAlias(prhs_copy_idx_8), "Acc_Pos");
  Acc_Vel = h_emlrt_marshallIn(&st, emlrtAlias(prhs_copy_idx_9), "Acc_Vel");
  kalman_on = c_emlrt_marshallIn(&st, emlrtAliasP(prhs[10]), "kalman_on");
  init_flag = c_emlrt_marshallIn(&st, emlrtAliasP(prhs[11]), "init_flag");
  UWBErrSum = c_emlrt_marshallIn(&st, emlrtAliasP(prhs[12]), "UWBErrSum");
  kf_psi = c_emlrt_marshallIn(&st, emlrtAliasP(prhs[13]), "kf_psi");
  gyro_psi = c_emlrt_marshallIn(&st, emlrtAliasP(prhs[14]), "gyro_psi");
  heading_est = c_emlrt_marshallIn(&st, emlrtAliasP(prhs[15]), "heading_est");
  acc_b_theta = c_emlrt_marshallIn(&st, emlrtAliasP(prhs[16]), "acc_b_theta");
  IMUSel = c_emlrt_marshallIn(&st, emlrtAliasP(prhs[17]), "IMUSel");
  Nanchor = c_emlrt_marshallIn(&st, emlrtAliasP(prhs[18]), "Nanchor");
  state_IMU = c_emlrt_marshallIn(&st, emlrtAliasP(prhs[19]), "state_IMU");
  // Invoke the target function
  tag_center_vel_est = fusion2(
      kl, *k0, tag_pos_est, tag_center_vel_est, *cent_pos_est, *cent_vel_est,
      *b_acc_o, acc_b_phi, *Acc_Pos, *Acc_Vel, kalman_on, init_flag, UWBErrSum,
      &kf_psi, &gyro_psi, heading_est, acc_b_theta, IMUSel, Nanchor, state_IMU);
  // Marshall function outputs
  plhs[0] = emlrt_marshallOut(tag_center_vel_est);
  if (nlhs > 1) {
    emlrt_marshallOut(*b_acc_o, prhs_copy_idx_6);
    plhs[1] = prhs_copy_idx_6;
  }
  if (nlhs > 2) {
    plhs[2] = emlrt_marshallOut(kf_psi);
  }
  if (nlhs > 3) {
    plhs[3] = emlrt_marshallOut(gyro_psi);
  }
  if (nlhs > 4) {
    b_emlrt_marshallOut(*cent_pos_est, prhs_copy_idx_4);
    plhs[4] = prhs_copy_idx_4;
  }
  if (nlhs > 5) {
    b_emlrt_marshallOut(*cent_vel_est, prhs_copy_idx_5);
    plhs[5] = prhs_copy_idx_5;
  }
}

//
// Arguments    : void
// Return Type  : void
//
void fusion2_atexit()
{
  emlrtStack st{
      nullptr, // site
      nullptr, // tls
      nullptr  // prev
  };
  mexFunctionCreateRootTLS();
  st.tls = emlrtRootTLSGlobal;
  emlrtEnterRtStackR2012b(&st);
  emlrtLeaveRtStackR2012b(&st);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
  fusion2_xil_terminate();
  fusion2_xil_shutdown();
  emlrtExitTimeCleanup(&emlrtContextGlobal);
}

//
// Arguments    : void
// Return Type  : void
//
void fusion2_initialize()
{
  emlrtStack st{
      nullptr, // site
      nullptr, // tls
      nullptr  // prev
  };
  mexFunctionCreateRootTLS();
  st.tls = emlrtRootTLSGlobal;
  emlrtClearAllocCountR2012b(&st, false, 0U, nullptr);
  emlrtEnterRtStackR2012b(&st);
  emlrtFirstTimeR2012b(emlrtRootTLSGlobal);
}

//
// Arguments    : void
// Return Type  : void
//
void fusion2_terminate()
{
  emlrtStack st{
      nullptr, // site
      nullptr, // tls
      nullptr  // prev
  };
  st.tls = emlrtRootTLSGlobal;
  emlrtLeaveRtStackR2012b(&st);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
}

//
// File trailer for _coder_fusion2_api.cpp
//
// [EOF]
//
