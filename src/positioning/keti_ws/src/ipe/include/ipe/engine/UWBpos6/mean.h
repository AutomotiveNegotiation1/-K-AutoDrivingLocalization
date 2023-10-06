/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: mean.h
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 05-Sep-2023 20:11:41
 */

#ifndef MEAN_H
#define MEAN_H

/* Include Files */
#include "UWBpos6_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
creal_T b_mean(const emxArray_creal_T *x);

creal_T mean(const creal_T x[3]);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for mean.h
 *
 * [EOF]
 */
