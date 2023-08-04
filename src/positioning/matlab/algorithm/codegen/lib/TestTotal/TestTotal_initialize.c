/*
 * TestTotal_initialize.c
 *
 * Code generation for function 'TestTotal_initialize'
 *
 */

/* Include files */
#include "TestTotal_initialize.h"
#include "CoderTimeAPI.h"
#include "TestTotal_data.h"
#include "UWBpos.h"
#include "eml_rand_mt19937ar_stateful.h"
#include "rt_nonfinite.h"
#include "timeKeeper.h"

/* Function Definitions */
void TestTotal_initialize(void)
{
  rt_InitInfAndNaN();
  savedTime_not_empty_init();
  freq_not_empty_init();
  c_eml_rand_mt19937ar_stateful_i();
  UWBpos_init();
  isInitialized_TestTotal = true;
}

/* End of code generation (TestTotal_initialize.c) */
