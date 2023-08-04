/*
 * GetPos3.h
 *
 * Code generation for function 'GetPos3'
 *
 */

#ifndef GETPOS3_H
#define GETPOS3_H

/* Include files */
#include "UWBpos_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void GetPos3(const double xa[6], const double ya[6], const double dist_data[],
             int dist_size, const double RxID_data[], const int RxID_size[2],
             const creal_T tag_pos_b[4], double Ln, double PP,
             const emxArray_creal_T *PredPos, const emxArray_real_T *DistPrev,
             const emxArray_real_T *b_RxIDprev, const double b_RxIDprevLen[4],
             const double b_PPprev[4], creal_T tag_pos_est[4],
             double *heading_est, creal_T *cand_tag_pos);

#ifdef __cplusplus
}
#endif

#endif
/* End of code generation (GetPos3.h) */
