//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: GetInitPos4.cpp
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 30-Nov-2023 16:43:53
//

// Include Files
#include "GetInitPos4.h"
#include "GetUWBPos_v4.h"
#include "ThreeAnchPos3.h"
#include "TwoAnchPos3.h"
#include "exp.h"
#include "find.h"
#include "fusion2_data.h"
#include "fusion2_rtwutil.h"
#include "mean.h"
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
  for (int i{0}; i < loop_ub; i++) {
    double d;
    d = in3_data[i * stride_1_1];
    b_in1_data[i].re = in1_data[i * stride_0_1] + 0.0 * d;
    b_in1_data[i].im = d;
  }
  loop_ub = in4.size(0);
  c_in4.set_size(loop_ub);
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
//                double *heading_est
//                double *ErrSum
// Return Type  : void
//
void GetInitPos4(const double xa_data[], const int xa_size[2],
                 const double ya_data[], const int ya_size[2],
                 const coder::array<double, 2U> &dist,
                 const creal_T tag_pos_b[4], double Ln,
                 const creal_T PrevPos[4], double b_PrevHeading,
                 creal_T tag_pos_est[4], double *heading_est, double *ErrSum)
{
  coder::array<creal_T, 2U> Pos2C;
  coder::array<double, 2U> disth;
  coder::array<double, 1U> b_dist;
  coder::array<double, 1U> dist1;
  coder::array<int, 1U> iidx;
  creal_T TempH[16];
  creal_T lP[4];
  creal_T tag_pos_c[4];
  creal_T Temp[3];
  creal_T b;
  creal_T y;
  double TempAbsH[16];
  double x[16];
  double Pos2[4];
  double DistPrev[3];
  double b_xa[3];
  double Pos3[2];
  double Cand_Err;
  double Cand_ErrH;
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
  double d;
  double im;
  double re;
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
  loop_ub = dist.size(0) * dist.size(1);
  for (i = 0; i < loop_ub; i++) {
    disth[i] = dist[i];
  }
  PPI = dist.size(0) * dist.size(1) - 1;
  l = 0;
  for (b_i = 0; b_i <= PPI; b_i++) {
    if (dist[b_i] == 0.0) {
      l++;
    }
  }
  iidx.set_size(l);
  l = 0;
  for (b_i = 0; b_i <= PPI; b_i++) {
    if (dist[b_i] == 0.0) {
      iidx[l] = b_i + 1;
      l++;
    }
  }
  loop_ub = iidx.size(0);
  for (i = 0; i < loop_ub; i++) {
    disth[iidx[i] - 1] = 100.0;
  }
  GetUWBPos_v4(xa_data, ya_data, dist, Ln, tag_pos_b, &Cand_HeadingDiff,
               &Cand_Err, &Cand_ErrH, &Cand_HErr);
  b.re = Cand_ErrH * 0.0;
  b.im = Cand_ErrH;
  coder::b_exp(&b);
  y.re = Cand_HeadingDiff + 0.0 * Cand_Err;
  tag_pos_c[0].re = y.re + (tag_pos_b[0].re * b.re - tag_pos_b[0].im * b.im);
  tag_pos_c[0].im =
      Cand_Err + (tag_pos_b[0].re * b.im + tag_pos_b[0].im * b.re);
  tag_pos_c[1].re = y.re + (tag_pos_b[1].re * b.re - tag_pos_b[1].im * b.im);
  tag_pos_c[1].im =
      Cand_Err + (tag_pos_b[1].re * b.im + tag_pos_b[1].im * b.re);
  tag_pos_c[2].re = y.re + (tag_pos_b[2].re * b.re - tag_pos_b[2].im * b.im);
  tag_pos_c[2].im =
      Cand_Err + (tag_pos_b[2].re * b.im + tag_pos_b[2].im * b.re);
  tag_pos_c[3].re = y.re + (tag_pos_b[3].re * b.re - tag_pos_b[3].im * b.im);
  tag_pos_c[3].im =
      Cand_Err + (tag_pos_b[3].re * b.im + tag_pos_b[3].im * b.re);
  loop_ub = dist.size(0);
  for (cnt = 0; cnt < 4; cnt++) {
    b_dist.set_size(dist.size(0));
    for (i = 0; i < loop_ub; i++) {
      b_dist[i] = dist[i + dist.size(0) * cnt];
    }
    if (coder::sum(b_dist) != 0.0) {
      l = disth.size(0);
      dist1.set_size(disth.size(0));
      for (i = 0; i < l; i++) {
        dist1[i] = disth[i + disth.size(0) * cnt];
      }
      coder::internal::sort(dist1, iidx);
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
          b_xa_data[i].re = xa_data[i] + 0.0 * ya_data[i];
          b_xa_data[i].im = ya_data[i];
        }
        l = dist.size(0);
        b_dist.set_size(dist.size(0));
        for (i = 0; i < l; i++) {
          b_dist[i] = dist[i + dist.size(0) * cnt];
        }
        TwoAnchPos3(xa, ya, c_dist, b_xa_data, b_xa_size, b_dist, Pos2, Pos3);
      } else {
        binary_expand_op(xa_data, xa_size, iidx, ya_data, ya_size, dist, cnt,
                         Pos2, Pos3);
      }
      PPI = dist1.size(0);
      l = 0;
      for (b_i = 0; b_i < PPI; b_i++) {
        if (dist1[b_i] < 30.0) {
          l++;
        }
      }
      if (l > 2) {
        if (dist1[3] < 30.0) {
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
        if (Cand_HErr == 1.0) {
          Cand_HeadingDiff = tag_pos_c[cnt].re;
          Cand_Err = tag_pos_c[cnt].im;
          DistPrev[0] =
              rt_hypotd_snf(Pos3[0] - Cand_HeadingDiff, Pos3[1] - Cand_Err);
          DistPrev[1] =
              rt_hypotd_snf(Pos2[0] - Cand_HeadingDiff, Pos2[2] - Cand_Err);
          DistPrev[2] =
              rt_hypotd_snf(Pos2[1] - Cand_HeadingDiff, Pos2[3] - Cand_Err);
          //                  Temp(1) = tag_pos_c(Pn);
          //                  Temp(2) = tag_pos_c(Pn);
        } else {
          DistPrev[0] = rt_hypotd_snf(Pos3[0] - PrevPos[cnt].re,
                                      Pos3[1] - PrevPos[cnt].im);
          DistPrev[1] = rt_hypotd_snf(Pos2[0] - PrevPos[cnt].re,
                                      Pos2[2] - PrevPos[cnt].im);
          DistPrev[2] = rt_hypotd_snf(Pos2[1] - PrevPos[cnt].re,
                                      Pos2[3] - PrevPos[cnt].im);
        }
        coder::internal::minimum(DistPrev, &Cand_Err, &l);
        DistPrev[0] = rt_hypotd_snf(Pos3[0] - PrevUWBpos[cnt].re,
                                    Pos3[1] - PrevUWBpos[cnt].im);
        DistPrev[1] = rt_hypotd_snf(Pos2[0] - PrevUWBpos[cnt].re,
                                    Pos2[2] - PrevUWBpos[cnt].im);
        DistPrev[2] = rt_hypotd_snf(Pos2[1] - PrevUWBpos[cnt].re,
                                    Pos2[3] - PrevUWBpos[cnt].im);
        coder::internal::minimum(DistPrev, &Cand_HeadingDiff, &PPI);
        if (l != PPI) {
          if ((Cand_Err < 1.0) && (Cand_HeadingDiff < 1.0)) {
            Pos2C[cnt] = Temp[l - 1];
            Pos2C[cnt + 4] = Temp[PPI - 1];
            l = 2;
          } else if ((Cand_Err < 1.0) && (Cand_HeadingDiff >= 1.0)) {
            Pos2C[cnt] = Temp[l - 1];
            l = 1;
          } else if ((Cand_Err >= 1.0) && (Cand_HeadingDiff < 1.0)) {
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
        Cand_Err = PrevPos[cnt].im;
        DistPrev[0] =
            rt_hypotd_snf(Pos2[0] - Cand_HeadingDiff, Pos2[2] - Cand_Err);
        d = PrevUWBpos[cnt].re;
        Cand_ErrH = PrevUWBpos[cnt].im;
        DistPrev[1] =
            rt_hypotd_snf(Pos2[1] - Cand_HeadingDiff, Pos2[3] - Cand_Err);
        DistPrev[2] =
            rt_hypotd_snf(Temp[2].re - Cand_HeadingDiff, Temp[2].im - Cand_Err);
        coder::internal::minimum(DistPrev, &Cand_Err, &l);
        DistPrev[0] = rt_hypotd_snf(Pos2[0] - d, Pos2[2] - Cand_ErrH);
        DistPrev[1] = rt_hypotd_snf(Pos2[1] - d, Pos2[3] - Cand_ErrH);
        DistPrev[2] = rt_hypotd_snf(Temp[2].re - d, Temp[2].im - Cand_ErrH);
        coder::internal::minimum(DistPrev, &Cand_HeadingDiff, &PPI);
        if (l != PPI) {
          if ((Cand_Err < 1.0) && (Cand_HeadingDiff < 1.0)) {
            Pos2C[cnt] = Temp[l - 1];
            Pos2C[cnt + 4] = Temp[PPI - 1];
            l = 2;
          } else if ((Cand_Err < 1.0) && (Cand_HeadingDiff >= 1.0)) {
            Pos2C[cnt] = Temp[l - 1];
            l = 1;
          } else if ((Cand_Err >= 1.0) && (Cand_HeadingDiff < 1.0)) {
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
  *ErrSum = 1.0E+6;
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
          double Cand_PosDiff;
          int b_iidx[16];
          int i5;
          if (Pos2CL[3] != 0) {
            lP[3] = Pos2C[4 * l4 + 3];
          } else {
            lP[3].re = 0.0;
            lP[3].im = 0.0;
          }
          cnt = -1;
          y.re =
              ((PrevPos[0].re + PrevPos[1].re) + PrevPos[2].re) + PrevPos[3].re;
          y.im =
              ((PrevPos[0].im + PrevPos[1].im) + PrevPos[2].im) + PrevPos[3].im;
          if (y.im == 0.0) {
            re = y.re / 4.0;
            im = 0.0;
          } else if (y.re == 0.0) {
            re = 0.0;
            im = y.im / 4.0;
          } else {
            re = y.re / 4.0;
            im = y.im / 4.0;
          }
          if ((re != 0.0) || (im != 0.0)) {
            y.re = ((lP[0].re + lP[1].re) + lP[2].re) + lP[3].re;
            y.im = ((lP[0].im + lP[1].im) + lP[2].im) + lP[3].im;
            if (y.im == 0.0) {
              Cand_HeadingDiff = y.re / 4.0;
              Cand_Err = 0.0;
            } else if (y.re == 0.0) {
              Cand_HeadingDiff = 0.0;
              Cand_Err = y.im / 4.0;
            } else {
              Cand_HeadingDiff = y.re / 4.0;
              Cand_Err = y.im / 4.0;
            }
            Cand_PosDiff =
                rt_hypotd_snf(Cand_HeadingDiff - re, Cand_Err - im) / 10.0;
          } else {
            Cand_PosDiff = 0.0;
          }
          std::memset(&SelectedPos[0], 0, 32U * sizeof(int));
          for (loop_ub = 0; loop_ub < i4; loop_ub++) {
            PPI = NZeroId_data[loop_ub] - 1;
            i5 = b_xa_size[1] - loop_ub;
            for (int ork{0}; ork <= i5 - 2; ork++) {
              double bi_tmp;
              double br_tmp;
              b_i = NZeroId_data[(loop_ub + ork) + 1];
              Cand_ErrH = lP[PPI].re - lP[b_i - 1].re;
              Cand_HErr = lP[PPI].im - lP[b_i - 1].im;
              br_tmp = tag_pos_b[PPI].re - tag_pos_b[b_i - 1].re;
              bi_tmp = tag_pos_b[PPI].im - tag_pos_b[b_i - 1].im;
              if (bi_tmp == 0.0) {
                if (Cand_HErr == 0.0) {
                  l = (cnt + ork) + 1;
                  d = Cand_ErrH / br_tmp;
                  TempH[l].re = d;
                  TempH[l].im = 0.0;
                  y.re = d;
                  y.im = 0.0;
                } else if (Cand_ErrH == 0.0) {
                  l = (cnt + ork) + 1;
                  TempH[l].re = 0.0;
                  TempH[l].im = Cand_HErr / br_tmp;
                  y.re = 0.0;
                  y.im = Cand_HErr / br_tmp;
                } else {
                  l = (cnt + ork) + 1;
                  TempH[l].re = Cand_ErrH / br_tmp;
                  TempH[l].im = Cand_HErr / br_tmp;
                  y.re = Cand_ErrH / br_tmp;
                  y.im = Cand_HErr / br_tmp;
                }
              } else if (br_tmp == 0.0) {
                if (Cand_ErrH == 0.0) {
                  l = (cnt + ork) + 1;
                  TempH[l].re = Cand_HErr / bi_tmp;
                  TempH[l].im = 0.0;
                  y.re = Cand_HErr / bi_tmp;
                  y.im = 0.0;
                } else {
                  if (Cand_HErr == 0.0) {
                    l = (cnt + ork) + 1;
                    TempH[l].re = 0.0;
                    TempH[l].im = -(Cand_ErrH / bi_tmp);
                  } else {
                    l = (cnt + ork) + 1;
                    TempH[l].re = Cand_HErr / bi_tmp;
                    TempH[l].im = -(Cand_ErrH / bi_tmp);
                  }
                  if (Cand_HErr == 0.0) {
                    y.re = 0.0;
                    y.im = -(Cand_ErrH / bi_tmp);
                  } else {
                    y.re = Cand_HErr / bi_tmp;
                    y.im = -(Cand_ErrH / bi_tmp);
                  }
                }
              } else {
                double bim_tmp;
                double brm_tmp;
                brm_tmp = std::abs(br_tmp);
                bim_tmp = std::abs(bi_tmp);
                if (brm_tmp > bim_tmp) {
                  Cand_HeadingDiff = bi_tmp / br_tmp;
                  d = br_tmp + Cand_HeadingDiff * bi_tmp;
                  l = (cnt + ork) + 1;
                  TempH[l].re = (Cand_ErrH + Cand_HeadingDiff * Cand_HErr) / d;
                  TempH[l].im = (Cand_HErr - Cand_HeadingDiff * Cand_ErrH) / d;
                  Cand_HeadingDiff = bi_tmp / br_tmp;
                  d = br_tmp + Cand_HeadingDiff * bi_tmp;
                  y.re = (Cand_ErrH + Cand_HeadingDiff * Cand_HErr) / d;
                  y.im = (Cand_HErr - Cand_HeadingDiff * Cand_ErrH) / d;
                } else {
                  if (bim_tmp == brm_tmp) {
                    if (br_tmp > 0.0) {
                      Cand_HeadingDiff = 0.5;
                    } else {
                      Cand_HeadingDiff = -0.5;
                    }
                    if (bi_tmp > 0.0) {
                      Cand_Err = 0.5;
                    } else {
                      Cand_Err = -0.5;
                    }
                    l = (cnt + ork) + 1;
                    TempH[l].re =
                        (Cand_ErrH * Cand_HeadingDiff + Cand_HErr * Cand_Err) /
                        brm_tmp;
                    TempH[l].im =
                        (Cand_HErr * Cand_HeadingDiff - Cand_ErrH * Cand_Err) /
                        brm_tmp;
                  } else {
                    Cand_HeadingDiff = br_tmp / bi_tmp;
                    d = bi_tmp + Cand_HeadingDiff * br_tmp;
                    l = (cnt + ork) + 1;
                    TempH[l].re =
                        (Cand_HeadingDiff * Cand_ErrH + Cand_HErr) / d;
                    TempH[l].im =
                        (Cand_HeadingDiff * Cand_HErr - Cand_ErrH) / d;
                  }
                  if (bim_tmp == brm_tmp) {
                    if (br_tmp > 0.0) {
                      Cand_HeadingDiff = 0.5;
                    } else {
                      Cand_HeadingDiff = -0.5;
                    }
                    if (bi_tmp > 0.0) {
                      Cand_Err = 0.5;
                    } else {
                      Cand_Err = -0.5;
                    }
                    y.re =
                        (Cand_ErrH * Cand_HeadingDiff + Cand_HErr * Cand_Err) /
                        brm_tmp;
                    y.im =
                        (Cand_HErr * Cand_HeadingDiff - Cand_ErrH * Cand_Err) /
                        brm_tmp;
                  } else {
                    Cand_HeadingDiff = br_tmp / bi_tmp;
                    d = bi_tmp + Cand_HeadingDiff * br_tmp;
                    y.re = (Cand_HeadingDiff * Cand_ErrH + Cand_HErr) / d;
                    y.im = (Cand_HeadingDiff * Cand_HErr - Cand_ErrH) / d;
                  }
                }
              }
              Cand_HeadingDiff = std::abs(rt_hypotd_snf(y.re, y.im) - 1.0);
              l = (cnt + ork) + 1;
              TempAbsH[l] = Cand_HeadingDiff * Cand_HeadingDiff;
              SelectedPos[l] = PPI + 1;
              SelectedPos[(cnt + ork) + 17] = b_i;
            }
            if (i5 - 2 >= 0) {
              cnt = (cnt + i5) - 1;
            }
          }
          std::copy(&TempAbsH[0], &TempAbsH[16], &x[0]);
          coder::internal::sort(x, b_iidx);
          for (i5 = 0; i5 < 16; i5++) {
            x[i5] = b_iidx[i5];
          }
          creal_T Temp_tmp;
          creal_T b_Temp_tmp;
          PPI = static_cast<int>(x[0]);
          b_xa[0] = TempAbsH[PPI - 1];
          b = TempH[PPI - 1];
          Temp[0] = b;
          b_i = static_cast<int>(x[1]);
          b_xa[1] = TempAbsH[b_i - 1];
          Temp_tmp = TempH[b_i - 1];
          Temp[1] = Temp_tmp;
          loop_ub = static_cast<int>(x[2]);
          b_xa[2] = TempAbsH[loop_ub - 1];
          b_Temp_tmp = TempH[loop_ub - 1];
          Temp[2] = b_Temp_tmp;
          Cand_Err = coder::mean(b_xa);
          y = coder::mean(Temp);
          Cand_ErrH = std::abs(rt_hypotd_snf(y.re, y.im) - 1.0);
          Temp[0] = b;
          Temp[1] = Temp_tmp;
          Temp[2] = b_Temp_tmp;
          b = coder::mean(Temp);
          d = std::abs(TempH[PPI - 1].re * -b.im + TempH[PPI - 1].im * b.re);
          DistPrev[0] = d * d;
          d = std::abs(TempH[b_i - 1].re * -b.im + TempH[b_i - 1].im * b.re);
          DistPrev[1] = d * d;
          d = std::abs(TempH[loop_ub - 1].re * -b.im +
                       TempH[loop_ub - 1].im * b.re);
          DistPrev[2] = d * d;
          Cand_HErr = coder::mean(DistPrev);
          if ((re != 0.0) || (im != 0.0)) {
            Cand_HeadingDiff =
                std::abs(coder::b_mod(rt_atan2d_snf(y.im, y.re)) -
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
          d = (((Cand_HErr + Cand_Err) + Cand_ErrH) + Cand_PosDiff) +
              Cand_HeadingDiff;
          if ((d < (((Sel_HErr + Sel_Err) + Sel_ErrH) + Sel_PosDiff) +
                       Sel_HeadingDiff) &&
              (cnt + 1 != 0)) {
            //  && (Cand_PosDiff < 10)
            // if ((Cand_HErr + Cand_Err + Cand_ErrH + Cand_PosDiff +
            // Cand_HeadingDiff + DistErr + DistAcc) < (Sel_HErr + Sel_Err +
            // Sel_ErrH + Sel_PosDiff + Sel_HeadingDiff + Sel_DistErr  +
            // Sel_DistAcc)) && (cnt~=0) % && (Cand_PosDiff < 10)
            Sel_H_re = y.re;
            Sel_H_im = y.im;
            Sel_HErr = Cand_HErr;
            Sel_Err = Cand_Err;
            Sel_ErrH = Cand_ErrH;
            // ErrSum = Cand_HErr + Cand_Err + Cand_ErrH ;
            *ErrSum = d;
            Sel_PosDiff = Cand_PosDiff;
            Sel_HeadingDiff = Cand_HeadingDiff;
            //                                      Sel_DistErr = DistErr;
            //                                      Sel_DistAcc = DistAcc;
            l = SelectedPos[PPI - 1] - 1;
            Cand_HeadingDiff = lP[l].re;
            Cand_Err = lP[l].im;
            if (Cand_Err == 0.0) {
              Cand_HErr = Cand_HeadingDiff / 6.0;
              d = 0.0;
            } else if (Cand_HeadingDiff == 0.0) {
              Cand_HErr = 0.0;
              d = Cand_Err / 6.0;
            } else {
              Cand_HErr = Cand_HeadingDiff / 6.0;
              d = Cand_Err / 6.0;
            }
            l = SelectedPos[PPI + 15] - 1;
            Cand_HeadingDiff = lP[l].re;
            Cand_Err = lP[l].im;
            if (Cand_Err == 0.0) {
              Cand_ErrH = Cand_HeadingDiff / 6.0;
              Cand_HeadingDiff = 0.0;
            } else if (Cand_HeadingDiff == 0.0) {
              Cand_ErrH = 0.0;
              Cand_HeadingDiff = Cand_Err / 6.0;
            } else {
              Cand_ErrH = Cand_HeadingDiff / 6.0;
              Cand_HeadingDiff = Cand_Err / 6.0;
            }
            Sel_C_re = Cand_HErr + Cand_ErrH;
            Sel_C_im = d + Cand_HeadingDiff;
            l = SelectedPos[b_i - 1] - 1;
            Cand_HeadingDiff = lP[l].re;
            Cand_Err = lP[l].im;
            if (Cand_Err == 0.0) {
              Cand_HErr = Cand_HeadingDiff / 6.0;
              d = 0.0;
            } else if (Cand_HeadingDiff == 0.0) {
              Cand_HErr = 0.0;
              d = Cand_Err / 6.0;
            } else {
              Cand_HErr = Cand_HeadingDiff / 6.0;
              d = Cand_Err / 6.0;
            }
            l = SelectedPos[b_i + 15] - 1;
            Cand_HeadingDiff = lP[l].re;
            Cand_Err = lP[l].im;
            if (Cand_Err == 0.0) {
              Cand_ErrH = Cand_HeadingDiff / 6.0;
              Cand_HeadingDiff = 0.0;
            } else if (Cand_HeadingDiff == 0.0) {
              Cand_ErrH = 0.0;
              Cand_HeadingDiff = Cand_Err / 6.0;
            } else {
              Cand_ErrH = Cand_HeadingDiff / 6.0;
              Cand_HeadingDiff = Cand_Err / 6.0;
            }
            Sel_C_re = (Sel_C_re + Cand_HErr) + Cand_ErrH;
            Sel_C_im = (Sel_C_im + d) + Cand_HeadingDiff;
            l = SelectedPos[loop_ub - 1] - 1;
            Cand_HeadingDiff = lP[l].re;
            Cand_Err = lP[l].im;
            if (Cand_Err == 0.0) {
              Cand_HErr = Cand_HeadingDiff / 6.0;
              d = 0.0;
            } else if (Cand_HeadingDiff == 0.0) {
              Cand_HErr = 0.0;
              d = Cand_Err / 6.0;
            } else {
              Cand_HErr = Cand_HeadingDiff / 6.0;
              d = Cand_Err / 6.0;
            }
            l = SelectedPos[loop_ub + 15] - 1;
            Cand_HeadingDiff = lP[l].re;
            Cand_Err = lP[l].im;
            if (Cand_Err == 0.0) {
              Cand_ErrH = Cand_HeadingDiff / 6.0;
              Cand_HeadingDiff = 0.0;
            } else if (Cand_HeadingDiff == 0.0) {
              Cand_ErrH = 0.0;
              Cand_HeadingDiff = Cand_Err / 6.0;
            } else {
              Cand_ErrH = Cand_HeadingDiff / 6.0;
              Cand_HeadingDiff = Cand_Err / 6.0;
            }
            Sel_C_re = (Sel_C_re + Cand_HErr) + Cand_ErrH;
            Sel_C_im = (Sel_C_im + d) + Cand_HeadingDiff;
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
  *heading_est = rt_atan2d_snf(Sel_H_im, Sel_H_re);
  b.re = *heading_est * 0.0;
  b.im = *heading_est;
  coder::b_exp(&b);
  re = Sel_C_re + (tag_pos_b[0].re * b.re - tag_pos_b[0].im * b.im);
  im = Sel_C_im + (tag_pos_b[0].re * b.im + tag_pos_b[0].im * b.re);
  tag_pos_est[0].re = re;
  tag_pos_est[0].im = im;
  PrevUWBpos[0].re = re;
  PrevUWBpos[0].im = im;
  re = Sel_C_re + (tag_pos_b[1].re * b.re - tag_pos_b[1].im * b.im);
  im = Sel_C_im + (tag_pos_b[1].re * b.im + tag_pos_b[1].im * b.re);
  tag_pos_est[1].re = re;
  tag_pos_est[1].im = im;
  PrevUWBpos[1].re = re;
  PrevUWBpos[1].im = im;
  re = Sel_C_re + (tag_pos_b[2].re * b.re - tag_pos_b[2].im * b.im);
  im = Sel_C_im + (tag_pos_b[2].re * b.im + tag_pos_b[2].im * b.re);
  tag_pos_est[2].re = re;
  tag_pos_est[2].im = im;
  PrevUWBpos[2].re = re;
  PrevUWBpos[2].im = im;
  re = Sel_C_re + (tag_pos_b[3].re * b.re - tag_pos_b[3].im * b.im);
  im = Sel_C_im + (tag_pos_b[3].re * b.im + tag_pos_b[3].im * b.re);
  tag_pos_est[3].re = re;
  tag_pos_est[3].im = im;
  PrevUWBpos[3].re = re;
  PrevUWBpos[3].im = im;
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
