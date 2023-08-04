/*
 * GetInitPos.h
 *
 * Code generation for function 'GetInitPos'
 *
 */

#ifndef GETINITPOS_H
#define GETINITPOS_H

/* Include files */
#include "UWBpos_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void GetInitPos(const emxArray_real_T *xa, const emxArray_real_T *ya,
                const emxArray_real_T *dist, const creal_T tag_pos_b[4],
                double Ln, double Lp, creal_T tag_pos_est[4],
                double *heading_est);

#ifdef __cplusplus
}
#endif

#endif
/* End of code generation (GetInitPos.h) */
