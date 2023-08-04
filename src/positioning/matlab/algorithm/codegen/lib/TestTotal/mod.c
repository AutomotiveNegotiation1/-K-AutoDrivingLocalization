/*
 * mod.c
 *
 * Code generation for function 'mod'
 *
 */

/* Include files */
#include "mod.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Definitions */
double b_mod(double x)
{
  double b_r;
  double q;
  boolean_T rEQ0;
  if (rtIsNaN(x) || rtIsInf(x)) {
    b_r = rtNaN;
  } else if (x == 0.0) {
    b_r = 0.0;
  } else {
    b_r = fmod(x, 6.2831853071795862);
    rEQ0 = (b_r == 0.0);
    if (!rEQ0) {
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

/* End of code generation (mod.c) */
