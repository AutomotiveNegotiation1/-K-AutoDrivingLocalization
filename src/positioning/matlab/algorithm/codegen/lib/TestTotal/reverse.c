/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: reverse.c
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 01-Aug-2023 15:24:45
 */

/* Include Files */
#include "reverse.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : const char s_data[]
 *                const int s_size[2]
 *                char rev_data[]
 *                int rev_size[2]
 * Return Type  : void
 */
void reverse(const char s_data[], const int s_size[2], char rev_data[],
             int rev_size[2])
{
  int b_j1;
  int loop_ub;
  rev_size[0] = 1;
  rev_size[1] = s_size[1];
  loop_ub = s_size[1];
  if (loop_ub - 1 >= 0) {
    memcpy(&rev_data[0], &s_data[0], (unsigned int)loop_ub * sizeof(char));
  }
  loop_ub = s_size[1] >> 1;
  for (b_j1 = 0; b_j1 < loop_ub; b_j1++) {
    int j2;
    char xtmp;
    j2 = (s_size[1] - b_j1) - 1;
    xtmp = rev_data[b_j1];
    rev_data[b_j1] = rev_data[j2];
    rev_data[j2] = xtmp;
  }
}

/*
 * File trailer for reverse.c
 *
 * [EOF]
 */
