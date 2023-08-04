/*
 * UWBpos_initialize.c
 *
 * Code generation for function 'UWBpos_initialize'
 *
 */

/* Include files */
#include "UWBpos_initialize.h"
#include "UWBpos.h"
#include "UWBpos_data.h"
#include "rt_nonfinite.h"

/* Function Definitions */
void UWBpos_initialize(void)
{
  rt_InitInfAndNaN();
  r_not_empty_init();
  UWBpos_init();
  isInitialized_UWBpos = true;
}

/* End of code generation (UWBpos_initialize.c) */
