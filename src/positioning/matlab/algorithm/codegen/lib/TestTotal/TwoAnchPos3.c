/*
 * TwoAnchPos3.c
 *
 * Code generation for function 'TwoAnchPos3'
 *
 */

/* Include files */
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
    in1_size[1] = in2_size[1];
  } else {
    in1_size[1] = *in3_size;
  }
  stride_0_1 = (in2_size[1] != 1);
  stride_1_1 = (*in3_size != 1);
  if (*in3_size == 1) {
    loop_ub = in2_size[1];
  } else {
    loop_ub = *in3_size;
  }
  for (i = 0; i < loop_ub; i++) {
    in1_data[i] = in2_data[i * stride_0_1] - in3_data[i * stride_1_1];
  }
}

void TwoAnchPos3(const double Xa[2], const double Ya[2], const double dist[2],
                 const creal_T anch_pos_data[], const int anch_pos_size[2],
                 const double dist_a_data[], int dist_a_size, double Pos[4],
                 double Prob[2])
{
  creal_T x_data[6];
  double b_x_data[6];
  double y_data[6];
  double A[4];
  double b_y_tmp[4];
  double y_tmp[4];
  double X1[2];
  double X2[2];
  double AA;
  double X1S;
  double b_d_idx_0_tmp;
  double c_d_idx_0_tmp;
  double d;
  double d1;
  double d2;
  double d3;
  double d_idx_0;
  double d_idx_0_tmp;
  double d_idx_1_tmp;
  double s;
  int x_size[2];
  int y_size[2];
  int i;
  int k;
  int loop_ub;
  int x_size_idx_1;
  /*  Xa = [0 1]; */
  /*  Ya = [0 0]; */
  /*  dist = [1 1]; */
  AA = Xa[0] - Xa[1];
  s = Ya[0] - Ya[1];
  AA = sqrt(AA * AA + s * s);
  /*  AA = 2;B=1;C=0.9 */
  s = ((AA + dist[0]) + dist[1]) / 2.0;
  if (s * (s - AA) * (s - dist[0]) * (s - dist[1]) > 0.0) {
    d = sqrt(s * (s - AA) * (s - dist[0]) * (s - dist[1]));
  } else {
    d = sqrt(-s * (s - AA) * (s - dist[0]) * (s - dist[1]));
  }
  AA = 2.0 * d / AA;
  /*  if (B^2-((B^2-C^2+AA^2)/(2*AA))^2) > 0 */
  /*      d = sqrt(B^2-((B^2-C^2+AA^2)/(2*AA))^2); */
  /*  else */
  /*      d = 0; */
  /*  end */
  s = Ya[1] - Ya[0];
  X1S = Xa[1] - Xa[0];
  A[1] = 2.0 * X1S;
  A[3] = 2.0 * s;
  /*  Y1 =
   * [d*sqrt((Ya(2)-Ya(1))^2+(Xa(2)-Xa(1))^2)+Xa(1)*Ya(2)-Xa(2)*Ya(1);(C^2-B^2-Xa(1)^2+Xa(2)^2-Ya(1)^2+Ya(2)^2)];
   */
  /*  Y2 =
   * [-d*sqrt((Ya(2)-Ya(1))^2+(Xa(2)-Xa(1))^2)+Xa(1)*Ya(2)-Xa(2)*Ya(1);(C^2-B^2-Xa(1)^2+Xa(2)^2-Ya(1)^2+Ya(2)^2)];
   */
  y_tmp[0] = s;
  y_tmp[1] = -X1S;
  y_tmp[2] = A[1];
  y_tmp[3] = A[3];
  d = A[1];
  d1 = A[3];
  for (i = 0; i < 2; i++) {
    d2 = y_tmp[i + 2];
    d3 = y_tmp[i];
    b_y_tmp[i] = d3 * s + d2 * d;
    b_y_tmp[i + 2] = d3 * -X1S + d2 * d1;
  }
  inv(b_y_tmp, A);
  d_idx_0_tmp = Xa[0] * Ya[1];
  b_d_idx_0_tmp = Ya[0] * Xa[1];
  c_d_idx_0_tmp = sqrt(s * s + X1S * X1S);
  d_idx_0 = (AA * c_d_idx_0_tmp + d_idx_0_tmp) - b_d_idx_0_tmp;
  d_idx_1_tmp = ((((-(dist[1] * dist[1]) + dist[0] * dist[0]) - Xa[0] * Xa[0]) +
                  Xa[1] * Xa[1]) -
                 Ya[0] * Ya[0]) +
                Ya[1] * Ya[1];
  d = y_tmp[2];
  d1 = y_tmp[3];
  for (i = 0; i < 2; i++) {
    d2 = A[i + 2];
    d3 = A[i];
    X1[i] = (d3 * s + d2 * -X1S) * d_idx_0 + (d3 * d + d2 * d1) * d_idx_1_tmp;
  }
  inv(b_y_tmp, A);
  d_idx_0 = (-AA * c_d_idx_0_tmp + d_idx_0_tmp) - b_d_idx_0_tmp;
  d = y_tmp[2];
  d1 = y_tmp[3];
  for (i = 0; i < 2; i++) {
    d2 = A[i + 2];
    d3 = A[i];
    X2[i] = (d3 * s + d2 * -X1S) * d_idx_0 + (d3 * d + d2 * d1) * d_idx_1_tmp;
  }
  x_size_idx_1 = anch_pos_size[1];
  AA = X1[0] + 0.0 * X1[1];
  s = X1[1];
  loop_ub = anch_pos_size[1];
  for (i = 0; i < loop_ub; i++) {
    x_data[i].re = anch_pos_data[i].re - AA;
    x_data[i].im = anch_pos_data[i].im - s;
  }
  y_size[0] = 1;
  y_size[1] = anch_pos_size[1];
  for (k = 0; k < x_size_idx_1; k++) {
    y_data[k] = rt_hypotd_snf(x_data[k].re, x_data[k].im);
  }
  if (dist_a_size == anch_pos_size[1]) {
    x_size[0] = 1;
    x_size[1] = anch_pos_size[1];
    loop_ub = anch_pos_size[1];
    for (i = 0; i < loop_ub; i++) {
      b_x_data[i] = y_data[i] - dist_a_data[i];
    }
  } else {
    binary_expand_op(b_x_data, x_size, y_data, y_size, dist_a_data,
                     &dist_a_size);
  }
  i = x_size[1];
  for (k = 0; k < i; k++) {
    y_data[k] = fabs(b_x_data[k]);
  }
  loop_ub = x_size[1];
  if (x_size[1] == 0) {
    X1S = 0.0;
  } else {
    X1S = y_data[0];
    for (k = 2; k <= loop_ub; k++) {
      X1S += y_data[k - 1];
    }
  }
  x_size_idx_1 = anch_pos_size[1];
  AA = X2[0] + 0.0 * X2[1];
  s = X2[1];
  loop_ub = anch_pos_size[1];
  for (i = 0; i < loop_ub; i++) {
    x_data[i].re = anch_pos_data[i].re - AA;
    x_data[i].im = anch_pos_data[i].im - s;
  }
  y_size[0] = 1;
  y_size[1] = anch_pos_size[1];
  for (k = 0; k < x_size_idx_1; k++) {
    y_data[k] = rt_hypotd_snf(x_data[k].re, x_data[k].im);
  }
  if (dist_a_size == anch_pos_size[1]) {
    x_size[1] = anch_pos_size[1];
    loop_ub = anch_pos_size[1];
    for (i = 0; i < loop_ub; i++) {
      b_x_data[i] = y_data[i] - dist_a_data[i];
    }
  } else {
    binary_expand_op(b_x_data, x_size, y_data, y_size, dist_a_data,
                     &dist_a_size);
  }
  i = x_size[1];
  for (k = 0; k < i; k++) {
    y_data[k] = fabs(b_x_data[k]);
  }
  loop_ub = x_size[1];
  if (x_size[1] == 0) {
    AA = 0.0;
  } else {
    AA = y_data[0];
    for (k = 2; k <= loop_ub; k++) {
      AA += y_data[k - 1];
    }
  }
  d = X1S + (X1[0] * X1[0] + X1[1] * X1[1]);
  d1 = AA + (X2[0] * X2[0] + X2[1] * X2[1]);
  if (d > d1) {
    Pos[0] = X2[0];
    Pos[1] = X1[0];
    Pos[2] = X2[1];
    Pos[3] = X1[1];
    Prob[0] = d1;
    Prob[1] = d;
  } else {
    Pos[0] = X1[0];
    Pos[1] = X2[0];
    Pos[2] = X1[1];
    Pos[3] = X2[1];
    Prob[0] = d;
    Prob[1] = d1;
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

/* End of code generation (TwoAnchPos3.c) */
