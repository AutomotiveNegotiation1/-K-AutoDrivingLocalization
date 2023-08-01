/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: mod.c
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 01-Aug-2023 15:24:45
 */

/* Include Files */
#include "mod.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Definitions */
/*
 * Arguments    : double x
 * Return Type  : double
 */
double b_mod(double x)
{
  double b_r;
  if (rtIsNaN(x) || rtIsInf(x)) {
    b_r = rtNaN;
  } else if (x == 0.0) {
    b_r = 0.0;
  } else {
    bool rEQ0;
    b_r = fmod(x, 6.2831853071795862);
    rEQ0 = (b_r == 0.0);
    if (!rEQ0) {
      double q;
      q = fabs(x / 6.2831853071795862);
      rEQ0 = !(fabs(q - floor(q + 0.5)) > 2.2204460492503131E-16 * q);
    }
    if (rEQ0) {
      b_r = 0.0;
    } else if (x < 0.0) {
      b_r += 6.2831853071795862;
    }
  }
  return b_r;
}

/*
 * File trailer for mod.c
 *
 * [EOF]
 */
