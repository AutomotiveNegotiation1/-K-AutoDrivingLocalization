/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: _coder_UWBpos_info.c
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 01-Aug-2023 14:35:53
 */

/* Include Files */
#include "_coder_UWBpos_info.h"
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
  const char_T *data[8] = {
      "789ced59cb6ed340147551a12da83405d10d3bd650276988d4058ba6246e42134262fa08"
      "428a634fe269edb1e3479a16895fe80ff0012cf901162c10fc0c2b36"
      "48a812499d496c4b262d53a675eabb995c1d7bcec9bdf6c9e88699ca17a71886b9cb38f1"
      "2be1acf3833c36586f30def0e3538375ce97e3b8c94c7beec3f8f160",
      "15356481aee5244850c1f04e4953211290c51fea803180a9291d209d224da8001eaaa0ea"
      "4e4afd4ccdb9a061d287fa9fd76520ee576d95316473a4507127c37a"
      "7c0cf8bed367ac4736a01e311ffe26fb96953515b00d5540ec8166ecb3ba66420b6a8835"
      "0d719840d46255c15284062b282dcd8096acb2afb7333d7c5975e9d6",
      "0975df1ea31be31cb0f2085a656d503ccc5f27e4bf15c8ef209266371430e2fb44c8970f"
      "e4f3e2447d1a15ebb457fd1857af8533eaf7afa3eb674fd7aaf15da2"
      "c9b7c1fe98a1c987e3b2f8ba01fb9df5f95b0ae08bf9f054aaddac5676e395b4522bd8dd"
      "a35589db5ccd8c7494c7f08cd3c104e4b4f60ffb7b4ceabb77c6e8c7",
      "387fa0ad2151ee4958f1f0d709f969fb6e2190cf8b13f5cb552cdc305a3ef1a843d777ef"
      "9f7c5da4c98763d27d57ecee4a52216e69e84007c02c75b276bb988d"
      "7cf7aaf82ee973b038463fc67b128ab662415e68f5647000f974d40975d0f6dfb22f677c"
      "d7619cb46fdea22dabb4fce219e573ef8cfdee214d3e1c93eebf472f",
      "ed527b85af893b9c65ca2f8a5be936dc7c3e39fe7b5de70c41f3a2980fcf230b18fa70cc"
      "70697e4bdaa78d403e2f4ed4a761b1e8f9ecfb932f547d16c775e1a3"
      "e5b3ad54b9933c2ca9e53da8644405ee1d365fc5d7239fbd6c9fad13ea9e09d4ed20bd23"
      "5acf2f92a1f5d55c209f17273dc7f68bd46f0c2d1f9835e99e5f97be",
      "cd2dd0e4c331e9beba8104ad218166822f259ea264a256d8592b4fd0dc36acef6f34af3d"
      "1f5f34afbd58be685eeb4434af3ddffe61f5db684efb6f7cd19cf662"
      "f9a239ad13d19cf67cfb87757e407ace9df7e57edd18772cbf029a108124135ebfdd0ce4"
      "f3e217f0fb38289673d2a5e50f1f8ee9faede39fb92734f97084d56f",
      "1f04f0c57c786657eae40ab523219e4ac1ed5a3109b700c74c8edf86fd3da6e5bbbc6c00"
      "e09a3044befbb7f982bb58747df7deefcfd1ff64ff918fd639574f43"
      "94344025cde5124d4954395db65213305ff803cb19161a",
      ""};
  nameCaptureInfo = NULL;
  emlrtNameCaptureMxArrayR2016a(&data[0], 12816U, &nameCaptureInfo);
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
  emlrtSetField(xEntryPoints, 0, "Name", emlrtMxCreateString("UWBpos"));
  emlrtSetField(xEntryPoints, 0, "NumberOfInputs",
                emlrtMxCreateDoubleScalar(10.0));
  emlrtSetField(xEntryPoints, 0, "NumberOfOutputs",
                emlrtMxCreateDoubleScalar(4.0));
  emlrtSetField(xEntryPoints, 0, "ConstantInputs", xInputs);
  emlrtSetField(xEntryPoints, 0, "FullPath",
                emlrtMxCreateString("/home/bman/work/position/src/positioning/"
                                    "matlab/algorithm/UWBpos.m"));
  emlrtSetField(xEntryPoints, 0, "TimeStamp",
                emlrtMxCreateDoubleScalar(739099.4998263889));
  xResult =
      emlrtCreateStructMatrix(1, 1, 7, (const char_T **)&propFieldName[0]);
  emlrtSetField(xResult, 0, "Version",
                emlrtMxCreateString("9.14.0.2206163 (R2023a)"));
  emlrtSetField(xResult, 0, "ResolvedFunctions",
                (mxArray *)emlrtMexFcnResolvedFunctionsInfo());
  emlrtSetField(xResult, 0, "Checksum",
                emlrtMxCreateString("T7UvfNeqRgIpqM4Qo4GI0G"));
  emlrtSetField(xResult, 0, "EntryPoints", xEntryPoints);
  return xResult;
}

/*
 * File trailer for _coder_UWBpos_info.c
 *
 * [EOF]
 */
