/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: UWBpos6.c
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 05-Sep-2023 20:11:41
 */

/* Include Files */
#include "UWBpos6.h"
#include "GetInitPos3.h"
#include "UWBpos6_data.h"
#include "UWBpos6_emxutil.h"
#include "UWBpos6_initialize.h"
#include "UWBpos6_types.h"
#include "combineVectorElements.h"
#include "mean.h"
#include "mod.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>
#include <stdio.h>

/* Variable Definitions */
static double r;

static bool r_not_empty;

static emxArray_creal_T *Tag_Pos_List;

static double heading_est_a[60];

static double centerest_a[120];

static double centerest_a_aver[120];

static double headingest_a_aver[60];

static emxArray_real_T *DistList;

static emxArray_real_T *DistListTime;

static emxArray_creal_T *PrevPos;

static double PrevHeading;

/* Function Declarations */
static double rt_roundd_snf(double u);

/* Function Definitions */
/*
 * Arguments    : double u
 * Return Type  : double
 */
static double rt_roundd_snf(double u)
{
  double y;
  if (fabs(u) < 4.503599627370496E+15) {
    if (u >= 0.5) {
      y = floor(u + 0.5);
    } else if (u > -0.5) {
      y = u * 0.0;
    } else {
      y = ceil(u - 0.5);
    }
  } else {
    y = u;
  }
  return y;
}

/*
 * [tag_pos_est, heading_est,tag_pos_est_aver,headingest_a_aver_v]
 *
 * Arguments    : double Ln
 *                double Lp
 *                double LnC
 *                double TagNum
 *                double Nanchor
 *                const double RxIDin[6]
 *                const double RxDistin[6]
 *                double s_time
 *                const creal_T tag_pos_b[4]
 *                const double xain[6]
 *                const double yain[6]
 *                const creal_T prevTagPos[4]
 *                double prevTagHeading
 *                double UWBout[21]
 * Return Type  : void
 */
void UWBpos6(double Ln, double Lp, double LnC, double TagNum, double Nanchor,
             const double RxIDin[6], const double RxDistin[6], double s_time,
             const creal_T tag_pos_b[4], const double xain[6],
             const double yain[6], const creal_T prevTagPos[4],
             double prevTagHeading, double UWBout[21])
{
  // printf("------------------------------------------------------------------\n");
  // printf("Ln: %lf\n", Ln);
  // printf("Lp: %lf\n", Lp);
  // printf("LnC: %lf\n", LnC);
  // printf("TagNum: %lf\n", TagNum);
  // printf("Nanchor: %lf\n", Nanchor);
  // printf("s_time: %lf\n", s_time);
  // printf("prevTagHeading: %lf\n", prevTagHeading);

  // for (int i = 0; i < 6; i++) {
  //     printf("RxIDin[%d]: %lf\n", i, RxIDin[i]);
  //     printf("RxDistin[%d]: %lf\n", i, RxDistin[i]);
  //     printf("xain[%d]: %lf\n", i, xain[i]);
  //     printf("yain[%d]: %lf\n", i, yain[i]);
  // }

  // for (int i = 0; i < 4; i++) {
  //     printf("tag_pos_b[%d]: %lf + %lfi\n", i, tag_pos_b[i].re, tag_pos_b[i].im);
  //     printf("prevTagPos[%d]: %lf + %lfi\n", i, prevTagPos[i].re, prevTagPos[i].im);
  // }
  // printf("------------------------------------------------------------------\n");
  emxArray_creal_T *b_r;
  emxArray_creal_T *x;
  emxArray_real_T *DistPredicted;
  emxArray_real_T *y;
  creal_T *PrevPos_data;
  creal_T *Tag_Pos_List_data;
  creal_T *x_data;
  double xain_data[6];
  double yain_data[6];
  double ErrSum;
  double heading_est_t3;
  double s;
  double *DistListTime_data;
  double *DistList_data;
  double *DistPredicted_data;
  int xain_size[2];
  int yain_size[2];
  int b_loop_ub;
  int firstBlockLength;
  int ib;
  int k;
  int lastBlockLength;
  int loop_ub;
  int nblocks;
  int xblockoffset;
  int xi;
  int xpageoffset;
  if (!isInitialized_UWBpos6) {
    UWBpos6_initialize();
  }
  PrevPos_data = PrevPos->data;
  DistListTime_data = DistListTime->data;
  DistList_data = DistList->data;
  Tag_Pos_List_data = Tag_Pos_List->data;
  if (!r_not_empty) {
    r_not_empty = true;
    xblockoffset =
        Tag_Pos_List->size[0] * Tag_Pos_List->size[1] * Tag_Pos_List->size[2];
    Tag_Pos_List->size[0] = 16;
    Tag_Pos_List->size[1] = 2;
    nblocks = (int)Lp;
    Tag_Pos_List->size[2] = (int)Lp;
    emxEnsureCapacity_creal_T(Tag_Pos_List, xblockoffset);
    Tag_Pos_List_data = Tag_Pos_List->data;
    loop_ub = (int)Lp << 5;
    for (xblockoffset = 0; xblockoffset < loop_ub; xblockoffset++) {
      Tag_Pos_List_data[xblockoffset].re = 0.0;
      Tag_Pos_List_data[xblockoffset].im = 0.0;
    }
    xblockoffset = DistList->size[0] * DistList->size[1] * DistList->size[2];
    DistList->size[0] = (int)Ln;
    DistList->size[1] = (int)Lp;
    DistList->size[2] = 30;
    emxEnsureCapacity_real_T(DistList, xblockoffset);
    DistList_data = DistList->data;
    firstBlockLength = (int)Ln * (int)Lp * 30;
    for (xblockoffset = 0; xblockoffset < firstBlockLength; xblockoffset++) {
      DistList_data[xblockoffset] = 0.0;
    }
    xblockoffset =
        DistListTime->size[0] * DistListTime->size[1] * DistListTime->size[2];
    DistListTime->size[0] = (int)Ln;
    DistListTime->size[1] = (int)Lp;
    DistListTime->size[2] = 30;
    emxEnsureCapacity_real_T(DistListTime, xblockoffset);
    DistListTime_data = DistListTime->data;
    for (xblockoffset = 0; xblockoffset < firstBlockLength; xblockoffset++) {
      DistListTime_data[xblockoffset] = 0.0;
    }
    xblockoffset = PrevPos->size[0] * PrevPos->size[1];
    PrevPos->size[0] = 1;
    PrevPos->size[1] = (int)Lp;
    emxEnsureCapacity_creal_T(PrevPos, xblockoffset);
    PrevPos_data = PrevPos->data;
    for (xblockoffset = 0; xblockoffset < nblocks; xblockoffset++) {
      PrevPos_data[xblockoffset].re = 0.0;
      PrevPos_data[xblockoffset].im = 0.0;
    }
  }
  if (r < 1000.0) {
    r++;
    printf("roop number-->[%f]\r\n",r);
  }
  if (LnC < 1.0) {
    loop_ub = 0;
    b_loop_ub = 0;
  } else {
    loop_ub = (int)LnC;
    b_loop_ub = (int)LnC;
  }
  xblockoffset = (int)Nanchor;
  for (firstBlockLength = 0; firstBlockLength < xblockoffset;
       firstBlockLength++) {
    double dv[29];
    /* if RxDistin(q)<20 */
    s = RxIDin[firstBlockLength];
    for (xpageoffset = 0; xpageoffset < 29; xpageoffset++) {
      dv[xpageoffset] =
          DistList_data[(((int)s + DistList->size[0] * ((int)TagNum - 1)) +
                         DistList->size[0] * DistList->size[1] *
                             (xpageoffset + 1)) -
                        1];
    }
    DistList_data[(((int)s + DistList->size[0] * ((int)TagNum - 1)) +
                   DistList->size[0] * DistList->size[1] * 29) -
                  1] = RxDistin[firstBlockLength];
    for (xpageoffset = 0; xpageoffset < 29; xpageoffset++) {
      DistList_data[(((int)s + DistList->size[0] * ((int)TagNum - 1)) +
                     DistList->size[0] * DistList->size[1] * xpageoffset) -
                    1] = dv[xpageoffset];
      dv[xpageoffset] = DistListTime_data
          [(((int)s + DistListTime->size[0] * ((int)TagNum - 1)) +
            DistListTime->size[0] * DistListTime->size[1] * (xpageoffset + 1)) -
           1];
    }
    for (xpageoffset = 0; xpageoffset < 29; xpageoffset++) {
      DistListTime_data[(((int)s + DistListTime->size[0] * ((int)TagNum - 1)) +
                         DistListTime->size[0] * DistListTime->size[1] *
                             xpageoffset) -
                        1] = dv[xpageoffset];
    }
    DistListTime_data[(((int)s + DistListTime->size[0] * ((int)TagNum - 1)) +
                       DistListTime->size[0] * DistListTime->size[1] * 29) -
                      1] = s_time;
    /* end */
  }
  xblockoffset = (int)Ln;
  emxInit_real_T(&DistPredicted, 2);
  xpageoffset = DistPredicted->size[0] * DistPredicted->size[1];
  DistPredicted->size[0] = (int)Ln;
  DistPredicted->size[1] = (int)Lp;
  emxEnsureCapacity_real_T(DistPredicted, xpageoffset);
  DistPredicted_data = DistPredicted->data;
  for (firstBlockLength = 0; firstBlockLength < xblockoffset;
       firstBlockLength++) {
    xpageoffset = (int)Lp;
    for (nblocks = 0; nblocks < xpageoffset; nblocks++) {
      for (lastBlockLength = 0; lastBlockLength < 30; lastBlockLength++) {
        if (s_time -
                DistListTime_data[(firstBlockLength +
                                   DistListTime->size[0] * nblocks) +
                                  DistListTime->size[0] *
                                      DistListTime->size[1] * lastBlockLength] >
            2.0) {
          DistList_data[(firstBlockLength + DistList->size[0] * nblocks) +
                        DistList->size[0] * DistList->size[1] *
                            lastBlockLength] = 0.0;
          DistListTime_data[(firstBlockLength +
                             DistListTime->size[0] * nblocks) +
                            DistListTime->size[0] * DistListTime->size[1] *
                                lastBlockLength] = 0.0;
        }
      }
      if (s_time - DistListTime_data
                       [(firstBlockLength + DistListTime->size[0] * nblocks) +
                        DistListTime->size[0] * DistListTime->size[1] * 29] <
          0.3) {
        DistPredicted_data[firstBlockLength +
                           DistPredicted->size[0] * nblocks] =
            DistList_data[(firstBlockLength + DistList->size[0] * nblocks) +
                          DistList->size[0] * DistList->size[1] * 29];
      } else {
        DistPredicted_data[firstBlockLength +
                           DistPredicted->size[0] * nblocks] = 0.0;
      }
      /*              leng = length(find(DistList(q,p,:)~=0)); */
      /*              if leng > 1 */
      /*                  DistPredicted(q,p) = sum((DistList(q,p,:)))/leng; */
      /*              else */
      /*                  DistPredicted(q,p) = 0; */
      /*              end */
      /*          if length(find(DistList(q,p,:)~=0))>3 */
      /*              DistPredicted(q,p) =
       * InterpPos3(DistListTime(q,p,:),DistList(q,p,:),s_time,1); */
      /*   */
      /*              %         elseif (q<=size(prevDist,1)) &&
       * (p<=size(prevDist,2)) */
      /*              %             DistPredicted(q,p) = prevDist(q,p); */
      /*          else */
      /*              DistPredicted(q,p) = 0; */
      /*          end */
    }
  }
  if (TagNum == 4) {
    creal_T dc;
    creal_T tag_center_pos_est_tmp;
    double MeanA_im;
    double MeanA_re;
    double ai;
    double ar;
    double brm;
    double bsum;
    double tag_center_pos_est_re;
    double tag_center_vel_est_im;
    double tag_center_vel_est_re;
    emxInit_real_T(&y, 2);
    if ((DistPredicted->size[0] == 0) || (DistPredicted->size[1] == 0)) {
      xblockoffset = y->size[0] * y->size[1];
      y->size[0] = 1;
      y->size[1] = DistPredicted->size[1];
      emxEnsureCapacity_real_T(y, xblockoffset);
      DistListTime_data = y->data;
      nblocks = DistPredicted->size[1];
      for (xblockoffset = 0; xblockoffset < nblocks; xblockoffset++) {
        DistListTime_data[xblockoffset] = 0.0;
      }
    } else {
      int npages;
      npages = DistPredicted->size[1];
      xblockoffset = y->size[0] * y->size[1];
      y->size[0] = 1;
      y->size[1] = DistPredicted->size[1];
      emxEnsureCapacity_real_T(y, xblockoffset);
      DistListTime_data = y->data;
      if (DistPredicted->size[0] <= 1024) {
        firstBlockLength = DistPredicted->size[0];
        lastBlockLength = 0;
        nblocks = 1;
      } else {
        firstBlockLength = 1024;
        nblocks = (int)((unsigned int)DistPredicted->size[0] >> 10);
        lastBlockLength = DistPredicted->size[0] - (nblocks << 10);
        if (lastBlockLength > 0) {
          nblocks++;
        } else {
          lastBlockLength = 1024;
        }
      }
      for (xi = 0; xi < npages; xi++) {
        xpageoffset = xi * DistPredicted->size[0];
        DistListTime_data[xi] = DistPredicted_data[xpageoffset];
        for (k = 2; k <= firstBlockLength; k++) {
          DistListTime_data[xi] += DistPredicted_data[(xpageoffset + k) - 1];
        }
        for (ib = 2; ib <= nblocks; ib++) {
          int hi;
          xblockoffset = xpageoffset + ((ib - 1) << 10);
          bsum = DistPredicted_data[xblockoffset];
          if (ib == nblocks) {
            hi = lastBlockLength;
          } else {
            hi = 1024;
          }
          for (k = 2; k <= hi; k++) {
            bsum += DistPredicted_data[(xblockoffset + k) - 1];
          }
          DistListTime_data[xi] += bsum;
        }
      }
    }
    nblocks = y->size[1];
    if (y->size[1] <= 2) {
      if (y->size[1] == 1) {
        bsum = DistListTime_data[0];
      } else if ((DistListTime_data[0] > DistListTime_data[y->size[1] - 1]) ||
                 (rtIsNaN(DistListTime_data[0]) &&
                  (!rtIsNaN(DistListTime_data[y->size[1] - 1])))) {
        bsum = DistListTime_data[y->size[1] - 1];
      } else {
        bsum = DistListTime_data[0];
      }
    } else {
      if (!rtIsNaN(DistListTime_data[0])) {
        firstBlockLength = 1;
      } else {
        bool exitg1;
        firstBlockLength = 0;
        k = 2;
        exitg1 = false;
        while ((!exitg1) && (k <= nblocks)) {
          if (!rtIsNaN(DistListTime_data[k - 1])) {
            firstBlockLength = k;
            exitg1 = true;
          } else {
            k++;
          }
        }
      }
      if (firstBlockLength == 0) {
        bsum = DistListTime_data[0];
      } else {
        bsum = DistListTime_data[firstBlockLength - 1];
        xblockoffset = firstBlockLength + 1;
        for (k = xblockoffset; k <= nblocks; k++) {
          s = DistListTime_data[k - 1];
          if (bsum > s) {
            bsum = s;
          }
        }
      }
    }
    emxFree_real_T(&y);
    if (bsum > 0.0) {
      /*  (length(find(DistPredicted(:)~=0))>3) */
      /*  if (length(find(DistPredicted(:)~=0))>8)     */
      /*  [tag_pos_est_t, heading_est_t] =  GetUWBPos_v3(xa, ya,
       * DistPredicted(1:LnC,:), tag_pos_b, tag_pos_est, heading_est); */
      /*          [tag_pos_est_t3, heading_est_t3] =  GetUWBPos_v2(xa, ya,
       * DistPredicted(1:LnC,:), tag_pos_b); */
      if (LnC < 1.0) {
        nblocks = 0;
      } else {
        nblocks = (int)LnC;
      }
      xain_size[0] = 1;
      xain_size[1] = loop_ub;
      if (loop_ub - 1 >= 0) {
        memcpy(&xain_data[0], &xain[0], (unsigned int)loop_ub * sizeof(double));
      }
      yain_size[0] = 1;
      yain_size[1] = b_loop_ub;
      if (b_loop_ub - 1 >= 0) {
        memcpy(&yain_data[0], &yain[0],
               (unsigned int)b_loop_ub * sizeof(double));
      }
      firstBlockLength = DistPredicted->size[1];
      for (xblockoffset = 0; xblockoffset < firstBlockLength; xblockoffset++) {
        for (xpageoffset = 0; xpageoffset < nblocks; xpageoffset++) {
          DistPredicted_data[xpageoffset + nblocks * xblockoffset] =
              DistPredicted_data[xpageoffset +
                                 DistPredicted->size[0] * xblockoffset];
        }
      }
      creal_T tag_pos_est_aver[4];
      xblockoffset = DistPredicted->size[0] * DistPredicted->size[1];
      DistPredicted->size[0] = nblocks;
      emxEnsureCapacity_real_T(DistPredicted, xblockoffset);
      GetInitPos3(xain_data, xain_size, yain_data, yain_size, DistPredicted,
                  tag_pos_b, LnC, prevTagPos, prevTagHeading, tag_pos_est_aver,
                  &heading_est_t3, &ErrSum);
      xblockoffset = PrevPos->size[0] * PrevPos->size[1];
      PrevPos->size[0] = 1;
      PrevPos->size[1] = 4;
      emxEnsureCapacity_creal_T(PrevPos, xblockoffset);
      PrevPos_data = PrevPos->data;
      PrevPos_data[0] = tag_pos_est_aver[0];
      PrevPos_data[1] = tag_pos_est_aver[1];
      PrevPos_data[2] = tag_pos_est_aver[2];
      PrevPos_data[3] = tag_pos_est_aver[3];
      /*  %     [tag_pos_est_t3, heading_est_t3] =  GetInitPos(xa,ya,
       * DistPredicted(1:LnC,:),xa+j*ya,tag_pos_b,LnC,4); */
      /*   */
      /*          Te = sum(abs(abs(transpose(anch_pos)-(tag_pos_est_t2)) -
       * prevDist).^2,"all"); */
      /*          Tl = sum(abs(abs(transpose(anch_pos)-(tag_pos_est_t3)) -
       * prevDist).^2,"all"); */
      /*          if Te < Tl */
      /*              tag_pos_est_t3 = tag_pos_est_t2; */
      /*              heading_est_t3 = heading_est_t2; */
      /*          end */
    } else {
      heading_est_t3 = PrevHeading;
      ErrSum = 10000.0;
    }
    emxInit_creal_T(&b_r, 3);
    xblockoffset = b_r->size[0] * b_r->size[1] * b_r->size[2];
    b_r->size[0] = 15;
    b_r->size[1] = 2;
    b_r->size[2] = Tag_Pos_List->size[2];
    emxEnsureCapacity_creal_T(b_r, xblockoffset);
    x_data = b_r->data;
    loop_ub = Tag_Pos_List->size[2];
    for (xblockoffset = 0; xblockoffset < loop_ub; xblockoffset++) {
      for (xpageoffset = 0; xpageoffset < 2; xpageoffset++) {
        for (firstBlockLength = 0; firstBlockLength < 15; firstBlockLength++) {
          x_data[(firstBlockLength + 15 * xpageoffset) + 30 * xblockoffset] =
              Tag_Pos_List_data[((firstBlockLength + 16 * xpageoffset) +
                                 32 * xblockoffset) +
                                1];
        }
      }
    }
    loop_ub = b_r->size[2];
    for (xblockoffset = 0; xblockoffset < loop_ub; xblockoffset++) {
      for (xpageoffset = 0; xpageoffset < 2; xpageoffset++) {
        for (firstBlockLength = 0; firstBlockLength < 15; firstBlockLength++) {
          Tag_Pos_List_data[(firstBlockLength + 16 * xpageoffset) +
                            32 * xblockoffset] =
              x_data[(firstBlockLength + 15 * xpageoffset) + 30 * xblockoffset];
        }
      }
    }
    emxFree_creal_T(&b_r);
    Tag_Pos_List_data[15].re = s_time;
    Tag_Pos_List_data[15].im = 0.0;
    Tag_Pos_List_data[31] = PrevPos_data[0];
    Tag_Pos_List_data[47].re = s_time;
    Tag_Pos_List_data[47].im = 0.0;
    Tag_Pos_List_data[63] = PrevPos_data[1];
    Tag_Pos_List_data[79].re = s_time;
    Tag_Pos_List_data[79].im = 0.0;
    Tag_Pos_List_data[95] = PrevPos_data[2];
    Tag_Pos_List_data[111].re = s_time;
    Tag_Pos_List_data[111].im = 0.0;
    Tag_Pos_List_data[127] = PrevPos_data[3];
    PrevHeading = heading_est_t3;
    tag_center_pos_est_tmp = b_mean(PrevPos);
    emxInit_creal_T(&x, 3);
    xblockoffset = x->size[0] * x->size[1] * x->size[2];
    x->size[0] = 1;
    x->size[1] = 1;
    x->size[2] = Tag_Pos_List->size[2];
    emxEnsureCapacity_creal_T(x, xblockoffset);
    x_data = x->data;
    loop_ub = Tag_Pos_List->size[2];
    for (xblockoffset = 0; xblockoffset < loop_ub; xblockoffset++) {
      xpageoffset = 32 * xblockoffset + 11;
      x_data[xblockoffset].re = Tag_Pos_List_data[xpageoffset].re -
                                Tag_Pos_List_data[32 * xblockoffset].re;
      x_data[xblockoffset].im = Tag_Pos_List_data[xpageoffset].im -
                                Tag_Pos_List_data[32 * xblockoffset].im;
    }
    dc = combineVectorElements(x);
    if ((dc.re != 0.0) || (dc.im != 0.0)) {
      xblockoffset = x->size[0] * x->size[1] * x->size[2];
      x->size[0] = 1;
      x->size[1] = 1;
      x->size[2] = Tag_Pos_List->size[2];
      emxEnsureCapacity_creal_T(x, xblockoffset);
      x_data = x->data;
      loop_ub = Tag_Pos_List->size[2];
      for (xblockoffset = 0; xblockoffset < loop_ub; xblockoffset++) {
        firstBlockLength = 32 * xblockoffset + 27;
        nblocks = 32 * xblockoffset + 16;
        ar = Tag_Pos_List_data[firstBlockLength].re -
             Tag_Pos_List_data[nblocks].re;
        ai = Tag_Pos_List_data[firstBlockLength].im -
             Tag_Pos_List_data[nblocks].im;
        firstBlockLength = 32 * xblockoffset + 11;
        MeanA_re = Tag_Pos_List_data[firstBlockLength].re -
                   Tag_Pos_List_data[32 * xblockoffset].re;
        MeanA_im = Tag_Pos_List_data[firstBlockLength].im -
                   Tag_Pos_List_data[32 * xblockoffset].im;
        if (MeanA_im == 0.0) {
          if (ai == 0.0) {
            x_data[xblockoffset].re = ar / MeanA_re;
            x_data[xblockoffset].im = 0.0;
          } else if (ar == 0.0) {
            x_data[xblockoffset].re = 0.0;
            x_data[xblockoffset].im = ai / MeanA_re;
          } else {
            x_data[xblockoffset].re = ar / MeanA_re;
            x_data[xblockoffset].im = ai / MeanA_re;
          }
        } else if (MeanA_re == 0.0) {
          if (ar == 0.0) {
            x_data[xblockoffset].re = ai / MeanA_im;
            x_data[xblockoffset].im = 0.0;
          } else if (ai == 0.0) {
            x_data[xblockoffset].re = 0.0;
            x_data[xblockoffset].im = -(ar / MeanA_im);
          } else {
            x_data[xblockoffset].re = ai / MeanA_im;
            x_data[xblockoffset].im = -(ar / MeanA_im);
          }
        } else {
          brm = fabs(MeanA_re);
          bsum = fabs(MeanA_im);
          if (brm > bsum) {
            s = MeanA_im / MeanA_re;
            bsum = MeanA_re + s * MeanA_im;
            x_data[xblockoffset].re = (ar + s * ai) / bsum;
            x_data[xblockoffset].im = (ai - s * ar) / bsum;
          } else if (bsum == brm) {
            if (MeanA_re > 0.0) {
              s = 0.5;
            } else {
              s = -0.5;
            }
            if (MeanA_im > 0.0) {
              bsum = 0.5;
            } else {
              bsum = -0.5;
            }
            x_data[xblockoffset].re = (ar * s + ai * bsum) / brm;
            x_data[xblockoffset].im = (ai * s - ar * bsum) / brm;
          } else {
            s = MeanA_re / MeanA_im;
            bsum = MeanA_im + s * MeanA_re;
            x_data[xblockoffset].re = (s * ar + ai) / bsum;
            x_data[xblockoffset].im = (s * ai - ar) / bsum;
          }
        }
      }
      dc = combineVectorElements(x);
      if (dc.im == 0.0) {
        tag_center_vel_est_re = dc.re / (double)x->size[2];
        tag_center_vel_est_im = 0.0;
      } else if (dc.re == 0.0) {
        tag_center_vel_est_re = 0.0;
        tag_center_vel_est_im = dc.im / (double)x->size[2];
      } else {
        tag_center_vel_est_re = dc.re / (double)x->size[2];
        tag_center_vel_est_im = dc.im / (double)x->size[2];
      }
    } else {
      tag_center_vel_est_re = 0.0;
      tag_center_vel_est_im = 0.0;
    }
    emxFree_creal_T(&x);
    /*  heading_est_temp = mean(Tag_Pos_List(12,2,[2
     * 4]))-mean(Tag_Pos_List(1,2,[1 3])); */
    /*  heading_est = angle(heading_est_temp); */
    for (xblockoffset = 0; xblockoffset < 59; xblockoffset++) {
      heading_est_a[xblockoffset] = heading_est_a[xblockoffset + 1];
    }
    s = rt_roundd_snf(fabs(heading_est_t3 - heading_est_a[58]) /
                      6.2831853071795862);
    if (s < 2.147483648E+9) {
      xblockoffset = (int)s;
    } else if (s >= 2.147483648E+9) {
      xblockoffset = MAX_int32_T;
    } else {
      xblockoffset = 0;
    }
    s = rt_roundd_snf(6.2831853071795862 * (double)xblockoffset);
    if (s < 2.147483648E+9) {
      xblockoffset = (int)s;
    } else {
      xblockoffset = MAX_int32_T;
    }
    bsum = rt_roundd_snf(heading_est_t3 - (double)xblockoffset);
    if (bsum < 2.147483648E+9) {
      if (bsum >= -2.147483648E+9) {
        xblockoffset = (int)bsum;
      } else {
        xblockoffset = MIN_int32_T;
      }
    } else if (bsum >= 2.147483648E+9) {
      xblockoffset = MAX_int32_T;
    } else {
      xblockoffset = 0;
    }
    if (s < 2.147483648E+9) {
      xpageoffset = (int)s;
    } else {
      xpageoffset = MAX_int32_T;
    }
    s = rt_roundd_snf(heading_est_t3 + (double)xpageoffset);
    if (s < 2.147483648E+9) {
      if (s >= -2.147483648E+9) {
        xpageoffset = (int)s;
      } else {
        xpageoffset = MIN_int32_T;
      }
    } else if (s >= 2.147483648E+9) {
      xpageoffset = MAX_int32_T;
    } else {
      xpageoffset = 0;
    }
    if (heading_est_t3 - heading_est_a[58] > 3.1415926535897931) {
      heading_est_a[59] = xblockoffset;
    } else if (heading_est_a[58] - heading_est_t3 > 3.1415926535897931) {
      heading_est_a[59] = xpageoffset;
    } else {
      heading_est_a[59] = heading_est_t3;
    }
    for (xblockoffset = 0; xblockoffset < 59; xblockoffset++) {
      centerest_a[xblockoffset] = centerest_a[xblockoffset + 1];
      centerest_a_aver[xblockoffset] = centerest_a_aver[xblockoffset + 1];
      centerest_a[xblockoffset + 60] = centerest_a[xblockoffset + 61];
      centerest_a_aver[xblockoffset + 60] = centerest_a_aver[xblockoffset + 61];
      headingest_a_aver[xblockoffset] = headingest_a_aver[xblockoffset + 1];
    }
    centerest_a[59] = tag_center_pos_est_tmp.re;
    centerest_a[119] = tag_center_pos_est_tmp.im;
    if (r > 60.0) {
      creal_T b_x[30];
      double b_y;
      for (xblockoffset = 0; xblockoffset < 30; xblockoffset++) {
        s = centerest_a[xblockoffset + 60];
        b_x[xblockoffset].re = centerest_a[xblockoffset] + 0.0 * s;
        b_x[xblockoffset].im = s;
      }
      tag_center_pos_est_re = b_x[0].re;
      bsum = b_x[0].im;
      for (k = 0; k < 29; k++) {
        tag_center_pos_est_re += b_x[k + 1].re;
        bsum += b_x[k + 1].im;
      }
      if (bsum == 0.0) {
        MeanA_re = tag_center_pos_est_re / 30.0;
        MeanA_im = 0.0;
      } else if (tag_center_pos_est_re == 0.0) {
        MeanA_re = 0.0;
        MeanA_im = bsum / 30.0;
      } else {
        MeanA_re = tag_center_pos_est_re / 30.0;
        MeanA_im = bsum / 30.0;
      }
      for (xblockoffset = 0; xblockoffset < 30; xblockoffset++) {
        s = centerest_a[xblockoffset + 90];
        b_x[xblockoffset].re = centerest_a[xblockoffset + 30] + 0.0 * s;
        b_x[xblockoffset].im = s;
      }
      tag_center_pos_est_re = b_x[0].re;
      bsum = b_x[0].im;
      b_y = heading_est_a[0];
      brm = heading_est_a[30];
      for (k = 0; k < 29; k++) {
        tag_center_pos_est_re += b_x[k + 1].re;
        bsum += b_x[k + 1].im;
        b_y += heading_est_a[k + 1];
        brm += heading_est_a[k + 31];
      }
      if (bsum == 0.0) {
        ar = tag_center_pos_est_re / 30.0;
        ai = 0.0;
      } else if (tag_center_pos_est_re == 0.0) {
        ar = 0.0;
        ai = bsum / 30.0;
      } else {
        ar = tag_center_pos_est_re / 30.0;
        ai = bsum / 30.0;
      }
      bsum = ar - MeanA_re;
      s = ai - MeanA_im;
      if (s == 0.0) {
        tag_center_pos_est_re = bsum / 2.0;
      } else if (bsum == 0.0) {
        tag_center_pos_est_re = 0.0;
      } else {
        tag_center_pos_est_re = bsum / 2.0;
      }
      centerest_a_aver[59] = ar + tag_center_pos_est_re;
      if (s == 0.0) {
        bsum = 0.0;
      } else {
        bsum = s / 2.0;
      }
      centerest_a_aver[119] = ai + bsum;
      bsum = brm / 30.0;
      headingest_a_aver[59] = bsum + (bsum - b_y / 30.0) / 2.0;
    } else {
      centerest_a_aver[59] = tag_center_pos_est_tmp.re;
      centerest_a_aver[119] = tag_center_pos_est_tmp.im;
      headingest_a_aver[59] = heading_est_a[59];
    }
    /*      tag_pos_est_aver =
     * get_tag_pos(centerest_a_aver(end,1)+j*centerest_a_aver(end,2),
     * headingest_a_aver(end), tag_pos_b); */
    s = b_mod(headingest_a_aver[59]);
    tag_center_pos_est_re = s * 0.0;
    if (s == 0.0) {
      tag_center_pos_est_re = exp(tag_center_pos_est_re);
      bsum = 0.0;
    } else {
      bsum = exp(tag_center_pos_est_re / 2.0);
      tag_center_pos_est_re = bsum * (bsum * cos(s));
      bsum *= bsum * sin(s);
    }
    MeanA_re = centerest_a_aver[119] * 0.0;
    /* tag_pos_est_aver =
     * tag_pos_b*exp(j*(mod(headingest_a_aver(end),2*pi)))+mean(tag_pos_est); */
    /* [tag_pos_est, heading_est,tag_pos_est_aver,headingest_a_aver_v] */
    UWBout[8] = heading_est_t3;
    UWBout[0] = PrevPos_data[0].re;
    UWBout[4] = PrevPos_data[0].im;
    UWBout[9] =
        ((tag_pos_b[0].re * tag_center_pos_est_re - tag_pos_b[0].im * bsum) +
         centerest_a_aver[59]) +
        MeanA_re;
    UWBout[13] =
        (tag_pos_b[0].re * bsum + tag_pos_b[0].im * tag_center_pos_est_re) +
        centerest_a_aver[119];
    UWBout[1] = PrevPos_data[1].re;
    UWBout[5] = PrevPos_data[1].im;
    UWBout[10] =
        ((tag_pos_b[1].re * tag_center_pos_est_re - tag_pos_b[1].im * bsum) +
         centerest_a_aver[59]) +
        MeanA_re;
    UWBout[14] =
        (tag_pos_b[1].re * bsum + tag_pos_b[1].im * tag_center_pos_est_re) +
        centerest_a_aver[119];
    UWBout[2] = PrevPos_data[2].re;
    UWBout[6] = PrevPos_data[2].im;
    UWBout[11] =
        ((tag_pos_b[2].re * tag_center_pos_est_re - tag_pos_b[2].im * bsum) +
         centerest_a_aver[59]) +
        MeanA_re;
    UWBout[15] =
        (tag_pos_b[2].re * bsum + tag_pos_b[2].im * tag_center_pos_est_re) +
        centerest_a_aver[119];
    UWBout[3] = PrevPos_data[3].re;
    UWBout[7] = PrevPos_data[3].im;
    UWBout[12] =
        ((tag_pos_b[3].re * tag_center_pos_est_re - tag_pos_b[3].im * bsum) +
         centerest_a_aver[59]) +
        MeanA_re;
    UWBout[16] =
        (tag_pos_b[3].re * bsum + tag_pos_b[3].im * tag_center_pos_est_re) +
        centerest_a_aver[119];
    UWBout[17] = s;
    UWBout[18] = tag_center_vel_est_re;
    UWBout[19] = tag_center_vel_est_im;
    UWBout[20] = ErrSum;
  } 
  else {
    UWBout[8] = PrevHeading;
    UWBout[0] = PrevPos_data[0].re;
    UWBout[4] = PrevPos_data[0].im;
    UWBout[9] = PrevPos_data[0].re;
    UWBout[13] = PrevPos_data[0].im;
    UWBout[1] = PrevPos_data[1].re;
    UWBout[5] = PrevPos_data[1].im;
    UWBout[10] = PrevPos_data[1].re;
    UWBout[14] = PrevPos_data[1].im;
    UWBout[2] = PrevPos_data[2].re;
    UWBout[6] = PrevPos_data[2].im;
    UWBout[11] = PrevPos_data[2].re;
    UWBout[15] = PrevPos_data[2].im;
    UWBout[3] = PrevPos_data[3].re;
    UWBout[7] = PrevPos_data[3].im;
    UWBout[12] = PrevPos_data[3].re;
    UWBout[16] = PrevPos_data[3].im;
    UWBout[17] = PrevHeading;
    UWBout[18] = 0.0;
    UWBout[19] = 0.0;
    UWBout[20] = 100.0;
    // printf("UWBout : %f", UWBout[1]);
  }
  emxFree_real_T(&DistPredicted);
}

/*
 * [tag_pos_est, heading_est,tag_pos_est_aver,headingest_a_aver_v]
 *
 * Arguments    : void
 * Return Type  : void
 */
void UWBpos6_free(void)
{
  emxFree_creal_T(&Tag_Pos_List);
  emxFree_real_T(&DistList);
  emxFree_real_T(&DistListTime);
  emxFree_creal_T(&PrevPos);
}

/*
 * [tag_pos_est, heading_est,tag_pos_est_aver,headingest_a_aver_v]
 *
 * Arguments    : void
 * Return Type  : void
 */
void UWBpos6_init(void)
{
  emxInit_creal_T(&PrevPos, 2);
  emxInit_real_T(&DistListTime, 3);
  emxInit_real_T(&DistList, 3);
  emxInit_creal_T(&Tag_Pos_List, 3);
  r = 0.0;
  memset(&heading_est_a[0], 0, 60U * sizeof(double));
  memset(&centerest_a[0], 0, 120U * sizeof(double));
  memset(&centerest_a_aver[0], 0, 120U * sizeof(double));
  memset(&headingest_a_aver[0], 0, 60U * sizeof(double));
  PrevHeading = 0.0;
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
 * File trailer for UWBpos6.c
 *
 * [EOF]
 */
