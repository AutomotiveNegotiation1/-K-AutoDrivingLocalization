/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: UWBpos.c
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 01-Aug-2023 16:36:28
 */

/* Include Files */
#include "UWBpos.h"
#include "GetPosRefine2.h"
#include "TestTotal_data.h"
#include "TestTotal_rtwutil.h"
#include "TwoAnchPos3.h"
#include "dec2bin.h"
#include "exp.h"
#include "find.h"
#include "inv.h"
#include "mod.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Variable Definitions */
static double r;

static creal_T InterpPosition[8];

static creal_T Tag_Pos_List[72];

static double TagDistInitCount[24];

static double TagDistInit[24];

static double heading_est_a[20];

static double centerest_a[40];

static double centerest_a_aver[40];

static double headingest_a_aver[20];

/* Function Declarations */
static double rt_powd_snf(double u0, double u1);

/* Function Definitions */
/*
 * Arguments    : double u0
 *                double u1
 * Return Type  : double
 */
static double rt_powd_snf(double u0, double u1)
{
  double y;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = rtNaN;
  } else {
    double d;
    double d1;
    d = fabs(u0);
    d1 = fabs(u1);
    if (rtIsInf(u1)) {
      if (d == 1.0) {
        y = 1.0;
      } else if (d > 1.0) {
        if (u1 > 0.0) {
          y = rtInf;
        } else {
          y = 0.0;
        }
      } else if (u1 > 0.0) {
        y = 0.0;
      } else {
        y = rtInf;
      }
    } else if (d1 == 0.0) {
      y = 1.0;
    } else if (d1 == 1.0) {
      if (u1 > 0.0) {
        y = u0;
      } else {
        y = 1.0 / u0;
      }
    } else if (u1 == 2.0) {
      y = u0 * u0;
    } else if ((u1 == 0.5) && (u0 >= 0.0)) {
      y = sqrt(u0);
    } else if ((u0 < 0.0) && (u1 > floor(u1))) {
      y = rtNaN;
    } else {
      y = pow(u0, u1);
    }
  }
  return y;
}

/*
 * Arguments    : double TagNum
 *                double Nanchor
 *                const double RxID_data[]
 *                const int RxID_size[2]
 *                const double RxDist_data[]
 *                int RxDist_size
 *                double s_time
 *                creal_T tag_pos_est[4]
 *                creal_T tag_pos_est_aver[4]
 *                double *headingest_a_aver_v
 * Return Type  : double
 */
double UWBpos(double TagNum, double Nanchor, const double RxID_data[],
              const int RxID_size[2], const double RxDist_data[],
              int RxDist_size, double s_time, creal_T tag_pos_est[4],
              creal_T tag_pos_est_aver[4], double *headingest_a_aver_v)
{
  static const signed char iv[6] = {10, 10, -10, -10, 20, -20};
  static const signed char iv1[6] = {10, -10, -10, 10, 15, -15};
  creal_T Pos2C[8];
  creal_T tmp_data[4];
  creal_T CandPos;
  creal_T MeanA;
  double Est_F[2];
  double Est_H[2];
  double RxDist[2];
  double heading_est;
  int IndT_data[6];
  int tmp_size[2];
  int IndT_size;
  int L1;
  int L2;
  int PPC;
  int i;
  int i1;
  int i2;
  int j2;
  int k;
  int loop_ub;
  char Va_data[64];
  char s_data[64];
  r++;
  heading_est = 0.0;
  memset(&tag_pos_est[0], 0, 4U * sizeof(creal_T));
  memset(&tag_pos_est_aver[0], 0, 4U * sizeof(creal_T));
  *headingest_a_aver_v = 0.0;
  if (r < 40.0) {
    i = (int)Nanchor;
    for (PPC = 0; PPC < i; PPC++) {
      double MeanB_head;
      /*              for PP = mod(r-1,Lp)+1:mod(r-1,Lp)+1 */
      j2 = ((int)RxID_data[PPC] + 6 * ((int)TagNum - 1)) - 1;
      TagDistInitCount[j2]++;
      MeanB_head = TagDistInitCount[j2];
      TagDistInit[j2] = TagDistInit[j2] * (MeanB_head - 1.0) / MeanB_head +
                        RxDist_data[PPC] / MeanB_head;
      /*              end */
    }
  } else if (r == 40.0) {
    double DistT[24];
    double DistT_data[24];
    double y[6];
    double MeanB_head;
    double Px_re_tmp;
    double Sel_C_im;
    double Sel_C_re;
    double Sel_F;
    double b_Px_re_tmp;
    double d;
    signed char xa_data[6];
    signed char ya_data[6];
    bool b_y[6];
    i = (int)Nanchor;
    for (PPC = 0; PPC < i; PPC++) {
      /*              for PP = mod(r-1,Lp)+1:mod(r-1,Lp)+1 */
      j2 = ((int)RxID_data[PPC] + 6 * ((int)TagNum - 1)) - 1;
      TagDistInitCount[j2]++;
      MeanB_head = TagDistInitCount[j2];
      TagDistInit[j2] = TagDistInit[j2] * (MeanB_head - 1.0) / MeanB_head +
                        RxDist_data[PPC] / MeanB_head;
      /*              end */
    }
    for (j2 = 0; j2 < 24; j2++) {
      d = TagDistInit[j2];
      DistT[j2] = d;
      if (d == 0.0) {
        DistT[j2] = 1.0E+6;
      }
    }
    for (PPC = 0; PPC < 6; PPC++) {
      y[PPC] = DistT[PPC];
    }
    for (k = 0; k < 3; k++) {
      j2 = (k + 1) * 6;
      for (PPC = 0; PPC < 6; PPC++) {
        y[PPC] += DistT[j2 + PPC];
      }
    }
    for (i = 0; i < 6; i++) {
      b_y[i] = (y[i] < 100000.0);
    }
    IndT_size = eml_find(b_y, IndT_data);
    for (i = 0; i < 4; i++) {
      for (i1 = 0; i1 < IndT_size; i1++) {
        DistT_data[i1 + IndT_size * i] = DistT[(IndT_data[i1] + 6 * i) - 1];
      }
    }
    for (i = 0; i < IndT_size; i++) {
      i1 = IndT_data[i];
      xa_data[i] = iv[i1 - 1];
      ya_data[i] = iv1[i1 - 1];
    }
    Sel_F = 1.0E+7;
    Sel_C_re = 0.0;
    Sel_C_im = 0.0;
    heading_est = 0.0;
    for (L1 = 0; L1 < IndT_size; L1++) {
      i = IndT_size - L1;
      if (i - 2 >= 0) {
        loop_ub = IndT_size;
      }
      for (L2 = 0; L2 <= i - 2; L2++) {
        creal_T Est_C[16];
        double b_Est_F[16];
        double b_Est_H[16];
        j2 = (L1 + L2) + 1;
        for (PPC = 0; PPC < 4; PPC++) {
          creal_T b_xa_data[6];
          Est_F[0] = xa_data[L1];
          Est_F[1] = xa_data[j2];
          Est_H[0] = ya_data[L1];
          Est_H[1] = ya_data[j2];
          RxDist[0] = DistT[(IndT_data[L1] + 6 * PPC) - 1];
          RxDist[1] = DistT[(IndT_data[j2] + 6 * PPC) - 1];
          tmp_size[0] = 1;
          tmp_size[1] = IndT_size;
          for (i1 = 0; i1 < loop_ub; i1++) {
            b_xa_data[i1].re = xa_data[i1];
            b_xa_data[i1].im = ya_data[i1];
            y[i1] = DistT_data[i1 + IndT_size * PPC];
          }
          double Pos2[4];
          double Prob2[2];
          TwoAnchPos3(Est_F, Est_H, RxDist, b_xa_data, tmp_size, y, IndT_size,
                      Pos2, Prob2);
          Pos2C[PPC].re = Pos2[0];
          Pos2C[PPC].im = Pos2[2];
          Pos2C[PPC + 4].re = Pos2[1];
          Pos2C[PPC + 4].im = Pos2[3];
        }
        for (k = 0; k < 16; k++) {
          double ia_data[64];
          double Est_H_p_im;
          double Est_H_p_re;
          double Pm_im;
          double Pm_re_tmp;
          double ai_tmp;
          double ar_tmp;
          double b_ai_tmp;
          double b_ar_tmp;
          double c_Px_re_tmp;
          double c_ai_tmp;
          double c_ar_tmp;
          double d_Px_re_tmp;
          double d_ai_tmp;
          double im;
          double re;
          int s_size[2];
          dec2bin(k, Va_data, s_size);
          j2 = s_size[1];
          for (PPC = 0; PPC < j2; PPC++) {
            if (Va_data[PPC] == '0') {
              ia_data[PPC] = 1.0;
            } else if (Va_data[PPC] == '1') {
              ia_data[PPC] = 2.0;
            } else {
              ia_data[PPC] = 1.0;
            }
          }
          /*  NB = length(tag_pos_b); */
          PPC = ((int)ia_data[0] - 1) << 2;
          Pm_im = Pos2C[PPC].re;
          ai_tmp = Pos2C[PPC].im;
          if (ai_tmp == 0.0) {
            i1 = s_size[1];
            c_Px_re_tmp = Pm_im / (double)s_size[1];
            MeanB_head = 0.0;
          } else if (Pm_im == 0.0) {
            c_Px_re_tmp = 0.0;
            i1 = s_size[1];
            MeanB_head = ai_tmp / (double)s_size[1];
          } else {
            i1 = s_size[1];
            c_Px_re_tmp = Pm_im / (double)s_size[1];
            MeanB_head = ai_tmp / (double)s_size[1];
          }
          re = c_Px_re_tmp;
          im = MeanB_head;
          PPC = (((int)ia_data[1] - 1) << 2) + 1;
          ar_tmp = Pos2C[PPC].re;
          b_ai_tmp = Pos2C[PPC].im;
          if (b_ai_tmp == 0.0) {
            c_Px_re_tmp = ar_tmp / (double)i1;
            MeanB_head = 0.0;
          } else if (ar_tmp == 0.0) {
            c_Px_re_tmp = 0.0;
            MeanB_head = b_ai_tmp / (double)i1;
          } else {
            c_Px_re_tmp = ar_tmp / (double)i1;
            MeanB_head = b_ai_tmp / (double)i1;
          }
          re += c_Px_re_tmp;
          im += MeanB_head;
          PPC = (((int)ia_data[2] - 1) << 2) + 2;
          b_ar_tmp = Pos2C[PPC].re;
          c_ai_tmp = Pos2C[PPC].im;
          if (c_ai_tmp == 0.0) {
            c_Px_re_tmp = b_ar_tmp / (double)i1;
            MeanB_head = 0.0;
          } else if (b_ar_tmp == 0.0) {
            c_Px_re_tmp = 0.0;
            MeanB_head = c_ai_tmp / (double)i1;
          } else {
            c_Px_re_tmp = b_ar_tmp / (double)i1;
            MeanB_head = c_ai_tmp / (double)i1;
          }
          re += c_Px_re_tmp;
          im += MeanB_head;
          PPC = (((int)ia_data[3] - 1) << 2) + 3;
          c_ar_tmp = Pos2C[PPC].re;
          d_ai_tmp = Pos2C[PPC].im;
          if (d_ai_tmp == 0.0) {
            c_Px_re_tmp = c_ar_tmp / (double)i1;
            MeanB_head = 0.0;
          } else if (c_ar_tmp == 0.0) {
            c_Px_re_tmp = 0.0;
            MeanB_head = d_ai_tmp / (double)i1;
          } else {
            c_Px_re_tmp = c_ar_tmp / (double)i1;
            MeanB_head = d_ai_tmp / (double)i1;
          }
          re += c_Px_re_tmp;
          im += MeanB_head;
          d_Px_re_tmp = Pm_im - re;
          b_Px_re_tmp = ai_tmp - im;
          if (dcv[0].im == 0.0) {
            if (b_Px_re_tmp == 0.0) {
              d = dcv[0].re;
              c_Px_re_tmp = d_Px_re_tmp / dcv[0].re;
              MeanB_head = 0.0;
            } else if (d_Px_re_tmp == 0.0) {
              c_Px_re_tmp = 0.0;
              d = dcv[0].re;
              MeanB_head = b_Px_re_tmp / dcv[0].re;
            } else {
              d = dcv[0].re;
              c_Px_re_tmp = d_Px_re_tmp / dcv[0].re;
              MeanB_head = b_Px_re_tmp / dcv[0].re;
            }
          } else {
            d = dcv[0].re;
            if (dcv[0].re == 0.0) {
              if (d_Px_re_tmp == 0.0) {
                c_Px_re_tmp = b_Px_re_tmp / dcv[0].im;
                MeanB_head = 0.0;
              } else if (b_Px_re_tmp == 0.0) {
                c_Px_re_tmp = 0.0;
                MeanB_head = -(d_Px_re_tmp / dcv[0].im);
              } else {
                c_Px_re_tmp = b_Px_re_tmp / dcv[0].im;
                MeanB_head = -(d_Px_re_tmp / dcv[0].im);
              }
            } else {
              Px_re_tmp = fabs(dcv[0].re);
              MeanB_head = fabs(dcv[0].im);
              if (Px_re_tmp > MeanB_head) {
                MeanB_head = dcv[0].im / dcv[0].re;
                Pm_re_tmp = dcv[0].re + MeanB_head * dcv[0].im;
                c_Px_re_tmp =
                    (d_Px_re_tmp + MeanB_head * b_Px_re_tmp) / Pm_re_tmp;
                MeanB_head =
                    (b_Px_re_tmp - MeanB_head * d_Px_re_tmp) / Pm_re_tmp;
              } else if (MeanB_head == Px_re_tmp) {
                if (dcv[0].re > 0.0) {
                  MeanB_head = 0.5;
                } else {
                  MeanB_head = -0.5;
                }
                if (dcv[0].im > 0.0) {
                  Pm_re_tmp = 0.5;
                } else {
                  Pm_re_tmp = -0.5;
                }
                c_Px_re_tmp =
                    (d_Px_re_tmp * MeanB_head + b_Px_re_tmp * Pm_re_tmp) /
                    Px_re_tmp;
                MeanB_head =
                    (b_Px_re_tmp * MeanB_head - d_Px_re_tmp * Pm_re_tmp) /
                    Px_re_tmp;
              } else {
                MeanB_head = dcv[0].re / dcv[0].im;
                Pm_re_tmp = dcv[0].im + MeanB_head * dcv[0].re;
                c_Px_re_tmp =
                    (MeanB_head * d_Px_re_tmp + b_Px_re_tmp) / Pm_re_tmp;
                MeanB_head =
                    (MeanB_head * b_Px_re_tmp - d_Px_re_tmp) / Pm_re_tmp;
              }
            }
          }
          Est_H_p_re = c_Px_re_tmp;
          Est_H_p_im = MeanB_head;
          d_Px_re_tmp = ar_tmp - re;
          b_Px_re_tmp = b_ai_tmp - im;
          if (dcv[0].im == 0.0) {
            if (b_Px_re_tmp == 0.0) {
              c_Px_re_tmp = d_Px_re_tmp / dcv[0].im;
              MeanB_head = 0.0;
            } else if (d_Px_re_tmp == 0.0) {
              c_Px_re_tmp = 0.0;
              MeanB_head = b_Px_re_tmp / dcv[0].im;
            } else {
              c_Px_re_tmp = d_Px_re_tmp / dcv[0].im;
              MeanB_head = b_Px_re_tmp / dcv[0].im;
            }
          } else if (dcv[0].im == 0.0) {
            if (d_Px_re_tmp == 0.0) {
              c_Px_re_tmp = b_Px_re_tmp / dcv[0].im;
              MeanB_head = 0.0;
            } else if (b_Px_re_tmp == 0.0) {
              c_Px_re_tmp = 0.0;
              MeanB_head = -(d_Px_re_tmp / dcv[0].im);
            } else {
              c_Px_re_tmp = b_Px_re_tmp / dcv[0].im;
              MeanB_head = -(d_Px_re_tmp / dcv[0].im);
            }
          } else {
            Px_re_tmp = fabs(dcv[0].im);
            if (dcv[0].im > 0.0) {
              MeanB_head = 0.5;
            } else {
              MeanB_head = -0.5;
            }
            Pm_re_tmp = b_Px_re_tmp * MeanB_head;
            MeanB_head *= d_Px_re_tmp;
            c_Px_re_tmp = (MeanB_head + Pm_re_tmp) / Px_re_tmp;
            MeanB_head = (Pm_re_tmp - MeanB_head) / Px_re_tmp;
          }
          Est_H_p_re += c_Px_re_tmp;
          Est_H_p_im += MeanB_head;
          d_Px_re_tmp = b_ar_tmp - re;
          b_Px_re_tmp = c_ai_tmp - im;
          if (d == 0.0) {
            if (b_Px_re_tmp == 0.0) {
              c_Px_re_tmp = d_Px_re_tmp / d;
              MeanB_head = 0.0;
            } else if (d_Px_re_tmp == 0.0) {
              c_Px_re_tmp = 0.0;
              MeanB_head = b_Px_re_tmp / d;
            } else {
              c_Px_re_tmp = d_Px_re_tmp / d;
              MeanB_head = b_Px_re_tmp / d;
            }
          } else if (d == 0.0) {
            if (d_Px_re_tmp == 0.0) {
              c_Px_re_tmp = b_Px_re_tmp / d;
              MeanB_head = 0.0;
            } else if (b_Px_re_tmp == 0.0) {
              c_Px_re_tmp = 0.0;
              MeanB_head = -(d_Px_re_tmp / d);
            } else {
              c_Px_re_tmp = b_Px_re_tmp / d;
              MeanB_head = -(d_Px_re_tmp / d);
            }
          } else {
            Px_re_tmp = fabs(d);
            if (d > 0.0) {
              MeanB_head = 0.5;
            } else {
              MeanB_head = -0.5;
            }
            Pm_re_tmp = b_Px_re_tmp * MeanB_head;
            MeanB_head *= d_Px_re_tmp;
            c_Px_re_tmp = (MeanB_head + Pm_re_tmp) / Px_re_tmp;
            MeanB_head = (Pm_re_tmp - MeanB_head) / Px_re_tmp;
          }
          Est_H_p_re += c_Px_re_tmp;
          Est_H_p_im += MeanB_head;
          d_Px_re_tmp = c_ar_tmp - re;
          b_Px_re_tmp = d_ai_tmp - im;
          if (d == 0.0) {
            if (b_Px_re_tmp == 0.0) {
              c_Px_re_tmp = d_Px_re_tmp / dcv[0].im;
              MeanB_head = 0.0;
            } else if (d_Px_re_tmp == 0.0) {
              c_Px_re_tmp = 0.0;
              MeanB_head = b_Px_re_tmp / dcv[0].im;
            } else {
              c_Px_re_tmp = d_Px_re_tmp / dcv[0].im;
              MeanB_head = b_Px_re_tmp / dcv[0].im;
            }
          } else if (dcv[0].im == 0.0) {
            if (d_Px_re_tmp == 0.0) {
              c_Px_re_tmp = b_Px_re_tmp / d;
              MeanB_head = 0.0;
            } else if (b_Px_re_tmp == 0.0) {
              c_Px_re_tmp = 0.0;
              MeanB_head = -(d_Px_re_tmp / d);
            } else {
              c_Px_re_tmp = b_Px_re_tmp / d;
              MeanB_head = -(d_Px_re_tmp / d);
            }
          } else {
            Px_re_tmp = fabs(dcv[0].im);
            MeanB_head = fabs(d);
            if (Px_re_tmp > MeanB_head) {
              MeanB_head = d / dcv[0].im;
              Pm_re_tmp = dcv[0].im + MeanB_head * d;
              c_Px_re_tmp =
                  (d_Px_re_tmp + MeanB_head * b_Px_re_tmp) / Pm_re_tmp;
              MeanB_head = (b_Px_re_tmp - MeanB_head * d_Px_re_tmp) / Pm_re_tmp;
            } else if (MeanB_head == Px_re_tmp) {
              if (dcv[0].im > 0.0) {
                MeanB_head = 0.5;
              } else {
                MeanB_head = -0.5;
              }
              if (d > 0.0) {
                Pm_re_tmp = 0.5;
              } else {
                Pm_re_tmp = -0.5;
              }
              c_Px_re_tmp =
                  (d_Px_re_tmp * MeanB_head + b_Px_re_tmp * Pm_re_tmp) /
                  Px_re_tmp;
              MeanB_head =
                  (b_Px_re_tmp * MeanB_head - d_Px_re_tmp * Pm_re_tmp) /
                  Px_re_tmp;
            } else {
              MeanB_head = dcv[0].im / d;
              Pm_re_tmp = d + MeanB_head * dcv[0].im;
              c_Px_re_tmp =
                  (MeanB_head * d_Px_re_tmp + b_Px_re_tmp) / Pm_re_tmp;
              MeanB_head = (MeanB_head * b_Px_re_tmp - d_Px_re_tmp) / Pm_re_tmp;
            }
          }
          Est_H_p_re += c_Px_re_tmp;
          Est_H_p_im += MeanB_head;
          Px_re_tmp = rt_atan2d_snf(Est_H_p_im, Est_H_p_re);
          MeanA.re = Px_re_tmp * 0.0;
          MeanA.im = Px_re_tmp;
          b_exp(&MeanA);
          MeanB_head = rt_hypotd_snf(
              (re + (d * MeanA.re - dcv[0].im * MeanA.im)) - Pm_im,
              (im + (d * MeanA.im + dcv[0].im * MeanA.re)) - ai_tmp);
          d = MeanB_head * MeanB_head;
          MeanA.re = Px_re_tmp * 0.0;
          MeanA.im = Px_re_tmp;
          b_exp(&MeanA);
          MeanB_head = dcv[1].im * MeanA.im;
          Pm_re_tmp = dcv[1].re * MeanA.re;
          MeanB_head =
              rt_hypotd_snf((re + (Pm_re_tmp - MeanB_head)) - ar_tmp,
                            (im + (MeanB_head + Pm_re_tmp)) - b_ai_tmp);
          d += MeanB_head * MeanB_head;
          MeanA.re = Px_re_tmp * 0.0;
          MeanA.im = Px_re_tmp;
          b_exp(&MeanA);
          MeanB_head = dcv[2].im * MeanA.im;
          Pm_re_tmp = dcv[2].re * MeanA.re;
          MeanB_head =
              rt_hypotd_snf((re + (Pm_re_tmp - MeanB_head)) - b_ar_tmp,
                            (im + (MeanB_head + Pm_re_tmp)) - c_ai_tmp);
          d += MeanB_head * MeanB_head;
          MeanA.re = Px_re_tmp * 0.0;
          MeanA.im = Px_re_tmp;
          b_exp(&MeanA);
          MeanB_head = rt_hypotd_snf(
              (re + (dcv[3].re * MeanA.re - dcv[3].im * MeanA.im)) - c_ar_tmp,
              (im + (dcv[3].re * MeanA.im + dcv[3].im * MeanA.re)) - d_ai_tmp);
          d += MeanB_head * MeanB_head;
          b_Est_F[k] = d;
          Est_C[k].re = re;
          Est_C[k].im = im;
          b_Est_H[k] = Px_re_tmp;
        }
        if (!rtIsNaN(b_Est_F[0])) {
          j2 = 1;
        } else {
          bool exitg1;
          j2 = 0;
          k = 2;
          exitg1 = false;
          while ((!exitg1) && (k < 17)) {
            if (!rtIsNaN(b_Est_F[k - 1])) {
              j2 = k;
              exitg1 = true;
            } else {
              k++;
            }
          }
        }
        if (j2 == 0) {
          MeanB_head = b_Est_F[0];
          j2 = 1;
        } else {
          MeanB_head = b_Est_F[j2 - 1];
          i1 = j2 + 1;
          for (k = i1; k < 17; k++) {
            d = b_Est_F[k - 1];
            if (MeanB_head > d) {
              MeanB_head = d;
              j2 = k;
            }
          }
        }
        if (Sel_F > MeanB_head) {
          Sel_F = MeanB_head;
          Sel_C_re = Est_C[j2 - 1].re;
          Sel_C_im = Est_C[j2 - 1].im;
          heading_est = b_Est_H[j2 - 1];
        }
      }
    }
    MeanA.re = heading_est * 0.0;
    MeanA.im = heading_est;
    b_exp(&MeanA);
    for (j2 = 0; j2 < 4; j2++) {
      Px_re_tmp = dcv[j2].re;
      b_Px_re_tmp = dcv[j2].im;
      tag_pos_est[j2].re =
          Sel_C_re + (Px_re_tmp * MeanA.re - b_Px_re_tmp * MeanA.im);
      tag_pos_est[j2].im =
          Sel_C_im + (Px_re_tmp * MeanA.im + b_Px_re_tmp * MeanA.re);
      for (PPC = 0; PPC < 9; PPC++) {
        i = PPC + 18 * j2;
        Tag_Pos_List[i].re = s_time + (((double)PPC + 1.0) - 9.0) * 0.1;
        Tag_Pos_List[i].im = 0.0;
        Tag_Pos_List[i + 9] = tag_pos_est[j2];
      }
    }
  } else {
    creal_T x[10];
    double Est_H_p_im;
    double Est_H_p_re;
    double MeanB_head;
    double Pm_im;
    double Pm_re_tmp;
    double Px_re_tmp;
    double Sel_C_im;
    double Sel_C_re;
    double Sel_F;
    double ai_tmp;
    double b_Px_re_tmp;
    double c_Px_re_tmp;
    double d;
    double d_Px_re_tmp;
    double im;
    double re;
    signed char Va_ind[4];
    /* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
    /* %%%%%%%%%%%%%%%%%  Next Position Prediction    %%%%%%%%%%%%%%%%%% */
    for (PPC = 0; PPC < 4; PPC++) {
      creal_T Pm[27];
      creal_T b_Px[27];
      creal_T Px[9];
      creal_T b_Pm[9];
      creal_T c_Px[3];
      for (k = 0; k < 9; k++) {
        j2 = k + 18 * PPC;
        re = Tag_Pos_List[j2].re - s_time;
        im = Tag_Pos_List[j2].im;
        Pm[k].re = re * re - im * im;
        d = re * im;
        Pm[k].im = d + d;
        Pm[k + 9].re = re;
        Pm[k + 9].im = im;
        Pm[k + 18].re = 1.0;
        Pm[k + 18].im = 0.0;
      }
      for (i = 0; i < 3; i++) {
        for (i1 = 0; i1 < 3; i1++) {
          re = 0.0;
          im = 0.0;
          for (IndT_size = 0; IndT_size < 9; IndT_size++) {
            j2 = IndT_size + 9 * i;
            Est_H_p_im = Pm[j2].re;
            Pm_im = -Pm[j2].im;
            j2 = IndT_size + 9 * i1;
            MeanB_head = Pm[j2].im;
            Pm_re_tmp = Pm[j2].re;
            re += Est_H_p_im * Pm_re_tmp - Pm_im * MeanB_head;
            im += Est_H_p_im * MeanB_head + Pm_im * Pm_re_tmp;
          }
          IndT_size = i + 3 * i1;
          b_Pm[IndT_size].re = re;
          b_Pm[IndT_size].im = im;
        }
      }
      b_inv(b_Pm, Px);
      for (i = 0; i < 3; i++) {
        MeanB_head = Px[i].re;
        Pm_re_tmp = Px[i].im;
        Px_re_tmp = Px[i + 3].re;
        b_Px_re_tmp = Px[i + 3].im;
        c_Px_re_tmp = Px[i + 6].re;
        d_Px_re_tmp = Px[i + 6].im;
        for (i1 = 0; i1 < 9; i1++) {
          Est_H_p_im = Pm[i1].re;
          Pm_im = -Pm[i1].im;
          re = MeanB_head * Est_H_p_im - Pm_re_tmp * Pm_im;
          im = MeanB_head * Pm_im + Pm_re_tmp * Est_H_p_im;
          Est_H_p_im = Pm[i1 + 9].re;
          Pm_im = -Pm[i1 + 9].im;
          re += Px_re_tmp * Est_H_p_im - b_Px_re_tmp * Pm_im;
          im += Px_re_tmp * Pm_im + b_Px_re_tmp * Est_H_p_im;
          Est_H_p_im = Pm[i1 + 18].re;
          Pm_im = -Pm[i1 + 18].im;
          re += c_Px_re_tmp * Est_H_p_im - d_Px_re_tmp * Pm_im;
          im += c_Px_re_tmp * Pm_im + d_Px_re_tmp * Est_H_p_im;
          IndT_size = i + 3 * i1;
          b_Px[IndT_size].re = re;
          b_Px[IndT_size].im = im;
        }
      }
      for (i = 0; i < 9; i++) {
        Px[i].re = Tag_Pos_List[(i + 18 * PPC) + 9].re;
        Px[i].im = 0.0;
      }
      for (i = 0; i < 3; i++) {
        re = 0.0;
        im = 0.0;
        for (i1 = 0; i1 < 9; i1++) {
          j2 = i + 3 * i1;
          MeanB_head = b_Px[j2].re;
          Pm_re_tmp = Px[i1].im;
          Px_re_tmp = b_Px[j2].im;
          b_Px_re_tmp = Px[i1].re;
          re += MeanB_head * b_Px_re_tmp - Px_re_tmp * Pm_re_tmp;
          im += MeanB_head * Pm_re_tmp + Px_re_tmp * b_Px_re_tmp;
        }
        c_Px[i].re = re;
        c_Px[i].im = im;
      }
      InterpPosition[PPC] = c_Px[2];
      /*  Py = pm x A */
      /*  tr(pm)xPy = tr(Pm)pm x A */
      /*  inv(tr(Pm)Pm) x tr(pm)xPy = A */
      for (k = 0; k < 9; k++) {
        j2 = k + 18 * PPC;
        re = Tag_Pos_List[j2].re - s_time;
        im = Tag_Pos_List[j2].im;
        Pm[k].re = re * re - im * im;
        d = re * im;
        Pm[k].im = d + d;
        Pm[k + 9].re = re;
        Pm[k + 9].im = im;
        Pm[k + 18].re = 1.0;
        Pm[k + 18].im = 0.0;
      }
      for (i = 0; i < 3; i++) {
        for (i1 = 0; i1 < 3; i1++) {
          re = 0.0;
          im = 0.0;
          for (IndT_size = 0; IndT_size < 9; IndT_size++) {
            j2 = IndT_size + 9 * i;
            Est_H_p_im = Pm[j2].re;
            Pm_im = -Pm[j2].im;
            j2 = IndT_size + 9 * i1;
            MeanB_head = Pm[j2].im;
            Pm_re_tmp = Pm[j2].re;
            re += Est_H_p_im * Pm_re_tmp - Pm_im * MeanB_head;
            im += Est_H_p_im * MeanB_head + Pm_im * Pm_re_tmp;
          }
          IndT_size = i + 3 * i1;
          b_Pm[IndT_size].re = re;
          b_Pm[IndT_size].im = im;
        }
      }
      b_inv(b_Pm, Px);
      for (i = 0; i < 3; i++) {
        MeanB_head = Px[i].re;
        Pm_re_tmp = Px[i].im;
        Px_re_tmp = Px[i + 3].re;
        b_Px_re_tmp = Px[i + 3].im;
        c_Px_re_tmp = Px[i + 6].re;
        d_Px_re_tmp = Px[i + 6].im;
        for (i1 = 0; i1 < 9; i1++) {
          Est_H_p_im = Pm[i1].re;
          Pm_im = -Pm[i1].im;
          re = MeanB_head * Est_H_p_im - Pm_re_tmp * Pm_im;
          im = MeanB_head * Pm_im + Pm_re_tmp * Est_H_p_im;
          Est_H_p_im = Pm[i1 + 9].re;
          Pm_im = -Pm[i1 + 9].im;
          re += Px_re_tmp * Est_H_p_im - b_Px_re_tmp * Pm_im;
          im += Px_re_tmp * Pm_im + b_Px_re_tmp * Est_H_p_im;
          Est_H_p_im = Pm[i1 + 18].re;
          Pm_im = -Pm[i1 + 18].im;
          re += c_Px_re_tmp * Est_H_p_im - d_Px_re_tmp * Pm_im;
          im += c_Px_re_tmp * Pm_im + d_Px_re_tmp * Est_H_p_im;
          IndT_size = i + 3 * i1;
          b_Px[IndT_size].re = re;
          b_Px[IndT_size].im = im;
        }
      }
      for (i = 0; i < 9; i++) {
        Px[i].re = Tag_Pos_List[(i + 18 * PPC) + 9].im;
        Px[i].im = 0.0;
      }
      for (i = 0; i < 3; i++) {
        re = 0.0;
        im = 0.0;
        for (i1 = 0; i1 < 9; i1++) {
          j2 = i + 3 * i1;
          MeanB_head = b_Px[j2].re;
          Pm_re_tmp = Px[i1].im;
          Px_re_tmp = b_Px[j2].im;
          b_Px_re_tmp = Px[i1].re;
          re += MeanB_head * b_Px_re_tmp - Px_re_tmp * Pm_re_tmp;
          im += MeanB_head * Pm_re_tmp + Px_re_tmp * b_Px_re_tmp;
        }
        c_Px[i].re = re;
        c_Px[i].im = im;
      }
      InterpPosition[PPC + 4] = c_Px[2];
      /*  Py = pm x A */
      /*  tr(pm)xPy = tr(Pm)pm x A */
      /*  inv(tr(Pm)Pm) x tr(pm)xPy = A */
    }
    /* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
    /* %%%%%%%%%%%%%%%% New Position Calc.   %%%%%%%%%%%%%%%%%%%%%%%%%%% */
    /*          for PP = mod(r-1,Lp)+1:mod(r-1,Lp)+1 */
    memset(&Pos2C[0], 0, 8U * sizeof(creal_T));
    Pos2C[0].re = InterpPosition[0].re +
                  (0.0 * InterpPosition[4].re - InterpPosition[4].im);
    Pos2C[0].im = InterpPosition[0].im +
                  (0.0 * InterpPosition[4].im + InterpPosition[4].re);
    Va_ind[0] = 0;
    Pos2C[1].re = InterpPosition[1].re +
                  (0.0 * InterpPosition[5].re - InterpPosition[5].im);
    Pos2C[1].im = InterpPosition[1].im +
                  (0.0 * InterpPosition[5].im + InterpPosition[5].re);
    Va_ind[1] = 0;
    Pos2C[2].re = InterpPosition[2].re +
                  (0.0 * InterpPosition[6].re - InterpPosition[6].im);
    Pos2C[2].im = InterpPosition[2].im +
                  (0.0 * InterpPosition[6].im + InterpPosition[6].re);
    Va_ind[2] = 0;
    Pos2C[3].re = InterpPosition[3].re +
                  (0.0 * InterpPosition[7].re - InterpPosition[7].im);
    Pos2C[3].im = InterpPosition[3].im +
                  (0.0 * InterpPosition[7].im + InterpPosition[7].re);
    Va_ind[3] = 0;
    Sel_F = 1.0E+7;
    Sel_C_re = 0.0;
    Sel_C_im = 0.0;
    heading_est = 0.0;
    CandPos.re = 0.0;
    CandPos.im = 0.0;
    i = (int)Nanchor;
    for (L1 = 0; L1 < i; L1++) {
      i1 = (int)(Nanchor + (1.0 - (((double)L1 + 1.0) + 1.0)));
      if (i1 - 1 >= 0) {
        i2 = (int)(TagNum + (1.0 - TagNum));
        Va_ind[(int)TagNum - 1] = 1;
      }
      for (L2 = 0; L2 < i1; L2++) {
        creal_T b_Est_C[2];
        MeanB_head = (((double)L1 + 1.0) + 1.0) + (double)L2;
        if (i2 - 1 >= 0) {
          j2 = (int)RxID_data[L1] - 1;
          Est_F[0] = iv[j2];
          PPC = (int)RxID_data[(int)MeanB_head - 1] - 1;
          Est_F[1] = iv[PPC];
          Est_H[0] = iv1[j2];
          Est_H[1] = iv1[PPC];
          RxDist[0] = RxDist_data[L1];
          RxDist[1] = RxDist_data[(int)MeanB_head - 1];
          loop_ub = RxID_size[1];
        }
        if (i2 - 1 >= 0) {
          tmp_size[0] = 1;
          tmp_size[1] = RxID_size[1];
          for (IndT_size = 0; IndT_size < loop_ub; IndT_size++) {
            j2 = (int)RxID_data[IndT_size] - 1;
            tmp_data[IndT_size].re = iv[j2];
            tmp_data[IndT_size].im = iv1[j2];
          }
        }
        for (PPC = 0; PPC < i2; PPC++) {
          double Pos2[4];
          double Prob2[2];
          MeanB_head = TagNum + (double)PPC;
          TwoAnchPos3(Est_F, Est_H, RxDist, tmp_data, tmp_size, RxDist_data,
                      RxDist_size, Pos2, Prob2);
          Pos2C[(int)MeanB_head - 1].re = Pos2[0];
          Pos2C[(int)MeanB_head - 1].im = Pos2[2];
          Pos2C[(int)MeanB_head + 3].re = Pos2[1];
          Pos2C[(int)MeanB_head + 3].im = Pos2[3];
        }
        for (k = 0; k < 2; k++) {
          double ia_data[64];
          double ar_tmp;
          double b_ai_tmp;
          double b_ar_tmp;
          double c_ai_tmp;
          double c_ar_tmp;
          double d_ai_tmp;
          int s_size[2];
          char xtmp;
          dec2bin(rt_powd_snf(2.0, TagNum * (double)k) - 1.0, s_data, s_size);
          j2 = s_size[1];
          if (j2 - 1 >= 0) {
            memcpy(&Va_data[0], &s_data[0], (unsigned int)j2 * sizeof(char));
          }
          IndT_size = s_size[1] >> 1;
          for (PPC = 0; PPC < IndT_size; PPC++) {
            j2 = (s_size[1] - PPC) - 1;
            xtmp = Va_data[PPC];
            Va_data[PPC] = Va_data[j2];
            Va_data[j2] = xtmp;
          }
          j2 = s_size[1];
          for (PPC = 0; PPC < j2; PPC++) {
            ia_data[PPC] = 0.0;
            xtmp = Va_data[PPC];
            if ((xtmp == '0') && (Va_ind[PPC] != 0)) {
              ia_data[PPC] = 1.0;
            } else if ((xtmp == '1') && (Va_ind[PPC] != 0)) {
              ia_data[PPC] = 2.0;
            } else {
              ia_data[PPC] = 1.0;
            }
          }
          /*  NB = length(tag_pos_b); */
          PPC = ((int)ia_data[0] - 1) << 2;
          Pm_im = Pos2C[PPC].re;
          ai_tmp = Pos2C[PPC].im;
          if (ai_tmp == 0.0) {
            IndT_size = s_size[1];
            c_Px_re_tmp = Pm_im / (double)s_size[1];
            MeanB_head = 0.0;
          } else if (Pm_im == 0.0) {
            c_Px_re_tmp = 0.0;
            IndT_size = s_size[1];
            MeanB_head = ai_tmp / (double)s_size[1];
          } else {
            IndT_size = s_size[1];
            c_Px_re_tmp = Pm_im / (double)s_size[1];
            MeanB_head = ai_tmp / (double)s_size[1];
          }
          re = c_Px_re_tmp;
          im = MeanB_head;
          PPC = (((int)ia_data[1] - 1) << 2) + 1;
          ar_tmp = Pos2C[PPC].re;
          b_ai_tmp = Pos2C[PPC].im;
          if (b_ai_tmp == 0.0) {
            c_Px_re_tmp = ar_tmp / (double)IndT_size;
            MeanB_head = 0.0;
          } else if (ar_tmp == 0.0) {
            c_Px_re_tmp = 0.0;
            MeanB_head = b_ai_tmp / (double)IndT_size;
          } else {
            c_Px_re_tmp = ar_tmp / (double)IndT_size;
            MeanB_head = b_ai_tmp / (double)IndT_size;
          }
          re += c_Px_re_tmp;
          im += MeanB_head;
          PPC = (((int)ia_data[2] - 1) << 2) + 2;
          b_ar_tmp = Pos2C[PPC].re;
          c_ai_tmp = Pos2C[PPC].im;
          if (c_ai_tmp == 0.0) {
            c_Px_re_tmp = b_ar_tmp / (double)IndT_size;
            MeanB_head = 0.0;
          } else if (b_ar_tmp == 0.0) {
            c_Px_re_tmp = 0.0;
            MeanB_head = c_ai_tmp / (double)IndT_size;
          } else {
            c_Px_re_tmp = b_ar_tmp / (double)IndT_size;
            MeanB_head = c_ai_tmp / (double)IndT_size;
          }
          re += c_Px_re_tmp;
          im += MeanB_head;
          PPC = (((int)ia_data[3] - 1) << 2) + 3;
          c_ar_tmp = Pos2C[PPC].re;
          d_ai_tmp = Pos2C[PPC].im;
          if (d_ai_tmp == 0.0) {
            c_Px_re_tmp = c_ar_tmp / (double)IndT_size;
            MeanB_head = 0.0;
          } else if (c_ar_tmp == 0.0) {
            c_Px_re_tmp = 0.0;
            MeanB_head = d_ai_tmp / (double)IndT_size;
          } else {
            c_Px_re_tmp = c_ar_tmp / (double)IndT_size;
            MeanB_head = d_ai_tmp / (double)IndT_size;
          }
          re += c_Px_re_tmp;
          im += MeanB_head;
          d_Px_re_tmp = Pm_im - re;
          b_Px_re_tmp = ai_tmp - im;
          if (dcv[0].im == 0.0) {
            if (b_Px_re_tmp == 0.0) {
              d = dcv[0].re;
              c_Px_re_tmp = d_Px_re_tmp / dcv[0].re;
              MeanB_head = 0.0;
            } else if (d_Px_re_tmp == 0.0) {
              c_Px_re_tmp = 0.0;
              d = dcv[0].re;
              MeanB_head = b_Px_re_tmp / dcv[0].re;
            } else {
              d = dcv[0].re;
              c_Px_re_tmp = d_Px_re_tmp / dcv[0].re;
              MeanB_head = b_Px_re_tmp / dcv[0].re;
            }
          } else {
            d = dcv[0].re;
            if (dcv[0].re == 0.0) {
              if (d_Px_re_tmp == 0.0) {
                c_Px_re_tmp = b_Px_re_tmp / dcv[0].im;
                MeanB_head = 0.0;
              } else if (b_Px_re_tmp == 0.0) {
                c_Px_re_tmp = 0.0;
                MeanB_head = -(d_Px_re_tmp / dcv[0].im);
              } else {
                c_Px_re_tmp = b_Px_re_tmp / dcv[0].im;
                MeanB_head = -(d_Px_re_tmp / dcv[0].im);
              }
            } else {
              Px_re_tmp = fabs(dcv[0].re);
              MeanB_head = fabs(dcv[0].im);
              if (Px_re_tmp > MeanB_head) {
                MeanB_head = dcv[0].im / dcv[0].re;
                Pm_re_tmp = dcv[0].re + MeanB_head * dcv[0].im;
                c_Px_re_tmp =
                    (d_Px_re_tmp + MeanB_head * b_Px_re_tmp) / Pm_re_tmp;
                MeanB_head =
                    (b_Px_re_tmp - MeanB_head * d_Px_re_tmp) / Pm_re_tmp;
              } else if (MeanB_head == Px_re_tmp) {
                if (dcv[0].re > 0.0) {
                  MeanB_head = 0.5;
                } else {
                  MeanB_head = -0.5;
                }
                if (dcv[0].im > 0.0) {
                  Pm_re_tmp = 0.5;
                } else {
                  Pm_re_tmp = -0.5;
                }
                c_Px_re_tmp =
                    (d_Px_re_tmp * MeanB_head + b_Px_re_tmp * Pm_re_tmp) /
                    Px_re_tmp;
                MeanB_head =
                    (b_Px_re_tmp * MeanB_head - d_Px_re_tmp * Pm_re_tmp) /
                    Px_re_tmp;
              } else {
                MeanB_head = dcv[0].re / dcv[0].im;
                Pm_re_tmp = dcv[0].im + MeanB_head * dcv[0].re;
                c_Px_re_tmp =
                    (MeanB_head * d_Px_re_tmp + b_Px_re_tmp) / Pm_re_tmp;
                MeanB_head =
                    (MeanB_head * b_Px_re_tmp - d_Px_re_tmp) / Pm_re_tmp;
              }
            }
          }
          Est_H_p_re = c_Px_re_tmp;
          Est_H_p_im = MeanB_head;
          d_Px_re_tmp = ar_tmp - re;
          b_Px_re_tmp = b_ai_tmp - im;
          if (dcv[0].im == 0.0) {
            if (b_Px_re_tmp == 0.0) {
              c_Px_re_tmp = d_Px_re_tmp / dcv[0].im;
              MeanB_head = 0.0;
            } else if (d_Px_re_tmp == 0.0) {
              c_Px_re_tmp = 0.0;
              MeanB_head = b_Px_re_tmp / dcv[0].im;
            } else {
              c_Px_re_tmp = d_Px_re_tmp / dcv[0].im;
              MeanB_head = b_Px_re_tmp / dcv[0].im;
            }
          } else if (dcv[0].im == 0.0) {
            if (d_Px_re_tmp == 0.0) {
              c_Px_re_tmp = b_Px_re_tmp / dcv[0].im;
              MeanB_head = 0.0;
            } else if (b_Px_re_tmp == 0.0) {
              c_Px_re_tmp = 0.0;
              MeanB_head = -(d_Px_re_tmp / dcv[0].im);
            } else {
              c_Px_re_tmp = b_Px_re_tmp / dcv[0].im;
              MeanB_head = -(d_Px_re_tmp / dcv[0].im);
            }
          } else {
            Px_re_tmp = fabs(dcv[0].im);
            if (dcv[0].im > 0.0) {
              MeanB_head = 0.5;
            } else {
              MeanB_head = -0.5;
            }
            Pm_re_tmp = b_Px_re_tmp * MeanB_head;
            MeanB_head *= d_Px_re_tmp;
            c_Px_re_tmp = (MeanB_head + Pm_re_tmp) / Px_re_tmp;
            MeanB_head = (Pm_re_tmp - MeanB_head) / Px_re_tmp;
          }
          Est_H_p_re += c_Px_re_tmp;
          Est_H_p_im += MeanB_head;
          d_Px_re_tmp = b_ar_tmp - re;
          b_Px_re_tmp = c_ai_tmp - im;
          if (d == 0.0) {
            if (b_Px_re_tmp == 0.0) {
              c_Px_re_tmp = d_Px_re_tmp / d;
              MeanB_head = 0.0;
            } else if (d_Px_re_tmp == 0.0) {
              c_Px_re_tmp = 0.0;
              MeanB_head = b_Px_re_tmp / d;
            } else {
              c_Px_re_tmp = d_Px_re_tmp / d;
              MeanB_head = b_Px_re_tmp / d;
            }
          } else if (d == 0.0) {
            if (d_Px_re_tmp == 0.0) {
              c_Px_re_tmp = b_Px_re_tmp / d;
              MeanB_head = 0.0;
            } else if (b_Px_re_tmp == 0.0) {
              c_Px_re_tmp = 0.0;
              MeanB_head = -(d_Px_re_tmp / d);
            } else {
              c_Px_re_tmp = b_Px_re_tmp / d;
              MeanB_head = -(d_Px_re_tmp / d);
            }
          } else {
            Px_re_tmp = fabs(d);
            if (d > 0.0) {
              MeanB_head = 0.5;
            } else {
              MeanB_head = -0.5;
            }
            Pm_re_tmp = b_Px_re_tmp * MeanB_head;
            MeanB_head *= d_Px_re_tmp;
            c_Px_re_tmp = (MeanB_head + Pm_re_tmp) / Px_re_tmp;
            MeanB_head = (Pm_re_tmp - MeanB_head) / Px_re_tmp;
          }
          Est_H_p_re += c_Px_re_tmp;
          Est_H_p_im += MeanB_head;
          d_Px_re_tmp = c_ar_tmp - re;
          b_Px_re_tmp = d_ai_tmp - im;
          if (d == 0.0) {
            if (b_Px_re_tmp == 0.0) {
              c_Px_re_tmp = d_Px_re_tmp / dcv[0].im;
              MeanB_head = 0.0;
            } else if (d_Px_re_tmp == 0.0) {
              c_Px_re_tmp = 0.0;
              MeanB_head = b_Px_re_tmp / dcv[0].im;
            } else {
              c_Px_re_tmp = d_Px_re_tmp / dcv[0].im;
              MeanB_head = b_Px_re_tmp / dcv[0].im;
            }
          } else if (dcv[0].im == 0.0) {
            if (d_Px_re_tmp == 0.0) {
              c_Px_re_tmp = b_Px_re_tmp / d;
              MeanB_head = 0.0;
            } else if (b_Px_re_tmp == 0.0) {
              c_Px_re_tmp = 0.0;
              MeanB_head = -(d_Px_re_tmp / d);
            } else {
              c_Px_re_tmp = b_Px_re_tmp / d;
              MeanB_head = -(d_Px_re_tmp / d);
            }
          } else {
            Px_re_tmp = fabs(dcv[0].im);
            MeanB_head = fabs(d);
            if (Px_re_tmp > MeanB_head) {
              MeanB_head = d / dcv[0].im;
              Pm_re_tmp = dcv[0].im + MeanB_head * d;
              c_Px_re_tmp =
                  (d_Px_re_tmp + MeanB_head * b_Px_re_tmp) / Pm_re_tmp;
              MeanB_head = (b_Px_re_tmp - MeanB_head * d_Px_re_tmp) / Pm_re_tmp;
            } else if (MeanB_head == Px_re_tmp) {
              if (dcv[0].im > 0.0) {
                MeanB_head = 0.5;
              } else {
                MeanB_head = -0.5;
              }
              if (d > 0.0) {
                Pm_re_tmp = 0.5;
              } else {
                Pm_re_tmp = -0.5;
              }
              c_Px_re_tmp =
                  (d_Px_re_tmp * MeanB_head + b_Px_re_tmp * Pm_re_tmp) /
                  Px_re_tmp;
              MeanB_head =
                  (b_Px_re_tmp * MeanB_head - d_Px_re_tmp * Pm_re_tmp) /
                  Px_re_tmp;
            } else {
              MeanB_head = dcv[0].im / d;
              Pm_re_tmp = d + MeanB_head * dcv[0].im;
              c_Px_re_tmp =
                  (MeanB_head * d_Px_re_tmp + b_Px_re_tmp) / Pm_re_tmp;
              MeanB_head = (MeanB_head * b_Px_re_tmp - d_Px_re_tmp) / Pm_re_tmp;
            }
          }
          Est_H_p_re += c_Px_re_tmp;
          Est_H_p_im += MeanB_head;
          Px_re_tmp = rt_atan2d_snf(Est_H_p_im, Est_H_p_re);
          MeanA.re = Px_re_tmp * 0.0;
          MeanA.im = Px_re_tmp;
          b_exp(&MeanA);
          MeanB_head = rt_hypotd_snf(
              (re + (d * MeanA.re - dcv[0].im * MeanA.im)) - Pm_im,
              (im + (d * MeanA.im + dcv[0].im * MeanA.re)) - ai_tmp);
          d = MeanB_head * MeanB_head;
          MeanA.re = Px_re_tmp * 0.0;
          MeanA.im = Px_re_tmp;
          b_exp(&MeanA);
          MeanB_head = dcv[1].im * MeanA.im;
          Pm_re_tmp = dcv[1].re * MeanA.re;
          MeanB_head =
              rt_hypotd_snf((re + (Pm_re_tmp - MeanB_head)) - ar_tmp,
                            (im + (MeanB_head + Pm_re_tmp)) - b_ai_tmp);
          d += MeanB_head * MeanB_head;
          MeanA.re = Px_re_tmp * 0.0;
          MeanA.im = Px_re_tmp;
          b_exp(&MeanA);
          MeanB_head = dcv[2].im * MeanA.im;
          Pm_re_tmp = dcv[2].re * MeanA.re;
          MeanB_head =
              rt_hypotd_snf((re + (Pm_re_tmp - MeanB_head)) - b_ar_tmp,
                            (im + (MeanB_head + Pm_re_tmp)) - c_ai_tmp);
          d += MeanB_head * MeanB_head;
          MeanA.re = Px_re_tmp * 0.0;
          MeanA.im = Px_re_tmp;
          b_exp(&MeanA);
          MeanB_head = rt_hypotd_snf(
              (re + (dcv[3].re * MeanA.re - dcv[3].im * MeanA.im)) - c_ar_tmp,
              (im + (dcv[3].re * MeanA.im + dcv[3].im * MeanA.re)) - d_ai_tmp);
          d += MeanB_head * MeanB_head;
          Est_F[k] = d;
          b_Est_C[k].re = re;
          b_Est_C[k].im = im;
          Est_H[k] = Px_re_tmp;
        }
        if ((Est_F[0] > Est_F[1]) ||
            (rtIsNaN(Est_F[0]) && (!rtIsNaN(Est_F[1])))) {
          MeanB_head = Est_F[1];
          j2 = 1;
        } else {
          MeanB_head = Est_F[0];
          j2 = 0;
        }
        if (Sel_F > MeanB_head) {
          Sel_F = MeanB_head;
          Sel_C_re = b_Est_C[j2].re;
          Sel_C_im = b_Est_C[j2].im;
          heading_est = Est_H[j2];
          CandPos = Pos2C[((int)TagNum + (j2 << 2)) - 1];
        }
      }
    }
    MeanA.re = heading_est * 0.0;
    MeanA.im = heading_est;
    b_exp(&MeanA);
    Px_re_tmp = dcv[0].im * MeanA.im;
    b_Px_re_tmp = dcv[0].re * MeanA.re;
    MeanB_head = dcv[0].im * MeanA.re;
    Pm_re_tmp = dcv[0].re * MeanA.im;
    tag_pos_est[0].re = Sel_C_re + (b_Px_re_tmp - Px_re_tmp);
    tag_pos_est[0].im = Sel_C_im + (Pm_re_tmp + MeanB_head);
    tag_pos_est[1].re = Sel_C_re + (MeanB_head - Px_re_tmp);
    tag_pos_est[1].im = Sel_C_im + (Px_re_tmp + MeanB_head);
    tag_pos_est[2].re = Sel_C_re + (b_Px_re_tmp - Pm_re_tmp);
    tag_pos_est[2].im = Sel_C_im + (Pm_re_tmp + b_Px_re_tmp);
    tag_pos_est[3].re = Sel_C_re + (MeanB_head - Pm_re_tmp);
    tag_pos_est[3].im = Sel_C_im + (Px_re_tmp + b_Px_re_tmp);
    GetPosRefine2(RxDist_data, RxID_data, RxID_size, Nanchor, TagNum,
                  tag_pos_est, &heading_est, &CandPos);
    for (i = 0; i < 2; i++) {
      for (i1 = 0; i1 < 8; i1++) {
        j2 = (i1 + 9 * i) + 18 * ((int)TagNum - 1);
        Tag_Pos_List[j2] = Tag_Pos_List[j2 + 1];
      }
    }
    i = 18 * ((int)TagNum - 1);
    Tag_Pos_List[i + 8].re = s_time;
    Tag_Pos_List[i + 8].im = 0.0;
    Tag_Pos_List[i + 17] = CandPos;
    /*              Tag_Pos_List(1:2,:,PP) = Tag_Pos_List(2:3,:,PP); */
    /*              Tag_Pos_List(3,:,PP) = [s_time(r) tag_pos_est(PP)]; */
    /*          end */
    Est_H_p_re = ((tag_pos_est[0].re + tag_pos_est[1].re) + tag_pos_est[2].re) +
                 tag_pos_est[3].re;
    Est_H_p_im = ((tag_pos_est[0].im + tag_pos_est[1].im) + tag_pos_est[2].im) +
                 tag_pos_est[3].im;
    if (Est_H_p_im == 0.0) {
      re = Est_H_p_re / 4.0;
      im = 0.0;
    } else if (Est_H_p_re == 0.0) {
      re = 0.0;
      im = Est_H_p_im / 4.0;
    } else {
      re = Est_H_p_re / 4.0;
      im = Est_H_p_im / 4.0;
    }
    for (i = 0; i < 19; i++) {
      heading_est_a[i] = heading_est_a[i + 1];
      centerest_a[i] = centerest_a[i + 1];
      centerest_a_aver[i] = centerest_a_aver[i + 1];
      centerest_a[i + 20] = centerest_a[i + 21];
      centerest_a_aver[i + 20] = centerest_a_aver[i + 21];
      headingest_a_aver[i] = headingest_a_aver[i + 1];
    }
    if (heading_est - heading_est_a[18] > 3.1415926535897931) {
      heading_est_a[19] = heading_est - 6.2831853071795862;
    } else if (heading_est_a[18] - heading_est > 3.1415926535897931) {
      heading_est_a[19] = heading_est + 6.2831853071795862;
    } else {
      heading_est_a[19] = heading_est;
    }
    centerest_a[19] = re;
    centerest_a[39] = im;
    for (i = 0; i < 10; i++) {
      d = centerest_a[i + 20];
      x[i].re = centerest_a[i] + 0.0 * d;
      x[i].im = d;
    }
    Est_H_p_re = x[0].re;
    Est_H_p_im = x[0].im;
    for (k = 0; k < 9; k++) {
      Est_H_p_re += x[k + 1].re;
      Est_H_p_im += x[k + 1].im;
    }
    if (Est_H_p_im == 0.0) {
      MeanA.re = Est_H_p_re / 10.0;
      MeanA.im = 0.0;
    } else if (Est_H_p_re == 0.0) {
      MeanA.re = 0.0;
      MeanA.im = Est_H_p_im / 10.0;
    } else {
      MeanA.re = Est_H_p_re / 10.0;
      MeanA.im = Est_H_p_im / 10.0;
    }
    for (i = 0; i < 10; i++) {
      d = centerest_a[i + 30];
      x[i].re = centerest_a[i + 10] + 0.0 * d;
      x[i].im = d;
    }
    Est_H_p_re = x[0].re;
    Est_H_p_im = x[0].im;
    MeanB_head = heading_est_a[10];
    Pm_re_tmp = heading_est_a[0];
    for (k = 0; k < 9; k++) {
      Est_H_p_re += x[k + 1].re;
      Est_H_p_im += x[k + 1].im;
      MeanB_head += heading_est_a[k + 11];
      Pm_re_tmp += heading_est_a[k + 1];
    }
    if (Est_H_p_im == 0.0) {
      re = Est_H_p_re / 10.0;
      im = 0.0;
    } else if (Est_H_p_re == 0.0) {
      re = 0.0;
      im = Est_H_p_im / 10.0;
    } else {
      re = Est_H_p_re / 10.0;
      im = Est_H_p_im / 10.0;
    }
    Pm_im = re - MeanA.re;
    ai_tmp = im - MeanA.im;
    if (ai_tmp == 0.0) {
      Est_H_p_re = Pm_im / 2.0;
    } else if (Pm_im == 0.0) {
      Est_H_p_re = 0.0;
    } else {
      Est_H_p_re = Pm_im / 2.0;
    }
    centerest_a_aver[19] = re + Est_H_p_re;
    if (ai_tmp == 0.0) {
      Est_H_p_im = 0.0;
    } else {
      Est_H_p_im = ai_tmp / 2.0;
    }
    centerest_a_aver[39] = im + Est_H_p_im;
    MeanB_head /= 10.0;
    headingest_a_aver[19] =
        b_mod(MeanB_head + (MeanB_head - Pm_re_tmp / 10.0) / 2.0);
    /*      tag_pos_est_aver =
     * get_tag_pos(centerest_a_aver(end,1)+j*centerest_a_aver(end,2),
     * headingest_a_aver(end), tag_pos_b); */
    MeanA.re = headingest_a_aver[19] * 0.0;
    MeanA.im = headingest_a_aver[19];
    b_exp(&MeanA);
    Est_H_p_re = centerest_a_aver[39] * 0.0;
    Px_re_tmp = dcv[0].im * MeanA.im;
    b_Px_re_tmp = dcv[0].re * MeanA.re;
    MeanB_head = dcv[0].im * MeanA.re;
    Pm_re_tmp = dcv[0].re * MeanA.im;
    tag_pos_est_aver[0].re =
        ((b_Px_re_tmp - Px_re_tmp) + centerest_a_aver[19]) + Est_H_p_re;
    tag_pos_est_aver[0].im = (Pm_re_tmp + MeanB_head) + centerest_a_aver[39];
    tag_pos_est_aver[1].re =
        ((MeanB_head - Px_re_tmp) + centerest_a_aver[19]) + Est_H_p_re;
    tag_pos_est_aver[1].im = (Px_re_tmp + MeanB_head) + centerest_a_aver[39];
    tag_pos_est_aver[2].re =
        ((b_Px_re_tmp - Pm_re_tmp) + centerest_a_aver[19]) + Est_H_p_re;
    tag_pos_est_aver[2].im = (Pm_re_tmp + b_Px_re_tmp) + centerest_a_aver[39];
    tag_pos_est_aver[3].re =
        ((MeanB_head - Pm_re_tmp) + centerest_a_aver[19]) + Est_H_p_re;
    tag_pos_est_aver[3].im = (Px_re_tmp + b_Px_re_tmp) + centerest_a_aver[39];
    *headingest_a_aver_v = headingest_a_aver[19];
    /*      K_heading_est = mod(headingest_a_aver(r),2*pi); */
    /*      K_centerest_a_aver(r, :) = centerest_a_aver(r,:); */
    /*      K_headingest_a_aver(r) = mod(headingest_a_aver(r),2*pi); */
  }
  return heading_est;
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void UWBpos_init(void)
{
  r = 0.0;
  memset(&InterpPosition[0], 0, 8U * sizeof(creal_T));
  memset(&Tag_Pos_List[0], 0, 72U * sizeof(creal_T));
  memset(&TagDistInitCount[0], 0, 24U * sizeof(double));
  memset(&TagDistInit[0], 0, 24U * sizeof(double));
  memset(&heading_est_a[0], 0, 20U * sizeof(double));
  memset(&centerest_a[0], 0, 40U * sizeof(double));
  memset(&centerest_a_aver[0], 0, 40U * sizeof(double));
  memset(&headingest_a_aver[0], 0, 20U * sizeof(double));
}

/*
 * File trailer for UWBpos.c
 *
 * [EOF]
 */
