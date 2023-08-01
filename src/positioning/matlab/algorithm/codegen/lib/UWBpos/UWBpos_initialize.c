/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: UWBpos_initialize.c
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 01-Aug-2023 14:00:18
 */

/* Include Files */
#include "UWBpos_initialize.h"
#include "UWBpos.h"
#include "UWBpos_data.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Arguments    : void
 * Return Type  : void
 */
void UWBpos_initialize(void)
{
  r_not_empty_init();
  UWBpos_init();
  isInitialized_UWBpos = true;
}

/*
 * File trailer for UWBpos_initialize.c
 *
 * [EOF]
 */
