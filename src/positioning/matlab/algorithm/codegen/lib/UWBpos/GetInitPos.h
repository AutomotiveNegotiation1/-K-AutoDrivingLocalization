/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: GetInitPos.h
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 01-Aug-2023 14:35:53
 */

#ifndef GETINITPOS_H
#define GETINITPOS_H

/* Include Files */
#include "UWBpos_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
double GetInitPos(const emxArray_real_T *xa, const emxArray_real_T *ya,
                  const emxArray_real_T *dist, const creal_T tag_pos_b[4],
                  double Ln, double Lp, creal_T tag_pos_est[4]);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for GetInitPos.h
 *
 * [EOF]
 */
