/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: sort.h
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 05-Sep-2023 20:11:41
 */

#ifndef SORT_H
#define SORT_H

/* Include Files */
#include "UWBpos6_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void b_sort(double x[16], int idx[16]);

void sort(emxArray_real_T *x, emxArray_int32_T *idx);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for sort.h
 *
 * [EOF]
 */
