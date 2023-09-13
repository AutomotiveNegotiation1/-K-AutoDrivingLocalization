/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: minOrMax.c
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 05-Sep-2023 20:11:41
 */

/* Include Files */
#include "minOrMax.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Arguments    : const double x[3]
 *                double *ex
 *                int *idx
 * Return Type  : void
 */
void minimum(const double x[3], double *ex, int *idx)
{
  int k;
  if (!rtIsNaN(x[0])) {
    *idx = 1;
  } else {
    bool exitg1;
    *idx = 0;
    k = 2;
    exitg1 = false;
    while ((!exitg1) && (k < 4)) {
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
    int i;
    *ex = x[*idx - 1];
    i = *idx + 1;
    for (k = i; k < 4; k++) {
      double d;
      d = x[k - 1];
      if (*ex > d) {
        *ex = d;
        *idx = k;
      }
    }
  }
}

/*
 * File trailer for minOrMax.c
 *
 * [EOF]
 */
