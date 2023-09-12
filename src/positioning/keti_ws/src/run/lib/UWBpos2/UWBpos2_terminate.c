/*
 * UWBpos2_terminate.c
 *
 * Code generation for function 'UWBpos2_terminate'
 *
 */

/* Include files */
#include "UWBpos2_terminate.h"
#include "UWBpos2.h"
#include "UWBpos2_data.h"
#include "rt_nonfinite.h"

/* Function Definitions */
void UWBpos2_terminate(void)
{
  UWBpos2_free();
  isInitialized_UWBpos2 = false;
}

/* End of code generation (UWBpos2_terminate.c) */
