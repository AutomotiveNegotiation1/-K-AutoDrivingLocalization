/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: UWBpos6_initialize.c
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 05-Sep-2023 20:11:41
 */

/* Include Files */
#include "UWBpos6_initialize.h"
#include "GetInitPos3.h"
#include "UWBpos6.h"
#include "UWBpos6_data.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Arguments    : void
 * Return Type  : void
 */
void UWBpos6_initialize(void)
{
  r_not_empty_init();
  UWBpos6_init();
  GetInitPos3_init();
  isInitialized_UWBpos6 = true;
}

/*
 * File trailer for UWBpos6_initialize.c
 *
 * [EOF]
 */
