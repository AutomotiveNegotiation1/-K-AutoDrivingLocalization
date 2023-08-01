/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: UWBpos.h
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 01-Aug-2023 16:36:28
 */

#ifndef UWBPOS_H
#define UWBPOS_H

/* Include Files */
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
double UWBpos(double TagNum, double Nanchor, const double RxID_data[],
              const int RxID_size[2], const double RxDist_data[],
              int RxDist_size, double s_time, creal_T tag_pos_est[4],
              creal_T tag_pos_est_aver[4], double *headingest_a_aver_v);

void UWBpos_init(void);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for UWBpos.h
 *
 * [EOF]
 */
