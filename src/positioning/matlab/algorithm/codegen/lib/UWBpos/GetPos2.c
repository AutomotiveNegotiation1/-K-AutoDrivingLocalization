/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: GetPos2.c
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 01-Aug-2023 14:35:53
 */

/* Include Files */
#include "GetPos2.h"
#include "TwoAnchPos3.h"
#include "UWBpos_data.h"
#include "UWBpos_emxutil.h"
#include "UWBpos_rtwutil.h"
#include "UWBpos_types.h"
#include "dec2bin.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Definitions */
/*
 * Arguments    : const double xa[6]
 *                const double ya[6]
 *                const double dist_data[]
 *                int dist_size
 *                const double RxID_data[]
 *                const int RxID_size[2]
 *                const creal_T tag_pos_b[4]
 *                double Ln
 *                double PP
 *                const emxArray_creal_T *PredPos
 *                creal_T tag_pos_est[4]
 *                creal_T *cand_tag_pos
 * Return Type  : double
 */
double GetPos2(const double xa[6], const double ya[6], const double dist_data[],
               int dist_size, const double RxID_data[], const int RxID_size[2],
               const creal_T tag_pos_b[4], double Ln, double PP,
               const emxArray_creal_T *PredPos, creal_T tag_pos_est[4],
               creal_T *cand_tag_pos)
{
  emxArray_char_T *Va;
  emxArray_creal_T b_xa_data;
  emxArray_creal_T *Pos2C;
  emxArray_int8_T *Va_ind;
  emxArray_real_T b_dist_data;
  emxArray_real_T *ia;
  creal_T xa_data[4];
  const creal_T *PredPos_data;
  creal_T *Pos2C_data;
  double Est_F[2];
  double Est_H[2];
  double dist[2];
  double Est_H_p_im;
  double Est_H_p_re;
  double Sel_C_im;
  double Sel_C_re;
  double Sel_F;
  double heading_est;
  double *ia_data;
  int xa_size[2];
  int L1;
  int L2;
  int Qn;
  int b_j1;
  int i;
  int i2;
  int i3;
  int j2;
  int loop_ub;
  int nd2;
  char *Va_data;
  signed char *Va_ind_data;
  PredPos_data = PredPos->data;
  emxInit_creal_T(&Pos2C, 2);
  i = Pos2C->size[0] * Pos2C->size[1];
  Pos2C->size[0] = PredPos->size[0];
  Pos2C->size[1] = 2;
  emxEnsureCapacity_creal_T(Pos2C, i);
  Pos2C_data = Pos2C->data;
  j2 = PredPos->size[0] << 1;
  for (i = 0; i < j2; i++) {
    Pos2C_data[i].re = 0.0;
    Pos2C_data[i].im = 0.0;
  }
  j2 = PredPos->size[0];
  emxInit_int8_T(&Va_ind);
  i = Va_ind->size[0] * Va_ind->size[1];
  Va_ind->size[0] = 1;
  Va_ind->size[1] = PredPos->size[0];
  emxEnsureCapacity_int8_T(Va_ind, i);
  Va_ind_data = Va_ind->data;
  for (i = 0; i < j2; i++) {
    Pos2C_data[i] = PredPos_data[i];
    Va_ind_data[i] = 0;
  }
  Sel_F = 1.0E+7;
  Sel_C_re = 0.0;
  Sel_C_im = 0.0;
  heading_est = 0.0;
  cand_tag_pos->re = 0.0;
  cand_tag_pos->im = 0.0;
  i = (int)Ln;
  emxInit_char_T(&Va);
  emxInit_real_T(&ia, 1);
  for (L1 = 0; L1 < i; L1++) {
    int i1;
    i1 = (int)(Ln + (1.0 - (((double)L1 + 1.0) + 1.0)));
    if (i1 - 1 >= 0) {
      i2 = (int)(PP + (1.0 - PP));
      Va_ind_data[(int)PP - 1] = 1;
    }
    for (L2 = 0; L2 < i1; L2++) {
      creal_T Est_C[2];
      double Pos2C_re;
      double b_L2;
      double brm;
      int i4;
      int i5;
      b_L2 = (((double)L1 + 1.0) + 1.0) + (double)L2;
      if (i2 - 1 >= 0) {
        j2 = (signed char)RxID_data[L1] - 1;
        Est_F[0] = xa[j2];
        nd2 = (signed char)RxID_data[(int)b_L2 - 1] - 1;
        Est_F[1] = xa[nd2];
        Est_H[0] = ya[j2];
        Est_H[1] = ya[nd2];
        dist[0] = dist_data[L1];
        dist[1] = dist_data[(int)b_L2 - 1];
        loop_ub = RxID_size[1];
      }
      for (nd2 = 0; nd2 < i2; nd2++) {
        b_L2 = PP + (double)nd2;
        xa_size[0] = 1;
        xa_size[1] = RxID_size[1];
        for (i3 = 0; i3 < loop_ub; i3++) {
          Pos2C_re = RxID_data[i3];
          brm = ya[(int)Pos2C_re - 1];
          xa_data[i3].re = xa[(int)Pos2C_re - 1] + 0.0 * brm;
          xa_data[i3].im = brm;
        }
        double Pos2[4];
        double Prob2[2];
        b_xa_data.data = &xa_data[0];
        b_xa_data.size = &xa_size[0];
        b_xa_data.allocatedSize = 4;
        b_xa_data.numDimensions = 2;
        b_xa_data.canFreeData = false;
        b_dist_data.data = (double *)&dist_data[0];
        b_dist_data.size = &dist_size;
        b_dist_data.allocatedSize = -1;
        b_dist_data.numDimensions = 1;
        b_dist_data.canFreeData = false;
        TwoAnchPos3(Est_F, Est_H, dist, &b_xa_data, &b_dist_data, Pos2, Prob2);
        Pos2C_data[(int)b_L2 - 1].re = Pos2[0];
        Pos2C_data[(int)b_L2 - 1].im = Pos2[2];
        Pos2C_data[((int)b_L2 + Pos2C->size[0]) - 1].re = Pos2[1];
        Pos2C_data[((int)b_L2 + Pos2C->size[0]) - 1].im = Pos2[3];
      }
      i3 = Pos2C->size[0];
      i4 = Pos2C->size[0];
      i5 = Pos2C->size[0];
      for (Qn = 0; Qn < 2; Qn++) {
        double ai;
        double ar;
        double bi;
        double br;
        double im;
        double re;
        char xtmp;
        dec2bin(rt_powd_snf(2.0, PP * (double)Qn) - 1.0, PredPos->size[0], Va);
        Va_data = Va->data;
        nd2 = Va->size[1] >> 1;
        for (b_j1 = 0; b_j1 < nd2; b_j1++) {
          j2 = (Va->size[1] - b_j1) - 1;
          xtmp = Va_data[b_j1];
          Va_data[b_j1] = Va_data[j2];
          Va_data[j2] = xtmp;
        }
        b_j1 = ia->size[0];
        ia->size[0] = Va->size[1];
        emxEnsureCapacity_real_T(ia, b_j1);
        ia_data = ia->data;
        j2 = Va->size[1];
        for (b_j1 = 0; b_j1 < j2; b_j1++) {
          ia_data[b_j1] = 0.0;
        }
        b_j1 = Va->size[1];
        for (nd2 = 0; nd2 < b_j1; nd2++) {
          xtmp = Va_data[nd2];
          if ((xtmp == '0') && (Va_ind_data[nd2] != 0)) {
            ia_data[nd2] = 1.0;
          } else if ((xtmp == '1') && (Va_ind_data[nd2] != 0)) {
            ia_data[nd2] = 2.0;
          } else {
            ia_data[nd2] = 1.0;
          }
        }
        /*  NB = length(tag_pos_b); */
        re = 0.0;
        im = 0.0;
        for (j2 = 0; j2 < i3; j2++) {
          nd2 = (int)ia_data[j2] - 1;
          ar = Pos2C_data[j2 + Pos2C->size[0] * nd2].re;
          ai = Pos2C_data[j2 + Pos2C->size[0] * nd2].im;
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
        for (j2 = 0; j2 < i4; j2++) {
          nd2 = (int)ia_data[j2] - 1;
          ar = Pos2C_data[j2 + Pos2C->size[0] * nd2].re - re;
          ai = Pos2C_data[j2 + Pos2C->size[0] * nd2].im - im;
          br = tag_pos_b[j2].re;
          bi = tag_pos_b[j2].im;
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
        for (j2 = 0; j2 < i5; j2++) {
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
          b_L2 = tag_pos_b[j2].re;
          brm = tag_pos_b[j2].im;
          br = b_L2 * Est_H_p_re - brm * Est_H_p_im;
          b_L2 = b_L2 * Est_H_p_im + brm * Est_H_p_re;
          nd2 = (int)ia_data[j2] - 1;
          b_L2 = rt_hypotd_snf(
              (re + br) - Pos2C_data[j2 + Pos2C->size[0] * nd2].re,
              (im + b_L2) - Pos2C_data[j2 + Pos2C->size[0] * nd2].im);
          Pos2C_re += b_L2 * b_L2;
        }
        Est_F[Qn] = Pos2C_re;
        Est_C[Qn].re = re;
        Est_C[Qn].im = im;
        Est_H[Qn] = bi;
      }
      if ((Est_F[0] > Est_F[1]) ||
          (rtIsNaN(Est_F[0]) && (!rtIsNaN(Est_F[1])))) {
        b_L2 = Est_F[1];
        j2 = 1;
      } else {
        b_L2 = Est_F[0];
        j2 = 0;
      }
      if (Sel_F > b_L2) {
        Sel_F = b_L2;
        Sel_C_re = Est_C[j2].re;
        Sel_C_im = Est_C[j2].im;
        heading_est = Est_H[j2];
        *cand_tag_pos = Pos2C_data[((int)PP + Pos2C->size[0] * j2) - 1];
      }
    }
  }
  emxFree_real_T(&ia);
  emxFree_char_T(&Va);
  emxFree_int8_T(&Va_ind);
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
 * File trailer for GetPos2.c
 *
 * [EOF]
 */
