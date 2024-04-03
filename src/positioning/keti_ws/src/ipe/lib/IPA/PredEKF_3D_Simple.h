/*
 * PredEKF_3D_Simple.h
 *
 * Code generation for function 'PredEKF_3D_Simple'
 *
 */

#ifndef PREDEKF_3D_SIMPLE_H
#define PREDEKF_3D_SIMPLE_H

/* Include files */
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void PredEKF_3D_Simple(const double Xhat[15], const double acc[3],
                       const double p[3], double dT, double Xbar[15]);

void b_PredEKF_3D_Simple(const double Xhat[15], const double acc[3],
                         const double p[3], double Xbar[15]);

#ifdef __cplusplus
}
#endif

#endif
/* End of code generation (PredEKF_3D_Simple.h) */
