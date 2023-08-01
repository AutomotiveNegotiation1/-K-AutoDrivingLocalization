/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: eml_rand_mt19937ar_stateful.c
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 01-Aug-2023 16:36:28
 */

/* Include Files */
#include "eml_rand_mt19937ar_stateful.h"
#include "TestTotal_data.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : void
 * Return Type  : void
 */
void c_eml_rand_mt19937ar_stateful_i(void)
{
  unsigned int b_r;
  int mti;
  memset(&state[0], 0, 625U * sizeof(unsigned int));
  b_r = 5489U;
  state[0] = 5489U;
  for (mti = 0; mti < 623; mti++) {
    b_r = ((b_r ^ b_r >> 30U) * 1812433253U + (unsigned int)mti) + 1U;
    state[mti + 1] = b_r;
  }
  state[624] = 624U;
}

/*
 * File trailer for eml_rand_mt19937ar_stateful.c
 *
 * [EOF]
 */
