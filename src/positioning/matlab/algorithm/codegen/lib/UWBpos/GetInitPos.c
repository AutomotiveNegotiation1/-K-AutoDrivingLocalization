/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: GetInitPos.c
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 01-Aug-2023 14:00:18
 */

/* Include Files */
#include "GetInitPos.h"
#include "TwoAnchPos3.h"
#include "UWBpos_data.h"
#include "UWBpos_emxutil.h"
#include "UWBpos_rtwutil.h"
#include "UWBpos_types.h"
#include "dec2bin.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Declarations */
static void b_binary_expand_op(const emxArray_real_T *in1, int in2, double in3,
                               const emxArray_real_T *in4,
                               const emxArray_real_T *in5, int in6,
                               double in8[4], double in9[2]);

/* Function Definitions */
/*
 * Arguments    : const emxArray_real_T *in1
 *                int in2
 *                double in3
 *                const emxArray_real_T *in4
 *                const emxArray_real_T *in5
 *                int in6
 *                double in8[4]
 *                double in9[2]
 * Return Type  : void
 */
static void b_binary_expand_op(const emxArray_real_T *in1, int in2, double in3,
                               const emxArray_real_T *in4,
                               const emxArray_real_T *in5, int in6,
                               double in8[4], double in9[2])
{
  emxArray_creal_T *c_in1;
  emxArray_real_T *c_in5;
  creal_T *b_in1_data;
  double b_in1[2];
  double b_in4[2];
  double b_in5[2];
  const double *in1_data;
  const double *in4_data;
  const double *in5_data;
  double *b_in5_data;
  int i;
  int loop_ub;
  int stride_0_1;
  int stride_1_1;
  in5_data = in5->data;
  in4_data = in4->data;
  in1_data = in1->data;
  b_in1[0] = in1_data[in2];
  b_in1[1] = in1_data[(int)in3 - 1];
  b_in4[0] = in4_data[in2];
  b_in4[1] = in4_data[(int)in3 - 1];
  b_in5[0] = in5_data[in2 + in5->size[0] * in6];
  b_in5[1] = in5_data[((int)in3 + in5->size[0] * in6) - 1];
  emxInit_creal_T(&c_in1, 2);
  i = c_in1->size[0] * c_in1->size[1];
  c_in1->size[0] = 1;
  if (in4->size[1] == 1) {
    loop_ub = in1->size[1];
  } else {
    loop_ub = in4->size[1];
  }
  c_in1->size[1] = loop_ub;
  emxEnsureCapacity_creal_T(c_in1, i);
  b_in1_data = c_in1->data;
  stride_0_1 = (in1->size[1] != 1);
  stride_1_1 = (in4->size[1] != 1);
  for (i = 0; i < loop_ub; i++) {
    double d;
    d = in4_data[i * stride_1_1];
    b_in1_data[i].re = in1_data[i * stride_0_1] + 0.0 * d;
    b_in1_data[i].im = d;
  }
  emxInit_real_T(&c_in5, 1);
  i = c_in5->size[0];
  c_in5->size[0] = in5->size[0];
  emxEnsureCapacity_real_T(c_in5, i);
  b_in5_data = c_in5->data;
  loop_ub = in5->size[0];
  for (i = 0; i < loop_ub; i++) {
    b_in5_data[i] = in5_data[i + in5->size[0] * in6];
  }
  TwoAnchPos3(b_in1, b_in4, b_in5, c_in1, c_in5, in8, in9);
  emxFree_real_T(&c_in5);
  emxFree_creal_T(&c_in1);
}

/*
 * Arguments    : const emxArray_real_T *xa
 *                const emxArray_real_T *ya
 *                const emxArray_real_T *dist
 *                const creal_T tag_pos_b[4]
 *                double Ln
 *                double Lp
 *                creal_T tag_pos_est[4]
 * Return Type  : double
 */
double GetInitPos(const emxArray_real_T *xa, const emxArray_real_T *ya,
                  const emxArray_real_T *dist, const creal_T tag_pos_b[4],
                  double Ln, double Lp, creal_T tag_pos_est[4])
{
  emxArray_char_T *Va;
  emxArray_creal_T *Est_C;
  emxArray_creal_T *Pos2C;
  emxArray_creal_T *b_xa;
  emxArray_real_T *Est_F;
  emxArray_real_T *Est_H;
  emxArray_real_T *ia;
  creal_T *Est_C_data;
  creal_T *Pos2C_data;
  creal_T *b_xa_data;
  const double *dist_data;
  const double *xa_data;
  const double *ya_data;
  double Est_H_p_im;
  double Est_H_p_re;
  double Sel_C_im;
  double Sel_C_re;
  double Sel_F;
  double c_tmp;
  double heading_est;
  double *Est_F_data;
  double *Est_H_data;
  double *ia_data;
  int L1;
  int L2;
  int P;
  int Pn;
  int Qn;
  int i;
  int i1;
  int i3;
  int last;
  char *Va_data;
  dist_data = dist->data;
  ya_data = ya->data;
  xa_data = xa->data;
  Sel_F = 1.0E+7;
  emxInit_creal_T(&Pos2C, 2);
  i = (int)Lp;
  i1 = Pos2C->size[0] * Pos2C->size[1];
  Pos2C->size[0] = (int)Lp;
  Pos2C->size[1] = 2;
  emxEnsureCapacity_creal_T(Pos2C, i1);
  Pos2C_data = Pos2C->data;
  last = (int)Lp << 1;
  for (i1 = 0; i1 < last; i1++) {
    Pos2C_data[i1].re = 0.0;
    Pos2C_data[i1].im = 0.0;
  }
  c_tmp = rt_powd_snf(2.0, Lp);
  emxInit_real_T(&Est_F, 1);
  last = (int)c_tmp;
  i1 = Est_F->size[0];
  Est_F->size[0] = (int)c_tmp;
  emxEnsureCapacity_real_T(Est_F, i1);
  Est_F_data = Est_F->data;
  for (i1 = 0; i1 < last; i1++) {
    Est_F_data[i1] = 0.0;
  }
  emxInit_creal_T(&Est_C, 1);
  i1 = Est_C->size[0];
  Est_C->size[0] = (int)c_tmp;
  emxEnsureCapacity_creal_T(Est_C, i1);
  Est_C_data = Est_C->data;
  for (i1 = 0; i1 < last; i1++) {
    Est_C_data[i1].re = 0.0;
    Est_C_data[i1].im = 0.0;
  }
  emxInit_real_T(&Est_H, 1);
  i1 = Est_H->size[0];
  Est_H->size[0] = (int)c_tmp;
  emxEnsureCapacity_real_T(Est_H, i1);
  Est_H_data = Est_H->data;
  for (i1 = 0; i1 < last; i1++) {
    Est_H_data[i1] = 0.0;
  }
  Sel_C_re = 0.0;
  Sel_C_im = 0.0;
  heading_est = 0.0;
  i1 = (int)Ln;
  emxInit_char_T(&Va);
  emxInit_real_T(&ia, 1);
  emxInit_creal_T(&b_xa, 2);
  for (L1 = 0; L1 < i1; L1++) {
    int i2;
    i2 = (int)(Ln + (1.0 - (((double)L1 + 1.0) + 1.0)));
    for (L2 = 0; L2 < i2; L2++) {
      double Pos2C_re;
      double b_L2;
      b_L2 = (((double)L1 + 1.0) + 1.0) + (double)L2;
      for (Pn = 0; Pn < i; Pn++) {
        double Pos2[4];
        if (xa->size[1] == ya->size[1]) {
          double b_dist[2];
          double b_ya[2];
          double c_xa[2];
          c_xa[0] = xa_data[L1];
          c_xa[1] = xa_data[(int)b_L2 - 1];
          b_ya[0] = ya_data[L1];
          b_ya[1] = ya_data[(int)b_L2 - 1];
          b_dist[0] = dist_data[L1 + dist->size[0] * Pn];
          b_dist[1] = dist_data[((int)b_L2 + dist->size[0] * Pn) - 1];
          i3 = b_xa->size[0] * b_xa->size[1];
          b_xa->size[0] = 1;
          b_xa->size[1] = xa->size[1];
          emxEnsureCapacity_creal_T(b_xa, i3);
          b_xa_data = b_xa->data;
          last = xa->size[1];
          for (i3 = 0; i3 < last; i3++) {
            b_xa_data[i3].re = xa_data[i3] + 0.0 * ya_data[i3];
            b_xa_data[i3].im = ya_data[i3];
          }
          i3 = ia->size[0];
          ia->size[0] = dist->size[0];
          emxEnsureCapacity_real_T(ia, i3);
          ia_data = ia->data;
          last = dist->size[0];
          for (i3 = 0; i3 < last; i3++) {
            ia_data[i3] = dist_data[i3 + dist->size[0] * Pn];
          }
          double Prob2[2];
          TwoAnchPos3(c_xa, b_ya, b_dist, b_xa, ia, Pos2, Prob2);
        } else {
          double Prob2[2];
          b_binary_expand_op(xa, L1, b_L2, ya, dist, Pn, Pos2, Prob2);
        }
        Pos2C_data[Pn].re = Pos2[0];
        Pos2C_data[Pn].im = Pos2[2];
        Pos2C_data[Pn + Pos2C->size[0]].re = Pos2[1];
        Pos2C_data[Pn + Pos2C->size[0]].im = Pos2[3];
      }
      i3 = (int)((c_tmp - 1.0) + 1.0);
      Pn = Pos2C->size[0];
      for (Qn = 0; Qn < i3; Qn++) {
        double ai;
        double ar;
        double bi;
        double br;
        double brm;
        double im;
        double re;
        dec2bin(Qn, Lp, Va);
        Va_data = Va->data;
        P = ia->size[0];
        ia->size[0] = Va->size[1];
        emxEnsureCapacity_real_T(ia, P);
        ia_data = ia->data;
        last = Va->size[1];
        for (P = 0; P < last; P++) {
          ia_data[P] = 0.0;
        }
        P = Va->size[1];
        for (last = 0; last < P; last++) {
          if (Va_data[last] == '0') {
            ia_data[last] = 1.0;
          } else if (Va_data[last] == '1') {
            ia_data[last] = 2.0;
          } else {
            ia_data[last] = 1.0;
          }
        }
        /*  NB = length(tag_pos_b); */
        re = 0.0;
        im = 0.0;
        for (P = 0; P < Pn; P++) {
          last = (int)ia_data[P] - 1;
          ar = Pos2C_data[P + Pos2C->size[0] * last].re;
          ai = Pos2C_data[P + Pos2C->size[0] * last].im;
          if (ai == 0.0) {
            Pos2C_re = ar / (double)Va->size[1];
            b_L2 = 0.0;
          } else if (ar == 0.0) {
            Pos2C_re = 0.0;
            b_L2 = ai / (double)Va->size[1];
          } else {
            Pos2C_re = ar / (double)Va->size[1];
            b_L2 = ai / (double)Va->size[1];
          }
          re += Pos2C_re;
          im += b_L2;
        }
        Est_H_p_re = 0.0;
        Est_H_p_im = 0.0;
        for (P = 0; P < Pn; P++) {
          last = (int)ia_data[P] - 1;
          ar = Pos2C_data[P + Pos2C->size[0] * last].re - re;
          ai = Pos2C_data[P + Pos2C->size[0] * last].im - im;
          br = tag_pos_b[P].re;
          bi = tag_pos_b[P].im;
          if (bi == 0.0) {
            if (ai == 0.0) {
              Pos2C_re = ar / br;
              b_L2 = 0.0;
            } else if (ar == 0.0) {
              Pos2C_re = 0.0;
              b_L2 = ai / br;
            } else {
              Pos2C_re = ar / br;
              b_L2 = ai / br;
            }
          } else if (br == 0.0) {
            if (ar == 0.0) {
              Pos2C_re = ai / bi;
              b_L2 = 0.0;
            } else if (ai == 0.0) {
              Pos2C_re = 0.0;
              b_L2 = -(ar / bi);
            } else {
              Pos2C_re = ai / bi;
              b_L2 = -(ar / bi);
            }
          } else {
            brm = fabs(br);
            b_L2 = fabs(bi);
            if (brm > b_L2) {
              brm = bi / br;
              b_L2 = br + brm * bi;
              Pos2C_re = (ar + brm * ai) / b_L2;
              b_L2 = (ai - brm * ar) / b_L2;
            } else if (b_L2 == brm) {
              if (br > 0.0) {
                br = 0.5;
              } else {
                br = -0.5;
              }
              if (bi > 0.0) {
                b_L2 = 0.5;
              } else {
                b_L2 = -0.5;
              }
              Pos2C_re = (ar * br + ai * b_L2) / brm;
              b_L2 = (ai * br - ar * b_L2) / brm;
            } else {
              brm = br / bi;
              b_L2 = bi + brm * br;
              Pos2C_re = (brm * ar + ai) / b_L2;
              b_L2 = (brm * ai - ar) / b_L2;
            }
          }
          Est_H_p_re += Pos2C_re;
          Est_H_p_im += b_L2;
        }
        bi = rt_atan2d_snf(Est_H_p_im, Est_H_p_re);
        Pos2C_re = 0.0;
        for (P = 0; P < Pn; P++) {
          if (bi * 0.0 == 0.0) {
            Est_H_p_re = cos(bi);
            Est_H_p_im = sin(bi);
          } else if (bi == 0.0) {
            Est_H_p_re = rtNaN;
            Est_H_p_im = 0.0;
          } else {
            Est_H_p_re = rtNaN;
            Est_H_p_im = rtNaN;
          }
          b_L2 = tag_pos_b[P].re;
          brm = tag_pos_b[P].im;
          br = b_L2 * Est_H_p_re - brm * Est_H_p_im;
          b_L2 = b_L2 * Est_H_p_im + brm * Est_H_p_re;
          last = (int)ia_data[P] - 1;
          b_L2 = rt_hypotd_snf(
              (re + br) - Pos2C_data[P + Pos2C->size[0] * last].re,
              (im + b_L2) - Pos2C_data[P + Pos2C->size[0] * last].im);
          Pos2C_re += b_L2 * b_L2;
        }
        Est_F_data[Qn] = Pos2C_re;
        Est_C_data[Qn].re = re;
        Est_C_data[Qn].im = im;
        Est_H_data[Qn] = bi;
      }
      last = Est_F->size[0];
      if (Est_F->size[0] <= 2) {
        if (Est_F->size[0] == 1) {
          b_L2 = Est_F_data[0];
          Pn = 1;
        } else {
          b_L2 = Est_F_data[Est_F->size[0] - 1];
          if ((Est_F_data[0] > b_L2) ||
              (rtIsNaN(Est_F_data[0]) && (!rtIsNaN(b_L2)))) {
            Pn = Est_F->size[0];
          } else {
            b_L2 = Est_F_data[0];
            Pn = 1;
          }
        }
      } else {
        if (!rtIsNaN(Est_F_data[0])) {
          Pn = 1;
        } else {
          bool exitg1;
          Pn = 0;
          P = 2;
          exitg1 = false;
          while ((!exitg1) && (P <= last)) {
            if (!rtIsNaN(Est_F_data[P - 1])) {
              Pn = P;
              exitg1 = true;
            } else {
              P++;
            }
          }
        }
        if (Pn == 0) {
          b_L2 = Est_F_data[0];
          Pn = 1;
        } else {
          b_L2 = Est_F_data[Pn - 1];
          i3 = Pn + 1;
          for (P = i3; P <= last; P++) {
            Pos2C_re = Est_F_data[P - 1];
            if (b_L2 > Pos2C_re) {
              b_L2 = Pos2C_re;
              Pn = P;
            }
          }
        }
      }
      if (Sel_F > b_L2) {
        Sel_F = b_L2;
        Sel_C_re = Est_C_data[Pn - 1].re;
        Sel_C_im = Est_C_data[Pn - 1].im;
        heading_est = Est_H_data[Pn - 1];
      }
    }
  }
  emxFree_creal_T(&b_xa);
  emxFree_real_T(&ia);
  emxFree_char_T(&Va);
  emxFree_real_T(&Est_H);
  emxFree_creal_T(&Est_C);
  emxFree_real_T(&Est_F);
  emxFree_creal_T(&Pos2C);
  if (heading_est * 0.0 == 0.0) {
    Est_H_p_re = cos(heading_est);
    Est_H_p_im = sin(heading_est);
  } else if (heading_est == 0.0) {
    Est_H_p_re = rtNaN;
    Est_H_p_im = 0.0;
  } else {
    Est_H_p_re = rtNaN;
    Est_H_p_im = rtNaN;
  }
  tag_pos_est[0].re =
      Sel_C_re + (tag_pos_b[0].re * Est_H_p_re - tag_pos_b[0].im * Est_H_p_im);
  tag_pos_est[0].im =
      Sel_C_im + (tag_pos_b[0].re * Est_H_p_im + tag_pos_b[0].im * Est_H_p_re);
  tag_pos_est[1].re =
      Sel_C_re + (tag_pos_b[1].re * Est_H_p_re - tag_pos_b[1].im * Est_H_p_im);
  tag_pos_est[1].im =
      Sel_C_im + (tag_pos_b[1].re * Est_H_p_im + tag_pos_b[1].im * Est_H_p_re);
  tag_pos_est[2].re =
      Sel_C_re + (tag_pos_b[2].re * Est_H_p_re - tag_pos_b[2].im * Est_H_p_im);
  tag_pos_est[2].im =
      Sel_C_im + (tag_pos_b[2].re * Est_H_p_im + tag_pos_b[2].im * Est_H_p_re);
  tag_pos_est[3].re =
      Sel_C_re + (tag_pos_b[3].re * Est_H_p_re - tag_pos_b[3].im * Est_H_p_im);
  tag_pos_est[3].im =
      Sel_C_im + (tag_pos_b[3].re * Est_H_p_im + tag_pos_b[3].im * Est_H_p_re);
  return heading_est;
}

/*
 * File trailer for GetInitPos.c
 *
 * [EOF]
 */
