/*
 * PositioningSystem_V2_3_terminate.c
 *
 * Code generation for function 'PositioningSystem_V2_3_terminate'
 *
 */

/* Include files */
#include "PositioningSystem_V2_3_terminate.h"
#include "PositioningSystem_V2_3_data.h"
#include "UWBPosition_V3_1.h"
#include "rt_nonfinite.h"

/* Function Definitions */
void PositioningSystem_V2_3_terminate(void)
{
  UWBPosition_V3_1_free();
  isInitialized_PositioningSystem_V2_3 = false;
}

/* End of code generation (PositioningSystem_V2_3_terminate.c) */
