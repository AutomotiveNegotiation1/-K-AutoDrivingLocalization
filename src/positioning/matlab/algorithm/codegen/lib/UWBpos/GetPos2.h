/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: GetPos2.h
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 01-Aug-2023 14:35:53
 */

#ifndef GETPOS2_H
#define GETPOS2_H

/* Include Files */
#include "UWBpos_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
double GetPos2(const double xa[6], const double ya[6], const double dist_data[],
               int dist_size, const double RxID_data[], const int RxID_size[2],
               const creal_T tag_pos_b[4], double Ln, double PP,
               const emxArray_creal_T *PredPos, creal_T tag_pos_est[4],
               creal_T *cand_tag_pos);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for GetPos2.h
 *
 * [EOF]
 */
