/*
 * minOrMax.c
 *
 * Code generation for function 'minOrMax'
 *
 */

/* Include files */
#include "minOrMax.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"

/* Function Definitions */
void minimum(const double x[16], double *ex, int *idx)
{
  double d;
  int i;
  int k;
  boolean_T exitg1;
  if (!rtIsNaN(x[0])) {
    *idx = 1;
  } else {
    *idx = 0;
    k = 2;
    exitg1 = false;
    while ((!exitg1) && (k < 17)) {
      if (!rtIsNaN(x[k - 1])) {
        *idx = k;
        exitg1 = true;
      } else {
        k++;
      }
    }
  }
  if (*idx == 0) {
    *ex = x[0];
    *idx = 1;
  } else {
    *ex = x[*idx - 1];
    i = *idx + 1;
    for (k = i; k < 17; k++) {
      d = x[k - 1];
      if (*ex > d) {
        *ex = d;
        *idx = k;
      }
    }
  }
}

/* End of code generation (minOrMax.c) */
