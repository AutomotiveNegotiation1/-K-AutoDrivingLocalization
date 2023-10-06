/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: GetInitPos3.c
 *
 * MATLAB Coder version            : 5.5
 * C/C++ source code generated on  : 05-Sep-2023 20:11:41
 */

/* Include Files */
#include "GetInitPos3.h"
#include "ThreeAnchPos3.h"
#include "TwoAnchPos3.h"
#include "UWBpos6_data.h"
#include "UWBpos6_emxutil.h"
#include "UWBpos6_rtwutil.h"
#include "UWBpos6_types.h"
#include "exp.h"
#include "find.h"
#include "mean.h"
#include "minOrMax.h"
#include "mod.h"
#include "rt_nonfinite.h"
#include "sort.h"
#include "sum.h"
#include "rt_defines.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Variable Definitions */
static creal_T PrevUWBpos[4];

/* Function Declarations */
static void b_binary_expand_op(const double in1_data[], const int in1_size[2],
                               const emxArray_int32_T *in2,
                               const double in3_data[], const int in3_size[2],
                               const emxArray_real_T *in4, int in5,
                               double in7[4], double in8[2]);

static double rt_atan2d_snf(double u0, double u1);

/* Function Definitions */
/*
 * Arguments    : const double in1_data[]
 *                const int in1_size[2]
 *                const emxArray_int32_T *in2
 *                const double in3_data[]
 *                const int in3_size[2]
 *                const emxArray_real_T *in4
 *                int in5
 *                double in7[4]
 *                double in8[2]
 * Return Type  : void
 */
static void b_binary_expand_op(const double in1_data[], const int in1_size[2],
                               const emxArray_int32_T *in2,
                               const double in3_data[], const int in3_size[2],
                               const emxArray_real_T *in4, int in5,
                               double in7[4], double in8[2])
{
  emxArray_real_T *c_in4;
  creal_T b_in1_data[6];
  double b_in4[2];
  double in1[2];
  double in3[2];
  const double *in4_data;
  double *b_in4_data;
  int b_in1_size[2];
  const int *in2_data;
  int i;
  int loop_ub;
  int stride_0_1;
  int stride_1_1;
  in4_data = in4->data;
  in2_data = in2->data;
  in1[0] = in1_data[in2_data[0] - 1];
  in1[1] = in1_data[in2_data[1] - 1];
  in3[0] = in3_data[in2_data[0] - 1];
  in3[1] = in3_data[in2_data[1] - 1];
  b_in4[0] = in4_data[(in2_data[0] + in4->size[0] * in5) - 1];
  b_in4[1] = in4_data[(in2_data[1] + in4->size[0] * in5) - 1];
  b_in1_size[0] = 1;
  if (in3_size[1] == 1) {
    b_in1_size[1] = in1_size[1];
  } else {
    b_in1_size[1] = in3_size[1];
  }
  stride_0_1 = (in1_size[1] != 1);
  stride_1_1 = (in3_size[1] != 1);
  if (in3_size[1] == 1) {
    loop_ub = in1_size[1];
  } else {
    loop_ub = in3_size[1];
  }
  for (i = 0; i < loop_ub; i++) {
    double d;
    d = in3_data[i * stride_1_1];
    b_in1_data[i].re = in1_data[i * stride_0_1] + 0.0 * d;
    b_in1_data[i].im = d;
  }
  emxInit_real_T(&c_in4, 1);
  i = c_in4->size[0];
  c_in4->size[0] = in4->size[0];
  emxEnsureCapacity_real_T(c_in4, i);
  b_in4_data = c_in4->data;
  loop_ub = in4->size[0];
  for (i = 0; i < loop_ub; i++) {
    b_in4_data[i] = in4_data[i + in4->size[0] * in5];
  }
  TwoAnchPos3(in1, in3, b_in4, b_in1_data, b_in1_size, c_in4, in7, in8);
  emxFree_real_T(&c_in4);
}

/*
 * Arguments    : double u0
 *                double u1
 * Return Type  : double
 */
static double rt_atan2d_snf(double u0, double u1)
{
  double y;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = rtNaN;
  } else if (rtIsInf(u0) && rtIsInf(u1)) {
    int i;
    int i1;
    if (u0 > 0.0) {
      i = 1;
    } else {
      i = -1;
    }
    if (u1 > 0.0) {
      i1 = 1;
    } else {
      i1 = -1;
    }
    y = atan2(i, i1);
  } else if (u1 == 0.0) {
    if (u0 > 0.0) {
      y = RT_PI / 2.0;
    } else if (u0 < 0.0) {
      y = -(RT_PI / 2.0);
    } else {
      y = 0.0;
    }
  } else {
    y = atan2(u0, u1);
  }
  return y;
}

/*
 * Arguments    : const double xa_data[]
 *                const int xa_size[2]
 *                const double ya_data[]
 *                const int ya_size[2]
 *                const emxArray_real_T *dist
 *                const creal_T tag_pos_b[4]
 *                double Ln
 *                const creal_T b_PrevPos[4]
 *                double b_PrevHeading
 *                creal_T tag_pos_est[4]
 *                double *heading_est
 *                double *ErrSum
 * Return Type  : void
 */
void GetInitPos3(const double xa_data[], const int xa_size[2],
                 const double ya_data[], const int ya_size[2],
                 const emxArray_real_T *dist, const creal_T tag_pos_b[4],
                 double Ln, const creal_T b_PrevPos[4], double b_PrevHeading,
                 creal_T tag_pos_est[4], double *heading_est, double *ErrSum)
{
  emxArray_creal_T *Pos2C;
  emxArray_int32_T *b_r;
  emxArray_int32_T *iidx;
  emxArray_real_T *b_dist;
  emxArray_real_T *dist1;
  emxArray_real_T *disth;
  creal_T TempH[16];
  creal_T lP[4];
  creal_T Temp[3];
  creal_T b;
  creal_T x;
  creal_T *Pos2C_data;
  double TempAbsH[16];
  double b_x[16];
  double Pos2[4];
  double DistPrev[3];
  double Pos3[2];
  const double *dist_data;
  double Cand_Err;
  double Cand_HeadingDiff;
  double Sel_C_im;
  double Sel_C_re;
  double Sel_Err;
  double Sel_ErrH;
  double Sel_HErr;
  double Sel_H_im;
  double Sel_H_re;
  double Sel_HeadingDiff;
  double Sel_PosDiff;
  double im_tmp;
  double *b_dist_data;
  double *dist1_data;
  double *disth_data;
  int SelectedPos[32];
  int NZeroId_data[4];
  int b_xa_size[2];
  int PPI;
  int b_i;
  int cnt;
  int i;
  int i1;
  int i2;
  int i3;
  int i4;
  int i5;
  int l;
  int l1;
  int l2;
  int l3;
  int l4;
  int loop_ub;
  int ork;
  int *iidx_data;
  int *r1;
  signed char Pos2CL[4];
  dist_data = dist->data;
  emxInit_creal_T(&Pos2C, 2);
  i = Pos2C->size[0] * Pos2C->size[1];
  Pos2C->size[0] = 4;
  i1 = (int)(2.0 * (Ln * (Ln - 1.0) / 2.0));
  Pos2C->size[1] = i1;
  emxEnsureCapacity_creal_T(Pos2C, i);
  Pos2C_data = Pos2C->data;
  loop_ub = i1 << 2;
  for (i = 0; i < loop_ub; i++) {
    Pos2C_data[i].re = 0.0;
    Pos2C_data[i].im = 0.0;
  }
  Pos2CL[0] = 0;
  Pos2CL[1] = 0;
  Pos2CL[2] = 0;
  Pos2CL[3] = 0;
  Sel_C_re = 0.0;
  Sel_C_im = 0.0;
  for (i = 0; i < 16; i++) {
    TempH[i].re = 0.0;
    TempH[i].im = 0.0;
    TempAbsH[i] = 1000.0;
  }
  Temp[2].re = 0.0;
  Temp[2].im = 0.0;
  /*  */
  emxInit_real_T(&disth, 2);
  i = disth->size[0] * disth->size[1];
  disth->size[0] = dist->size[0];
  disth->size[1] = dist->size[1];
  emxEnsureCapacity_real_T(disth, i);
  disth_data = disth->data;
  loop_ub = dist->size[0] * dist->size[1];
  for (i = 0; i < loop_ub; i++) {
    disth_data[i] = dist_data[i];
  }
  PPI = dist->size[0] * dist->size[1] - 1;
  l = 0;
  for (b_i = 0; b_i <= PPI; b_i++) {
    if (dist_data[b_i] == 0.0) {
      l++;
    }
  }
  emxInit_int32_T(&iidx);
  i = iidx->size[0];
  iidx->size[0] = l;
  emxEnsureCapacity_int32_T(iidx, i);
  iidx_data = iidx->data;
  l = 0;
  for (b_i = 0; b_i <= PPI; b_i++) {
    if (dist_data[b_i] == 0.0) {
      iidx_data[l] = b_i + 1;
      l++;
    }
  }
  loop_ub = iidx->size[0];
  for (i = 0; i < loop_ub; i++) {
    disth_data[iidx_data[i] - 1] = 100.0;
  }
  loop_ub = dist->size[0];
  emxInit_real_T(&dist1, 1);
  emxInit_int32_T(&b_r);
  emxInit_real_T(&b_dist, 1);
  for (cnt = 0; cnt < 4; cnt++) {
    i = b_dist->size[0];
    b_dist->size[0] = dist->size[0];
    emxEnsureCapacity_real_T(b_dist, i);
    b_dist_data = b_dist->data;
    for (i = 0; i < loop_ub; i++) {
      b_dist_data[i] = dist_data[i + dist->size[0] * cnt];
    }
    if (sum(b_dist) != 0.0) {
      l = disth->size[0];
      i = dist1->size[0];
      dist1->size[0] = disth->size[0];
      emxEnsureCapacity_real_T(dist1, i);
      dist1_data = dist1->data;
      for (i = 0; i < l; i++) {
        dist1_data[i] = disth_data[i + disth->size[0] * cnt];
      }
      sort(dist1, iidx);
      iidx_data = iidx->data;
      dist1_data = dist1->data;
      l = xa_size[1];
      if (xa_size[1] == ya_size[1]) {
        creal_T b_xa_data[6];
        double c_dist[2];
        double xa[2];
        double ya[2];
        xa[0] = xa_data[iidx_data[0] - 1];
        xa[1] = xa_data[iidx_data[1] - 1];
        ya[0] = ya_data[iidx_data[0] - 1];
        ya[1] = ya_data[iidx_data[1] - 1];
        c_dist[0] = dist_data[(iidx_data[0] + dist->size[0] * cnt) - 1];
        c_dist[1] = dist_data[(iidx_data[1] + dist->size[0] * cnt) - 1];
        b_xa_size[0] = 1;
        b_xa_size[1] = xa_size[1];
        for (i = 0; i < l; i++) {
          im_tmp = ya_data[i];
          b_xa_data[i].re = xa_data[i] + 0.0 * im_tmp;
          b_xa_data[i].im = im_tmp;
        }
        i = b_dist->size[0];
        b_dist->size[0] = dist->size[0];
        emxEnsureCapacity_real_T(b_dist, i);
        b_dist_data = b_dist->data;
        l = dist->size[0];
        for (i = 0; i < l; i++) {
          b_dist_data[i] = dist_data[i + dist->size[0] * cnt];
        }
        TwoAnchPos3(xa, ya, c_dist, b_xa_data, b_xa_size, b_dist, Pos2, Pos3);
      } else {
        b_binary_expand_op(xa_data, xa_size, iidx, ya_data, ya_size, dist, cnt,
                           Pos2, Pos3);
      }
      PPI = dist1->size[0] - 1;
      l = 0;
      for (b_i = 0; b_i <= PPI; b_i++) {
        if (dist1_data[b_i] != 0.0) {
          l++;
        }
      }
      i = b_r->size[0];
      b_r->size[0] = l;
      emxEnsureCapacity_int32_T(b_r, i);
      r1 = b_r->data;
      l = 0;
      for (b_i = 0; b_i <= PPI; b_i++) {
        if (dist1_data[b_i] != 0.0) {
          r1[l] = b_i + 1;
          l++;
        }
      }
      if (b_r->size[0] > 2) {
        if (dist1_data[3] != 0.0) {
          double Prob3[4];
          double c_xa[4];
          double c_ya[4];
          double e_dist[4];
          /* && (dist1(4)<25) */
          c_xa[0] = xa_data[iidx_data[0] - 1];
          c_ya[0] = ya_data[iidx_data[0] - 1];
          e_dist[0] = dist_data[(iidx_data[0] + dist->size[0] * cnt) - 1];
          c_xa[1] = xa_data[iidx_data[1] - 1];
          c_ya[1] = ya_data[iidx_data[1] - 1];
          e_dist[1] = dist_data[(iidx_data[1] + dist->size[0] * cnt) - 1];
          c_xa[2] = xa_data[iidx_data[2] - 1];
          c_ya[2] = ya_data[iidx_data[2] - 1];
          e_dist[2] = dist_data[(iidx_data[2] + dist->size[0] * cnt) - 1];
          c_xa[3] = xa_data[iidx_data[3] - 1];
          c_ya[3] = ya_data[iidx_data[3] - 1];
          e_dist[3] = dist_data[(iidx_data[3] + dist->size[0] * cnt) - 1];
          ThreeAnchPos3(c_xa, c_ya, e_dist, Pos3, Prob3);
        } else {
          double b_xa[3];
          double b_ya[3];
          double d_dist[3];
          b_xa[0] = xa_data[iidx_data[0] - 1];
          b_ya[0] = ya_data[iidx_data[0] - 1];
          d_dist[0] = dist_data[(iidx_data[0] + dist->size[0] * cnt) - 1];
          b_xa[1] = xa_data[iidx_data[1] - 1];
          b_ya[1] = ya_data[iidx_data[1] - 1];
          d_dist[1] = dist_data[(iidx_data[1] + dist->size[0] * cnt) - 1];
          b_xa[2] = xa_data[iidx_data[2] - 1];
          b_ya[2] = ya_data[iidx_data[2] - 1];
          d_dist[2] = dist_data[(iidx_data[2] + dist->size[0] * cnt) - 1];
          b_ThreeAnchPos3(b_xa, b_ya, d_dist, Pos3, DistPrev);
        }
        Temp[0].re = Pos3[0];
        Temp[0].im = Pos3[1];
        Temp[1].re = Pos2[0];
        Temp[1].im = Pos2[2];
        Temp[2].re = Pos2[1];
        Temp[2].im = Pos2[3];
        Cand_HeadingDiff = b_PrevPos[cnt].re;
        im_tmp = b_PrevPos[cnt].im;
        DistPrev[0] =
            rt_hypotd_snf(Pos3[0] - Cand_HeadingDiff, Pos3[1] - im_tmp);
        DistPrev[1] =
            rt_hypotd_snf(Pos2[0] - Cand_HeadingDiff, Pos2[2] - im_tmp);
        DistPrev[2] =
            rt_hypotd_snf(Pos2[1] - Cand_HeadingDiff, Pos2[3] - im_tmp);
        minimum(DistPrev, &Cand_Err, &l);
        DistPrev[0] = rt_hypotd_snf(Pos3[0] - PrevUWBpos[cnt].re,
                                    Pos3[1] - PrevUWBpos[cnt].im);
        DistPrev[1] = rt_hypotd_snf(Pos2[0] - PrevUWBpos[cnt].re,
                                    Pos2[2] - PrevUWBpos[cnt].im);
        DistPrev[2] = rt_hypotd_snf(Pos2[1] - PrevUWBpos[cnt].re,
                                    Pos2[3] - PrevUWBpos[cnt].im);
        minimum(DistPrev, &Cand_HeadingDiff, &PPI);
        if (l != PPI) {
          if ((Cand_Err < 1.0) && (Cand_HeadingDiff < 1.0)) {
            Pos2C_data[cnt] = Temp[l - 1];
            Pos2C_data[cnt + 4] = Temp[PPI - 1];
            l = 2;
          } else if ((Cand_Err < 1.0) && (Cand_HeadingDiff >= 1.0)) {
            Pos2C_data[cnt] = Temp[l - 1];
            l = 1;
          } else if ((Cand_Err >= 1.0) && (Cand_HeadingDiff < 1.0)) {
            Pos2C_data[cnt] = Temp[PPI - 1];
            l = 1;
          } else {
            Pos2C_data[cnt] = Temp[0];
            Pos2C_data[cnt + 4] = Temp[1];
            Pos2C_data[cnt + 8] = Temp[2];
            l = 3;
          }
        } else {
          Pos2C_data[cnt] = Temp[l - 1];
          /*                  Pos2C(Pn,l+1) = Temp(2); */
          /*                  Pos2C(Pn,l+2) = Temp(3); */
          l = 1;
        }
      } else {
        Temp[0].re = Pos2[0];
        Temp[0].im = Pos2[2];
        Temp[1].re = Pos2[1];
        Temp[1].im = Pos2[3];
        Cand_HeadingDiff = b_PrevPos[cnt].re;
        im_tmp = b_PrevPos[cnt].im;
        DistPrev[0] =
            rt_hypotd_snf(Pos2[0] - Cand_HeadingDiff, Pos2[2] - im_tmp);
        DistPrev[1] =
            rt_hypotd_snf(Pos2[1] - Cand_HeadingDiff, Pos2[3] - im_tmp);
        DistPrev[2] =
            rt_hypotd_snf(Temp[2].re - Cand_HeadingDiff, Temp[2].im - im_tmp);
        minimum(DistPrev, &Cand_Err, &l);
        Cand_HeadingDiff = PrevUWBpos[cnt].re;
        im_tmp = PrevUWBpos[cnt].im;
        DistPrev[0] =
            rt_hypotd_snf(Pos2[0] - Cand_HeadingDiff, Pos2[2] - im_tmp);
        DistPrev[1] =
            rt_hypotd_snf(Pos2[1] - Cand_HeadingDiff, Pos2[3] - im_tmp);
        DistPrev[2] =
            rt_hypotd_snf(Temp[2].re - Cand_HeadingDiff, Temp[2].im - im_tmp);
        minimum(DistPrev, &Cand_HeadingDiff, &PPI);
        if (l != PPI) {
          if ((Cand_Err < 1.0) && (Cand_HeadingDiff < 1.0)) {
            Pos2C_data[cnt] = Temp[l - 1];
            Pos2C_data[cnt + 4] = Temp[PPI - 1];
            l = 2;
          } else if ((Cand_Err < 1.0) && (Cand_HeadingDiff >= 1.0)) {
            Pos2C_data[cnt] = Temp[l - 1];
            l = 1;
          } else if ((Cand_Err >= 1.0) && (Cand_HeadingDiff < 1.0)) {
            Pos2C_data[cnt] = Temp[PPI - 1];
            l = 1;
          } else {
            Pos2C_data[cnt] = Temp[0];
            Pos2C_data[cnt + 4] = Temp[1];
            l = 2;
          }
        } else {
          Pos2C_data[cnt] = Temp[0];
          l = 1;
        }
      }
      Pos2CL[cnt] = (signed char)l;
    }
  }
  bool b_Pos2CL[4];
  emxFree_real_T(&b_dist);
  emxFree_int32_T(&iidx);
  emxFree_int32_T(&b_r);
  emxFree_real_T(&dist1);
  emxFree_real_T(&disth);
  /*  for Pn = 1 : Lp */
  /*      l = 1; */
  /*      if sum(dist(:,Pn))~=0 */
  /*          [dist1, L1i] = sort(disth(:,Pn)); */
  /*          L1 = L1i(1); */
  /*          L2 = L1i(2); */
  /*           */
  /*          [Pos2,Prob2]=TwoAnchPos3([xa(L1) xa(L2)], [ya(L1) ya(L2)],
   * [dist(L1,Pn) dist(L2,Pn)],[0 0],[0 0],xa+j*ya,dist(:,Pn)); */
  /*   */
  /*          if length(dist(dist1~=0)) > 2 */
  /*              if (dist1(3)~=0) %&& (dist1(3)<25) */
  /*                  L3 = L1i(3); */
  /*                  if (dist1(4)~=0) %&& (dist1(4)<25) */
  /*                      L4 = L1i(4); */
  /*                      [Pos3,Prob3]=ThreeAnchPos3([xa(L1i(1:4))],
   * [ya(L1i(1:4))], [dist(L1i(1:4),Pn)]); */
  /*                  else */
  /*                      [Pos3,Prob3]=ThreeAnchPos3([xa(L1i(1:3))],
   * [ya(L1i(1:3))], [dist(L1i(1:3),Pn)]); */
  /*                  end */
  /*                  Pos2_C = Pos2(:,1) + j*Pos2(:,2); */
  /*                  Pos3_C = Pos3(1) + j*Pos3(2); */
  /*   */
  /*                  d_1 = abs(Pos2_C(1)-Pos3_C); */
  /*                  d_2 = abs(Pos2_C(2)-Pos3_C); */
  /*   */
  /*                  if (abs(d_1-d_2) > 1)  */
  /*   */
  /*                      if d_1<d_2 */
  /*                          Pos2C(Pn,l) = Pos2(1,1)+j*Pos2(1,2); */
  /*                      else */
  /*                          Pos2C(Pn,l) = Pos2(2,1)+j*Pos2(2,2); */
  /*                      end */
  /*                       */
  /*   */
  /*                  else */
  /*                      Pos2C(Pn,l) = Pos2(1,1)+j*Pos2(1,2); */
  /*                      Pos2C(Pn,l+1) = Pos2(2,1)+j*Pos2(2,2); */
  /*                      l = 2; */
  /*                  end */
  /*              else */
  /*                  Pos2C(Pn,l) = Pos2(1,1)+j*Pos2(1,2); */
  /*                  Pos2C(Pn,l+1) = Pos2(2,1)+j*Pos2(2,2); */
  /*                  l = 2; */
  /*              end */
  /*          else */
  /*              Pos2C(Pn,l) = Pos2(1,1)+j*Pos2(1,2); */
  /*              Pos2C(Pn,l+1) = Pos2(2,1)+j*Pos2(2,2); */
  /*              l = 2; */
  /*          end */
  /*   */
  /*          Pos2CL(Pn) = l; */
  /*   */
  /*      end */
  /*   */
  /*  end */
  b_Pos2CL[0] = (Pos2CL[0] != 0);
  b_Pos2CL[1] = (Pos2CL[1] != 0);
  b_Pos2CL[2] = (Pos2CL[2] != 0);
  b_Pos2CL[3] = (Pos2CL[3] != 0);
  eml_find(b_Pos2CL, NZeroId_data, b_xa_size);
  Sel_H_re = 0.0;
  Sel_H_im = 0.0;
  Sel_HErr = 1.0E+6;
  Sel_Err = 1.0E+6;
  Sel_ErrH = 1.0E+6;
  Sel_PosDiff = 1.0E+6;
  Sel_HeadingDiff = 1.0E+6;
  *ErrSum = 1.0E+6;
  /* Ld = min(Pos2Prob(:))*5; */
  i = (int)fmax(1.0, Pos2CL[0]);
  i1 = (int)fmax(1.0, Pos2CL[1]);
  i2 = (int)fmax(1.0, Pos2CL[2]);
  i3 = (int)fmax(1.0, Pos2CL[3]);
  i4 = b_xa_size[1];
  for (l1 = 0; l1 < i; l1++) {
    if (Pos2CL[0] != 0) {
      lP[0] = Pos2C_data[4 * l1];
    } else {
      lP[0].re = 0.0;
      lP[0].im = 0.0;
    }
    for (l2 = 0; l2 < i1; l2++) {
      if (Pos2CL[1] != 0) {
        lP[1] = Pos2C_data[4 * l2 + 1];
      } else {
        lP[1].re = 0.0;
        lP[1].im = 0.0;
      }
      for (l3 = 0; l3 < i2; l3++) {
        if (Pos2CL[2] != 0) {
          lP[2] = Pos2C_data[4 * l3 + 2];
        } else {
          lP[2].re = 0.0;
          lP[2].im = 0.0;
        }
        for (l4 = 0; l4 < i3; l4++) {
          double Cand_ErrH;
          double Cand_HErr;
          double Cand_PosDiff;
          double PrevPos_im;
          double PrevPos_re;
          int b_iidx[16];
          if (Pos2CL[3] != 0) {
            lP[3] = Pos2C_data[4 * l4 + 3];
          } else {
            lP[3].re = 0.0;
            lP[3].im = 0.0;
          }
          cnt = -1;
          x.re = ((b_PrevPos[0].re + b_PrevPos[1].re) + b_PrevPos[2].re) +
                 b_PrevPos[3].re;
          x.im = ((b_PrevPos[0].im + b_PrevPos[1].im) + b_PrevPos[2].im) +
                 b_PrevPos[3].im;
          if (x.im == 0.0) {
            PrevPos_re = x.re / 4.0;
            PrevPos_im = 0.0;
          } else if (x.re == 0.0) {
            PrevPos_re = 0.0;
            PrevPos_im = x.im / 4.0;
          } else {
            PrevPos_re = x.re / 4.0;
            PrevPos_im = x.im / 4.0;
          }
          if ((PrevPos_re != 0.0) || (PrevPos_im != 0.0)) {
            x.re = ((lP[0].re + lP[1].re) + lP[2].re) + lP[3].re;
            x.im = ((lP[0].im + lP[1].im) + lP[2].im) + lP[3].im;
            if (x.im == 0.0) {
              Cand_HeadingDiff = x.re / 4.0;
              im_tmp = 0.0;
            } else if (x.re == 0.0) {
              Cand_HeadingDiff = 0.0;
              im_tmp = x.im / 4.0;
            } else {
              Cand_HeadingDiff = x.re / 4.0;
              im_tmp = x.im / 4.0;
            }
            Cand_PosDiff = rt_hypotd_snf(Cand_HeadingDiff - PrevPos_re,
                                         im_tmp - PrevPos_im) /
                           10.0;
          } else {
            Cand_PosDiff = 0.0;
          }
          memset(&SelectedPos[0], 0, 32U * sizeof(int));
          for (b_i = 0; b_i < i4; b_i++) {
            PPI = NZeroId_data[b_i] - 1;
            i5 = b_xa_size[1] - b_i;
            for (ork = 0; ork <= i5 - 2; ork++) {
              double bi_tmp;
              loop_ub = NZeroId_data[(b_i + ork) + 1];
              Cand_Err = lP[PPI].re - lP[loop_ub - 1].re;
              Cand_ErrH = lP[PPI].im - lP[loop_ub - 1].im;
              Cand_HErr = tag_pos_b[PPI].re - tag_pos_b[loop_ub - 1].re;
              bi_tmp = tag_pos_b[PPI].im - tag_pos_b[loop_ub - 1].im;
              if (bi_tmp == 0.0) {
                if (Cand_ErrH == 0.0) {
                  l = (cnt + ork) + 1;
                  im_tmp = Cand_Err / Cand_HErr;
                  TempH[l].re = im_tmp;
                  TempH[l].im = 0.0;
                  x.re = im_tmp;
                  x.im = 0.0;
                } else if (Cand_Err == 0.0) {
                  l = (cnt + ork) + 1;
                  TempH[l].re = 0.0;
                  TempH[l].im = Cand_ErrH / Cand_HErr;
                  x.re = 0.0;
                  x.im = Cand_ErrH / Cand_HErr;
                } else {
                  l = (cnt + ork) + 1;
                  TempH[l].re = Cand_Err / Cand_HErr;
                  TempH[l].im = Cand_ErrH / Cand_HErr;
                  x.re = Cand_Err / Cand_HErr;
                  x.im = Cand_ErrH / Cand_HErr;
                }
              } else if (Cand_HErr == 0.0) {
                if (Cand_Err == 0.0) {
                  l = (cnt + ork) + 1;
                  TempH[l].re = Cand_ErrH / bi_tmp;
                  TempH[l].im = 0.0;
                  x.re = Cand_ErrH / bi_tmp;
                  x.im = 0.0;
                } else {
                  if (Cand_ErrH == 0.0) {
                    l = (cnt + ork) + 1;
                    TempH[l].re = 0.0;
                    TempH[l].im = -(Cand_Err / bi_tmp);
                  } else {
                    l = (cnt + ork) + 1;
                    TempH[l].re = Cand_ErrH / bi_tmp;
                    TempH[l].im = -(Cand_Err / bi_tmp);
                  }
                  if (Cand_ErrH == 0.0) {
                    x.re = 0.0;
                    x.im = -(Cand_Err / bi_tmp);
                  } else {
                    x.re = Cand_ErrH / bi_tmp;
                    x.im = -(Cand_Err / bi_tmp);
                  }
                }
              } else {
                double bim_tmp;
                double brm_tmp;
                brm_tmp = fabs(Cand_HErr);
                bim_tmp = fabs(bi_tmp);
                if (brm_tmp > bim_tmp) {
                  im_tmp = bi_tmp / Cand_HErr;
                  Cand_HeadingDiff = Cand_HErr + im_tmp * bi_tmp;
                  l = (cnt + ork) + 1;
                  TempH[l].re =
                      (Cand_Err + im_tmp * Cand_ErrH) / Cand_HeadingDiff;
                  TempH[l].im =
                      (Cand_ErrH - im_tmp * Cand_Err) / Cand_HeadingDiff;
                  im_tmp = bi_tmp / Cand_HErr;
                  Cand_HeadingDiff = Cand_HErr + im_tmp * bi_tmp;
                  x.re = (Cand_Err + im_tmp * Cand_ErrH) / Cand_HeadingDiff;
                  x.im = (Cand_ErrH - im_tmp * Cand_Err) / Cand_HeadingDiff;
                } else {
                  if (bim_tmp == brm_tmp) {
                    if (Cand_HErr > 0.0) {
                      im_tmp = 0.5;
                    } else {
                      im_tmp = -0.5;
                    }
                    if (bi_tmp > 0.0) {
                      Cand_HeadingDiff = 0.5;
                    } else {
                      Cand_HeadingDiff = -0.5;
                    }
                    l = (cnt + ork) + 1;
                    TempH[l].re =
                        (Cand_Err * im_tmp + Cand_ErrH * Cand_HeadingDiff) /
                        brm_tmp;
                    TempH[l].im =
                        (Cand_ErrH * im_tmp - Cand_Err * Cand_HeadingDiff) /
                        brm_tmp;
                  } else {
                    im_tmp = Cand_HErr / bi_tmp;
                    Cand_HeadingDiff = bi_tmp + im_tmp * Cand_HErr;
                    l = (cnt + ork) + 1;
                    TempH[l].re =
                        (im_tmp * Cand_Err + Cand_ErrH) / Cand_HeadingDiff;
                    TempH[l].im =
                        (im_tmp * Cand_ErrH - Cand_Err) / Cand_HeadingDiff;
                  }
                  if (bim_tmp == brm_tmp) {
                    if (Cand_HErr > 0.0) {
                      im_tmp = 0.5;
                    } else {
                      im_tmp = -0.5;
                    }
                    if (bi_tmp > 0.0) {
                      Cand_HeadingDiff = 0.5;
                    } else {
                      Cand_HeadingDiff = -0.5;
                    }
                    x.re = (Cand_Err * im_tmp + Cand_ErrH * Cand_HeadingDiff) /
                           brm_tmp;
                    x.im = (Cand_ErrH * im_tmp - Cand_Err * Cand_HeadingDiff) /
                           brm_tmp;
                  } else {
                    im_tmp = Cand_HErr / bi_tmp;
                    Cand_HeadingDiff = bi_tmp + im_tmp * Cand_HErr;
                    x.re = (im_tmp * Cand_Err + Cand_ErrH) / Cand_HeadingDiff;
                    x.im = (im_tmp * Cand_ErrH - Cand_Err) / Cand_HeadingDiff;
                  }
                }
              }
              Cand_HeadingDiff = fabs(rt_hypotd_snf(x.re, x.im) - 1.0);
              l = (cnt + ork) + 1;
              TempAbsH[l] = Cand_HeadingDiff * Cand_HeadingDiff;
              SelectedPos[l] = PPI + 1;
              SelectedPos[(cnt + ork) + 17] = loop_ub;
            }
            cnt = (cnt + i5) - 1;
          }
          memcpy(&b_x[0], &TempAbsH[0], 16U * sizeof(double));
          b_sort(b_x, b_iidx);
          for (i5 = 0; i5 < 16; i5++) {
            b_x[i5] = b_iidx[i5];
          }
          creal_T Temp_tmp;
          creal_T b_Temp_tmp;
          PPI = (int)b_x[0];
          loop_ub = (int)b_x[1];
          b_i = (int)b_x[2];
          Cand_Err = ((TempAbsH[PPI - 1] + TempAbsH[loop_ub - 1]) +
                      TempAbsH[b_i - 1]) /
                     3.0;
          b = TempH[PPI - 1];
          Temp[0] = b;
          Temp_tmp = TempH[loop_ub - 1];
          Temp[1] = Temp_tmp;
          b_Temp_tmp = TempH[b_i - 1];
          Temp[2] = b_Temp_tmp;
          x = mean(Temp);
          Cand_ErrH = fabs(rt_hypotd_snf(x.re, x.im) - 1.0);
          Temp[0] = b;
          Temp[1] = Temp_tmp;
          Temp[2] = b_Temp_tmp;
          b = mean(Temp);
          im_tmp = fabs(TempH[PPI - 1].re * -b.im + TempH[PPI - 1].im * b.re);
          DistPrev[0] = im_tmp * im_tmp;
          im_tmp = fabs(TempH[loop_ub - 1].re * -b.im +
                        TempH[loop_ub - 1].im * b.re);
          DistPrev[1] = im_tmp * im_tmp;
          im_tmp = fabs(TempH[b_i - 1].re * -b.im + TempH[b_i - 1].im * b.re);
          Cand_HErr = ((DistPrev[0] + DistPrev[1]) + im_tmp * im_tmp) / 3.0;
          if ((PrevPos_re != 0.0) || (PrevPos_im != 0.0)) {
            Cand_HeadingDiff =
                fabs(b_mod(rt_atan2d_snf(x.im, x.re)) - b_mod(b_PrevHeading));
            Cand_HeadingDiff =
                fmin(Cand_HeadingDiff,
                     fabs(Cand_HeadingDiff - 6.2831853071795862)) /
                30.0;
          } else {
            Cand_HeadingDiff = 0.0;
          }
          /* if ((Cand_HErr + Cand_Err + Cand_ErrH + Cand_HeadingDiff) <
           * (Sel_HErr + Sel_Err + Sel_ErrH+ Sel_HeadingDiff )) && (cnt~=0) % &&
           * (Cand_PosDiff < 10) */
          /* if ((Cand_HErr + Cand_Err + Cand_ErrH) < (Sel_HErr + Sel_Err +
           * Sel_ErrH  )) && (cnt~=0) % && (Cand_PosDiff < 10) */
          im_tmp = (((Cand_HErr + Cand_Err) + Cand_ErrH) + Cand_PosDiff) +
                   Cand_HeadingDiff;
          if ((im_tmp < (((Sel_HErr + Sel_Err) + Sel_ErrH) + Sel_PosDiff) +
                            Sel_HeadingDiff) &&
              (cnt + 1 != 0)) {
            /*  && (Cand_PosDiff < 10) */
            /* if ((Cand_HErr + Cand_Err + Cand_ErrH + Cand_PosDiff +
             * Cand_HeadingDiff + DistErr + DistAcc) < (Sel_HErr + Sel_Err +
             * Sel_ErrH + Sel_PosDiff + Sel_HeadingDiff + Sel_DistErr  +
             * Sel_DistAcc)) && (cnt~=0) % && (Cand_PosDiff < 10) */
            Sel_H_re = x.re;
            Sel_H_im = x.im;
            Sel_HErr = Cand_HErr;
            Sel_Err = Cand_Err;
            Sel_ErrH = Cand_ErrH;
            /* ErrSum = Cand_HErr + Cand_Err + Cand_ErrH ; */
            *ErrSum = im_tmp;
            Sel_PosDiff = Cand_PosDiff;
            Sel_HeadingDiff = Cand_HeadingDiff;
            /*                                      Sel_DistErr = DistErr; */
            /*                                      Sel_DistAcc = DistAcc; */
            l = SelectedPos[PPI - 1] - 1;
            Cand_HeadingDiff = lP[l].re;
            im_tmp = lP[l].im;
            if (im_tmp == 0.0) {
              Cand_ErrH = Cand_HeadingDiff / 6.0;
              Cand_HErr = 0.0;
            } else if (Cand_HeadingDiff == 0.0) {
              Cand_ErrH = 0.0;
              Cand_HErr = im_tmp / 6.0;
            } else {
              Cand_ErrH = Cand_HeadingDiff / 6.0;
              Cand_HErr = im_tmp / 6.0;
            }
            l = SelectedPos[PPI + 15] - 1;
            Cand_HeadingDiff = lP[l].re;
            im_tmp = lP[l].im;
            if (im_tmp == 0.0) {
              Cand_Err = Cand_HeadingDiff / 6.0;
              Cand_HeadingDiff = 0.0;
            } else if (Cand_HeadingDiff == 0.0) {
              Cand_Err = 0.0;
              Cand_HeadingDiff = im_tmp / 6.0;
            } else {
              Cand_Err = Cand_HeadingDiff / 6.0;
              Cand_HeadingDiff = im_tmp / 6.0;
            }
            Sel_C_re = Cand_ErrH + Cand_Err;
            Sel_C_im = Cand_HErr + Cand_HeadingDiff;
            l = SelectedPos[loop_ub - 1] - 1;
            Cand_HeadingDiff = lP[l].re;
            im_tmp = lP[l].im;
            if (im_tmp == 0.0) {
              Cand_ErrH = Cand_HeadingDiff / 6.0;
              Cand_HErr = 0.0;
            } else if (Cand_HeadingDiff == 0.0) {
              Cand_ErrH = 0.0;
              Cand_HErr = im_tmp / 6.0;
            } else {
              Cand_ErrH = Cand_HeadingDiff / 6.0;
              Cand_HErr = im_tmp / 6.0;
            }
            l = SelectedPos[loop_ub + 15] - 1;
            Cand_HeadingDiff = lP[l].re;
            im_tmp = lP[l].im;
            if (im_tmp == 0.0) {
              Cand_Err = Cand_HeadingDiff / 6.0;
              Cand_HeadingDiff = 0.0;
            } else if (Cand_HeadingDiff == 0.0) {
              Cand_Err = 0.0;
              Cand_HeadingDiff = im_tmp / 6.0;
            } else {
              Cand_Err = Cand_HeadingDiff / 6.0;
              Cand_HeadingDiff = im_tmp / 6.0;
            }
            Sel_C_re = (Sel_C_re + Cand_ErrH) + Cand_Err;
            Sel_C_im = (Sel_C_im + Cand_HErr) + Cand_HeadingDiff;
            l = SelectedPos[b_i - 1] - 1;
            Cand_HeadingDiff = lP[l].re;
            im_tmp = lP[l].im;
            if (im_tmp == 0.0) {
              Cand_ErrH = Cand_HeadingDiff / 6.0;
              Cand_HErr = 0.0;
            } else if (Cand_HeadingDiff == 0.0) {
              Cand_ErrH = 0.0;
              Cand_HErr = im_tmp / 6.0;
            } else {
              Cand_ErrH = Cand_HeadingDiff / 6.0;
              Cand_HErr = im_tmp / 6.0;
            }
            l = SelectedPos[b_i + 15] - 1;
            Cand_HeadingDiff = lP[l].re;
            im_tmp = lP[l].im;
            if (im_tmp == 0.0) {
              Cand_Err = Cand_HeadingDiff / 6.0;
              Cand_HeadingDiff = 0.0;
            } else if (Cand_HeadingDiff == 0.0) {
              Cand_Err = 0.0;
              Cand_HeadingDiff = im_tmp / 6.0;
            } else {
              Cand_Err = Cand_HeadingDiff / 6.0;
              Cand_HeadingDiff = im_tmp / 6.0;
            }
            Sel_C_re = (Sel_C_re + Cand_ErrH) + Cand_Err;
            Sel_C_im = (Sel_C_im + Cand_HErr) + Cand_HeadingDiff;
            /*                      for dfg = 1 : length(NZeroId) */
            /*                          Sel_C = Sel_C +
             * (lP(NZeroId(dfg))-exp(j*angle(Sel_H))*tag_pos_b(NZeroId(dfg)))/length(NZeroId);
             */
            /*                      end */
          }
        }
        /*                          end */
        /*                      end */
        /*                  end */
        /*              end */
      }
    }
  }
  emxFree_creal_T(&Pos2C);
  /*  */
  /*  for L1 = 1 : Ln */
  /*      for L2 = L1 + 1 : Ln */
  /*          for Pn = 1 : Lp */
  /*              if (dist(L1,Pn)~=0) && (dist(L2,Pn)~=0) */
  /*                  [Pos2,Prob2]=TwoAnchPos3([xa(L1) xa(L2)], [ya(L1) ya(L2)],
   * [dist(L1,Pn) dist(L2,Pn)],[0 0],[0 0],xa+j*ya,dist(:,Pn)); */
  /*                  Pos2C(Pn,1) = Pos2(1,1)+j*Pos2(1,2); */
  /*                  Pos2C(Pn,2) = Pos2(2,1)+j*Pos2(2,2); */
  /*              else */
  /*                  Pos2C(Pn,1) = 0; */
  /*                  Pos2C(Pn,2) = 0; */
  /*              end */
  /*          end */
  /*          for Qn = 0 : 2^Lp-1 */
  /*              Va = dec2bin(Qn,Lp); */
  /*              [Est_F(Qn+1),Est_C(Qn+1),Est_H(Qn+1)] =
   * GetMultiTagPosGen(Pos2C,Va,tag_pos_b,[1 1 1 1]); */
  /*          end */
  /*          [val,ind] = min(Est_F); */
  /*          Cand_F = val; */
  /*          Cand_C = Est_C(ind); */
  /*          Cand_H = Est_H(ind); */
  /*          if Sel_F > Cand_F */
  /*              Sel_F = Cand_F; */
  /*              Sel_C = Cand_C; */
  /*              Sel_H = Cand_H; */
  /*          end */
  /*      end */
  /*  end */
  *heading_est = rt_atan2d_snf(Sel_H_im, Sel_H_re);
  b.re = *heading_est * 0.0;
  b.im = *heading_est;
  b_exp(&b);
  Cand_HeadingDiff =
      Sel_C_re + (tag_pos_b[0].re * b.re - tag_pos_b[0].im * b.im);
  im_tmp = Sel_C_im + (tag_pos_b[0].re * b.im + tag_pos_b[0].im * b.re);
  tag_pos_est[0].re = Cand_HeadingDiff;
  tag_pos_est[0].im = im_tmp;
  PrevUWBpos[0].re = Cand_HeadingDiff;
  PrevUWBpos[0].im = im_tmp;
  Cand_HeadingDiff =
      Sel_C_re + (tag_pos_b[1].re * b.re - tag_pos_b[1].im * b.im);
  im_tmp = Sel_C_im + (tag_pos_b[1].re * b.im + tag_pos_b[1].im * b.re);
  tag_pos_est[1].re = Cand_HeadingDiff;
  tag_pos_est[1].im = im_tmp;
  PrevUWBpos[1].re = Cand_HeadingDiff;
  PrevUWBpos[1].im = im_tmp;
  Cand_HeadingDiff =
      Sel_C_re + (tag_pos_b[2].re * b.re - tag_pos_b[2].im * b.im);
  im_tmp = Sel_C_im + (tag_pos_b[2].re * b.im + tag_pos_b[2].im * b.re);
  tag_pos_est[2].re = Cand_HeadingDiff;
  tag_pos_est[2].im = im_tmp;
  PrevUWBpos[2].re = Cand_HeadingDiff;
  PrevUWBpos[2].im = im_tmp;
  Cand_HeadingDiff =
      Sel_C_re + (tag_pos_b[3].re * b.re - tag_pos_b[3].im * b.im);
  im_tmp = Sel_C_im + (tag_pos_b[3].re * b.im + tag_pos_b[3].im * b.re);
  tag_pos_est[3].re = Cand_HeadingDiff;
  tag_pos_est[3].im = im_tmp;
  PrevUWBpos[3].re = Cand_HeadingDiff;
  PrevUWBpos[3].im = im_tmp;
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void GetInitPos3_init(void)
{
  memset(&PrevUWBpos[0], 0, 4U * sizeof(creal_T));
}

/*
 * File trailer for GetInitPos3.c
 *
 * [EOF]
 */
