//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: TwoAnchPos3.cpp
//
// MATLAB Coder version            : 5.6
// C/C++ source code generated on  : 17-Oct-2023 17:18:56
//

// Include Files
#include "TwoAnchPos3.h"
#include "UWBpos6_rtwutil.h"
#include "inv.h"
#include "rt_nonfinite.h"
#include "sum.h"
#include "coder_array.h"
#include <cmath>

// Function Declarations
static void binary_expand_op(coder::array<double, 2U> &in1,
                             const double in2_data[], const int in2_size[2],
                             const coder::array<double, 1U> &in3);

// Function Definitions
//
// Arguments    : coder::array<double, 2U> &in1
//                const double in2_data[]
//                const int in2_size[2]
//                const coder::array<double, 1U> &in3
// Return Type  : void
//
static void binary_expand_op(coder::array<double, 2U> &in1,
                             const double in2_data[], const int in2_size[2],
                             const coder::array<double, 1U> &in3)
{
  int loop_ub;
  int stride_0_1;
  int stride_1_1;
  in1.set_size(1, in1.size(1));
  if (in3.size(0) == 1) {
    loop_ub = in2_size[1];
  } else {
    loop_ub = in3.size(0);
  }
  in1.set_size(in1.size(0), loop_ub);
  stride_0_1 = (in2_size[1] != 1);
  stride_1_1 = (in3.size(0) != 1);
  for (int i{0}; i < loop_ub; i++) {
    in1[i] = in2_data[i * stride_0_1] - in3[i * stride_1_1];
  }
}

//
// Xa = [0 1];
//  Ya = [0 0];
//  dist = [1 1];
//
// Arguments    : const double Xa[2]
//                const double Ya[2]
//                const double dist[2]
//                const creal_T anch_pos_data[]
//                const int anch_pos_size[2]
//                const coder::array<double, 1U> &dist_a
//                double Pos[4]
//                double Prob[2]
// Return Type  : void
//
void TwoAnchPos3(const double Xa[2], const double Ya[2], const double dist[2],
                 const creal_T anch_pos_data[], const int anch_pos_size[2],
                 const coder::array<double, 1U> &dist_a, double Pos[4],
                 double Prob[2])
{
  coder::array<double, 2U> x;
  coder::array<double, 2U> y;
  double tmp_data[6];
  double A[4];
  double b_y_tmp[4];
  double y_tmp[4];
  double X1[2];
  double X2[2];
  double AA;
  double b_d;
  double b_d_idx_0_tmp;
  double b_d_tmp_tmp;
  double c_d_idx_0_tmp;
  double d;
  double d_idx_0;
  double d_idx_0_tmp;
  double d_idx_1_tmp;
  double d_tmp;
  double d_tmp_tmp;
  double s;
  int tmp_size[2];
  int nx;
  AA = Xa[0] - Xa[1];
  s = Ya[0] - Ya[1];
  AA = std::sqrt(AA * AA + s * s);
  //  AA = 2;B=1;C=0.9
  s = ((AA + dist[0]) + dist[1]) / 2.0;
  d = s - AA;
  d_tmp_tmp = s - dist[0];
  b_d_tmp_tmp = s - dist[1];
  d_tmp = s * d * d_tmp_tmp * b_d_tmp_tmp;
  if (d_tmp > 0.0) {
    b_d = std::sqrt(d_tmp);
  } else {
    b_d = std::sqrt(-s * d * d_tmp_tmp * b_d_tmp_tmp);
  }
  d = 2.0 * b_d / AA;
  //  if (B^2-((B^2-C^2+AA^2)/(2*AA))^2) > 0
  //      d = sqrt(B^2-((B^2-C^2+AA^2)/(2*AA))^2);
  //  else
  //      d = 0;
  //  end
  AA = Ya[1] - Ya[0];
  s = Xa[1] - Xa[0];
  A[1] = 2.0 * s;
  A[3] = 2.0 * AA;
  //  Y1 =
  //  [d*sqrt((Ya(2)-Ya(1))^2+(Xa(2)-Xa(1))^2)+Xa(1)*Ya(2)-Xa(2)*Ya(1);(C^2-B^2-Xa(1)^2+Xa(2)^2-Ya(1)^2+Ya(2)^2)];
  //  Y2 =
  //  [-d*sqrt((Ya(2)-Ya(1))^2+(Xa(2)-Xa(1))^2)+Xa(1)*Ya(2)-Xa(2)*Ya(1);(C^2-B^2-Xa(1)^2+Xa(2)^2-Ya(1)^2+Ya(2)^2)];
  y_tmp[0] = AA;
  y_tmp[1] = -s;
  y_tmp[2] = A[1];
  y_tmp[3] = A[3];
  b_d = A[1];
  d_tmp_tmp = A[3];
  for (int k{0}; k < 2; k++) {
    b_d_tmp_tmp = y_tmp[k + 2];
    d_tmp = y_tmp[k];
    b_y_tmp[k] = d_tmp * AA + b_d_tmp_tmp * b_d;
    b_y_tmp[k + 2] = d_tmp * -s + b_d_tmp_tmp * d_tmp_tmp;
  }
  coder::inv(b_y_tmp, A);
  d_idx_0_tmp = std::sqrt(AA * AA + s * s);
  b_d_idx_0_tmp = Xa[0] * Ya[1];
  c_d_idx_0_tmp = Ya[0] * Xa[1];
  d_idx_0 = (d * d_idx_0_tmp + b_d_idx_0_tmp) - c_d_idx_0_tmp;
  d_idx_1_tmp = ((((-(dist[1] * dist[1]) + dist[0] * dist[0]) - Xa[0] * Xa[0]) +
                  Xa[1] * Xa[1]) -
                 Ya[0] * Ya[0]) +
                Ya[1] * Ya[1];
  b_d = y_tmp[2];
  d_tmp_tmp = y_tmp[3];
  for (int k{0}; k < 2; k++) {
    b_d_tmp_tmp = A[k + 2];
    d_tmp = A[k];
    X1[k] = (d_tmp * AA + b_d_tmp_tmp * -s) * d_idx_0 +
            (d_tmp * b_d + b_d_tmp_tmp * d_tmp_tmp) * d_idx_1_tmp;
  }
  coder::inv(b_y_tmp, A);
  d_idx_0 = (-d * d_idx_0_tmp + b_d_idx_0_tmp) - c_d_idx_0_tmp;
  b_d = y_tmp[2];
  d_tmp_tmp = y_tmp[3];
  for (int k{0}; k < 2; k++) {
    b_d_tmp_tmp = A[k + 2];
    d_tmp = A[k];
    X2[k] = (d_tmp * AA + b_d_tmp_tmp * -s) * d_idx_0 +
            (d_tmp * b_d + b_d_tmp_tmp * d_tmp_tmp) * d_idx_1_tmp;
  }
  AA = X1[0] + 0.0 * X1[1];
  s = X1[1];
  nx = anch_pos_size[1];
  tmp_size[0] = 1;
  tmp_size[1] = anch_pos_size[1];
  for (int k{0}; k < nx; k++) {
    tmp_data[k] =
        rt_hypotd_snf(anch_pos_data[k].re - AA, anch_pos_data[k].im - s);
  }
  if (dist_a.size(0) == anch_pos_size[1]) {
    x.set_size(1, anch_pos_size[1]);
    nx = anch_pos_size[1];
    for (int k{0}; k < nx; k++) {
      x[k] = tmp_data[k] - dist_a[k];
    }
  } else {
    binary_expand_op(x, tmp_data, tmp_size, dist_a);
  }
  nx = x.size(1);
  y.set_size(1, x.size(1));
  for (int k{0}; k < nx; k++) {
    y[k] = std::abs(x[k]);
  }
  d = coder::sum(y);
  AA = X2[0] + 0.0 * X2[1];
  s = X2[1];
  nx = anch_pos_size[1];
  tmp_size[0] = 1;
  tmp_size[1] = anch_pos_size[1];
  for (int k{0}; k < nx; k++) {
    tmp_data[k] =
        rt_hypotd_snf(anch_pos_data[k].re - AA, anch_pos_data[k].im - s);
  }
  if (dist_a.size(0) == anch_pos_size[1]) {
    x.set_size(1, anch_pos_size[1]);
    nx = anch_pos_size[1];
    for (int k{0}; k < nx; k++) {
      x[k] = tmp_data[k] - dist_a[k];
    }
  } else {
    binary_expand_op(x, tmp_data, tmp_size, dist_a);
  }
  nx = x.size(1);
  y.set_size(1, x.size(1));
  for (int k{0}; k < nx; k++) {
    y[k] = std::abs(x[k]);
  }
  b_d = coder::sum(y) + (X2[0] * X2[0] + X2[1] * X2[1]);
  d_tmp_tmp = d + (X1[0] * X1[0] + X1[1] * X1[1]);
  if (d_tmp_tmp > b_d) {
    Pos[0] = X2[0];
    Pos[1] = X1[0];
    Pos[2] = X2[1];
    Pos[3] = X1[1];
    Prob[0] = b_d;
    Prob[1] = d_tmp_tmp;
  } else {
    Pos[0] = X1[0];
    Pos[1] = X2[0];
    Pos[2] = X1[1];
    Pos[3] = X2[1];
    Prob[0] = d_tmp_tmp;
    Prob[1] = b_d;
  }
  //  if (X1S+Y1S)-(X2S+Y2S)>0.1
  //      Pos = [X2'];
  //  elseif ((X1S+Y1S)-(X2S+Y2S))<-0.1
  //      Pos = [X1'];
  //  elseif Z1S < Z2S
  //      Pos = [X1'];
  //  else
  //      Pos = [X2'];
  //  end
  //  if sum((X1-EstCenter').^2)>sum((X2-EstCenter').^2)
  //      Pos = [X2'];
  //  else
  //      Pos = [X1'];
  //  end
}

//
// File trailer for TwoAnchPos3.cpp
//
// [EOF]
//
