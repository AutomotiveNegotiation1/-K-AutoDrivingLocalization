//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: GetInitPos4.cpp
//
// MATLAB Coder version            : 5.6
// C/C++ source code generated on  : 17-Oct-2023 17:24:31
//

// Include Files
#include "GetInitPos4.h"
#include "GetUWBPos_v4.h"
#include "ThreeAnchPos3.h"
#include "TwoAnchPos3.h"
#include "exp.h"
#include "find.h"
#include "fusion_data.h"
#include "fusion_rtwutil.h"
#include "minOrMax.h"
#include "mod.h"
#include "rt_nonfinite.h"
#include "sort.h"
#include "sum.h"
#include "coder_array.h"
#include <algorithm>
#include <cmath>
#include <cstring>

// Variable Definitions
static creal_T PrevUWBpos[4];

// Function Declarations
static void binary_expand_op(const double in1_data[], const int in1_size[2],
                             const coder::array<int, 1U> &in2,
                             const double in3_data[], const int in3_size[2],
                             const coder::array<double, 2U> &in4, int in5,
                             double in7[4], double in8[2]);

// Function Definitions
//
// Arguments    : const double in1_data[]
//                const int in1_size[2]
//                const coder::array<int, 1U> &in2
//                const double in3_data[]
//                const int in3_size[2]
//                const coder::array<double, 2U> &in4
//                int in5
//                double in7[4]
//                double in8[2]
// Return Type  : void
//
static void binary_expand_op(const double in1_data[], const int in1_size[2],
                             const coder::array<int, 1U> &in2,
                             const double in3_data[], const int in3_size[2],
                             const coder::array<double, 2U> &in4, int in5,
                             double in7[4], double in8[2])
{
  coder::array<double, 1U> c_in4;
  creal_T b_in1_data[6];
  double b_in4[2];
  double in1[2];
  double in3[2];
  int b_in1_size[2];
  int loop_ub;
  int stride_0_1;
  int stride_1_1;
  in1[0] = in1_data[in2[0] - 1];
  in1[1] = in1_data[in2[1] - 1];
  in3[0] = in3_data[in2[0] - 1];
  in3[1] = in3_data[in2[1] - 1];
  b_in4[0] = in4[(in2[0] + in4.size(0) * in5) - 1];
  b_in4[1] = in4[(in2[1] + in4.size(0) * in5) - 1];
  b_in1_size[0] = 1;
  if (in3_size[1] == 1) {
    loop_ub = in1_size[1];
  } else {
    loop_ub = in3_size[1];
  }
  b_in1_size[1] = loop_ub;
  stride_0_1 = (in1_size[1] != 1);
  stride_1_1 = (in3_size[1] != 1);
  for (int i{0}; i < loop_ub; i++) {
    double d;
    d = in3_data[i * stride_1_1];
    b_in1_data[i].re = in1_data[i * stride_0_1] + 0.0 * d;
    b_in1_data[i].im = d;
  }
  c_in4.set_size(in4.size(0));
  loop_ub = in4.size(0);
  for (int i{0}; i < loop_ub; i++) {
    c_in4[i] = in4[i + in4.size(0) * in5];
  }
  TwoAnchPos3(in1, in3, b_in4, b_in1_data, b_in1_size, c_in4, in7, in8);
}

//
// Arguments    : const double xa_data[]
//                const int xa_size[2]
//                const double ya_data[]
//                const int ya_size[2]
//                const coder::array<double, 2U> &dist
//                const creal_T tag_pos_b[4]
//                double Ln
//                const creal_T PrevPos[4]
//                double b_PrevHeading
//                creal_T tag_pos_est[4]
//                double &ErrSum
// Return Type  : double
//
double GetInitPos4(const double xa_data[], const int xa_size[2],
                   const double ya_data[], const int ya_size[2],
                   const coder::array<double, 2U> &dist,
                   const creal_T tag_pos_b[4], double Ln,
                   const creal_T PrevPos[4], double b_PrevHeading,
                   creal_T tag_pos_est[4], double &ErrSum)
{
  coder::array<creal_T, 2U> Pos2C;
  coder::array<double, 2U> disth;
  coder::array<double, 1U> b_dist;
  coder::array<double, 1U> y;
  coder::array<int, 1U> iidx;
  creal_T TempH[16];
  creal_T lP[4];
  creal_T tag_pos_c[4];
  creal_T Temp[3];
  creal_T b_b;
  double TempAbsH[16];
  double b_y[16];
  double DistPrev[3];
  double Cand_HErr;
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
  double b;
  double brm_tmp;
  double flag;
  double heading_est;
  double vale;
  double y_re;
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
  int l;
  int loop_ub;
  signed char Pos2CL[4];
  i = static_cast<int>(2.0 * (Ln * (Ln - 1.0) / 2.0));
  Pos2C.set_size(4, i);
  loop_ub = i << 2;
  for (i = 0; i < loop_ub; i++) {
    Pos2C[i].re = 0.0;
    Pos2C[i].im = 0.0;
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
  //
  disth.set_size(dist.size(0), dist.size(1));
  l = dist.size(0) * dist.size(1);
  for (i = 0; i < l; i++) {
    disth[i] = dist[i];
  }
  l--;
  for (b_i = 0; b_i <= l; b_i++) {
    if (dist[b_i] == 0.0) {
      disth[b_i] = 100.0;
    }
  }
  Cand_HeadingDiff =
      GetUWBPos_v4(xa_data, ya_data, dist, Ln, tag_pos_b, b, Cand_HErr, flag);
  b_b.re = Cand_HErr * 0.0;
  b_b.im = Cand_HErr;
  coder::b_exp(b_b);
  y_re = Cand_HeadingDiff + 0.0 * b;
  tag_pos_c[0].re =
      y_re + (tag_pos_b[0].re * b_b.re - tag_pos_b[0].im * b_b.im);
  tag_pos_c[0].im = b + (tag_pos_b[0].re * b_b.im + tag_pos_b[0].im * b_b.re);
  tag_pos_c[1].re =
      y_re + (tag_pos_b[1].re * b_b.re - tag_pos_b[1].im * b_b.im);
  tag_pos_c[1].im = b + (tag_pos_b[1].re * b_b.im + tag_pos_b[1].im * b_b.re);
  tag_pos_c[2].re =
      y_re + (tag_pos_b[2].re * b_b.re - tag_pos_b[2].im * b_b.im);
  tag_pos_c[2].im = b + (tag_pos_b[2].re * b_b.im + tag_pos_b[2].im * b_b.re);
  tag_pos_c[3].re =
      y_re + (tag_pos_b[3].re * b_b.re - tag_pos_b[3].im * b_b.im);
  tag_pos_c[3].im = b + (tag_pos_b[3].re * b_b.im + tag_pos_b[3].im * b_b.re);
  loop_ub = dist.size(0);
  for (cnt = 0; cnt < 4; cnt++) {
    b_dist.set_size(dist.size(0));
    for (i = 0; i < loop_ub; i++) {
      b_dist[i] = dist[i + dist.size(0) * cnt];
    }
    if (coder::sum(b_dist) != 0.0) {
      double Pos2[4];
      double Pos3[2];
      l = disth.size(0);
      y.set_size(disth.size(0));
      for (i = 0; i < l; i++) {
        y[i] = disth[i + disth.size(0) * cnt];
      }
      coder::internal::sort(y, iidx);
      l = xa_size[1];
      if (xa_size[1] == ya_size[1]) {
        creal_T b_xa_data[6];
        double c_dist[2];
        double xa[2];
        double ya[2];
        xa[0] = xa_data[iidx[0] - 1];
        xa[1] = xa_data[iidx[1] - 1];
        ya[0] = ya_data[iidx[0] - 1];
        ya[1] = ya_data[iidx[1] - 1];
        c_dist[0] = dist[(iidx[0] + dist.size(0) * cnt) - 1];
        c_dist[1] = dist[(iidx[1] + dist.size(0) * cnt) - 1];
        b_xa_size[0] = 1;
        b_xa_size[1] = xa_size[1];
        for (i = 0; i < l; i++) {
          brm_tmp = ya_data[i];
          b_xa_data[i].re = xa_data[i] + 0.0 * brm_tmp;
          b_xa_data[i].im = brm_tmp;
        }
        b_dist.set_size(dist.size(0));
        l = dist.size(0);
        for (i = 0; i < l; i++) {
          b_dist[i] = dist[i + dist.size(0) * cnt];
        }
        TwoAnchPos3(xa, ya, c_dist, b_xa_data, b_xa_size, b_dist, Pos2, Pos3);
      } else {
        binary_expand_op(xa_data, xa_size, iidx, ya_data, ya_size, dist, cnt,
                         Pos2, Pos3);
      }
      l = y.size(0) - 1;
      PPI = 0;
      for (b_i = 0; b_i <= l; b_i++) {
        if (y[b_i] < 30.0) {
          PPI++;
        }
      }
      if (PPI > 2) {
        if (y[3] < 30.0) {
          double Prob3[4];
          double c_xa[4];
          double c_ya[4];
          double e_dist[4];
          // && (dist1(4)<25)
          c_xa[0] = xa_data[iidx[0] - 1];
          c_ya[0] = ya_data[iidx[0] - 1];
          e_dist[0] = dist[(iidx[0] + dist.size(0) * cnt) - 1];
          c_xa[1] = xa_data[iidx[1] - 1];
          c_ya[1] = ya_data[iidx[1] - 1];
          e_dist[1] = dist[(iidx[1] + dist.size(0) * cnt) - 1];
          c_xa[2] = xa_data[iidx[2] - 1];
          c_ya[2] = ya_data[iidx[2] - 1];
          e_dist[2] = dist[(iidx[2] + dist.size(0) * cnt) - 1];
          c_xa[3] = xa_data[iidx[3] - 1];
          c_ya[3] = ya_data[iidx[3] - 1];
          e_dist[3] = dist[(iidx[3] + dist.size(0) * cnt) - 1];
          ThreeAnchPos3(c_xa, c_ya, e_dist, Pos3, Prob3);
        } else {
          double b_xa[3];
          double b_ya[3];
          double d_dist[3];
          b_xa[0] = xa_data[iidx[0] - 1];
          b_ya[0] = ya_data[iidx[0] - 1];
          d_dist[0] = dist[(iidx[0] + dist.size(0) * cnt) - 1];
          b_xa[1] = xa_data[iidx[1] - 1];
          b_ya[1] = ya_data[iidx[1] - 1];
          d_dist[1] = dist[(iidx[1] + dist.size(0) * cnt) - 1];
          b_xa[2] = xa_data[iidx[2] - 1];
          b_ya[2] = ya_data[iidx[2] - 1];
          d_dist[2] = dist[(iidx[2] + dist.size(0) * cnt) - 1];
          b_ThreeAnchPos3(b_xa, b_ya, d_dist, Pos3, DistPrev);
        }
        Temp[0].re = Pos3[0];
        Temp[0].im = Pos3[1];
        Temp[1].re = Pos2[0];
        Temp[1].im = Pos2[2];
        Temp[2].re = Pos2[1];
        Temp[2].im = Pos2[3];
        if (flag == 1.0) {
          Cand_HeadingDiff = tag_pos_c[cnt].re;
          Cand_HErr = tag_pos_c[cnt].im;
          DistPrev[0] =
              rt_hypotd_snf(Pos3[0] - Cand_HeadingDiff, Pos3[1] - Cand_HErr);
          DistPrev[1] =
              rt_hypotd_snf(Pos2[0] - Cand_HeadingDiff, Pos2[2] - Cand_HErr);
          DistPrev[2] =
              rt_hypotd_snf(Pos2[1] - Cand_HeadingDiff, Pos2[3] - Cand_HErr);
          //                  Temp(1) = tag_pos_c(Pn);
          //                  Temp(2) = tag_pos_c(Pn);
        } else {
          Cand_HeadingDiff = PrevPos[cnt].re;
          Cand_HErr = PrevPos[cnt].im;
          DistPrev[0] =
              rt_hypotd_snf(Pos3[0] - Cand_HeadingDiff, Pos3[1] - Cand_HErr);
          DistPrev[1] =
              rt_hypotd_snf(Pos2[0] - Cand_HeadingDiff, Pos2[2] - Cand_HErr);
          DistPrev[2] =
              rt_hypotd_snf(Pos2[1] - Cand_HeadingDiff, Pos2[3] - Cand_HErr);
        }
        vale = coder::internal::minimum(DistPrev, l);
        Cand_HeadingDiff = PrevUWBpos[cnt].re;
        Cand_HErr = PrevUWBpos[cnt].im;
        DistPrev[0] =
            rt_hypotd_snf(Pos3[0] - Cand_HeadingDiff, Pos3[1] - Cand_HErr);
        DistPrev[1] =
            rt_hypotd_snf(Pos2[0] - Cand_HeadingDiff, Pos2[2] - Cand_HErr);
        DistPrev[2] =
            rt_hypotd_snf(Pos2[1] - Cand_HeadingDiff, Pos2[3] - Cand_HErr);
        Cand_HeadingDiff = coder::internal::minimum(DistPrev, PPI);
        if (l != PPI) {
          if ((vale < 1.0) && (Cand_HeadingDiff < 1.0)) {
            Pos2C[cnt] = Temp[l - 1];
            Pos2C[cnt + 4] = Temp[PPI - 1];
            l = 2;
          } else if ((vale < 1.0) && (Cand_HeadingDiff >= 1.0)) {
            Pos2C[cnt] = Temp[l - 1];
            l = 1;
          } else if ((vale >= 1.0) && (Cand_HeadingDiff < 1.0)) {
            Pos2C[cnt] = Temp[PPI - 1];
            l = 1;
          } else {
            Pos2C[cnt] = Temp[0];
            Pos2C[cnt + 4] = Temp[1];
            Pos2C[cnt + 8] = Temp[2];
            l = 3;
          }
        } else {
          Pos2C[cnt] = Temp[l - 1];
          //                  Pos2C(Pn,l+1) = Temp(2);
          //                  Pos2C(Pn,l+2) = Temp(3);
          l = 1;
        }
      } else {
        Temp[0].re = Pos2[0];
        Temp[0].im = Pos2[2];
        Temp[1].re = Pos2[1];
        Temp[1].im = Pos2[3];
        Cand_HeadingDiff = PrevPos[cnt].re;
        Cand_HErr = PrevPos[cnt].im;
        DistPrev[0] =
            rt_hypotd_snf(Pos2[0] - Cand_HeadingDiff, Pos2[2] - Cand_HErr);
        brm_tmp = PrevUWBpos[cnt].re;
        b = PrevUWBpos[cnt].im;
        DistPrev[1] =
            rt_hypotd_snf(Pos2[1] - Cand_HeadingDiff, Pos2[3] - Cand_HErr);
        DistPrev[2] = rt_hypotd_snf(Temp[2].re - Cand_HeadingDiff,
                                    Temp[2].im - Cand_HErr);
        vale = coder::internal::minimum(DistPrev, l);
        DistPrev[0] = rt_hypotd_snf(Pos2[0] - brm_tmp, Pos2[2] - b);
        DistPrev[1] = rt_hypotd_snf(Pos2[1] - brm_tmp, Pos2[3] - b);
        DistPrev[2] = rt_hypotd_snf(Temp[2].re - brm_tmp, Temp[2].im - b);
        Cand_HeadingDiff = coder::internal::minimum(DistPrev, PPI);
        if (l != PPI) {
          if ((vale < 1.0) && (Cand_HeadingDiff < 1.0)) {
            Pos2C[cnt] = Temp[l - 1];
            Pos2C[cnt + 4] = Temp[PPI - 1];
            l = 2;
          } else if ((vale < 1.0) && (Cand_HeadingDiff >= 1.0)) {
            Pos2C[cnt] = Temp[l - 1];
            l = 1;
          } else if ((vale >= 1.0) && (Cand_HeadingDiff < 1.0)) {
            Pos2C[cnt] = Temp[PPI - 1];
            l = 1;
          } else {
            Pos2C[cnt] = Temp[0];
            Pos2C[cnt + 4] = Temp[1];
            l = 2;
          }
        } else {
          Pos2C[cnt] = Temp[0];
          l = 1;
        }
      }
      Pos2CL[cnt] = static_cast<signed char>(l);
    }
  }
  bool b_Pos2CL[4];
  //  for Pn = 1 : Lp
  //      l = 1;
  //      if sum(dist(:,Pn))~=0
  //          [dist1, L1i] = sort(disth(:,Pn));
  //          L1 = L1i(1);
  //          L2 = L1i(2);
  //
  //          [Pos2,Prob2]=TwoAnchPos3([xa(L1) xa(L2)], [ya(L1) ya(L2)],
  //          [dist(L1,Pn) dist(L2,Pn)],[0 0],[0 0],xa+j*ya,dist(:,Pn));
  //
  //          if length(dist(dist1~=0)) > 2
  //              if (dist1(3)~=0) %&& (dist1(3)<25)
  //                  L3 = L1i(3);
  //                  if (dist1(4)~=0) %&& (dist1(4)<25)
  //                      L4 = L1i(4);
  //                      [Pos3,Prob3]=ThreeAnchPos3([xa(L1i(1:4))],
  //                      [ya(L1i(1:4))], [dist(L1i(1:4),Pn)]);
  //                  else
  //                      [Pos3,Prob3]=ThreeAnchPos3([xa(L1i(1:3))],
  //                      [ya(L1i(1:3))], [dist(L1i(1:3),Pn)]);
  //                  end
  //                  Pos2_C = Pos2(:,1) + j*Pos2(:,2);
  //                  Pos3_C = Pos3(1) + j*Pos3(2);
  //
  //                  d_1 = abs(Pos2_C(1)-Pos3_C);
  //                  d_2 = abs(Pos2_C(2)-Pos3_C);
  //
  //                  if (abs(d_1-d_2) > 1)
  //
  //                      if d_1<d_2
  //                          Pos2C(Pn,l) = Pos2(1,1)+j*Pos2(1,2);
  //                      else
  //                          Pos2C(Pn,l) = Pos2(2,1)+j*Pos2(2,2);
  //                      end
  //
  //
  //                  else
  //                      Pos2C(Pn,l) = Pos2(1,1)+j*Pos2(1,2);
  //                      Pos2C(Pn,l+1) = Pos2(2,1)+j*Pos2(2,2);
  //                      l = 2;
  //                  end
  //              else
  //                  Pos2C(Pn,l) = Pos2(1,1)+j*Pos2(1,2);
  //                  Pos2C(Pn,l+1) = Pos2(2,1)+j*Pos2(2,2);
  //                  l = 2;
  //              end
  //          else
  //              Pos2C(Pn,l) = Pos2(1,1)+j*Pos2(1,2);
  //              Pos2C(Pn,l+1) = Pos2(2,1)+j*Pos2(2,2);
  //              l = 2;
  //          end
  //
  //          Pos2CL(Pn) = l;
  //
  //      end
  //
  //  end
  b_Pos2CL[0] = (Pos2CL[0] != 0);
  b_Pos2CL[1] = (Pos2CL[1] != 0);
  b_Pos2CL[2] = (Pos2CL[2] != 0);
  b_Pos2CL[3] = (Pos2CL[3] != 0);
  coder::eml_find(b_Pos2CL, NZeroId_data, b_xa_size);
  Sel_H_re = 0.0;
  Sel_H_im = 0.0;
  Sel_HErr = 1.0E+6;
  Sel_Err = 1.0E+6;
  Sel_ErrH = 1.0E+6;
  Sel_PosDiff = 1.0E+6;
  Sel_HeadingDiff = 1.0E+6;
  ErrSum = 1.0E+6;
  // Ld = min(Pos2Prob(:))*5;
  i = static_cast<int>(std::fmax(1.0, static_cast<double>(Pos2CL[0])));
  i1 = static_cast<int>(std::fmax(1.0, static_cast<double>(Pos2CL[1])));
  i2 = static_cast<int>(std::fmax(1.0, static_cast<double>(Pos2CL[2])));
  i3 = static_cast<int>(std::fmax(1.0, static_cast<double>(Pos2CL[3])));
  i4 = b_xa_size[1];
  for (int l1{0}; l1 < i; l1++) {
    if (Pos2CL[0] != 0) {
      lP[0] = Pos2C[4 * l1];
    } else {
      lP[0].re = 0.0;
      lP[0].im = 0.0;
    }
    for (int l2{0}; l2 < i1; l2++) {
      if (Pos2CL[1] != 0) {
        lP[1] = Pos2C[4 * l2 + 1];
      } else {
        lP[1].re = 0.0;
        lP[1].im = 0.0;
      }
      for (int l3{0}; l3 < i2; l3++) {
        if (Pos2CL[2] != 0) {
          lP[2] = Pos2C[4 * l3 + 2];
        } else {
          lP[2].re = 0.0;
          lP[2].im = 0.0;
        }
        for (int l4{0}; l4 < i3; l4++) {
          double Cand_Err;
          double Cand_ErrH;
          double Cand_PosDiff;
          int b_iidx[16];
          int i5;
          bool c_b;
          if (Pos2CL[3] != 0) {
            lP[3] = Pos2C[4 * l4 + 3];
          } else {
            lP[3].re = 0.0;
            lP[3].im = 0.0;
          }
          cnt = -1;
          y_re =
              ((PrevPos[0].re + PrevPos[1].re) + PrevPos[2].re) + PrevPos[3].re;
          b = ((PrevPos[0].im + PrevPos[1].im) + PrevPos[2].im) + PrevPos[3].im;
          if (b == 0.0) {
            Cand_HeadingDiff = y_re / 4.0;
            Cand_HErr = 0.0;
          } else if (y_re == 0.0) {
            Cand_HeadingDiff = 0.0;
            Cand_HErr = b / 4.0;
          } else {
            Cand_HeadingDiff = y_re / 4.0;
            Cand_HErr = b / 4.0;
          }
          c_b = ((Cand_HeadingDiff != 0.0) || (Cand_HErr != 0.0));
          if (c_b) {
            y_re = ((lP[0].re + lP[1].re) + lP[2].re) + lP[3].re;
            b = ((lP[0].im + lP[1].im) + lP[2].im) + lP[3].im;
            if (b == 0.0) {
              y_re /= 4.0;
              b = 0.0;
            } else if (y_re == 0.0) {
              y_re = 0.0;
              b /= 4.0;
            } else {
              y_re /= 4.0;
              b /= 4.0;
            }
            Cand_PosDiff =
                rt_hypotd_snf(y_re - Cand_HeadingDiff, b - Cand_HErr) / 10.0;
          } else {
            Cand_PosDiff = 0.0;
          }
          std::memset(&SelectedPos[0], 0, 32U * sizeof(int));
          for (loop_ub = 0; loop_ub < i4; loop_ub++) {
            PPI = NZeroId_data[loop_ub] - 1;
            i5 = b_xa_size[1] - loop_ub;
            for (int ork{0}; ork <= i5 - 2; ork++) {
              b_i = NZeroId_data[(loop_ub + ork) + 1];
              Cand_HeadingDiff = lP[PPI].re - lP[b_i - 1].re;
              Cand_HErr = lP[PPI].im - lP[b_i - 1].im;
              Cand_Err = tag_pos_b[PPI].re - tag_pos_b[b_i - 1].re;
              Cand_ErrH = tag_pos_b[PPI].im - tag_pos_b[b_i - 1].im;
              if (Cand_ErrH == 0.0) {
                if (Cand_HErr == 0.0) {
                  l = (cnt + ork) + 1;
                  y_re = Cand_HeadingDiff / Cand_Err;
                  TempH[l].re = y_re;
                  TempH[l].im = 0.0;
                  b = 0.0;
                } else if (Cand_HeadingDiff == 0.0) {
                  l = (cnt + ork) + 1;
                  TempH[l].re = 0.0;
                  TempH[l].im = Cand_HErr / Cand_Err;
                  y_re = 0.0;
                  b = Cand_HErr / Cand_Err;
                } else {
                  l = (cnt + ork) + 1;
                  TempH[l].re = Cand_HeadingDiff / Cand_Err;
                  TempH[l].im = Cand_HErr / Cand_Err;
                  y_re = Cand_HeadingDiff / Cand_Err;
                  b = Cand_HErr / Cand_Err;
                }
              } else if (Cand_Err == 0.0) {
                if (Cand_HeadingDiff == 0.0) {
                  l = (cnt + ork) + 1;
                  TempH[l].re = Cand_HErr / Cand_ErrH;
                  TempH[l].im = 0.0;
                  y_re = Cand_HErr / Cand_ErrH;
                  b = 0.0;
                } else {
                  if (Cand_HErr == 0.0) {
                    l = (cnt + ork) + 1;
                    TempH[l].re = 0.0;
                    TempH[l].im = -(Cand_HeadingDiff / Cand_ErrH);
                  } else {
                    l = (cnt + ork) + 1;
                    TempH[l].re = Cand_HErr / Cand_ErrH;
                    TempH[l].im = -(Cand_HeadingDiff / Cand_ErrH);
                  }
                  if (Cand_HErr == 0.0) {
                    y_re = 0.0;
                    b = -(Cand_HeadingDiff / Cand_ErrH);
                  } else {
                    y_re = Cand_HErr / Cand_ErrH;
                    b = -(Cand_HeadingDiff / Cand_ErrH);
                  }
                }
              } else {
                brm_tmp = std::abs(Cand_Err);
                flag = std::abs(Cand_ErrH);
                if (brm_tmp > flag) {
                  b = Cand_ErrH / Cand_Err;
                  vale = Cand_Err + b * Cand_ErrH;
                  l = (cnt + ork) + 1;
                  TempH[l].re = (Cand_HeadingDiff + b * Cand_HErr) / vale;
                  TempH[l].im = (Cand_HErr - b * Cand_HeadingDiff) / vale;
                  b = Cand_ErrH / Cand_Err;
                  vale = Cand_Err + b * Cand_ErrH;
                  y_re = (Cand_HeadingDiff + b * Cand_HErr) / vale;
                  b = (Cand_HErr - b * Cand_HeadingDiff) / vale;
                } else {
                  if (flag == brm_tmp) {
                    if (Cand_Err > 0.0) {
                      b = 0.5;
                    } else {
                      b = -0.5;
                    }
                    if (Cand_ErrH > 0.0) {
                      vale = 0.5;
                    } else {
                      vale = -0.5;
                    }
                    l = (cnt + ork) + 1;
                    TempH[l].re =
                        (Cand_HeadingDiff * b + Cand_HErr * vale) / brm_tmp;
                    TempH[l].im =
                        (Cand_HErr * b - Cand_HeadingDiff * vale) / brm_tmp;
                  } else {
                    b = Cand_Err / Cand_ErrH;
                    vale = Cand_ErrH + b * Cand_Err;
                    l = (cnt + ork) + 1;
                    TempH[l].re = (b * Cand_HeadingDiff + Cand_HErr) / vale;
                    TempH[l].im = (b * Cand_HErr - Cand_HeadingDiff) / vale;
                  }
                  if (flag == brm_tmp) {
                    if (Cand_Err > 0.0) {
                      b = 0.5;
                    } else {
                      b = -0.5;
                    }
                    if (Cand_ErrH > 0.0) {
                      vale = 0.5;
                    } else {
                      vale = -0.5;
                    }
                    y_re = (Cand_HeadingDiff * b + Cand_HErr * vale) / brm_tmp;
                    b = (Cand_HErr * b - Cand_HeadingDiff * vale) / brm_tmp;
                  } else {
                    b = Cand_Err / Cand_ErrH;
                    vale = Cand_ErrH + b * Cand_Err;
                    y_re = (b * Cand_HeadingDiff + Cand_HErr) / vale;
                    b = (b * Cand_HErr - Cand_HeadingDiff) / vale;
                  }
                }
              }
              Cand_HeadingDiff = std::abs(rt_hypotd_snf(y_re, b) - 1.0);
              l = cnt + ork;
              TempAbsH[l + 1] = Cand_HeadingDiff * Cand_HeadingDiff;
              SelectedPos[l + 1] = PPI + 1;
              SelectedPos[l + 17] = b_i;
            }
            if (i5 - 2 >= 0) {
              cnt = (cnt + i5) - 1;
            }
          }
          std::copy(&TempAbsH[0], &TempAbsH[16], &b_y[0]);
          coder::internal::sort(b_y, b_iidx);
          for (i5 = 0; i5 < 16; i5++) {
            b_y[i5] = b_iidx[i5];
          }
          creal_T b_x_tmp;
          creal_T x_tmp;
          PPI = static_cast<int>(b_y[0]);
          b_i = static_cast<int>(b_y[1]);
          loop_ub = static_cast<int>(b_y[2]);
          Cand_Err = ((TempAbsH[PPI - 1] + TempAbsH[b_i - 1]) +
                      TempAbsH[loop_ub - 1]) /
                     3.0;
          b_b = TempH[PPI - 1];
          x_tmp = TempH[b_i - 1];
          b_x_tmp = TempH[loop_ub - 1];
          Cand_HeadingDiff = (b_b.re + x_tmp.re) + b_x_tmp.re;
          Cand_HErr = (b_b.im + x_tmp.im) + b_x_tmp.im;
          if (Cand_HErr == 0.0) {
            vale = Cand_HeadingDiff / 3.0;
            flag = 0.0;
          } else if (Cand_HeadingDiff == 0.0) {
            vale = 0.0;
            flag = Cand_HErr / 3.0;
          } else {
            vale = Cand_HeadingDiff / 3.0;
            flag = Cand_HErr / 3.0;
          }
          Cand_ErrH = std::abs(rt_hypotd_snf(vale, flag) - 1.0);
          if (Cand_HErr == 0.0) {
            y_re = Cand_HeadingDiff / 3.0;
            b = 0.0;
          } else if (Cand_HeadingDiff == 0.0) {
            y_re = 0.0;
            b = Cand_HErr / 3.0;
          } else {
            y_re = Cand_HeadingDiff / 3.0;
            b = Cand_HErr / 3.0;
          }
          brm_tmp = std::abs(TempH[PPI - 1].re * -b + TempH[PPI - 1].im * y_re);
          DistPrev[0] = brm_tmp * brm_tmp;
          brm_tmp = std::abs(TempH[b_i - 1].re * -b + TempH[b_i - 1].im * y_re);
          DistPrev[1] = brm_tmp * brm_tmp;
          brm_tmp = std::abs(TempH[loop_ub - 1].re * -b +
                             TempH[loop_ub - 1].im * y_re);
          Cand_HErr = ((DistPrev[0] + DistPrev[1]) + brm_tmp * brm_tmp) / 3.0;
          if (c_b) {
            Cand_HeadingDiff =
                std::abs(coder::b_mod(rt_atan2d_snf(flag, vale)) -
                         coder::b_mod(b_PrevHeading));
            Cand_HeadingDiff =
                std::fmin(Cand_HeadingDiff,
                          std::abs(Cand_HeadingDiff - 6.2831853071795862)) /
                30.0;
          } else {
            Cand_HeadingDiff = 0.0;
          }
          // if ((Cand_HErr + Cand_Err + Cand_ErrH + Cand_HeadingDiff) <
          // (Sel_HErr + Sel_Err + Sel_ErrH+ Sel_HeadingDiff )) && (cnt~=0) % &&
          // (Cand_PosDiff < 10) if ((Cand_HErr + Cand_Err + Cand_ErrH) <
          // (Sel_HErr + Sel_Err + Sel_ErrH  )) && (cnt~=0) % && (Cand_PosDiff <
          // 10)
          brm_tmp = (((Cand_HErr + Cand_Err) + Cand_ErrH) + Cand_PosDiff) +
                    Cand_HeadingDiff;
          if ((brm_tmp < (((Sel_HErr + Sel_Err) + Sel_ErrH) + Sel_PosDiff) +
                             Sel_HeadingDiff) &&
              (cnt + 1 != 0)) {
            //  && (Cand_PosDiff < 10)
            // if ((Cand_HErr + Cand_Err + Cand_ErrH + Cand_PosDiff +
            // Cand_HeadingDiff + DistErr + DistAcc) < (Sel_HErr + Sel_Err +
            // Sel_ErrH + Sel_PosDiff + Sel_HeadingDiff + Sel_DistErr  +
            // Sel_DistAcc)) && (cnt~=0) % && (Cand_PosDiff < 10)
            Sel_H_re = vale;
            Sel_H_im = flag;
            Sel_HErr = Cand_HErr;
            Sel_Err = Cand_Err;
            Sel_ErrH = Cand_ErrH;
            // ErrSum = Cand_HErr + Cand_Err + Cand_ErrH ;
            ErrSum = brm_tmp;
            Sel_PosDiff = Cand_PosDiff;
            Sel_HeadingDiff = Cand_HeadingDiff;
            //                                      Sel_DistErr = DistErr;
            //                                      Sel_DistAcc = DistAcc;
            l = SelectedPos[PPI - 1] - 1;
            Cand_HeadingDiff = lP[l].re;
            Cand_HErr = lP[l].im;
            if (Cand_HErr == 0.0) {
              vale = Cand_HeadingDiff / 6.0;
              flag = 0.0;
            } else if (Cand_HeadingDiff == 0.0) {
              vale = 0.0;
              flag = Cand_HErr / 6.0;
            } else {
              vale = Cand_HeadingDiff / 6.0;
              flag = Cand_HErr / 6.0;
            }
            l = SelectedPos[PPI + 15] - 1;
            Cand_HeadingDiff = lP[l].re;
            Cand_HErr = lP[l].im;
            if (Cand_HErr == 0.0) {
              b = Cand_HeadingDiff / 6.0;
              Cand_HeadingDiff = 0.0;
            } else if (Cand_HeadingDiff == 0.0) {
              b = 0.0;
              Cand_HeadingDiff = Cand_HErr / 6.0;
            } else {
              b = Cand_HeadingDiff / 6.0;
              Cand_HeadingDiff = Cand_HErr / 6.0;
            }
            Sel_C_re = vale + b;
            Sel_C_im = flag + Cand_HeadingDiff;
            l = SelectedPos[b_i - 1] - 1;
            Cand_HeadingDiff = lP[l].re;
            Cand_HErr = lP[l].im;
            if (Cand_HErr == 0.0) {
              vale = Cand_HeadingDiff / 6.0;
              flag = 0.0;
            } else if (Cand_HeadingDiff == 0.0) {
              vale = 0.0;
              flag = Cand_HErr / 6.0;
            } else {
              vale = Cand_HeadingDiff / 6.0;
              flag = Cand_HErr / 6.0;
            }
            l = SelectedPos[b_i + 15] - 1;
            Cand_HeadingDiff = lP[l].re;
            Cand_HErr = lP[l].im;
            if (Cand_HErr == 0.0) {
              b = Cand_HeadingDiff / 6.0;
              Cand_HeadingDiff = 0.0;
            } else if (Cand_HeadingDiff == 0.0) {
              b = 0.0;
              Cand_HeadingDiff = Cand_HErr / 6.0;
            } else {
              b = Cand_HeadingDiff / 6.0;
              Cand_HeadingDiff = Cand_HErr / 6.0;
            }
            Sel_C_re = (Sel_C_re + vale) + b;
            Sel_C_im = (Sel_C_im + flag) + Cand_HeadingDiff;
            l = SelectedPos[loop_ub - 1] - 1;
            Cand_HeadingDiff = lP[l].re;
            Cand_HErr = lP[l].im;
            if (Cand_HErr == 0.0) {
              vale = Cand_HeadingDiff / 6.0;
              flag = 0.0;
            } else if (Cand_HeadingDiff == 0.0) {
              vale = 0.0;
              flag = Cand_HErr / 6.0;
            } else {
              vale = Cand_HeadingDiff / 6.0;
              flag = Cand_HErr / 6.0;
            }
            l = SelectedPos[loop_ub + 15] - 1;
            Cand_HeadingDiff = lP[l].re;
            Cand_HErr = lP[l].im;
            if (Cand_HErr == 0.0) {
              b = Cand_HeadingDiff / 6.0;
              Cand_HeadingDiff = 0.0;
            } else if (Cand_HeadingDiff == 0.0) {
              b = 0.0;
              Cand_HeadingDiff = Cand_HErr / 6.0;
            } else {
              b = Cand_HeadingDiff / 6.0;
              Cand_HeadingDiff = Cand_HErr / 6.0;
            }
            Sel_C_re = (Sel_C_re + vale) + b;
            Sel_C_im = (Sel_C_im + flag) + Cand_HeadingDiff;
            //                      for dfg = 1 : length(NZeroId)
            //                          Sel_C = Sel_C +
            //                          (lP(NZeroId(dfg))-exp(j*angle(Sel_H))*tag_pos_b(NZeroId(dfg)))/length(NZeroId);
            //                      end
          }
        }
        //                          end
        //                      end
        //                  end
        //              end
      }
    }
  }
  //
  //  for L1 = 1 : Ln
  //      for L2 = L1 + 1 : Ln
  //          for Pn = 1 : Lp
  //              if (dist(L1,Pn)~=0) && (dist(L2,Pn)~=0)
  //                  [Pos2,Prob2]=TwoAnchPos3([xa(L1) xa(L2)], [ya(L1) ya(L2)],
  //                  [dist(L1,Pn) dist(L2,Pn)],[0 0],[0 0],xa+j*ya,dist(:,Pn));
  //                  Pos2C(Pn,1) = Pos2(1,1)+j*Pos2(1,2);
  //                  Pos2C(Pn,2) = Pos2(2,1)+j*Pos2(2,2);
  //              else
  //                  Pos2C(Pn,1) = 0;
  //                  Pos2C(Pn,2) = 0;
  //              end
  //          end
  //          for Qn = 0 : 2^Lp-1
  //              Va = dec2bin(Qn,Lp);
  //              [Est_F(Qn+1),Est_C(Qn+1),Est_H(Qn+1)] =
  //              GetMultiTagPosGen(Pos2C,Va,tag_pos_b,[1 1 1 1]);
  //          end
  //          [val,ind] = min(Est_F);
  //          Cand_F = val;
  //          Cand_C = Est_C(ind);
  //          Cand_H = Est_H(ind);
  //          if Sel_F > Cand_F
  //              Sel_F = Cand_F;
  //              Sel_C = Cand_C;
  //              Sel_H = Cand_H;
  //          end
  //      end
  //  end
  heading_est = rt_atan2d_snf(Sel_H_im, Sel_H_re);
  b_b.re = heading_est * 0.0;
  b_b.im = heading_est;
  coder::b_exp(b_b);
  Cand_HeadingDiff =
      Sel_C_re + (tag_pos_b[0].re * b_b.re - tag_pos_b[0].im * b_b.im);
  Cand_HErr = Sel_C_im + (tag_pos_b[0].re * b_b.im + tag_pos_b[0].im * b_b.re);
  tag_pos_est[0].re = Cand_HeadingDiff;
  tag_pos_est[0].im = Cand_HErr;
  PrevUWBpos[0].re = Cand_HeadingDiff;
  PrevUWBpos[0].im = Cand_HErr;
  Cand_HeadingDiff =
      Sel_C_re + (tag_pos_b[1].re * b_b.re - tag_pos_b[1].im * b_b.im);
  Cand_HErr = Sel_C_im + (tag_pos_b[1].re * b_b.im + tag_pos_b[1].im * b_b.re);
  tag_pos_est[1].re = Cand_HeadingDiff;
  tag_pos_est[1].im = Cand_HErr;
  PrevUWBpos[1].re = Cand_HeadingDiff;
  PrevUWBpos[1].im = Cand_HErr;
  Cand_HeadingDiff =
      Sel_C_re + (tag_pos_b[2].re * b_b.re - tag_pos_b[2].im * b_b.im);
  Cand_HErr = Sel_C_im + (tag_pos_b[2].re * b_b.im + tag_pos_b[2].im * b_b.re);
  tag_pos_est[2].re = Cand_HeadingDiff;
  tag_pos_est[2].im = Cand_HErr;
  PrevUWBpos[2].re = Cand_HeadingDiff;
  PrevUWBpos[2].im = Cand_HErr;
  Cand_HeadingDiff =
      Sel_C_re + (tag_pos_b[3].re * b_b.re - tag_pos_b[3].im * b_b.im);
  Cand_HErr = Sel_C_im + (tag_pos_b[3].re * b_b.im + tag_pos_b[3].im * b_b.re);
  tag_pos_est[3].re = Cand_HeadingDiff;
  tag_pos_est[3].im = Cand_HErr;
  PrevUWBpos[3].re = Cand_HeadingDiff;
  PrevUWBpos[3].im = Cand_HErr;
  return heading_est;
}

//
// Arguments    : void
// Return Type  : void
//
void GetInitPos4_init()
{
  std::memset(&PrevUWBpos[0], 0, 4U * sizeof(creal_T));
}

//
// File trailer for GetInitPos4.cpp
//
// [EOF]
//
