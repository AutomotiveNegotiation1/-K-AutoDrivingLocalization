//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: GetUWBPos_v4.cpp
//
// MATLAB Coder version            : 5.6
// C/C++ source code generated on  : 17-Oct-2023 17:24:31
//

// Include Files
#include "GetUWBPos_v4.h"
#include "TrilateralPos.h"
#include "fusion_rtwutil.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <cmath>

// Function Definitions
//
// Get Center Pos
//
// Arguments    : const double xa_data[]
//                const double ya_data[]
//                const coder::array<double, 2U> &dist
//                double Ln
//                const creal_T tag_pos_b[4]
//                double &Yc
//                double &heading
//                double &flag
// Return Type  : double
//
double GetUWBPos_v4(const double xa_data[], const double ya_data[],
                    const coder::array<double, 2U> &dist, double Ln,
                    const creal_T tag_pos_b[4], double &Yc, double &heading,
                    double &flag)
{
  coder::array<double, 2U> distc;
  coder::array<double, 2U> diste;
  coder::array<double, 2U> distn;
  coder::array<double, 2U> dists;
  coder::array<double, 2U> distw;
  coder::array<double, 2U> xae;
  coder::array<double, 2U> xan;
  coder::array<double, 2U> xas;
  coder::array<double, 2U> xat;
  coder::array<double, 2U> xaw;
  coder::array<double, 2U> yae;
  coder::array<double, 2U> yan;
  coder::array<double, 2U> yas;
  coder::array<double, 2U> yat;
  coder::array<double, 2U> yaw;
  coder::array<int, 2U> ii;
  coder::array<bool, 2U> x;
  double Xc;
  double Xt_e_e;
  double Xt_n_e;
  double Xt_s_e;
  double Xt_w_e;
  double Yt_e_e;
  double Yt_n_e;
  double Yt_s_e;
  double Yt_w_e;
  int flagc;
  int flage;
  int flagn;
  int flags;
  int flagw;
  int idx;
  int loop_ub_tmp;
  unsigned int qq;
  unsigned int rr;
  unsigned int ss;
  unsigned int tt;
  qq = 1U;
  flagc = 0;
  flage = 0;
  flags = 0;
  flagw = 0;
  flagn = 0;
  //  Preallocation
  //  Assuming Ln is the maximum size you'd iterate over
  loop_ub_tmp = static_cast<int>(Ln);
  xat.set_size(1, loop_ub_tmp);
  yat.set_size(1, loop_ub_tmp);
  distc.set_size(1, loop_ub_tmp);
  xae.set_size(1, loop_ub_tmp);
  yae.set_size(1, loop_ub_tmp);
  diste.set_size(1, loop_ub_tmp);
  xaw.set_size(1, loop_ub_tmp);
  yaw.set_size(1, loop_ub_tmp);
  distw.set_size(1, loop_ub_tmp);
  xan.set_size(1, loop_ub_tmp);
  yan.set_size(1, loop_ub_tmp);
  distn.set_size(1, loop_ub_tmp);
  xas.set_size(1, loop_ub_tmp);
  yas.set_size(1, loop_ub_tmp);
  dists.set_size(1, loop_ub_tmp);
  for (idx = 0; idx < loop_ub_tmp; idx++) {
    xat[idx] = 0.0;
    yat[idx] = 0.0;
    distc[idx] = 0.0;
    xae[idx] = 0.0;
    yae[idx] = 0.0;
    diste[idx] = 0.0;
    xaw[idx] = 0.0;
    yaw[idx] = 0.0;
    distw[idx] = 0.0;
    xan[idx] = 0.0;
    yan[idx] = 0.0;
    distn[idx] = 0.0;
    xas[idx] = 0.0;
    yas[idx] = 0.0;
    dists[idx] = 0.0;
  }
  for (int kk{0}; kk < loop_ub_tmp; kk++) {
    int b_ii;
    int nx;
    bool exitg1;
    nx = dist.size(1);
    x.set_size(1, dist.size(1));
    for (idx = 0; idx < nx; idx++) {
      x[idx] = (dist[kk + dist.size(0) * idx] != 0.0);
    }
    nx = x.size(1);
    idx = 0;
    ii.set_size(1, x.size(1));
    b_ii = 0;
    exitg1 = false;
    while ((!exitg1) && (b_ii <= nx - 1)) {
      if (x[b_ii]) {
        idx++;
        ii[idx - 1] = b_ii + 1;
        if (idx >= nx) {
          exitg1 = true;
        } else {
          b_ii++;
        }
      } else {
        b_ii++;
      }
    }
    if (x.size(1) == 1) {
      if (idx == 0) {
        ii.set_size(ii.size(0), 0);
      }
    } else {
      if (idx < 1) {
        idx = 0;
      }
      ii.set_size(ii.size(0), idx);
    }
    if (ii.size(1) == 4) {
      xat[static_cast<int>(qq) - 1] = xa_data[kk];
      yat[static_cast<int>(qq) - 1] = ya_data[kk];
      Xt_e_e = dist[kk];
      Yt_e_e = dist[kk + dist.size(0) * 3];
      Xt_w_e = rt_hypotd_snf(tag_pos_b[0].re - tag_pos_b[3].re,
                             tag_pos_b[0].im - tag_pos_b[3].im);
      Yt_w_e = dist[kk + dist.size(0)];
      Xt_s_e = dist[kk + dist.size(0) * 2];
      Yt_s_e = rt_hypotd_snf(tag_pos_b[1].re - tag_pos_b[2].re,
                             tag_pos_b[1].im - tag_pos_b[2].im);
      distc[static_cast<int>(qq) - 1] =
          (std::sqrt((Xt_e_e * Xt_e_e + Yt_e_e * Yt_e_e) / 2.0 +
                     Xt_w_e * Xt_w_e) +
           std::sqrt((Yt_w_e * Yt_w_e + Xt_s_e * Xt_s_e) / 2.0 +
                     Yt_s_e * Yt_s_e)) /
          2.0;
      // distc(qq) = mean(dist(kk,:));
      qq++;
    } else if ((dist[kk] != 0.0) && (dist[kk + dist.size(0) * 3] != 0.0)) {
      xat[static_cast<int>(qq) - 1] = xa_data[kk];
      yat[static_cast<int>(qq) - 1] = ya_data[kk];
      Xt_e_e = dist[kk];
      Yt_e_e = dist[kk + dist.size(0) * 3];
      Xt_w_e = rt_hypotd_snf(tag_pos_b[0].re - tag_pos_b[3].re,
                             tag_pos_b[0].im - tag_pos_b[3].im);
      distc[static_cast<int>(qq) - 1] = std::sqrt(
          (Xt_e_e * Xt_e_e + Yt_e_e * Yt_e_e) / 2.0 + Xt_w_e * Xt_w_e);
      // distc(qq) = mean(dist(kk,[1 4]));
      qq++;
    } else if ((dist[kk + dist.size(0)] != 0.0) &&
               (dist[kk + dist.size(0) * 2] != 0.0)) {
      xat[static_cast<int>(qq) - 1] = xa_data[kk];
      yat[static_cast<int>(qq) - 1] = ya_data[kk];
      Xt_e_e = dist[kk + dist.size(0)];
      Yt_e_e = dist[kk + dist.size(0) * 2];
      Xt_w_e = rt_hypotd_snf(tag_pos_b[1].re - tag_pos_b[2].re,
                             tag_pos_b[1].im - tag_pos_b[2].im);
      distc[static_cast<int>(qq) - 1] = std::sqrt(
          (Xt_e_e * Xt_e_e + Yt_e_e * Yt_e_e) / 2.0 + Xt_w_e * Xt_w_e);
      // distc(qq) = mean(dist(kk,[2 3]));
      qq++;
    }
  }
  if (xat.size(1) > 2) {
    Xc = TrilateralPos(xat, yat, distc, Yc);
    flagc = 1;
  } else {
    Xc = 0.0;
    Yc = 0.0;
  }
  //  %% Get Side Pos
  qq = 1U;
  rr = 1U;
  ss = 1U;
  tt = 1U;
  for (int kk{0}; kk < loop_ub_tmp; kk++) {
    Xt_e_e = dist[kk + dist.size(0)];
    if ((Xt_e_e != 0.0) && (dist[kk + dist.size(0) * 3] != 0.0)) {
      xae[static_cast<int>(qq) - 1] = xa_data[kk];
      yae[static_cast<int>(qq) - 1] = ya_data[kk];
      // diste(qq) = mean(dist(kk,[2 4]));
      Yt_e_e = dist[kk + dist.size(0) * 3];
      Xt_w_e = rt_hypotd_snf(tag_pos_b[1].re - tag_pos_b[3].re,
                             tag_pos_b[1].im - tag_pos_b[3].im);
      diste[static_cast<int>(qq) - 1] = std::sqrt(
          (Xt_e_e * Xt_e_e + Yt_e_e * Yt_e_e) / 2.0 + Xt_w_e * Xt_w_e);
      qq++;
    }
    Yt_w_e = dist[kk];
    if ((Yt_w_e != 0.0) && (dist[kk + dist.size(0) * 2] != 0.0)) {
      xaw[static_cast<int>(rr) - 1] = xa_data[kk];
      yaw[static_cast<int>(rr) - 1] = ya_data[kk];
      // distw(rr) = mean(dist(kk,[2 4]));
      Yt_e_e = dist[kk + dist.size(0) * 2];
      Xt_w_e = rt_hypotd_snf(tag_pos_b[0].re - tag_pos_b[2].re,
                             tag_pos_b[0].im - tag_pos_b[2].im);
      distw[static_cast<int>(rr) - 1] = std::sqrt(
          (Yt_w_e * Yt_w_e + Yt_e_e * Yt_e_e) / 2.0 + Xt_w_e * Xt_w_e);
      rr++;
    }
    if ((Yt_w_e != 0.0) && (Xt_e_e != 0.0)) {
      xan[static_cast<int>(ss) - 1] = xa_data[kk];
      yan[static_cast<int>(ss) - 1] = ya_data[kk];
      // dists(ss) = mean(dist(kk,[2 4]));
      Xt_w_e = rt_hypotd_snf(tag_pos_b[0].re - tag_pos_b[1].re,
                             tag_pos_b[0].im - tag_pos_b[1].im);
      distn[static_cast<int>(ss) - 1] = std::sqrt(
          (Yt_w_e * Yt_w_e + Xt_e_e * Xt_e_e) / 2.0 + Xt_w_e * Xt_w_e);
      ss++;
    }
    Xt_e_e = dist[kk + dist.size(0) * 2];
    if ((Xt_e_e != 0.0) && (dist[kk + dist.size(0) * 3] != 0.0)) {
      xas[static_cast<int>(tt) - 1] = xa_data[kk];
      yas[static_cast<int>(tt) - 1] = ya_data[kk];
      // dists(tt) = mean(dist(kk,[2 4]));
      Yt_e_e = dist[kk + dist.size(0) * 3];
      Xt_w_e = rt_hypotd_snf(tag_pos_b[2].re - tag_pos_b[3].re,
                             tag_pos_b[2].im - tag_pos_b[3].im);
      dists[static_cast<int>(tt) - 1] = std::sqrt(
          (Xt_e_e * Xt_e_e + Yt_e_e * Yt_e_e) / 2.0 + Xt_w_e * Xt_w_e);
      tt++;
    }
  }
  if (xas.size(1) > 2) {
    Xt_s_e = TrilateralPos(xas, yas, dists, Yt_s_e);
    flags = 1;
  } else {
    Xt_s_e = 0.0;
    Yt_s_e = 0.0;
  }
  if (xae.size(1) > 2) {
    Xt_e_e = TrilateralPos(xae, yae, diste, Yt_e_e);
    flage = 1;
  } else {
    Xt_e_e = 0.0;
    Yt_e_e = 0.0;
  }
  if (xaw.size(1) > 2) {
    Xt_w_e = TrilateralPos(xaw, yaw, distw, Yt_w_e);
    flagw = 1;
  } else {
    Xt_w_e = 0.0;
    Yt_w_e = 0.0;
  }
  if (xan.size(1) > 2) {
    Xt_n_e = TrilateralPos(xan, yan, distn, Yt_n_e);
    flagn = 1;
  } else {
    Xt_n_e = 0.0;
    Yt_n_e = 0.0;
  }
  if (flagc == 1) {
    heading = rt_atan2d_snf(Yt_e_e - Yc, Xt_e_e - Xc);
    flag = 1.0;
  } else if (flage * flagw == 1) {
    heading = rt_atan2d_snf(Yt_e_e - Yt_w_e, Xt_e_e - Xt_w_e);
    Xc = (Xt_e_e + Xt_w_e) / 2.0;
    Yc = (Yt_e_e + Yt_w_e) / 2.0;
    flag = 1.0;
  } else if (flagn * flags == 1) {
    heading =
        rt_atan2d_snf(Yt_n_e - Yt_s_e, Xt_n_e - Xt_s_e) - 1.5707963267948966;
    Xc = (Xt_n_e + Xt_s_e) / 2.0;
    Yc = (Yt_n_e + Yt_s_e) / 2.0;
    flag = 1.0;
  } else {
    Xc = 0.0;
    Yc = 0.0;
    heading = 0.0;
    flag = 0.0;
  }
  return Xc;
}

//
// File trailer for GetUWBPos_v4.cpp
//
// [EOF]
//
