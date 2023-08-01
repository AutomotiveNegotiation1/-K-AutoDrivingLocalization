/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: _coder_UWBpos_info.c
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 01-Aug-2023 14:00:18
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
      "789ced594f73d240144f9d6a5b9d5aaa632fde3c6b032932f6e0a1d492160b5248ff619d"
      "36240bd936d9400894d619bf42bf801fc48b337a70f423f8253c7974"
      "3a239004486676a0926e21eebb2c2f3fb2bf7d6fb3bf3c1eccc4466a826198fb8c653f39"
      "6b9cb5fd903dde62dce6c527ec71c6e33b769b9974dde7e017f628e9",
      "c8040dd37290a881ce9db2ae41242253382b03c600555dad03b98d14a10a04a8815caf93"
      "6e795aa207ea382da8f5795501d249aea6318652edae50ed753af9f8"
      "84897772c07c6c61f211f2e06fd7deb18aae01b6a089883dd58d13b6ac57a10975c4560d"
      "a9e340546235d154c5029bb12f1daea8a5432ecc2d855f8423ecf66e",
      "bcf9dd45cd134779c838eef689c3c179606e206836d7e6e63f1a92ff0e96df4264bd5650"
      "817ffbb68de573e3beed5b3771edbdeb97b7b901e3f08eddef4fb7c7"
      "9cf1a37d9848f1adb3bfa648f23976537c0dcc7c833e870b18be90078f462bc55c763f9c"
      "8da9f964ad71be2cf39bcbf1ee3a327d78faad83c1f8a4e60fca791e",
      "5687eff589c3c185537d05494a937ea9ed8fab0eef60f9dcb86ffbd693b8d6c691d28d27"
      "75b23afcf0f2db3c493ec782aec352635f9693615347a76500aae9fa"
      "5aad925aa33a3c6a3a3cecf330df270e076fd2a76aaa0905b1d45c020f907dfda6f4f8cb"
      "907c07583e37eee7feb91368fdb421a5232f09d7c753b5f78f49f239",
      "16745d3e7f534b579684bcb4c79b55e5756a2756819bafa82e07ad4f81eb3f853cf80632"
      "8151eeb429c6b63e16b07c6edcb77deb248eac0e7fb8fc4a54871dfb"
      "5ff848e970299aa97367692d730cd5b8a4c2e3b3e2567895eaf0a8e9f0d190714c61e3b0"
      "906659d75c0f37b6ba9bc5f2b9713febe056c29c8d22a513d355b2f5",
      "efc2f79939927c8e055d77d791a81764508c08e9c873c445f2c9bd950ced0f8fdc79a6fd"
      "e1abf1d1fef0f5f0d1feb065b43f7cb5f983a2c3b43ffc6f7cb43f7c"
      "bd7cb43f6c19ed0f5f6dfea0f42586ad8f673dbe370e07b75e0b5950840870ccf8d6c77b"
      "583e37eef3fbd44edca2464a373e5e90d5e1a7bf13cf48f23936ae3a",
      "fc08c317f2e0f17db99e48e6cfc570340a77f3290eee009ea13a3c6ae799940e0b8a0140"
      "4fa782eaf0a07d8adec491d3e1077f3ed3ffe9ae918f543d5c8e41c4"
      "19201be31391a22c697c5931a301e853fc0517a74d6b",
      ""};
  nameCaptureInfo = NULL;
  emlrtNameCaptureMxArrayR2016a(&data[0], 13040U, &nameCaptureInfo);
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
                                    "matlab/Position_Alg_20230801/UWBpos.m"));
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
