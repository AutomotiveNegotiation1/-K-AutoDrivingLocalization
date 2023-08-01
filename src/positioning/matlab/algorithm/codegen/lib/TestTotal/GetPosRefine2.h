/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: GetPosRefine2.h
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 01-Aug-2023 15:24:45
 */

#ifndef GETPOSREFINE2_H
#define GETPOSREFINE2_H

/* Include Files */
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void GetPosRefine2(const double dist_data[], const double RxID_data[],
                   const int RxID_size[2], double Ln, double PP,
                   creal_T tag_pos_est[4], double *heading_est,
                   creal_T *cand_tag_pos);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for GetPosRefine2.h
 *
 * [EOF]
 */
