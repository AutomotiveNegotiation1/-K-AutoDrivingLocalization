/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: exp.c
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 01-Aug-2023 15:24:45
 */

/* Include Files */
#include "exp.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Definitions */
/*
 * Arguments    : creal_T *x
 * Return Type  : void
 */
void b_exp(creal_T *x)
{
  if (x->re == 0.0) {
    double d;
    d = x->im;
    x->re = cos(d);
    x->im = sin(d);
  } else if (x->im == 0.0) {
    x->re = exp(x->re);
    x->im = 0.0;
  } else if (rtIsInf(x->im) && rtIsInf(x->re) && (x->re < 0.0)) {
    x->re = 0.0;
    x->im = 0.0;
  } else {
    double b_r;
    double d;
    b_r = exp(x->re / 2.0);
    d = x->im;
    x->re = b_r * (b_r * cos(d));
    x->im = b_r * (b_r * sin(d));
  }
}

/*
 * File trailer for exp.c
 *
 * [EOF]
 */
