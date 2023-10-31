/*
 * UWBpos.c
 *
 * Code generation for function 'UWBpos'
 *
 */

/* Include files */
#include "UWBpos.h"
#include "GetInitPos.h"
#include "GetPos3.h"
#include "GetPosRefine2.h"
#include "UWBpos_data.h"
#include "UWBpos_emxutil.h"
#include "UWBpos_initialize.h"
#include "UWBpos_rtwutil.h"
#include "UWBpos_types.h"
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

static boolean_T r_not_empty;

static emxArray_creal_T *InterpPosition;

static emxArray_creal_T *Tag_Pos_List;

static emxArray_real_T *TagDistInitCount;

static emxArray_real_T *TagDistInit;

static double heading_est_a[20];

static double centerest_a[40];

static double centerest_a_aver[40];

static double headingest_a_aver[20];

static emxArray_real_T *RxIDprev;

static double PPprev[4];

static double RxIDprevLen[4];

/* Function Definitions */
void UWBpos(double Ln, double Lp, double TagNum, double Nanchor,
            const double RxID_data[], const int RxID_size[2],
            const double RxDist_data[], const int RxDist_size[1], double s_time,
            const creal_T tag_pos_b[4], const double xa[6], const double ya[6],
            emxArray_creal_T *tag_pos_est, double *heading_est,
            emxArray_creal_T *tag_pos_est_aver, double *headingest_a_aver_v)
{
  emxArray_boolean_T *x;
  emxArray_creal_T *InterpPosT;
  emxArray_int32_T *i;
  emxArray_real_T *DistT;
  emxArray_real_T *IndT;
  emxArray_real_T *b_DistT;
  emxArray_real_T *b_r;
  emxArray_real_T *b_xa;
  emxArray_real_T *b_ya;
  creal_T Pm[36];
  creal_T dcv1[36];
  creal_T Px[12];
  creal_T b_x[10];
  creal_T b_Pm[9];
  creal_T dcv[9];
  creal_T b_tag_pos_est[4];
  creal_T dcv2[3];
  creal_T bsum;
  creal_T bsum_tmp;
  creal_T *InterpPosition_data;
  creal_T *Tag_Pos_List_data;
  creal_T *tag_pos_est_aver_data;
  creal_T *tag_pos_est_data;
  double Py[12];
  double Est_H;
  double MeanA_im;
  double MeanA_re;
  double Pm_im;
  double Pm_re;
  double TempC_im;
  double TempC_re;
  double c_r;
  double im;
  double re;
  double *DistT_data;
  double *RxIDprev_data;
  double *TagDistInitCount_data;
  double *TagDistInit_data;
  int b_tmp_data[12];
  int tmp_data[4];
  int firstBlockLength;
  int hi;
  int ib;
  int idx;
  int k;
  int lastBlockLength;
  int nblocks;
  int *i_data;
  boolean_T b_Px[12];
  boolean_T exitg1;
  boolean_T rEQ0;
  boolean_T *x_data;
  if (!isInitialized_UWBpos) {
    UWBpos_initialize();
  }
  RxIDprev_data = RxIDprev->data;
  TagDistInit_data = TagDistInit->data;
  TagDistInitCount_data = TagDistInitCount->data;
  Tag_Pos_List_data = Tag_Pos_List->data;
  InterpPosition_data = InterpPosition->data;
  if (!r_not_empty) {
    r_not_empty = true;
    ib = InterpPosition->size[0] * InterpPosition->size[1];
    InterpPosition->size[0] = (int)Lp;
    InterpPosition->size[1] = 2;
    emxEnsureCapacity_creal_T(InterpPosition, ib);
    InterpPosition_data = InterpPosition->data;
    nblocks = (int)Lp << 1;
    for (ib = 0; ib < nblocks; ib++) {
      InterpPosition_data[ib].re = 0.0;
      InterpPosition_data[ib].im = 0.0;
    }
    ib = Tag_Pos_List->size[0] * Tag_Pos_List->size[1] * Tag_Pos_List->size[2];
    Tag_Pos_List->size[0] = 12;
    Tag_Pos_List->size[1] = 2;
    Tag_Pos_List->size[2] = (int)Lp;
    emxEnsureCapacity_creal_T(Tag_Pos_List, ib);
    Tag_Pos_List_data = Tag_Pos_List->data;
    nblocks = 24 * (int)Lp;
    for (ib = 0; ib < nblocks; ib++) {
      Tag_Pos_List_data[ib].re = 0.0;
      Tag_Pos_List_data[ib].im = 0.0;
    }
    ib = TagDistInitCount->size[0] * TagDistInitCount->size[1];
    TagDistInitCount->size[0] = (int)Ln;
    TagDistInitCount->size[1] = (int)Lp;
    emxEnsureCapacity_real_T(TagDistInitCount, ib);
    TagDistInitCount_data = TagDistInitCount->data;
    firstBlockLength = (int)Ln * (int)Lp;
    for (ib = 0; ib < firstBlockLength; ib++) {
      TagDistInitCount_data[ib] = 0.0;
    }
    ib = TagDistInit->size[0] * TagDistInit->size[1];
    TagDistInit->size[0] = (int)Ln;
    TagDistInit->size[1] = (int)Lp;
    emxEnsureCapacity_real_T(TagDistInit, ib);
    TagDistInit_data = TagDistInit->data;
    for (ib = 0; ib < firstBlockLength; ib++) {
      TagDistInit_data[ib] = 0.0;
    }
    ib = RxIDprev->size[0] * RxIDprev->size[1];
    RxIDprev->size[0] = 4;
    RxIDprev->size[1] = (int)Ln;
    emxEnsureCapacity_real_T(RxIDprev, ib);
    RxIDprev_data = RxIDprev->data;
    nblocks = (int)Ln << 2;
    for (ib = 0; ib < nblocks; ib++) {
      RxIDprev_data[ib] = 0.0;
    }
  }
  r++;
  ib = tag_pos_est->size[0] * tag_pos_est->size[1];
  tag_pos_est->size[0] = 1;
  nblocks = (int)Lp;
  tag_pos_est->size[1] = (int)Lp;
  emxEnsureCapacity_creal_T(tag_pos_est, ib);
  tag_pos_est_data = tag_pos_est->data;
  for (ib = 0; ib < nblocks; ib++) {
    tag_pos_est_data[ib].re = 0.0;
    tag_pos_est_data[ib].im = 0.0;
  }
  *heading_est = 0.0;
  ib = tag_pos_est_aver->size[0] * tag_pos_est_aver->size[1];
  tag_pos_est_aver->size[0] = 1;
  tag_pos_est_aver->size[1] = (int)Lp;
  emxEnsureCapacity_creal_T(tag_pos_est_aver, ib);
  tag_pos_est_aver_data = tag_pos_est_aver->data;
  for (ib = 0; ib < nblocks; ib++) {
    tag_pos_est_aver_data[ib].re = 0.0;
    tag_pos_est_aver_data[ib].im = 0.0;
  }
  *headingest_a_aver_v = 0.0;
  if (r < 10.0 * Lp) {
    ib = (int)Nanchor;
    for (firstBlockLength = 0; firstBlockLength < ib; firstBlockLength++) {
      /*              for PP = mod(r-1,Lp)+1:mod(r-1,Lp)+1 */
      TempC_im = RxID_data[firstBlockLength];
      TagDistInitCount_data[((int)TempC_im +
                             TagDistInitCount->size[0] * ((int)TagNum - 1)) -
                            1]++;
      TempC_re = TagDistInitCount_data
          [((int)TempC_im + TagDistInitCount->size[0] * ((int)TagNum - 1)) - 1];
      TagDistInit_data[((int)TempC_im +
                        TagDistInit->size[0] * ((int)TagNum - 1)) -
                       1] =
          TagDistInit_data[((int)TempC_im +
                            TagDistInit->size[0] * ((int)TagNum - 1)) -
                           1] *
              (TempC_re - 1.0) / TempC_re +
          RxDist_data[firstBlockLength] / TempC_re;
      /*              end */
    }
  } else if (r == 10.0 * Lp) {
    ib = (int)Nanchor;
    for (firstBlockLength = 0; firstBlockLength < ib; firstBlockLength++) {
      /*              for PP = mod(r-1,Lp)+1:mod(r-1,Lp)+1 */
      TempC_im = RxID_data[firstBlockLength];
      TagDistInitCount_data[((int)TempC_im +
                             TagDistInitCount->size[0] * ((int)TagNum - 1)) -
                            1]++;
      TempC_re = TagDistInitCount_data
          [((int)TempC_im + TagDistInitCount->size[0] * ((int)TagNum - 1)) - 1];
      TagDistInit_data[((int)TempC_im +
                        TagDistInit->size[0] * ((int)TagNum - 1)) -
                       1] =
          TagDistInit_data[((int)TempC_im +
                            TagDistInit->size[0] * ((int)TagNum - 1)) -
                           1] *
              (TempC_re - 1.0) / TempC_re +
          RxDist_data[firstBlockLength] / TempC_re;
      /*              end */
    }
    emxInit_real_T(&DistT, 2);
    ib = DistT->size[0] * DistT->size[1];
    DistT->size[0] = TagDistInit->size[0];
    DistT->size[1] = TagDistInit->size[1];
    emxEnsureCapacity_real_T(DistT, ib);
    DistT_data = DistT->data;
    firstBlockLength = TagDistInit->size[0] * TagDistInit->size[1];
    for (ib = 0; ib < firstBlockLength; ib++) {
      DistT_data[ib] = TagDistInit_data[ib];
    }
    hi = TagDistInit->size[0] * TagDistInit->size[1] - 1;
    firstBlockLength = 0;
    for (idx = 0; idx <= hi; idx++) {
      if (TagDistInit_data[idx] == 0.0) {
        firstBlockLength++;
      }
    }
    emxInit_int32_T(&i, 1);
    ib = i->size[0];
    i->size[0] = firstBlockLength;
    emxEnsureCapacity_int32_T(i, ib);
    i_data = i->data;
    firstBlockLength = 0;
    for (idx = 0; idx <= hi; idx++) {
      if (TagDistInit_data[idx] == 0.0) {
        i_data[firstBlockLength] = idx + 1;
        firstBlockLength++;
      }
    }
    firstBlockLength = i->size[0];
    for (ib = 0; ib < firstBlockLength; ib++) {
      DistT_data[i_data[ib] - 1] = 1.0E+6;
    }
    emxInit_real_T(&IndT, 1);
    sum(DistT, IndT);
    TagDistInit_data = IndT->data;
    emxInit_boolean_T(&x, 1);
    ib = x->size[0];
    x->size[0] = IndT->size[0];
    emxEnsureCapacity_boolean_T(x, ib);
    x_data = x->data;
    firstBlockLength = IndT->size[0];
    for (ib = 0; ib < firstBlockLength; ib++) {
      x_data[ib] = (TagDistInit_data[ib] < 100000.0);
    }
    hi = x->size[0];
    idx = 0;
    ib = i->size[0];
    i->size[0] = x->size[0];
    emxEnsureCapacity_int32_T(i, ib);
    i_data = i->data;
    firstBlockLength = 0;
    exitg1 = false;
    while ((!exitg1) && (firstBlockLength <= hi - 1)) {
      if (x_data[firstBlockLength]) {
        idx++;
        i_data[idx - 1] = firstBlockLength + 1;
        if (idx >= hi) {
          exitg1 = true;
        } else {
          firstBlockLength++;
        }
      } else {
        firstBlockLength++;
      }
    }
    if (x->size[0] == 1) {
      if (idx == 0) {
        i->size[0] = 0;
      }
    } else {
      ib = i->size[0];
      if (idx < 1) {
        i->size[0] = 0;
      } else {
        i->size[0] = idx;
      }
      emxEnsureCapacity_int32_T(i, ib);
      i_data = i->data;
    }
    emxFree_boolean_T(&x);
    ib = IndT->size[0];
    IndT->size[0] = i->size[0];
    emxEnsureCapacity_real_T(IndT, ib);
    TagDistInit_data = IndT->data;
    firstBlockLength = i->size[0];
    for (ib = 0; ib < firstBlockLength; ib++) {
      TagDistInit_data[ib] = i_data[ib];
    }
    emxFree_int32_T(&i);
    hi = DistT->size[1];
    emxInit_real_T(&b_DistT, 2);
    ib = b_DistT->size[0] * b_DistT->size[1];
    b_DistT->size[0] = IndT->size[0];
    b_DistT->size[1] = DistT->size[1];
    emxEnsureCapacity_real_T(b_DistT, ib);
    TagDistInitCount_data = b_DistT->data;
    for (ib = 0; ib < hi; ib++) {
      firstBlockLength = IndT->size[0];
      for (lastBlockLength = 0; lastBlockLength < firstBlockLength;
           lastBlockLength++) {
        TagDistInitCount_data[lastBlockLength + b_DistT->size[0] * ib] =
            DistT_data[((int)TagDistInit_data[lastBlockLength] +
                        DistT->size[0] * ib) -
                       1];
      }
    }
    ib = DistT->size[0] * DistT->size[1];
    DistT->size[0] = b_DistT->size[0];
    DistT->size[1] = b_DistT->size[1];
    emxEnsureCapacity_real_T(DistT, ib);
    DistT_data = DistT->data;
    firstBlockLength = b_DistT->size[0] * b_DistT->size[1];
    for (ib = 0; ib < firstBlockLength; ib++) {
      DistT_data[ib] = TagDistInitCount_data[ib];
    }
    emxFree_real_T(&b_DistT);
    emxInit_real_T(&b_xa, 2);
    ib = b_xa->size[0] * b_xa->size[1];
    b_xa->size[0] = 1;
    b_xa->size[1] = IndT->size[0];
    emxEnsureCapacity_real_T(b_xa, ib);
    TagDistInitCount_data = b_xa->data;
    firstBlockLength = IndT->size[0];
    for (ib = 0; ib < firstBlockLength; ib++) {
      TagDistInitCount_data[ib] = xa[(int)TagDistInit_data[ib] - 1];
    }
    emxInit_real_T(&b_ya, 2);
    ib = b_ya->size[0] * b_ya->size[1];
    b_ya->size[0] = 1;
    b_ya->size[1] = IndT->size[0];
    emxEnsureCapacity_real_T(b_ya, ib);
    TagDistInitCount_data = b_ya->data;
    firstBlockLength = IndT->size[0];
    for (ib = 0; ib < firstBlockLength; ib++) {
      TagDistInitCount_data[ib] = ya[(int)TagDistInit_data[ib] - 1];
    }
    GetInitPos(b_xa, b_ya, DistT, tag_pos_b, IndT->size[0], Lp, b_tag_pos_est,
               heading_est);
    emxFree_real_T(&b_ya);
    emxFree_real_T(&b_xa);
    emxFree_real_T(&IndT);
    emxFree_real_T(&DistT);
    ib = tag_pos_est->size[0] * tag_pos_est->size[1];
    tag_pos_est->size[0] = 1;
    tag_pos_est->size[1] = 4;
    emxEnsureCapacity_creal_T(tag_pos_est, ib);
    tag_pos_est_data = tag_pos_est->data;
    tag_pos_est_data[0] = b_tag_pos_est[0];
    tag_pos_est_data[1] = b_tag_pos_est[1];
    tag_pos_est_data[2] = b_tag_pos_est[2];
    tag_pos_est_data[3] = b_tag_pos_est[3];
    for (firstBlockLength = 0; firstBlockLength < nblocks; firstBlockLength++) {
      for (idx = 0; idx < 12; idx++) {
        ib = idx + 24 * firstBlockLength;
        Tag_Pos_List_data[ib].re = s_time + (((double)idx + 1.0) - 12.0) * 0.1;
        Tag_Pos_List_data[ib].im = 0.0;
        Tag_Pos_List_data[ib + 12] = b_tag_pos_est[firstBlockLength];
      }
    }
  } else {
    /* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
    /* %%%%%%%%%%%%%%%%%  Next Position Prediction    %%%%%%%%%%%%%%%%%% */
    for (hi = 0; hi < nblocks; hi++) {
      for (ib = 0; ib < 12; ib++) {
        firstBlockLength = ib + 24 * hi;
        Py[ib] = Tag_Pos_List_data[firstBlockLength + 12].re;
        re = Tag_Pos_List_data[firstBlockLength].re - s_time;
        im = Tag_Pos_List_data[firstBlockLength].im;
        Px[ib].re = re;
        Px[ib].im = im;
        b_Px[ib] = ((re == 0.0) && (im == 0.0));
      }
      eml_find(b_Px, b_tmp_data, &firstBlockLength);
      if (firstBlockLength == 0) {
        for (k = 0; k < 12; k++) {
          TempC_im = Px[k].re;
          TempC_re = Px[k].im;
          Pm[k].re = TempC_im * TempC_im - TempC_re * TempC_re;
          TempC_im *= TempC_re;
          Pm[k].im = TempC_im + TempC_im;
          Pm[k + 12] = Px[k];
          Pm[k + 24].re = 1.0;
          Pm[k + 24].im = 0.0;
        }
        for (ib = 0; ib < 3; ib++) {
          for (lastBlockLength = 0; lastBlockLength < 3; lastBlockLength++) {
            re = 0.0;
            im = 0.0;
            for (idx = 0; idx < 12; idx++) {
              firstBlockLength = idx + 12 * ib;
              Pm_re = Pm[firstBlockLength].re;
              Pm_im = -Pm[firstBlockLength].im;
              firstBlockLength = idx + 12 * lastBlockLength;
              TempC_re = Pm[firstBlockLength].im;
              TempC_im = Pm[firstBlockLength].re;
              re += Pm_re * TempC_im - Pm_im * TempC_re;
              im += Pm_re * TempC_re + Pm_im * TempC_im;
            }
            idx = ib + 3 * lastBlockLength;
            b_Pm[idx].re = re;
            b_Pm[idx].im = im;
          }
        }
        b_inv(b_Pm, dcv);
        for (ib = 0; ib < 3; ib++) {
          TempC_re = dcv[ib].re;
          TempC_im = dcv[ib].im;
          c_r = dcv[ib + 3].re;
          Est_H = dcv[ib + 3].im;
          MeanA_re = dcv[ib + 6].re;
          MeanA_im = dcv[ib + 6].im;
          for (lastBlockLength = 0; lastBlockLength < 12; lastBlockLength++) {
            Pm_re = Pm[lastBlockLength].re;
            Pm_im = -Pm[lastBlockLength].im;
            re = TempC_re * Pm_re - TempC_im * Pm_im;
            im = TempC_re * Pm_im + TempC_im * Pm_re;
            Pm_re = Pm[lastBlockLength + 12].re;
            Pm_im = -Pm[lastBlockLength + 12].im;
            re += c_r * Pm_re - Est_H * Pm_im;
            im += c_r * Pm_im + Est_H * Pm_re;
            Pm_re = Pm[lastBlockLength + 24].re;
            Pm_im = -Pm[lastBlockLength + 24].im;
            re += MeanA_re * Pm_re - MeanA_im * Pm_im;
            im += MeanA_re * Pm_im + MeanA_im * Pm_re;
            idx = ib + 3 * lastBlockLength;
            dcv1[idx].re = re;
            dcv1[idx].im = im;
          }
        }
        for (ib = 0; ib < 12; ib++) {
          Px[ib].re = Py[ib];
          Px[ib].im = 0.0;
        }
        for (ib = 0; ib < 3; ib++) {
          re = 0.0;
          im = 0.0;
          for (lastBlockLength = 0; lastBlockLength < 12; lastBlockLength++) {
            firstBlockLength = ib + 3 * lastBlockLength;
            TempC_re = dcv1[firstBlockLength].re;
            TempC_im = Px[lastBlockLength].im;
            c_r = dcv1[firstBlockLength].im;
            Est_H = Px[lastBlockLength].re;
            re += TempC_re * Est_H - c_r * TempC_im;
            im += TempC_re * TempC_im + c_r * Est_H;
          }
          dcv2[ib].re = re;
          dcv2[ib].im = im;
        }
        InterpPosition_data[hi] = dcv2[2];
      } else {
        InterpPosition_data[hi].re = Py[2];
        InterpPosition_data[hi].im = 0.0;
      }
      /*  Py = pm x A */
      /*  tr(pm)xPy = tr(Pm)pm x A */
      /*  inv(tr(Pm)Pm) x tr(pm)xPy = A */
      for (ib = 0; ib < 12; ib++) {
        firstBlockLength = ib + 24 * hi;
        Py[ib] = Tag_Pos_List_data[firstBlockLength + 12].im;
        re = Tag_Pos_List_data[firstBlockLength].re - s_time;
        im = Tag_Pos_List_data[firstBlockLength].im;
        Px[ib].re = re;
        Px[ib].im = im;
        b_Px[ib] = ((re == 0.0) && (im == 0.0));
      }
      eml_find(b_Px, b_tmp_data, &firstBlockLength);
      if (firstBlockLength == 0) {
        for (k = 0; k < 12; k++) {
          TempC_im = Px[k].re;
          TempC_re = Px[k].im;
          Pm[k].re = TempC_im * TempC_im - TempC_re * TempC_re;
          TempC_im *= TempC_re;
          Pm[k].im = TempC_im + TempC_im;
          Pm[k + 12] = Px[k];
          Pm[k + 24].re = 1.0;
          Pm[k + 24].im = 0.0;
        }
        for (ib = 0; ib < 3; ib++) {
          for (lastBlockLength = 0; lastBlockLength < 3; lastBlockLength++) {
            re = 0.0;
            im = 0.0;
            for (idx = 0; idx < 12; idx++) {
              firstBlockLength = idx + 12 * ib;
              Pm_re = Pm[firstBlockLength].re;
              Pm_im = -Pm[firstBlockLength].im;
              firstBlockLength = idx + 12 * lastBlockLength;
              TempC_re = Pm[firstBlockLength].im;
              TempC_im = Pm[firstBlockLength].re;
              re += Pm_re * TempC_im - Pm_im * TempC_re;
              im += Pm_re * TempC_re + Pm_im * TempC_im;
            }
            idx = ib + 3 * lastBlockLength;
            b_Pm[idx].re = re;
            b_Pm[idx].im = im;
          }
        }
        b_inv(b_Pm, dcv);
        for (ib = 0; ib < 3; ib++) {
          TempC_re = dcv[ib].re;
          TempC_im = dcv[ib].im;
          c_r = dcv[ib + 3].re;
          Est_H = dcv[ib + 3].im;
          MeanA_re = dcv[ib + 6].re;
          MeanA_im = dcv[ib + 6].im;
          for (lastBlockLength = 0; lastBlockLength < 12; lastBlockLength++) {
            Pm_re = Pm[lastBlockLength].re;
            Pm_im = -Pm[lastBlockLength].im;
            re = TempC_re * Pm_re - TempC_im * Pm_im;
            im = TempC_re * Pm_im + TempC_im * Pm_re;
            Pm_re = Pm[lastBlockLength + 12].re;
            Pm_im = -Pm[lastBlockLength + 12].im;
            re += c_r * Pm_re - Est_H * Pm_im;
            im += c_r * Pm_im + Est_H * Pm_re;
            Pm_re = Pm[lastBlockLength + 24].re;
            Pm_im = -Pm[lastBlockLength + 24].im;
            re += MeanA_re * Pm_re - MeanA_im * Pm_im;
            im += MeanA_re * Pm_im + MeanA_im * Pm_re;
            idx = ib + 3 * lastBlockLength;
            dcv1[idx].re = re;
            dcv1[idx].im = im;
          }
        }
        for (ib = 0; ib < 12; ib++) {
          Px[ib].re = Py[ib];
          Px[ib].im = 0.0;
        }
        for (ib = 0; ib < 3; ib++) {
          re = 0.0;
          im = 0.0;
          for (lastBlockLength = 0; lastBlockLength < 12; lastBlockLength++) {
            firstBlockLength = ib + 3 * lastBlockLength;
            TempC_re = dcv1[firstBlockLength].re;
            TempC_im = Px[lastBlockLength].im;
            c_r = dcv1[firstBlockLength].im;
            Est_H = Px[lastBlockLength].re;
            re += TempC_re * Est_H - c_r * TempC_im;
            im += TempC_re * TempC_im + c_r * Est_H;
          }
          dcv2[ib].re = re;
          dcv2[ib].im = im;
        }
        InterpPosition_data[hi + InterpPosition->size[0]] = dcv2[2];
      } else {
        InterpPosition_data[hi + InterpPosition->size[0]].re = Py[2];
        InterpPosition_data[hi + InterpPosition->size[0]].im = 0.0;
      }
      /*  Py = pm x A */
      /*  tr(pm)xPy = tr(Pm)pm x A */
      /*  inv(tr(Pm)Pm) x tr(pm)xPy = A */
    }
    /* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
    nblocks = RxID_size[1];
    for (ib = 0; ib < nblocks; ib++) {
      tmp_data[ib] = (int)RxID_data[ib] - 1;
    }
    nblocks = RxDist_size[0];
    for (ib = 0; ib < nblocks; ib++) {
      TagDistInit_data[tmp_data[ib] + TagDistInit->size[0] *
                                          ((int)TagNum - 1)] = RxDist_data[ib];
    }
    /*      NP = size(TagDistInit,1); */
    /*      AnchIDList = [1:NP]; */
    /*      for kk = 1 : 4 */
    /*          if kk == 1 */
    /*              A = RxIDprev{kk}; */
    /*          else */
    /*              for ll = 1 : length(RxIDprev{kk}) */
    /*                  if length(find(RxIDprev{kk}(ll)==A))==0 */
    /*                      A = [A RxIDprev{kk}(ll)]; */
    /*                  end */
    /*              end */
    /*          end */
    /*      end */
    /* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
    /* %%%%%%%%%%%%%%%% New Position Calc.   %%%%%%%%%%%%%%%%%%%%%%%%%%% */
    /*          for PP = mod(r-1,Lp)+1:mod(r-1,Lp)+1 */
    emxInit_creal_T(&InterpPosT, 1);
    if (RxID_size[1] > 1) {
      /*          [tag_pos_est, heading_est, CandPos] =
       * GetPos2(xa,ya,RxDist,RxID,tag_pos_b,Nanchor,PP,InterpPosition(:,1)+j*InterpPosition(:,2));
       */
      /*          [tag_pos_est, heading_est, CandPos] =
       * GetPos3(xa,ya,RxDist,RxID,tag_pos_b,Nanchor,PP,InterpPosition(:,1)+j*InterpPosition(:,2),TagDistInitPrev,RxIDprev);
       */
      ib = InterpPosT->size[0];
      InterpPosT->size[0] = InterpPosition->size[0];
      emxEnsureCapacity_creal_T(InterpPosT, ib);
      tag_pos_est_aver_data = InterpPosT->data;
      nblocks = InterpPosition->size[0];
      for (ib = 0; ib < nblocks; ib++) {
        TempC_re = InterpPosition_data[ib + InterpPosition->size[0]].im;
        TempC_im = InterpPosition_data[ib + InterpPosition->size[0]].re;
        tag_pos_est_aver_data[ib].re =
            InterpPosition_data[ib].re + (0.0 * TempC_im - TempC_re);
        tag_pos_est_aver_data[ib].im =
            InterpPosition_data[ib].im + (0.0 * TempC_re + TempC_im);
      }
      GetPos3(xa, ya, RxDist_data, RxDist_size[0], RxID_data, RxID_size,
              tag_pos_b, Nanchor, TagNum, InterpPosT, TagDistInit, RxIDprev,
              RxIDprevLen, PPprev, b_tag_pos_est, heading_est, &bsum);
      /*   */
      GetPosRefine2(xa, ya, RxDist_data, RxID_data, RxID_size, tag_pos_b,
                    Nanchor, TagNum, b_tag_pos_est, heading_est, &bsum);
      ib = tag_pos_est->size[0] * tag_pos_est->size[1];
      tag_pos_est->size[0] = 1;
      tag_pos_est->size[1] = 4;
      emxEnsureCapacity_creal_T(tag_pos_est, ib);
      tag_pos_est_data = tag_pos_est->data;
      tag_pos_est_data[0] = b_tag_pos_est[0];
      tag_pos_est_data[1] = b_tag_pos_est[1];
      tag_pos_est_data[2] = b_tag_pos_est[2];
      tag_pos_est_data[3] = b_tag_pos_est[3];
      /*          [tag_pos_est, heading_est] =
       * GetInitPos(xa(A),ya(A),TagDistInit,anch_pos(A),tag_pos_b,length(A),Lp);
       */
      /*          CandPos = tag_pos_est(PP); */
    } else {
      ib = InterpPosT->size[0];
      InterpPosT->size[0] = InterpPosition->size[0];
      emxEnsureCapacity_creal_T(InterpPosT, ib);
      tag_pos_est_aver_data = InterpPosT->data;
      nblocks = InterpPosition->size[0];
      for (ib = 0; ib < nblocks; ib++) {
        TempC_re = InterpPosition_data[ib + InterpPosition->size[0]].im;
        TempC_im = InterpPosition_data[ib + InterpPosition->size[0]].re;
        tag_pos_est_aver_data[ib].re =
            InterpPosition_data[ib].re + (0.0 * TempC_im - TempC_re);
        tag_pos_est_aver_data[ib].im =
            InterpPosition_data[ib].im + (0.0 * TempC_re + TempC_im);
      }
      if (InterpPosT->size[0] == 0) {
        TempC_re = 0.0;
        TempC_im = 0.0;
      } else {
        if (InterpPosT->size[0] <= 1024) {
          firstBlockLength = InterpPosT->size[0];
          lastBlockLength = 0;
          nblocks = 1;
        } else {
          firstBlockLength = 1024;
          nblocks = (int)((unsigned int)InterpPosT->size[0] >> 10);
          lastBlockLength = InterpPosT->size[0] - (nblocks << 10);
          if (lastBlockLength > 0) {
            nblocks++;
          } else {
            lastBlockLength = 1024;
          }
        }
        TempC_re = tag_pos_est_aver_data[0].re;
        TempC_im = tag_pos_est_aver_data[0].im;
        for (k = 2; k <= firstBlockLength; k++) {
          TempC_re += tag_pos_est_aver_data[k - 1].re;
          TempC_im += tag_pos_est_aver_data[k - 1].im;
        }
        for (ib = 2; ib <= nblocks; ib++) {
          firstBlockLength = (ib - 1) << 10;
          bsum = tag_pos_est_aver_data[firstBlockLength];
          if (ib == nblocks) {
            hi = lastBlockLength;
          } else {
            hi = 1024;
          }
          for (k = 2; k <= hi; k++) {
            idx = (firstBlockLength + k) - 1;
            bsum.re += tag_pos_est_aver_data[idx].re;
            bsum.im += tag_pos_est_aver_data[idx].im;
          }
          TempC_re += bsum.re;
          TempC_im += bsum.im;
        }
      }
      if (TempC_im == 0.0) {
        re = TempC_re / (double)InterpPosT->size[0];
        im = 0.0;
      } else if (TempC_re == 0.0) {
        re = 0.0;
        im = TempC_im / (double)InterpPosT->size[0];
      } else {
        re = TempC_re / (double)InterpPosT->size[0];
        im = TempC_im / (double)InterpPosT->size[0];
      }
      bsum.re = 0.0;
      bsum.im = 0.0;
      ib = InterpPosT->size[0];
      for (firstBlockLength = 0; firstBlockLength < ib; firstBlockLength++) {
        Pm_re = tag_pos_est_aver_data[firstBlockLength].re - re;
        Pm_im = tag_pos_est_aver_data[firstBlockLength].im - im;
        c_r = tag_pos_b[firstBlockLength].re;
        Est_H = tag_pos_b[firstBlockLength].im;
        if (Est_H == 0.0) {
          if (Pm_im == 0.0) {
            MeanA_im = Pm_re / c_r;
            TempC_re = 0.0;
          } else if (Pm_re == 0.0) {
            MeanA_im = 0.0;
            TempC_re = Pm_im / c_r;
          } else {
            MeanA_im = Pm_re / c_r;
            TempC_re = Pm_im / c_r;
          }
        } else if (c_r == 0.0) {
          if (Pm_re == 0.0) {
            MeanA_im = Pm_im / Est_H;
            TempC_re = 0.0;
          } else if (Pm_im == 0.0) {
            MeanA_im = 0.0;
            TempC_re = -(Pm_re / Est_H);
          } else {
            MeanA_im = Pm_im / Est_H;
            TempC_re = -(Pm_re / Est_H);
          }
        } else {
          MeanA_re = fabs(c_r);
          TempC_re = fabs(Est_H);
          if (MeanA_re > TempC_re) {
            TempC_im = Est_H / c_r;
            TempC_re = c_r + TempC_im * Est_H;
            MeanA_im = (Pm_re + TempC_im * Pm_im) / TempC_re;
            TempC_re = (Pm_im - TempC_im * Pm_re) / TempC_re;
          } else if (TempC_re == MeanA_re) {
            if (c_r > 0.0) {
              TempC_im = 0.5;
            } else {
              TempC_im = -0.5;
            }
            if (Est_H > 0.0) {
              TempC_re = 0.5;
            } else {
              TempC_re = -0.5;
            }
            MeanA_im = (Pm_re * TempC_im + Pm_im * TempC_re) / MeanA_re;
            TempC_re = (Pm_im * TempC_im - Pm_re * TempC_re) / MeanA_re;
          } else {
            TempC_im = c_r / Est_H;
            TempC_re = Est_H + TempC_im * c_r;
            MeanA_im = (TempC_im * Pm_re + Pm_im) / TempC_re;
            TempC_re = (TempC_im * Pm_im - Pm_re) / TempC_re;
          }
        }
        bsum.re += MeanA_im;
        bsum.im += TempC_re;
      }
      Est_H = rt_atan2d_snf(bsum.im, bsum.re);
      ib = InterpPosT->size[0];
      for (firstBlockLength = 0; firstBlockLength < ib; firstBlockLength++) {
        MeanA_re = Est_H * 0.0;
        if (Est_H == 0.0) {
          MeanA_re = exp(MeanA_re);
          MeanA_im = 0.0;
        } else {
          c_r = exp(MeanA_re / 2.0);
          MeanA_re = c_r * (c_r * cos(Est_H));
          MeanA_im = c_r * (c_r * sin(Est_H));
        }
        TempC_re = tag_pos_b[firstBlockLength].re;
        TempC_im = tag_pos_b[firstBlockLength].im;
        tag_pos_est_data[firstBlockLength].re =
            re + (TempC_re * MeanA_re - TempC_im * MeanA_im);
        tag_pos_est_data[firstBlockLength].im =
            im + (TempC_re * MeanA_im + TempC_im * MeanA_re);
      }
      TempC_re =
          InterpPosition_data[((int)TagNum + InterpPosition->size[0]) - 1].im;
      TempC_im =
          InterpPosition_data[((int)TagNum + InterpPosition->size[0]) - 1].re;
      bsum.re =
          InterpPosition_data[(int)TagNum - 1].re + (0.0 * TempC_im - TempC_re);
      bsum.im =
          InterpPosition_data[(int)TagNum - 1].im + (0.0 * TempC_re + TempC_im);
    }
    emxFree_creal_T(&InterpPosT);
    /* %%%%%%%%%%%%%  Original %%%%%%%%%%%%% */
    for (ib = 0; ib < 2; ib++) {
      for (lastBlockLength = 0; lastBlockLength < 11; lastBlockLength++) {
        idx = (lastBlockLength + 12 * ib) + 24 * ((int)TagNum - 1);
        Tag_Pos_List_data[idx] = Tag_Pos_List_data[idx + 1];
      }
    }
    ib = 24 * ((int)TagNum - 1);
    Tag_Pos_List_data[ib + 11].re = s_time;
    Tag_Pos_List_data[ib + 11].im = 0.0;
    Tag_Pos_List_data[ib + 23] = bsum;
    /* %%%%%%%%%%%%%  Test %%%%%%%%%%%%% */
    /*      Tag_Pos_List(1:NumInterpPoint-1,:,:) =
     * Tag_Pos_List(2:NumInterpPoint,:,:); */
    /*      Tag_Pos_List(NumInterpPoint,1,:) = s_time; */
    /*      Tag_Pos_List(NumInterpPoint,2,:) = tag_pos_est; */
    /*      Tag_Pos_List(NumInterpPoint,:,PP) = [s_time CandPos]; */
    /* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
    /*              Tag_Pos_List(1:2,:,PP) = Tag_Pos_List(2:3,:,PP); */
    /*              Tag_Pos_List(3,:,PP) = [s_time(r) tag_pos_est(PP)]; */
    /*          end */
    bsum_tmp = mean(tag_pos_est);
    for (ib = 0; ib < 19; ib++) {
      heading_est_a[ib] = heading_est_a[ib + 1];
      centerest_a[ib] = centerest_a[ib + 1];
      centerest_a_aver[ib] = centerest_a_aver[ib + 1];
      centerest_a[ib + 20] = centerest_a[ib + 21];
      centerest_a_aver[ib + 20] = centerest_a_aver[ib + 21];
      headingest_a_aver[ib] = headingest_a_aver[ib + 1];
    }
    if (*heading_est - heading_est_a[18] > 3.1415926535897931) {
      heading_est_a[19] = *heading_est - 6.2831853071795862;
    } else if (heading_est_a[18] - *heading_est > 3.1415926535897931) {
      heading_est_a[19] = *heading_est + 6.2831853071795862;
    } else {
      heading_est_a[19] = *heading_est;
    }
    centerest_a[19] = bsum_tmp.re;
    centerest_a[39] = bsum_tmp.im;
    if (r > 20.0) {
      for (ib = 0; ib < 10; ib++) {
        TempC_im = centerest_a[ib + 20];
        b_x[ib].re = centerest_a[ib] + 0.0 * TempC_im;
        b_x[ib].im = TempC_im;
      }
      TempC_re = b_x[0].re;
      TempC_im = b_x[0].im;
      for (k = 0; k < 9; k++) {
        TempC_re += b_x[k + 1].re;
        TempC_im += b_x[k + 1].im;
      }
      if (TempC_im == 0.0) {
        MeanA_re = TempC_re / 10.0;
        MeanA_im = 0.0;
      } else if (TempC_re == 0.0) {
        MeanA_re = 0.0;
        MeanA_im = TempC_im / 10.0;
      } else {
        MeanA_re = TempC_re / 10.0;
        MeanA_im = TempC_im / 10.0;
      }
      for (ib = 0; ib < 10; ib++) {
        TempC_im = centerest_a[ib + 30];
        b_x[ib].re = centerest_a[ib + 10] + 0.0 * TempC_im;
        b_x[ib].im = TempC_im;
      }
      TempC_re = b_x[0].re;
      TempC_im = b_x[0].im;
      Est_H = heading_est_a[0];
      c_r = heading_est_a[10];
      for (k = 0; k < 9; k++) {
        TempC_re += b_x[k + 1].re;
        TempC_im += b_x[k + 1].im;
        Est_H += heading_est_a[k + 1];
        c_r += heading_est_a[k + 11];
      }
      if (TempC_im == 0.0) {
        bsum.re = TempC_re / 10.0;
        bsum.im = 0.0;
      } else if (TempC_re == 0.0) {
        bsum.re = 0.0;
        bsum.im = TempC_im / 10.0;
      } else {
        bsum.re = TempC_re / 10.0;
        bsum.im = TempC_im / 10.0;
      }
      TempC_re = bsum.re - MeanA_re;
      TempC_im = bsum.im - MeanA_im;
      if (TempC_im == 0.0) {
        TempC_re /= 2.0;
      } else if (TempC_re == 0.0) {
        TempC_re = 0.0;
      } else {
        TempC_re /= 2.0;
      }
      centerest_a_aver[19] = bsum.re + TempC_re;
      if (TempC_im == 0.0) {
        TempC_re = 0.0;
      } else {
        TempC_re = TempC_im / 2.0;
      }
      centerest_a_aver[39] = bsum.im + TempC_re;
      TempC_re = c_r / 10.0;
      TempC_re += (TempC_re - Est_H / 10.0) / 2.0;
      if (rtIsNaN(TempC_re) || rtIsInf(TempC_re)) {
        c_r = rtNaN;
      } else if (TempC_re == 0.0) {
        c_r = 0.0;
      } else {
        c_r = fmod(TempC_re, 6.2831853071795862);
        rEQ0 = (c_r == 0.0);
        if (!rEQ0) {
          TempC_im = fabs(TempC_re / 6.2831853071795862);
          rEQ0 = !(fabs(TempC_im - floor(TempC_im + 0.5)) >
                   2.2204460492503131E-16 * TempC_im);
        }
        if (rEQ0) {
          c_r = 0.0;
        } else if (TempC_re < 0.0) {
          c_r += 6.2831853071795862;
        }
      }
      headingest_a_aver[19] = c_r;
    } else {
      centerest_a_aver[19] = bsum_tmp.re;
      centerest_a_aver[39] = bsum_tmp.im;
      headingest_a_aver[19] = heading_est_a[19];
    }
    /*      tag_pos_est_aver =
     * get_tag_pos(centerest_a_aver(end,1)+j*centerest_a_aver(end,2),
     * headingest_a_aver(end), tag_pos_b); */
    MeanA_re = headingest_a_aver[19] * 0.0;
    if (headingest_a_aver[19] == 0.0) {
      MeanA_re = exp(MeanA_re);
      MeanA_im = 0.0;
    } else {
      c_r = exp(MeanA_re / 2.0);
      MeanA_re = c_r * (c_r * cos(headingest_a_aver[19]));
      MeanA_im = c_r * (c_r * sin(headingest_a_aver[19]));
    }
    bsum.re = centerest_a_aver[39] * 0.0;
    ib = tag_pos_est_aver->size[0] * tag_pos_est_aver->size[1];
    tag_pos_est_aver->size[0] = 1;
    tag_pos_est_aver->size[1] = 4;
    emxEnsureCapacity_creal_T(tag_pos_est_aver, ib);
    tag_pos_est_aver_data = tag_pos_est_aver->data;
    tag_pos_est_aver_data[0].re =
        ((tag_pos_b[0].re * MeanA_re - tag_pos_b[0].im * MeanA_im) +
         centerest_a_aver[19]) +
        bsum.re;
    tag_pos_est_aver_data[0].im =
        (tag_pos_b[0].re * MeanA_im + tag_pos_b[0].im * MeanA_re) +
        centerest_a_aver[39];
    tag_pos_est_aver_data[1].re =
        ((tag_pos_b[1].re * MeanA_re - tag_pos_b[1].im * MeanA_im) +
         centerest_a_aver[19]) +
        bsum.re;
    tag_pos_est_aver_data[1].im =
        (tag_pos_b[1].re * MeanA_im + tag_pos_b[1].im * MeanA_re) +
        centerest_a_aver[39];
    tag_pos_est_aver_data[2].re =
        ((tag_pos_b[2].re * MeanA_re - tag_pos_b[2].im * MeanA_im) +
         centerest_a_aver[19]) +
        bsum.re;
    tag_pos_est_aver_data[2].im =
        (tag_pos_b[2].re * MeanA_im + tag_pos_b[2].im * MeanA_re) +
        centerest_a_aver[39];
    tag_pos_est_aver_data[3].re =
        ((tag_pos_b[3].re * MeanA_re - tag_pos_b[3].im * MeanA_im) +
         centerest_a_aver[19]) +
        bsum.re;
    tag_pos_est_aver_data[3].im =
        (tag_pos_b[3].re * MeanA_im + tag_pos_b[3].im * MeanA_re) +
        centerest_a_aver[39];
    *headingest_a_aver_v = headingest_a_aver[19];
    /*      K_heading_est = mod(headingest_a_aver(r),2*pi); */
    /*      K_centerest_a_aver(r, :) = centerest_a_aver(r,:); */
    /*      K_headingest_a_aver(r) = mod(headingest_a_aver(r),2*pi); */
  }
  /*  RxIDprev{1} = RxIDprev{2}; */
  /*  RxIDprev{2} = RxID; */
  RxIDprevLen[0] = RxIDprevLen[1];
  RxIDprevLen[1] = RxIDprevLen[2];
  RxIDprevLen[2] = RxIDprevLen[3];
  RxIDprevLen[3] = RxID_size[1];
  emxInit_real_T(&b_r, 2);
  ib = b_r->size[0] * b_r->size[1];
  b_r->size[0] = 3;
  b_r->size[1] = RxIDprev->size[1];
  emxEnsureCapacity_real_T(b_r, ib);
  TagDistInitCount_data = b_r->data;
  nblocks = RxIDprev->size[1];
  for (ib = 0; ib < nblocks; ib++) {
    TagDistInitCount_data[3 * ib] = RxIDprev_data[4 * ib + 1];
    TagDistInitCount_data[3 * ib + 1] = RxIDprev_data[4 * ib + 2];
    TagDistInitCount_data[3 * ib + 2] = RxIDprev_data[4 * ib + 3];
  }
  nblocks = b_r->size[1];
  for (ib = 0; ib < nblocks; ib++) {
    RxIDprev_data[4 * ib] = TagDistInitCount_data[3 * ib];
    RxIDprev_data[4 * ib + 1] = TagDistInitCount_data[3 * ib + 1];
    RxIDprev_data[4 * ib + 2] = TagDistInitCount_data[3 * ib + 2];
  }
  emxFree_real_T(&b_r);
  nblocks = RxID_size[1];
  for (ib = 0; ib < nblocks; ib++) {
    RxIDprev_data[4 * ib + 3] = RxID_data[ib];
  }
  PPprev[0] = PPprev[1];
  PPprev[1] = PPprev[2];
  PPprev[2] = PPprev[3];
  PPprev[3] = TagNum;
}

void UWBpos_free(void)
{
  emxFree_creal_T(&InterpPosition);
  emxFree_creal_T(&Tag_Pos_List);
  emxFree_real_T(&TagDistInitCount);
  emxFree_real_T(&TagDistInit);
  emxFree_real_T(&RxIDprev);
}

void UWBpos_init(void)
{
  emxInit_real_T(&RxIDprev, 2);
  emxInit_real_T(&TagDistInit, 2);
  emxInit_real_T(&TagDistInitCount, 2);
  emxInit_creal_T(&Tag_Pos_List, 3);
  emxInit_creal_T(&InterpPosition, 2);
  r = 0.0;
  memset(&heading_est_a[0], 0, 20U * sizeof(double));
  memset(&centerest_a[0], 0, 40U * sizeof(double));
  memset(&centerest_a_aver[0], 0, 40U * sizeof(double));
  memset(&headingest_a_aver[0], 0, 20U * sizeof(double));
  RxIDprevLen[0] = 0.0;
  PPprev[0] = 0.0;
  RxIDprevLen[1] = 0.0;
  PPprev[1] = 0.0;
  RxIDprevLen[2] = 0.0;
  PPprev[2] = 0.0;
  RxIDprevLen[3] = 0.0;
  PPprev[3] = 0.0;
}

void r_not_empty_init(void)
{
  r_not_empty = false;
}

/* End of code generation (UWBpos.c) */
