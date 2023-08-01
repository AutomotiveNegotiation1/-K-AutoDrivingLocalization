/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: UWBpos.c
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 01-Aug-2023 14:00:18
 */

/* Include Files */
#include "UWBpos.h"
#include "GetInitPos.h"
#include "GetPos2.h"
#include "UWBpos_data.h"
#include "UWBpos_emxutil.h"
#include "UWBpos_initialize.h"
#include "UWBpos_rtwutil.h"
#include "UWBpos_types.h"
#include "combineVectorElements.h"
#include "find.h"
#include "inv.h"
#include "mean.h"
#include "rt_nonfinite.h"
#include "sum.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Variable Definitions */
static double r;

static bool r_not_empty;

static emxArray_creal_T *InterpPosition;

static emxArray_creal_T *Tag_Pos_List;

static emxArray_real_T *TagDistInitCount;

static emxArray_real_T *TagDistInit;

static double heading_est_a[20];

static double centerest_a[40];

static double centerest_a_aver[40];

static double headingest_a_aver[20];

/* Function Definitions */
/*
 * Arguments    : double Ln
 *                double Lp
 *                double TagNum
 *                double Nanchor
 *                const double RxID_data[]
 *                const int RxID_size[2]
 *                const double RxDist_data[]
 *                const int RxDist_size[1]
 *                double s_time
 *                const creal_T tag_pos_b[4]
 *                const double xa[6]
 *                const double ya[6]
 *                emxArray_creal_T *tag_pos_est
 *                double *heading_est
 *                emxArray_creal_T *tag_pos_est_aver
 *                double *headingest_a_aver_v
 * Return Type  : void
 */
void UWBpos(double Ln, double Lp, double TagNum, double Nanchor,
            const double RxID_data[], const int RxID_size[2],
            const double RxDist_data[], const int RxDist_size[1], double s_time,
            const creal_T tag_pos_b[4], const double xa[6], const double ya[6],
            emxArray_creal_T *tag_pos_est, double *heading_est,
            emxArray_creal_T *tag_pos_est_aver, double *headingest_a_aver_v)
{
  emxArray_boolean_T *b_calc_dist_err;
  emxArray_creal_T *InterpPosT;
  emxArray_int32_T *ii;
  emxArray_int32_T *jj;
  emxArray_real_T *A;
  emxArray_real_T *b_C;
  emxArray_real_T *b_xa;
  emxArray_real_T *b_ya;
  emxArray_real_T *calc_dist_err;
  emxArray_real_T *y;
  creal_T TempC;
  creal_T *InterpPosition_data;
  creal_T *Tag_Pos_List_data;
  creal_T *tag_pos_est_aver_data;
  creal_T *tag_pos_est_data;
  double Est_H;
  double x_im;
  double x_re;
  double *A_data;
  double *DistT_data;
  double *TagDistInitCount_data;
  double *TagDistInit_data;
  double *y_data;
  int M;
  int PPC;
  int coffset;
  int i;
  int k;
  int loop_ub;
  int *jj_data;
  bool *calc_dist_err_data;
  if (!isInitialized_UWBpos) {
    UWBpos_initialize();
  }
  TagDistInit_data = TagDistInit->data;
  TagDistInitCount_data = TagDistInitCount->data;
  Tag_Pos_List_data = Tag_Pos_List->data;
  InterpPosition_data = InterpPosition->data;
  if (!r_not_empty) {
    r_not_empty = true;
    i = InterpPosition->size[0] * InterpPosition->size[1];
    InterpPosition->size[0] = (int)Lp;
    InterpPosition->size[1] = 2;
    emxEnsureCapacity_creal_T(InterpPosition, i);
    InterpPosition_data = InterpPosition->data;
    loop_ub = (int)Lp << 1;
    for (i = 0; i < loop_ub; i++) {
      InterpPosition_data[i].re = 0.0;
      InterpPosition_data[i].im = 0.0;
    }
    i = Tag_Pos_List->size[0] * Tag_Pos_List->size[1] * Tag_Pos_List->size[2];
    Tag_Pos_List->size[0] = 9;
    Tag_Pos_List->size[1] = 2;
    Tag_Pos_List->size[2] = (int)Lp;
    emxEnsureCapacity_creal_T(Tag_Pos_List, i);
    Tag_Pos_List_data = Tag_Pos_List->data;
    loop_ub = 18 * (int)Lp;
    for (i = 0; i < loop_ub; i++) {
      Tag_Pos_List_data[i].re = 0.0;
      Tag_Pos_List_data[i].im = 0.0;
    }
    i = TagDistInitCount->size[0] * TagDistInitCount->size[1];
    TagDistInitCount->size[0] = (int)Ln;
    TagDistInitCount->size[1] = (int)Lp;
    emxEnsureCapacity_real_T(TagDistInitCount, i);
    TagDistInitCount_data = TagDistInitCount->data;
    coffset = (int)Ln * (int)Lp;
    for (i = 0; i < coffset; i++) {
      TagDistInitCount_data[i] = 0.0;
    }
    i = TagDistInit->size[0] * TagDistInit->size[1];
    TagDistInit->size[0] = (int)Ln;
    TagDistInit->size[1] = (int)Lp;
    emxEnsureCapacity_real_T(TagDistInit, i);
    TagDistInit_data = TagDistInit->data;
    for (i = 0; i < coffset; i++) {
      TagDistInit_data[i] = 0.0;
    }
  }
  r++;
  i = tag_pos_est->size[0] * tag_pos_est->size[1];
  tag_pos_est->size[0] = 1;
  coffset = (int)Lp;
  tag_pos_est->size[1] = (int)Lp;
  emxEnsureCapacity_creal_T(tag_pos_est, i);
  tag_pos_est_data = tag_pos_est->data;
  for (i = 0; i < coffset; i++) {
    tag_pos_est_data[i].re = 0.0;
    tag_pos_est_data[i].im = 0.0;
  }
  Est_H = 0.0;
  i = tag_pos_est_aver->size[0] * tag_pos_est_aver->size[1];
  tag_pos_est_aver->size[0] = 1;
  tag_pos_est_aver->size[1] = (int)Lp;
  emxEnsureCapacity_creal_T(tag_pos_est_aver, i);
  tag_pos_est_aver_data = tag_pos_est_aver->data;
  for (i = 0; i < coffset; i++) {
    tag_pos_est_aver_data[i].re = 0.0;
    tag_pos_est_aver_data[i].im = 0.0;
  }
  *headingest_a_aver_v = 0.0;
  if (r < 10.0 * Lp) {
    i = (int)Nanchor;
    for (M = 0; M < i; M++) {
      double b_div;
      double q;
      /*              for PP = mod(r-1,Lp)+1:mod(r-1,Lp)+1 */
      q = RxID_data[M];
      TagDistInitCount_data[((int)q +
                             TagDistInitCount->size[0] * ((int)TagNum - 1)) -
                            1]++;
      b_div = TagDistInitCount_data[((int)q + TagDistInitCount->size[0] *
                                                  ((int)TagNum - 1)) -
                                    1];
      TagDistInit_data[((int)q + TagDistInit->size[0] * ((int)TagNum - 1)) -
                       1] =
          TagDistInit_data[((int)q + TagDistInit->size[0] * ((int)TagNum - 1)) -
                           1] *
              (b_div - 1.0) / b_div +
          RxDist_data[M] / b_div;
      /*              end */
    }
  } else {
    emxInit_real_T(&b_xa, 2);
    emxInit_real_T(&b_ya, 2);
    emxInit_real_T(&y, 1);
    if (r == 10.0 * Lp) {
      creal_T Est_P[4];
      i = (int)Nanchor;
      for (M = 0; M < i; M++) {
        double b_div;
        double q;
        /*              for PP = mod(r-1,Lp)+1:mod(r-1,Lp)+1 */
        q = RxID_data[M];
        TagDistInitCount_data[((int)q +
                               TagDistInitCount->size[0] * ((int)TagNum - 1)) -
                              1]++;
        b_div = TagDistInitCount_data[((int)q + TagDistInitCount->size[0] *
                                                    ((int)TagNum - 1)) -
                                      1];
        TagDistInit_data[((int)q + TagDistInit->size[0] * ((int)TagNum - 1)) -
                         1] =
            TagDistInit_data[((int)q +
                              TagDistInit->size[0] * ((int)TagNum - 1)) -
                             1] *
                (b_div - 1.0) / b_div +
            RxDist_data[M] / b_div;
        /*              end */
      }
      emxInit_real_T(&calc_dist_err, 2);
      i = calc_dist_err->size[0] * calc_dist_err->size[1];
      calc_dist_err->size[0] = TagDistInit->size[0];
      calc_dist_err->size[1] = TagDistInit->size[1];
      emxEnsureCapacity_real_T(calc_dist_err, i);
      DistT_data = calc_dist_err->data;
      M = TagDistInit->size[0] * TagDistInit->size[1];
      for (i = 0; i < M; i++) {
        DistT_data[i] = TagDistInit_data[i];
      }
      M--;
      for (loop_ub = 0; loop_ub <= M; loop_ub++) {
        if (TagDistInit_data[loop_ub] == 0.0) {
          DistT_data[loop_ub] = 1.0E+6;
        }
      }
      sum(calc_dist_err, y);
      y_data = y->data;
      emxInit_boolean_T(&b_calc_dist_err, 1);
      i = b_calc_dist_err->size[0];
      b_calc_dist_err->size[0] = y->size[0];
      emxEnsureCapacity_boolean_T(b_calc_dist_err, i);
      calc_dist_err_data = b_calc_dist_err->data;
      loop_ub = y->size[0];
      for (i = 0; i < loop_ub; i++) {
        calc_dist_err_data[i] = (y_data[i] < 100000.0);
      }
      emxInit_int32_T(&jj, 1);
      eml_find(b_calc_dist_err, jj);
      jj_data = jj->data;
      emxFree_boolean_T(&b_calc_dist_err);
      i = y->size[0];
      y->size[0] = jj->size[0];
      emxEnsureCapacity_real_T(y, i);
      y_data = y->data;
      loop_ub = jj->size[0];
      for (i = 0; i < loop_ub; i++) {
        y_data[i] = jj_data[i];
      }
      emxFree_int32_T(&jj);
      M = calc_dist_err->size[1];
      emxInit_real_T(&A, 2);
      i = A->size[0] * A->size[1];
      A->size[0] = y->size[0];
      A->size[1] = calc_dist_err->size[1];
      emxEnsureCapacity_real_T(A, i);
      TagDistInitCount_data = A->data;
      for (i = 0; i < M; i++) {
        loop_ub = y->size[0];
        for (k = 0; k < loop_ub; k++) {
          TagDistInitCount_data[k + A->size[0] * i] =
              DistT_data[((int)y_data[k] + calc_dist_err->size[0] * i) - 1];
        }
      }
      i = calc_dist_err->size[0] * calc_dist_err->size[1];
      calc_dist_err->size[0] = A->size[0];
      calc_dist_err->size[1] = A->size[1];
      emxEnsureCapacity_real_T(calc_dist_err, i);
      DistT_data = calc_dist_err->data;
      loop_ub = A->size[0] * A->size[1];
      for (i = 0; i < loop_ub; i++) {
        DistT_data[i] = TagDistInitCount_data[i];
      }
      emxFree_real_T(&A);
      i = b_xa->size[0] * b_xa->size[1];
      b_xa->size[0] = 1;
      b_xa->size[1] = y->size[0];
      emxEnsureCapacity_real_T(b_xa, i);
      TagDistInit_data = b_xa->data;
      loop_ub = y->size[0];
      i = b_ya->size[0] * b_ya->size[1];
      b_ya->size[0] = 1;
      b_ya->size[1] = y->size[0];
      emxEnsureCapacity_real_T(b_ya, i);
      DistT_data = b_ya->data;
      for (i = 0; i < loop_ub; i++) {
        M = (int)y_data[i] - 1;
        TagDistInit_data[i] = xa[M];
        DistT_data[i] = ya[M];
      }
      Est_H = GetInitPos(b_xa, b_ya, calc_dist_err, tag_pos_b, y->size[0], Lp,
                         Est_P);
      emxFree_real_T(&calc_dist_err);
      i = tag_pos_est->size[0] * tag_pos_est->size[1];
      tag_pos_est->size[0] = 1;
      tag_pos_est->size[1] = 4;
      emxEnsureCapacity_creal_T(tag_pos_est, i);
      tag_pos_est_data = tag_pos_est->data;
      tag_pos_est_data[0] = Est_P[0];
      tag_pos_est_data[1] = Est_P[1];
      tag_pos_est_data[2] = Est_P[2];
      tag_pos_est_data[3] = Est_P[3];
      for (M = 0; M < coffset; M++) {
        for (loop_ub = 0; loop_ub < 9; loop_ub++) {
          i = loop_ub + 18 * M;
          Tag_Pos_List_data[i].re =
              s_time + (((double)loop_ub + 1.0) - 9.0) * 0.1;
          Tag_Pos_List_data[i].im = 0.0;
          Tag_Pos_List_data[i + 9] = Est_P[M];
        }
      }
    } else {
      creal_T TempC_tmp;
      double Est_C_im;
      double Est_C_re;
      double Est_H_p_im;
      double Est_H_p_re;
      double Pm_re;
      double Px_re_tmp;
      double b_Est_H;
      double b_Px_re_tmp;
      double b_div;
      double q;
      /* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
      /* %%%%%%%%%%%%%%%%%  Next Position Prediction    %%%%%%%%%%%%%%%%%% */
      for (PPC = 0; PPC < coffset; PPC++) {
        creal_T Pm[27];
        creal_T b_Px[27];
        creal_T Px[9];
        creal_T b_Pm[9];
        creal_T c_Px[3];
        for (k = 0; k < 9; k++) {
          M = k + 18 * PPC;
          Est_H_p_re = Tag_Pos_List_data[M].re - s_time;
          Est_H_p_im = Tag_Pos_List_data[M].im;
          Pm[k].re = Est_H_p_re * Est_H_p_re - Est_H_p_im * Est_H_p_im;
          q = Est_H_p_re * Est_H_p_im;
          Pm[k].im = q + q;
          Pm[k + 9].re = Est_H_p_re;
          Pm[k + 9].im = Est_H_p_im;
          Pm[k + 18].re = 1.0;
          Pm[k + 18].im = 0.0;
        }
        for (i = 0; i < 3; i++) {
          for (k = 0; k < 3; k++) {
            Est_H_p_re = 0.0;
            Est_H_p_im = 0.0;
            for (loop_ub = 0; loop_ub < 9; loop_ub++) {
              M = loop_ub + 9 * i;
              Pm_re = Pm[M].re;
              Est_C_im = -Pm[M].im;
              M = loop_ub + 9 * k;
              b_Est_H = Pm[M].im;
              q = Pm[M].re;
              Est_H_p_re += Pm_re * q - Est_C_im * b_Est_H;
              Est_H_p_im += Pm_re * b_Est_H + Est_C_im * q;
            }
            loop_ub = i + 3 * k;
            b_Pm[loop_ub].re = Est_H_p_re;
            b_Pm[loop_ub].im = Est_H_p_im;
          }
        }
        b_inv(b_Pm, Px);
        for (i = 0; i < 3; i++) {
          b_Est_H = Px[i].re;
          q = Px[i].im;
          b_div = Px[i + 3].re;
          Est_C_re = Px[i + 3].im;
          Px_re_tmp = Px[i + 6].re;
          b_Px_re_tmp = Px[i + 6].im;
          for (k = 0; k < 9; k++) {
            Pm_re = Pm[k].re;
            Est_C_im = -Pm[k].im;
            Est_H_p_re = b_Est_H * Pm_re - q * Est_C_im;
            Est_H_p_im = b_Est_H * Est_C_im + q * Pm_re;
            Pm_re = Pm[k + 9].re;
            Est_C_im = -Pm[k + 9].im;
            Est_H_p_re += b_div * Pm_re - Est_C_re * Est_C_im;
            Est_H_p_im += b_div * Est_C_im + Est_C_re * Pm_re;
            Pm_re = Pm[k + 18].re;
            Est_C_im = -Pm[k + 18].im;
            Est_H_p_re += Px_re_tmp * Pm_re - b_Px_re_tmp * Est_C_im;
            Est_H_p_im += Px_re_tmp * Est_C_im + b_Px_re_tmp * Pm_re;
            loop_ub = i + 3 * k;
            b_Px[loop_ub].re = Est_H_p_re;
            b_Px[loop_ub].im = Est_H_p_im;
          }
        }
        for (i = 0; i < 9; i++) {
          Px[i].re = Tag_Pos_List_data[(i + 18 * PPC) + 9].re;
          Px[i].im = 0.0;
        }
        for (i = 0; i < 3; i++) {
          Est_H_p_re = 0.0;
          Est_H_p_im = 0.0;
          for (k = 0; k < 9; k++) {
            M = i + 3 * k;
            b_Est_H = b_Px[M].re;
            q = Px[k].im;
            b_div = b_Px[M].im;
            Est_C_re = Px[k].re;
            Est_H_p_re += b_Est_H * Est_C_re - b_div * q;
            Est_H_p_im += b_Est_H * q + b_div * Est_C_re;
          }
          c_Px[i].re = Est_H_p_re;
          c_Px[i].im = Est_H_p_im;
        }
        InterpPosition_data[PPC] = c_Px[2];
        /*  Py = pm x A */
        /*  tr(pm)xPy = tr(Pm)pm x A */
        /*  inv(tr(Pm)Pm) x tr(pm)xPy = A */
        for (k = 0; k < 9; k++) {
          M = k + 18 * PPC;
          Est_H_p_re = Tag_Pos_List_data[M].re - s_time;
          Est_H_p_im = Tag_Pos_List_data[M].im;
          Pm[k].re = Est_H_p_re * Est_H_p_re - Est_H_p_im * Est_H_p_im;
          q = Est_H_p_re * Est_H_p_im;
          Pm[k].im = q + q;
          Pm[k + 9].re = Est_H_p_re;
          Pm[k + 9].im = Est_H_p_im;
          Pm[k + 18].re = 1.0;
          Pm[k + 18].im = 0.0;
        }
        for (i = 0; i < 3; i++) {
          for (k = 0; k < 3; k++) {
            Est_H_p_re = 0.0;
            Est_H_p_im = 0.0;
            for (loop_ub = 0; loop_ub < 9; loop_ub++) {
              M = loop_ub + 9 * i;
              Pm_re = Pm[M].re;
              Est_C_im = -Pm[M].im;
              M = loop_ub + 9 * k;
              b_Est_H = Pm[M].im;
              q = Pm[M].re;
              Est_H_p_re += Pm_re * q - Est_C_im * b_Est_H;
              Est_H_p_im += Pm_re * b_Est_H + Est_C_im * q;
            }
            loop_ub = i + 3 * k;
            b_Pm[loop_ub].re = Est_H_p_re;
            b_Pm[loop_ub].im = Est_H_p_im;
          }
        }
        b_inv(b_Pm, Px);
        for (i = 0; i < 3; i++) {
          b_Est_H = Px[i].re;
          q = Px[i].im;
          b_div = Px[i + 3].re;
          Est_C_re = Px[i + 3].im;
          Px_re_tmp = Px[i + 6].re;
          b_Px_re_tmp = Px[i + 6].im;
          for (k = 0; k < 9; k++) {
            Pm_re = Pm[k].re;
            Est_C_im = -Pm[k].im;
            Est_H_p_re = b_Est_H * Pm_re - q * Est_C_im;
            Est_H_p_im = b_Est_H * Est_C_im + q * Pm_re;
            Pm_re = Pm[k + 9].re;
            Est_C_im = -Pm[k + 9].im;
            Est_H_p_re += b_div * Pm_re - Est_C_re * Est_C_im;
            Est_H_p_im += b_div * Est_C_im + Est_C_re * Pm_re;
            Pm_re = Pm[k + 18].re;
            Est_C_im = -Pm[k + 18].im;
            Est_H_p_re += Px_re_tmp * Pm_re - b_Px_re_tmp * Est_C_im;
            Est_H_p_im += Px_re_tmp * Est_C_im + b_Px_re_tmp * Pm_re;
            loop_ub = i + 3 * k;
            b_Px[loop_ub].re = Est_H_p_re;
            b_Px[loop_ub].im = Est_H_p_im;
          }
        }
        for (i = 0; i < 9; i++) {
          Px[i].re = Tag_Pos_List_data[(i + 18 * PPC) + 9].im;
          Px[i].im = 0.0;
        }
        for (i = 0; i < 3; i++) {
          Est_H_p_re = 0.0;
          Est_H_p_im = 0.0;
          for (k = 0; k < 9; k++) {
            M = i + 3 * k;
            b_Est_H = b_Px[M].re;
            q = Px[k].im;
            b_div = b_Px[M].im;
            Est_C_re = Px[k].re;
            Est_H_p_re += b_Est_H * Est_C_re - b_div * q;
            Est_H_p_im += b_Est_H * q + b_div * Est_C_re;
          }
          c_Px[i].re = Est_H_p_re;
          c_Px[i].im = Est_H_p_im;
        }
        InterpPosition_data[PPC + InterpPosition->size[0]] = c_Px[2];
        /*  Py = pm x A */
        /*  tr(pm)xPy = tr(Pm)pm x A */
        /*  inv(tr(Pm)Pm) x tr(pm)xPy = A */
      }
      /* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
      /* %%%%%%%%%%%%%%%% New Position Calc.   %%%%%%%%%%%%%%%%%%%%%%%%%%% */
      /*          for PP = mod(r-1,Lp)+1:mod(r-1,Lp)+1 */
      emxInit_creal_T(&InterpPosT, 1);
      if (RxID_size[1] > 1) {
        creal_T Est_P[4];
        creal_T anch_pos_data[4];
        i = InterpPosT->size[0];
        InterpPosT->size[0] = InterpPosition->size[0];
        emxEnsureCapacity_creal_T(InterpPosT, i);
        tag_pos_est_aver_data = InterpPosT->data;
        loop_ub = InterpPosition->size[0];
        for (i = 0; i < loop_ub; i++) {
          b_Est_H = InterpPosition_data[i + InterpPosition->size[0]].im;
          q = InterpPosition_data[i + InterpPosition->size[0]].re;
          tag_pos_est_aver_data[i].re =
              InterpPosition_data[i].re + (0.0 * q - b_Est_H);
          tag_pos_est_aver_data[i].im =
              InterpPosition_data[i].im + (0.0 * b_Est_H + q);
        }
        Est_H =
            GetPos2(xa, ya, RxDist_data, RxDist_size[0], RxID_data, RxID_size,
                    tag_pos_b, Nanchor, TagNum, InterpPosT, Est_P, &TempC);
        loop_ub = RxID_size[1];
        for (i = 0; i < loop_ub; i++) {
          q = RxID_data[i];
          b_Est_H = ya[(int)q - 1];
          anch_pos_data[i].re = xa[(int)q - 1] + 0.0 * b_Est_H;
          anch_pos_data[i].im = b_Est_H;
        }
        if (!(Nanchor == 1.0)) {
          i = (int)Nanchor;
          emxInit_real_T(&calc_dist_err, 2);
          k = calc_dist_err->size[0] * calc_dist_err->size[1];
          calc_dist_err->size[0] = 1;
          calc_dist_err->size[1] = (int)Nanchor;
          emxEnsureCapacity_real_T(calc_dist_err, k);
          TagDistInitCount_data = calc_dist_err->data;
          for (M = 0; M < i; M++) {
            b_div = fabs(
                rt_hypotd_snf(anch_pos_data[M].re - Est_P[(int)TagNum - 1].re,
                              anch_pos_data[M].im - Est_P[(int)TagNum - 1].im) -
                RxDist_data[M]);
            TagDistInitCount_data[M] = b_div * b_div;
          }
          b_Est_H = fmax(sqrt(combineVectorElements(calc_dist_err) /
                              (double)calc_dist_err->size[1]) *
                             0.3,
                         0.2);
          emxInit_boolean_T(&b_calc_dist_err, 2);
          i = b_calc_dist_err->size[0] * b_calc_dist_err->size[1];
          b_calc_dist_err->size[0] = 1;
          b_calc_dist_err->size[1] = calc_dist_err->size[1];
          emxEnsureCapacity_boolean_T(b_calc_dist_err, i);
          calc_dist_err_data = b_calc_dist_err->data;
          loop_ub = calc_dist_err->size[1];
          for (i = 0; i < loop_ub; i++) {
            calc_dist_err_data[i] = (TagDistInitCount_data[i] < b_Est_H);
          }
          emxInit_int32_T(&ii, 2);
          emxInit_int32_T(&jj, 2);
          b_eml_find(b_calc_dist_err, ii, jj);
          jj_data = jj->data;
          emxFree_boolean_T(&b_calc_dist_err);
          emxFree_int32_T(&ii);
          i = calc_dist_err->size[0] * calc_dist_err->size[1];
          calc_dist_err->size[0] = 1;
          calc_dist_err->size[1] = jj->size[1];
          emxEnsureCapacity_real_T(calc_dist_err, i);
          TagDistInitCount_data = calc_dist_err->data;
          loop_ub = jj->size[1];
          for (i = 0; i < loop_ub; i++) {
            TagDistInitCount_data[i] = jj_data[i];
          }
          emxFree_int32_T(&jj);
          if (calc_dist_err->size[1] >= 3) {
            double C[4];
            double b_A[4];
            i = b_xa->size[0] * b_xa->size[1];
            b_xa->size[0] = 1;
            b_xa->size[1] = calc_dist_err->size[1];
            emxEnsureCapacity_real_T(b_xa, i);
            TagDistInit_data = b_xa->data;
            loop_ub = calc_dist_err->size[1];
            i = b_ya->size[0] * b_ya->size[1];
            b_ya->size[0] = 1;
            b_ya->size[1] = calc_dist_err->size[1];
            emxEnsureCapacity_real_T(b_ya, i);
            DistT_data = b_ya->data;
            for (i = 0; i < loop_ub; i++) {
              q = TagDistInitCount_data[i];
              TagDistInit_data[i] = anch_pos_data[(int)q - 1].re;
              DistT_data[i] = anch_pos_data[(int)q - 1].im;
            }
            M = b_xa->size[1] - 1;
            emxInit_real_T(&A, 2);
            i = A->size[0] * A->size[1];
            A->size[0] = b_xa->size[1] - 1;
            A->size[1] = 2;
            emxEnsureCapacity_real_T(A, i);
            A_data = A->data;
            coffset = (b_xa->size[1] - 1) << 1;
            for (i = 0; i < coffset; i++) {
              A_data[i] = 0.0;
            }
            i = y->size[0];
            y->size[0] = b_xa->size[1] - 1;
            emxEnsureCapacity_real_T(y, i);
            y_data = y->data;
            loop_ub = b_xa->size[1];
            b_div =
                RxDist_data[(int)TagDistInitCount_data[b_xa->size[1] - 1] - 1];
            for (k = 0; k <= loop_ub - 2; k++) {
              A_data[k] = -2.0 * (TagDistInit_data[k] - TagDistInit_data[M]);
              A_data[k + A->size[0]] = -2.0 * (DistT_data[k] - DistT_data[M]);
              b_Est_H = RxDist_data[(int)TagDistInitCount_data[k] - 1];
              y_data[k] = ((b_Est_H * b_Est_H - b_div * b_div) -
                           (TagDistInit_data[k] * TagDistInit_data[k] -
                            TagDistInit_data[M] * TagDistInit_data[M])) -
                          (DistT_data[k] * DistT_data[k] -
                           DistT_data[M] * DistT_data[M]);
            }
            loop_ub = A->size[0];
            for (PPC = 0; PPC < 2; PPC++) {
              coffset = PPC << 1;
              M = PPC * A->size[0];
              C[coffset] = 0.0;
              C[coffset + 1] = 0.0;
              for (k = 0; k < loop_ub; k++) {
                b_Est_H = A_data[M + k];
                C[coffset] += A_data[k] * b_Est_H;
                C[coffset + 1] += A_data[A->size[0] + k] * b_Est_H;
              }
            }
            inv(C, b_A);
            M = A->size[0];
            emxInit_real_T(&b_C, 2);
            i = b_C->size[0] * b_C->size[1];
            b_C->size[0] = 2;
            b_C->size[1] = A->size[0];
            emxEnsureCapacity_real_T(b_C, i);
            TagDistInitCount_data = b_C->data;
            for (PPC = 0; PPC < M; PPC++) {
              coffset = PPC << 1;
              b_Est_H = A_data[A->size[0] + PPC];
              TagDistInitCount_data[coffset] =
                  b_A[0] * A_data[PPC] + b_A[2] * b_Est_H;
              TagDistInitCount_data[coffset + 1] =
                  b_A[1] * A_data[PPC] + b_A[3] * b_Est_H;
            }
            emxFree_real_T(&A);
            loop_ub = b_C->size[1];
            b_Est_H = 0.0;
            b_div = 0.0;
            for (k = 0; k < loop_ub; k++) {
              M = k << 1;
              b_Est_H += TagDistInitCount_data[M] * y_data[k];
              b_div += TagDistInitCount_data[M + 1] * y_data[k];
            }
            emxFree_real_T(&b_C);
            TempC.re = b_div * 0.0;
            TempC.im = b_div;
            q = b_Est_H + TempC.re;
            Est_P[(int)TagNum - 1].re = q;
            Est_P[(int)TagNum - 1].im = b_div;
            TempC.re = q;
            if (Est_P[0].im == 0.0) {
              b_div = Est_P[0].re / 4.0;
              b_Est_H = 0.0;
            } else if (Est_P[0].re == 0.0) {
              b_div = 0.0;
              b_Est_H = Est_P[0].im / 4.0;
            } else {
              b_div = Est_P[0].re / 4.0;
              b_Est_H = Est_P[0].im / 4.0;
            }
            Est_C_re = b_div;
            Est_C_im = b_Est_H;
            if (Est_P[1].im == 0.0) {
              b_div = Est_P[1].re / 4.0;
              b_Est_H = 0.0;
            } else if (Est_P[1].re == 0.0) {
              b_div = 0.0;
              b_Est_H = Est_P[1].im / 4.0;
            } else {
              b_div = Est_P[1].re / 4.0;
              b_Est_H = Est_P[1].im / 4.0;
            }
            Est_C_re += b_div;
            Est_C_im += b_Est_H;
            if (Est_P[2].im == 0.0) {
              b_div = Est_P[2].re / 4.0;
              b_Est_H = 0.0;
            } else if (Est_P[2].re == 0.0) {
              b_div = 0.0;
              b_Est_H = Est_P[2].im / 4.0;
            } else {
              b_div = Est_P[2].re / 4.0;
              b_Est_H = Est_P[2].im / 4.0;
            }
            Est_C_re += b_div;
            Est_C_im += b_Est_H;
            if (Est_P[3].im == 0.0) {
              b_div = Est_P[3].re / 4.0;
              b_Est_H = 0.0;
            } else if (Est_P[3].re == 0.0) {
              b_div = 0.0;
              b_Est_H = Est_P[3].im / 4.0;
            } else {
              b_div = Est_P[3].re / 4.0;
              b_Est_H = Est_P[3].im / 4.0;
            }
            Est_C_re += b_div;
            Est_C_im += b_Est_H;
            b_Px_re_tmp = Est_P[3].re - Est_C_re;
            Pm_re = Est_P[3].im - Est_C_im;
            if (tag_pos_b[3].im == 0.0) {
              if (Pm_re == 0.0) {
                Est_H_p_re = b_Px_re_tmp / tag_pos_b[3].re;
                Est_H_p_im = 0.0;
              } else if (b_Px_re_tmp == 0.0) {
                Est_H_p_re = 0.0;
                Est_H_p_im = Pm_re / tag_pos_b[3].re;
              } else {
                Est_H_p_re = b_Px_re_tmp / tag_pos_b[3].re;
                Est_H_p_im = Pm_re / tag_pos_b[3].re;
              }
            } else if (tag_pos_b[3].re == 0.0) {
              if (b_Px_re_tmp == 0.0) {
                Est_H_p_re = Pm_re / tag_pos_b[3].im;
                Est_H_p_im = 0.0;
              } else if (Pm_re == 0.0) {
                Est_H_p_re = 0.0;
                Est_H_p_im = -(b_Px_re_tmp / tag_pos_b[3].im);
              } else {
                Est_H_p_re = Pm_re / tag_pos_b[3].im;
                Est_H_p_im = -(b_Px_re_tmp / tag_pos_b[3].im);
              }
            } else {
              Px_re_tmp = fabs(tag_pos_b[3].re);
              q = fabs(tag_pos_b[3].im);
              if (Px_re_tmp > q) {
                q = tag_pos_b[3].im / tag_pos_b[3].re;
                b_Est_H = tag_pos_b[3].re + q * tag_pos_b[3].im;
                Est_H_p_re = (b_Px_re_tmp + q * Pm_re) / b_Est_H;
                Est_H_p_im = (Pm_re - q * b_Px_re_tmp) / b_Est_H;
              } else if (q == Px_re_tmp) {
                if (tag_pos_b[3].re > 0.0) {
                  q = 0.5;
                } else {
                  q = -0.5;
                }
                if (tag_pos_b[3].im > 0.0) {
                  b_Est_H = 0.5;
                } else {
                  b_Est_H = -0.5;
                }
                Est_H_p_re = (b_Px_re_tmp * q + Pm_re * b_Est_H) / Px_re_tmp;
                Est_H_p_im = (Pm_re * q - b_Px_re_tmp * b_Est_H) / Px_re_tmp;
              } else {
                q = tag_pos_b[3].re / tag_pos_b[3].im;
                b_Est_H = tag_pos_b[3].im + q * tag_pos_b[3].re;
                Est_H_p_re = (q * b_Px_re_tmp + Pm_re) / b_Est_H;
                Est_H_p_im = (q * Pm_re - b_Px_re_tmp) / b_Est_H;
              }
            }
            Est_H = rt_atan2d_snf(Est_H_p_im, Est_H_p_re);
            b_Est_H = Est_H * 0.0;
            if (b_Est_H == 0.0) {
              x_re = cos(Est_H);
              x_im = sin(Est_H);
            } else if (Est_H == 0.0) {
              x_re = rtNaN;
              x_im = 0.0;
            } else {
              x_re = rtNaN;
              x_im = rtNaN;
            }
            Est_P[0].re =
                Est_C_re + (tag_pos_b[0].re * x_re - tag_pos_b[0].im * x_im);
            Est_P[0].im =
                Est_C_im + (tag_pos_b[0].re * x_im + tag_pos_b[0].im * x_re);
            if (b_Est_H == 0.0) {
              x_re = cos(Est_H);
              x_im = sin(Est_H);
            } else if (Est_H == 0.0) {
              x_re = rtNaN;
              x_im = 0.0;
            } else {
              x_re = rtNaN;
              x_im = rtNaN;
            }
            Est_P[1].re =
                Est_C_re + (tag_pos_b[1].re * x_re - tag_pos_b[1].im * x_im);
            Est_P[1].im =
                Est_C_im + (tag_pos_b[1].re * x_im + tag_pos_b[1].im * x_re);
            if (b_Est_H == 0.0) {
              x_re = cos(Est_H);
              x_im = sin(Est_H);
            } else if (Est_H == 0.0) {
              x_re = rtNaN;
              x_im = 0.0;
            } else {
              x_re = rtNaN;
              x_im = rtNaN;
            }
            Est_P[2].re =
                Est_C_re + (tag_pos_b[2].re * x_re - tag_pos_b[2].im * x_im);
            Est_P[2].im =
                Est_C_im + (tag_pos_b[2].re * x_im + tag_pos_b[2].im * x_re);
            if (b_Est_H == 0.0) {
              x_re = cos(Est_H);
              x_im = sin(Est_H);
            } else if (Est_H == 0.0) {
              x_re = rtNaN;
              x_im = 0.0;
            } else {
              x_re = rtNaN;
              x_im = rtNaN;
            }
            Est_P[3].re =
                Est_C_re + (tag_pos_b[3].re * x_re - tag_pos_b[3].im * x_im);
            Est_P[3].im =
                Est_C_im + (tag_pos_b[3].re * x_im + tag_pos_b[3].im * x_re);
          }
          emxFree_real_T(&calc_dist_err);
        }
        i = tag_pos_est->size[0] * tag_pos_est->size[1];
        tag_pos_est->size[0] = 1;
        tag_pos_est->size[1] = 4;
        emxEnsureCapacity_creal_T(tag_pos_est, i);
        tag_pos_est_data = tag_pos_est->data;
        tag_pos_est_data[0] = Est_P[0];
        tag_pos_est_data[1] = Est_P[1];
        tag_pos_est_data[2] = Est_P[2];
        tag_pos_est_data[3] = Est_P[3];
      } else {
        i = InterpPosT->size[0];
        InterpPosT->size[0] = InterpPosition->size[0];
        emxEnsureCapacity_creal_T(InterpPosT, i);
        tag_pos_est_aver_data = InterpPosT->data;
        loop_ub = InterpPosition->size[0];
        for (i = 0; i < loop_ub; i++) {
          b_Est_H = InterpPosition_data[i + InterpPosition->size[0]].im;
          q = InterpPosition_data[i + InterpPosition->size[0]].re;
          tag_pos_est_aver_data[i].re =
              InterpPosition_data[i].re + (0.0 * q - b_Est_H);
          tag_pos_est_aver_data[i].im =
              InterpPosition_data[i].im + (0.0 * b_Est_H + q);
        }
        TempC = mean(InterpPosT);
        Est_H_p_re = 0.0;
        Est_H_p_im = 0.0;
        i = InterpPosT->size[0];
        for (M = 0; M < i; M++) {
          b_Px_re_tmp = tag_pos_est_aver_data[M].re - TempC.re;
          Pm_re = tag_pos_est_aver_data[M].im - TempC.im;
          b_Est_H = tag_pos_b[M].re;
          b_div = tag_pos_b[M].im;
          if (b_div == 0.0) {
            if (Pm_re == 0.0) {
              Est_C_re = b_Px_re_tmp / b_Est_H;
              b_Est_H = 0.0;
            } else if (b_Px_re_tmp == 0.0) {
              Est_C_re = 0.0;
              b_Est_H = Pm_re / b_Est_H;
            } else {
              Est_C_re = b_Px_re_tmp / b_Est_H;
              b_Est_H = Pm_re / b_Est_H;
            }
          } else if (b_Est_H == 0.0) {
            if (b_Px_re_tmp == 0.0) {
              Est_C_re = Pm_re / b_div;
              b_Est_H = 0.0;
            } else if (Pm_re == 0.0) {
              Est_C_re = 0.0;
              b_Est_H = -(b_Px_re_tmp / b_div);
            } else {
              Est_C_re = Pm_re / b_div;
              b_Est_H = -(b_Px_re_tmp / b_div);
            }
          } else {
            Px_re_tmp = fabs(b_Est_H);
            q = fabs(b_div);
            if (Px_re_tmp > q) {
              q = b_div / b_Est_H;
              b_Est_H += q * b_div;
              Est_C_re = (b_Px_re_tmp + q * Pm_re) / b_Est_H;
              b_Est_H = (Pm_re - q * b_Px_re_tmp) / b_Est_H;
            } else if (q == Px_re_tmp) {
              if (b_Est_H > 0.0) {
                q = 0.5;
              } else {
                q = -0.5;
              }
              if (b_div > 0.0) {
                b_Est_H = 0.5;
              } else {
                b_Est_H = -0.5;
              }
              Est_C_re = (b_Px_re_tmp * q + Pm_re * b_Est_H) / Px_re_tmp;
              b_Est_H = (Pm_re * q - b_Px_re_tmp * b_Est_H) / Px_re_tmp;
            } else {
              q = b_Est_H / b_div;
              b_Est_H = b_div + q * b_Est_H;
              Est_C_re = (q * b_Px_re_tmp + Pm_re) / b_Est_H;
              b_Est_H = (q * Pm_re - b_Px_re_tmp) / b_Est_H;
            }
          }
          Est_H_p_re += Est_C_re;
          Est_H_p_im += b_Est_H;
        }
        b_Est_H = rt_atan2d_snf(Est_H_p_im, Est_H_p_re);
        i = InterpPosT->size[0];
        if (i - 1 >= 0) {
          if (b_Est_H * 0.0 == 0.0) {
            x_re = cos(b_Est_H);
            x_im = sin(b_Est_H);
          } else if (b_Est_H == 0.0) {
            x_re = rtNaN;
            x_im = 0.0;
          } else {
            x_re = rtNaN;
            x_im = rtNaN;
          }
        }
        for (M = 0; M < i; M++) {
          b_Est_H = tag_pos_b[M].re;
          q = tag_pos_b[M].im;
          tag_pos_est_data[M].re = TempC.re + (b_Est_H * x_re - q * x_im);
          tag_pos_est_data[M].im = TempC.im + (b_Est_H * x_im + q * x_re);
        }
        b_Est_H =
            InterpPosition_data[((int)TagNum + InterpPosition->size[0]) - 1].im;
        q = InterpPosition_data[((int)TagNum + InterpPosition->size[0]) - 1].re;
        TempC.re =
            InterpPosition_data[(int)TagNum - 1].re + (0.0 * q - b_Est_H);
        TempC.im =
            InterpPosition_data[(int)TagNum - 1].im + (0.0 * b_Est_H + q);
      }
      emxFree_creal_T(&InterpPosT);
      for (i = 0; i < 2; i++) {
        for (k = 0; k < 8; k++) {
          loop_ub = (k + 9 * i) + 18 * ((int)TagNum - 1);
          Tag_Pos_List_data[loop_ub] = Tag_Pos_List_data[loop_ub + 1];
        }
      }
      i = 18 * ((int)TagNum - 1);
      Tag_Pos_List_data[i + 8].re = s_time;
      Tag_Pos_List_data[i + 8].im = 0.0;
      Tag_Pos_List_data[i + 17] = TempC;
      /*              Tag_Pos_List(1:2,:,PP) = Tag_Pos_List(2:3,:,PP); */
      /*              Tag_Pos_List(3,:,PP) = [s_time(r) tag_pos_est(PP)]; */
      /*          end */
      TempC_tmp = b_mean(tag_pos_est);
      for (i = 0; i < 19; i++) {
        heading_est_a[i] = heading_est_a[i + 1];
        centerest_a[i] = centerest_a[i + 1];
        centerest_a_aver[i] = centerest_a_aver[i + 1];
        centerest_a[i + 20] = centerest_a[i + 21];
        centerest_a_aver[i + 20] = centerest_a_aver[i + 21];
        headingest_a_aver[i] = headingest_a_aver[i + 1];
      }
      if (Est_H - heading_est_a[18] > 3.1415926535897931) {
        heading_est_a[19] = Est_H - 6.2831853071795862;
      } else if (heading_est_a[18] - Est_H > 3.1415926535897931) {
        heading_est_a[19] = Est_H + 6.2831853071795862;
      } else {
        heading_est_a[19] = Est_H;
      }
      centerest_a[19] = TempC_tmp.re;
      centerest_a[39] = TempC_tmp.im;
      if (r > 20.0) {
        creal_T x[10];
        for (i = 0; i < 10; i++) {
          q = centerest_a[i + 20];
          x[i].re = centerest_a[i] + 0.0 * q;
          x[i].im = q;
        }
        x_re = x[0].re;
        x_im = x[0].im;
        for (k = 0; k < 9; k++) {
          x_re += x[k + 1].re;
          x_im += x[k + 1].im;
        }
        if (x_im == 0.0) {
          Est_H_p_re = x_re / 10.0;
          Est_H_p_im = 0.0;
        } else if (x_re == 0.0) {
          Est_H_p_re = 0.0;
          Est_H_p_im = x_im / 10.0;
        } else {
          Est_H_p_re = x_re / 10.0;
          Est_H_p_im = x_im / 10.0;
        }
        for (i = 0; i < 10; i++) {
          q = centerest_a[i + 30];
          x[i].re = centerest_a[i + 10] + 0.0 * q;
          x[i].im = q;
        }
        x_re = x[0].re;
        x_im = x[0].im;
        Px_re_tmp = heading_est_a[0];
        Est_C_re = heading_est_a[10];
        for (k = 0; k < 9; k++) {
          x_re += x[k + 1].re;
          x_im += x[k + 1].im;
          Px_re_tmp += heading_est_a[k + 1];
          Est_C_re += heading_est_a[k + 11];
        }
        if (x_im == 0.0) {
          TempC.re = x_re / 10.0;
          TempC.im = 0.0;
        } else if (x_re == 0.0) {
          TempC.re = 0.0;
          TempC.im = x_im / 10.0;
        } else {
          TempC.re = x_re / 10.0;
          TempC.im = x_im / 10.0;
        }
        b_Est_H = TempC.re - Est_H_p_re;
        q = TempC.im - Est_H_p_im;
        if (q == 0.0) {
          b_Est_H /= 2.0;
        } else if (b_Est_H == 0.0) {
          b_Est_H = 0.0;
        } else {
          b_Est_H /= 2.0;
        }
        centerest_a_aver[19] = TempC.re + b_Est_H;
        if (q == 0.0) {
          b_div = 0.0;
        } else {
          b_div = q / 2.0;
        }
        centerest_a_aver[39] = TempC.im + b_div;
        b_Est_H = Est_C_re / 10.0;
        b_div = b_Est_H + (b_Est_H - Px_re_tmp / 10.0) / 2.0;
        if (rtIsNaN(b_div) || rtIsInf(b_div)) {
          b_Est_H = rtNaN;
        } else if (b_div == 0.0) {
          b_Est_H = 0.0;
        } else {
          bool rEQ0;
          b_Est_H = fmod(b_div, 6.2831853071795862);
          rEQ0 = (b_Est_H == 0.0);
          if (!rEQ0) {
            q = fabs(b_div / 6.2831853071795862);
            rEQ0 = !(fabs(q - floor(q + 0.5)) > 2.2204460492503131E-16 * q);
          }
          if (rEQ0) {
            b_Est_H = 0.0;
          } else if (b_div < 0.0) {
            b_Est_H += 6.2831853071795862;
          }
        }
        headingest_a_aver[19] = b_Est_H;
      } else {
        centerest_a_aver[19] = TempC_tmp.re;
        centerest_a_aver[39] = TempC_tmp.im;
        headingest_a_aver[19] = heading_est_a[19];
      }
      /*      tag_pos_est_aver =
       * get_tag_pos(centerest_a_aver(end,1)+j*centerest_a_aver(end,2),
       * headingest_a_aver(end), tag_pos_b); */
      if (headingest_a_aver[19] * 0.0 == 0.0) {
        TempC.re = cos(headingest_a_aver[19]);
        TempC.im = sin(headingest_a_aver[19]);
      } else if (headingest_a_aver[19] == 0.0) {
        TempC.re = rtNaN;
        TempC.im = 0.0;
      } else {
        TempC.re = rtNaN;
        TempC.im = rtNaN;
      }
      Est_H_p_re = centerest_a_aver[39] * 0.0;
      i = tag_pos_est_aver->size[0] * tag_pos_est_aver->size[1];
      tag_pos_est_aver->size[0] = 1;
      tag_pos_est_aver->size[1] = 4;
      emxEnsureCapacity_creal_T(tag_pos_est_aver, i);
      tag_pos_est_aver_data = tag_pos_est_aver->data;
      tag_pos_est_aver_data[0].re =
          ((tag_pos_b[0].re * TempC.re - tag_pos_b[0].im * TempC.im) +
           centerest_a_aver[19]) +
          Est_H_p_re;
      tag_pos_est_aver_data[0].im =
          (tag_pos_b[0].re * TempC.im + tag_pos_b[0].im * TempC.re) +
          centerest_a_aver[39];
      tag_pos_est_aver_data[1].re =
          ((tag_pos_b[1].re * TempC.re - tag_pos_b[1].im * TempC.im) +
           centerest_a_aver[19]) +
          Est_H_p_re;
      tag_pos_est_aver_data[1].im =
          (tag_pos_b[1].re * TempC.im + tag_pos_b[1].im * TempC.re) +
          centerest_a_aver[39];
      tag_pos_est_aver_data[2].re =
          ((tag_pos_b[2].re * TempC.re - tag_pos_b[2].im * TempC.im) +
           centerest_a_aver[19]) +
          Est_H_p_re;
      tag_pos_est_aver_data[2].im =
          (tag_pos_b[2].re * TempC.im + tag_pos_b[2].im * TempC.re) +
          centerest_a_aver[39];
      tag_pos_est_aver_data[3].re =
          ((tag_pos_b[3].re * TempC.re - tag_pos_b[3].im * TempC.im) +
           centerest_a_aver[19]) +
          Est_H_p_re;
      tag_pos_est_aver_data[3].im =
          (tag_pos_b[3].re * TempC.im + tag_pos_b[3].im * TempC.re) +
          centerest_a_aver[39];
      *headingest_a_aver_v = headingest_a_aver[19];
      /*      K_heading_est = mod(headingest_a_aver(r),2*pi); */
      /*      K_centerest_a_aver(r, :) = centerest_a_aver(r,:); */
      /*      K_headingest_a_aver(r) = mod(headingest_a_aver(r),2*pi); */
    }
    emxFree_real_T(&y);
    emxFree_real_T(&b_ya);
    emxFree_real_T(&b_xa);
  }
  *heading_est = Est_H;
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void UWBpos_free(void)
{
  emxFree_creal_T(&InterpPosition);
  emxFree_creal_T(&Tag_Pos_List);
  emxFree_real_T(&TagDistInitCount);
  emxFree_real_T(&TagDistInit);
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void UWBpos_init(void)
{
  emxInit_real_T(&TagDistInit, 2);
  emxInit_real_T(&TagDistInitCount, 2);
  emxInit_creal_T(&Tag_Pos_List, 3);
  emxInit_creal_T(&InterpPosition, 2);
  r = 0.0;
  memset(&heading_est_a[0], 0, 20U * sizeof(double));
  memset(&centerest_a[0], 0, 40U * sizeof(double));
  memset(&centerest_a_aver[0], 0, 40U * sizeof(double));
  memset(&headingest_a_aver[0], 0, 20U * sizeof(double));
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void r_not_empty_init(void)
{
  r_not_empty = false;
}

/*
 * File trailer for UWBpos.c
 *
 * [EOF]
 */
