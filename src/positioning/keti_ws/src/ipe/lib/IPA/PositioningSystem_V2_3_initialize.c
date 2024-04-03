/*
 * PositioningSystem_V2_3_initialize.c
 *
 * Code generation for function 'PositioningSystem_V2_3_initialize'
 *
 */

/* Include files */
#include "PositioningSystem_V2_3_initialize.h"
#include "PositioningSystem_V2_3.h"
#include "PositioningSystem_V2_3_data.h"
#include "UWBPosition_V3_1.h"
#include "rt_nonfinite.h"

/* Function Definitions */
void PositioningSystem_V2_3_initialize(void)
{
  PositioningSystem_V2_3_init();
  UWBPosition_V3_1_init();
  isInitialized_PositioningSystem_V2_3 = true;
}

/* End of code generation (PositioningSystem_V2_3_initialize.c) */
