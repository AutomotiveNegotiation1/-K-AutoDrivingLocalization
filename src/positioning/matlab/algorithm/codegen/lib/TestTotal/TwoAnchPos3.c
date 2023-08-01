/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: TwoAnchPos3.c
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 01-Aug-2023 16:36:28
 */

/* Include Files */
#include "TwoAnchPos3.h"
#include "TestTotal_rtwutil.h"
#include "inv.h"
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
 * Xa = [0 1];
 *  Ya = [0 0];
 *  dist = [1 1];
 *
 * Arguments    : const double Xa[2]
 *                const double Ya[2]
 *                const double dist[2]
 *                const creal_T anch_pos_data[]
 *                const int anch_pos_size[2]
 *                const double dist_a_data[]
 *                int dist_a_size
 *                double Pos[4]
 *                double Prob[2]
 * Return Type  : void
 */
void TwoAnchPos3(const double Xa[2], const double Ya[2], const double dist[2],
                 const creal_T anch_pos_data[], const int anch_pos_size[2],
                 const double dist_a_data[], int dist_a_size, double Pos[4],
                 double Prob[2])
{
  double x_data[6];
  double y_data[6];
  double A[4];
  double b_y_tmp[4];
  double y_tmp[4];
  double X1[2];
  double X2[2];
  double AA;
  double b_d;
  double b_d_idx_0_tmp;
  double b_d_tmp_tmp;
  double c_d_idx_0_tmp;
  double d;
  double d_idx_0;
  double d_idx_0_tmp;
  double d_idx_1_tmp;
  double d_tmp;
  double d_tmp_tmp;
  double s;
  int x_size[2];
  int y_size[2];
  int i;
  int k;
  int loop_ub;
  s = Xa[0] - Xa[1];
  AA = Ya[0] - Ya[1];
  AA = sqrt(s * s + AA * AA);
  /*  AA = 2;B=1;C=0.9 */
  s = ((AA + dist[0]) + dist[1]) / 2.0;
  d = s - AA;
  d_tmp_tmp = s - dist[0];
  b_d_tmp_tmp = s - dist[1];
  d_tmp = s * d * d_tmp_tmp * b_d_tmp_tmp;
  if (d_tmp > 0.0) {
    b_d = sqrt(d_tmp);
  } else {
    b_d = sqrt(-s * d * d_tmp_tmp * b_d_tmp_tmp);
  }
  d = 2.0 * b_d / AA;
  /*  if (B^2-((B^2-C^2+AA^2)/(2*AA))^2) > 0 */
  /*      d = sqrt(B^2-((B^2-C^2+AA^2)/(2*AA))^2); */
  /*  else */
  /*      d = 0; */
  /*  end */
  AA = Ya[1] - Ya[0];
  s = Xa[1] - Xa[0];
  A[1] = 2.0 * s;
  A[3] = 2.0 * AA;
  /*  Y1 =
   * [d*sqrt((Ya(2)-Ya(1))^2+(Xa(2)-Xa(1))^2)+Xa(1)*Ya(2)-Xa(2)*Ya(1);(C^2-B^2-Xa(1)^2+Xa(2)^2-Ya(1)^2+Ya(2)^2)];
   */
  /*  Y2 =
   * [-d*sqrt((Ya(2)-Ya(1))^2+(Xa(2)-Xa(1))^2)+Xa(1)*Ya(2)-Xa(2)*Ya(1);(C^2-B^2-Xa(1)^2+Xa(2)^2-Ya(1)^2+Ya(2)^2)];
   */
  y_tmp[0] = AA;
  y_tmp[1] = -s;
  y_tmp[2] = A[1];
  y_tmp[3] = A[3];
  b_d = A[1];
  d_tmp_tmp = A[3];
  for (i = 0; i < 2; i++) {
    b_d_tmp_tmp = y_tmp[i + 2];
    d_tmp = y_tmp[i];
    b_y_tmp[i] = d_tmp * AA + b_d_tmp_tmp * b_d;
    b_y_tmp[i + 2] = d_tmp * -s + b_d_tmp_tmp * d_tmp_tmp;
  }
  inv(b_y_tmp, A);
  d_idx_0_tmp = sqrt(AA * AA + s * s);
  b_d_idx_0_tmp = Xa[0] * Ya[1];
  c_d_idx_0_tmp = Ya[0] * Xa[1];
  d_idx_0 = (d * d_idx_0_tmp + b_d_idx_0_tmp) - c_d_idx_0_tmp;
  d_idx_1_tmp = ((((-(dist[1] * dist[1]) + dist[0] * dist[0]) - Xa[0] * Xa[0]) +
                  Xa[1] * Xa[1]) -
                 Ya[0] * Ya[0]) +
                Ya[1] * Ya[1];
  b_d = y_tmp[2];
  d_tmp_tmp = y_tmp[3];
  for (i = 0; i < 2; i++) {
    b_d_tmp_tmp = A[i + 2];
    d_tmp = A[i];
    X1[i] = (d_tmp * AA + b_d_tmp_tmp * -s) * d_idx_0 +
            (d_tmp * b_d + b_d_tmp_tmp * d_tmp_tmp) * d_idx_1_tmp;
  }
  inv(b_y_tmp, A);
  d_idx_0 = (-d * d_idx_0_tmp + b_d_idx_0_tmp) - c_d_idx_0_tmp;
  b_d = y_tmp[2];
  d_tmp_tmp = y_tmp[3];
  for (i = 0; i < 2; i++) {
    b_d_tmp_tmp = A[i + 2];
    d_tmp = A[i];
    X2[i] = (d_tmp * AA + b_d_tmp_tmp * -s) * d_idx_0 +
            (d_tmp * b_d + b_d_tmp_tmp * d_tmp_tmp) * d_idx_1_tmp;
  }
  AA = X1[0] + 0.0 * X1[1];
  s = X1[1];
  loop_ub = anch_pos_size[1];
  y_size[0] = 1;
  y_size[1] = anch_pos_size[1];
  for (k = 0; k < loop_ub; k++) {
    y_data[k] =
        rt_hypotd_snf(anch_pos_data[k].re - AA, anch_pos_data[k].im - s);
  }
  if (dist_a_size == anch_pos_size[1]) {
    x_size[0] = 1;
    x_size[1] = anch_pos_size[1];
    loop_ub = anch_pos_size[1];
    for (i = 0; i < loop_ub; i++) {
      x_data[i] = y_data[i] - dist_a_data[i];
    }
  } else {
    binary_expand_op(x_data, x_size, y_data, y_size, dist_a_data, &dist_a_size);
  }
  i = x_size[1];
  for (k = 0; k < i; k++) {
    y_data[k] = fabs(x_data[k]);
  }
  loop_ub = x_size[1];
  if (x_size[1] == 0) {
    d = 0.0;
  } else {
    d = y_data[0];
    for (k = 2; k <= loop_ub; k++) {
      d += y_data[k - 1];
    }
  }
  AA = X2[0] + 0.0 * X2[1];
  s = X2[1];
  loop_ub = anch_pos_size[1];
  y_size[0] = 1;
  y_size[1] = anch_pos_size[1];
  for (k = 0; k < loop_ub; k++) {
    y_data[k] =
        rt_hypotd_snf(anch_pos_data[k].re - AA, anch_pos_data[k].im - s);
  }
  if (dist_a_size == anch_pos_size[1]) {
    x_size[1] = anch_pos_size[1];
    loop_ub = anch_pos_size[1];
    for (i = 0; i < loop_ub; i++) {
      x_data[i] = y_data[i] - dist_a_data[i];
    }
  } else {
    binary_expand_op(x_data, x_size, y_data, y_size, dist_a_data, &dist_a_size);
  }
  i = x_size[1];
  for (k = 0; k < i; k++) {
    y_data[k] = fabs(x_data[k]);
  }
  loop_ub = x_size[1];
  if (x_size[1] == 0) {
    s = 0.0;
  } else {
    s = y_data[0];
    for (k = 2; k <= loop_ub; k++) {
      s += y_data[k - 1];
    }
  }
  b_d = s + (X2[0] * X2[0] + X2[1] * X2[1]);
  d_tmp_tmp = d + (X1[0] * X1[0] + X1[1] * X1[1]);
  if (d_tmp_tmp > b_d) {
    Pos[0] = X2[0];
    Pos[1] = X1[0];
    Pos[2] = X2[1];
    Pos[3] = X1[1];
    Prob[0] = b_d;
    Prob[1] = d_tmp_tmp;
  } else {
    Pos[0] = X1[0];
    Pos[1] = X2[0];
    Pos[2] = X1[1];
    Pos[3] = X2[1];
    Prob[0] = d_tmp_tmp;
    Prob[1] = b_d;
  }
  /*  if (X1S+Y1S)-(X2S+Y2S)>0.1 */
  /*      Pos = [X2']; */
  /*  elseif ((X1S+Y1S)-(X2S+Y2S))<-0.1 */
  /*      Pos = [X1']; */
  /*  elseif Z1S < Z2S */
  /*      Pos = [X1']; */
  /*  else */
  /*      Pos = [X2']; */
  /*  end */
  /*  if sum((X1-EstCenter').^2)>sum((X2-EstCenter').^2) */
  /*      Pos = [X2']; */
  /*  else */
  /*      Pos = [X1']; */
  /*  end */
}

/*
 * File trailer for TwoAnchPos3.c
 *
 * [EOF]
 */
