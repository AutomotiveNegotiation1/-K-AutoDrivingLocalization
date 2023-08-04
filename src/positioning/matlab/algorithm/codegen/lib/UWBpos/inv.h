/*
 * inv.h
 *
 * Code generation for function 'inv'
 *
 */

#ifndef INV_H
#define INV_H

/* Include files */
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void b_inv(const creal_T x[9], creal_T y[9]);

void inv(const double x[4], double y[4]);

#ifdef __cplusplus
}
#endif

#endif
/* End of code generation (inv.h) */
