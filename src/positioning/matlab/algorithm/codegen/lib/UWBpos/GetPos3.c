/*
 * GetPos3.c
 *
 * Code generation for function 'GetPos3'
 *
 */

/* Include files */
#include "GetPos3.h"
#include "GetMultiTagPosGen2.h"
#include "TwoAnchPos3.h"
#include "UWBpos_data.h"
#include "UWBpos_emxutil.h"
#include "UWBpos_rtwutil.h"
#include "UWBpos_types.h"
#include "dec2bin.h"
#include "rt_nonfinite.h"
#include "sort.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Definitions */
void GetPos3(const double xa[], const double x_size, const double ya[], const double y_size, const double dist_data[],
             int dist_size, const double RxID_data[], const int RxID_size[2],
             const creal_T tag_pos_b[4], double Ln, double PP,
             const emxArray_creal_T *PredPos, const emxArray_real_T *DistPrev,
             const emxArray_real_T *b_RxIDprev, const double b_RxIDprevLen[4],
             const double b_PPprev[4], creal_T tag_pos_est[4],
             double *heading_est, creal_T *cand_tag_pos)
{
  emxArray_char_T *Va;
  emxArray_creal_T b_xa_data;
  emxArray_creal_T *Est_P;
  emxArray_creal_T *Pos2C;
  emxArray_int8_T *Va_ind;
  emxArray_real_T b_dist_data;
  emxArray_real_T *ia;
  creal_T anch_pos_data[4];
  creal_T xa_data[4];
  creal_T Est_C[2];
  const creal_T *PredPos_data;
  creal_T *Est_P_data;
  creal_T *Pos2C_data;
  double Est_Ft_data[4];
  double Est_F[2];
  double Est_H[2];
  double Prob2[2];
  double dist[2];
  const double *DistPrev_data;
  const double *RxIDprev_data;
  double Est_H_p_im;
  double Est_H_p_re;
  double Pos2C_re;
  double Sel_C_im;
  double Sel_C_re;
  double Sel_F;
  double ai;
  double ar;
  double b_r;
  double bi;
  double br;
  double brm;
  double im;
  double re;
  double *ia_data;
  int anch_pos_size[2];
  int xa_size[2];
  int L1;
  int L2;
  int P;
  int Qn;
  int b_loop_ub;
  int i;
  int i1;
  int i2;
  int i3=0;
  int i4;
  int i5;
  int i6;
  int i7;
  int indp;
  int j2;
  int loop_ub;
  int nd2;
  signed char j_data[4];
  char xtmp;
  char *Va_data;
  signed char *Va_ind_data;
  boolean_T exitg1;
  RxIDprev_data = b_RxIDprev->data;
  DistPrev_data = DistPrev->data;
  PredPos_data = PredPos->data;
  emxInit_creal_T(&Pos2C, 2);
  i = Pos2C->size[0] * Pos2C->size[1];
  Pos2C->size[0] = PredPos->size[0];
  Pos2C->size[1] = 2;
  emxEnsureCapacity_creal_T(Pos2C, i);
  Pos2C_data = Pos2C->data;
  loop_ub = PredPos->size[0] << 1;
  for (i = 0; i < loop_ub; i++) {
    Pos2C_data[i].re = 0.0;
    Pos2C_data[i].im = 0.0;
  }
  loop_ub = PredPos->size[0];
  for (i = 0; i < loop_ub; i++) {
    Pos2C_data[i] = PredPos_data[i];
  }
  emxInit_int8_T(&Va_ind);
  i = Va_ind->size[0] * Va_ind->size[1];
  Va_ind->size[0] = 1;
  Va_ind->size[1] = PredPos->size[0];
  emxEnsureCapacity_int8_T(Va_ind, i);
  Va_ind_data = Va_ind->data;
  loop_ub = PredPos->size[0];
  for (i = 0; i < loop_ub; i++) {
    Va_ind_data[i] = 0;
  }
  Sel_F = 1.0E+7;
  Sel_C_re = 0.0;
  Sel_C_im = 0.0;
  *heading_est = 0.0;
  cand_tag_pos->re = 0.0;
  cand_tag_pos->im = 0.0;
  i = (int)Ln;
  emxInit_char_T(&Va);
  emxInit_real_T(&ia, 1);
  emxInit_creal_T(&Est_P, 1);
  for (L1 = 0; L1 < i; L1++) {
    i1 = (int)(Ln + (1.0 - (((double)L1 + 1.0) + 1.0)));
    if (i1 - 1 >= 0) {
      i2 = (int)(PP + (1.0 - PP));
      Va_ind_data[(int)PP - 1] = 1;
    }
    for (L2 = 0; L2 < i1; L2++) {
      b_r = (((double)L1 + 1.0) + 1.0) + (double)L2;
      if (i2 - 1 >= 0) {
        nd2 = (int)RxID_data[L1] - 1;
        Est_F[0] = xa[nd2];
        j2 = (int)RxID_data[(int)b_r - 1] - 1;
        Est_F[1] = xa[j2];
        Est_H[0] = ya[nd2];
        Est_H[1] = ya[j2];
        dist[0] = dist_data[L1];
        dist[1] = dist_data[(int)b_r - 1];
        b_loop_ub = RxID_size[0];
      }
      for (nd2 = 0; nd2 < i2; nd2++) {
        b_r = PP + (double)nd2;
        xa_size[0] = 1;
        xa_size[1] = RxID_size[0];
        printf("GetPos3.c : b_loop_ub-->[%d]\r\n", b_loop_ub);
        for (i3 = 0; i3 < b_loop_ub; i3++) {
          printf("GetPos3.c : wow\r\n");
          printf("GetPos3.c : i3-->[%f]\r\n", i3);
          printf("ok\r\n");
          Pos2C_re = RxID_data[i3];
          brm = ya[(int)Pos2C_re - 1];
          xa_data[i3].re = xa[(int)Pos2C_re - 1] + 0.0 * brm;
          xa_data[i3].im = brm;
        }
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
        TwoAnchPos3(Est_F, Est_H, dist, &b_xa_data, &b_dist_data, Est_Ft_data,
                    Prob2);
        Pos2C_data[(int)b_r - 1].re = Est_Ft_data[0];
        Pos2C_data[(int)b_r - 1].im = Est_Ft_data[2];
        Pos2C_data[((int)b_r + Pos2C->size[0]) - 1].re = Est_Ft_data[1];
        Pos2C_data[((int)b_r + Pos2C->size[0]) - 1].im = Est_Ft_data[3];
      }
      loop_ub = Pos2C->size[0];
      i3 = Pos2C->size[0];
      i4 = Pos2C->size[0];
      i5 = Pos2C->size[0];
      i6 = Pos2C->size[0];
      for (Qn = 0; Qn < 2; Qn++) {
        dec2bin(rt_powd_snf(2.0, PP * (double)Qn) - 1.0, PredPos->size[0], Va);
        Va_data = Va->data;
        nd2 = Va->size[1] >> 1;
        for (indp = 0; indp < nd2; indp++) {
          j2 = (Va->size[1] - indp) - 1;
          xtmp = Va_data[indp];
          Va_data[indp] = Va_data[j2];
          Va_data[j2] = xtmp;
        }
        i7 = ia->size[0];
        ia->size[0] = Va->size[1];
        emxEnsureCapacity_real_T(ia, i7);
        ia_data = ia->data;
        j2 = Va->size[1];
        for (i7 = 0; i7 < j2; i7++) {
          ia_data[i7] = 0.0;
        }
        /*  AnchIDList = [1:NP]; */
        /*  for kk = 1 : 4 */
        /*      if kk == 1 */
        /*          A = RxIDprev(kk,1:RxIDprevLen(kk)); */
        /*      else */
        /*          for ll = 1 : (RxIDprevLen(kk)) */
        /*              if length(find(RxIDprev(kk,ll)==A))==0 */
        /*                   A = [A RxIDprev(kk,ll)]; */
        /*              end */
        /*          end */
        /*      end */
        /*  end */
        i7 = Est_P->size[0];
        Est_P->size[0] = Pos2C->size[0];
        emxEnsureCapacity_creal_T(Est_P, i7);
        Est_P_data = Est_P->data;
        for (i7 = 0; i7 < loop_ub; i7++) {
          Est_P_data[i7].re = 0.0;
          Est_P_data[i7].im = 0.0;
        }
        i7 = Va->size[1];
        for (nd2 = 0; nd2 < i7; nd2++) {
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
        for (P = 0; P < i3; P++) {
          nd2 = (int)ia_data[P] - 1;
          ar = Pos2C_data[P + Pos2C->size[0] * nd2].re;
          ai = Pos2C_data[P + Pos2C->size[0] * nd2].im;
          if (ai == 0.0) {
            Pos2C_re = ar / (double)Va->size[1];
            b_r = 0.0;
          } else if (ar == 0.0) {
            Pos2C_re = 0.0;
            b_r = ai / (double)Va->size[1];
          } else {
            Pos2C_re = ar / (double)Va->size[1];
            b_r = ai / (double)Va->size[1];
          }
          re += Pos2C_re;
          im += b_r;
        }
        Est_H_p_re = 0.0;
        Est_H_p_im = 0.0;
        for (P = 0; P < i4; P++) {
          nd2 = (int)ia_data[P] - 1;
          ar = Pos2C_data[P + Pos2C->size[0] * nd2].re - re;
          ai = Pos2C_data[P + Pos2C->size[0] * nd2].im - im;
          br = tag_pos_b[P].re;
          bi = tag_pos_b[P].im;
          if (bi == 0.0) {
            if (ai == 0.0) {
              Pos2C_re = ar / br;
              b_r = 0.0;
            } else if (ar == 0.0) {
              Pos2C_re = 0.0;
              b_r = ai / br;
            } else {
              Pos2C_re = ar / br;
              b_r = ai / br;
            }
          } else if (br == 0.0) {
            if (ar == 0.0) {
              Pos2C_re = ai / bi;
              b_r = 0.0;
            } else if (ai == 0.0) {
              Pos2C_re = 0.0;
              b_r = -(ar / bi);
            } else {
              Pos2C_re = ai / bi;
              b_r = -(ar / bi);
            }
          } else {
            brm = fabs(br);
            b_r = fabs(bi);
            if (brm > b_r) {
              brm = bi / br;
              b_r = br + brm * bi;
              Pos2C_re = (ar + brm * ai) / b_r;
              b_r = (ai - brm * ar) / b_r;
            } else if (b_r == brm) {
              if (br > 0.0) {
                br = 0.5;
              } else {
                br = -0.5;
              }
              if (bi > 0.0) {
                b_r = 0.5;
              } else {
                b_r = -0.5;
              }
              Pos2C_re = (ar * br + ai * b_r) / brm;
              b_r = (ai * br - ar * b_r) / brm;
            } else {
              brm = br / bi;
              b_r = bi + brm * br;
              Pos2C_re = (brm * ar + ai) / b_r;
              b_r = (brm * ai - ar) / b_r;
            }
          }
          Est_H_p_re += Pos2C_re;
          Est_H_p_im += b_r;
        }
        bi = rt_atan2d_snf(Est_H_p_im, Est_H_p_re);
        Pos2C_re = 0.0;
        for (P = 0; P < i5; P++) {
          /*      [v,indp] = find(PPprev==P); */
          /*      if length(indp)>1 */
          /*          indp = indp(end); */
          /*      end */
          /*      anch_pos = xa(RxIDprev{indp})+j*ya(RxIDprev{indp}); */
          Est_H_p_re = bi * 0.0;
          if (bi == 0.0) {
            Est_H_p_re = exp(Est_H_p_re);
            Est_H_p_im = 0.0;
          } else {
            b_r = exp(Est_H_p_re / 2.0);
            Est_H_p_re = b_r * (b_r * cos(bi));
            Est_H_p_im = b_r * (b_r * sin(bi));
          }
          b_r = tag_pos_b[P].re;
          brm = tag_pos_b[P].im;
          br = re + (b_r * Est_H_p_re - brm * Est_H_p_im);
          brm = im + (b_r * Est_H_p_im + brm * Est_H_p_re);
          Est_P_data[P].re = br;
          Est_P_data[P].im = brm;
          nd2 = (int)ia_data[P] - 1;
          b_r = rt_hypotd_snf(br - Pos2C_data[P + Pos2C->size[0] * nd2].re,
                              brm - Pos2C_data[P + Pos2C->size[0] * nd2].im);
          Pos2C_re += b_r * b_r;
          /*      for q = 1 : length(RxIDprev{indp}) */
          /*          Est_F = Est_F +
           * abs(abs(anch_pos(q)-Pos2C(P,ia(P)))-PrevDist(RxIDprev{indp}(q),P))^2;
           */
          /*      end */
        }
        /*  Est_F = 0; */
        for (P = 0; P < i6; P++) {
          nd2 = 0;
          j2 = 1;
          exitg1 = false;
          while ((!exitg1) && (j2 <= 4)) {
            if (b_PPprev[j2 - 1] == (double)P + 1.0) {
              nd2++;
              j_data[nd2 - 1] = (signed char)j2;
              if (nd2 >= 4) {
                exitg1 = true;
              } else {
                j2++;
              }
            } else {
              j2++;
            }
          }
          if (nd2 < 1) {
            nd2 = 0;
          }
          if (nd2 > 1) {
            indp = j_data[nd2 - 1] - 1;
          } else {
            indp = 0;
          }
          if (b_RxIDprevLen[indp] < 1.0) {
            j2 = 0;
            i7 = 0;
          } else {
            j2 = (int)b_RxIDprevLen[indp];
            i7 = (int)b_RxIDprevLen[indp];
          }
          if (j2 == i7) {
            anch_pos_size[0] = 1;
            anch_pos_size[1] = j2;
            for (i7 = 0; i7 < j2; i7++) {
              nd2 = (int)RxIDprev_data[indp + 4 * i7] - 1;
              brm = ya[nd2];
              anch_pos_data[i7].re = xa[nd2] + 0.0 * brm;
              anch_pos_data[i7].im = brm;
            }
          } else {
            c_binary_expand_op(anch_pos_data, anch_pos_size, xa, b_RxIDprev,
                               indp, j2 - 1, ya, i7 - 1);
          }
          j2 = (int)b_RxIDprevLen[indp];
          if (j2 - 1 >= 0) {
            memset(&Est_Ft_data[0], 0, (unsigned int)j2 * sizeof(double));
          }
          for (nd2 = 0; nd2 < j2; nd2++) {
            b_r = fabs(rt_hypotd_snf(anch_pos_data[nd2].re - Est_P_data[P].re,
                                     anch_pos_data[nd2].im - Est_P_data[P].im) -
                       DistPrev_data[((int)RxIDprev_data[indp + 4 * nd2] +
                                      DistPrev->size[0] * P) -
                                     1]);
            Est_Ft_data[nd2] = b_r * b_r;
            /*          Est_Ft(q) =
             * abs(abs(anch_pos(q)-Pos2C(P,ia(P)))-PrevDist(RxIDprev{indp}(q),P))^2;
             */
          }
          sort(Est_Ft_data, &j2);
          Pos2C_re += Est_Ft_data[0] + Est_Ft_data[1];
        }
        Est_F[Qn] = Pos2C_re;
        Est_C[Qn].re = re;
        Est_C[Qn].im = im;
        Est_H[Qn] = bi;
      }
      if ((Est_F[0] > Est_F[1]) ||
          (rtIsNaN(Est_F[0]) && (!rtIsNaN(Est_F[1])))) {
        b_r = Est_F[1];
        nd2 = 1;
      } else {
        b_r = Est_F[0];
        nd2 = 0;
      }
      if (Sel_F > b_r) {
        Sel_F = b_r;
        Sel_C_re = Est_C[nd2].re;
        Sel_C_im = Est_C[nd2].im;
        *heading_est = Est_H[nd2];
        *cand_tag_pos = Pos2C_data[((int)PP + Pos2C->size[0] * nd2) - 1];
      }
    }
  }
  emxFree_creal_T(&Est_P);
  emxFree_real_T(&ia);
  emxFree_char_T(&Va);
  emxFree_int8_T(&Va_ind);
  emxFree_creal_T(&Pos2C);
  Est_H_p_re = *heading_est * 0.0;
  if (*heading_est == 0.0) {
    Est_H_p_re = exp(Est_H_p_re);
    Est_H_p_im = 0.0;
  } else {
    b_r = exp(Est_H_p_re / 2.0);
    Est_H_p_re = b_r * (b_r * cos(*heading_est));
    Est_H_p_im = b_r * (b_r * sin(*heading_est));
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
}

/* End of code generation (GetPos3.c) */
