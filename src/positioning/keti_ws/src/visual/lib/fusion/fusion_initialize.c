/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: fusion_initialize.c
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 04-Oct-2023 13:17:07
 */

/* Include Files */
#include "fusion_initialize.h"
#include "EulerKalman_2.h"
#include "PosKalman2.h"
#include "fusion.h"
#include "fusion_data.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Arguments    : void
 * Return Type  : void
 */
void fusion_initialize(void)
{
  fusion_init();
  EulerKalman_2_init();
  PosKalman2_init();
  isInitialized_fusion = true;
}

/*
 * File trailer for fusion_initialize.c
 *
 * [EOF]
 */
