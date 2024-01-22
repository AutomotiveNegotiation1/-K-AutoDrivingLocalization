//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: _coder_fusion2_info.cpp
//
// MATLAB Coder version            : 5.6
// C/C++ source code generated on  : 09-Nov-2023 17:48:28
//

// Include Files
#include "_coder_fusion2_info.h"
#include "emlrt.h"
#include "tmwtypes.h"

// Function Declarations
static const mxArray *emlrtMexFcnResolvedFunctionsInfo();

// Function Definitions
//
// Arguments    : void
// Return Type  : const mxArray *
//
static const mxArray *emlrtMexFcnResolvedFunctionsInfo()
{
  const mxArray *nameCaptureInfo;
  const char_T *data[10]{
      "789ced9b4d93e24418c71b5dadad75d6c52acb8317abf4a207b7580699594f9bf036bc33"
      "8138c388052134902149435e783b585659e51e3c78f0e0c5c31ebd58"
      "fa21fc045679d0efa16703990c246b172c61329bd87dc934ff24ff87e799fc68fa294028"
      "5b0c0100de04e67892318ff7afe6e1abe32bc03e9c7ac8f17ac87e3a",
      "780ddcf94ffdbbab238f640d4e3573227312bcbeb2832441e664ad361b42a040158963d8"
      "592a5d4184354182d5f549693193d26bd2f564212dfe4ef4213fa8ea"
      "1250faea2a42717d729d8f9f30eff7ce96f9a030f9083bf4cf535f243e6db02a54d4c61c"
      "eaea2527f71a49349145c475d44605a982262059907b4d4aec35a391",
      "e8e1a34824d6e8eaaaf172f4a1e4887bea32eeb736c46de9295d844a0d9dea9c0615d908"
      "c51147cb651caf63e330950ed2db225cf9fdead22f8ff5b3ebbbd6eb"
      "b97c5d556e53be1e6c19bff3b83affeef2d8fbe7cf90977ed3fcf7f7bcf4b3c66df9b97d"
      "eedec1f8851d7a81e5bab1fe50672279baab6bb438604602bd8aa3b2",
      "c167531c0033f7eafe7ee5eed065dcb8cfcfb0435f7224cf89122737a3e0f678ebb64e19"
      "ac9f5d77c55b2b4f46b582cad9ea2fdf12ce82fd73b64c65e0e4fc93"
      "f3239e49964f2896e50f67f314e1acdf397b6f43dc966e0466d2236af76fb9f4f79ab349"
      "ac9f5ddfb54eab3c99a50a2a673ff86172e0a59f3582ce596a10cd64",
      "4795b3c299f0592ed79f8c52c9c2653a389c7d86b97edb3c3ec1dc3fecd0777d7eb34576"
      "88d487d2bef8fac03177c66be9cbf55966a62076d831be12fb76df20"
      "8bf5b3ebaed6b1ab3c3df779d8c2c4e757ce26074fc97a16ec9fb3d97cbe9e639434ac53"
      "83cbf131334787f14e8670f6b638db7219ef5dc77c15afa92cb941f1",
      "bc5fd7ad34d6cfaebbe2aa911f0ba841e569e6d5af094fc1fe791ab950656d1e3f4f0caa"
      "f5dc6c46a301c715094f6f8da7a4efb59b1fe97bedd78ff4bdcc41fa"
      "5e2f76ff6798eb5f56debadd2720fd2efb79a4dfb59d1fe9779983f4bb5eecfe2f7bbf8b"
      "3da30dbec6f7deef7a6343dc969e815a56163423be98cdbfe5d2df6b",
      "cea6b07e767dd73aade56959aba072f6ef6ffe209c05fbe7ecd171b75e1cd2229a68a931"
      "3bcd1763d5141fa07d03bf3ebf6e397bb0216e4b37fc0dd41bf6cdf1"
      "12b47ee56c1aeb67d75dd4e93a4f8b4279c585dfc15f9e72f6e901f3a1977ed6083a67cb"
      "d9799b425d1651dcd9842924e88b5e241ea0fd02bf3ebf5eed1b24a0",
      "ac41c5f04f0aeae2d7257ee5ec4def1bd8f214e07d83df7e6e92f52cb8813e58896247ed"
      "ec18b61f33b3631a952a5d3d46d6b3ff1bced614415cb47638d18801"
      "10cee2ea64cb538039fbd1c7efddf7d2cf1a41e76ce1e8a2a2e44ed552e1f4749ed6ca47"
      "ed139121eb59dfef1b6cbb3f5b9b204ae6fb86ffe172ee57cede749d",
      "d6f214e8fdd9f7bf8a12ce82fd73969fd63b9d5c4443f26408a15a1aa7f45191f4c17ccf"
      "d9add7b37d05c235d2fa95b337be9e5dcf5380d7b35ffef82ee12cd8"
      "feffee6d8c5fd8a13f92b4519942e3c8f1e35e2c3e10d913d85b8fc3af9cfd17577e237"
      "2",
      ""};
  nameCaptureInfo = nullptr;
  emlrtNameCaptureMxArrayR2016a(&data[0], 18264U, &nameCaptureInfo);
  return nameCaptureInfo;
}

//
// Arguments    : void
// Return Type  : mxArray *
//
mxArray *emlrtMexFcnProperties()
{
  mxArray *xEntryPoints;
  mxArray *xInputs;
  mxArray *xResult;
  const char_T *propFieldName[7]{
      "Version",      "ResolvedFunctions", "Checksum",    "EntryPoints",
      "CoverageInfo", "IsPolymorphic",     "PropertyList"};
  const char_T *epFieldName[6]{
      "Name",           "NumberOfInputs", "NumberOfOutputs",
      "ConstantInputs", "FullPath",       "TimeStamp"};
  xEntryPoints =
      emlrtCreateStructMatrix(1, 3, 6, (const char_T **)&epFieldName[0]);
  xInputs = emlrtCreateLogicalMatrix(1, 20);
  emlrtSetField(xEntryPoints, 0, "Name", emlrtMxCreateString("fusion2"));
  emlrtSetField(xEntryPoints, 0, "NumberOfInputs",
                emlrtMxCreateDoubleScalar(20.0));
  emlrtSetField(xEntryPoints, 0, "NumberOfOutputs",
                emlrtMxCreateDoubleScalar(6.0));
  emlrtSetField(xEntryPoints, 0, "ConstantInputs", xInputs);
  emlrtSetField(
      xEntryPoints, 0, "FullPath",
      emlrtMxCreateString("C:\\Users\\zeusjang\\Downloads\\Positioning_Alg_"
                          "20231004\\fusion2.m"));
  emlrtSetField(xEntryPoints, 0, "TimeStamp",
                emlrtMxCreateDoubleScalar(739199.69199074071));
  xInputs = emlrtCreateLogicalMatrix(1, 10);
  emlrtSetField(xEntryPoints, 1, "Name", emlrtMxCreateString("IMUpos"));
  emlrtSetField(xEntryPoints, 1, "NumberOfInputs",
                emlrtMxCreateDoubleScalar(10.0));
  emlrtSetField(xEntryPoints, 1, "NumberOfOutputs",
                emlrtMxCreateDoubleScalar(9.0));
  emlrtSetField(xEntryPoints, 1, "ConstantInputs", xInputs);
  emlrtSetField(
      xEntryPoints, 1, "FullPath",
      emlrtMxCreateString("C:\\Users\\zeusjang\\Downloads\\Positioning_Alg_"
                          "20231004\\IMUpos.m"));
  emlrtSetField(xEntryPoints, 1, "TimeStamp",
                emlrtMxCreateDoubleScalar(739163.46112268523));
  xInputs = emlrtCreateLogicalMatrix(1, 13);
  emlrtSetField(xEntryPoints, 2, "Name", emlrtMxCreateString("UWBpos6"));
  emlrtSetField(xEntryPoints, 2, "NumberOfInputs",
                emlrtMxCreateDoubleScalar(13.0));
  emlrtSetField(xEntryPoints, 2, "NumberOfOutputs",
                emlrtMxCreateDoubleScalar(1.0));
  emlrtSetField(xEntryPoints, 2, "ConstantInputs", xInputs);
  emlrtSetField(
      xEntryPoints, 2, "FullPath",
      emlrtMxCreateString("C:\\Users\\zeusjang\\Downloads\\Positioning_Alg_"
                          "20231004\\UWBpos6.m"));
  emlrtSetField(xEntryPoints, 2, "TimeStamp",
                emlrtMxCreateDoubleScalar(739163.46116898151));
  xResult =
      emlrtCreateStructMatrix(1, 1, 7, (const char_T **)&propFieldName[0]);
  emlrtSetField(xResult, 0, "Version",
                emlrtMxCreateString("9.14.0.2306882 (R2023a) Update 4"));
  emlrtSetField(xResult, 0, "ResolvedFunctions",
                (mxArray *)emlrtMexFcnResolvedFunctionsInfo());
  emlrtSetField(xResult, 0, "Checksum",
                emlrtMxCreateString("LZb8nieQSYA3vwXY3N9mMH"));
  emlrtSetField(xResult, 0, "EntryPoints", xEntryPoints);
  return xResult;
}

//
// File trailer for _coder_fusion2_info.cpp
//
// [EOF]
//
