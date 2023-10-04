/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: IMUpos_terminate.c
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 04-Oct-2023 14:13:20
 */

/* Include Files */
#include "IMUpos_terminate.h"
#include "IMUpos_data.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Arguments    : void
 * Return Type  : void
 */
void IMUpos_terminate(void)
{
  isInitialized_IMUpos = false;
}

/*
 * File trailer for IMUpos_terminate.c
 *
 * [EOF]
 */
