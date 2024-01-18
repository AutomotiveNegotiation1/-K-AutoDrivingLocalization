//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: TrilateralPos.cpp
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 30-Nov-2023 16:43:53
//

// Include Files
#include "TrilateralPos.h"
#include "inv.h"
#include "rt_nonfinite.h"
#include "coder_array.h"

// Function Definitions
//
// Arguments    : const coder::array<double, 2U> &xat
//                const coder::array<double, 2U> &yat
//                const coder::array<double, 2U> &dists
//                double *Xc
//                double *Yc
// Return Type  : void
//
void TrilateralPos(const coder::array<double, 2U> &xat,
                   const coder::array<double, 2U> &yat,
                   const coder::array<double, 2U> &dists, double *Xc,
                   double *Yc)
{
  coder::array<double, 2U> A;
  coder::array<double, 2U> b_y;
  coder::array<double, 1U> Y;
  double a[4];
  double y[4];
  double Res_idx_1;
  double bkj;
  unsigned int Li;
  int Li_tmp;
  int boffset;
  int coffset;
  int j;
  int n;
  A.set_size(static_cast<int>(static_cast<double>(xat.size(1)) *
                              (static_cast<double>(xat.size(1)) - 1.0) / 2.0),
             2);
  boffset = static_cast<int>(static_cast<double>(xat.size(1)) *
                             (static_cast<double>(xat.size(1)) - 1.0) / 2.0)
            << 1;
  for (j = 0; j < boffset; j++) {
    A[j] = 0.0;
  }
  Y.set_size(static_cast<int>(static_cast<double>(xat.size(1)) *
                              (static_cast<double>(xat.size(1)) - 1.0) / 2.0));
  boffset = static_cast<int>(static_cast<double>(xat.size(1)) *
                             (static_cast<double>(xat.size(1)) - 1.0) / 2.0);
  for (j = 0; j < boffset; j++) {
    Y[j] = 0.0;
  }
  Li = 1U;
  j = xat.size(1);
  for (n = 0; n < j; n++) {
    boffset = xat.size(1) - n;
    for (coffset = 0; coffset <= boffset - 2; coffset++) {
      double Y_tmp;
      unsigned int m;
      m = (static_cast<unsigned int>(n) + coffset) + 2U;
      Li_tmp = static_cast<int>(Li + coffset) - 1;
      bkj = xat[static_cast<int>(m) - 1];
      A[Li_tmp] = 2.0 * (xat[n] - bkj);
      Res_idx_1 = yat[static_cast<int>(m) - 1];
      A[Li_tmp + A.size(0)] = 2.0 * (yat[n] - Res_idx_1);
      Y_tmp = dists[static_cast<int>(m) - 1];
      Y[Li_tmp] = ((((dists[n] * dists[n] - Y_tmp * Y_tmp) - xat[n] * xat[n]) +
                    bkj * bkj) -
                   yat[n] * yat[n]) +
                  Res_idx_1 * Res_idx_1;
    }
    if (boffset - 2 >= 0) {
      Li = (Li + boffset) - 1U;
    }
  }
  // %%% Calc. Tag position %%%%%%%
  Li_tmp = A.size(0);
  for (j = 0; j < 2; j++) {
    coffset = j << 1;
    boffset = j * A.size(0);
    y[coffset] = 0.0;
    y[coffset + 1] = 0.0;
    for (n = 0; n < Li_tmp; n++) {
      bkj = A[boffset + n];
      y[coffset] += A[n] * bkj;
      y[coffset + 1] += A[A.size(0) + n] * bkj;
    }
  }
  coder::inv(y, a);
  n = A.size(0);
  b_y.set_size(2, A.size(0));
  for (j = 0; j < n; j++) {
    coffset = j << 1;
    b_y[coffset] = a[0] * A[j] + a[2] * A[A.size(0) + j];
    b_y[coffset + 1] = a[1] * A[j] + a[3] * A[A.size(0) + j];
  }
  Li_tmp = b_y.size(1);
  bkj = 0.0;
  Res_idx_1 = 0.0;
  for (n = 0; n < Li_tmp; n++) {
    boffset = n << 1;
    bkj += b_y[boffset] * Y[n];
    Res_idx_1 += b_y[boffset + 1] * Y[n];
  }
  *Xc = -bkj;
  *Yc = -Res_idx_1;
}

//
// File trailer for TrilateralPos.cpp
//
// [EOF]
//
