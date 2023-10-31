/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: _coder_IMUpos_info.c
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 20-Sep-2023 17:05:14
 */

/* Include Files */
#include "_coder_IMUpos_info.h"
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
  const char_T *data[6] = {
      "789ced58cd8ed25018bd1834b3991137b3766f621ac0512726a6c50e320cce5868028a81"
      "da5e86daf6de4e7fc68197f01558fa0cc607d02798b832be870ba13f"
      "94dee4a64a274508dfe6e3e4b43da7df474e6e0a72b5460e00b007fc32f7fdbe1be042d0"
      "6f8178917c2ee8790287753b607609fe53d0658c1c78e5f80049069c",
      "dfa960434512725a2313020bda58bf848ac70c541db654033617c1ab19328e16a8399851"
      "b3df952194b5a66b006b68470ef545309fc784f2be79102fda3c0e29"
      "f32810fc5bfe1d7fd8ad210563aba763b97b866dd5513152d139ab9ff78a4cb1c43c65ca"
      "dd06db3a61b9fbe56ead219ad87e68c4fd9a29fdde4df01bf2bcab43",
      "ab3ab2b0682a9203e7fafd94fa77a8fa3ea360f7bd0ea3f7fd9c52af42d58bf3ffbe1f62"
      "42c1a292e614ce37c937d9a3eb77bcfeebe78f5c967ae8fa6b254bbd"
      "b056a5774579dedffeeff6297a0582afd5eb9d63c13a821d56fb70f94418e3d281528d7c"
      "9c25e824f900149cd5f32794fbffd77ceda7f4bb43e0c8afcf78a9c1",
      "caf2aaf2749252ef19552fce2f99a7d3c9f81bd9d41cfdfeed3997a55e589b9ea3cc1b1b"
      "39e38376456b768e47230e6b92d4d8e6e8ca7234eddeef25f80d792f"
      "355af8b53b3d825968ea13c47df453fac8fabcca53f5e2fc92f9ba38296f659b9ab3852f"
      "bfb7e75570f3397b224a83f2d07405a6ce0d5c87d335e142e5b639bb",
      "aedf03c8ef35a4df90f7d2a32ee986847a45b0bef9ca52f5e2fc92f91a4e285cd3c6e6eb"
      "83fcf5365fc1cde7eb295b851fdb8fda8f65e1c5e94b5614e5d268cc"
      "af7fbefe0110d0442f",
      ""};
  nameCaptureInfo = NULL;
  emlrtNameCaptureMxArrayR2016a(&data[0], 5768U, &nameCaptureInfo);
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
      emlrtCreateStructMatrix(1, 1, 6, (const char_T **)&epFieldName[0]);
  xInputs = emlrtCreateLogicalMatrix(1, 10);
  emlrtSetField(xEntryPoints, 0, "Name", emlrtMxCreateString("IMUpos"));
  emlrtSetField(xEntryPoints, 0, "NumberOfInputs",
                emlrtMxCreateDoubleScalar(10.0));
  emlrtSetField(xEntryPoints, 0, "NumberOfOutputs",
                emlrtMxCreateDoubleScalar(9.0));
  emlrtSetField(xEntryPoints, 0, "ConstantInputs", xInputs);
  emlrtSetField(
      xEntryPoints, 0, "FullPath",
      emlrtMxCreateString(
          "E:\\Indoor_loc\\PositioningAlg_20230904\\MATLAB 4\\IMUpos.m"));
  emlrtSetField(xEntryPoints, 0, "TimeStamp",
                emlrtMxCreateDoubleScalar(739134.799212963));
  xResult =
      emlrtCreateStructMatrix(1, 1, 7, (const char_T **)&propFieldName[0]);
  emlrtSetField(xResult, 0, "Version",
                emlrtMxCreateString("9.14.0.2306882 (R2023a) Update 4"));
  emlrtSetField(xResult, 0, "ResolvedFunctions",
                (mxArray *)emlrtMexFcnResolvedFunctionsInfo());
  emlrtSetField(xResult, 0, "Checksum",
                emlrtMxCreateString("BDPRIPpZ8xVqHjCX88YybE"));
  emlrtSetField(xResult, 0, "EntryPoints", xEntryPoints);
  return xResult;
}

/*
 * File trailer for _coder_IMUpos_info.c
 *
 * [EOF]
 */
