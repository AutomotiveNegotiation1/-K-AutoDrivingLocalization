/*
 * UWBpos2.h
 *
 * Code generation for function 'UWBpos2'
 *
 */

#ifndef UWBPOS2_H
#define UWBPOS2_H

/* Include files */
#include "UWBpos2_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
extern void UWBpos2(double Ln, double Lp, double LnC, double TagNum,
                    double Nanchor, const double RxIDin[6],
                    const double RxDistin[6], double s_time,
                    const creal_T tag_pos_b[4], const double xain[6],
                    const double yain[6], emxArray_real_T *UWBout);

void UWBpos2_free(void);

void UWBpos2_init(void);

void r_not_empty_init(void);

#ifdef __cplusplus
}
#endif

#endif
/* End of code generation (UWBpos2.h) */
