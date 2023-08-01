/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: TwoAnchPos3.c
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 01-Aug-2023 14:00:18
 */

/* Include Files */
#include "TwoAnchPos3.h"
#include "UWBpos_emxutil.h"
#include "UWBpos_rtwutil.h"
#include "UWBpos_types.h"
#include "combineVectorElements.h"
#include "inv.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Declarations */
static void binary_expand_op(emxArray_real_T *in1, const emxArray_real_T *in2,
                             const emxArray_real_T *in3);

/* Function Definitions */
/*
 * Arguments    : emxArray_real_T *in1
 *                const emxArray_real_T *in2
 *                const emxArray_real_T *in3
 * Return Type  : void
 */
static void binary_expand_op(emxArray_real_T *in1, const emxArray_real_T *in2,
                             const emxArray_real_T *in3)
{
  const double *in2_data;
  const double *in3_data;
  double *in1_data;
  int i;
  int loop_ub;
  int stride_0_1;
  int stride_1_1;
  in3_data = in3->data;
  in2_data = in2->data;
  i = in1->size[0] * in1->size[1];
  in1->size[0] = 1;
  emxEnsureCapacity_real_T(in1, i);
  if (in3->size[0] == 1) {
    loop_ub = in2->size[1];
  } else {
    loop_ub = in3->size[0];
  }
  i = in1->size[0] * in1->size[1];
  in1->size[1] = loop_ub;
  emxEnsureCapacity_real_T(in1, i);
  in1_data = in1->data;
  stride_0_1 = (in2->size[1] != 1);
  stride_1_1 = (in3->size[0] != 1);
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
 *                const emxArray_creal_T *anch_pos
 *                const emxArray_real_T *dist_a
 *                double Pos[4]
 *                double Prob[2]
 * Return Type  : void
 */
void TwoAnchPos3(const double Xa[2], const double Ya[2], const double dist[2],
                 const emxArray_creal_T *anch_pos,
                 const emxArray_real_T *dist_a, double Pos[4], double Prob[2])
{
  emxArray_creal_T *x;
  emxArray_real_T *b_x;
  emxArray_real_T *y;
  const creal_T *anch_pos_data;
  creal_T *x_data;
  double A[4];
  double b_y_tmp[4];
  double y_tmp[4];
  double X1[2];
  double X2[2];
  const double *dist_a_data;
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
  double *b_x_data;
  double *y_data;
  int k;
  int nx;
  dist_a_data = dist_a->data;
  anch_pos_data = anch_pos->data;
  AA = Xa[0] - Xa[1];
  s = Ya[0] - Ya[1];
  AA = sqrt(AA * AA + s * s);
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
  for (k = 0; k < 2; k++) {
    b_d_tmp_tmp = y_tmp[k + 2];
    d_tmp = y_tmp[k];
    b_y_tmp[k] = d_tmp * AA + b_d_tmp_tmp * b_d;
    b_y_tmp[k + 2] = d_tmp * -s + b_d_tmp_tmp * d_tmp_tmp;
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
  for (k = 0; k < 2; k++) {
    b_d_tmp_tmp = A[k + 2];
    d_tmp = A[k];
    X1[k] = (d_tmp * AA + b_d_tmp_tmp * -s) * d_idx_0 +
            (d_tmp * b_d + b_d_tmp_tmp * d_tmp_tmp) * d_idx_1_tmp;
  }
  inv(b_y_tmp, A);
  d_idx_0 = (-d * d_idx_0_tmp + b_d_idx_0_tmp) - c_d_idx_0_tmp;
  b_d = y_tmp[2];
  d_tmp_tmp = y_tmp[3];
  for (k = 0; k < 2; k++) {
    b_d_tmp_tmp = A[k + 2];
    d_tmp = A[k];
    X2[k] = (d_tmp * AA + b_d_tmp_tmp * -s) * d_idx_0 +
            (d_tmp * b_d + b_d_tmp_tmp * d_tmp_tmp) * d_idx_1_tmp;
  }
  emxInit_creal_T(&x, 2);
  k = x->size[0] * x->size[1];
  x->size[0] = 1;
  x->size[1] = anch_pos->size[1];
  emxEnsureCapacity_creal_T(x, k);
  x_data = x->data;
  AA = X1[0] + 0.0 * X1[1];
  s = X1[1];
  nx = anch_pos->size[1];
  for (k = 0; k < nx; k++) {
    x_data[k].re = anch_pos_data[k].re - AA;
    x_data[k].im = anch_pos_data[k].im - s;
  }
  nx = x->size[1];
  emxInit_real_T(&y, 2);
  k = y->size[0] * y->size[1];
  y->size[0] = 1;
  y->size[1] = x->size[1];
  emxEnsureCapacity_real_T(y, k);
  y_data = y->data;
  for (k = 0; k < nx; k++) {
    y_data[k] = rt_hypotd_snf(x_data[k].re, x_data[k].im);
  }
  emxInit_real_T(&b_x, 2);
  if (dist_a->size[0] == y->size[1]) {
    k = b_x->size[0] * b_x->size[1];
    b_x->size[0] = 1;
    b_x->size[1] = y->size[1];
    emxEnsureCapacity_real_T(b_x, k);
    b_x_data = b_x->data;
    nx = y->size[1];
    for (k = 0; k < nx; k++) {
      b_x_data[k] = y_data[k] - dist_a_data[k];
    }
  } else {
    binary_expand_op(b_x, y, dist_a);
    b_x_data = b_x->data;
  }
  nx = b_x->size[1];
  k = y->size[0] * y->size[1];
  y->size[0] = 1;
  y->size[1] = b_x->size[1];
  emxEnsureCapacity_real_T(y, k);
  y_data = y->data;
  for (k = 0; k < nx; k++) {
    y_data[k] = fabs(b_x_data[k]);
  }
  d = combineVectorElements(y);
  k = x->size[0] * x->size[1];
  x->size[0] = 1;
  x->size[1] = anch_pos->size[1];
  emxEnsureCapacity_creal_T(x, k);
  x_data = x->data;
  AA = X2[0] + 0.0 * X2[1];
  s = X2[1];
  nx = anch_pos->size[1];
  for (k = 0; k < nx; k++) {
    x_data[k].re = anch_pos_data[k].re - AA;
    x_data[k].im = anch_pos_data[k].im - s;
  }
  nx = x->size[1];
  k = y->size[0] * y->size[1];
  y->size[0] = 1;
  y->size[1] = x->size[1];
  emxEnsureCapacity_real_T(y, k);
  y_data = y->data;
  for (k = 0; k < nx; k++) {
    y_data[k] = rt_hypotd_snf(x_data[k].re, x_data[k].im);
  }
  emxFree_creal_T(&x);
  if (dist_a->size[0] == y->size[1]) {
    k = b_x->size[0] * b_x->size[1];
    b_x->size[0] = 1;
    b_x->size[1] = y->size[1];
    emxEnsureCapacity_real_T(b_x, k);
    b_x_data = b_x->data;
    nx = y->size[1];
    for (k = 0; k < nx; k++) {
      b_x_data[k] = y_data[k] - dist_a_data[k];
    }
  } else {
    binary_expand_op(b_x, y, dist_a);
    b_x_data = b_x->data;
  }
  nx = b_x->size[1];
  k = y->size[0] * y->size[1];
  y->size[0] = 1;
  y->size[1] = b_x->size[1];
  emxEnsureCapacity_real_T(y, k);
  y_data = y->data;
  for (k = 0; k < nx; k++) {
    y_data[k] = fabs(b_x_data[k]);
  }
  emxFree_real_T(&b_x);
  b_d = combineVectorElements(y) + (X2[0] * X2[0] + X2[1] * X2[1]);
  emxFree_real_T(&y);
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
