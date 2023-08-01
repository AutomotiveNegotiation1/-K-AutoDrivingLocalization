/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: UWBpos.h
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 01-Aug-2023 14:00:18
 */

#ifndef UWBPOS_H
#define UWBPOS_H

/* Include Files */
#include "UWBpos_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
extern void UWBpos(double Ln, double Lp, double TagNum, double Nanchor,
                   const double RxID_data[], const int RxID_size[2],
                   const double RxDist_data[], const int RxDist_size[1],
                   double s_time, const creal_T tag_pos_b[4],
                   const double xa[6], const double ya[6],
                   emxArray_creal_T *tag_pos_est, double *heading_est,
                   emxArray_creal_T *tag_pos_est_aver,
                   double *headingest_a_aver_v);

void UWBpos_free(void);

void UWBpos_init(void);

void r_not_empty_init(void);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for UWBpos.h
 *
 * [EOF]
 */
