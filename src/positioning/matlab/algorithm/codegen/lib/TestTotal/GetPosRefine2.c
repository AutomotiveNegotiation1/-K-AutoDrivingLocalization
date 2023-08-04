/*
 * GetPosRefine2.c
 *
 * Code generation for function 'GetPosRefine2'
 *
 */

/* Include files */
#include "GetPosRefine2.h"
#include "TestTotal_data.h"
#include "TestTotal_rtwutil.h"
#include "inv.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Definitions */
void GetPosRefine2(const double dist_data[], const double RxID_data[],
                   const int RxID_size[2], double Ln, double PP,
                   creal_T tag_pos_est[4], double *heading_est,
                   creal_T *cand_tag_pos)
{
  static const signed char iv[6] = {10, 10, -10, -10, 20, -20};
  static const signed char iv1[6] = {10, -10, -10, 10, 15, -15};
  cint8_T anch_pos_data[4];
  double b_y_data[6];
  double a[4];
  double calc_dist_err_data[4];
  double y_data[3];
  double Est_C_im;
  double Est_C_re;
  double Est_H;
  double Est_H_p_im;
  double Est_H_p_re;
  double ai;
  double ai_tmp;
  double ar;
  double b_r;
  int M_tmp;
  int coffset;
  int idx;
  int j;
  int jj;
  int jj_size_idx_1;
  int k;
  short A_data[6];
  signed char jj_data[4];
  signed char ya_data[4];
  boolean_T x_data[4];
  boolean_T exitg1;
  idx = RxID_size[1];
  for (jj = 0; jj < idx; jj++) {
    Est_H = RxID_data[jj];
    anch_pos_data[jj].re = iv[(int)Est_H - 1];
    anch_pos_data[jj].im = iv1[(int)Est_H - 1];
  }
  idx = (int)Ln;
  for (jj = 0; jj < idx; jj++) {
    b_r = fabs(rt_hypotd_snf(
                   (double)anch_pos_data[jj].re - tag_pos_est[(int)PP - 1].re,
                   (double)anch_pos_data[jj].im - tag_pos_est[(int)PP - 1].im) -
               dist_data[jj]);
    calc_dist_err_data[jj] = b_r * b_r;
  }
  if ((int)Ln == 0) {
    Est_H_p_im = 0.0;
  } else {
    Est_H_p_im = calc_dist_err_data[0];
    for (k = 2; k <= idx; k++) {
      Est_H_p_im += calc_dist_err_data[k - 1];
    }
  }
  Est_H_p_im = sqrt(Est_H_p_im / (double)(int)Ln) * 0.3;
  if (!(Est_H_p_im >= 0.2)) {
    Est_H_p_im = 0.2;
  }
  for (jj = 0; jj < idx; jj++) {
    x_data[jj] = (calc_dist_err_data[jj] < Est_H_p_im);
  }
  idx = 0;
  jj = 1;
  exitg1 = false;
  while ((!exitg1) && (jj <= (int)Ln)) {
    if (x_data[jj - 1]) {
      idx++;
      jj_data[idx - 1] = (signed char)jj;
      if (idx >= (int)Ln) {
        exitg1 = true;
      } else {
        jj++;
      }
    } else {
      jj++;
    }
  }
  if (idx < 1) {
    jj_size_idx_1 = 0;
  } else {
    jj_size_idx_1 = idx;
  }
  for (jj = 0; jj < jj_size_idx_1; jj++) {
    calc_dist_err_data[jj] = jj_data[jj];
  }
  if (jj_size_idx_1 >= 3) {
    for (jj = 0; jj < jj_size_idx_1; jj++) {
      jj_data[jj] = anch_pos_data[(int)calc_dist_err_data[jj] - 1].re;
    }
    for (jj = 0; jj < jj_size_idx_1; jj++) {
      ya_data[jj] = anch_pos_data[(int)calc_dist_err_data[jj] - 1].im;
    }
    M_tmp = jj_size_idx_1 - 1;
    idx = (jj_size_idx_1 - 1) << 1;
    memset(&A_data[0], 0, (unsigned int)idx * sizeof(short));
    memset(&y_data[0], 0, (unsigned int)(jj_size_idx_1 - 1) * sizeof(double));
    b_r = dist_data[(int)calc_dist_err_data[jj_size_idx_1 - 1] - 1];
    for (k = 0; k <= jj_size_idx_1 - 2; k++) {
      A_data[k] = (short)(-2 * (jj_data[k] - jj_data[jj_size_idx_1 - 1]));
      A_data[(k + jj_size_idx_1) - 1] =
          (short)(-2 * (ya_data[k] - ya_data[jj_size_idx_1 - 1]));
      Est_H_p_im = dist_data[(int)calc_dist_err_data[k] - 1];
      y_data[k] =
          ((Est_H_p_im * Est_H_p_im - b_r * b_r) -
           (double)(jj_data[k] * jj_data[k] -
                    jj_data[jj_size_idx_1 - 1] * jj_data[jj_size_idx_1 - 1])) -
          (double)(ya_data[k] * ya_data[k] -
                   ya_data[jj_size_idx_1 - 1] * ya_data[jj_size_idx_1 - 1]);
    }
    for (j = 0; j < 2; j++) {
      coffset = j << 1;
      idx = j * (jj_size_idx_1 - 1);
      calc_dist_err_data[coffset] = 0.0;
      calc_dist_err_data[coffset + 1] = 0.0;
      for (k = 0; k < M_tmp; k++) {
        jj = A_data[idx + k];
        calc_dist_err_data[coffset] += (double)(A_data[k] * jj);
        calc_dist_err_data[coffset + 1] +=
            (double)(A_data[(jj_size_idx_1 + k) - 1] * jj);
      }
    }
    inv(calc_dist_err_data, a);
    for (j = 0; j < M_tmp; j++) {
      coffset = j << 1;
      idx = A_data[j];
      Est_H_p_im = A_data[(jj_size_idx_1 + j) - 1];
      b_y_data[coffset] = a[0] * (double)idx + a[2] * Est_H_p_im;
      b_y_data[coffset + 1] = a[1] * (double)idx + a[3] * Est_H_p_im;
    }
    Est_H_p_im = 0.0;
    b_r = 0.0;
    for (k = 0; k < M_tmp; k++) {
      idx = k << 1;
      Est_H = y_data[k];
      Est_H_p_im += b_y_data[idx] * Est_H;
      b_r += b_y_data[idx + 1] * Est_H;
    }
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
    if (dcv[3].im == 0.0) {
      if (ai == 0.0) {
        Est_H_p_re = ar / dcv[3].re;
        Est_H_p_im = 0.0;
      } else if (ar == 0.0) {
        Est_H_p_re = 0.0;
        Est_H_p_im = ai / dcv[3].re;
      } else {
        Est_H_p_re = ar / dcv[3].re;
        Est_H_p_im = ai / dcv[3].re;
      }
    } else if (dcv[3].re == 0.0) {
      if (ar == 0.0) {
        Est_H_p_re = ai / dcv[3].im;
        Est_H_p_im = 0.0;
      } else if (ai == 0.0) {
        Est_H_p_re = 0.0;
        Est_H_p_im = -(ar / dcv[3].im);
      } else {
        Est_H_p_re = ai / dcv[3].im;
        Est_H_p_im = -(ar / dcv[3].im);
      }
    } else {
      ai_tmp = fabs(dcv[3].re);
      b_r = fabs(dcv[3].im);
      if (ai_tmp > b_r) {
        Est_H_p_im = dcv[3].im / dcv[3].re;
        Est_H = dcv[3].re + Est_H_p_im * dcv[3].im;
        Est_H_p_re = (ar + Est_H_p_im * ai) / Est_H;
        Est_H_p_im = (ai - Est_H_p_im * ar) / Est_H;
      } else if (b_r == ai_tmp) {
        if (dcv[3].re > 0.0) {
          Est_H_p_im = 0.5;
        } else {
          Est_H_p_im = -0.5;
        }
        if (dcv[3].im > 0.0) {
          b_r = 0.5;
        } else {
          b_r = -0.5;
        }
        Est_H_p_re = (ar * Est_H_p_im + ai * b_r) / ai_tmp;
        Est_H_p_im = (ai * Est_H_p_im - ar * b_r) / ai_tmp;
      } else {
        Est_H_p_im = dcv[3].re / dcv[3].im;
        Est_H = dcv[3].im + Est_H_p_im * dcv[3].re;
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
    tag_pos_est[0].re =
        Est_C_re + (dcv[0].re * Est_H_p_re - dcv[0].im * Est_H_p_im);
    tag_pos_est[0].im =
        Est_C_im + (dcv[0].re * Est_H_p_im + dcv[0].im * Est_H_p_re);
    if (Est_H == 0.0) {
      Est_H_p_re = exp(ai_tmp);
      Est_H_p_im = 0.0;
    } else {
      b_r = exp(ai_tmp / 2.0);
      Est_H_p_re = b_r * (b_r * cos(Est_H));
      Est_H_p_im = b_r * (b_r * sin(Est_H));
    }
    tag_pos_est[1].re =
        Est_C_re + (dcv[1].re * Est_H_p_re - dcv[1].im * Est_H_p_im);
    tag_pos_est[1].im =
        Est_C_im + (dcv[1].re * Est_H_p_im + dcv[1].im * Est_H_p_re);
    if (Est_H == 0.0) {
      Est_H_p_re = exp(ai_tmp);
      Est_H_p_im = 0.0;
    } else {
      b_r = exp(ai_tmp / 2.0);
      Est_H_p_re = b_r * (b_r * cos(Est_H));
      Est_H_p_im = b_r * (b_r * sin(Est_H));
    }
    tag_pos_est[2].re =
        Est_C_re + (dcv[2].re * Est_H_p_re - dcv[2].im * Est_H_p_im);
    tag_pos_est[2].im =
        Est_C_im + (dcv[2].re * Est_H_p_im + dcv[2].im * Est_H_p_re);
    if (Est_H == 0.0) {
      Est_H_p_re = exp(ai_tmp);
      Est_H_p_im = 0.0;
    } else {
      b_r = exp(ai_tmp / 2.0);
      Est_H_p_re = b_r * (b_r * cos(Est_H));
      Est_H_p_im = b_r * (b_r * sin(Est_H));
    }
    tag_pos_est[3].re =
        Est_C_re + (dcv[3].re * Est_H_p_re - dcv[3].im * Est_H_p_im);
    tag_pos_est[3].im =
        Est_C_im + (dcv[3].re * Est_H_p_im + dcv[3].im * Est_H_p_re);
    *heading_est = Est_H;
  }
}

/* End of code generation (GetPosRefine2.c) */
