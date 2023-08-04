/*
 * _coder_TestTotal_info.c
 *
 * Code generation for function 'TestTotal'
 *
 */

/* Include files */
#include "_coder_TestTotal_info.h"
#include "emlrt.h"
#include "tmwtypes.h"

/* Function Declarations */
static const mxArray *emlrtMexFcnResolvedFunctionsInfo(void);

/* Function Definitions */
static const mxArray *emlrtMexFcnResolvedFunctionsInfo(void)
{
  const mxArray *nameCaptureInfo;
  const char_T *data[10] = {
      "789ced5a4d6c1b45147e41015268615ba995e0048843107ffeab9d70b3637b6b8383eb6c"
      "92c69862c73baed7d99df5cfda4e0287de101c5a2e481c3800474e45"
      "54824bab542a4d0f55d5031c907aa890b823ce1c18db33b1b36814579b8eebc5ef90cf2f"
      "6f77bf997dbbdf8c3f19a612a92900780e7a919febe1319a4b149f80",
      "fd61af4f517cc696b37812a6f79dc7ea9729164d6ca14dab97e08281f6ce544d43c3056c"
      "295b550475d430f51652bb9592a6234533d0d260b2d8c98cf840692f"
      "e9943a9f17caa8b8b1d434a05e6ef447a80f267bf7e367ce7ca787bc1faf73eec7718a2b"
      "143f80187c0832bc0339b87ae525b87ff5939facdda3d773f0c5eeec",
      "ee3fb73ffb6de6d6cebd8fc00b73372e5ecb811f66ef447fb8f9630e5ebe3bbd9b030510"
      "34c0226892bf05d0e12d3006e69177388fa738f3906865793552351b"
      "e2f854b3b9aea33edf15877cb31c3ed6a73445a77d5a865588409574a9d1edd041f7ebf9"
      "21c76fc7fef1335dd46ffdde2d89e2f36cdfd345f2b11815df26e77a",
      "c33e7fa7387c92adae9f0d55948ab1818c5a50cb64cdb677c35c8df5c7c19e531ecf41e3"
      "004e2eeafae3fe1e571d8e9fb77e4ab6ba8cac04d6acb44917ad51e9"
      "aed3f5f10d0e1febd7398a4efb2593f5d1820460d008a685ebef2b37c5ea2ffca15c12ca"
      "47c3edfa1b08d44a4b99354f26a86793cdcded79557e6f3ee21efd75",
      "cbfbec54879fe5cc43b2d595b619c6c532d1617f371f950eef38e47b93c3c7fa96a5e8fc"
      "7b4a9bf42a4cfa568432ed9b9f7e57e98428fd7855b41e7f7fe257a1"
      "7c34dcaec7c5cd35554d7a2c13b7ab0835165bb1662de5a2fdb05bf4d8e9f370dc9683ed"
      "385627fbe25453b734a5708168b28c30fdffa874f9b6433e3f878fcd",
      "b744f170fa978226e80435a2d305b840fbd8a9e121fda4c3d293a3bf88d5e7fcd7deef44"
      "f2b170bb3e63d91356d697ced4420befceafd75a66ab1e50e3eed1e7"
      "ffbb5f7184337ec9564f600bd5ab7b76c5d8fa15a2fcfcce4a6a11ddad930ef55753717a"
      "8104fbc50f3efe744e241f0bb7eb6fc87bae196dc763ef975ae1ade0",
      "d905a31056a213fd7d6cf437ef70fc4f73c62fd10ad90f77bd8971d5dbd7387cac3f198a"
      "87b3ff1d9d1f5111acb77f7d8bff16c9c7c2ed7aeb8f35b642dbebcb"
      "15d5588ea663feb06f2eb428bb476fddf23e4ffce187e39bf8c38f966fe20ff762e20f3f"
      "dcf5dda2c74e9f87139c7948b6fa7ffc61dffe71e41d8e43b43f1ce0",
      "f0b1fe95293e7a7fd827f4f7149f8bf62736ae63917c2cdcaecf2b95e4e9682669f95a6a"
      "33118ba493055fad31f1271e1b7fc2e93ef9982d07db71acdef32932"
      "a8a461d491e471dd27bfcde163fd3a4ff130d7d30cf95c229a8c09fa04fbc47ec1fbe46f"
      "5e7ce1bc483e16e3aac327397c92ad1e59535bf16476bbe00904b4d5",
      "6ccaa7ad2019dca3c33b9cf3c7f5bd16a5cb4ab98ed0808331d1e583fc8b32d449bf10c7"
      "c310a5237f5e13ec5f7c39f395503e1ae3aacbc3ee8fab410dfbea28"
      "1394e3de925a34e46ad90ab8c0bff817ce159522",
      ""};
  nameCaptureInfo = NULL;
  emlrtNameCaptureMxArrayR2016a(&data[0], 14456U, &nameCaptureInfo);
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
  xInputs = emlrtCreateLogicalMatrix(1, 0);
  emlrtSetField(xEntryPoints, 0, "Name", emlrtMxCreateString("TestTotal"));
  emlrtSetField(xEntryPoints, 0, "NumberOfInputs",
                emlrtMxCreateDoubleScalar(0.0));
  emlrtSetField(xEntryPoints, 0, "NumberOfOutputs",
                emlrtMxCreateDoubleScalar(4.0));
  emlrtSetField(xEntryPoints, 0, "ConstantInputs", xInputs);
  emlrtSetField(
      xEntryPoints, 0, "FullPath",
      emlrtMxCreateString(
          "G:\\\xeb\x82\xb4 "
          "\xeb\x93\x9c\xeb\x9d\xbc\xec\x9d\xb4\xeb\xb8\x8c\\\xec\x9e\x90\xec"
          "\x9c\xa8\xec\xa3\xbc\xed\x96\x89\xec\x98\x88\xed"
          "\x83\x80_"
          "1\xec\x84\xb8\xeb\xb6\x80\\"
          "3\xec\xb0\xa8\xeb\x85\x84\xeb\x8f\x84\\\xec\xb8\xa1\xec\x9c\x84\\T"
          "estTotal.m"));
  emlrtSetField(xEntryPoints, 0, "TimeStamp",
                emlrtMxCreateDoubleScalar(739102.55375));
  xResult =
      emlrtCreateStructMatrix(1, 1, 7, (const char_T **)&propFieldName[0]);
  emlrtSetField(xResult, 0, "Version",
                emlrtMxCreateString("9.13.0.2166757 (R2022b) Update 4"));
  emlrtSetField(xResult, 0, "ResolvedFunctions",
                (mxArray *)emlrtMexFcnResolvedFunctionsInfo());
  emlrtSetField(xResult, 0, "Checksum",
                emlrtMxCreateString("uf5NOF4DwKQKraZFQCT89B"));
  emlrtSetField(xResult, 0, "EntryPoints", xEntryPoints);
  return xResult;
}

/* End of code generation (_coder_TestTotal_info.c) */
