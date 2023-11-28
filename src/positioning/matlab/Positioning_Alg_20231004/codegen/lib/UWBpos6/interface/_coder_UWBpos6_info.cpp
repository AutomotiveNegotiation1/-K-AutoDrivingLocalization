//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: _coder_UWBpos6_info.cpp
//
// MATLAB Coder version            : 5.6
// C/C++ source code generated on  : 17-Oct-2023 17:18:56
//

// Include Files
#include "_coder_UWBpos6_info.h"
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
  const char_T *data[7]{
      "789ced98cf8e935014c66fcd688cce6817c6851ba32b5d68984ed376dcd10eed74ecff96"
      "4c66aca9506e290e702950dae9c2b8d2a52fe003b8f12d7c021317fa"
      "1eba96d2d2020929997698c07037b7275fe13b9c03bf1c00b162390600b807664b7a32db"
      "77e6717cbedf00f6e5d463f3fd962336d74db0653bced4bfccf72e12",
      "5538566781480970712483044ea444b5752e41204305f11a640ca5c7f1b0c509b0690d2a"
      "d348c85ba4453095a6bf737dd83d6b0e0520f7956586bc3558d4e39b"
      "cbf56e79ac07ee528fb8437f43bccdbd6a930a9495f6040e95f794c8b60fd048e411c528"
      "ed1a523895432227b21d9c673b092cb1b78b61c936799c9590927a29",
      "38f296d6ccfbee8abc4dbd00d5a2c8a97a7e499bffbb35fdddeea3f85c61d090e6e1e6fa"
      "44b8fad9f58bf6c95227a357abea74df63dece7df9ffdbc6cefefb6d"
      "487ef9fdfdf4eb8e9f7ee6ba2abfb1cbf9bcde770f5dfce20e3d9de99d94a52c8f462aa1"
      "91e3d7e56493e8169679d456f8acca03b8c47e9d3fa8cfefba9cdd5e",
      "91b7a9ebfe3aea75fb8e668036a89ccdbbfad9f535fab4a8d3b4517e71e127f8e32b673f"
      "6f379ef9e967aeb073b65a9cd038ea9108a78e478d522e7bca62a96c"
      "c4d9ab7e7ed7e5acf3bdc399b7a9e7a0fe1e22ebfe079c327d1b092a670bae7e76fda27d"
      "b2d5496f5458e7d91fdf3bd13c0b36cf59ac829303baa8417abf719e",
      "c9a24aad374c46f3ecb5e16c4be6784a2708c5eb398088b36e7db2d529c49c7dfee2f18e"
      "9f7ee60a3b674be9d39a7c54572aa57a7d9257ab69fa906f44f36ce0"
      "bf1b78fd3edb1a215cecf675ff3d230e2a672fbb4f963a85fafbecd38f8988b360f39ced"
      "8e4f18e6085391389220542a1a311c948988b341e7ace779b62f4368",
      "216d50397be9f3acb54e219e673f7c7d14711678bfef1eb8f8c51dfaaea00eaa38d2b0cc"
      "3e9b4c9df1e42164ad790495b3ff0110f1fda5",
      ""};
  nameCaptureInfo = nullptr;
  emlrtNameCaptureMxArrayR2016a(&data[0], 8584U, &nameCaptureInfo);
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
      emlrtCreateStructMatrix(1, 1, 6, (const char_T **)&epFieldName[0]);
  xInputs = emlrtCreateLogicalMatrix(1, 13);
  emlrtSetField(xEntryPoints, 0, "Name", emlrtMxCreateString("UWBpos6"));
  emlrtSetField(xEntryPoints, 0, "NumberOfInputs",
                emlrtMxCreateDoubleScalar(13.0));
  emlrtSetField(xEntryPoints, 0, "NumberOfOutputs",
                emlrtMxCreateDoubleScalar(1.0));
  emlrtSetField(xEntryPoints, 0, "ConstantInputs", xInputs);
  emlrtSetField(
      xEntryPoints, 0, "FullPath",
      emlrtMxCreateString("C:\\Users\\zeusjang\\Downloads\\Positioning_Alg_"
                          "20231004\\UWBpos6.m"));
  emlrtSetField(xEntryPoints, 0, "TimeStamp",
                emlrtMxCreateDoubleScalar(739163.46116898151));
  xResult =
      emlrtCreateStructMatrix(1, 1, 7, (const char_T **)&propFieldName[0]);
  emlrtSetField(xResult, 0, "Version",
                emlrtMxCreateString("9.14.0.2306882 (R2023a) Update 4"));
  emlrtSetField(xResult, 0, "ResolvedFunctions",
                (mxArray *)emlrtMexFcnResolvedFunctionsInfo());
  emlrtSetField(xResult, 0, "Checksum",
                emlrtMxCreateString("o3Ey3WANJ9yEzes9uMo45E"));
  emlrtSetField(xResult, 0, "EntryPoints", xEntryPoints);
  return xResult;
}

//
// File trailer for _coder_UWBpos6_info.cpp
//
// [EOF]
//
