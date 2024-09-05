/*
 * mod1.c
 *
 * Code generation for function 'mod1'
 *
 */

/* Include files */
#include "mod1.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Definitions */
double c_mod(double x)
{
  double q;
  double r;
  boolean_T rEQ0;
  if (rtIsNaN(x) || rtIsInf(x)) {
    r = rtNaN;
  } else if (x == 0.0) {
    r = 0.0;
  } else {
    r = fmod(x, 3.1415926535897931);
    rEQ0 = (r == 0.0);
    if (!rEQ0) {
      q = fabs(x / 3.1415926535897931);
      rEQ0 = !(fabs(q - floor(q + 0.5)) > 2.2204460492503131E-16 * q);
    }
    if (rEQ0) {
      r = 0.0;
    } else if (x < 0.0) {
      r += 3.1415926535897931;
    }
  }
  return r;
}

/* End of code generation (mod1.c) */
