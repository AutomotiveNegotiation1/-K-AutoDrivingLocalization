/*
 * PositioningSystem_V2_3.h
 *
 * Code generation for function 'PositioningSystem_V2_3'
 *
 */

#ifndef POSITIONINGSYSTEM_V2_3_H
#define POSITIONINGSYSTEM_V2_3_H

/* Include files */
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
extern void PositioningSystem_V2_3(const double PositionVector_data[],
                                   const int PositionVector_size[2],
                                   double PositionOut[10]);

void PositioningSystem_V2_3_init(void);

#ifdef __cplusplus
}
#endif

#endif
/* End of code generation (PositioningSystem_V2_3.h) */
