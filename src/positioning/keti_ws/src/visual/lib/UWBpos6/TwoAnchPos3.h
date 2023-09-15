/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: TwoAnchPos3.h
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 05-Sep-2023 20:11:41
 */

#ifndef TWOANCHPOS3_H
#define TWOANCHPOS3_H

/* Include Files */
#include "UWBpos6_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void TwoAnchPos3(const double Xa[2], const double Ya[2], const double dist[2],
                 const creal_T anch_pos_data[], const int anch_pos_size[2],
                 const emxArray_real_T *dist_a, double Pos[4], double Prob[2]);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for TwoAnchPos3.h
 *
 * [EOF]
 */
