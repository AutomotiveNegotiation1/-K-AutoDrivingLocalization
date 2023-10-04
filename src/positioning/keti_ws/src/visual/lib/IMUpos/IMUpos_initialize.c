/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: IMUpos_initialize.c
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 04-Oct-2023 14:13:20
 */

/* Include Files */
#include "IMUpos_initialize.h"
#include "EulerKalman_2.h"
#include "IMUpos.h"
#include "IMUpos_data.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Arguments    : void
 * Return Type  : void
 */
void IMUpos_initialize(void)
{
  IMUpos_init();
  EulerKalman_2_init();
  isInitialized_IMUpos = true;
}

/*
 * File trailer for IMUpos_initialize.c
 *
 * [EOF]
 */
