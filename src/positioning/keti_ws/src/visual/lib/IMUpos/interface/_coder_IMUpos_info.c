/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: _coder_IMUpos_info.c
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 04-Oct-2023 14:13:20
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
      "789ced984f8fd24018c6075dcdc664152ffb313615d6d578b205b6b295ddb5d004b486ad"
      "ed00ddb633b57f7629d73d68e2c1835f608fdef4ebf8453c0bfd4369"
      "93498d2520a4737979fbd0791ede925f2605a566ab0400780882f5793fa87b615f0eeb1d"
      "905c69bd14d69d541fad7ba1b297d2bf8655c6c8816327689064c0f9",
      "9d0a36542421a7e3991058d0c6fa15547c65a0eab0a31ab0bdd89cce3ae378419a373369"
      "f6b93682b2d6760d608dec38a1bed8cce7714bf8bd3b20b948f37849"
      "984739a5bf6bbcafbd10051b5ab63881ae7d29a1a158c7d748c792628be7d8561d152315"
      "0dfbb43eec57a84af509451d8acd966062fbc088f29a39f33ecac81b",
      "e90d578716eb59583015c981f3795de4f4bf4ff40f1405bb1f74183f9f9f39fd9a44bfa4"
      "feafcf2735a70323dc2f6b5ed19cb3f2a76bfcfd5dbf0e7fff2aadd2"
      "afae7d7ab04abf68adcb6f4cd8ef6fff7ffb04bf724a6f725cef84b78e618fd62eaf9ef3"
      "135c3d52d838c779864f560e40e857b5ff2de1feff95b31739f3eea6",
      "fa386fa0f8dca065795d5cfd9ed38f21fa25f55c5c9dce2702eab6f294bd7b53f0142c9f"
      "a7d45b1b3993a36e4d6bf74e3c8fc19a24b50a9eae8da7799ffbe38c"
      "bc91ee73a383dfb8d3c39885a6c9c2eb9b7a7ee5887e493d176717e71502775b793be6be"
      "15bc05cbe7ed6b411a1c8e4c97a73866e03a8caef11f55a6e0eda6be",
      "2748bfc749e78d749f1f9ca41b12ea57c0fa389bf73ccb12fd927a2ece46739a32765bf9"
      "dafef1a5e02b583e5fcf68165e779f769fc97cfdec152d0872d59b34"
      "369faf7f00e0c55ed9",
      ""};
  nameCaptureInfo = NULL;
  emlrtNameCaptureMxArrayR2016a(&data[0], 5792U, &nameCaptureInfo);
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
      emlrtMxCreateString("C:\\Users\\zeusjang\\Downloads\\Positioning_Alg_"
                          "20231004\\IMUpos.m"));
  emlrtSetField(xEntryPoints, 0, "TimeStamp",
                emlrtMxCreateDoubleScalar(739163.46112268523));
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
