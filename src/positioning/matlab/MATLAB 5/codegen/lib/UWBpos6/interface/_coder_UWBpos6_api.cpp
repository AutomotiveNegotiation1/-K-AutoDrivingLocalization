//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: _coder_UWBpos6_api.cpp
//
// MATLAB Coder version            : 5.6
// C/C++ source code generated on  : 17-Oct-2023 17:18:56
//

// Include Files
#include "_coder_UWBpos6_api.h"
#include "_coder_UWBpos6_mex.h"

// Variable Definitions
emlrtCTX emlrtRootTLSGlobal{nullptr};

emlrtContext emlrtContextGlobal{
    true,                                                 // bFirstTime
    false,                                                // bInitialized
    131642U,                                              // fVersionInfo
    nullptr,                                              // fErrorFunction
    "UWBpos6",                                            // fFunctionName
    nullptr,                                              // fRTCallStack
    false,                                                // bDebugMode
    {2045744189U, 2170104910U, 2743257031U, 4284093946U}, // fSigWrd
    nullptr                                               // fSigMem
};

// Function Declarations
static real_T (*b_emlrt_marshallIn(const emlrtStack &sp, const mxArray *RxIDin,
                                   const char_T *identifier))[6];

static real_T (*b_emlrt_marshallIn(const emlrtStack &sp, const mxArray *u,
                                   const emlrtMsgIdentifier *parentId))[6];

static void b_emlrt_marshallIn(const emlrtStack &sp, const mxArray *src,
                               const emlrtMsgIdentifier *msgId, creal_T ret[4]);

static real_T (*c_emlrt_marshallIn(const emlrtStack &sp, const mxArray *xain,
                                   const char_T *identifier))[6];

static real_T (*c_emlrt_marshallIn(const emlrtStack &sp, const mxArray *u,
                                   const emlrtMsgIdentifier *parentId))[6];

static real_T d_emlrt_marshallIn(const emlrtStack &sp, const mxArray *src,
                                 const emlrtMsgIdentifier *msgId);

static real_T (*e_emlrt_marshallIn(const emlrtStack &sp, const mxArray *src,
                                   const emlrtMsgIdentifier *msgId))[6];

static real_T emlrt_marshallIn(const emlrtStack &sp, const mxArray *Ln,
                               const char_T *identifier);

static real_T emlrt_marshallIn(const emlrtStack &sp, const mxArray *u,
                               const emlrtMsgIdentifier *parentId);

static void emlrt_marshallIn(const emlrtStack &sp, const mxArray *tag_pos_b,
                             const char_T *identifier, creal_T y[4]);

static void emlrt_marshallIn(const emlrtStack &sp, const mxArray *u,
                             const emlrtMsgIdentifier *parentId, creal_T y[4]);

static const mxArray *emlrt_marshallOut(const real_T u[21]);

static real_T (*f_emlrt_marshallIn(const emlrtStack &sp, const mxArray *src,
                                   const emlrtMsgIdentifier *msgId))[6];

// Function Definitions
//
// Arguments    : const emlrtStack &sp
//                const mxArray *RxIDin
//                const char_T *identifier
// Return Type  : real_T (*)[6]
//
static real_T (*b_emlrt_marshallIn(const emlrtStack &sp, const mxArray *RxIDin,
                                   const char_T *identifier))[6]
{
  emlrtMsgIdentifier thisId;
  real_T(*y)[6];
  thisId.fIdentifier = const_cast<const char_T *>(identifier);
  thisId.fParent = nullptr;
  thisId.bParentIsCell = false;
  y = b_emlrt_marshallIn(sp, emlrtAlias(RxIDin), &thisId);
  emlrtDestroyArray(&RxIDin);
  return y;
}

//
// Arguments    : const emlrtStack &sp
//                const mxArray *u
//                const emlrtMsgIdentifier *parentId
// Return Type  : real_T (*)[6]
//
static real_T (*b_emlrt_marshallIn(const emlrtStack &sp, const mxArray *u,
                                   const emlrtMsgIdentifier *parentId))[6]
{
  real_T(*y)[6];
  y = e_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

//
// Arguments    : const emlrtStack &sp
//                const mxArray *src
//                const emlrtMsgIdentifier *msgId
//                creal_T ret[4]
// Return Type  : void
//
static void b_emlrt_marshallIn(const emlrtStack &sp, const mxArray *src,
                               const emlrtMsgIdentifier *msgId, creal_T ret[4])
{
  static const int32_T dims[2]{1, 4};
  emlrtCheckBuiltInR2012b((emlrtConstCTX)&sp, msgId, src, "double", true, 2U,
                          (const void *)&dims[0]);
  emlrtImportArrayR2015b((emlrtConstCTX)&sp, src, &ret[0], 8, true);
  emlrtDestroyArray(&src);
}

//
// Arguments    : const emlrtStack &sp
//                const mxArray *xain
//                const char_T *identifier
// Return Type  : real_T (*)[6]
//
static real_T (*c_emlrt_marshallIn(const emlrtStack &sp, const mxArray *xain,
                                   const char_T *identifier))[6]
{
  emlrtMsgIdentifier thisId;
  real_T(*y)[6];
  thisId.fIdentifier = const_cast<const char_T *>(identifier);
  thisId.fParent = nullptr;
  thisId.bParentIsCell = false;
  y = c_emlrt_marshallIn(sp, emlrtAlias(xain), &thisId);
  emlrtDestroyArray(&xain);
  return y;
}

//
// Arguments    : const emlrtStack &sp
//                const mxArray *u
//                const emlrtMsgIdentifier *parentId
// Return Type  : real_T (*)[6]
//
static real_T (*c_emlrt_marshallIn(const emlrtStack &sp, const mxArray *u,
                                   const emlrtMsgIdentifier *parentId))[6]
{
  real_T(*y)[6];
  y = f_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

//
// Arguments    : const emlrtStack &sp
//                const mxArray *src
//                const emlrtMsgIdentifier *msgId
// Return Type  : real_T
//
static real_T d_emlrt_marshallIn(const emlrtStack &sp, const mxArray *src,
                                 const emlrtMsgIdentifier *msgId)
{
  static const int32_T dims{0};
  real_T ret;
  emlrtCheckBuiltInR2012b((emlrtConstCTX)&sp, msgId, src, "double", false, 0U,
                          (const void *)&dims);
  ret = *static_cast<real_T *>(emlrtMxGetData(src));
  emlrtDestroyArray(&src);
  return ret;
}

//
// Arguments    : const emlrtStack &sp
//                const mxArray *src
//                const emlrtMsgIdentifier *msgId
// Return Type  : real_T (*)[6]
//
static real_T (*e_emlrt_marshallIn(const emlrtStack &sp, const mxArray *src,
                                   const emlrtMsgIdentifier *msgId))[6]
{
  static const int32_T dims{6};
  real_T(*ret)[6];
  int32_T i;
  boolean_T b{false};
  emlrtCheckVsBuiltInR2012b((emlrtConstCTX)&sp, msgId, src, "double", false, 1U,
                            (const void *)&dims, &b, &i);
  ret = (real_T(*)[6])emlrtMxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}

//
// Arguments    : const emlrtStack &sp
//                const mxArray *Ln
//                const char_T *identifier
// Return Type  : real_T
//
static real_T emlrt_marshallIn(const emlrtStack &sp, const mxArray *Ln,
                               const char_T *identifier)
{
  emlrtMsgIdentifier thisId;
  real_T y;
  thisId.fIdentifier = const_cast<const char_T *>(identifier);
  thisId.fParent = nullptr;
  thisId.bParentIsCell = false;
  y = emlrt_marshallIn(sp, emlrtAlias(Ln), &thisId);
  emlrtDestroyArray(&Ln);
  return y;
}

//
// Arguments    : const emlrtStack &sp
//                const mxArray *u
//                const emlrtMsgIdentifier *parentId
// Return Type  : real_T
//
static real_T emlrt_marshallIn(const emlrtStack &sp, const mxArray *u,
                               const emlrtMsgIdentifier *parentId)
{
  real_T y;
  y = d_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

//
// Arguments    : const emlrtStack &sp
//                const mxArray *tag_pos_b
//                const char_T *identifier
//                creal_T y[4]
// Return Type  : void
//
static void emlrt_marshallIn(const emlrtStack &sp, const mxArray *tag_pos_b,
                             const char_T *identifier, creal_T y[4])
{
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = const_cast<const char_T *>(identifier);
  thisId.fParent = nullptr;
  thisId.bParentIsCell = false;
  emlrt_marshallIn(sp, emlrtAlias(tag_pos_b), &thisId, y);
  emlrtDestroyArray(&tag_pos_b);
}

//
// Arguments    : const emlrtStack &sp
//                const mxArray *u
//                const emlrtMsgIdentifier *parentId
//                creal_T y[4]
// Return Type  : void
//
static void emlrt_marshallIn(const emlrtStack &sp, const mxArray *u,
                             const emlrtMsgIdentifier *parentId, creal_T y[4])
{
  b_emlrt_marshallIn(sp, emlrtAlias(u), parentId, y);
  emlrtDestroyArray(&u);
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
// Arguments    : const emlrtStack &sp
//                const mxArray *src
//                const emlrtMsgIdentifier *msgId
// Return Type  : real_T (*)[6]
//
static real_T (*f_emlrt_marshallIn(const emlrtStack &sp, const mxArray *src,
                                   const emlrtMsgIdentifier *msgId))[6]
{
  static const int32_T dims[2]{1, 6};
  real_T(*ret)[6];
  int32_T iv[2];
  boolean_T bv[2]{false, false};
  emlrtCheckVsBuiltInR2012b((emlrtConstCTX)&sp, msgId, src, "double", false, 2U,
                            (const void *)&dims[0], &bv[0], &iv[0]);
  ret = (real_T(*)[6])emlrtMxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
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
  Ln = emlrt_marshallIn(st, emlrtAliasP(prhs[0]), "Ln");
  Lp = emlrt_marshallIn(st, emlrtAliasP(prhs[1]), "Lp");
  LnC = emlrt_marshallIn(st, emlrtAliasP(prhs[2]), "LnC");
  TagNum = emlrt_marshallIn(st, emlrtAliasP(prhs[3]), "TagNum");
  Nanchor = emlrt_marshallIn(st, emlrtAliasP(prhs[4]), "Nanchor");
  RxIDin = b_emlrt_marshallIn(st, emlrtAlias(prhs[5]), "RxIDin");
  RxDistin = b_emlrt_marshallIn(st, emlrtAlias(prhs[6]), "RxDistin");
  s_time = emlrt_marshallIn(st, emlrtAliasP(prhs[7]), "s_time");
  emlrt_marshallIn(st, emlrtAliasP(prhs[8]), "tag_pos_b", tag_pos_b);
  xain = c_emlrt_marshallIn(st, emlrtAlias(prhs[9]), "xain");
  yain = c_emlrt_marshallIn(st, emlrtAlias(prhs[10]), "yain");
  emlrt_marshallIn(st, emlrtAliasP(prhs[11]), "prevTagPos", prevTagPos);
  prevTagHeading =
      emlrt_marshallIn(st, emlrtAliasP(prhs[12]), "prevTagHeading");
  // Invoke the target function
  UWBpos6(Ln, Lp, LnC, TagNum, Nanchor, *RxIDin, *RxDistin, s_time, tag_pos_b,
          *xain, *yain, prevTagPos, prevTagHeading, *UWBout);
  // Marshall function outputs
  *plhs = emlrt_marshallOut(*UWBout);
}

//
// Arguments    : void
// Return Type  : void
//
void UWBpos6_atexit()
{
  emlrtStack st{
      nullptr, // site
      nullptr, // tls
      nullptr  // prev
  };
  mexFunctionCreateRootTLS();
  st.tls = emlrtRootTLSGlobal;
  emlrtEnterRtStackR2012b(&st);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
  UWBpos6_xil_terminate();
  UWBpos6_xil_shutdown();
  emlrtExitTimeCleanup(&emlrtContextGlobal);
}

//
// Arguments    : void
// Return Type  : void
//
void UWBpos6_initialize()
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
void UWBpos6_terminate()
{
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
}

//
// File trailer for _coder_UWBpos6_api.cpp
//
// [EOF]
//
