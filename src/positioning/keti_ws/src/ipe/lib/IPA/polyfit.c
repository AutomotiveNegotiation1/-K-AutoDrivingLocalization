/*
 * polyfit.c
 *
 * Code generation for function 'polyfit'
 *
 */

/* Include files */
#include "polyfit.h"
#include "PositioningSystem_V2_3_rtwutil.h"
#include "rt_nonfinite.h"
#include "xnrm2.h"
#include <math.h>
#include <string.h>

/* Function Declarations */
static int div_nde_s32_floor(int numerator);

/* Function Definitions */
static int div_nde_s32_floor(int numerator)
{
  int i;
  if ((numerator < 0) && (numerator % 10 != 0)) {
    i = -1;
  } else {
    i = 0;
  }
  return numerator / 10 + i;
}

void polyfit(const double x[10], const double y[10], double p[3])
{
  double V[30];
  double B[10];
  double tau[3];
  double vn1[3];
  double vn2[3];
  double work[3];
  double absxk;
  double scale;
  double smax;
  double t;
  int assumedRank;
  int b_i;
  int i;
  int ix0;
  int iy;
  int j;
  int kend;
  int lastc;
  signed char jpvt[3];
  for (lastc = 0; lastc < 10; lastc++) {
    V[lastc + 20] = 1.0;
    absxk = x[lastc];
    V[lastc + 10] = absxk;
    V[lastc] = absxk * absxk;
    B[lastc] = y[lastc];
  }
  for (lastc = 0; lastc < 3; lastc++) {
    jpvt[lastc] = (signed char)(lastc + 1);
    tau[lastc] = 0.0;
    work[lastc] = 0.0;
    ix0 = lastc * 10;
    smax = 0.0;
    scale = 3.3121686421112381E-170;
    kend = ix0 + 10;
    for (iy = ix0 + 1; iy <= kend; iy++) {
      absxk = fabs(V[iy - 1]);
      if (absxk > scale) {
        t = scale / absxk;
        smax = smax * t * t + 1.0;
        scale = absxk;
      } else {
        t = absxk / scale;
        smax += t * t;
      }
    }
    absxk = scale * sqrt(smax);
    vn1[lastc] = absxk;
    vn2[lastc] = absxk;
  }
  for (i = 0; i < 3; i++) {
    int ii;
    int ip1;
    int pvt;
    ip1 = i + 2;
    ii = i * 10 + i;
    kend = 3 - i;
    iy = 0;
    if (3 - i > 1) {
      smax = fabs(vn1[i]);
      for (lastc = 2; lastc <= kend; lastc++) {
        scale = fabs(vn1[(i + lastc) - 1]);
        if (scale > smax) {
          iy = lastc - 1;
          smax = scale;
        }
      }
    }
    pvt = i + iy;
    if (pvt != i) {
      kend = pvt * 10;
      iy = i * 10;
      for (lastc = 0; lastc < 10; lastc++) {
        assumedRank = kend + lastc;
        smax = V[assumedRank];
        ix0 = iy + lastc;
        V[assumedRank] = V[ix0];
        V[ix0] = smax;
      }
      kend = jpvt[pvt];
      jpvt[pvt] = jpvt[i];
      jpvt[i] = (signed char)kend;
      vn1[pvt] = vn1[i];
      vn2[pvt] = vn2[i];
    }
    t = V[ii];
    ix0 = ii + 2;
    tau[i] = 0.0;
    smax = xnrm2(9 - i, V, ii + 2);
    if (smax != 0.0) {
      absxk = V[ii];
      scale = rt_hypotd_snf(absxk, smax);
      if (absxk >= 0.0) {
        scale = -scale;
      }
      if (fabs(scale) < 1.0020841800044864E-292) {
        kend = 0;
        b_i = (ii - i) + 10;
        do {
          kend++;
          for (lastc = ix0; lastc <= b_i; lastc++) {
            V[lastc - 1] *= 9.9792015476736E+291;
          }
          scale *= 9.9792015476736E+291;
          t *= 9.9792015476736E+291;
        } while ((fabs(scale) < 1.0020841800044864E-292) && (kend < 20));
        scale = rt_hypotd_snf(t, xnrm2(9 - i, V, ii + 2));
        if (t >= 0.0) {
          scale = -scale;
        }
        tau[i] = (scale - t) / scale;
        smax = 1.0 / (t - scale);
        for (lastc = ix0; lastc <= b_i; lastc++) {
          V[lastc - 1] *= smax;
        }
        for (lastc = 0; lastc < kend; lastc++) {
          scale *= 1.0020841800044864E-292;
        }
        t = scale;
      } else {
        tau[i] = (scale - absxk) / scale;
        smax = 1.0 / (absxk - scale);
        b_i = (ii - i) + 10;
        for (lastc = ix0; lastc <= b_i; lastc++) {
          V[lastc - 1] *= smax;
        }
        t = scale;
      }
    }
    V[ii] = t;
    if (i + 1 < 3) {
      int lastv;
      V[ii] = 1.0;
      iy = ii + 11;
      if (tau[i] != 0.0) {
        boolean_T exitg2;
        lastv = 10 - i;
        kend = (ii - i) + 9;
        while ((lastv > 0) && (V[kend] == 0.0)) {
          lastv--;
          kend--;
        }
        lastc = 1 - i;
        exitg2 = false;
        while ((!exitg2) && (lastc + 1 > 0)) {
          int exitg1;
          kend = (ii + lastc * 10) + 10;
          assumedRank = kend;
          do {
            exitg1 = 0;
            if (assumedRank + 1 <= kend + lastv) {
              if (V[assumedRank] != 0.0) {
                exitg1 = 1;
              } else {
                assumedRank++;
              }
            } else {
              lastc--;
              exitg1 = 2;
            }
          } while (exitg1 == 0);
          if (exitg1 == 1) {
            exitg2 = true;
          }
        }
      } else {
        lastv = 0;
        lastc = -1;
      }
      if (lastv > 0) {
        if (lastc + 1 != 0) {
          memset(&work[0], 0, (unsigned int)(lastc + 1) * sizeof(double));
          b_i = (ii + 10 * lastc) + 11;
          for (ix0 = iy; ix0 <= b_i; ix0 += 10) {
            smax = 0.0;
            pvt = (ix0 + lastv) - 1;
            for (assumedRank = ix0; assumedRank <= pvt; assumedRank++) {
              smax += V[assumedRank - 1] * V[(ii + assumedRank) - ix0];
            }
            kend = div_nde_s32_floor((ix0 - ii) - 11);
            work[kend] += smax;
          }
        }
        if (!(-tau[i] == 0.0)) {
          kend = ii;
          for (j = 0; j <= lastc; j++) {
            absxk = work[j];
            if (absxk != 0.0) {
              smax = absxk * -tau[i];
              b_i = kend + 11;
              pvt = lastv + kend;
              for (assumedRank = b_i; assumedRank <= pvt + 10; assumedRank++) {
                V[assumedRank - 1] +=
                    V[((ii + assumedRank) - kend) - 11] * smax;
              }
            }
            kend += 10;
          }
        }
      }
      V[ii] = t;
    }
    for (j = ip1; j < 4; j++) {
      kend = i + (j - 1) * 10;
      absxk = vn1[j - 1];
      if (absxk != 0.0) {
        smax = fabs(V[kend]) / absxk;
        smax = 1.0 - smax * smax;
        if (smax < 0.0) {
          smax = 0.0;
        }
        scale = absxk / vn2[j - 1];
        scale = smax * (scale * scale);
        if (scale <= 1.4901161193847656E-8) {
          absxk = xnrm2(9 - i, V, kend + 2);
          vn1[j - 1] = absxk;
          vn2[j - 1] = absxk;
        } else {
          vn1[j - 1] = absxk * sqrt(smax);
        }
      }
    }
  }
  assumedRank = 0;
  for (lastc = 0; lastc < 3; lastc++) {
    if (V[lastc + 10 * lastc] != 0.0) {
      assumedRank++;
    }
    p[lastc] = 0.0;
    if (tau[lastc] != 0.0) {
      smax = B[lastc];
      b_i = lastc + 2;
      for (i = b_i; i < 11; i++) {
        smax += V[(i + 10 * lastc) - 1] * B[i - 1];
      }
      smax *= tau[lastc];
      if (smax != 0.0) {
        B[lastc] -= smax;
        for (i = b_i; i < 11; i++) {
          B[i - 1] -= V[(i + 10 * lastc) - 1] * smax;
        }
      }
    }
  }
  for (i = 0; i < assumedRank; i++) {
    p[jpvt[i] - 1] = B[i];
  }
  for (j = assumedRank; j >= 1; j--) {
    kend = jpvt[j - 1] - 1;
    iy = 10 * (j - 1);
    p[kend] /= V[(j + iy) - 1];
    for (i = 0; i <= j - 2; i++) {
      ix0 = jpvt[i] - 1;
      p[ix0] -= p[kend] * V[i + iy];
    }
  }
}

/* End of code generation (polyfit.c) */
