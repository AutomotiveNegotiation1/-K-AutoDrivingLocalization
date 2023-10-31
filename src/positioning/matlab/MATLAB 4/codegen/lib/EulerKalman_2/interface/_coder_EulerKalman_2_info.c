/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: _coder_EulerKalman_2_info.c
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 22-Sep-2023 10:38:00
 */

/* Include Files */
#include "_coder_EulerKalman_2_info.h"
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
      "789c6360f4f465646060e0638000054108cd0be50b4069260654802ecf08a599d1f830c0"
      "cac082a20f26df0fa593f3f34a522b4a209cbcc4dc54b8ce94fcdccc"
      "bcc4bc9290ca825486a2d4e2fc9cb2d414b04c5a664e6a48666e6a3032c70fc4cb754392"
      "8273405220b673466a727670692e43514631c28539c81c78782cc0e1",
      "5f1622c3c30a477808a0c947bbc6ba5ac578e6a5e4e717c5e7e427c704e417679664e6e7"
      "65e6a53be6a4c71b1918191b581a98c4f83a86f8383a2998c4a49516"
      "03a5f57251dd5b41a17b0509b81726ef5a9a935a14921f589a58925a9407740803aa3b12"
      "2874071b4e77406452f24b93725211f6ada0d03e579cf6a1ca931e4f",
      "1821058e3242e1c44fa4bbd169847a0e30fdf0fe4db014bdec13d8f5c7999ef6c1c040d9"
      "47697e13c3619f009abc4f68629a494641699081b7535a6989534e76"
      "5061a613c21d0104ec21e40e061c7c7a99bf0087fec15ace1650e85ef47a10ddbd307970"
      "e9e19d98939b98176fc43074cb57479cf6a1ca9359bec24208164dc3",
      "b67cd566b93e5abe3250bf7cf577744f2d8f308d304f0e72f1f7700c0d4d36aeac721d2d"
      "5f876af9ca45c0bd3079a0f320658711aafd0914da4f6af9ba8042fb"
      "ec70da872a4f7afc2042081447f42a07d4ded0b75ccd93b87e949ef6c1c0702f571d2b53"
      "4b230ad3422a5243928abdfd0d8db212cd025d867eb90a003ceefbf9",
      ""};
  nameCaptureInfo = NULL;
  emlrtNameCaptureMxArrayR2016a(&data[0], 4408U, &nameCaptureInfo);
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
      emlrtCreateStructMatrix(1, 5, 6, (const char_T **)&epFieldName[0]);
  xInputs = emlrtCreateLogicalMatrix(1, 3);
  emlrtSetField(xEntryPoints, 0, "Name", emlrtMxCreateString("EulerKalman_2"));
  emlrtSetField(xEntryPoints, 0, "NumberOfInputs",
                emlrtMxCreateDoubleScalar(3.0));
  emlrtSetField(xEntryPoints, 0, "NumberOfOutputs",
                emlrtMxCreateDoubleScalar(3.0));
  emlrtSetField(xEntryPoints, 0, "ConstantInputs", xInputs);
  emlrtSetField(xEntryPoints, 0, "FullPath",
                emlrtMxCreateString("E:\\Indoor_loc\\PositioningAlg_"
                                    "20230904\\MATLAB 4\\EulerKalman_2.m"));
  emlrtSetField(xEntryPoints, 0, "TimeStamp",
                emlrtMxCreateDoubleScalar(739134.79928240739));
  xInputs = emlrtCreateLogicalMatrix(1, 3);
  emlrtSetField(xEntryPoints, 1, "Name",
                emlrtMxCreateString("EulerToQuaternion"));
  emlrtSetField(xEntryPoints, 1, "NumberOfInputs",
                emlrtMxCreateDoubleScalar(3.0));
  emlrtSetField(xEntryPoints, 1, "NumberOfOutputs",
                emlrtMxCreateDoubleScalar(1.0));
  emlrtSetField(xEntryPoints, 1, "ConstantInputs", xInputs);
  emlrtSetField(xEntryPoints, 1, "FullPath",
                emlrtMxCreateString("E:\\Indoor_loc\\PositioningAlg_"
                                    "20230904\\MATLAB 4\\EulerToQuaternion.m"));
  emlrtSetField(xEntryPoints, 1, "TimeStamp",
                emlrtMxCreateDoubleScalar(739134.79928240739));
  xInputs = emlrtCreateLogicalMatrix(1, 16);
  emlrtSetField(xEntryPoints, 2, "Name", emlrtMxCreateString("fusion"));
  emlrtSetField(xEntryPoints, 2, "NumberOfInputs",
                emlrtMxCreateDoubleScalar(16.0));
  emlrtSetField(xEntryPoints, 2, "NumberOfOutputs",
                emlrtMxCreateDoubleScalar(6.0));
  emlrtSetField(xEntryPoints, 2, "ConstantInputs", xInputs);
  emlrtSetField(
      xEntryPoints, 2, "FullPath",
      emlrtMxCreateString(
          "E:\\Indoor_loc\\PositioningAlg_20230904\\MATLAB 4\\fusion.m"));
  emlrtSetField(xEntryPoints, 2, "TimeStamp",
                emlrtMxCreateDoubleScalar(739151.40402777772));
  xInputs = emlrtCreateLogicalMatrix(1, 4);
  emlrtSetField(xEntryPoints, 3, "Name", emlrtMxCreateString("PosKalman2"));
  emlrtSetField(xEntryPoints, 3, "NumberOfInputs",
                emlrtMxCreateDoubleScalar(4.0));
  emlrtSetField(xEntryPoints, 3, "NumberOfOutputs",
                emlrtMxCreateDoubleScalar(1.0));
  emlrtSetField(xEntryPoints, 3, "ConstantInputs", xInputs);
  emlrtSetField(
      xEntryPoints, 3, "FullPath",
      emlrtMxCreateString(
          "E:\\Indoor_loc\\PositioningAlg_20230904\\MATLAB 4\\PosKalman2.m"));
  emlrtSetField(xEntryPoints, 3, "TimeStamp",
                emlrtMxCreateDoubleScalar(739150.41579861112));
  xInputs = emlrtCreateLogicalMatrix(1, 5);
  emlrtSetField(xEntryPoints, 4, "Name", emlrtMxCreateString("ReliCalc"));
  emlrtSetField(xEntryPoints, 4, "NumberOfInputs",
                emlrtMxCreateDoubleScalar(5.0));
  emlrtSetField(xEntryPoints, 4, "NumberOfOutputs",
                emlrtMxCreateDoubleScalar(1.0));
  emlrtSetField(xEntryPoints, 4, "ConstantInputs", xInputs);
  emlrtSetField(
      xEntryPoints, 4, "FullPath",
      emlrtMxCreateString(
          "E:\\Indoor_loc\\PositioningAlg_20230904\\MATLAB 4\\ReliCalc.m"));
  emlrtSetField(xEntryPoints, 4, "TimeStamp",
                emlrtMxCreateDoubleScalar(739134.79928240739));
  xResult =
      emlrtCreateStructMatrix(1, 1, 7, (const char_T **)&propFieldName[0]);
  emlrtSetField(xResult, 0, "Version",
                emlrtMxCreateString("9.14.0.2306882 (R2023a) Update 4"));
  emlrtSetField(xResult, 0, "ResolvedFunctions",
                (mxArray *)emlrtMexFcnResolvedFunctionsInfo());
  emlrtSetField(xResult, 0, "Checksum",
                emlrtMxCreateString("bicYGj7j5ipzihsNPsceRB"));
  emlrtSetField(xResult, 0, "EntryPoints", xEntryPoints);
  return xResult;
}

/*
 * File trailer for _coder_EulerKalman_2_info.c
 *
 * [EOF]
 */
