/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: EulerKalman_2.h
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 22-Sep-2023 10:38:00
 */

#ifndef EULERKALMAN_2_H
#define EULERKALMAN_2_H

/* Include Files */
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
extern void EulerKalman_2(double x[4], const double A[16], const double z[4],
                          double *phi, double *theta, double *psi);

void EulerKalman_2_init(void);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for EulerKalman_2.h
 *
 * [EOF]
 */
