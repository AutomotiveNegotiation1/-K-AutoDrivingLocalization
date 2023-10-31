/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: ReliCalc.c
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 22-Sep-2023 10:38:00
 */

/* Include Files */
#include "ReliCalc.h"
#include "EulerKalman_2_data.h"
#include "EulerKalman_2_initialize.h"
#include "EulerKalman_2_rtwutil.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Declarations */
static void binary_expand_op(double in1_data[], int in1_size[2],
                             const double in2_data[], const int in2_size[2],
                             const double in3_data[], const int *in3_size);

/* Function Definitions */
/*
 * Arguments    : double in1_data[]
 *                int in1_size[2]
 *                const double in2_data[]
 *                const int in2_size[2]
 *                const double in3_data[]
 *                const int *in3_size
 * Return Type  : void
 */
static void binary_expand_op(double in1_data[], int in1_size[2],
                             const double in2_data[], const int in2_size[2],
                             const double in3_data[], const int *in3_size)
{
  int i;
  int loop_ub;
  int stride_0_1;
  int stride_1_1;
  in1_size[0] = 1;
  if (*in3_size == 1) {
    loop_ub = in2_size[1];
  } else {
    loop_ub = *in3_size;
  }
  in1_size[1] = loop_ub;
  stride_0_1 = (in2_size[1] != 1);
  stride_1_1 = (*in3_size != 1);
  for (i = 0; i < loop_ub; i++) {
    in1_data[i] = in2_data[i * stride_0_1] - in3_data[i * stride_1_1];
  }
}

/*
 * Arguments    : const creal_T tag_pos_est[4]
 *                const creal_T anchor_pos[6]
 *                const double RxDist_data[]
 *                const int RxDist_size[1]
 *                const double RxID_data[]
 *                const int RxID_size[2]
 *                double PP
 * Return Type  : double
 */
double ReliCalc(const creal_T tag_pos_est[4], const creal_T anchor_pos[6],
                const double RxDist_data[], const int RxDist_size[1],
                const double RxID_data[], const int RxID_size[2], double PP)
{
  double TT_data[4];
  double x_data[4];
  int TT_size[2];
  int x_size[2];
  int b_k;
  int i;
  int loop_ub;
  if (!isInitialized_EulerKalman_2) {
    EulerKalman_2_initialize();
  }
  loop_ub = RxID_size[1];
  TT_size[0] = 1;
  TT_size[1] = RxID_size[1];
  for (b_k = 0; b_k < loop_ub; b_k++) {
    i = (int)RxID_data[b_k] - 1;
    TT_data[b_k] =
        rt_hypotd_snf(tag_pos_est[(int)PP - 1].re - anchor_pos[i].re,
                      tag_pos_est[(int)PP - 1].im - anchor_pos[i].im);
  }
  if (RxDist_size[0] == RxID_size[1]) {
    x_size[1] = RxID_size[1];
    loop_ub = RxID_size[1];
    for (i = 0; i < loop_ub; i++) {
      x_data[i] = TT_data[i] - RxDist_data[i];
    }
  } else {
    binary_expand_op(x_data, x_size, TT_data, TT_size, RxDist_data,
                     &RxDist_size[0]);
  }
  i = x_size[1];
  for (b_k = 0; b_k < i; b_k++) {
    TT_data[b_k] = fabs(x_data[b_k]);
  }
  return 1.0 / sqrt(((TT_data[0] * TT_data[0] + TT_data[1] * TT_data[1]) +
                     TT_data[2] * TT_data[2]) /
                    3.0);
}

/*
 * File trailer for ReliCalc.c
 *
 * [EOF]
 */
