/*
 * timeKeeper.h
 *
 * Code generation for function 'timeKeeper'
 *
 */

#ifndef TIMEKEEPER_H
#define TIMEKEEPER_H

/* Include files */
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void b_timeKeeper(double *outTime_tv_sec, double *outTime_tv_nsec);

void savedTime_not_empty_init(void);

void timeKeeper(double newTime_tv_sec, double newTime_tv_nsec);

#ifdef __cplusplus
}
#endif

#endif
/* End of code generation (timeKeeper.h) */
