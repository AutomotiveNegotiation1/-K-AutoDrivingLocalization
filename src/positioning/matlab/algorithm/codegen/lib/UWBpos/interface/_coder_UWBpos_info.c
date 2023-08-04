/*
 * _coder_UWBpos_info.c
 *
 * Code generation for function 'UWBpos'
 *
 */

/* Include files */
#include "_coder_UWBpos_info.h"
#include "emlrt.h"
#include "tmwtypes.h"

/* Function Declarations */
static const mxArray *emlrtMexFcnResolvedFunctionsInfo(void);

/* Function Definitions */
static const mxArray *emlrtMexFcnResolvedFunctionsInfo(void)
{
  const mxArray *nameCaptureInfo;
  const char_T *data[8] = {
      "789ced594b73d250140e4ed5faa86267746cc7853aae3b3420b4ee4209112c15217d5a1d"
      "1eb9485af2009294d64d773a2e74e58c0b17d5a5ff4157ea2f70e3ce"
      "71c6bde32f10482e90e86de8a45e4accd9dc9e7e0ddf774f7abede734bf812291f4110e7"
      "083df6a6f475ccc8fdc67a8c308715f719eb294b0ee33831627a0ee2",
      "2f8cb528890a68287a22e605d0799293045ecc8b0abb2d03a206ea5245035c1b29f115c0"
      "f202c8f6260bad4c88f7409da405b5be9e2b83e2665615885ab9de55"
      "58e94d3af56820f63bd2673d2e22eae1b7e0f7e907f4adf514c5ce53d1f5c5e5a82cd5a7"
      "841e1db2431da76d74409c014a42e495b4641403f2e71cf29f40f2eb",
      "0827a9850ae8f2ed3ae49b40f2997153ddbb9b6fd7be1576fb3fdfa71eebdafdf9d1f67a"
      "fde3571f4e3ee23bfb1c2b9f1183e273dac797107c7e0b1e0a554bd9"
      "cc6a2013aeac25d5c6ce2cc7cccf46bb3ad2363c763a08448eebf37711cf0faa2f9dfae2"
      "191b3d1067b7244a2c969b128226fe9c437edcbe3889e433e3a6faf7",
      "6c1ebe005c7d7c03b72fbe1bff8295cf08b7fb62b1b1ca71c98022895b3200f5058d56ab"
      "29daf3c57fe58b4edfeb051b3d106f4a48a9158567f38f9a3218205a"
      "74e41ceac0ed8f57917c66dcfa1ecc45981270f5f3d94f78fd31f77afa2d4e3e186ef747"
      "9109506c217bbb1a99bb335ba86a92560b7171f7f8a35be668d4fd86",
      "df82274405d4e4ce183d303f745af7cb483e336eaa7b67f3f87c107cc6eb83df1e3f99c1"
      "c907c3ed3e18995e51635b71fa6e49a3b6c3f7e6843cc5c63c1f3c74"
      "1fcc39d47112a943479a47a2f6ac3cacbed7efefabf51c0867645c7dba81d9f77ebe117f"
      "e1e483e176df0bd2f5edc84e617183131663693a4891339105c6f3bd",
      "c3ee47efbef0607cde7de1fe7cde7da11ede7de1c13effa8f8a1531de3363a20fec71519"
      "a97f7f50bef8d421df35249f19dfff9e90c4fe7fe667b8e7e4cd0f22"
      "4e3e186ef7c7a58de4cd5826a9901aa726e8683a9927ab756f4e3e72f7856396dcaa03e2"
      "ba356740891701490cef79f10a92cf8cffe5ef92b179dd1271f56f10",
      "f379716f72e2214e3e18c3ea87fdf6717495d3e2c9b59d7c2014e297d75224bf0418c23d"
      "7eb88b787e507d89cb17d9720d809e49fabff245d3e6f1fae28ff798"
      "e7e897a3afb0f21931acbed8ef39510ef32259039930139f2e71458191cb4ac80573f46f"
      "52afb284",
      ""};
  nameCaptureInfo = NULL;
  emlrtNameCaptureMxArrayR2016a(&data[0], 11960U, &nameCaptureInfo);
  return nameCaptureInfo;
}

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
                emlrtMxCreateString("E:\\MATLAB\\UWBpos.m"));
  emlrtSetField(xEntryPoints, 0, "TimeStamp",
                emlrtMxCreateDoubleScalar(739102.40388888889));
  xResult =
      emlrtCreateStructMatrix(1, 1, 7, (const char_T **)&propFieldName[0]);
  emlrtSetField(xResult, 0, "Version",
                emlrtMxCreateString("9.13.0.2166757 (R2022b) Update 4"));
  emlrtSetField(xResult, 0, "ResolvedFunctions",
                (mxArray *)emlrtMexFcnResolvedFunctionsInfo());
  emlrtSetField(xResult, 0, "Checksum",
                emlrtMxCreateString("OK3DczXoYESHfpZJPJA9OB"));
  emlrtSetField(xResult, 0, "EntryPoints", xEntryPoints);
  return xResult;
}

/* End of code generation (_coder_UWBpos_info.c) */
