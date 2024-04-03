/*
 * UWBMultiTagPos_V3_1.c
 *
 * Code generation for function 'UWBMultiTagPos_V3_1'
 *
 */

/* Include files */
#include "UWBMultiTagPos_V3_1.h"
#include "PositioningSystem_V2_3_data.h"
#include "PositioningSystem_V2_3_emxutil.h"
#include "PositioningSystem_V2_3_rtwutil.h"
#include "PositioningSystem_V2_3_types.h"
#include "UWBPosition_V3_1.h"
#include "div.h"
#include "find.h"
#include "mean.h"
#include "rt_nonfinite.h"
#include "sort.h"
#include "rt_defines.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Declarations */
static void binary_expand_op_1(double in1[32], const emxArray_real_T *in2);

static void binary_expand_op_4(creal_T in1[8], const double in3_data[],
                               const int in3_size[2], const double in5_data[],
                               const int in5_size[2]);

static double rt_atan2d_snf(double u0, double u1);

/* Function Definitions */
static void binary_expand_op_1(double in1[32], const emxArray_real_T *in2)
{
  const double *in2_data;
  int aux_0_1;
  int i;
  int stride_0_1;
  in2_data = in2->data;
  stride_0_1 = (in2->size[1] != 1);
  aux_0_1 = 0;
  for (i = 0; i < 8; i++) {
    int i1;
    i1 = i << 2;
    in1[i1] = in2_data[4 * aux_0_1 + 4 * in2->size[1] * 99] - in1[i1];
    in1[i1 + 1] =
        in2_data[(4 * aux_0_1 + 4 * in2->size[1] * 99) + 1] - in1[i1 + 1];
    in1[i1 + 2] =
        in2_data[(4 * aux_0_1 + 4 * in2->size[1] * 99) + 2] - in1[i1 + 2];
    in1[i1 + 3] =
        in2_data[(4 * aux_0_1 + 4 * in2->size[1] * 99) + 3] - in1[i1 + 3];
    aux_0_1 += stride_0_1;
  }
}

static void binary_expand_op_4(creal_T in1[8], const double in3_data[],
                               const int in3_size[2], const double in5_data[],
                               const int in5_size[2])
{
  int i;
  int loop_ub;
  int stride_0_1;
  int stride_1_1;
  stride_0_1 = (in3_size[1] != 1);
  stride_1_1 = (in5_size[1] != 1);
  if (in5_size[1] == 1) {
    loop_ub = in3_size[1];
  } else {
    loop_ub = in5_size[1];
  }
  for (i = 0; i < loop_ub; i++) {
    double d;
    d = in5_data[i * stride_1_1];
    in1[i].re = in3_data[i * stride_0_1] + 0.0 * d;
    in1[i].im = d;
  }
}

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

double UWBMultiTagPos_V3_1(const cell_wrap_0 b_PosUWB2[4],
                           const double xt_b_data[], const int xt_b_size[2],
                           const double yt_b_data[], const int yt_b_size[2],
                           const double xain_data[], const int xain_size[2],
                           const double yain_data[], const int yain_size[2],
                           const emxArray_real_T *b_DistMap, creal_T *PosHH)
{
  static creal_T TagCandA[80000];
  static creal_T CenterCands[20000];
  static creal_T HeadingCands[20000];
  static double y[20000];
  static int iidx[20000];
  emxArray_creal_T *sortedCenter;
  emxArray_creal_T *sortedHeading;
  emxArray_int32_T *r;
  creal_T Xt_b_data[50];
  creal_T Xain[8];
  creal_T Temp[4];
  creal_T z[4];
  creal_T CenterCandA;
  creal_T HeadingCandA;
  creal_T *sortedCenter_data;
  const double *DistMap_data;
  double HeadingCandA_tmp;
  double HeadingHH;
  double PrevAbsHeadingCandA;
  double PrevHeadingCandA_im;
  double PrevHeadingCandA_re;
  int Xt_b_size[2];
  int i;
  int i4;
  int idx;
  int kk;
  int ll;
  int mm;
  int nn;
  int ss;
  signed char LessThan1m1[20000];
  boolean_T b_Temp_tmp;
  DistMap_data = b_DistMap->data;
  if (xt_b_size[1] == yt_b_size[1]) {
    Xt_b_size[0] = 1;
    Xt_b_size[1] = xt_b_size[1];
    idx = xt_b_size[1];
    for (i = 0; i < idx; i++) {
      HeadingCandA_tmp = yt_b_data[i];
      Xt_b_data[i].re = xt_b_data[i] + 0.0 * HeadingCandA_tmp;
      Xt_b_data[i].im = HeadingCandA_tmp;
    }
  } else {
    binary_expand_op(Xt_b_data, Xt_b_size, xt_b_data, xt_b_size, yt_b_data,
                     yt_b_size);
  }
  memset(&Xain[0], 0, 8U * sizeof(creal_T));
  if (xain_size[1] == yain_size[1]) {
    idx = xain_size[1];
    for (i = 0; i < idx; i++) {
      HeadingCandA_tmp = yain_data[i];
      Xain[i].re = xain_data[i] + 0.0 * HeadingCandA_tmp;
      Xain[i].im = HeadingCandA_tmp;
    }
  } else {
    binary_expand_op_4(Xain, xain_data, xain_size, yain_data, yain_size);
  }
  memset(&HeadingCands[0], 0, 20000U * sizeof(creal_T));
  memset(&CenterCands[0], 0, 20000U * sizeof(creal_T));
  memset(&TagCandA[0], 0, 80000U * sizeof(creal_T));
  memset(&LessThan1m1[0], 0, 20000U * sizeof(signed char));
  ss = 0;
  PrevAbsHeadingCandA = 100.0;
  PosHH->re = 0.0;
  PosHH->im = 0.0;
  PrevHeadingCandA_re = 0.0;
  PrevHeadingCandA_im = 0.0;
  i = b_PosUWB2[0].f1.size[1];
  for (kk = 0; kk < i; kk++) {
    int i1;
    Temp[0] = b_PosUWB2[0].f1.data[kk];
    i1 = b_PosUWB2[1].f1.size[1];
    for (ll = 0; ll < i1; ll++) {
      int i2;
      Temp[1] = b_PosUWB2[1].f1.data[ll];
      i2 = b_PosUWB2[2].f1.size[1];
      for (mm = 0; mm < i2; mm++) {
        int i3;
        Temp[2] = b_PosUWB2[2].f1.data[mm];
        i3 = b_PosUWB2[3].f1.size[1];
        for (nn = 0; nn < i3; nn++) {
          double PosUWB2_tmp;
          double b_HeadingCandA_tmp;
          Temp[3] = b_PosUWB2[3].f1.data[nn];
          idx = 0;
          if ((Temp[0].re == 0.0) && (Temp[0].im == 0.0)) {
            idx = 1;
          }
          if ((Temp[1].re == 0.0) && (Temp[1].im == 0.0)) {
            idx++;
          }
          if ((Temp[2].re == 0.0) && (Temp[2].im == 0.0)) {
            idx++;
          }
          if ((Temp[3].re == 0.0) && (Temp[3].im == 0.0)) {
            idx++;
          }
          if (idx == 0) {
            double brm;
            b_HeadingCandA_tmp =
                ((Temp[0].re + Temp[1].re) + Temp[2].re) + Temp[3].re;
            HeadingCandA_tmp =
                ((Temp[0].im + Temp[1].im) + Temp[2].im) + Temp[3].im;
            if (HeadingCandA_tmp == 0.0) {
              CenterCandA.re = b_HeadingCandA_tmp / 4.0;
              CenterCandA.im = 0.0;
            } else if (b_HeadingCandA_tmp == 0.0) {
              CenterCandA.re = 0.0;
              CenterCandA.im = HeadingCandA_tmp / 4.0;
            } else {
              CenterCandA.re = b_HeadingCandA_tmp / 4.0;
              CenterCandA.im = HeadingCandA_tmp / 4.0;
            }
            if (Xt_b_size[1] == 4) {
              double ai;
              double ar;
              ar = Temp[0].re - CenterCandA.re;
              ai = Temp[0].im - CenterCandA.im;
              if (Xt_b_data[0].im == 0.0) {
                if (ai == 0.0) {
                  z[0].re = ar / Xt_b_data[0].re;
                  z[0].im = 0.0;
                } else if (ar == 0.0) {
                  z[0].re = 0.0;
                  z[0].im = ai / Xt_b_data[0].re;
                } else {
                  z[0].re = ar / Xt_b_data[0].re;
                  z[0].im = ai / Xt_b_data[0].re;
                }
              } else if (Xt_b_data[0].re == 0.0) {
                if (ar == 0.0) {
                  z[0].re = ai / Xt_b_data[0].im;
                  z[0].im = 0.0;
                } else if (ai == 0.0) {
                  z[0].re = 0.0;
                  z[0].im = -(ar / Xt_b_data[0].im);
                } else {
                  z[0].re = ai / Xt_b_data[0].im;
                  z[0].im = -(ar / Xt_b_data[0].im);
                }
              } else {
                brm = fabs(Xt_b_data[0].re);
                b_HeadingCandA_tmp = fabs(Xt_b_data[0].im);
                if (brm > b_HeadingCandA_tmp) {
                  HeadingCandA_tmp = Xt_b_data[0].im / Xt_b_data[0].re;
                  b_HeadingCandA_tmp =
                      Xt_b_data[0].re + HeadingCandA_tmp * Xt_b_data[0].im;
                  z[0].re = (ar + HeadingCandA_tmp * ai) / b_HeadingCandA_tmp;
                  z[0].im = (ai - HeadingCandA_tmp * ar) / b_HeadingCandA_tmp;
                } else if (b_HeadingCandA_tmp == brm) {
                  if (Xt_b_data[0].re > 0.0) {
                    HeadingCandA_tmp = 0.5;
                  } else {
                    HeadingCandA_tmp = -0.5;
                  }
                  if (Xt_b_data[0].im > 0.0) {
                    b_HeadingCandA_tmp = 0.5;
                  } else {
                    b_HeadingCandA_tmp = -0.5;
                  }
                  z[0].re =
                      (ar * HeadingCandA_tmp + ai * b_HeadingCandA_tmp) / brm;
                  z[0].im =
                      (ai * HeadingCandA_tmp - ar * b_HeadingCandA_tmp) / brm;
                } else {
                  HeadingCandA_tmp = Xt_b_data[0].re / Xt_b_data[0].im;
                  b_HeadingCandA_tmp =
                      Xt_b_data[0].im + HeadingCandA_tmp * Xt_b_data[0].re;
                  z[0].re = (HeadingCandA_tmp * ar + ai) / b_HeadingCandA_tmp;
                  z[0].im = (HeadingCandA_tmp * ai - ar) / b_HeadingCandA_tmp;
                }
              }
              ar = Temp[1].re - CenterCandA.re;
              ai = Temp[1].im - CenterCandA.im;
              if (Xt_b_data[1].im == 0.0) {
                if (ai == 0.0) {
                  z[1].re = ar / Xt_b_data[1].re;
                  z[1].im = 0.0;
                } else if (ar == 0.0) {
                  z[1].re = 0.0;
                  z[1].im = ai / Xt_b_data[1].re;
                } else {
                  z[1].re = ar / Xt_b_data[1].re;
                  z[1].im = ai / Xt_b_data[1].re;
                }
              } else if (Xt_b_data[1].re == 0.0) {
                if (ar == 0.0) {
                  z[1].re = ai / Xt_b_data[1].im;
                  z[1].im = 0.0;
                } else if (ai == 0.0) {
                  z[1].re = 0.0;
                  z[1].im = -(ar / Xt_b_data[1].im);
                } else {
                  z[1].re = ai / Xt_b_data[1].im;
                  z[1].im = -(ar / Xt_b_data[1].im);
                }
              } else {
                brm = fabs(Xt_b_data[1].re);
                b_HeadingCandA_tmp = fabs(Xt_b_data[1].im);
                if (brm > b_HeadingCandA_tmp) {
                  HeadingCandA_tmp = Xt_b_data[1].im / Xt_b_data[1].re;
                  b_HeadingCandA_tmp =
                      Xt_b_data[1].re + HeadingCandA_tmp * Xt_b_data[1].im;
                  z[1].re = (ar + HeadingCandA_tmp * ai) / b_HeadingCandA_tmp;
                  z[1].im = (ai - HeadingCandA_tmp * ar) / b_HeadingCandA_tmp;
                } else if (b_HeadingCandA_tmp == brm) {
                  if (Xt_b_data[1].re > 0.0) {
                    HeadingCandA_tmp = 0.5;
                  } else {
                    HeadingCandA_tmp = -0.5;
                  }
                  if (Xt_b_data[1].im > 0.0) {
                    b_HeadingCandA_tmp = 0.5;
                  } else {
                    b_HeadingCandA_tmp = -0.5;
                  }
                  z[1].re =
                      (ar * HeadingCandA_tmp + ai * b_HeadingCandA_tmp) / brm;
                  z[1].im =
                      (ai * HeadingCandA_tmp - ar * b_HeadingCandA_tmp) / brm;
                } else {
                  HeadingCandA_tmp = Xt_b_data[1].re / Xt_b_data[1].im;
                  b_HeadingCandA_tmp =
                      Xt_b_data[1].im + HeadingCandA_tmp * Xt_b_data[1].re;
                  z[1].re = (HeadingCandA_tmp * ar + ai) / b_HeadingCandA_tmp;
                  z[1].im = (HeadingCandA_tmp * ai - ar) / b_HeadingCandA_tmp;
                }
              }
              ar = Temp[2].re - CenterCandA.re;
              ai = Temp[2].im - CenterCandA.im;
              if (Xt_b_data[2].im == 0.0) {
                if (ai == 0.0) {
                  z[2].re = ar / Xt_b_data[2].re;
                  z[2].im = 0.0;
                } else if (ar == 0.0) {
                  z[2].re = 0.0;
                  z[2].im = ai / Xt_b_data[2].re;
                } else {
                  z[2].re = ar / Xt_b_data[2].re;
                  z[2].im = ai / Xt_b_data[2].re;
                }
              } else if (Xt_b_data[2].re == 0.0) {
                if (ar == 0.0) {
                  z[2].re = ai / Xt_b_data[2].im;
                  z[2].im = 0.0;
                } else if (ai == 0.0) {
                  z[2].re = 0.0;
                  z[2].im = -(ar / Xt_b_data[2].im);
                } else {
                  z[2].re = ai / Xt_b_data[2].im;
                  z[2].im = -(ar / Xt_b_data[2].im);
                }
              } else {
                brm = fabs(Xt_b_data[2].re);
                b_HeadingCandA_tmp = fabs(Xt_b_data[2].im);
                if (brm > b_HeadingCandA_tmp) {
                  HeadingCandA_tmp = Xt_b_data[2].im / Xt_b_data[2].re;
                  b_HeadingCandA_tmp =
                      Xt_b_data[2].re + HeadingCandA_tmp * Xt_b_data[2].im;
                  z[2].re = (ar + HeadingCandA_tmp * ai) / b_HeadingCandA_tmp;
                  z[2].im = (ai - HeadingCandA_tmp * ar) / b_HeadingCandA_tmp;
                } else if (b_HeadingCandA_tmp == brm) {
                  if (Xt_b_data[2].re > 0.0) {
                    HeadingCandA_tmp = 0.5;
                  } else {
                    HeadingCandA_tmp = -0.5;
                  }
                  if (Xt_b_data[2].im > 0.0) {
                    b_HeadingCandA_tmp = 0.5;
                  } else {
                    b_HeadingCandA_tmp = -0.5;
                  }
                  z[2].re =
                      (ar * HeadingCandA_tmp + ai * b_HeadingCandA_tmp) / brm;
                  z[2].im =
                      (ai * HeadingCandA_tmp - ar * b_HeadingCandA_tmp) / brm;
                } else {
                  HeadingCandA_tmp = Xt_b_data[2].re / Xt_b_data[2].im;
                  b_HeadingCandA_tmp =
                      Xt_b_data[2].im + HeadingCandA_tmp * Xt_b_data[2].re;
                  z[2].re = (HeadingCandA_tmp * ar + ai) / b_HeadingCandA_tmp;
                  z[2].im = (HeadingCandA_tmp * ai - ar) / b_HeadingCandA_tmp;
                }
              }
              ar = Temp[3].re - CenterCandA.re;
              ai = Temp[3].im - CenterCandA.im;
              if (Xt_b_data[3].im == 0.0) {
                if (ai == 0.0) {
                  z[3].re = ar / Xt_b_data[3].re;
                  z[3].im = 0.0;
                } else if (ar == 0.0) {
                  z[3].re = 0.0;
                  z[3].im = ai / Xt_b_data[3].re;
                } else {
                  z[3].re = ar / Xt_b_data[3].re;
                  z[3].im = ai / Xt_b_data[3].re;
                }
              } else if (Xt_b_data[3].re == 0.0) {
                if (ar == 0.0) {
                  z[3].re = ai / Xt_b_data[3].im;
                  z[3].im = 0.0;
                } else if (ai == 0.0) {
                  z[3].re = 0.0;
                  z[3].im = -(ar / Xt_b_data[3].im);
                } else {
                  z[3].re = ai / Xt_b_data[3].im;
                  z[3].im = -(ar / Xt_b_data[3].im);
                }
              } else {
                brm = fabs(Xt_b_data[3].re);
                b_HeadingCandA_tmp = fabs(Xt_b_data[3].im);
                if (brm > b_HeadingCandA_tmp) {
                  HeadingCandA_tmp = Xt_b_data[3].im / Xt_b_data[3].re;
                  b_HeadingCandA_tmp =
                      Xt_b_data[3].re + HeadingCandA_tmp * Xt_b_data[3].im;
                  z[3].re = (ar + HeadingCandA_tmp * ai) / b_HeadingCandA_tmp;
                  z[3].im = (ai - HeadingCandA_tmp * ar) / b_HeadingCandA_tmp;
                } else if (b_HeadingCandA_tmp == brm) {
                  if (Xt_b_data[3].re > 0.0) {
                    HeadingCandA_tmp = 0.5;
                  } else {
                    HeadingCandA_tmp = -0.5;
                  }
                  if (Xt_b_data[3].im > 0.0) {
                    b_HeadingCandA_tmp = 0.5;
                  } else {
                    b_HeadingCandA_tmp = -0.5;
                  }
                  z[3].re =
                      (ar * HeadingCandA_tmp + ai * b_HeadingCandA_tmp) / brm;
                  z[3].im =
                      (ai * HeadingCandA_tmp - ar * b_HeadingCandA_tmp) / brm;
                } else {
                  HeadingCandA_tmp = Xt_b_data[3].re / Xt_b_data[3].im;
                  b_HeadingCandA_tmp =
                      Xt_b_data[3].im + HeadingCandA_tmp * Xt_b_data[3].re;
                  z[3].re = (HeadingCandA_tmp * ar + ai) / b_HeadingCandA_tmp;
                  z[3].im = (HeadingCandA_tmp * ai - ar) / b_HeadingCandA_tmp;
                }
              }
            } else {
              binary_expand_op_2(z, Temp, CenterCandA, Xt_b_data, Xt_b_size);
            }
            b_HeadingCandA_tmp = ((z[0].re + z[1].re) + z[2].re) + z[3].re;
            HeadingCandA_tmp = ((z[0].im + z[1].im) + z[2].im) + z[3].im;
            if (HeadingCandA_tmp == 0.0) {
              brm = b_HeadingCandA_tmp / 4.0;
              b_HeadingCandA_tmp = 0.0;
            } else if (b_HeadingCandA_tmp == 0.0) {
              brm = 0.0;
              b_HeadingCandA_tmp = HeadingCandA_tmp / 4.0;
            } else {
              brm = b_HeadingCandA_tmp / 4.0;
              b_HeadingCandA_tmp = HeadingCandA_tmp / 4.0;
            }
            HeadingCandA.re = brm;
            HeadingCandA.im = b_HeadingCandA_tmp;
          } else {
            creal_T b_z[2];
            double ai;
            double ar;
            double b_PosUWB2_tmp;
            double bi;
            double br;
            double brm;
            boolean_T Temp_tmp;
            boolean_T guard1;
            boolean_T guard2;
            boolean_T guard3;
            Temp_tmp = ((Temp[0].re != 0.0) || (Temp[0].im != 0.0));
            guard1 = false;
            guard2 = false;
            guard3 = false;
            if (Temp_tmp) {
              PosUWB2_tmp = b_PosUWB2[3].f1.data[nn].re;
              b_PosUWB2_tmp = b_PosUWB2[3].f1.data[nn].im;
              if ((PosUWB2_tmp != 0.0) || (b_PosUWB2_tmp != 0.0)) {
                ar = Temp[0].re + PosUWB2_tmp;
                ai = Temp[0].im + b_PosUWB2_tmp;
                if (ai == 0.0) {
                  CenterCandA.re = ar / 2.0;
                  CenterCandA.im = 0.0;
                } else if (ar == 0.0) {
                  CenterCandA.re = 0.0;
                  CenterCandA.im = ai / 2.0;
                } else {
                  CenterCandA.re = ar / 2.0;
                  CenterCandA.im = ai / 2.0;
                }
                ar = Temp[0].re - CenterCandA.re;
                ai = Temp[0].im - CenterCandA.im;
                if (Xt_b_data[0].im == 0.0) {
                  if (ai == 0.0) {
                    b_z[0].re = ar / Xt_b_data[0].re;
                    b_z[0].im = 0.0;
                  } else if (ar == 0.0) {
                    b_z[0].re = 0.0;
                    b_z[0].im = ai / Xt_b_data[0].re;
                  } else {
                    b_z[0].re = ar / Xt_b_data[0].re;
                    b_z[0].im = ai / Xt_b_data[0].re;
                  }
                } else if (Xt_b_data[0].re == 0.0) {
                  if (ar == 0.0) {
                    b_z[0].re = ai / Xt_b_data[0].im;
                    b_z[0].im = 0.0;
                  } else if (ai == 0.0) {
                    b_z[0].re = 0.0;
                    b_z[0].im = -(ar / Xt_b_data[0].im);
                  } else {
                    b_z[0].re = ai / Xt_b_data[0].im;
                    b_z[0].im = -(ar / Xt_b_data[0].im);
                  }
                } else {
                  brm = fabs(Xt_b_data[0].re);
                  b_HeadingCandA_tmp = fabs(Xt_b_data[0].im);
                  if (brm > b_HeadingCandA_tmp) {
                    HeadingCandA_tmp = Xt_b_data[0].im / Xt_b_data[0].re;
                    b_HeadingCandA_tmp =
                        Xt_b_data[0].re + HeadingCandA_tmp * Xt_b_data[0].im;
                    b_z[0].re =
                        (ar + HeadingCandA_tmp * ai) / b_HeadingCandA_tmp;
                    b_z[0].im =
                        (ai - HeadingCandA_tmp * ar) / b_HeadingCandA_tmp;
                  } else if (b_HeadingCandA_tmp == brm) {
                    if (Xt_b_data[0].re > 0.0) {
                      HeadingCandA_tmp = 0.5;
                    } else {
                      HeadingCandA_tmp = -0.5;
                    }
                    if (Xt_b_data[0].im > 0.0) {
                      b_HeadingCandA_tmp = 0.5;
                    } else {
                      b_HeadingCandA_tmp = -0.5;
                    }
                    b_z[0].re =
                        (ar * HeadingCandA_tmp + ai * b_HeadingCandA_tmp) / brm;
                    b_z[0].im =
                        (ai * HeadingCandA_tmp - ar * b_HeadingCandA_tmp) / brm;
                  } else {
                    HeadingCandA_tmp = Xt_b_data[0].re / Xt_b_data[0].im;
                    b_HeadingCandA_tmp =
                        Xt_b_data[0].im + HeadingCandA_tmp * Xt_b_data[0].re;
                    b_z[0].re =
                        (HeadingCandA_tmp * ar + ai) / b_HeadingCandA_tmp;
                    b_z[0].im =
                        (HeadingCandA_tmp * ai - ar) / b_HeadingCandA_tmp;
                  }
                }
                ar = Temp[3].re - CenterCandA.re;
                ai = Temp[3].im - CenterCandA.im;
                if (Xt_b_data[3].im == 0.0) {
                  if (ai == 0.0) {
                    b_z[1].re = ar / Xt_b_data[3].re;
                    b_z[1].im = 0.0;
                  } else if (ar == 0.0) {
                    b_z[1].re = 0.0;
                    b_z[1].im = ai / Xt_b_data[3].re;
                  } else {
                    b_z[1].re = ar / Xt_b_data[3].re;
                    b_z[1].im = ai / Xt_b_data[3].re;
                  }
                } else if (Xt_b_data[3].re == 0.0) {
                  if (ar == 0.0) {
                    b_z[1].re = ai / Xt_b_data[3].im;
                    b_z[1].im = 0.0;
                  } else if (ai == 0.0) {
                    b_z[1].re = 0.0;
                    b_z[1].im = -(ar / Xt_b_data[3].im);
                  } else {
                    b_z[1].re = ai / Xt_b_data[3].im;
                    b_z[1].im = -(ar / Xt_b_data[3].im);
                  }
                } else {
                  brm = fabs(Xt_b_data[3].re);
                  b_HeadingCandA_tmp = fabs(Xt_b_data[3].im);
                  if (brm > b_HeadingCandA_tmp) {
                    HeadingCandA_tmp = Xt_b_data[3].im / Xt_b_data[3].re;
                    b_HeadingCandA_tmp =
                        Xt_b_data[3].re + HeadingCandA_tmp * Xt_b_data[3].im;
                    b_z[1].re =
                        (ar + HeadingCandA_tmp * ai) / b_HeadingCandA_tmp;
                    b_z[1].im =
                        (ai - HeadingCandA_tmp * ar) / b_HeadingCandA_tmp;
                  } else if (b_HeadingCandA_tmp == brm) {
                    if (Xt_b_data[3].re > 0.0) {
                      HeadingCandA_tmp = 0.5;
                    } else {
                      HeadingCandA_tmp = -0.5;
                    }
                    if (Xt_b_data[3].im > 0.0) {
                      b_HeadingCandA_tmp = 0.5;
                    } else {
                      b_HeadingCandA_tmp = -0.5;
                    }
                    b_z[1].re =
                        (ar * HeadingCandA_tmp + ai * b_HeadingCandA_tmp) / brm;
                    b_z[1].im =
                        (ai * HeadingCandA_tmp - ar * b_HeadingCandA_tmp) / brm;
                  } else {
                    HeadingCandA_tmp = Xt_b_data[3].re / Xt_b_data[3].im;
                    b_HeadingCandA_tmp =
                        Xt_b_data[3].im + HeadingCandA_tmp * Xt_b_data[3].re;
                    b_z[1].re =
                        (HeadingCandA_tmp * ar + ai) / b_HeadingCandA_tmp;
                    b_z[1].im =
                        (HeadingCandA_tmp * ai - ar) / b_HeadingCandA_tmp;
                  }
                }
                ar = b_z[0].re + b_z[1].re;
                ai = b_z[0].im + b_z[1].im;
                if (ai == 0.0) {
                  HeadingCandA.re = ar / 2.0;
                  HeadingCandA.im = 0.0;
                } else if (ar == 0.0) {
                  HeadingCandA.re = 0.0;
                  HeadingCandA.im = ai / 2.0;
                } else {
                  HeadingCandA.re = ar / 2.0;
                  HeadingCandA.im = ai / 2.0;
                }
              } else {
                guard3 = true;
              }
            } else {
              guard3 = true;
            }
            if (guard3) {
              b_Temp_tmp = ((Temp[1].re != 0.0) || (Temp[1].im != 0.0));
              if (b_Temp_tmp && ((Temp[2].re != 0.0) || (Temp[2].im != 0.0))) {
                ar = Temp[1].re + Temp[2].re;
                ai = Temp[1].im + Temp[2].im;
                if (ai == 0.0) {
                  CenterCandA.re = ar / 2.0;
                  CenterCandA.im = 0.0;
                } else if (ar == 0.0) {
                  CenterCandA.re = 0.0;
                  CenterCandA.im = ai / 2.0;
                } else {
                  CenterCandA.re = ar / 2.0;
                  CenterCandA.im = ai / 2.0;
                }
                ar = Temp[1].re - CenterCandA.re;
                ai = Temp[1].im - CenterCandA.im;
                if (Xt_b_data[1].im == 0.0) {
                  if (ai == 0.0) {
                    b_z[0].re = ar / Xt_b_data[1].re;
                    b_z[0].im = 0.0;
                  } else if (ar == 0.0) {
                    b_z[0].re = 0.0;
                    b_z[0].im = ai / Xt_b_data[1].re;
                  } else {
                    b_z[0].re = ar / Xt_b_data[1].re;
                    b_z[0].im = ai / Xt_b_data[1].re;
                  }
                } else if (Xt_b_data[1].re == 0.0) {
                  if (ar == 0.0) {
                    b_z[0].re = ai / Xt_b_data[1].im;
                    b_z[0].im = 0.0;
                  } else if (ai == 0.0) {
                    b_z[0].re = 0.0;
                    b_z[0].im = -(ar / Xt_b_data[1].im);
                  } else {
                    b_z[0].re = ai / Xt_b_data[1].im;
                    b_z[0].im = -(ar / Xt_b_data[1].im);
                  }
                } else {
                  brm = fabs(Xt_b_data[1].re);
                  b_HeadingCandA_tmp = fabs(Xt_b_data[1].im);
                  if (brm > b_HeadingCandA_tmp) {
                    HeadingCandA_tmp = Xt_b_data[1].im / Xt_b_data[1].re;
                    b_HeadingCandA_tmp =
                        Xt_b_data[1].re + HeadingCandA_tmp * Xt_b_data[1].im;
                    b_z[0].re =
                        (ar + HeadingCandA_tmp * ai) / b_HeadingCandA_tmp;
                    b_z[0].im =
                        (ai - HeadingCandA_tmp * ar) / b_HeadingCandA_tmp;
                  } else if (b_HeadingCandA_tmp == brm) {
                    if (Xt_b_data[1].re > 0.0) {
                      HeadingCandA_tmp = 0.5;
                    } else {
                      HeadingCandA_tmp = -0.5;
                    }
                    if (Xt_b_data[1].im > 0.0) {
                      b_HeadingCandA_tmp = 0.5;
                    } else {
                      b_HeadingCandA_tmp = -0.5;
                    }
                    b_z[0].re =
                        (ar * HeadingCandA_tmp + ai * b_HeadingCandA_tmp) / brm;
                    b_z[0].im =
                        (ai * HeadingCandA_tmp - ar * b_HeadingCandA_tmp) / brm;
                  } else {
                    HeadingCandA_tmp = Xt_b_data[1].re / Xt_b_data[1].im;
                    b_HeadingCandA_tmp =
                        Xt_b_data[1].im + HeadingCandA_tmp * Xt_b_data[1].re;
                    b_z[0].re =
                        (HeadingCandA_tmp * ar + ai) / b_HeadingCandA_tmp;
                    b_z[0].im =
                        (HeadingCandA_tmp * ai - ar) / b_HeadingCandA_tmp;
                  }
                }
                ar = Temp[2].re - CenterCandA.re;
                ai = Temp[2].im - CenterCandA.im;
                if (Xt_b_data[2].im == 0.0) {
                  if (ai == 0.0) {
                    b_z[1].re = ar / Xt_b_data[2].re;
                    b_z[1].im = 0.0;
                  } else if (ar == 0.0) {
                    b_z[1].re = 0.0;
                    b_z[1].im = ai / Xt_b_data[2].re;
                  } else {
                    b_z[1].re = ar / Xt_b_data[2].re;
                    b_z[1].im = ai / Xt_b_data[2].re;
                  }
                } else if (Xt_b_data[2].re == 0.0) {
                  if (ar == 0.0) {
                    b_z[1].re = ai / Xt_b_data[2].im;
                    b_z[1].im = 0.0;
                  } else if (ai == 0.0) {
                    b_z[1].re = 0.0;
                    b_z[1].im = -(ar / Xt_b_data[2].im);
                  } else {
                    b_z[1].re = ai / Xt_b_data[2].im;
                    b_z[1].im = -(ar / Xt_b_data[2].im);
                  }
                } else {
                  brm = fabs(Xt_b_data[2].re);
                  b_HeadingCandA_tmp = fabs(Xt_b_data[2].im);
                  if (brm > b_HeadingCandA_tmp) {
                    HeadingCandA_tmp = Xt_b_data[2].im / Xt_b_data[2].re;
                    b_HeadingCandA_tmp =
                        Xt_b_data[2].re + HeadingCandA_tmp * Xt_b_data[2].im;
                    b_z[1].re =
                        (ar + HeadingCandA_tmp * ai) / b_HeadingCandA_tmp;
                    b_z[1].im =
                        (ai - HeadingCandA_tmp * ar) / b_HeadingCandA_tmp;
                  } else if (b_HeadingCandA_tmp == brm) {
                    if (Xt_b_data[2].re > 0.0) {
                      HeadingCandA_tmp = 0.5;
                    } else {
                      HeadingCandA_tmp = -0.5;
                    }
                    if (Xt_b_data[2].im > 0.0) {
                      b_HeadingCandA_tmp = 0.5;
                    } else {
                      b_HeadingCandA_tmp = -0.5;
                    }
                    b_z[1].re =
                        (ar * HeadingCandA_tmp + ai * b_HeadingCandA_tmp) / brm;
                    b_z[1].im =
                        (ai * HeadingCandA_tmp - ar * b_HeadingCandA_tmp) / brm;
                  } else {
                    HeadingCandA_tmp = Xt_b_data[2].re / Xt_b_data[2].im;
                    b_HeadingCandA_tmp =
                        Xt_b_data[2].im + HeadingCandA_tmp * Xt_b_data[2].re;
                    b_z[1].re =
                        (HeadingCandA_tmp * ar + ai) / b_HeadingCandA_tmp;
                    b_z[1].im =
                        (HeadingCandA_tmp * ai - ar) / b_HeadingCandA_tmp;
                  }
                }
                ar = b_z[0].re + b_z[1].re;
                ai = b_z[0].im + b_z[1].im;
                if (ai == 0.0) {
                  HeadingCandA.re = ar / 2.0;
                  HeadingCandA.im = 0.0;
                } else if (ar == 0.0) {
                  HeadingCandA.re = 0.0;
                  HeadingCandA.im = ai / 2.0;
                } else {
                  HeadingCandA.re = ar / 2.0;
                  HeadingCandA.im = ai / 2.0;
                }
              } else if (Temp_tmp && b_Temp_tmp) {
                ar = Temp[0].re - Temp[1].re;
                ai = Temp[0].im - Temp[1].im;
                br = Xt_b_data[0].re - Xt_b_data[1].re;
                bi = Xt_b_data[0].im - Xt_b_data[1].im;
                if (bi == 0.0) {
                  if (ai == 0.0) {
                    HeadingCandA.re = ar / br;
                    HeadingCandA.im = 0.0;
                  } else if (ar == 0.0) {
                    HeadingCandA.re = 0.0;
                    HeadingCandA.im = ai / br;
                  } else {
                    HeadingCandA.re = ar / br;
                    HeadingCandA.im = ai / br;
                  }
                } else if (br == 0.0) {
                  if (ar == 0.0) {
                    HeadingCandA.re = ai / bi;
                    HeadingCandA.im = 0.0;
                  } else if (ai == 0.0) {
                    HeadingCandA.re = 0.0;
                    HeadingCandA.im = -(ar / bi);
                  } else {
                    HeadingCandA.re = ai / bi;
                    HeadingCandA.im = -(ar / bi);
                  }
                } else {
                  brm = fabs(br);
                  b_HeadingCandA_tmp = fabs(bi);
                  if (brm > b_HeadingCandA_tmp) {
                    HeadingCandA_tmp = bi / br;
                    b_HeadingCandA_tmp = br + HeadingCandA_tmp * bi;
                    HeadingCandA.re =
                        (ar + HeadingCandA_tmp * ai) / b_HeadingCandA_tmp;
                    HeadingCandA.im =
                        (ai - HeadingCandA_tmp * ar) / b_HeadingCandA_tmp;
                  } else if (b_HeadingCandA_tmp == brm) {
                    if (br > 0.0) {
                      HeadingCandA_tmp = 0.5;
                    } else {
                      HeadingCandA_tmp = -0.5;
                    }
                    if (bi > 0.0) {
                      b_HeadingCandA_tmp = 0.5;
                    } else {
                      b_HeadingCandA_tmp = -0.5;
                    }
                    HeadingCandA.re =
                        (ar * HeadingCandA_tmp + ai * b_HeadingCandA_tmp) / brm;
                    HeadingCandA.im =
                        (ai * HeadingCandA_tmp - ar * b_HeadingCandA_tmp) / brm;
                  } else {
                    HeadingCandA_tmp = br / bi;
                    b_HeadingCandA_tmp = bi + HeadingCandA_tmp * br;
                    HeadingCandA.re =
                        (HeadingCandA_tmp * ar + ai) / b_HeadingCandA_tmp;
                    HeadingCandA.im =
                        (HeadingCandA_tmp * ai - ar) / b_HeadingCandA_tmp;
                  }
                }
                ar = Temp[0].re + Temp[1].re;
                ai = Temp[0].im + Temp[1].im;
                if (ai == 0.0) {
                  brm = ar / 2.0;
                  bi = 0.0;
                } else if (ar == 0.0) {
                  brm = 0.0;
                  bi = ai / 2.0;
                } else {
                  brm = ar / 2.0;
                  bi = ai / 2.0;
                }
                ar = Xt_b_data[0].re + Xt_b_data[1].re;
                ai = Xt_b_data[0].im + Xt_b_data[1].im;
                if (ai == 0.0) {
                  PosUWB2_tmp = ar / 2.0;
                  HeadingCandA_tmp = 0.0;
                } else if (ar == 0.0) {
                  PosUWB2_tmp = 0.0;
                  HeadingCandA_tmp = ai / 2.0;
                } else {
                  PosUWB2_tmp = ar / 2.0;
                  HeadingCandA_tmp = ai / 2.0;
                }
                b_HeadingCandA_tmp = PosUWB2_tmp * HeadingCandA.re -
                                     HeadingCandA_tmp * HeadingCandA.im;
                HeadingCandA_tmp = PosUWB2_tmp * HeadingCandA.im +
                                   HeadingCandA_tmp * HeadingCandA.re;
                br = rt_hypotd_snf(HeadingCandA.re, HeadingCandA.im);
                if (HeadingCandA_tmp == 0.0) {
                  PosUWB2_tmp = b_HeadingCandA_tmp / br;
                  HeadingCandA_tmp = 0.0;
                } else if (b_HeadingCandA_tmp == 0.0) {
                  PosUWB2_tmp = 0.0;
                  HeadingCandA_tmp /= br;
                } else {
                  PosUWB2_tmp = b_HeadingCandA_tmp / br;
                  HeadingCandA_tmp /= br;
                }
                CenterCandA.re = brm - PosUWB2_tmp;
                CenterCandA.im = bi - HeadingCandA_tmp;
              } else if (b_Temp_tmp) {
                PosUWB2_tmp = b_PosUWB2[3].f1.data[nn].re;
                b_PosUWB2_tmp = b_PosUWB2[3].f1.data[nn].im;
                if ((PosUWB2_tmp != 0.0) || (b_PosUWB2_tmp != 0.0)) {
                  ar = Temp[1].re - PosUWB2_tmp;
                  ai = Temp[1].im - b_PosUWB2_tmp;
                  br = Xt_b_data[1].re - Xt_b_data[3].re;
                  bi = Xt_b_data[1].im - Xt_b_data[3].im;
                  if (bi == 0.0) {
                    if (ai == 0.0) {
                      HeadingCandA.re = ar / br;
                      HeadingCandA.im = 0.0;
                    } else if (ar == 0.0) {
                      HeadingCandA.re = 0.0;
                      HeadingCandA.im = ai / br;
                    } else {
                      HeadingCandA.re = ar / br;
                      HeadingCandA.im = ai / br;
                    }
                  } else if (br == 0.0) {
                    if (ar == 0.0) {
                      HeadingCandA.re = ai / bi;
                      HeadingCandA.im = 0.0;
                    } else if (ai == 0.0) {
                      HeadingCandA.re = 0.0;
                      HeadingCandA.im = -(ar / bi);
                    } else {
                      HeadingCandA.re = ai / bi;
                      HeadingCandA.im = -(ar / bi);
                    }
                  } else {
                    brm = fabs(br);
                    b_HeadingCandA_tmp = fabs(bi);
                    if (brm > b_HeadingCandA_tmp) {
                      HeadingCandA_tmp = bi / br;
                      b_HeadingCandA_tmp = br + HeadingCandA_tmp * bi;
                      HeadingCandA.re =
                          (ar + HeadingCandA_tmp * ai) / b_HeadingCandA_tmp;
                      HeadingCandA.im =
                          (ai - HeadingCandA_tmp * ar) / b_HeadingCandA_tmp;
                    } else if (b_HeadingCandA_tmp == brm) {
                      if (br > 0.0) {
                        HeadingCandA_tmp = 0.5;
                      } else {
                        HeadingCandA_tmp = -0.5;
                      }
                      if (bi > 0.0) {
                        b_HeadingCandA_tmp = 0.5;
                      } else {
                        b_HeadingCandA_tmp = -0.5;
                      }
                      HeadingCandA.re =
                          (ar * HeadingCandA_tmp + ai * b_HeadingCandA_tmp) /
                          brm;
                      HeadingCandA.im =
                          (ai * HeadingCandA_tmp - ar * b_HeadingCandA_tmp) /
                          brm;
                    } else {
                      HeadingCandA_tmp = br / bi;
                      b_HeadingCandA_tmp = bi + HeadingCandA_tmp * br;
                      HeadingCandA.re =
                          (HeadingCandA_tmp * ar + ai) / b_HeadingCandA_tmp;
                      HeadingCandA.im =
                          (HeadingCandA_tmp * ai - ar) / b_HeadingCandA_tmp;
                    }
                  }
                  ar = Temp[1].re + PosUWB2_tmp;
                  ai = Temp[1].im + b_PosUWB2_tmp;
                  if (ai == 0.0) {
                    brm = ar / 2.0;
                    bi = 0.0;
                  } else if (ar == 0.0) {
                    brm = 0.0;
                    bi = ai / 2.0;
                  } else {
                    brm = ar / 2.0;
                    bi = ai / 2.0;
                  }
                  ar = Xt_b_data[1].re + Xt_b_data[3].re;
                  ai = Xt_b_data[1].im + Xt_b_data[3].im;
                  if (ai == 0.0) {
                    PosUWB2_tmp = ar / 2.0;
                    HeadingCandA_tmp = 0.0;
                  } else if (ar == 0.0) {
                    PosUWB2_tmp = 0.0;
                    HeadingCandA_tmp = ai / 2.0;
                  } else {
                    PosUWB2_tmp = ar / 2.0;
                    HeadingCandA_tmp = ai / 2.0;
                  }
                  b_HeadingCandA_tmp = PosUWB2_tmp * HeadingCandA.re -
                                       HeadingCandA_tmp * HeadingCandA.im;
                  HeadingCandA_tmp = PosUWB2_tmp * HeadingCandA.im +
                                     HeadingCandA_tmp * HeadingCandA.re;
                  br = rt_hypotd_snf(HeadingCandA.re, HeadingCandA.im);
                  if (HeadingCandA_tmp == 0.0) {
                    PosUWB2_tmp = b_HeadingCandA_tmp / br;
                    HeadingCandA_tmp = 0.0;
                  } else if (b_HeadingCandA_tmp == 0.0) {
                    PosUWB2_tmp = 0.0;
                    HeadingCandA_tmp /= br;
                  } else {
                    PosUWB2_tmp = b_HeadingCandA_tmp / br;
                    HeadingCandA_tmp /= br;
                  }
                  CenterCandA.re = brm - PosUWB2_tmp;
                  CenterCandA.im = bi - HeadingCandA_tmp;
                } else {
                  guard2 = true;
                }
              } else {
                guard2 = true;
              }
            }
            if (guard2) {
              b_Temp_tmp = ((Temp[2].re != 0.0) || (Temp[2].im != 0.0));
              if (b_Temp_tmp) {
                PosUWB2_tmp = b_PosUWB2[3].f1.data[nn].re;
                b_PosUWB2_tmp = b_PosUWB2[3].f1.data[nn].im;
                if ((PosUWB2_tmp != 0.0) || (b_PosUWB2_tmp != 0.0)) {
                  ar = Temp[2].re - PosUWB2_tmp;
                  ai = Temp[2].im - b_PosUWB2_tmp;
                  br = Xt_b_data[2].re - Xt_b_data[3].re;
                  bi = Xt_b_data[2].im - Xt_b_data[3].im;
                  if (bi == 0.0) {
                    if (ai == 0.0) {
                      HeadingCandA.re = ar / br;
                      HeadingCandA.im = 0.0;
                    } else if (ar == 0.0) {
                      HeadingCandA.re = 0.0;
                      HeadingCandA.im = ai / br;
                    } else {
                      HeadingCandA.re = ar / br;
                      HeadingCandA.im = ai / br;
                    }
                  } else if (br == 0.0) {
                    if (ar == 0.0) {
                      HeadingCandA.re = ai / bi;
                      HeadingCandA.im = 0.0;
                    } else if (ai == 0.0) {
                      HeadingCandA.re = 0.0;
                      HeadingCandA.im = -(ar / bi);
                    } else {
                      HeadingCandA.re = ai / bi;
                      HeadingCandA.im = -(ar / bi);
                    }
                  } else {
                    brm = fabs(br);
                    b_HeadingCandA_tmp = fabs(bi);
                    if (brm > b_HeadingCandA_tmp) {
                      HeadingCandA_tmp = bi / br;
                      b_HeadingCandA_tmp = br + HeadingCandA_tmp * bi;
                      HeadingCandA.re =
                          (ar + HeadingCandA_tmp * ai) / b_HeadingCandA_tmp;
                      HeadingCandA.im =
                          (ai - HeadingCandA_tmp * ar) / b_HeadingCandA_tmp;
                    } else if (b_HeadingCandA_tmp == brm) {
                      if (br > 0.0) {
                        HeadingCandA_tmp = 0.5;
                      } else {
                        HeadingCandA_tmp = -0.5;
                      }
                      if (bi > 0.0) {
                        b_HeadingCandA_tmp = 0.5;
                      } else {
                        b_HeadingCandA_tmp = -0.5;
                      }
                      HeadingCandA.re =
                          (ar * HeadingCandA_tmp + ai * b_HeadingCandA_tmp) /
                          brm;
                      HeadingCandA.im =
                          (ai * HeadingCandA_tmp - ar * b_HeadingCandA_tmp) /
                          brm;
                    } else {
                      HeadingCandA_tmp = br / bi;
                      b_HeadingCandA_tmp = bi + HeadingCandA_tmp * br;
                      HeadingCandA.re =
                          (HeadingCandA_tmp * ar + ai) / b_HeadingCandA_tmp;
                      HeadingCandA.im =
                          (HeadingCandA_tmp * ai - ar) / b_HeadingCandA_tmp;
                    }
                  }
                  ar = Temp[2].re + PosUWB2_tmp;
                  ai = Temp[2].im + b_PosUWB2_tmp;
                  if (ai == 0.0) {
                    brm = ar / 2.0;
                    bi = 0.0;
                  } else if (ar == 0.0) {
                    brm = 0.0;
                    bi = ai / 2.0;
                  } else {
                    brm = ar / 2.0;
                    bi = ai / 2.0;
                  }
                  ar = Xt_b_data[2].re + Xt_b_data[3].re;
                  ai = Xt_b_data[2].im + Xt_b_data[3].im;
                  if (ai == 0.0) {
                    PosUWB2_tmp = ar / 2.0;
                    HeadingCandA_tmp = 0.0;
                  } else if (ar == 0.0) {
                    PosUWB2_tmp = 0.0;
                    HeadingCandA_tmp = ai / 2.0;
                  } else {
                    PosUWB2_tmp = ar / 2.0;
                    HeadingCandA_tmp = ai / 2.0;
                  }
                  b_HeadingCandA_tmp = PosUWB2_tmp * HeadingCandA.re -
                                       HeadingCandA_tmp * HeadingCandA.im;
                  HeadingCandA_tmp = PosUWB2_tmp * HeadingCandA.im +
                                     HeadingCandA_tmp * HeadingCandA.re;
                  br = rt_hypotd_snf(HeadingCandA.re, HeadingCandA.im);
                  if (HeadingCandA_tmp == 0.0) {
                    PosUWB2_tmp = b_HeadingCandA_tmp / br;
                    HeadingCandA_tmp = 0.0;
                  } else if (b_HeadingCandA_tmp == 0.0) {
                    PosUWB2_tmp = 0.0;
                    HeadingCandA_tmp /= br;
                  } else {
                    PosUWB2_tmp = b_HeadingCandA_tmp / br;
                    HeadingCandA_tmp /= br;
                  }
                  CenterCandA.re = brm - PosUWB2_tmp;
                  CenterCandA.im = bi - HeadingCandA_tmp;
                } else {
                  guard1 = true;
                }
              } else {
                guard1 = true;
              }
            }
            if (guard1) {
              if (b_Temp_tmp && Temp_tmp) {
                ar = Temp[2].re - Temp[0].re;
                ai = Temp[2].im - Temp[0].im;
                br = Xt_b_data[2].re - Xt_b_data[0].re;
                bi = Xt_b_data[2].im - Xt_b_data[0].im;
                if (bi == 0.0) {
                  if (ai == 0.0) {
                    HeadingCandA.re = ar / br;
                    HeadingCandA.im = 0.0;
                  } else if (ar == 0.0) {
                    HeadingCandA.re = 0.0;
                    HeadingCandA.im = ai / br;
                  } else {
                    HeadingCandA.re = ar / br;
                    HeadingCandA.im = ai / br;
                  }
                } else if (br == 0.0) {
                  if (ar == 0.0) {
                    HeadingCandA.re = ai / bi;
                    HeadingCandA.im = 0.0;
                  } else if (ai == 0.0) {
                    HeadingCandA.re = 0.0;
                    HeadingCandA.im = -(ar / bi);
                  } else {
                    HeadingCandA.re = ai / bi;
                    HeadingCandA.im = -(ar / bi);
                  }
                } else {
                  brm = fabs(br);
                  b_HeadingCandA_tmp = fabs(bi);
                  if (brm > b_HeadingCandA_tmp) {
                    HeadingCandA_tmp = bi / br;
                    b_HeadingCandA_tmp = br + HeadingCandA_tmp * bi;
                    HeadingCandA.re =
                        (ar + HeadingCandA_tmp * ai) / b_HeadingCandA_tmp;
                    HeadingCandA.im =
                        (ai - HeadingCandA_tmp * ar) / b_HeadingCandA_tmp;
                  } else if (b_HeadingCandA_tmp == brm) {
                    if (br > 0.0) {
                      HeadingCandA_tmp = 0.5;
                    } else {
                      HeadingCandA_tmp = -0.5;
                    }
                    if (bi > 0.0) {
                      b_HeadingCandA_tmp = 0.5;
                    } else {
                      b_HeadingCandA_tmp = -0.5;
                    }
                    HeadingCandA.re =
                        (ar * HeadingCandA_tmp + ai * b_HeadingCandA_tmp) / brm;
                    HeadingCandA.im =
                        (ai * HeadingCandA_tmp - ar * b_HeadingCandA_tmp) / brm;
                  } else {
                    HeadingCandA_tmp = br / bi;
                    b_HeadingCandA_tmp = bi + HeadingCandA_tmp * br;
                    HeadingCandA.re =
                        (HeadingCandA_tmp * ar + ai) / b_HeadingCandA_tmp;
                    HeadingCandA.im =
                        (HeadingCandA_tmp * ai - ar) / b_HeadingCandA_tmp;
                  }
                }
                ar = Temp[0].re + Temp[2].re;
                ai = Temp[0].im + Temp[2].im;
                if (ai == 0.0) {
                  brm = ar / 2.0;
                  bi = 0.0;
                } else if (ar == 0.0) {
                  brm = 0.0;
                  bi = ai / 2.0;
                } else {
                  brm = ar / 2.0;
                  bi = ai / 2.0;
                }
                ar = Xt_b_data[0].re + Xt_b_data[2].re;
                ai = Xt_b_data[0].im + Xt_b_data[2].im;
                if (ai == 0.0) {
                  PosUWB2_tmp = ar / 2.0;
                  HeadingCandA_tmp = 0.0;
                } else if (ar == 0.0) {
                  PosUWB2_tmp = 0.0;
                  HeadingCandA_tmp = ai / 2.0;
                } else {
                  PosUWB2_tmp = ar / 2.0;
                  HeadingCandA_tmp = ai / 2.0;
                }
                b_HeadingCandA_tmp = PosUWB2_tmp * HeadingCandA.re -
                                     HeadingCandA_tmp * HeadingCandA.im;
                HeadingCandA_tmp = PosUWB2_tmp * HeadingCandA.im +
                                   HeadingCandA_tmp * HeadingCandA.re;
                br = rt_hypotd_snf(HeadingCandA.re, HeadingCandA.im);
                if (HeadingCandA_tmp == 0.0) {
                  PosUWB2_tmp = b_HeadingCandA_tmp / br;
                  HeadingCandA_tmp = 0.0;
                } else if (b_HeadingCandA_tmp == 0.0) {
                  PosUWB2_tmp = 0.0;
                  HeadingCandA_tmp /= br;
                } else {
                  PosUWB2_tmp = b_HeadingCandA_tmp / br;
                  HeadingCandA_tmp /= br;
                }
                CenterCandA.re = brm - PosUWB2_tmp;
                CenterCandA.im = bi - HeadingCandA_tmp;
              } else {
                b_HeadingCandA_tmp =
                    ((Temp[0].re + Temp[1].re) + Temp[2].re) + Temp[3].re;
                HeadingCandA_tmp =
                    ((Temp[0].im + Temp[1].im) + Temp[2].im) + Temp[3].im;
                if (HeadingCandA_tmp == 0.0) {
                  CenterCandA.re = b_HeadingCandA_tmp / 4.0;
                  CenterCandA.im = 0.0;
                } else if (b_HeadingCandA_tmp == 0.0) {
                  CenterCandA.re = 0.0;
                  CenterCandA.im = HeadingCandA_tmp / 4.0;
                } else {
                  CenterCandA.re = b_HeadingCandA_tmp / 4.0;
                  CenterCandA.im = HeadingCandA_tmp / 4.0;
                }
                if (Xt_b_size[1] == 4) {
                  ar = Temp[0].re - CenterCandA.re;
                  ai = Temp[0].im - CenterCandA.im;
                  if (Xt_b_data[0].im == 0.0) {
                    if (ai == 0.0) {
                      z[0].re = ar / Xt_b_data[0].re;
                      z[0].im = 0.0;
                    } else if (ar == 0.0) {
                      z[0].re = 0.0;
                      z[0].im = ai / Xt_b_data[0].re;
                    } else {
                      z[0].re = ar / Xt_b_data[0].re;
                      z[0].im = ai / Xt_b_data[0].re;
                    }
                  } else if (Xt_b_data[0].re == 0.0) {
                    if (ar == 0.0) {
                      z[0].re = ai / Xt_b_data[0].im;
                      z[0].im = 0.0;
                    } else if (ai == 0.0) {
                      z[0].re = 0.0;
                      z[0].im = -(ar / Xt_b_data[0].im);
                    } else {
                      z[0].re = ai / Xt_b_data[0].im;
                      z[0].im = -(ar / Xt_b_data[0].im);
                    }
                  } else {
                    brm = fabs(Xt_b_data[0].re);
                    b_HeadingCandA_tmp = fabs(Xt_b_data[0].im);
                    if (brm > b_HeadingCandA_tmp) {
                      HeadingCandA_tmp = Xt_b_data[0].im / Xt_b_data[0].re;
                      b_HeadingCandA_tmp =
                          Xt_b_data[0].re + HeadingCandA_tmp * Xt_b_data[0].im;
                      z[0].re =
                          (ar + HeadingCandA_tmp * ai) / b_HeadingCandA_tmp;
                      z[0].im =
                          (ai - HeadingCandA_tmp * ar) / b_HeadingCandA_tmp;
                    } else if (b_HeadingCandA_tmp == brm) {
                      if (Xt_b_data[0].re > 0.0) {
                        HeadingCandA_tmp = 0.5;
                      } else {
                        HeadingCandA_tmp = -0.5;
                      }
                      if (Xt_b_data[0].im > 0.0) {
                        b_HeadingCandA_tmp = 0.5;
                      } else {
                        b_HeadingCandA_tmp = -0.5;
                      }
                      z[0].re =
                          (ar * HeadingCandA_tmp + ai * b_HeadingCandA_tmp) /
                          brm;
                      z[0].im =
                          (ai * HeadingCandA_tmp - ar * b_HeadingCandA_tmp) /
                          brm;
                    } else {
                      HeadingCandA_tmp = Xt_b_data[0].re / Xt_b_data[0].im;
                      b_HeadingCandA_tmp =
                          Xt_b_data[0].im + HeadingCandA_tmp * Xt_b_data[0].re;
                      z[0].re =
                          (HeadingCandA_tmp * ar + ai) / b_HeadingCandA_tmp;
                      z[0].im =
                          (HeadingCandA_tmp * ai - ar) / b_HeadingCandA_tmp;
                    }
                  }
                  ar = Temp[1].re - CenterCandA.re;
                  ai = Temp[1].im - CenterCandA.im;
                  if (Xt_b_data[1].im == 0.0) {
                    if (ai == 0.0) {
                      z[1].re = ar / Xt_b_data[1].re;
                      z[1].im = 0.0;
                    } else if (ar == 0.0) {
                      z[1].re = 0.0;
                      z[1].im = ai / Xt_b_data[1].re;
                    } else {
                      z[1].re = ar / Xt_b_data[1].re;
                      z[1].im = ai / Xt_b_data[1].re;
                    }
                  } else if (Xt_b_data[1].re == 0.0) {
                    if (ar == 0.0) {
                      z[1].re = ai / Xt_b_data[1].im;
                      z[1].im = 0.0;
                    } else if (ai == 0.0) {
                      z[1].re = 0.0;
                      z[1].im = -(ar / Xt_b_data[1].im);
                    } else {
                      z[1].re = ai / Xt_b_data[1].im;
                      z[1].im = -(ar / Xt_b_data[1].im);
                    }
                  } else {
                    brm = fabs(Xt_b_data[1].re);
                    b_HeadingCandA_tmp = fabs(Xt_b_data[1].im);
                    if (brm > b_HeadingCandA_tmp) {
                      HeadingCandA_tmp = Xt_b_data[1].im / Xt_b_data[1].re;
                      b_HeadingCandA_tmp =
                          Xt_b_data[1].re + HeadingCandA_tmp * Xt_b_data[1].im;
                      z[1].re =
                          (ar + HeadingCandA_tmp * ai) / b_HeadingCandA_tmp;
                      z[1].im =
                          (ai - HeadingCandA_tmp * ar) / b_HeadingCandA_tmp;
                    } else if (b_HeadingCandA_tmp == brm) {
                      if (Xt_b_data[1].re > 0.0) {
                        HeadingCandA_tmp = 0.5;
                      } else {
                        HeadingCandA_tmp = -0.5;
                      }
                      if (Xt_b_data[1].im > 0.0) {
                        b_HeadingCandA_tmp = 0.5;
                      } else {
                        b_HeadingCandA_tmp = -0.5;
                      }
                      z[1].re =
                          (ar * HeadingCandA_tmp + ai * b_HeadingCandA_tmp) /
                          brm;
                      z[1].im =
                          (ai * HeadingCandA_tmp - ar * b_HeadingCandA_tmp) /
                          brm;
                    } else {
                      HeadingCandA_tmp = Xt_b_data[1].re / Xt_b_data[1].im;
                      b_HeadingCandA_tmp =
                          Xt_b_data[1].im + HeadingCandA_tmp * Xt_b_data[1].re;
                      z[1].re =
                          (HeadingCandA_tmp * ar + ai) / b_HeadingCandA_tmp;
                      z[1].im =
                          (HeadingCandA_tmp * ai - ar) / b_HeadingCandA_tmp;
                    }
                  }
                  ar = Temp[2].re - CenterCandA.re;
                  ai = Temp[2].im - CenterCandA.im;
                  if (Xt_b_data[2].im == 0.0) {
                    if (ai == 0.0) {
                      z[2].re = ar / Xt_b_data[2].re;
                      z[2].im = 0.0;
                    } else if (ar == 0.0) {
                      z[2].re = 0.0;
                      z[2].im = ai / Xt_b_data[2].re;
                    } else {
                      z[2].re = ar / Xt_b_data[2].re;
                      z[2].im = ai / Xt_b_data[2].re;
                    }
                  } else if (Xt_b_data[2].re == 0.0) {
                    if (ar == 0.0) {
                      z[2].re = ai / Xt_b_data[2].im;
                      z[2].im = 0.0;
                    } else if (ai == 0.0) {
                      z[2].re = 0.0;
                      z[2].im = -(ar / Xt_b_data[2].im);
                    } else {
                      z[2].re = ai / Xt_b_data[2].im;
                      z[2].im = -(ar / Xt_b_data[2].im);
                    }
                  } else {
                    brm = fabs(Xt_b_data[2].re);
                    b_HeadingCandA_tmp = fabs(Xt_b_data[2].im);
                    if (brm > b_HeadingCandA_tmp) {
                      HeadingCandA_tmp = Xt_b_data[2].im / Xt_b_data[2].re;
                      b_HeadingCandA_tmp =
                          Xt_b_data[2].re + HeadingCandA_tmp * Xt_b_data[2].im;
                      z[2].re =
                          (ar + HeadingCandA_tmp * ai) / b_HeadingCandA_tmp;
                      z[2].im =
                          (ai - HeadingCandA_tmp * ar) / b_HeadingCandA_tmp;
                    } else if (b_HeadingCandA_tmp == brm) {
                      if (Xt_b_data[2].re > 0.0) {
                        HeadingCandA_tmp = 0.5;
                      } else {
                        HeadingCandA_tmp = -0.5;
                      }
                      if (Xt_b_data[2].im > 0.0) {
                        b_HeadingCandA_tmp = 0.5;
                      } else {
                        b_HeadingCandA_tmp = -0.5;
                      }
                      z[2].re =
                          (ar * HeadingCandA_tmp + ai * b_HeadingCandA_tmp) /
                          brm;
                      z[2].im =
                          (ai * HeadingCandA_tmp - ar * b_HeadingCandA_tmp) /
                          brm;
                    } else {
                      HeadingCandA_tmp = Xt_b_data[2].re / Xt_b_data[2].im;
                      b_HeadingCandA_tmp =
                          Xt_b_data[2].im + HeadingCandA_tmp * Xt_b_data[2].re;
                      z[2].re =
                          (HeadingCandA_tmp * ar + ai) / b_HeadingCandA_tmp;
                      z[2].im =
                          (HeadingCandA_tmp * ai - ar) / b_HeadingCandA_tmp;
                    }
                  }
                  ar = Temp[3].re - CenterCandA.re;
                  ai = Temp[3].im - CenterCandA.im;
                  if (Xt_b_data[3].im == 0.0) {
                    if (ai == 0.0) {
                      z[3].re = ar / Xt_b_data[3].re;
                      z[3].im = 0.0;
                    } else if (ar == 0.0) {
                      z[3].re = 0.0;
                      z[3].im = ai / Xt_b_data[3].re;
                    } else {
                      z[3].re = ar / Xt_b_data[3].re;
                      z[3].im = ai / Xt_b_data[3].re;
                    }
                  } else if (Xt_b_data[3].re == 0.0) {
                    if (ar == 0.0) {
                      z[3].re = ai / Xt_b_data[3].im;
                      z[3].im = 0.0;
                    } else if (ai == 0.0) {
                      z[3].re = 0.0;
                      z[3].im = -(ar / Xt_b_data[3].im);
                    } else {
                      z[3].re = ai / Xt_b_data[3].im;
                      z[3].im = -(ar / Xt_b_data[3].im);
                    }
                  } else {
                    brm = fabs(Xt_b_data[3].re);
                    b_HeadingCandA_tmp = fabs(Xt_b_data[3].im);
                    if (brm > b_HeadingCandA_tmp) {
                      HeadingCandA_tmp = Xt_b_data[3].im / Xt_b_data[3].re;
                      b_HeadingCandA_tmp =
                          Xt_b_data[3].re + HeadingCandA_tmp * Xt_b_data[3].im;
                      z[3].re =
                          (ar + HeadingCandA_tmp * ai) / b_HeadingCandA_tmp;
                      z[3].im =
                          (ai - HeadingCandA_tmp * ar) / b_HeadingCandA_tmp;
                    } else if (b_HeadingCandA_tmp == brm) {
                      if (Xt_b_data[3].re > 0.0) {
                        HeadingCandA_tmp = 0.5;
                      } else {
                        HeadingCandA_tmp = -0.5;
                      }
                      if (Xt_b_data[3].im > 0.0) {
                        b_HeadingCandA_tmp = 0.5;
                      } else {
                        b_HeadingCandA_tmp = -0.5;
                      }
                      z[3].re =
                          (ar * HeadingCandA_tmp + ai * b_HeadingCandA_tmp) /
                          brm;
                      z[3].im =
                          (ai * HeadingCandA_tmp - ar * b_HeadingCandA_tmp) /
                          brm;
                    } else {
                      HeadingCandA_tmp = Xt_b_data[3].re / Xt_b_data[3].im;
                      b_HeadingCandA_tmp =
                          Xt_b_data[3].im + HeadingCandA_tmp * Xt_b_data[3].re;
                      z[3].re =
                          (HeadingCandA_tmp * ar + ai) / b_HeadingCandA_tmp;
                      z[3].im =
                          (HeadingCandA_tmp * ai - ar) / b_HeadingCandA_tmp;
                    }
                  }
                } else {
                  binary_expand_op_2(z, Temp, CenterCandA, Xt_b_data,
                                     Xt_b_size);
                }
                b_HeadingCandA_tmp = ((z[0].re + z[1].re) + z[2].re) + z[3].re;
                HeadingCandA_tmp = ((z[0].im + z[1].im) + z[2].im) + z[3].im;
                if (HeadingCandA_tmp == 0.0) {
                  brm = b_HeadingCandA_tmp / 4.0;
                  b_HeadingCandA_tmp = 0.0;
                } else if (b_HeadingCandA_tmp == 0.0) {
                  brm = 0.0;
                  b_HeadingCandA_tmp = HeadingCandA_tmp / 4.0;
                } else {
                  brm = b_HeadingCandA_tmp / 4.0;
                  b_HeadingCandA_tmp = HeadingCandA_tmp / 4.0;
                }
                HeadingCandA.re = brm;
                HeadingCandA.im = b_HeadingCandA_tmp;
              }
            }
          }
          b_HeadingCandA_tmp = rt_hypotd_snf(HeadingCandA.re, HeadingCandA.im);
          HeadingCandA_tmp = fabs(b_HeadingCandA_tmp - 1.0);
          if (HeadingCandA_tmp <= 1.0) {
            creal_T x[32];
            double b_y[32];
            int y_tmp;
            boolean_T b_x[32];
            boolean_T exitg1;
            PosUWB2_tmp = Xt_b_data[0].re * HeadingCandA.re -
                          Xt_b_data[0].im * HeadingCandA.im;
            HeadingCandA_tmp = Xt_b_data[0].re * HeadingCandA.im +
                               Xt_b_data[0].im * HeadingCandA.re;
            if (HeadingCandA_tmp == 0.0) {
              PosUWB2_tmp /= b_HeadingCandA_tmp;
              HeadingCandA_tmp = 0.0;
            } else if (PosUWB2_tmp == 0.0) {
              PosUWB2_tmp = 0.0;
              HeadingCandA_tmp /= b_HeadingCandA_tmp;
            } else {
              PosUWB2_tmp /= b_HeadingCandA_tmp;
              HeadingCandA_tmp /= b_HeadingCandA_tmp;
            }
            TagCandA[ss].re = CenterCandA.re + PosUWB2_tmp;
            TagCandA[ss].im = CenterCandA.im + HeadingCandA_tmp;
            PosUWB2_tmp = Xt_b_data[1].re * HeadingCandA.re -
                          Xt_b_data[1].im * HeadingCandA.im;
            HeadingCandA_tmp = Xt_b_data[1].re * HeadingCandA.im +
                               Xt_b_data[1].im * HeadingCandA.re;
            if (HeadingCandA_tmp == 0.0) {
              PosUWB2_tmp /= b_HeadingCandA_tmp;
              HeadingCandA_tmp = 0.0;
            } else if (PosUWB2_tmp == 0.0) {
              PosUWB2_tmp = 0.0;
              HeadingCandA_tmp /= b_HeadingCandA_tmp;
            } else {
              PosUWB2_tmp /= b_HeadingCandA_tmp;
              HeadingCandA_tmp /= b_HeadingCandA_tmp;
            }
            TagCandA[ss + 20000].re = CenterCandA.re + PosUWB2_tmp;
            TagCandA[ss + 20000].im = CenterCandA.im + HeadingCandA_tmp;
            PosUWB2_tmp = Xt_b_data[2].re * HeadingCandA.re -
                          Xt_b_data[2].im * HeadingCandA.im;
            HeadingCandA_tmp = Xt_b_data[2].re * HeadingCandA.im +
                               Xt_b_data[2].im * HeadingCandA.re;
            if (HeadingCandA_tmp == 0.0) {
              PosUWB2_tmp /= b_HeadingCandA_tmp;
              HeadingCandA_tmp = 0.0;
            } else if (PosUWB2_tmp == 0.0) {
              PosUWB2_tmp = 0.0;
              HeadingCandA_tmp /= b_HeadingCandA_tmp;
            } else {
              PosUWB2_tmp /= b_HeadingCandA_tmp;
              HeadingCandA_tmp /= b_HeadingCandA_tmp;
            }
            TagCandA[ss + 40000].re = CenterCandA.re + PosUWB2_tmp;
            TagCandA[ss + 40000].im = CenterCandA.im + HeadingCandA_tmp;
            PosUWB2_tmp = Xt_b_data[3].re * HeadingCandA.re -
                          Xt_b_data[3].im * HeadingCandA.im;
            HeadingCandA_tmp = Xt_b_data[3].re * HeadingCandA.im +
                               Xt_b_data[3].im * HeadingCandA.re;
            if (HeadingCandA_tmp == 0.0) {
              PosUWB2_tmp /= b_HeadingCandA_tmp;
              HeadingCandA_tmp = 0.0;
            } else if (PosUWB2_tmp == 0.0) {
              PosUWB2_tmp = 0.0;
              HeadingCandA_tmp /= b_HeadingCandA_tmp;
            } else {
              PosUWB2_tmp /= b_HeadingCandA_tmp;
              HeadingCandA_tmp /= b_HeadingCandA_tmp;
            }
            TagCandA[ss + 60000].re = CenterCandA.re + PosUWB2_tmp;
            TagCandA[ss + 60000].im = CenterCandA.im + HeadingCandA_tmp;
            for (i4 = 0; i4 < 8; i4++) {
              HeadingCandA_tmp = Xain[i4].re;
              idx = i4 << 2;
              x[idx].re = TagCandA[ss].re - HeadingCandA_tmp;
              b_HeadingCandA_tmp = Xain[i4].im;
              x[idx].im = TagCandA[ss].im - b_HeadingCandA_tmp;
              x[idx + 1].re = TagCandA[ss + 20000].re - HeadingCandA_tmp;
              x[idx + 1].im = TagCandA[ss + 20000].im - b_HeadingCandA_tmp;
              x[idx + 2].re = TagCandA[ss + 40000].re - HeadingCandA_tmp;
              x[idx + 2].im = TagCandA[ss + 40000].im - b_HeadingCandA_tmp;
              x[idx + 3].re = TagCandA[ss + 60000].re - HeadingCandA_tmp;
              x[idx + 3].im = TagCandA[ss + 60000].im - b_HeadingCandA_tmp;
            }
            for (idx = 0; idx < 32; idx++) {
              b_y[idx] = rt_hypotd_snf(x[idx].re, x[idx].im);
            }
            idx = b_DistMap->size[1];
            if (b_DistMap->size[1] == 8) {
              for (i4 = 0; i4 < idx; i4++) {
                y_tmp = i4 << 2;
                b_y[y_tmp] =
                    DistMap_data[4 * i4 + 4 * b_DistMap->size[1] * 99] -
                    b_y[y_tmp];
                b_y[y_tmp + 1] =
                    DistMap_data[(4 * i4 + 4 * b_DistMap->size[1] * 99) + 1] -
                    b_y[y_tmp + 1];
                b_y[y_tmp + 2] =
                    DistMap_data[(4 * i4 + 4 * b_DistMap->size[1] * 99) + 2] -
                    b_y[y_tmp + 2];
                b_y[y_tmp + 3] =
                    DistMap_data[(4 * i4 + 4 * b_DistMap->size[1] * 99) + 3] -
                    b_y[y_tmp + 3];
              }
            } else {
              binary_expand_op_1(b_y, b_DistMap);
            }
            for (idx = 0; idx < 32; idx++) {
              b_x[idx] = (fabs(b_y[idx]) < 1.0);
            }
            idx = 0;
            y_tmp = 0;
            exitg1 = false;
            while ((!exitg1) && (y_tmp < 32)) {
              if (b_x[y_tmp]) {
                idx++;
                if (idx >= 32) {
                  exitg1 = true;
                } else {
                  y_tmp++;
                }
              } else {
                y_tmp++;
              }
            }
            if (idx < 1) {
              LessThan1m1[ss] = 0;
            } else {
              LessThan1m1[ss] = (signed char)idx;
            }
            CenterCands[ss] = CenterCandA;
            HeadingCands[ss] = HeadingCandA;
            ss++;
          } else if (HeadingCandA_tmp < PrevAbsHeadingCandA) {
            PrevAbsHeadingCandA = HeadingCandA_tmp;
            *PosHH = CenterCandA;
            PrevHeadingCandA_re = HeadingCandA.re;
            PrevHeadingCandA_im = HeadingCandA.im;
          }
        }
      }
    }
  }
  emxInit_creal_T(&sortedCenter);
  emxInit_creal_T(&sortedHeading);
  emxInit_int32_T(&r);
  if ((ss + 1 == 1) && (PrevAbsHeadingCandA == 100.0)) {
    PosHH->re = 0.0;
    PosHH->im = 0.0;
    HeadingHH = 0.0;
  } else if (ss + 1 == 1) {
    HeadingHH = rt_atan2d_snf(PrevHeadingCandA_im, PrevHeadingCandA_re);
  } else {
    short size_tmp_idx_1;
    boolean_T b_LessThan1m1[20000];
    for (i = 0; i < 20000; i++) {
      y[i] = LessThan1m1[i];
    }
    sort(y, iidx);
    for (i = 0; i < 20000; i++) {
      b_LessThan1m1[i] = (LessThan1m1[i] == y[0]);
    }
    eml_find(b_LessThan1m1, r);
    size_tmp_idx_1 = (short)r->size[1];
    for (i = 0; i < 20000; i++) {
      b_LessThan1m1[i] = (LessThan1m1[i] == y[0]);
    }
    eml_find(b_LessThan1m1, r);
    if (r->size[1] < 1) {
      idx = 0;
    } else {
      idx = size_tmp_idx_1;
    }
    i = sortedCenter->size[0] * sortedCenter->size[1];
    sortedCenter->size[0] = 1;
    sortedCenter->size[1] = idx;
    emxEnsureCapacity_creal_T(sortedCenter, i);
    sortedCenter_data = sortedCenter->data;
    for (i = 0; i < idx; i++) {
      sortedCenter_data[i] = CenterCands[iidx[i] - 1];
    }
    i = sortedHeading->size[0] * sortedHeading->size[1];
    sortedHeading->size[0] = 1;
    sortedHeading->size[1] = idx;
    emxEnsureCapacity_creal_T(sortedHeading, i);
    sortedCenter_data = sortedHeading->data;
    for (i = 0; i < idx; i++) {
      sortedCenter_data[i] = HeadingCands[iidx[i] - 1];
    }
    *PosHH = mean(sortedCenter);
    HeadingCandA = mean(sortedHeading);
    HeadingHH = rt_atan2d_snf(HeadingCandA.im, HeadingCandA.re);
  }
  emxFree_int32_T(&r);
  emxFree_creal_T(&sortedHeading);
  emxFree_creal_T(&sortedCenter);
  return HeadingHH;
}

/* End of code generation (UWBMultiTagPos_V3_1.c) */
