/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: GetPosRefine2.c
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 01-Aug-2023 15:24:45
 */

/* Include Files */
#include "GetPosRefine2.h"
#include "TestTotal_data.h"
#include "TestTotal_rtwutil.h"
#include "inv.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : const double dist_data[]
 *                const double RxID_data[]
 *                const int RxID_size[2]
 *                double Ln
 *                double PP
 *                creal_T tag_pos_est[4]
 *                double *heading_est
 *                creal_T *cand_tag_pos
 * Return Type  : void
 */
void GetPosRefine2(const double dist_data[], const double RxID_data[],
                   const int RxID_size[2], double Ln, double PP,
                   creal_T tag_pos_est[4], double *heading_est,
                   creal_T *cand_tag_pos)
{
  static const signed char iv[6] = {10, 10, -10, -10, 20, -20};
  static const signed char iv1[6] = {10, -10, -10, 10, 15, -15};
  cint8_T anch_pos_data[4];
  double calc_dist_err_data[4];
  double Est_H;
  double Est_H_p_im;
  double a;
  int idx;
  int j;
  int jj;
  int jj_size_idx_1;
  int k;
  short A_data[6];
  signed char jj_data[4];
  bool x_data[4];
  bool exitg1;
  idx = RxID_size[1];
  for (jj = 0; jj < idx; jj++) {
    Est_H = RxID_data[jj];
    anch_pos_data[jj].re = iv[(int)Est_H - 1];
    anch_pos_data[jj].im = iv1[(int)Est_H - 1];
  }
  idx = (int)Ln;
  for (jj = 0; jj < idx; jj++) {
    a = fabs(rt_hypotd_snf(
                 (double)anch_pos_data[jj].re - tag_pos_est[(int)PP - 1].re,
                 (double)anch_pos_data[jj].im - tag_pos_est[(int)PP - 1].im) -
             dist_data[jj]);
    calc_dist_err_data[jj] = a * a;
  }
  if ((int)Ln == 0) {
    Est_H_p_im = 0.0;
  } else {
    Est_H_p_im = calc_dist_err_data[0];
    for (k = 2; k <= idx; k++) {
      Est_H_p_im += calc_dist_err_data[k - 1];
    }
  }
  Est_H_p_im = fmax(sqrt(Est_H_p_im / (double)(int)Ln) * 0.3, 0.2);
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
    double b_a[4];
    double y_data[3];
    double Est_C_im;
    double Est_C_re;
    double Est_H_p_re;
    double ai;
    double ai_tmp;
    int M_tmp;
    signed char ya_data[4];
    for (jj = 0; jj < jj_size_idx_1; jj++) {
      idx = (int)calc_dist_err_data[jj];
      jj_data[jj] = anch_pos_data[idx - 1].re;
      ya_data[jj] = anch_pos_data[idx - 1].im;
    }
    M_tmp = jj_size_idx_1 - 1;
    idx = (jj_size_idx_1 - 1) << 1;
    memset(&A_data[0], 0, (unsigned int)idx * sizeof(short));
    a = dist_data[(int)calc_dist_err_data[jj_size_idx_1 - 1] - 1];
    for (k = 0; k <= jj_size_idx_1 - 2; k++) {
      A_data[k] = (short)(-2 * (jj_data[k] - jj_data[jj_size_idx_1 - 1]));
      A_data[(k + jj_size_idx_1) - 1] =
          (short)(-2 * (ya_data[k] - ya_data[jj_size_idx_1 - 1]));
      Est_H_p_im = dist_data[(int)calc_dist_err_data[k] - 1];
      y_data[k] =
          ((Est_H_p_im * Est_H_p_im - a * a) -
           (double)(jj_data[k] * jj_data[k] -
                    jj_data[jj_size_idx_1 - 1] * jj_data[jj_size_idx_1 - 1])) -
          (double)(ya_data[k] * ya_data[k] -
                   ya_data[jj_size_idx_1 - 1] * ya_data[jj_size_idx_1 - 1]);
    }
    for (j = 0; j < 2; j++) {
      idx = j << 1;
      jj = j * (jj_size_idx_1 - 1);
      calc_dist_err_data[idx] = 0.0;
      calc_dist_err_data[idx + 1] = 0.0;
      for (k = 0; k < M_tmp; k++) {
        int bkj;
        bkj = A_data[jj + k];
        calc_dist_err_data[idx] += (double)(A_data[k] * bkj);
        calc_dist_err_data[idx + 1] +=
            (double)(A_data[(jj_size_idx_1 + k) - 1] * bkj);
      }
    }
    inv(calc_dist_err_data, b_a);
    Est_H_p_im = 0.0;
    a = 0.0;
    for (j = 0; j < M_tmp; j++) {
      idx = A_data[j];
      jj = A_data[(jj_size_idx_1 + j) - 1];
      Est_H = y_data[j];
      Est_H_p_im += (b_a[0] * (double)idx + b_a[2] * (double)jj) * Est_H;
      a += (b_a[1] * (double)idx + b_a[3] * (double)jj) * Est_H;
    }
    Est_H = Est_H_p_im + a * 0.0;
    tag_pos_est[(int)PP - 1].re = Est_H;
    tag_pos_est[(int)PP - 1].im = a;
    cand_tag_pos->re = Est_H;
    cand_tag_pos->im = a;
    Est_H = tag_pos_est[0].re;
    ai = tag_pos_est[0].im;
    if (ai == 0.0) {
      Est_H_p_im = Est_H / 4.0;
      a = 0.0;
    } else if (Est_H == 0.0) {
      Est_H_p_im = 0.0;
      a = ai / 4.0;
    } else {
      Est_H_p_im = Est_H / 4.0;
      a = ai / 4.0;
    }
    Est_C_re = Est_H_p_im;
    Est_C_im = a;
    Est_H = tag_pos_est[1].re;
    ai = tag_pos_est[1].im;
    if (ai == 0.0) {
      Est_H_p_im = Est_H / 4.0;
      a = 0.0;
    } else if (Est_H == 0.0) {
      Est_H_p_im = 0.0;
      a = ai / 4.0;
    } else {
      Est_H_p_im = Est_H / 4.0;
      a = ai / 4.0;
    }
    Est_C_re += Est_H_p_im;
    Est_C_im += a;
    Est_H = tag_pos_est[2].re;
    ai = tag_pos_est[2].im;
    if (ai == 0.0) {
      Est_H_p_im = Est_H / 4.0;
      a = 0.0;
    } else if (Est_H == 0.0) {
      Est_H_p_im = 0.0;
      a = ai / 4.0;
    } else {
      Est_H_p_im = Est_H / 4.0;
      a = ai / 4.0;
    }
    Est_C_re += Est_H_p_im;
    Est_C_im += a;
    Est_H = tag_pos_est[3].re;
    ai_tmp = tag_pos_est[3].im;
    if (ai_tmp == 0.0) {
      Est_H_p_im = Est_H / 4.0;
      a = 0.0;
    } else if (Est_H == 0.0) {
      Est_H_p_im = 0.0;
      a = ai_tmp / 4.0;
    } else {
      Est_H_p_im = Est_H / 4.0;
      a = ai_tmp / 4.0;
    }
    Est_C_re += Est_H_p_im;
    Est_C_im += a;
    Est_H -= Est_C_re;
    ai = ai_tmp - Est_C_im;
    if (dcv[3].im == 0.0) {
      if (ai == 0.0) {
        Est_H_p_re = Est_H / dcv[3].re;
        Est_H_p_im = 0.0;
      } else if (Est_H == 0.0) {
        Est_H_p_re = 0.0;
        Est_H_p_im = ai / dcv[3].re;
      } else {
        Est_H_p_re = Est_H / dcv[3].re;
        Est_H_p_im = ai / dcv[3].re;
      }
    } else if (dcv[3].re == 0.0) {
      if (Est_H == 0.0) {
        Est_H_p_re = ai / dcv[3].im;
        Est_H_p_im = 0.0;
      } else if (ai == 0.0) {
        Est_H_p_re = 0.0;
        Est_H_p_im = -(Est_H / dcv[3].im);
      } else {
        Est_H_p_re = ai / dcv[3].im;
        Est_H_p_im = -(Est_H / dcv[3].im);
      }
    } else {
      ai_tmp = fabs(dcv[3].re);
      a = fabs(dcv[3].im);
      if (ai_tmp > a) {
        a = dcv[3].im / dcv[3].re;
        Est_H_p_im = dcv[3].re + a * dcv[3].im;
        Est_H_p_re = (Est_H + a * ai) / Est_H_p_im;
        Est_H_p_im = (ai - a * Est_H) / Est_H_p_im;
      } else if (a == ai_tmp) {
        if (dcv[3].re > 0.0) {
          a = 0.5;
        } else {
          a = -0.5;
        }
        if (dcv[3].im > 0.0) {
          Est_H_p_im = 0.5;
        } else {
          Est_H_p_im = -0.5;
        }
        Est_H_p_re = (Est_H * a + ai * Est_H_p_im) / ai_tmp;
        Est_H_p_im = (ai * a - Est_H * Est_H_p_im) / ai_tmp;
      } else {
        a = dcv[3].re / dcv[3].im;
        Est_H_p_im = dcv[3].im + a * dcv[3].re;
        Est_H_p_re = (a * Est_H + ai) / Est_H_p_im;
        Est_H_p_im = (a * ai - Est_H) / Est_H_p_im;
      }
    }
    Est_H = rt_atan2d_snf(Est_H_p_im, Est_H_p_re);
    ai_tmp = Est_H * 0.0;
    if (ai_tmp == 0.0) {
      Est_H_p_re = cos(Est_H);
      Est_H_p_im = sin(Est_H);
    } else if (Est_H == 0.0) {
      Est_H_p_re = rtNaN;
      Est_H_p_im = 0.0;
    } else {
      Est_H_p_re = rtNaN;
      Est_H_p_im = rtNaN;
    }
    tag_pos_est[0].re =
        Est_C_re + (dcv[0].re * Est_H_p_re - dcv[0].im * Est_H_p_im);
    tag_pos_est[0].im =
        Est_C_im + (dcv[0].re * Est_H_p_im + dcv[0].im * Est_H_p_re);
    if (ai_tmp == 0.0) {
      Est_H_p_re = cos(Est_H);
      Est_H_p_im = sin(Est_H);
    } else if (Est_H == 0.0) {
      Est_H_p_re = rtNaN;
      Est_H_p_im = 0.0;
    } else {
      Est_H_p_re = rtNaN;
      Est_H_p_im = rtNaN;
    }
    Est_H_p_im *= dcv[1].im;
    a = dcv[1].re * Est_H_p_re;
    tag_pos_est[1].re = Est_C_re + (a - Est_H_p_im);
    tag_pos_est[1].im = Est_C_im + (Est_H_p_im + a);
    if (ai_tmp == 0.0) {
      Est_H_p_re = cos(Est_H);
      Est_H_p_im = sin(Est_H);
    } else if (Est_H == 0.0) {
      Est_H_p_re = rtNaN;
      Est_H_p_im = 0.0;
    } else {
      Est_H_p_re = rtNaN;
      Est_H_p_im = rtNaN;
    }
    Est_H_p_im *= dcv[2].im;
    a = dcv[2].re * Est_H_p_re;
    tag_pos_est[2].re = Est_C_re + (a - Est_H_p_im);
    tag_pos_est[2].im = Est_C_im + (Est_H_p_im + a);
    if (ai_tmp == 0.0) {
      Est_H_p_re = cos(Est_H);
      Est_H_p_im = sin(Est_H);
    } else if (Est_H == 0.0) {
      Est_H_p_re = rtNaN;
      Est_H_p_im = 0.0;
    } else {
      Est_H_p_re = rtNaN;
      Est_H_p_im = rtNaN;
    }
    tag_pos_est[3].re =
        Est_C_re + (dcv[3].re * Est_H_p_re - dcv[3].im * Est_H_p_im);
    tag_pos_est[3].im =
        Est_C_im + (dcv[3].re * Est_H_p_im + dcv[3].im * Est_H_p_re);
    *heading_est = Est_H;
  }
}

/*
 * File trailer for GetPosRefine2.c
 *
 * [EOF]
 */
