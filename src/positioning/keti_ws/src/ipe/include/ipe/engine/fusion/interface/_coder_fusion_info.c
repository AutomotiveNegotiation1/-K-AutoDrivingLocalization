/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: _coder_fusion_info.c
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 04-Oct-2023 13:17:07
 */

/* Include Files */
#include "_coder_fusion_info.h"
#include "emlrt.h"
#include "tmwtypes.h"

/* Function Declarations */
static const mxArray *emlrtMexFcnResolvedFunctionsInfo(void);

/* Function Definitions */
/*
 * Arguments    : void
 * Return Type  : const mxArray *
 */
static const mxArray *emlrtMexFcnResolvedFunctionsInfo(void)
{
  const mxArray *nameCaptureInfo;
  const char_T *data[5] = {
      "789ced56cb4ec240141d7cc5101fb8f107dc935a3026ae0441e4a1220f35b1062b0c5099"
      "ce401f82fe820b172edcba74a7bfe38fb8166887d29a498d3528c4bb"
      "b93d3d9d39a7b7e969812fb9ef03002c01a3b815a32f9a3860f629602f27ef33fbb403d3"
      "9a0533b67594bf377b99600d763403605186839515224b58c45ae1a6",
      "0981025582ae61a5cf5425040b920cf3c3e0a087e4dd216a007a54ef78a70ecb8dbc2e03"
      "a5ae5a0ed13018cce38971bf335f9cc736631e01077f163fdfd9128a"
      "2a5454e116eaea95886b428cb43122624515b2449534896009d74a11542bf11c1f5ae7b8"
      "b050d5d5eee9a04cfd763cfa5d71f14bf9b88ea0522047baa8410577",
      "2d98e7a98f0b8f3ee6983e0ca642f44b042dbd578f7a69a69e9dffee73fa34afa0dcdfcf"
      "6d5ecb5ff4efecd6f5f3fd5e7b7ff38d52af937ef08f528fd66fe979"
      "7def56197a01079f298ad570bda9e7b874b4aa6b51d4c8b5a4a8e523eba2e3e60330f0a8"
      "f67f62acffab79dbf4e8d7f93d74faa57c3f3fd22292455ce2c1efe5",
      "ecb347bd0453cfce7bca593aa76ec64e6abee65feefef315fc7cbe1e4612b07dba71ba59"
      "cec50ef722c5623974731bffcfd771cd57bf8b5fca770d19a9c11b78"
      "5cf335c6d4b3f3df7d3ed69c8c1fd849cdd7b5c7f6c228f5684d7abe461a7c22d9ca9e64"
      "4ea4e354aade6ec56399abddf1cfd70f86a91106",
      ""};
  nameCaptureInfo = NULL;
  emlrtNameCaptureMxArrayR2016a(&data[0], 4424U, &nameCaptureInfo);
  return nameCaptureInfo;
}

/*
 * Arguments    : void
 * Return Type  : mxArray *
 */
mxArray *emlrtMexFcnProperties(void)
{
  mxArray *xEntryPoints;
  mxArray *xInputs;
  mxArray *xResult;
  const char_T *propFieldName[7] = {
      "Version",      "ResolvedFunctions", "Checksum",    "EntryPoints",
      "CoverageInfo", "IsPolymorphic",     "PropertyList"};
  const char_T *epFieldName[6] = {
      "Name",           "NumberOfInputs", "NumberOfOutputs",
      "ConstantInputs", "FullPath",       "TimeStamp"};
  xEntryPoints =
      emlrtCreateStructMatrix(1, 2, 6, (const char_T **)&epFieldName[0]);
  xInputs = emlrtCreateLogicalMatrix(1, 16);
  emlrtSetField(xEntryPoints, 0, "Name", emlrtMxCreateString("fusion"));
  emlrtSetField(xEntryPoints, 0, "NumberOfInputs",
                emlrtMxCreateDoubleScalar(16.0));
  emlrtSetField(xEntryPoints, 0, "NumberOfOutputs",
                emlrtMxCreateDoubleScalar(6.0));
  emlrtSetField(xEntryPoints, 0, "ConstantInputs", xInputs);
  emlrtSetField(
      xEntryPoints, 0, "FullPath",
      emlrtMxCreateString("C:\\Users\\zeusjang\\Downloads\\Positioning_Alg_"
                          "20231004\\fusion.m"));
  emlrtSetField(xEntryPoints, 0, "TimeStamp",
                emlrtMxCreateDoubleScalar(739163.4611111111));
  xInputs = emlrtCreateLogicalMatrix(1, 4);
  emlrtSetField(xEntryPoints, 1, "Name", emlrtMxCreateString("PosKalman2"));
  emlrtSetField(xEntryPoints, 1, "NumberOfInputs",
                emlrtMxCreateDoubleScalar(4.0));
  emlrtSetField(xEntryPoints, 1, "NumberOfOutputs",
                emlrtMxCreateDoubleScalar(1.0));
  emlrtSetField(xEntryPoints, 1, "ConstantInputs", xInputs);
  emlrtSetField(
      xEntryPoints, 1, "FullPath",
      emlrtMxCreateString("C:\\Users\\zeusjang\\Downloads\\Positioning_Alg_"
                          "20231004\\PosKalman2.m"));
  emlrtSetField(xEntryPoints, 1, "TimeStamp",
                emlrtMxCreateDoubleScalar(739163.46114583337));
  xResult =
      emlrtCreateStructMatrix(1, 1, 7, (const char_T **)&propFieldName[0]);
  emlrtSetField(xResult, 0, "Version",
                emlrtMxCreateString("9.14.0.2306882 (R2023a) Update 4"));
  emlrtSetField(xResult, 0, "ResolvedFunctions",
                (mxArray *)emlrtMexFcnResolvedFunctionsInfo());
  emlrtSetField(xResult, 0, "Checksum",
                emlrtMxCreateString("8ZNcjukWpl0105Onl3sY3C"));
  emlrtSetField(xResult, 0, "EntryPoints", xEntryPoints);
  return xResult;
}

/*
 * File trailer for _coder_fusion_info.c
 *
 * [EOF]
 */
