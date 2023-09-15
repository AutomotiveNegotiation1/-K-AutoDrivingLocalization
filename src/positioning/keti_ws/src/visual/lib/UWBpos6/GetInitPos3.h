/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: GetInitPos3.h
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 05-Sep-2023 20:11:41
 */

#ifndef GETINITPOS3_H
#define GETINITPOS3_H

/* Include Files */
#include "UWBpos6_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void GetInitPos3(const double xa_data[], const int xa_size[2],
                 const double ya_data[], const int ya_size[2],
                 const emxArray_real_T *dist, const creal_T tag_pos_b[4],
                 double Ln, const creal_T b_PrevPos[4], double b_PrevHeading,
                 creal_T tag_pos_est[4], double *heading_est, double *ErrSum);

void GetInitPos3_init(void);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for GetInitPos3.h
 *
 * [EOF]
 */
