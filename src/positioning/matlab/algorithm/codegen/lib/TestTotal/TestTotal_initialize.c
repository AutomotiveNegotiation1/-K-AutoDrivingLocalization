/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: TestTotal_initialize.c
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 01-Aug-2023 16:36:28
 */

/* Include Files */
#include "TestTotal_initialize.h"
#include "TestTotal_data.h"
#include "UWBpos.h"
#include "eml_rand_mt19937ar_stateful.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Arguments    : void
 * Return Type  : void
 */
void TestTotal_initialize(void)
{
  c_eml_rand_mt19937ar_stateful_i();
  UWBpos_init();
  isInitialized_TestTotal = true;
}

/*
 * File trailer for TestTotal_initialize.c
 *
 * [EOF]
 */
