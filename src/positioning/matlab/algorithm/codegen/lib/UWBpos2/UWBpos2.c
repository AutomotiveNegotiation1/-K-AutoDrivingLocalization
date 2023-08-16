/*
 * UWBpos2.c
 *
 * Code generation for function 'UWBpos2'
 *
 */

/* Include files */
#include "UWBpos2.h"
#include "GetInitPos.h"
#include "GetPos3.h"
#include "GetPosRefine2.h"
#include "UWBpos2_data.h"
#include "UWBpos2_emxutil.h"
#include "UWBpos2_initialize.h"
#include "UWBpos2_rtwutil.h"
#include "UWBpos2_types.h"
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

/* Function Declarations */
static double rt_roundd_snf(double u);

/* Function Definitions */
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

void UWBpos2(double Ln, double Lp, double LnC, double TagNum, double Nanchor,
             const double RxIDin[6], const double RxDistin[6], double s_time,
             const creal_T tag_pos_b[4], const double xain[6],
             const double yain[6], emxArray_real_T *UWBout)
{
  // printf("UWBpos.c : Ln-->[%f]\r\n", Ln);
  // printf("UWBpos.c : Lp-->[%f]\r\n", Lp);
  // printf("UWBpos.c : UWBout.size[0]-->[%d]\r\n", UWBout->size[0]);
  // printf("UWBpos.c : UWBout.size[1]-->[%d]\r\n", UWBout->size[1]);
  // printf("UWBpos.c : LnC-->[%f]\r\n", LnC);
  printf("UWBpos.c : TagNum-->[%f]\r\n", TagNum);
  // printf("UWBpos.c : Nanchor-->[%f]\r\n", Nanchor);
  int z;
  printf("UWBpos.c :RxIDin-->[");
  for (z=0;z<6;z++){
    printf("%f ", RxIDin[z]);
  }
  printf("]\r\n");
  // printf("UWBpos.c : RxID_size[0]-->[%f]\r\n", RxID_size[0]);
  
  int a;
  printf("UWBpos.c :RxDistin-->[");
  for (a=0;a<6;a++){
    printf("%f ", RxDistin[a]);
  }
  printf("]\r\n");
  
  // printf("UWBpos.c :s_time-->[%f]\r\n", s_time);
  // // printf("UWBpos.c :tag_pos_b-->[%f]\r\n", tag_pos_b);
  // int b;
  // printf("UWBpos.c :tag_pos_b-->[");
  // for (b=0;b<4;b++){
  //   printf("%f ", tag_pos_b[b].re);
  //   printf("%f ", tag_pos_b[b].im);
  // }
  // printf("]\r\n");
  // int c;
  // printf("UWBpos.c :xa-->[");
  // for (c=0;c<6;c++){
  //   printf("%f ", xain[c]);
  // }
  // printf("]\r\n");

  // int d;
  // printf("UWBpos.c :ya-->[");
  // for (d=0;d<6;d++){
  //   printf("%f ", yain[d]);
  // }
  // printf("]\r\n");
  emxArray_boolean_T *x;
  emxArray_creal_T *InterpPosT;
  emxArray_creal_T *tag_pos_est;
  emxArray_creal_T *tag_pos_est_aver;
  emxArray_int32_T *b_i;
  emxArray_real_T *DistT;
  emxArray_real_T *IndT;
  emxArray_real_T *b_DistT;
  emxArray_real_T *b_xain;
  emxArray_real_T *b_yain;
  emxArray_real_T *c_r;
  creal_T Pm[36];
  creal_T dcv1[36];
  creal_T Px[12];
  creal_T b_x[10];
  creal_T b_Pm[9];
  creal_T dcv[9];
  creal_T b_tag_pos_est[4];
  creal_T dcv2[3];
  creal_T Est_H_p;
  creal_T TempC;
  creal_T dc;
  creal_T *InterpPosition_data;
  creal_T *Tag_Pos_List_data;
  creal_T *tag_pos_est_aver_data;
  creal_T *tag_pos_est_data;
  double Py[12];
  double RxDist_data[6];
  double RxID_data[6];
  double xa_data[6];
  double ya_data[6];
  double Pm_im;
  double Pm_re;
  double Pm_re_tmp;
  double b_r;
  double b_re_tmp;
  double c_re_tmp;
  double heading_est;
  double headingest_a_aver_v;
  double im;
  double re;
  double re_tmp;
  double *DistT_data;
  double *RxIDprev_data;
  double *TagDistInitCount_data;
  double *TagDistInit_data;
  int tmp_data[12];
  int PPC;
  int i;
  int i1;
  int i2;
  int idx;
  int loop_ub;
  int loop_ub_tmp;
  int nx;
  int *i_data;
  boolean_T b_Px[12];
  boolean_T exitg1;
  boolean_T rEQ0;
  boolean_T *x_data;
  if (!isInitialized_UWBpos2) {
    UWBpos2_initialize();
  }
  RxIDprev_data = RxIDprev->data;
  TagDistInit_data = TagDistInit->data;
  TagDistInitCount_data = TagDistInitCount->data;
  Tag_Pos_List_data = Tag_Pos_List->data;
  InterpPosition_data = InterpPosition->data;
  /* [tag_pos_est, heading_est,tag_pos_est_aver,headingest_a_aver_v] */
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
    Tag_Pos_List->size[0] = 12;
    Tag_Pos_List->size[1] = 2;
    Tag_Pos_List->size[2] = (int)Lp;
    emxEnsureCapacity_creal_T(Tag_Pos_List, i);
    Tag_Pos_List_data = Tag_Pos_List->data;
    loop_ub = 24 * (int)Lp;
    for (i = 0; i < loop_ub; i++) {
      Tag_Pos_List_data[i].re = 0.0;
      Tag_Pos_List_data[i].im = 0.0;
    }
    i = TagDistInitCount->size[0] * TagDistInitCount->size[1];
    TagDistInitCount->size[0] = (int)Ln;
    TagDistInitCount->size[1] = (int)Lp;
    emxEnsureCapacity_real_T(TagDistInitCount, i);
    TagDistInitCount_data = TagDistInitCount->data;
    loop_ub_tmp = (int)Ln * (int)Lp;
    for (i = 0; i < loop_ub_tmp; i++) {
      TagDistInitCount_data[i] = 0.0;
    }
    i = TagDistInit->size[0] * TagDistInit->size[1];
    TagDistInit->size[0] = (int)Ln;
    TagDistInit->size[1] = (int)Lp;
    emxEnsureCapacity_real_T(TagDistInit, i);
    TagDistInit_data = TagDistInit->data;
    for (i = 0; i < loop_ub_tmp; i++) {
      TagDistInit_data[i] = 0.0;
    }
    i = RxIDprev->size[0] * RxIDprev->size[1];
    RxIDprev->size[0] = 4;
    RxIDprev->size[1] = (int)Ln;
    emxEnsureCapacity_real_T(RxIDprev, i);
    RxIDprev_data = RxIDprev->data;
    loop_ub = (int)Ln << 2;
    for (i = 0; i < loop_ub; i++) {
      RxIDprev_data[i] = 0.0;
    }
  }
  r++;
  if (Nanchor < 1.0) {
    loop_ub = 0;
  } else {
    loop_ub = (int)Nanchor;
  }
  if (loop_ub - 1 >= 0) {
    memcpy(&RxID_data[0], &RxIDin[0], (unsigned int)loop_ub * sizeof(double));
  }
  if (LnC < 1.0) {
    nx = 0;
  } else {
    nx = (int)LnC;
  }
  if (nx - 1 >= 0) {
    memcpy(&xa_data[0], &xain[0], (unsigned int)nx * sizeof(double));
  }
  if (LnC < 1.0) {
    nx = 0;
  } else {
    nx = (int)LnC;
  }
  if (nx - 1 >= 0) {
    memcpy(&ya_data[0], &yain[0], (unsigned int)nx * sizeof(double));
  }
  if (Nanchor < 1.0) {
    nx = 0;
  } else {
    nx = (int)Nanchor;
  }
  if (nx - 1 >= 0) {
    memcpy(&RxDist_data[0], &RxDistin[0], (unsigned int)nx * sizeof(double));
  }
  emxInit_creal_T(&tag_pos_est, 2);
  i = tag_pos_est->size[0] * tag_pos_est->size[1];
  tag_pos_est->size[0] = 1;
  tag_pos_est->size[1] = (int)Lp;
  emxEnsureCapacity_creal_T(tag_pos_est, i);
  tag_pos_est_data = tag_pos_est->data;
  loop_ub_tmp = (int)Lp;
  for (i = 0; i < loop_ub_tmp; i++) {
    tag_pos_est_data[i].re = 0.0;
    tag_pos_est_data[i].im = 0.0;
  }
  heading_est = 0.0;
  emxInit_creal_T(&tag_pos_est_aver, 2);
  i = tag_pos_est_aver->size[0] * tag_pos_est_aver->size[1];
  tag_pos_est_aver->size[0] = 1;
  tag_pos_est_aver->size[1] = (int)Lp;
  emxEnsureCapacity_creal_T(tag_pos_est_aver, i);
  tag_pos_est_aver_data = tag_pos_est_aver->data;
  loop_ub_tmp = (int)Lp;
  for (i = 0; i < loop_ub_tmp; i++) {
    tag_pos_est_aver_data[i].re = 0.0;
    tag_pos_est_aver_data[i].im = 0.0;
  }
  headingest_a_aver_v = 0.0;
  if (r < 10.0 * Lp) {
    i = (int)Nanchor;
    for (nx = 0; nx < i; nx++) {
      /*              for PP = mod(r-1,Lp)+1:mod(r-1,Lp)+1 */
      loop_ub_tmp = (int)RxIDin[nx] - 1;
      TagDistInitCount_data[loop_ub_tmp +
                            TagDistInitCount->size[0] * ((int)TagNum - 1)]++;
      b_r = TagDistInitCount_data[loop_ub_tmp + TagDistInitCount->size[0] *
                                                    ((int)TagNum - 1)];
      TagDistInit_data[loop_ub_tmp + TagDistInit->size[0] * ((int)TagNum - 1)] =
          TagDistInit_data[loop_ub_tmp +
                           TagDistInit->size[0] * ((int)TagNum - 1)] *
              (b_r - 1.0) / b_r +
          RxDistin[nx] / b_r;
      /*              end */
    }
    
  } else if (r == 10.0 * Lp) {
    i = (int)Nanchor;
    for (nx = 0; nx < i; nx++) {
      /*              for PP = mod(r-1,Lp)+1:mod(r-1,Lp)+1 */
      loop_ub_tmp = (int)RxIDin[nx] - 1;
      TagDistInitCount_data[loop_ub_tmp +
                            TagDistInitCount->size[0] * ((int)TagNum - 1)]++;
      b_r = TagDistInitCount_data[loop_ub_tmp + TagDistInitCount->size[0] *
                                                    ((int)TagNum - 1)];
      TagDistInit_data[loop_ub_tmp + TagDistInit->size[0] * ((int)TagNum - 1)] =
          TagDistInit_data[loop_ub_tmp +
                           TagDistInit->size[0] * ((int)TagNum - 1)] *
              (b_r - 1.0) / b_r +
          RxDistin[nx] / b_r;
      /*              end */
    }
    emxInit_real_T(&DistT, 2);
    i = DistT->size[0] * DistT->size[1];
    DistT->size[0] = TagDistInit->size[0];
    DistT->size[1] = TagDistInit->size[1];
    emxEnsureCapacity_real_T(DistT, i);
    DistT_data = DistT->data;
    nx = TagDistInit->size[0] * TagDistInit->size[1];
    for (i = 0; i < nx; i++) {
      DistT_data[i] = TagDistInit_data[i];
    }
    nx = TagDistInit->size[0] * TagDistInit->size[1] - 1;
    loop_ub_tmp = 0;
    for (idx = 0; idx <= nx; idx++) {
      if (TagDistInit_data[idx] == 0.0) {
        loop_ub_tmp++;
      }
    }
    emxInit_int32_T(&b_i, 1);
    i = b_i->size[0];
    b_i->size[0] = loop_ub_tmp;
    emxEnsureCapacity_int32_T(b_i, i);
    i_data = b_i->data;
    loop_ub_tmp = 0;
    for (idx = 0; idx <= nx; idx++) {
      if (TagDistInit_data[idx] == 0.0) {
        i_data[loop_ub_tmp] = idx + 1;
        loop_ub_tmp++;
      }
    }
    nx = b_i->size[0];
    for (i = 0; i < nx; i++) {
      DistT_data[i_data[i] - 1] = 1.0E+6;
    }
    emxInit_real_T(&IndT, 1);
    sum(DistT, IndT);
    TagDistInit_data = IndT->data;
    emxInit_boolean_T(&x, 1);
    i = x->size[0];
    x->size[0] = IndT->size[0];
    emxEnsureCapacity_boolean_T(x, i);
    x_data = x->data;
    nx = IndT->size[0];
    for (i = 0; i < nx; i++) {
      x_data[i] = (TagDistInit_data[i] < 100000.0);
    }
    nx = x->size[0];
    idx = 0;
    i = b_i->size[0];
    b_i->size[0] = x->size[0];
    emxEnsureCapacity_int32_T(b_i, i);
    i_data = b_i->data;
    loop_ub_tmp = 0;
    exitg1 = false;
    while ((!exitg1) && (loop_ub_tmp <= nx - 1)) {
      if (x_data[loop_ub_tmp]) {
        idx++;
        i_data[idx - 1] = loop_ub_tmp + 1;
        if (idx >= nx) {
          exitg1 = true;
        } else {
          loop_ub_tmp++;
        }
      } else {
        loop_ub_tmp++;
      }
    }
    if (x->size[0] == 1) {
      if (idx == 0) {
        b_i->size[0] = 0;
      }
    } else {
      i = b_i->size[0];
      if (idx < 1) {
        b_i->size[0] = 0;
      } else {
        b_i->size[0] = idx;
      }
      emxEnsureCapacity_int32_T(b_i, i);
      i_data = b_i->data;
    }
    emxFree_boolean_T(&x);
    i = IndT->size[0];
    IndT->size[0] = b_i->size[0];
    emxEnsureCapacity_real_T(IndT, i);
    TagDistInit_data = IndT->data;
    nx = b_i->size[0];
    for (i = 0; i < nx; i++) {
      TagDistInit_data[i] = i_data[i];
    }
    emxFree_int32_T(&b_i);
    loop_ub_tmp = DistT->size[1];
    emxInit_real_T(&b_DistT, 2);
    i = b_DistT->size[0] * b_DistT->size[1];
    b_DistT->size[0] = IndT->size[0];
    b_DistT->size[1] = DistT->size[1];
    emxEnsureCapacity_real_T(b_DistT, i);
    TagDistInitCount_data = b_DistT->data;
    for (i = 0; i < loop_ub_tmp; i++) {
      nx = IndT->size[0];
      for (i1 = 0; i1 < nx; i1++) {
        TagDistInitCount_data[i1 + b_DistT->size[0] * i] =
            DistT_data[((int)TagDistInit_data[i1] + DistT->size[0] * i) - 1];
      }
    }
    i = DistT->size[0] * DistT->size[1];
    DistT->size[0] = b_DistT->size[0];
    DistT->size[1] = b_DistT->size[1];
    emxEnsureCapacity_real_T(DistT, i);
    DistT_data = DistT->data;
    nx = b_DistT->size[0] * b_DistT->size[1];
    for (i = 0; i < nx; i++) {
      DistT_data[i] = TagDistInitCount_data[i];
    }
    emxFree_real_T(&b_DistT);
    emxInit_real_T(&b_xain, 2);
    i = b_xain->size[0] * b_xain->size[1];
    b_xain->size[0] = 1;
    b_xain->size[1] = IndT->size[0];
    emxEnsureCapacity_real_T(b_xain, i);
    TagDistInitCount_data = b_xain->data;
    nx = IndT->size[0];
    for (i = 0; i < nx; i++) {
      TagDistInitCount_data[i] = xain[(int)TagDistInit_data[i] - 1];
    }
    emxInit_real_T(&b_yain, 2);
    i = b_yain->size[0] * b_yain->size[1];
    b_yain->size[0] = 1;
    b_yain->size[1] = IndT->size[0];
    emxEnsureCapacity_real_T(b_yain, i);
    TagDistInitCount_data = b_yain->data;
    nx = IndT->size[0];
    for (i = 0; i < nx; i++) {
      TagDistInitCount_data[i] = yain[(int)TagDistInit_data[i] - 1];
    }
    GetInitPos(b_xain, b_yain, DistT, tag_pos_b, IndT->size[0], Lp,
               b_tag_pos_est, &heading_est);
    emxFree_real_T(&b_yain);
    emxFree_real_T(&b_xain);
    emxFree_real_T(&IndT);
    emxFree_real_T(&DistT);
    i = tag_pos_est->size[0] * tag_pos_est->size[1];
    tag_pos_est->size[0] = 1;
    tag_pos_est->size[1] = 4;
    emxEnsureCapacity_creal_T(tag_pos_est, i);
    tag_pos_est_data = tag_pos_est->data;
    tag_pos_est_data[0] = b_tag_pos_est[0];
    tag_pos_est_data[1] = b_tag_pos_est[1];
    tag_pos_est_data[2] = b_tag_pos_est[2];
    tag_pos_est_data[3] = b_tag_pos_est[3];
    i = (int)Lp;
    for (loop_ub_tmp = 0; loop_ub_tmp < i; loop_ub_tmp++) {
      for (nx = 0; nx < 12; nx++) {
        i1 = nx + 24 * loop_ub_tmp;
        Tag_Pos_List_data[i1].re = s_time + (((double)nx + 1.0) - 12.0) * 0.1;
        Tag_Pos_List_data[i1].im = 0.0;
        Tag_Pos_List_data[i1 + 12] = b_tag_pos_est[loop_ub_tmp];
      }
    }
  } else {
    /* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
    /* %%%%%%%%%%%%%%%%%  Next Position Prediction    %%%%%%%%%%%%%%%%%% */
    i = (int)Lp;
    for (PPC = 0; PPC < i; PPC++) {
      for (i1 = 0; i1 < 12; i1++) {
        loop_ub_tmp = i1 + 24 * PPC;
        Py[i1] = Tag_Pos_List_data[loop_ub_tmp + 12].re;
        re = Tag_Pos_List_data[loop_ub_tmp].re - s_time;
        im = Tag_Pos_List_data[loop_ub_tmp].im;
        Px[i1].re = re;
        Px[i1].im = im;
        b_Px[i1] = ((re == 0.0) && (im == 0.0));
      }
      eml_find(b_Px, tmp_data, &loop_ub_tmp);
      if (loop_ub_tmp == 0) {
        for (loop_ub_tmp = 0; loop_ub_tmp < 12; loop_ub_tmp++) {
          Pm_re_tmp = Px[loop_ub_tmp].re;
          b_r = Px[loop_ub_tmp].im;
          Pm[loop_ub_tmp].re = Pm_re_tmp * Pm_re_tmp - b_r * b_r;
          Pm_re_tmp *= b_r;
          Pm[loop_ub_tmp].im = Pm_re_tmp + Pm_re_tmp;
          Pm[loop_ub_tmp + 12] = Px[loop_ub_tmp];
          Pm[loop_ub_tmp + 24].re = 1.0;
          Pm[loop_ub_tmp + 24].im = 0.0;
        }
        for (i1 = 0; i1 < 3; i1++) {
          for (i2 = 0; i2 < 3; i2++) {
            re = 0.0;
            im = 0.0;
            for (idx = 0; idx < 12; idx++) {
              loop_ub_tmp = idx + 12 * i1;
              Pm_re = Pm[loop_ub_tmp].re;
              Pm_im = -Pm[loop_ub_tmp].im;
              loop_ub_tmp = idx + 12 * i2;
              b_r = Pm[loop_ub_tmp].im;
              Pm_re_tmp = Pm[loop_ub_tmp].re;
              re += Pm_re * Pm_re_tmp - Pm_im * b_r;
              im += Pm_re * b_r + Pm_im * Pm_re_tmp;
            }
            idx = i1 + 3 * i2;
            b_Pm[idx].re = re;
            b_Pm[idx].im = im;
          }
        }
        b_inv(b_Pm, dcv);
        for (i1 = 0; i1 < 3; i1++) {
          b_r = dcv[i1].re;
          headingest_a_aver_v = dcv[i1].im;
          Pm_re_tmp = dcv[i1 + 3].re;
          b_re_tmp = dcv[i1 + 3].im;
          c_re_tmp = dcv[i1 + 6].re;
          re_tmp = dcv[i1 + 6].im;
          for (i2 = 0; i2 < 12; i2++) {
            Pm_re = Pm[i2].re;
            Pm_im = -Pm[i2].im;
            re = b_r * Pm_re - headingest_a_aver_v * Pm_im;
            im = b_r * Pm_im + headingest_a_aver_v * Pm_re;
            Pm_re = Pm[i2 + 12].re;
            Pm_im = -Pm[i2 + 12].im;
            re += Pm_re_tmp * Pm_re - b_re_tmp * Pm_im;
            im += Pm_re_tmp * Pm_im + b_re_tmp * Pm_re;
            Pm_re = Pm[i2 + 24].re;
            Pm_im = -Pm[i2 + 24].im;
            re += c_re_tmp * Pm_re - re_tmp * Pm_im;
            im += c_re_tmp * Pm_im + re_tmp * Pm_re;
            idx = i1 + 3 * i2;
            dcv1[idx].re = re;
            dcv1[idx].im = im;
          }
        }
        for (i1 = 0; i1 < 12; i1++) {
          Px[i1].re = Py[i1];
          Px[i1].im = 0.0;
        }
        for (i1 = 0; i1 < 3; i1++) {
          re = 0.0;
          im = 0.0;
          for (i2 = 0; i2 < 12; i2++) {
            loop_ub_tmp = i1 + 3 * i2;
            b_r = dcv1[loop_ub_tmp].re;
            headingest_a_aver_v = Px[i2].im;
            Pm_re_tmp = dcv1[loop_ub_tmp].im;
            b_re_tmp = Px[i2].re;
            re += b_r * b_re_tmp - Pm_re_tmp * headingest_a_aver_v;
            im += b_r * headingest_a_aver_v + Pm_re_tmp * b_re_tmp;
          }
          dcv2[i1].re = re;
          dcv2[i1].im = im;
        }
        InterpPosition_data[PPC] = dcv2[2];
      } else {
        InterpPosition_data[PPC].re = Py[2];
        InterpPosition_data[PPC].im = 0.0;
      }
      /*  Py = pm x A */
      /*  tr(pm)xPy = tr(Pm)pm x A */
      /*  inv(tr(Pm)Pm) x tr(pm)xPy = A */
      for (i1 = 0; i1 < 12; i1++) {
        loop_ub_tmp = i1 + 24 * PPC;
        Py[i1] = Tag_Pos_List_data[loop_ub_tmp + 12].im;
        re = Tag_Pos_List_data[loop_ub_tmp].re - s_time;
        im = Tag_Pos_List_data[loop_ub_tmp].im;
        Px[i1].re = re;
        Px[i1].im = im;
        b_Px[i1] = ((re == 0.0) && (im == 0.0));
      }
      eml_find(b_Px, tmp_data, &loop_ub_tmp);
      if (loop_ub_tmp == 0) {
        for (loop_ub_tmp = 0; loop_ub_tmp < 12; loop_ub_tmp++) {
          Pm_re_tmp = Px[loop_ub_tmp].re;
          b_r = Px[loop_ub_tmp].im;
          Pm[loop_ub_tmp].re = Pm_re_tmp * Pm_re_tmp - b_r * b_r;
          Pm_re_tmp *= b_r;
          Pm[loop_ub_tmp].im = Pm_re_tmp + Pm_re_tmp;
          Pm[loop_ub_tmp + 12] = Px[loop_ub_tmp];
          Pm[loop_ub_tmp + 24].re = 1.0;
          Pm[loop_ub_tmp + 24].im = 0.0;
        }
        for (i1 = 0; i1 < 3; i1++) {
          for (i2 = 0; i2 < 3; i2++) {
            re = 0.0;
            im = 0.0;
            for (idx = 0; idx < 12; idx++) {
              loop_ub_tmp = idx + 12 * i1;
              Pm_re = Pm[loop_ub_tmp].re;
              Pm_im = -Pm[loop_ub_tmp].im;
              loop_ub_tmp = idx + 12 * i2;
              b_r = Pm[loop_ub_tmp].im;
              Pm_re_tmp = Pm[loop_ub_tmp].re;
              re += Pm_re * Pm_re_tmp - Pm_im * b_r;
              im += Pm_re * b_r + Pm_im * Pm_re_tmp;
            }
            idx = i1 + 3 * i2;
            b_Pm[idx].re = re;
            b_Pm[idx].im = im;
          }
        }
        b_inv(b_Pm, dcv);
        for (i1 = 0; i1 < 3; i1++) {
          b_r = dcv[i1].re;
          headingest_a_aver_v = dcv[i1].im;
          Pm_re_tmp = dcv[i1 + 3].re;
          b_re_tmp = dcv[i1 + 3].im;
          c_re_tmp = dcv[i1 + 6].re;
          re_tmp = dcv[i1 + 6].im;
          for (i2 = 0; i2 < 12; i2++) {
            Pm_re = Pm[i2].re;
            Pm_im = -Pm[i2].im;
            re = b_r * Pm_re - headingest_a_aver_v * Pm_im;
            im = b_r * Pm_im + headingest_a_aver_v * Pm_re;
            Pm_re = Pm[i2 + 12].re;
            Pm_im = -Pm[i2 + 12].im;
            re += Pm_re_tmp * Pm_re - b_re_tmp * Pm_im;
            im += Pm_re_tmp * Pm_im + b_re_tmp * Pm_re;
            Pm_re = Pm[i2 + 24].re;
            Pm_im = -Pm[i2 + 24].im;
            re += c_re_tmp * Pm_re - re_tmp * Pm_im;
            im += c_re_tmp * Pm_im + re_tmp * Pm_re;
            idx = i1 + 3 * i2;
            dcv1[idx].re = re;
            dcv1[idx].im = im;
          }
        }
        for (i1 = 0; i1 < 12; i1++) {
          Px[i1].re = Py[i1];
          Px[i1].im = 0.0;
        }
        for (i1 = 0; i1 < 3; i1++) {
          re = 0.0;
          im = 0.0;
          for (i2 = 0; i2 < 12; i2++) {
            loop_ub_tmp = i1 + 3 * i2;
            b_r = dcv1[loop_ub_tmp].re;
            headingest_a_aver_v = Px[i2].im;
            Pm_re_tmp = dcv1[loop_ub_tmp].im;
            b_re_tmp = Px[i2].re;
            re += b_r * b_re_tmp - Pm_re_tmp * headingest_a_aver_v;
            im += b_r * headingest_a_aver_v + Pm_re_tmp * b_re_tmp;
          }
          dcv2[i1].re = re;
          dcv2[i1].im = im;
        }
        InterpPosition_data[PPC + InterpPosition->size[0]] = dcv2[2];
      } else {
        InterpPosition_data[PPC + InterpPosition->size[0]].re = Py[2];
        InterpPosition_data[PPC + InterpPosition->size[0]].im = 0.0;
      }
      /*  Py = pm x A */
      /*  tr(pm)xPy = tr(Pm)pm x A */
      /*  inv(tr(Pm)Pm) x tr(pm)xPy = A */
    }
    /* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
    for (i = 0; i < nx; i++) {
      TagDistInit_data[((int)RxIDin[i] +
                        TagDistInit->size[0] * ((int)TagNum - 1)) -
                       1] = RxDistin[i];
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
    if (loop_ub > 1) {
      /*          [tag_pos_est, heading_est, CandPos] =
       * GetPos2(xa,ya,RxDist,RxID,tag_pos_b,Nanchor,PP,InterpPosition(:,1)+j*InterpPosition(:,2));
       */
      /*          [tag_pos_est, heading_est, CandPos] =
       * GetPos3(xa,ya,RxDist,RxID,tag_pos_b,Nanchor,PP,InterpPosition(:,1)+j*InterpPosition(:,2),TagDistInitPrev,RxIDprev);
       */
      i = InterpPosT->size[0];
      InterpPosT->size[0] = InterpPosition->size[0];
      emxEnsureCapacity_creal_T(InterpPosT, i);
      tag_pos_est_aver_data = InterpPosT->data;
      loop_ub_tmp = InterpPosition->size[0];
      for (i = 0; i < loop_ub_tmp; i++) {
        b_r = InterpPosition_data[i + InterpPosition->size[0]].im;
        headingest_a_aver_v =
            InterpPosition_data[i + InterpPosition->size[0]].re;
        tag_pos_est_aver_data[i].re =
            InterpPosition_data[i].re + (0.0 * headingest_a_aver_v - b_r);
        tag_pos_est_aver_data[i].im =
            InterpPosition_data[i].im + (0.0 * b_r + headingest_a_aver_v);
      }
      GetPos3(xa_data, ya_data, RxDist_data, nx, RxID_data, loop_ub, tag_pos_b,
              Nanchor, TagNum, InterpPosT, TagDistInit, RxIDprev, RxIDprevLen,
              PPprev, b_tag_pos_est, &heading_est, &Est_H_p);
      /*   */
      GetPosRefine2(xa_data, ya_data, RxDist_data, RxID_data, loop_ub,
                    tag_pos_b, Nanchor, TagNum, b_tag_pos_est, &heading_est,
                    &Est_H_p);
      i = tag_pos_est->size[0] * tag_pos_est->size[1];
      tag_pos_est->size[0] = 1;
      tag_pos_est->size[1] = 4;
      emxEnsureCapacity_creal_T(tag_pos_est, i);
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
      i = InterpPosT->size[0];
      InterpPosT->size[0] = InterpPosition->size[0];
      emxEnsureCapacity_creal_T(InterpPosT, i);
      tag_pos_est_aver_data = InterpPosT->data;
      nx = InterpPosition->size[0];
      for (i = 0; i < nx; i++) {
        b_r = InterpPosition_data[i + InterpPosition->size[0]].im;
        headingest_a_aver_v =
            InterpPosition_data[i + InterpPosition->size[0]].re;
        tag_pos_est_aver_data[i].re =
            InterpPosition_data[i].re + (0.0 * headingest_a_aver_v - b_r);
        tag_pos_est_aver_data[i].im =
            InterpPosition_data[i].im + (0.0 * b_r + headingest_a_aver_v);
      }
      TempC = mean(InterpPosT);
      Est_H_p.re = 0.0;
      Est_H_p.im = 0.0;
      i = InterpPosT->size[0];
      for (loop_ub_tmp = 0; loop_ub_tmp < i; loop_ub_tmp++) {
        Pm_re = tag_pos_est_aver_data[loop_ub_tmp].re - TempC.re;
        Pm_im = tag_pos_est_aver_data[loop_ub_tmp].im - TempC.im;
        Pm_re_tmp = tag_pos_b[loop_ub_tmp].re;
        b_re_tmp = tag_pos_b[loop_ub_tmp].im;
        if (b_re_tmp == 0.0) {
          if (Pm_im == 0.0) {
            re_tmp = Pm_re / Pm_re_tmp;
            b_r = 0.0;
          } else if (Pm_re == 0.0) {
            re_tmp = 0.0;
            b_r = Pm_im / Pm_re_tmp;
          } else {
            re_tmp = Pm_re / Pm_re_tmp;
            b_r = Pm_im / Pm_re_tmp;
          }
        } else if (Pm_re_tmp == 0.0) {
          if (Pm_re == 0.0) {
            re_tmp = Pm_im / b_re_tmp;
            b_r = 0.0;
          } else if (Pm_im == 0.0) {
            re_tmp = 0.0;
            b_r = -(Pm_re / b_re_tmp);
          } else {
            re_tmp = Pm_im / b_re_tmp;
            b_r = -(Pm_re / b_re_tmp);
          }
        } else {
          c_re_tmp = fabs(Pm_re_tmp);
          b_r = fabs(b_re_tmp);
          if (c_re_tmp > b_r) {
            headingest_a_aver_v = b_re_tmp / Pm_re_tmp;
            b_r = Pm_re_tmp + headingest_a_aver_v * b_re_tmp;
            re_tmp = (Pm_re + headingest_a_aver_v * Pm_im) / b_r;
            b_r = (Pm_im - headingest_a_aver_v * Pm_re) / b_r;
          } else if (b_r == c_re_tmp) {
            if (Pm_re_tmp > 0.0) {
              headingest_a_aver_v = 0.5;
            } else {
              headingest_a_aver_v = -0.5;
            }
            if (b_re_tmp > 0.0) {
              b_r = 0.5;
            } else {
              b_r = -0.5;
            }
            re_tmp = (Pm_re * headingest_a_aver_v + Pm_im * b_r) / c_re_tmp;
            b_r = (Pm_im * headingest_a_aver_v - Pm_re * b_r) / c_re_tmp;
          } else {
            headingest_a_aver_v = Pm_re_tmp / b_re_tmp;
            b_r = b_re_tmp + headingest_a_aver_v * Pm_re_tmp;
            re_tmp = (headingest_a_aver_v * Pm_re + Pm_im) / b_r;
            b_r = (headingest_a_aver_v * Pm_im - Pm_re) / b_r;
          }
        }
        Est_H_p.re += re_tmp;
        Est_H_p.im += b_r;
      }
      headingest_a_aver_v = rt_atan2d_snf(Est_H_p.im, Est_H_p.re);
      i = InterpPosT->size[0];
      for (loop_ub_tmp = 0; loop_ub_tmp < i; loop_ub_tmp++) {
        Est_H_p.re = headingest_a_aver_v * 0.0;
        if (headingest_a_aver_v == 0.0) {
          Pm_re_tmp = Est_H_p.re;
          Est_H_p.re = exp(Pm_re_tmp);
          Est_H_p.im = 0.0;
        } else {
          b_r = exp(Est_H_p.re / 2.0);
          Est_H_p.re = b_r * (b_r * cos(headingest_a_aver_v));
          Est_H_p.im = b_r * (b_r * sin(headingest_a_aver_v));
        }
        b_r = tag_pos_b[loop_ub_tmp].re;
        Pm_re_tmp = tag_pos_b[loop_ub_tmp].im;
        tag_pos_est_data[loop_ub_tmp].re =
            TempC.re + (b_r * Est_H_p.re - Pm_re_tmp * Est_H_p.im);
        tag_pos_est_data[loop_ub_tmp].im =
            TempC.im + (b_r * Est_H_p.im + Pm_re_tmp * Est_H_p.re);
      }
      b_r = InterpPosition_data[((int)TagNum + InterpPosition->size[0]) - 1].im;
      headingest_a_aver_v =
          InterpPosition_data[((int)TagNum + InterpPosition->size[0]) - 1].re;
      Est_H_p.re = InterpPosition_data[(int)TagNum - 1].re +
                   (0.0 * headingest_a_aver_v - b_r);
      Est_H_p.im = InterpPosition_data[(int)TagNum - 1].im +
                   (0.0 * b_r + headingest_a_aver_v);
    }
    emxFree_creal_T(&InterpPosT);
    /* %%%%%%%%%%%%%  Original %%%%%%%%%%%%% */
    for (i = 0; i < 2; i++) {
      for (i1 = 0; i1 < 11; i1++) {
        i2 = (i1 + 12 * i) + 24 * ((int)TagNum - 1);
        Tag_Pos_List_data[i2] = Tag_Pos_List_data[i2 + 1];
      }
    }
    i = 24 * ((int)TagNum - 1);
    Tag_Pos_List_data[i + 11].re = s_time;
    Tag_Pos_List_data[i + 11].im = 0.0;
    Tag_Pos_List_data[i + 23] = Est_H_p;
    /* %%%%%%%%%%%%%  Test %%%%%%%%%%%%% */
    /*      Tag_Pos_List(1:NumInterpPoint-1,:,:) =
     * Tag_Pos_List(2:NumInterpPoint,:,:); */
    /*      Tag_Pos_List(NumInterpPoint,1,:) = s_time; */
    /*      Tag_Pos_List(NumInterpPoint,2,:) = tag_pos_est; */
    /*      Tag_Pos_List(NumInterpPoint,:,PP) = [s_time CandPos]; */
    /* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
    /*              Tag_Pos_List(1:2,:,PP) = Tag_Pos_List(2:3,:,PP); */
    /*              Tag_Pos_List(3,:,PP) = [s_time(r) tag_pos_est(PP)]; */
    dc = b_mean(tag_pos_est);
    /*          end */
    for (i = 0; i < 19; i++) {
      heading_est_a[i] = heading_est_a[i + 1];
    }
    Pm_re_tmp = rt_roundd_snf(fabs(heading_est - heading_est_a[18]) /
                              6.2831853071795862);
    if (Pm_re_tmp < 2.147483648E+9) {
      i = (int)Pm_re_tmp;
    } else if (Pm_re_tmp >= 2.147483648E+9) {
      i = MAX_int32_T;
    } else {
      i = 0;
    }
    Pm_re_tmp = rt_roundd_snf(6.2831853071795862 * (double)i);
    if (Pm_re_tmp < 2.147483648E+9) {
      i = (int)Pm_re_tmp;
    } else {
      i = MAX_int32_T;
    }
    b_r = rt_roundd_snf(heading_est - (double)i);
    if (b_r < 2.147483648E+9) {
      if (b_r >= -2.147483648E+9) {
        i = (int)b_r;
      } else {
        i = MIN_int32_T;
      }
    } else if (b_r >= 2.147483648E+9) {
      i = MAX_int32_T;
    } else {
      i = 0;
    }
    if (Pm_re_tmp < 2.147483648E+9) {
      i1 = (int)Pm_re_tmp;
    } else {
      i1 = MAX_int32_T;
    }
    Pm_re_tmp = rt_roundd_snf(heading_est + (double)i1);
    if (Pm_re_tmp < 2.147483648E+9) {
      if (Pm_re_tmp >= -2.147483648E+9) {
        i1 = (int)Pm_re_tmp;
      } else {
        i1 = MIN_int32_T;
      }
    } else if (Pm_re_tmp >= 2.147483648E+9) {
      i1 = MAX_int32_T;
    } else {
      i1 = 0;
    }
    if (heading_est - heading_est_a[18] > 3.1415926535897931) {
      heading_est_a[19] = i;
    } else if (heading_est_a[18] - heading_est > 3.1415926535897931) {
      heading_est_a[19] = i1;
    } else {
      heading_est_a[19] = heading_est;
    }
    for (i = 0; i < 19; i++) {
      centerest_a[i] = centerest_a[i + 1];
      centerest_a_aver[i] = centerest_a_aver[i + 1];
      centerest_a[i + 20] = centerest_a[i + 21];
      centerest_a_aver[i + 20] = centerest_a_aver[i + 21];
      headingest_a_aver[i] = headingest_a_aver[i + 1];
    }
    centerest_a[19] = dc.re;
    centerest_a[39] = dc.im;
    if (r > 20.0) {
      for (i = 0; i < 10; i++) {
        Pm_re_tmp = centerest_a[i + 20];
        b_x[i].re = centerest_a[i] + 0.0 * Pm_re_tmp;
        b_x[i].im = Pm_re_tmp;
      }
      Est_H_p = b_x[0];
      for (loop_ub_tmp = 0; loop_ub_tmp < 9; loop_ub_tmp++) {
        Est_H_p.re += b_x[loop_ub_tmp + 1].re;
        Est_H_p.im += b_x[loop_ub_tmp + 1].im;
      }
      if (Est_H_p.im == 0.0) {
        TempC.re = Est_H_p.re / 10.0;
        TempC.im = 0.0;
      } else if (Est_H_p.re == 0.0) {
        TempC.re = 0.0;
        TempC.im = Est_H_p.im / 10.0;
      } else {
        TempC.re = Est_H_p.re / 10.0;
        TempC.im = Est_H_p.im / 10.0;
      }
      for (i = 0; i < 10; i++) {
        Pm_re_tmp = centerest_a[i + 30];
        b_x[i].re = centerest_a[i + 10] + 0.0 * Pm_re_tmp;
        b_x[i].im = Pm_re_tmp;
      }
      Est_H_p = b_x[0];
      b_re_tmp = heading_est_a[0];
      Pm_re_tmp = heading_est_a[10];
      for (loop_ub_tmp = 0; loop_ub_tmp < 9; loop_ub_tmp++) {
        Est_H_p.re += b_x[loop_ub_tmp + 1].re;
        Est_H_p.im += b_x[loop_ub_tmp + 1].im;
        b_re_tmp += heading_est_a[loop_ub_tmp + 1];
        Pm_re_tmp += heading_est_a[loop_ub_tmp + 11];
      }
      if (Est_H_p.im == 0.0) {
        re = Est_H_p.re / 10.0;
        im = 0.0;
      } else if (Est_H_p.re == 0.0) {
        re = 0.0;
        im = Est_H_p.im / 10.0;
      } else {
        re = Est_H_p.re / 10.0;
        im = Est_H_p.im / 10.0;
      }
      b_r = re - TempC.re;
      headingest_a_aver_v = im - TempC.im;
      if (headingest_a_aver_v == 0.0) {
        b_r /= 2.0;
      } else if (b_r == 0.0) {
        b_r = 0.0;
      } else {
        b_r /= 2.0;
      }
      centerest_a_aver[19] = re + b_r;
      if (headingest_a_aver_v == 0.0) {
        b_r = 0.0;
      } else {
        b_r = headingest_a_aver_v / 2.0;
      }
      centerest_a_aver[39] = im + b_r;
      b_r = Pm_re_tmp / 10.0;
      headingest_a_aver[19] = b_r + (b_r - b_re_tmp / 10.0) / 2.0;
    } else {
      centerest_a_aver[19] = dc.re;
      centerest_a_aver[39] = dc.im;
      headingest_a_aver[19] = heading_est_a[19];
    }
    /*      tag_pos_est_aver =
     * get_tag_pos(centerest_a_aver(end,1)+j*centerest_a_aver(end,2),
     * headingest_a_aver(end), tag_pos_b); */
    if (rtIsNaN(headingest_a_aver[19]) || rtIsInf(headingest_a_aver[19])) {
      headingest_a_aver_v = rtNaN;
    } else if (headingest_a_aver[19] == 0.0) {
      headingest_a_aver_v = 0.0;
    } else {
      headingest_a_aver_v = fmod(headingest_a_aver[19], 6.2831853071795862);
      rEQ0 = (headingest_a_aver_v == 0.0);
      if (!rEQ0) {
        b_r = fabs(headingest_a_aver[19] / 6.2831853071795862);
        rEQ0 = !(fabs(b_r - floor(b_r + 0.5)) > 2.2204460492503131E-16 * b_r);
      }
      if (rEQ0) {
        headingest_a_aver_v = 0.0;
      } else if (headingest_a_aver[19] < 0.0) {
        headingest_a_aver_v += 6.2831853071795862;
      }
    }
    Est_H_p.re = headingest_a_aver_v * 0.0;
    if (headingest_a_aver_v == 0.0) {
      Pm_re_tmp = Est_H_p.re;
      Est_H_p.re = exp(Pm_re_tmp);
      Est_H_p.im = 0.0;
    } else {
      b_r = exp(Est_H_p.re / 2.0);
      Est_H_p.re = b_r * (b_r * cos(headingest_a_aver_v));
      Est_H_p.im = b_r * (b_r * sin(headingest_a_aver_v));
    }
    i = tag_pos_est_aver->size[0] * tag_pos_est_aver->size[1];
    tag_pos_est_aver->size[0] = 1;
    tag_pos_est_aver->size[1] = 4;
    emxEnsureCapacity_creal_T(tag_pos_est_aver, i);
    tag_pos_est_aver_data = tag_pos_est_aver->data;
    TempC.re = 0.0 * dc.im;
    tag_pos_est_aver_data[0].re =
        ((tag_pos_b[0].re * Est_H_p.re - tag_pos_b[0].im * Est_H_p.im) +
         dc.re) +
        TempC.re;
    tag_pos_est_aver_data[0].im =
        (tag_pos_b[0].re * Est_H_p.im + tag_pos_b[0].im * Est_H_p.re) + dc.im;
    tag_pos_est_aver_data[1].re =
        ((tag_pos_b[1].re * Est_H_p.re - tag_pos_b[1].im * Est_H_p.im) +
         dc.re) +
        TempC.re;
    tag_pos_est_aver_data[1].im =
        (tag_pos_b[1].re * Est_H_p.im + tag_pos_b[1].im * Est_H_p.re) + dc.im;
    tag_pos_est_aver_data[2].re =
        ((tag_pos_b[2].re * Est_H_p.re - tag_pos_b[2].im * Est_H_p.im) +
         dc.re) +
        TempC.re;
    tag_pos_est_aver_data[2].im =
        (tag_pos_b[2].re * Est_H_p.im + tag_pos_b[2].im * Est_H_p.re) + dc.im;
    tag_pos_est_aver_data[3].re =
        ((tag_pos_b[3].re * Est_H_p.re - tag_pos_b[3].im * Est_H_p.im) +
         dc.re) +
        TempC.re;
    tag_pos_est_aver_data[3].im =
        (tag_pos_b[3].re * Est_H_p.im + tag_pos_b[3].im * Est_H_p.re) + dc.im;
    /*      K_heading_est = mod(headingest_a_aver(r),2*pi); */
    /*      K_centerest_a_aver(r, :) = centerest_a_aver(r,:); */
    /*      K_headingest_a_aver(r) = mod(headingest_a_aver(r),2*pi); */
  }
  /*  RxIDprev{1} = RxIDprev{2}; */
  /*  RxIDprev{2} = RxID; */
  RxIDprevLen[0] = RxIDprevLen[1];
  RxIDprevLen[1] = RxIDprevLen[2];
  RxIDprevLen[2] = RxIDprevLen[3];
  RxIDprevLen[3] = loop_ub;
  emxInit_real_T(&c_r, 2);
  i = c_r->size[0] * c_r->size[1];
  c_r->size[0] = 3;
  c_r->size[1] = RxIDprev->size[1];
  emxEnsureCapacity_real_T(c_r, i);
  TagDistInitCount_data = c_r->data;
  loop_ub = RxIDprev->size[1];
  for (i = 0; i < loop_ub; i++) {
    TagDistInitCount_data[3 * i] = RxIDprev_data[4 * i + 1];
    TagDistInitCount_data[3 * i + 1] = RxIDprev_data[4 * i + 2];
    TagDistInitCount_data[3 * i + 2] = RxIDprev_data[4 * i + 3];
  }
  loop_ub = c_r->size[1];
  for (i = 0; i < loop_ub; i++) {
    RxIDprev_data[4 * i] = TagDistInitCount_data[3 * i];
    RxIDprev_data[4 * i + 1] = TagDistInitCount_data[3 * i + 1];
    RxIDprev_data[4 * i + 2] = TagDistInitCount_data[3 * i + 2];
  }
  emxFree_real_T(&c_r);
  if (RxIDprevLen[3] < 1.0) {
    loop_ub = 0;
  } else {
    loop_ub = (int)RxIDprevLen[3];
  }
  for (i = 0; i < loop_ub; i++) {
    RxIDprev_data[4 * i + 3] = RxIDin[i];
  }
  PPprev[0] = PPprev[1];
  PPprev[1] = PPprev[2];
  PPprev[2] = PPprev[3];
  PPprev[3] = TagNum;
  /* [tag_pos_est, heading_est,tag_pos_est_aver,headingest_a_aver_v] */
  i = UWBout->size[0] * UWBout->size[1];
  UWBout->size[0] = 1;
  UWBout->size[1] = (((tag_pos_est->size[1] + tag_pos_est->size[1]) +
                      tag_pos_est_aver->size[1]) +
                     tag_pos_est_aver->size[1]) +
                    2;
  emxEnsureCapacity_real_T(UWBout, i);
  TagDistInitCount_data = UWBout->data;
  loop_ub = tag_pos_est->size[1];
  for (i = 0; i < loop_ub; i++) {
    TagDistInitCount_data[i] = tag_pos_est_data[i].re;
  }
  loop_ub = tag_pos_est->size[1];
  for (i = 0; i < loop_ub; i++) {
    TagDistInitCount_data[i + tag_pos_est->size[1]] = tag_pos_est_data[i].im;
  }
  TagDistInitCount_data[tag_pos_est->size[1] + tag_pos_est->size[1]] =
      heading_est;
  loop_ub = tag_pos_est_aver->size[1];
  for (i = 0; i < loop_ub; i++) {
    TagDistInitCount_data[((i + tag_pos_est->size[1]) + tag_pos_est->size[1]) +
                          1] = tag_pos_est_aver_data[i].re;
  }
  loop_ub = tag_pos_est_aver->size[1];
  for (i = 0; i < loop_ub; i++) {
    TagDistInitCount_data[(((i + tag_pos_est->size[1]) + tag_pos_est->size[1]) +
                           tag_pos_est_aver->size[1]) +
                          1] = tag_pos_est_aver_data[i].im;
  }
  TagDistInitCount_data[(((tag_pos_est->size[1] + tag_pos_est->size[1]) +
                          tag_pos_est_aver->size[1]) +
                         tag_pos_est_aver->size[1]) +
                        1] = headingest_a_aver_v;
  emxFree_creal_T(&tag_pos_est_aver);
  emxFree_creal_T(&tag_pos_est);
}

void UWBpos2_free(void)
{
  emxFree_creal_T(&InterpPosition);
  emxFree_creal_T(&Tag_Pos_List);
  emxFree_real_T(&TagDistInitCount);
  emxFree_real_T(&TagDistInit);
  emxFree_real_T(&RxIDprev);
}

void UWBpos2_init(void)
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

/* End of code generation (UWBpos2.c) */
