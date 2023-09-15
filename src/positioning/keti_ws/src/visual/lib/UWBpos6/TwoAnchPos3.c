/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: TwoAnchPos3.c
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 05-Sep-2023 20:11:41
 */

/* Include Files */
#include "TwoAnchPos3.h"
#include "UWBpos6_emxutil.h"
#include "UWBpos6_rtwutil.h"
#include "UWBpos6_types.h"
#include "inv.h"
#include "rt_nonfinite.h"
#include "sum.h"
#include <math.h>

/* Function Declarations */
static void binary_expand_op(emxArray_real_T *in1, const double in2_data[],
                             const int in2_size[2], const emxArray_real_T *in3);

/* Function Definitions */
/*
 * Arguments    : emxArray_real_T *in1
 *                const double in2_data[]
 *                const int in2_size[2]
 *                const emxArray_real_T *in3
 * Return Type  : void
 */
static void binary_expand_op(emxArray_real_T *in1, const double in2_data[],
                             const int in2_size[2], const emxArray_real_T *in3)
{
  const double *in3_data;
  double *in1_data;
  int i;
  int loop_ub;
  int stride_0_1;
  int stride_1_1;
  in3_data = in3->data;
  i = in1->size[0] * in1->size[1];
  in1->size[0] = 1;
  if (in3->size[0] == 1) {
    in1->size[1] = in2_size[1];
  } else {
    in1->size[1] = in3->size[0];
  }
  emxEnsureCapacity_real_T(in1, i);
  in1_data = in1->data;
  stride_0_1 = (in2_size[1] != 1);
  stride_1_1 = (in3->size[0] != 1);
  if (in3->size[0] == 1) {
    loop_ub = in2_size[1];
  } else {
    loop_ub = in3->size[0];
  }
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
 *                const emxArray_real_T *dist_a
 *                double Pos[4]
 *                double Prob[2]
 * Return Type  : void
 */
void TwoAnchPos3(const double Xa[2], const double Ya[2], const double dist[2],
                 const creal_T anch_pos_data[], const int anch_pos_size[2],
                 const emxArray_real_T *dist_a, double Pos[4], double Prob[2])
{
  emxArray_real_T *x;
  emxArray_real_T *y;
  creal_T x_data[6];
  double tmp_data[6];
  double A[4];
  double b_y_tmp[4];
  double y_tmp[4];
  double X1[2];
  double X2[2];
  const double *dist_a_data;
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
  double *b_x_data;
  double *y_data;
  int tmp_size[2];
  int k;
  int nx;
  int x_size_idx_1;
  dist_a_data = dist_a->data;
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
  for (k = 0; k < 2; k++) {
    d2 = y_tmp[k + 2];
    d3 = y_tmp[k];
    b_y_tmp[k] = d3 * s + d2 * d;
    b_y_tmp[k + 2] = d3 * -X1S + d2 * d1;
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
  for (k = 0; k < 2; k++) {
    d2 = A[k + 2];
    d3 = A[k];
    X1[k] = (d3 * s + d2 * -X1S) * d_idx_0 + (d3 * d + d2 * d1) * d_idx_1_tmp;
  }
  inv(b_y_tmp, A);
  d_idx_0 = (-AA * c_d_idx_0_tmp + d_idx_0_tmp) - b_d_idx_0_tmp;
  d = y_tmp[2];
  d1 = y_tmp[3];
  for (k = 0; k < 2; k++) {
    d2 = A[k + 2];
    d3 = A[k];
    X2[k] = (d3 * s + d2 * -X1S) * d_idx_0 + (d3 * d + d2 * d1) * d_idx_1_tmp;
  }
  x_size_idx_1 = anch_pos_size[1];
  AA = X1[0] + 0.0 * X1[1];
  s = X1[1];
  nx = anch_pos_size[1];
  for (k = 0; k < nx; k++) {
    x_data[k].re = anch_pos_data[k].re - AA;
    x_data[k].im = anch_pos_data[k].im - s;
  }
  tmp_size[0] = 1;
  tmp_size[1] = anch_pos_size[1];
  for (k = 0; k < x_size_idx_1; k++) {
    tmp_data[k] = rt_hypotd_snf(x_data[k].re, x_data[k].im);
  }
  emxInit_real_T(&x, 2);
  if (dist_a->size[0] == anch_pos_size[1]) {
    k = x->size[0] * x->size[1];
    x->size[0] = 1;
    x->size[1] = anch_pos_size[1];
    emxEnsureCapacity_real_T(x, k);
    b_x_data = x->data;
    nx = anch_pos_size[1];
    for (k = 0; k < nx; k++) {
      b_x_data[k] = tmp_data[k] - dist_a_data[k];
    }
  } else {
    binary_expand_op(x, tmp_data, tmp_size, dist_a);
    b_x_data = x->data;
  }
  nx = x->size[1];
  emxInit_real_T(&y, 2);
  k = y->size[0] * y->size[1];
  y->size[0] = 1;
  y->size[1] = x->size[1];
  emxEnsureCapacity_real_T(y, k);
  y_data = y->data;
  for (k = 0; k < nx; k++) {
    y_data[k] = fabs(b_x_data[k]);
  }
  X1S = b_sum(y);
  x_size_idx_1 = anch_pos_size[1];
  AA = X2[0] + 0.0 * X2[1];
  s = X2[1];
  nx = anch_pos_size[1];
  for (k = 0; k < nx; k++) {
    x_data[k].re = anch_pos_data[k].re - AA;
    x_data[k].im = anch_pos_data[k].im - s;
  }
  tmp_size[0] = 1;
  tmp_size[1] = anch_pos_size[1];
  for (k = 0; k < x_size_idx_1; k++) {
    tmp_data[k] = rt_hypotd_snf(x_data[k].re, x_data[k].im);
  }
  if (dist_a->size[0] == anch_pos_size[1]) {
    k = x->size[0] * x->size[1];
    x->size[0] = 1;
    x->size[1] = anch_pos_size[1];
    emxEnsureCapacity_real_T(x, k);
    b_x_data = x->data;
    nx = anch_pos_size[1];
    for (k = 0; k < nx; k++) {
      b_x_data[k] = tmp_data[k] - dist_a_data[k];
    }
  } else {
    binary_expand_op(x, tmp_data, tmp_size, dist_a);
    b_x_data = x->data;
  }
  nx = x->size[1];
  k = y->size[0] * y->size[1];
  y->size[0] = 1;
  y->size[1] = x->size[1];
  emxEnsureCapacity_real_T(y, k);
  y_data = y->data;
  for (k = 0; k < nx; k++) {
    y_data[k] = fabs(b_x_data[k]);
  }
  emxFree_real_T(&x);
  d = X1S + (X1[0] * X1[0] + X1[1] * X1[1]);
  d1 = b_sum(y) + (X2[0] * X2[0] + X2[1] * X2[1]);
  emxFree_real_T(&y);
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

/*
 * File trailer for TwoAnchPos3.c
 *
 * [EOF]
 */
