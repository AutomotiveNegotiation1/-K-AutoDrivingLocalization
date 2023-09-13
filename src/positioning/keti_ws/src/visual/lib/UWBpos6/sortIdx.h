/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: sortIdx.h
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 05-Sep-2023 20:11:41
 */

#ifndef SORTIDX_H
#define SORTIDX_H

/* Include Files */
#include "UWBpos6_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void b_merge(int idx[16], double x[16], int offset, int np, int nq,
             int iwork[16], double xwork[16]);

void merge_block(emxArray_int32_T *idx, emxArray_real_T *x, int offset, int n,
                 int preSortLevel, emxArray_int32_T *iwork,
                 emxArray_real_T *xwork);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for sortIdx.h
 *
 * [EOF]
 */
