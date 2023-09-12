/*
 * TwoAnchPos3.h
 *
 * Code generation for function 'TwoAnchPos3'
 *
 */

#ifndef TWOANCHPOS3_H
#define TWOANCHPOS3_H

/* Include files */
#include "UWBpos2_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void TwoAnchPos3(const double Xa[2], const double Ya[2], const double dist[2],
                 const emxArray_creal_T *anch_pos,
                 const emxArray_real_T *dist_a, double Pos[4], double Prob[2]);

#ifdef __cplusplus
}
#endif

#endif
/* End of code generation (TwoAnchPos3.h) */
