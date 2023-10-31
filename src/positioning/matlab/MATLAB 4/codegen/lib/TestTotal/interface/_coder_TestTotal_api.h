/*
 * _coder_TestTotal_api.h
 *
 * Code generation for function 'TestTotal'
 *
 */

#ifndef _CODER_TESTTOTAL_API_H
#define _CODER_TESTTOTAL_API_H

/* Include files */
#include "emlrt.h"
#include "tmwtypes.h"
#include <string.h>

/* Variable Declarations */
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void TestTotal(creal_T tag_pos_est[4], real_T *heading_est,
               creal_T tag_pos_est_aver[4], real_T *headingest_a_aver_v);

void TestTotal_api(int32_T nlhs, const mxArray *plhs[4]);

void TestTotal_atexit(void);

void TestTotal_initialize(void);

void TestTotal_terminate(void);

void TestTotal_xil_shutdown(void);

void TestTotal_xil_terminate(void);

#ifdef __cplusplus
}
#endif

#endif
/* End of code generation (_coder_TestTotal_api.h) */
