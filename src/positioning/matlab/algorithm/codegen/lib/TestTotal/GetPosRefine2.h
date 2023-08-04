/*
 * GetPosRefine2.h
 *
 * Code generation for function 'GetPosRefine2'
 *
 */

#ifndef GETPOSREFINE2_H
#define GETPOSREFINE2_H

/* Include files */
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
/* End of code generation (GetPosRefine2.h) */
