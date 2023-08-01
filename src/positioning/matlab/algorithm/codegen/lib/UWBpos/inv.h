/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: inv.h
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 01-Aug-2023 14:35:53
 */

#ifndef INV_H
#define INV_H

/* Include Files */
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void b_inv(const creal_T x[9], creal_T y[9]);

void inv(const double x[4], double y[4]);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for inv.h
 *
 * [EOF]
 */
