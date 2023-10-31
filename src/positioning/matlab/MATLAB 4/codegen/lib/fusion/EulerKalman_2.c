/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: EulerKalman_2.c
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 22-Sep-2023 10:33:23
 */

/* Include Files */
#include "EulerKalman_2.h"
#include "fusion_data.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Arguments    : void
 * Return Type  : void
 */
void EulerKalman_2_init(void)
{
  static const signed char b_iv[16] = {100, 0, 0,   0, 0, 100, 0, 0,
                                       0,   0, 100, 0, 0, 0,   0, 100};
  int i;
  /*      x = [1 0 0 0]'; */
  for (i = 0; i < 16; i++) {
    int i1;
    i1 = iv[i];
    Q[i] = i1;
    R[i] = b_iv[i];
    H[i] = 0.0;
    P[i] = i1;
  }
  H[0] = 1.0;
  H[5] = 1.0;
  H[10] = 1.0;
  H[15] = 1.0;
}

/*
 * File trailer for EulerKalman_2.c
 *
 * [EOF]
 */
