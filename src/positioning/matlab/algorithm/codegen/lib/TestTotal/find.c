/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: find.c
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 01-Aug-2023 15:24:45
 */

/* Include Files */
#include "find.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Arguments    : const bool x[6]
 *                int i_data[]
 * Return Type  : int
 */
int eml_find(const bool x[6], int i_data[])
{
  int i_size;
  int ii;
  bool exitg1;
  i_size = 0;
  ii = 0;
  exitg1 = false;
  while ((!exitg1) && (ii < 6)) {
    if (x[ii]) {
      i_size++;
      i_data[i_size - 1] = ii + 1;
      if (i_size >= 6) {
        exitg1 = true;
      } else {
        ii++;
      }
    } else {
      ii++;
    }
  }
  if (i_size < 1) {
    i_size = 0;
  }
  return i_size;
}

/*
 * File trailer for find.c
 *
 * [EOF]
 */
