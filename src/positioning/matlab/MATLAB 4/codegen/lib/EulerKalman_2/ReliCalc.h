/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: ReliCalc.h
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 22-Sep-2023 10:38:00
 */

#ifndef RELICALC_H
#define RELICALC_H

/* Include Files */
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
extern double ReliCalc(const creal_T tag_pos_est[4],
                       const creal_T anchor_pos[6], const double RxDist_data[],
                       const int RxDist_size[1], const double RxID_data[],
                       const int RxID_size[2], double PP);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for ReliCalc.h
 *
 * [EOF]
 */
