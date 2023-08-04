/*
 * UWBpos_terminate.c
 *
 * Code generation for function 'UWBpos_terminate'
 *
 */

/* Include files */
#include "UWBpos_terminate.h"
#include "UWBpos.h"
#include "UWBpos_data.h"
#include "rt_nonfinite.h"

/* Function Definitions */
void UWBpos_terminate(void)
{
  UWBpos_free();
  isInitialized_UWBpos = false;
}

/* End of code generation (UWBpos_terminate.c) */
