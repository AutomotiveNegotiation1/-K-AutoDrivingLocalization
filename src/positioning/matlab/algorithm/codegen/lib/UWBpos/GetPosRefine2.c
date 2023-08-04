/*
 * GetPosRefine2.c
 *
 * Code generation for function 'GetPosRefine2'
 *
 */

/* Include files */
#include "GetPosRefine2.h"
#include "UWBpos_data.h"
#include "UWBpos_emxutil.h"
#include "UWBpos_rtwutil.h"
#include "UWBpos_types.h"
#include "combineVectorElements.h"
#include "inv.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Definitions */
void GetPosRefine2(const double xa[6], const double ya[6],
                   const double dist_data[], const double RxID_data[],
                   const int RxID_size[2], const creal_T tag_pos_b[4],
                   double Ln, double PP, creal_T tag_pos_est[4],
                   double *heading_est, creal_T *cand_tag_pos)
{
  emxArray_boolean_T *x;
  emxArray_int32_T *b_jj;
  emxArray_real_T *A;
  emxArray_real_T *b_xa;
  emxArray_real_T *b_ya;
  emxArray_real_T *c_y;
  emxArray_real_T *calc_dist_err;
  emxArray_real_T *y;
  creal_T anch_pos_data[4];
  double a[4];
  double b_y[4];
  double Est_C_im;
  double Est_C_re;
  double Est_H;
  double Est_H_p_im;
  double Est_H_p_re;
  double ai;
  double ai_tmp;
  double ar;
  double b_r;
  double *A_data;
  double *calc_dist_err_data;
  double *xa_data;
  double *y_data;
  double *ya_data;
  int coffset;
  int idx;
  int j;
  int jj;
  int k;
  int *jj_data;
  boolean_T exitg1;
  boolean_T *x_data;
  jj = RxID_size[1];
  for (coffset = 0; coffset < jj; coffset++) {
    Est_H = RxID_data[coffset];
    Est_H_p_im = ya[(int)Est_H - 1];
    anch_pos_data[coffset].re = xa[(int)Est_H - 1] + 0.0 * Est_H_p_im;
    anch_pos_data[coffset].im = Est_H_p_im;
  }
  if (!(Ln == 1.0)) {
    coffset = (int)Ln;
    emxInit_real_T(&calc_dist_err, 2);
    jj = calc_dist_err->size[0] * calc_dist_err->size[1];
    calc_dist_err->size[0] = 1;
    calc_dist_err->size[1] = (int)Ln;
    emxEnsureCapacity_real_T(calc_dist_err, jj);
    calc_dist_err_data = calc_dist_err->data;
    for (jj = 0; jj < coffset; jj++) {
      b_r = fabs(
          rt_hypotd_snf(anch_pos_data[jj].re - tag_pos_est[(int)PP - 1].re,
                        anch_pos_data[jj].im - tag_pos_est[(int)PP - 1].im) -
          dist_data[jj]);
      calc_dist_err_data[jj] = b_r * b_r;
    }
    Est_H_p_im = sqrt(combineVectorElements(calc_dist_err) /
                      (double)calc_dist_err->size[1]) *
                 0.3;
    if (!(Est_H_p_im >= 0.2)) {
      Est_H_p_im = 0.2;
    }
    emxInit_boolean_T(&x, 2);
    coffset = x->size[0] * x->size[1];
    x->size[0] = 1;
    x->size[1] = calc_dist_err->size[1];
    emxEnsureCapacity_boolean_T(x, coffset);
    x_data = x->data;
    jj = calc_dist_err->size[1];
    for (coffset = 0; coffset < jj; coffset++) {
      x_data[coffset] = (calc_dist_err_data[coffset] < Est_H_p_im);
    }
    emxInit_int32_T(&b_jj, 2);
    jj_data = b_jj->data;
    if (x->size[1] == 0) {
      b_jj->size[0] = 1;
      b_jj->size[1] = 0;
    } else {
      idx = 0;
      coffset = b_jj->size[0] * b_jj->size[1];
      b_jj->size[0] = 1;
      b_jj->size[1] = x->size[1];
      emxEnsureCapacity_int32_T(b_jj, coffset);
      jj_data = b_jj->data;
      jj = 1;
      exitg1 = false;
      while ((!exitg1) && (jj <= x->size[1])) {
        if (x_data[jj - 1]) {
          idx++;
          jj_data[idx - 1] = jj;
          if (idx >= x->size[1]) {
            exitg1 = true;
          } else {
            jj++;
          }
        } else {
          jj++;
        }
      }
      if (x->size[1] == 1) {
        if (idx == 0) {
          b_jj->size[0] = 1;
          b_jj->size[1] = 0;
        }
      } else {
        coffset = b_jj->size[0] * b_jj->size[1];
        if (idx < 1) {
          b_jj->size[1] = 0;
        } else {
          b_jj->size[1] = idx;
        }
        emxEnsureCapacity_int32_T(b_jj, coffset);
        jj_data = b_jj->data;
      }
    }
    emxFree_boolean_T(&x);
    coffset = calc_dist_err->size[0] * calc_dist_err->size[1];
    calc_dist_err->size[0] = 1;
    calc_dist_err->size[1] = b_jj->size[1];
    emxEnsureCapacity_real_T(calc_dist_err, coffset);
    calc_dist_err_data = calc_dist_err->data;
    jj = b_jj->size[1];
    for (coffset = 0; coffset < jj; coffset++) {
      calc_dist_err_data[coffset] = jj_data[coffset];
    }
    emxFree_int32_T(&b_jj);
    if (calc_dist_err->size[1] >= 3) {
      emxInit_real_T(&b_xa, 2);
      coffset = b_xa->size[0] * b_xa->size[1];
      b_xa->size[0] = 1;
      b_xa->size[1] = calc_dist_err->size[1];
      emxEnsureCapacity_real_T(b_xa, coffset);
      xa_data = b_xa->data;
      jj = calc_dist_err->size[1];
      for (coffset = 0; coffset < jj; coffset++) {
        xa_data[coffset] =
            anch_pos_data[(int)calc_dist_err_data[coffset] - 1].re;
      }
      emxInit_real_T(&b_ya, 2);
      coffset = b_ya->size[0] * b_ya->size[1];
      b_ya->size[0] = 1;
      b_ya->size[1] = calc_dist_err->size[1];
      emxEnsureCapacity_real_T(b_ya, coffset);
      ya_data = b_ya->data;
      jj = calc_dist_err->size[1];
      for (coffset = 0; coffset < jj; coffset++) {
        ya_data[coffset] =
            anch_pos_data[(int)calc_dist_err_data[coffset] - 1].im;
      }
      idx = b_xa->size[1] - 1;
      emxInit_real_T(&A, 2);
      coffset = A->size[0] * A->size[1];
      A->size[0] = b_xa->size[1] - 1;
      A->size[1] = 2;
      emxEnsureCapacity_real_T(A, coffset);
      A_data = A->data;
      jj = (b_xa->size[1] - 1) << 1;
      for (coffset = 0; coffset < jj; coffset++) {
        A_data[coffset] = 0.0;
      }
      emxInit_real_T(&y, 1);
      coffset = y->size[0];
      y->size[0] = b_xa->size[1] - 1;
      emxEnsureCapacity_real_T(y, coffset);
      y_data = y->data;
      jj = b_xa->size[1];
      for (coffset = 0; coffset <= jj - 2; coffset++) {
        y_data[coffset] = 0.0;
      }
      coffset = b_xa->size[1];
      b_r = dist_data[(int)calc_dist_err_data[b_xa->size[1] - 1] - 1];
      for (k = 0; k <= coffset - 2; k++) {
        A_data[k] = -2.0 * (xa_data[k] - xa_data[idx]);
        A_data[k + A->size[0]] = -2.0 * (ya_data[k] - ya_data[idx]);
        Est_H_p_im = dist_data[(int)calc_dist_err_data[k] - 1];
        y_data[k] = ((Est_H_p_im * Est_H_p_im - b_r * b_r) -
                     (xa_data[k] * xa_data[k] - xa_data[idx] * xa_data[idx])) -
                    (ya_data[k] * ya_data[k] - ya_data[idx] * ya_data[idx]);
      }
      emxFree_real_T(&b_ya);
      emxFree_real_T(&b_xa);
      idx = A->size[0];
      for (j = 0; j < 2; j++) {
        coffset = j << 1;
        jj = j * A->size[0];
        b_y[coffset] = 0.0;
        b_y[coffset + 1] = 0.0;
        for (k = 0; k < idx; k++) {
          Est_H_p_im = A_data[jj + k];
          b_y[coffset] += A_data[k] * Est_H_p_im;
          b_y[coffset + 1] += A_data[A->size[0] + k] * Est_H_p_im;
        }
      }
      inv(b_y, a);
      jj = A->size[0];
      emxInit_real_T(&c_y, 2);
      coffset = c_y->size[0] * c_y->size[1];
      c_y->size[0] = 2;
      c_y->size[1] = A->size[0];
      emxEnsureCapacity_real_T(c_y, coffset);
      calc_dist_err_data = c_y->data;
      for (j = 0; j < jj; j++) {
        coffset = j << 1;
        calc_dist_err_data[coffset] =
            a[0] * A_data[j] + a[2] * A_data[A->size[0] + j];
        calc_dist_err_data[coffset + 1] =
            a[1] * A_data[j] + a[3] * A_data[A->size[0] + j];
      }
      emxFree_real_T(&A);
      idx = c_y->size[1];
      Est_H_p_im = 0.0;
      b_r = 0.0;
      for (k = 0; k < idx; k++) {
        jj = k << 1;
        Est_H_p_im += calc_dist_err_data[jj] * y_data[k];
        b_r += calc_dist_err_data[jj + 1] * y_data[k];
      }
      emxFree_real_T(&c_y);
      emxFree_real_T(&y);
      Est_H = Est_H_p_im + b_r * 0.0;
      tag_pos_est[(int)PP - 1].re = Est_H;
      tag_pos_est[(int)PP - 1].im = b_r;
      cand_tag_pos->re = Est_H;
      cand_tag_pos->im = b_r;
      ar = tag_pos_est[0].re;
      ai = tag_pos_est[0].im;
      if (ai == 0.0) {
        Est_H_p_im = ar / 4.0;
        b_r = 0.0;
      } else if (ar == 0.0) {
        Est_H_p_im = 0.0;
        b_r = ai / 4.0;
      } else {
        Est_H_p_im = ar / 4.0;
        b_r = ai / 4.0;
      }
      Est_C_re = Est_H_p_im;
      Est_C_im = b_r;
      ar = tag_pos_est[1].re;
      ai = tag_pos_est[1].im;
      if (ai == 0.0) {
        Est_H_p_im = ar / 4.0;
        b_r = 0.0;
      } else if (ar == 0.0) {
        Est_H_p_im = 0.0;
        b_r = ai / 4.0;
      } else {
        Est_H_p_im = ar / 4.0;
        b_r = ai / 4.0;
      }
      Est_C_re += Est_H_p_im;
      Est_C_im += b_r;
      ar = tag_pos_est[2].re;
      ai = tag_pos_est[2].im;
      if (ai == 0.0) {
        Est_H_p_im = ar / 4.0;
        b_r = 0.0;
      } else if (ar == 0.0) {
        Est_H_p_im = 0.0;
        b_r = ai / 4.0;
      } else {
        Est_H_p_im = ar / 4.0;
        b_r = ai / 4.0;
      }
      Est_C_re += Est_H_p_im;
      Est_C_im += b_r;
      Est_H = tag_pos_est[3].re;
      ai_tmp = tag_pos_est[3].im;
      if (ai_tmp == 0.0) {
        Est_H_p_im = Est_H / 4.0;
        b_r = 0.0;
      } else if (Est_H == 0.0) {
        Est_H_p_im = 0.0;
        b_r = ai_tmp / 4.0;
      } else {
        Est_H_p_im = Est_H / 4.0;
        b_r = ai_tmp / 4.0;
      }
      Est_C_re += Est_H_p_im;
      Est_C_im += b_r;
      ar = Est_H - Est_C_re;
      ai = ai_tmp - Est_C_im;
      if (tag_pos_b[3].im == 0.0) {
        if (ai == 0.0) {
          Est_H_p_re = ar / tag_pos_b[3].re;
          Est_H_p_im = 0.0;
        } else if (ar == 0.0) {
          Est_H_p_re = 0.0;
          Est_H_p_im = ai / tag_pos_b[3].re;
        } else {
          Est_H_p_re = ar / tag_pos_b[3].re;
          Est_H_p_im = ai / tag_pos_b[3].re;
        }
      } else if (tag_pos_b[3].re == 0.0) {
        if (ar == 0.0) {
          Est_H_p_re = ai / tag_pos_b[3].im;
          Est_H_p_im = 0.0;
        } else if (ai == 0.0) {
          Est_H_p_re = 0.0;
          Est_H_p_im = -(ar / tag_pos_b[3].im);
        } else {
          Est_H_p_re = ai / tag_pos_b[3].im;
          Est_H_p_im = -(ar / tag_pos_b[3].im);
        }
      } else {
        ai_tmp = fabs(tag_pos_b[3].re);
        b_r = fabs(tag_pos_b[3].im);
        if (ai_tmp > b_r) {
          Est_H_p_im = tag_pos_b[3].im / tag_pos_b[3].re;
          Est_H = tag_pos_b[3].re + Est_H_p_im * tag_pos_b[3].im;
          Est_H_p_re = (ar + Est_H_p_im * ai) / Est_H;
          Est_H_p_im = (ai - Est_H_p_im * ar) / Est_H;
        } else if (b_r == ai_tmp) {
          if (tag_pos_b[3].re > 0.0) {
            Est_H_p_im = 0.5;
          } else {
            Est_H_p_im = -0.5;
          }
          if (tag_pos_b[3].im > 0.0) {
            b_r = 0.5;
          } else {
            b_r = -0.5;
          }
          Est_H_p_re = (ar * Est_H_p_im + ai * b_r) / ai_tmp;
          Est_H_p_im = (ai * Est_H_p_im - ar * b_r) / ai_tmp;
        } else {
          Est_H_p_im = tag_pos_b[3].re / tag_pos_b[3].im;
          Est_H = tag_pos_b[3].im + Est_H_p_im * tag_pos_b[3].re;
          Est_H_p_re = (Est_H_p_im * ar + ai) / Est_H;
          Est_H_p_im = (Est_H_p_im * ai - ar) / Est_H;
        }
      }
      Est_H = rt_atan2d_snf(Est_H_p_im, Est_H_p_re);
      ai_tmp = Est_H * 0.0;
      if (Est_H == 0.0) {
        Est_H_p_re = exp(ai_tmp);
        Est_H_p_im = 0.0;
      } else {
        b_r = exp(ai_tmp / 2.0);
        Est_H_p_re = b_r * (b_r * cos(Est_H));
        Est_H_p_im = b_r * (b_r * sin(Est_H));
      }
      tag_pos_est[0].re = Est_C_re + (tag_pos_b[0].re * Est_H_p_re -
                                      tag_pos_b[0].im * Est_H_p_im);
      tag_pos_est[0].im = Est_C_im + (tag_pos_b[0].re * Est_H_p_im +
                                      tag_pos_b[0].im * Est_H_p_re);
      if (Est_H == 0.0) {
        Est_H_p_re = exp(ai_tmp);
        Est_H_p_im = 0.0;
      } else {
        b_r = exp(ai_tmp / 2.0);
        Est_H_p_re = b_r * (b_r * cos(Est_H));
        Est_H_p_im = b_r * (b_r * sin(Est_H));
      }
      tag_pos_est[1].re = Est_C_re + (tag_pos_b[1].re * Est_H_p_re -
                                      tag_pos_b[1].im * Est_H_p_im);
      tag_pos_est[1].im = Est_C_im + (tag_pos_b[1].re * Est_H_p_im +
                                      tag_pos_b[1].im * Est_H_p_re);
      if (Est_H == 0.0) {
        Est_H_p_re = exp(ai_tmp);
        Est_H_p_im = 0.0;
      } else {
        b_r = exp(ai_tmp / 2.0);
        Est_H_p_re = b_r * (b_r * cos(Est_H));
        Est_H_p_im = b_r * (b_r * sin(Est_H));
      }
      tag_pos_est[2].re = Est_C_re + (tag_pos_b[2].re * Est_H_p_re -
                                      tag_pos_b[2].im * Est_H_p_im);
      tag_pos_est[2].im = Est_C_im + (tag_pos_b[2].re * Est_H_p_im +
                                      tag_pos_b[2].im * Est_H_p_re);
      if (Est_H == 0.0) {
        Est_H_p_re = exp(ai_tmp);
        Est_H_p_im = 0.0;
      } else {
        b_r = exp(ai_tmp / 2.0);
        Est_H_p_re = b_r * (b_r * cos(Est_H));
        Est_H_p_im = b_r * (b_r * sin(Est_H));
      }
      tag_pos_est[3].re = Est_C_re + (tag_pos_b[3].re * Est_H_p_re -
                                      tag_pos_b[3].im * Est_H_p_im);
      tag_pos_est[3].im = Est_C_im + (tag_pos_b[3].re * Est_H_p_im +
                                      tag_pos_b[3].im * Est_H_p_re);
      *heading_est = Est_H;
    }
    emxFree_real_T(&calc_dist_err);
  }
}

/* End of code generation (GetPosRefine2.c) */
