/*
 * _coder_UWBpos_api.c
 *
 * Code generation for function 'UWBpos'
 *
 */

/* Include files */
#include "_coder_UWBpos_api.h"
#include "_coder_UWBpos_mex.h"

/* Variable Definitions */
emlrtCTX emlrtRootTLSGlobal = NULL;

emlrtContext emlrtContextGlobal = {
    true,                                                 /* bFirstTime */
    false,                                                /* bInitialized */
    131627U,                                              /* fVersionInfo */
    NULL,                                                 /* fErrorFunction */
    "UWBpos",                                             /* fFunctionName */
    NULL,                                                 /* fRTCallStack */
    false,                                                /* bDebugMode */
    {2045744189U, 2170104910U, 2743257031U, 4284093946U}, /* fSigWrd */
    NULL                                                  /* fSigMem */
};

/* Function Declarations */
static real_T b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                 const emlrtMsgIdentifier *parentId);

static const mxArray *b_emlrt_marshallOut(const real_T u);

static void c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *RxID,
                               const char_T *identifier, real_T **y_data,
                               int32_T y_size[2]);

static void d_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                               const emlrtMsgIdentifier *parentId,
                               real_T **y_data, int32_T y_size[2]);

static void e_emlrt_marshallIn(const emlrtStack *sp, const mxArray *RxDist,
                               const char_T *identifier, real_T **y_data,
                               int32_T *y_size);

static real_T emlrt_marshallIn(const emlrtStack *sp, const mxArray *Ln,
                               const char_T *identifier);

static const mxArray *emlrt_marshallOut(const emlrtStack *sp,
                                        const emxArray_creal_T *u);

static void emxFree_creal_T(const emlrtStack *sp, emxArray_creal_T **pEmxArray);

static void emxInit_creal_T(const emlrtStack *sp, emxArray_creal_T **pEmxArray);

static void f_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                               const emlrtMsgIdentifier *parentId,
                               real_T **y_data, int32_T *y_size);

static void g_emlrt_marshallIn(const emlrtStack *sp, const mxArray *tag_pos_b,
                               const char_T *identifier, creal_T y[4]);

static void h_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                               const emlrtMsgIdentifier *parentId,
                               creal_T y[4]);

static real_T (*i_emlrt_marshallIn(const emlrtStack *sp, const mxArray *xa,
                                   const char_T *identifier))[6];

static real_T (*j_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                   const emlrtMsgIdentifier *parentId))[6];

static real_T k_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                 const emlrtMsgIdentifier *msgId);

static void l_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                               const emlrtMsgIdentifier *msgId,
                               real_T **ret_data, int32_T ret_size[2]);

static void m_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                               const emlrtMsgIdentifier *msgId,
                               real_T **ret_data, int32_T *ret_size);

static void n_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                               const emlrtMsgIdentifier *msgId, creal_T ret[4]);

static real_T (*o_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                   const emlrtMsgIdentifier *msgId))[6];

/* Function Definitions */
static real_T b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                 const emlrtMsgIdentifier *parentId)
{
  real_T y;
  y = k_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

static const mxArray *b_emlrt_marshallOut(const real_T u)
{
  const mxArray *m;
  const mxArray *y;
  y = NULL;
  m = emlrtCreateDoubleScalar(u);
  emlrtAssign(&y, m);
  return y;
}

static void c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *RxID,
                               const char_T *identifier, real_T **y_data,
                               int32_T y_size[2])
{
  emlrtMsgIdentifier thisId;
  real_T *r;
  thisId.fIdentifier = (const char_T *)identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  d_emlrt_marshallIn(sp, emlrtAlias(RxID), &thisId, &r, y_size);
  *y_data = r;
  emlrtDestroyArray(&RxID);
}

static void d_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                               const emlrtMsgIdentifier *parentId,
                               real_T **y_data, int32_T y_size[2])
{
  real_T *r;
  l_emlrt_marshallIn(sp, emlrtAlias(u), parentId, &r, y_size);
  *y_data = r;
  emlrtDestroyArray(&u);
}

static void e_emlrt_marshallIn(const emlrtStack *sp, const mxArray *RxDist,
                               const char_T *identifier, real_T **y_data,
                               int32_T *y_size)
{
  emlrtMsgIdentifier thisId;
  real_T *r;
  int32_T i;
  thisId.fIdentifier = (const char_T *)identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  f_emlrt_marshallIn(sp, emlrtAlias(RxDist), &thisId, &r, &i);
  *y_size = i;
  *y_data = r;
  emlrtDestroyArray(&RxDist);
}

static real_T emlrt_marshallIn(const emlrtStack *sp, const mxArray *Ln,
                               const char_T *identifier)
{
  emlrtMsgIdentifier thisId;
  real_T y;
  thisId.fIdentifier = (const char_T *)identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  y = b_emlrt_marshallIn(sp, emlrtAlias(Ln), &thisId);
  emlrtDestroyArray(&Ln);
  return y;
}

static const mxArray *emlrt_marshallOut(const emlrtStack *sp,
                                        const emxArray_creal_T *u)
{
  const mxArray *m;
  const mxArray *y;
  const creal_T *u_data;
  int32_T iv[2];
  u_data = u->data;
  y = NULL;
  iv[0] = 1;
  iv[1] = u->size[1];
  m = emlrtCreateNumericArray(2, &iv[0], mxDOUBLE_CLASS, mxCOMPLEX);
  emlrtExportNumericArrayR2013b((emlrtConstCTX)sp, m, (const void *)&u_data[0],
                                8);
  emlrtAssign(&y, m);
  return y;
}

static void emxFree_creal_T(const emlrtStack *sp, emxArray_creal_T **pEmxArray)
{
  if (*pEmxArray != (emxArray_creal_T *)NULL) {
    if (((*pEmxArray)->data != (creal_T *)NULL) && (*pEmxArray)->canFreeData) {
      emlrtFreeMex((*pEmxArray)->data);
    }
    emlrtFreeMex((*pEmxArray)->size);
    emlrtRemoveHeapReference((emlrtCTX)sp, (void *)pEmxArray);
    emlrtFreeEmxArray(*pEmxArray);
    *pEmxArray = (emxArray_creal_T *)NULL;
  }
}

static void emxInit_creal_T(const emlrtStack *sp, emxArray_creal_T **pEmxArray)
{
  emxArray_creal_T *emxArray;
  int32_T i;
  *pEmxArray =
      (emxArray_creal_T *)emlrtMallocEmxArray(sizeof(emxArray_creal_T));
  emlrtPushHeapReferenceStackEmxArray((emlrtCTX)sp, true, (void *)pEmxArray,
                                      (void *)&emxFree_creal_T, NULL, NULL,
                                      NULL);
  emxArray = *pEmxArray;
  emxArray->data = (creal_T *)NULL;
  emxArray->numDimensions = 2;
  emxArray->size = (int32_T *)emlrtMallocMex(sizeof(int32_T) * 2U);
  emxArray->allocatedSize = 0;
  emxArray->canFreeData = true;
  for (i = 0; i < 2; i++) {
    emxArray->size[i] = 0;
  }
}

static void f_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                               const emlrtMsgIdentifier *parentId,
                               real_T **y_data, int32_T *y_size)
{
  real_T *r;
  int32_T i;
  m_emlrt_marshallIn(sp, emlrtAlias(u), parentId, &r, &i);
  *y_size = i;
  *y_data = r;
  emlrtDestroyArray(&u);
}

static void g_emlrt_marshallIn(const emlrtStack *sp, const mxArray *tag_pos_b,
                               const char_T *identifier, creal_T y[4])
{
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = (const char_T *)identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  h_emlrt_marshallIn(sp, emlrtAlias(tag_pos_b), &thisId, y);
  emlrtDestroyArray(&tag_pos_b);
}

static void h_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                               const emlrtMsgIdentifier *parentId, creal_T y[4])
{
  n_emlrt_marshallIn(sp, emlrtAlias(u), parentId, y);
  emlrtDestroyArray(&u);
}

static real_T (*i_emlrt_marshallIn(const emlrtStack *sp, const mxArray *xa,
                                   const char_T *identifier))[6]
{
  emlrtMsgIdentifier thisId;
  real_T(*y)[6];
  thisId.fIdentifier = (const char_T *)identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  y = j_emlrt_marshallIn(sp, emlrtAlias(xa), &thisId);
  emlrtDestroyArray(&xa);
  return y;
}

static real_T (*j_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
                                   const emlrtMsgIdentifier *parentId))[6]
{
  real_T(*y)[6];
  y = o_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

static real_T k_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
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

static void l_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                               const emlrtMsgIdentifier *msgId,
                               real_T **ret_data, int32_T ret_size[2])
{
  static const int32_T dims[2] = {1, 4};
  int32_T iv[2];
  const boolean_T bv[2] = {false, true};
  emlrtCheckVsBuiltInR2012b((emlrtConstCTX)sp, msgId, src, "double", false, 2U,
                            (const void *)&dims[0], &bv[0], &iv[0]);
  ret_size[0] = iv[0];
  ret_size[1] = iv[1];
  *ret_data = (real_T *)emlrtMxGetData(src);
  emlrtDestroyArray(&src);
}

static void m_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                               const emlrtMsgIdentifier *msgId,
                               real_T **ret_data, int32_T *ret_size)
{
  static const int32_T dims = 4;
  const boolean_T b = true;
  emlrtCheckVsBuiltInR2012b((emlrtConstCTX)sp, msgId, src, "double", false, 1U,
                            (const void *)&dims, &b, ret_size);
  *ret_data = (real_T *)emlrtMxGetData(src);
  emlrtDestroyArray(&src);
}

static void n_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                               const emlrtMsgIdentifier *msgId, creal_T ret[4])
{
  static const int32_T dims[2] = {1, 4};
  emlrtCheckBuiltInR2012b((emlrtConstCTX)sp, msgId, src, "double", true, 2U,
                          (const void *)&dims[0]);
  emlrtImportArrayR2015b((emlrtConstCTX)sp, src, &ret[0], 8, true);
  emlrtDestroyArray(&src);
}

static real_T (*o_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
                                   const emlrtMsgIdentifier *msgId))[6]
{
  static const int32_T dims[2] = {1, 6};
  real_T(*ret)[6];
  emlrtCheckBuiltInR2012b((emlrtConstCTX)sp, msgId, src, "double", false, 2U,
                          (const void *)&dims[0]);
  ret = (real_T(*)[6])emlrtMxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}

void UWBpos_api(const mxArray *const prhs[10], int32_T nlhs,
                const mxArray *plhs[4])
{
  emlrtStack st = {
      NULL, /* site */
      NULL, /* tls */
      NULL  /* prev */
  };
  emxArray_creal_T *tag_pos_est;
  emxArray_creal_T *tag_pos_est_aver;
  creal_T tag_pos_b[4];
  real_T(*xa)[6];
  real_T(*ya)[6];
  real_T(*RxDist_data)[4];
  real_T(*RxID_data)[4];
  real_T Ln;
  real_T Lp;
  real_T Nanchor;
  real_T TagNum;
  real_T heading_est;
  real_T headingest_a_aver_v;
  real_T s_time;
  int32_T RxID_size[2];
  int32_T RxDist_size;
  st.tls = emlrtRootTLSGlobal;
  emlrtHeapReferenceStackEnterFcnR2012b(&st);
  /* Marshall function inputs */
  Ln = emlrt_marshallIn(&st, emlrtAliasP(prhs[0]), "Ln");
  Lp = emlrt_marshallIn(&st, emlrtAliasP(prhs[1]), "Lp");
  TagNum = emlrt_marshallIn(&st, emlrtAliasP(prhs[2]), "TagNum");
  Nanchor = emlrt_marshallIn(&st, emlrtAliasP(prhs[3]), "Nanchor");
  c_emlrt_marshallIn(&st, emlrtAlias(prhs[4]), "RxID", (real_T **)&RxID_data,
                     RxID_size);
  e_emlrt_marshallIn(&st, emlrtAlias(prhs[5]), "RxDist",
                     (real_T **)&RxDist_data, &RxDist_size);
  s_time = emlrt_marshallIn(&st, emlrtAliasP(prhs[6]), "s_time");
  g_emlrt_marshallIn(&st, emlrtAliasP(prhs[7]), "tag_pos_b", tag_pos_b);
  xa = i_emlrt_marshallIn(&st, emlrtAlias(prhs[8]), "xa");
  ya = i_emlrt_marshallIn(&st, emlrtAlias(prhs[9]), "ya");
  /* Invoke the target function */
  emxInit_creal_T(&st, &tag_pos_est);
  emxInit_creal_T(&st, &tag_pos_est_aver);
  UWBpos(Ln, Lp, TagNum, Nanchor, *RxID_data, RxID_size, *RxDist_data,
         *(int32_T(*)[1]) & RxDist_size, s_time, tag_pos_b, *xa, *ya,
         tag_pos_est, &heading_est, tag_pos_est_aver, &headingest_a_aver_v);
  /* Marshall function outputs */
  plhs[0] = emlrt_marshallOut(&st, tag_pos_est);
  emxFree_creal_T(&st, &tag_pos_est);
  if (nlhs > 1) {
    plhs[1] = b_emlrt_marshallOut(heading_est);
  }
  if (nlhs > 2) {
    plhs[2] = emlrt_marshallOut(&st, tag_pos_est_aver);
  }
  emxFree_creal_T(&st, &tag_pos_est_aver);
  if (nlhs > 3) {
    plhs[3] = b_emlrt_marshallOut(headingest_a_aver_v);
  }
  emlrtHeapReferenceStackLeaveFcnR2012b(&st);
}

void UWBpos_atexit(void)
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
  UWBpos_xil_terminate();
  UWBpos_xil_shutdown();
  emlrtExitTimeCleanup(&emlrtContextGlobal);
}

void UWBpos_initialize(void)
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

void UWBpos_terminate(void)
{
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
}

/* End of code generation (_coder_UWBpos_api.c) */
