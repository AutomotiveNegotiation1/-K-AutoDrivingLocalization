//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: _coder_fusion_info.cpp
//
// MATLAB Coder version            : 5.6
// C/C++ source code generated on  : 17-Oct-2023 17:24:31
//

// Include Files
#include "_coder_fusion_info.h"
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
      "789ced9b4b8fdb5414c76fa0a0aa4c692aa12ed820954d5950a599909976859dd7e49d71"
      "626632a4ca38ce4de289ed9bf891d7022121d1050b162cd8b098251b"
      "041f824f80c402be07ac71ec7812bbb592c6194f6deedd788effb6ff27e78e7f73e75c05"
      "84b2c51000e07d608c4719e37877118717c7b78075d8f590ed7cc87a",
      "397807dc7aa5fefde2c822518113c5084446805777b691c0898ca8d4a603082428237e04"
      "dbbad2e17858e304585d0d4af34848af4857c15c9aff9ce841b65f55"
      "0520f5e46586fc6a70558f4b87cf7b6bc37a7cee508fb04dff32f53cf1ac41cb50921b33"
      "a8ca178cd86d24d158e411d3961b1524730a87444eec3609bedb8c46",
      "a2fb4f229158a3a3cadae9c78299efc465bef7d7e46bea299587520d1dab8c0225514b61"
      "71decce3dc651eef3ae661286da4b678b8f4fbcda55fded1cfaa6f3b"
      "4f2fd5ebb1a03f6f5dbdee6d98bffdb8bcfeb67eecfefb57c84bbf49fe873b5efa99e3a6"
      "fcdcbe770f1cfcc236bd40339d586fa052913cd9511592ef53438e5c",
      "e65159e3b32e0fe0107bf5fc4b87fbdf54de0e5ce6ebf4f7326cd3757ee4195e60c46614"
      "dc1c677f76e99771f4b3eaae386bd649636c50f95afdf53bcc57b07b"
      "be96890c1c9f7e767ac052c9f21141d3ecfe7496c27cf52b5fefacc9d7d4b5840c6a448d"
      "d8af7c4d3afa59f56de767592763011b54be7efce378cf4b3f73049d",
      "af443f9ac90e2b278513ee8b5cae371ea692858b34e6ab577ccd16e9019277c7d77bb6d8"
      "9eafa9ebebb2cc5442f4a0adfd0becdb3e41d6d1cfaabb5abf2eebb4"
      "e8120497b3c9fe0bbc8e05bbe76c369fafe728290deb44ff627448cdd07ebc9dc19cbd29"
      "ce9ebbccf7b62d5ee66b283a370896f5ebba9574f4b3eaaeb8aad5c7",
      "046a50799a79fb1bcc53b07b9e46ce645199c54f13fd6a3d379d92a8cf3045ccd31be329"
      "dee7dace0fef73edd60fef731903ef73bddef32f1dee7f5379ebb64f"
      "80f7b9acd7e17daecdfcf03e9731f03ed7eb3ddfedfb4b383c3f6cd3b77d7fe91352e36b"
      "fca5fe9e5bcebeb7266f53cf40252b728a965fcce27feed2df6bcea6",
      "1cfdacfab6f3b452277dae82cad97fbefd137316ec9eb307879d7a7140f268aca446f424"
      "5f8c55536c80fa067e7d7fdd72766f4ddea6aef96ba8d7ec9b231db4"
      "7ee56cdad1cfaabb98a7ab3acd27ca2b2efc01fef694b32ff6a8475efa9923e89c2d6767"
      "2d02756844302763aa9020cfba917880fa057e7d7fbdea1b24a0a840",
      "49f34f72f2fc5b247ee5ec75f70d2c750a70dfe0f75f9a783d0bae611fac44d0c3567604"
      "5b4fa9e921894a958e1ac3ebd9ff0d676b12c7cfb776185ecb0160ce"
      "3acd93a54e01e6ec279f7e74d74b3f73049db38583b38a943b964b85e3e3595a291fb48e"
      "780aaf677ddf37d8b43f5b1b2342647b9affbe1efb95b3d73d4f2b75",
      "0a747ff6e1d751cc59b07bceb2937abb9d8b28481c0f20944ba3943a2ce27d30df7376e3"
      "f56c4f827085b47ee5ecb5af6757eb14e0f5ec573f7d88390b36ffbd"
      "fbc0c12f6cd39f08cab04ca051e4f0693716eff3f411ecaee6e157cefe070c8c22a6",
      ""};
  nameCaptureInfo = nullptr;
  emlrtNameCaptureMxArrayR2016a(&data[0], 18240U, &nameCaptureInfo);
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
  xInputs = emlrtCreateLogicalMatrix(1, 16);
  emlrtSetField(xEntryPoints, 0, "Name", emlrtMxCreateString("fusion"));
  emlrtSetField(xEntryPoints, 0, "NumberOfInputs",
                emlrtMxCreateDoubleScalar(16.0));
  emlrtSetField(xEntryPoints, 0, "NumberOfOutputs",
                emlrtMxCreateDoubleScalar(6.0));
  emlrtSetField(xEntryPoints, 0, "ConstantInputs", xInputs);
  emlrtSetField(
      xEntryPoints, 0, "FullPath",
      emlrtMxCreateString("C:\\Users\\zeusjang\\Downloads\\Positioning_Alg_"
                          "20231004\\fusion.m"));
  emlrtSetField(xEntryPoints, 0, "TimeStamp",
                emlrtMxCreateDoubleScalar(739176.72379629628));
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
// File trailer for _coder_fusion_info.cpp
//
// [EOF]
//
