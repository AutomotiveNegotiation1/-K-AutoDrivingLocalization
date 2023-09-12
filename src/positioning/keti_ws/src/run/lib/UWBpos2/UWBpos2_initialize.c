/*
 * UWBpos2_initialize.c
 *
 * Code generation for function 'UWBpos2_initialize'
 *
 */

/* Include files */
#include "UWBpos2_initialize.h"
#include "UWBpos2.h"
#include "UWBpos2_data.h"
#include "rt_nonfinite.h"

/* Function Definitions */
void UWBpos2_initialize(void)
{
  rt_InitInfAndNaN();
  r_not_empty_init();
  UWBpos2_init();
  isInitialized_UWBpos2 = true;
}

/* End of code generation (UWBpos2_initialize.c) */
