/*
 * TwoAnchPos3.h
 *
 * Code generation for function 'TwoAnchPos3'
 *
 */

#ifndef TWOANCHPOS3_H
#define TWOANCHPOS3_H

/* Include files */
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void TwoAnchPos3(const double Xa[2], const double Ya[2], const double dist[2],
                 const creal_T anch_pos_data[], const int anch_pos_size[2],
                 const double dist_a_data[], int dist_a_size, double Pos[4],
                 double Prob[2]);

#ifdef __cplusplus
}
#endif

#endif
/* End of code generation (TwoAnchPos3.h) */
