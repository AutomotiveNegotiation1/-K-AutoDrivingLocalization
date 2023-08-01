/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: _coder_TestTotal_info.c
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 01-Aug-2023 15:24:45
 */

/* Include Files */
#include "_coder_TestTotal_info.h"
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
  const char_T *data[9] = {
      "789ced59cd72d250184d9d6a5b9d5aaa6337ae74e9680314d12e5c941628589042fa87e3"
      "0c21b990db2637901f0a75c657e8b8551fc0a52fe0c285a32fe3caa5"
      "d3198170816426d236f4d6a4f936976f0ee41cbe4b4e6e4fa989546682a2a8db9451b967"
      "c63adbeb03bdf51a652e2b3ed15b6f5a7a5cd7a949d3e7307edc5b39",
      "1969a0a9190d6225d0ff242f4b10b148635a3540294095c506e0bb48058a808112280c37"
      "d94e272586a07ed3813aaf5705c01d147489520475a0501c6efaf3f8"
      "6cf37d274f398f759b79042cf8ebf81b5a902540972516d187b27240d764156a5046b4aa"
      "70fd06a22a2db19ac8966956acca0ad404896680aa31b2c68a8b12d6",
      "5d72a8fb86ad6e03d9da89b5250de674d17cbcac974530be7d89dbf2997147fb620c6951"
      "a246cf69ee94baadebe0fdd3ddf581feb37b7390e2fbf8fec323927c"
      "b82e8baf6973bdd3feee166cf802167c73ed7923166a6e090a9f5e4d6c3723519e692586"
      "7c7a04cf281d944d4feafa6ebd7f6b0e75db3d1f03163c09b414825a",
      "4eee3d942ecb67bf38e44bd9f2997147fb34185677af3a45ca1f0a0a59bf5da77f4d91e4"
      "c3e575bf8d44ea95427e2f988f8ac5b4de3c5ae6931bcb31eff8addb"
      "ef63a7be7b6b847e8c3387f20ae284b68425137fc9213f69df4ddbf29971677f770c8685"
      "378c944f3c6c90f5ddbb27dfe749f2e1f2baef72cd3d9e4f0735191d",
      "d60050b38db85ecfc47ddffd5f7cd7e9ef607e847e8cb725647451830c5b6dcb480264d1"
      "5172a883b4ffe62c3d65791fc69dee9b7968a3f39f71f9c50bc2e7de"
      "29fded7d927cb8bceebf47aff46c7d892972bb494d155e66b6a375b8b1e61dffbdaa39c3"
      "cc08dd184f210d28b57ecce0da3c9748cede1f16399f7d77f28da8cf",
      "e2ba2a7ca47cb61ac935c2adac94db87628c13e17eabb2195cf57df6b27db6e450f794ad"
      "6e03691fd1da7e1176adaf266cf9ccb8d3736c67489d8d21e503d32a"
      "d9f3ebc28f9939927cb8bceeabeb8895cb3ca884986ce8290a878ae9dd959c87725bb7de"
      "bf7e5e7b363e3faf1d2f9f9fd71ae5e7b567bbbe5bfdd6cf69cfc7e7",
      "e7b4e3e5f3735aa3fc9cf66cd7776b7ee0f49c3b6be9adba316e587e1e54200261cabd7e"
      "bb61cb67c6c7f07cec0dcb38e992f2874fc764fdf6f1efc413927cb8"
      "dceab7f76cf802163cb6c73712e9e2111b8c44e04e311386db204979c76fdd7e1f93f25d"
      "465000184a187cdffd57be303c2cb2be7be7cf57ffff6417c847ea9c",
      "5b8b421456403e9a4c842a3c27256b8216f140bef017a47d651d",
      ""};
  nameCaptureInfo = NULL;
  emlrtNameCaptureMxArrayR2016a(&data[0], 14184U, &nameCaptureInfo);
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
  xInputs = emlrtCreateLogicalMatrix(1, 0);
  emlrtSetField(xEntryPoints, 0, "Name", emlrtMxCreateString("TestTotal"));
  emlrtSetField(xEntryPoints, 0, "NumberOfInputs",
                emlrtMxCreateDoubleScalar(0.0));
  emlrtSetField(xEntryPoints, 0, "NumberOfOutputs",
                emlrtMxCreateDoubleScalar(4.0));
  emlrtSetField(xEntryPoints, 0, "ConstantInputs", xInputs);
  emlrtSetField(xEntryPoints, 0, "FullPath",
                emlrtMxCreateString("/home/bman/work/position/src/positioning/"
                                    "matlab/algorithm/TestTotal.m"));
  emlrtSetField(xEntryPoints, 0, "TimeStamp",
                emlrtMxCreateDoubleScalar(739099.6415393519));
  xResult =
      emlrtCreateStructMatrix(1, 1, 7, (const char_T **)&propFieldName[0]);
  emlrtSetField(xResult, 0, "Version",
                emlrtMxCreateString("9.14.0.2206163 (R2023a)"));
  emlrtSetField(xResult, 0, "ResolvedFunctions",
                (mxArray *)emlrtMexFcnResolvedFunctionsInfo());
  emlrtSetField(xResult, 0, "Checksum",
                emlrtMxCreateString("CM2MJtcNYfM7UeZrdPWy6D"));
  emlrtSetField(xResult, 0, "EntryPoints", xEntryPoints);
  return xResult;
}

/*
 * File trailer for _coder_TestTotal_info.c
 *
 * [EOF]
 */
