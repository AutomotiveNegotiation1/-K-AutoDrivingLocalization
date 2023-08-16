/*
 * _coder_UWBpos2_api.h
 *
 * Code generation for function 'UWBpos2'
 *
 */

#ifndef _CODER_UWBPOS2_API_H
#define _CODER_UWBPOS2_API_H

/* Include files */
#include "emlrt.h"
#include "tmwtypes.h"
#include <string.h>

/* Type Definitions */
#ifndef struct_emxArray_real_T
#define struct_emxArray_real_T
struct emxArray_real_T {
  real_T *data;
  int32_T *size;
  int32_T allocatedSize;
  int32_T numDimensions;
  boolean_T canFreeData;
};
#endif /* struct_emxArray_real_T */
#ifndef typedef_emxArray_real_T
#define typedef_emxArray_real_T
typedef struct emxArray_real_T emxArray_real_T;
#endif /* typedef_emxArray_real_T */

/* Variable Declarations */
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void UWBpos2(real_T Ln, real_T Lp, real_T LnC, real_T TagNum, real_T Nanchor,
             real_T RxIDin[6], real_T RxDistin[6], real_T s_time,
             creal_T tag_pos_b[4], real_T xain[6], real_T yain[6],
             emxArray_real_T *UWBout);

void UWBpos2_api(const mxArray *const prhs[11], const mxArray **plhs);

void UWBpos2_atexit(void);

void UWBpos2_initialize(void);

void UWBpos2_terminate(void);

void UWBpos2_xil_shutdown(void);

void UWBpos2_xil_terminate(void);

#ifdef __cplusplus
}
#endif

#endif
/* End of code generation (_coder_UWBpos2_api.h) */
