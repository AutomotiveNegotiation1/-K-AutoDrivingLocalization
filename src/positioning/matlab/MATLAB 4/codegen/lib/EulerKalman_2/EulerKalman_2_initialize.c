/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: EulerKalman_2_initialize.c
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 22-Sep-2023 10:38:00
 */

/* Include Files */
#include "EulerKalman_2_initialize.h"
#include "EulerKalman_2.h"
#include "EulerKalman_2_data.h"
#include "PosKalman2.h"
#include "fusion.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Arguments    : void
 * Return Type  : void
 */
void EulerKalman_2_initialize(void)
{
  EulerKalman_2_init();
  fusion_init();
  PosKalman2_init();
  isInitialized_EulerKalman_2 = true;
}

/*
 * File trailer for EulerKalman_2_initialize.c
 *
 * [EOF]
 */
