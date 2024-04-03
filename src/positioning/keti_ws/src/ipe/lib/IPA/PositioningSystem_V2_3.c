/*
 * PositioningSystem_V2_3.c
 *
 * Code generation for function 'PositioningSystem_V2_3'
 *
 */

/* Include files */
#include "PositioningSystem_V2_3.h"
#include "AngleMatching.h"
#include "PositioningSystem_V2_3_data.h"
#include "PositioningSystem_V2_3_initialize.h"
#include "PredEKF_3D_Simple.h"
#include "UWBPosition_V3_1.h"
#include "makePredA_3D_Simple.h"
#include "mrdivide_helper.h"
#include "polyfit.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Type Definitions */
#ifndef struct_emxArray_real_T_1x50
#define struct_emxArray_real_T_1x50
struct emxArray_real_T_1x50 {
  double data[50];
  int size[2];
};
#endif /* struct_emxArray_real_T_1x50 */
#ifndef typedef_emxArray_real_T_1x50
#define typedef_emxArray_real_T_1x50
typedef struct emxArray_real_T_1x50 emxArray_real_T_1x50;
#endif /* typedef_emxArray_real_T_1x50 */

/* Variable Definitions */
static double prev_state;

static double IMUacc_c_fifo[12];

static double IMUgyro_c_fifo[12];

static double XhatUWB[15];

static double XhatIMU[15];

static creal_T PrevPosHUWB;

static double PrevHeadingHUWB;

static double s_time_vec[4];

static double UWBDataSet;

static double prevUWBDataSet;

static emxArray_real_T_1x50 xt_b;

static emxArray_real_T_1x50 yt_b;

static double P[225];

static double UWBAnc_Full;

static double PrevHeadingIMU;

static creal_T PosH_vec[10];

static double PosH_vec_stime[10];

static double HeadingH_vec[10];

/* Function Definitions */
void PositioningSystem_V2_3(const double PositionVector_data[],
                            const int PositionVector_size[2],
                            double PositionOut[10])
{
  static const double dv1[225] = {
      1.0E-5, 0.0, 0.0, 0.0, 0.0,    0.0, 0.0, 0.0, 0.0,    0.0, 0.0, 0.0,
      0.0,    0.0, 0.0, 0.0, 1.0E-5, 0.0, 0.0, 0.0, 0.0,    0.0, 0.0, 0.0,
      0.0,    0.0, 0.0, 0.0, 0.0,    0.0, 0.0, 0.0, 1.0E-5, 0.0, 0.0, 0.0,
      0.0,    0.0, 0.0, 0.0, 0.0,    0.0, 0.0, 0.0, 0.0,    0.0, 0.0, 0.0,
      1.0E-5, 0.0, 0.0, 0.0, 0.0,    0.0, 0.0, 0.0, 0.0,    0.0, 0.0, 0.0,
      0.0,    0.0, 0.0, 0.0, 1.0E-5, 0.0, 0.0, 0.0, 0.0,    0.0, 0.0, 0.0,
      0.0,    0.0, 0.0, 0.0, 0.0,    0.0, 0.0, 0.0, 1.0E-5, 0.0, 0.0, 0.0,
      0.0,    0.0, 0.0, 0.0, 0.0,    0.0, 0.0, 0.0, 0.0,    0.0, 0.0, 0.0,
      1.0E-5, 0.0, 0.0, 0.0, 0.0,    0.0, 0.0, 0.0, 0.0,    0.0, 0.0, 0.0,
      0.0,    0.0, 0.0, 0.0, 1.0E-5, 0.0, 0.0, 0.0, 0.0,    0.0, 0.0, 0.0,
      0.0,    0.0, 0.0, 0.0, 0.0,    0.0, 0.0, 0.0, 1.0E-5, 0.0, 0.0, 0.0,
      0.0,    0.0, 0.0, 0.0, 0.0,    0.0, 0.0, 0.0, 0.0,    0.0, 0.0, 0.0,
      1.0E-5, 0.0, 0.0, 0.0, 0.0,    0.0, 0.0, 0.0, 0.0,    0.0, 0.0, 0.0,
      0.0,    0.0, 0.0, 0.0, 1.0E-5, 0.0, 0.0, 0.0, 0.0,    0.0, 0.0, 0.0,
      0.0,    0.0, 0.0, 0.0, 0.0,    0.0, 0.0, 0.0, 1.0E-5, 0.0, 0.0, 0.0,
      0.0,    0.0, 0.0, 0.0, 0.0,    0.0, 0.0, 0.0, 0.0,    0.0, 0.0, 0.0,
      1.0E-5, 0.0, 0.0, 0.0, 0.0,    0.0, 0.0, 0.0, 0.0,    0.0, 0.0, 0.0,
      0.0,    0.0, 0.0, 0.0, 1.0E-5, 0.0, 0.0, 0.0, 0.0,    0.0, 0.0, 0.0,
      0.0,    0.0, 0.0, 0.0, 0.0,    0.0, 0.0, 0.0, 1.0E-5};
  static const double R[36] = {
      0.001, 0.0, 0.0,   0.0, 0.0,   0.0, 0.0, 0.001, 0.0, 0.0,   0.0, 0.0,
      0.0,   0.0, 0.001, 0.0, 0.0,   0.0, 0.0, 0.0,   0.0, 0.001, 0.0, 0.0,
      0.0,   0.0, 0.0,   0.0, 0.001, 0.0, 0.0, 0.0,   0.0, 0.0,   0.0, 0.001};
  static const double dv2[36] = {
      1.0E-5, 0.0,    0.0, 0.0,    0.0, 0.0,    0.0, 1.0E-5, 0.0,
      0.0,    0.0,    0.0, 0.0,    0.0, 1.0E-5, 0.0, 0.0,    0.0,
      0.0,    0.0,    0.0, 1.0E-5, 0.0, 0.0,    0.0, 0.0,    0.0,
      0.0,    1.0E-5, 0.0, 0.0,    0.0, 0.0,    0.0, 0.0,    1.0E-5};
  creal_T PosH;
  creal_T PrevPosHF;
  double Q[225];
  double Xbar[15];
  double dv[12];
  double Zv[6];
  double BetaHF;
  double DiffTemp;
  double GammHF;
  double HeadingH;
  int b_PositionVector_size[2];
  int c_PositionVector_size[2];
  int d_PositionVector_size[2];
  int e_PositionVector_size[2];
  int b_i;
  int i;
  int i1;
  int i2;
  int i9;
  signed char H[90];
  (void)PositionVector_size;
  if (!isInitialized_PositioningSystem_V2_3) {
    PositioningSystem_V2_3_initialize();
  }
  /*  dT = 0.001; */
  if (PositionVector_data[1] == 5.0) {
    double GyroD[3];
    double acc[3];
    int trueCount;
    boolean_T guard1;
    /*  if dTtemp > 1 */
    /*  else */
    /*      dT = 0.01; */
    /*  end */
    /*  if dTtemp > 10 */
    /*      dT = 0.03; */
    /*  else */
    /*      dT = dTtemp; */
    /*  end */
    for (i = 0; i < 3; i++) {
      i1 = i << 2;
      dv[i1] = IMUacc_c_fifo[i1 + 1];
      dv[i1 + 1] = IMUacc_c_fifo[i1 + 2];
      dv[i1 + 2] = IMUacc_c_fifo[i1 + 3];
      dv[i1 + 3] = PositionVector_data[i + 2];
    }
    memcpy(&IMUacc_c_fifo[0], &dv[0], 12U * sizeof(double));
    for (i = 0; i < 3; i++) {
      i1 = i << 2;
      dv[i1] = IMUgyro_c_fifo[i1 + 1];
      dv[i1 + 1] = IMUgyro_c_fifo[i1 + 2];
      dv[i1 + 2] = IMUgyro_c_fifo[i1 + 3];
      dv[i1 + 3] = PositionVector_data[i + 5];
    }
    memcpy(&IMUgyro_c_fifo[0], &dv[0], 12U * sizeof(double));
    for (b_i = 0; b_i < 3; b_i++) {
      trueCount = b_i << 2;
      GyroD[b_i] =
          (((IMUgyro_c_fifo[trueCount] + IMUgyro_c_fifo[trueCount + 1]) +
            IMUgyro_c_fifo[trueCount + 2]) +
           IMUgyro_c_fifo[trueCount + 3]) /
          4.0 * 3.1415926535897931 / 180.0;
      acc[b_i] = (((IMUacc_c_fifo[trueCount] + IMUacc_c_fifo[trueCount + 1]) +
                   IMUacc_c_fifo[trueCount + 2]) +
                  IMUacc_c_fifo[trueCount + 3]) /
                 4.0;
    }
    acc[2] += 9.85;
    if (prev_state == 1.0) {
      prev_state = 2.0;
      b_PredEKF_3D_Simple(XhatUWB, acc, GyroD, XhatIMU);
    } else {
      memcpy(&Xbar[0], &XhatIMU[0], 15U * sizeof(double));
      b_PredEKF_3D_Simple(Xbar, acc, GyroD, XhatIMU);
    }
    /*  if (0) */
    guard1 = false;
    if ((prevUWBDataSet != 0.0) && (UWBDataSet != 0.0) &&
        ((PositionVector_data[0] - UWBDataSet > 0.02) ||
         (UWBAnc_Full == 1.0))) {
      BetaHF = UWBDataSet - prevUWBDataSet;
      if ((BetaHF < 1.0) &&
          ((PrevPosHUWB.re != 0.0) || (PrevPosHUWB.im != 0.0))) {
        double A[225];
        double b_A[225];
        double K[90];
        double b_H[90];
        double c_H[36];
        signed char y_tmp[90];
        boolean_T x[90];
        boolean_T exitg1;
        boolean_T y;
        PredEKF_3D_Simple(XhatUWB, acc, GyroD, BetaHF, Xbar);
        makePredA_3D_Simple(XhatUWB, acc, GyroD, BetaHF, A);
        Zv[3] = Xbar[0];
        Zv[4] = Xbar[1];
        Zv[5] = Xbar[2];
        Zv[0] = Xbar[9];
        Zv[1] = Xbar[10];
        Zv[2] = Xbar[11];
        memset(&H[0], 0, 90U * sizeof(signed char));
        H[54] = 1;
        H[61] = 1;
        H[68] = 1;
        H[3] = 1;
        H[10] = 1;
        H[17] = 1;
        /*  H(2,7) = -xb(1)*sin(alph)+xb(2)*cos(alph); */
        /*  H(3,7) = -xb(1)*cos(alph)-xb(2)*sin(alph); */
        /*  if ((s_time - s_time_prev) < 1) && (cnt == 0) */
        memcpy(&Q[0], &dv1[0], 225U * sizeof(double));
        for (i = 0; i < 6; i++) {
          for (i1 = 0; i1 < 6; i1++) {
            Q[i1 + 15 * i] = dv2[i1 + 6 * i];
          }
        }
        /*  Q = Q * 100; */
        /*  R = eye(6)*5e-3; */
        /*  R = eye(6)*1e-2; */
        /*  R = eye(6)*1e-1; */
        /*  elseif (s_time - s_time_prev) >= 1 */
        /*      cnt = 1; */
        /*      Q = eye(15)*1e-5; */
        /*      Q(1:6,1:6) = eye(6)*1e-5; */
        /*   */
        /*      % Q = Q * 0.1; */
        /*      Q = Q * 10; */
        /*   */
        /*      R = eye(6)*1e-3; */
        /*      % R = eye(6)*1e-2; */
        /*      % R = eye(6)*1e-1; */
        /*  else */
        /*      if cnt>3 */
        /*          cnt = 0; */
        /*      else */
        /*          cnt = cnt + 1; */
        /*      end */
        /*      Q = eye(15)* 1e-5; */
        /*      Q(1:6,1:6) = eye(6)*1e-5; */
        /*   */
        /*      % Q = Q * 0.1; */
        /*      Q = Q * 10; */
        /*   */
        /*      R = eye(6)*1e-3; */
        /*      % R = eye(6)*1e-2; */
        /*      % R = eye(6)*1e-1; */
        /*  end     */
        for (i = 0; i < 15; i++) {
          for (i1 = 0; i1 < 15; i1++) {
            BetaHF = 0.0;
            for (i2 = 0; i2 < 15; i2++) {
              BetaHF += A[i + 15 * i2] * P[i2 + 15 * i1];
            }
            b_A[i + 15 * i1] = BetaHF;
          }
        }
        for (i = 0; i < 15; i++) {
          for (i1 = 0; i1 < 15; i1++) {
            BetaHF = 0.0;
            for (i2 = 0; i2 < 15; i2++) {
              BetaHF += b_A[i + 15 * i2] * A[i1 + 15 * i2];
            }
            trueCount = i + 15 * i1;
            P[trueCount] = BetaHF + Q[trueCount];
          }
        }
        for (i = 0; i < 6; i++) {
          for (i1 = 0; i1 < 15; i1++) {
            y_tmp[i1 + 15 * i] = H[i + 6 * i1];
          }
        }
        for (i = 0; i < 15; i++) {
          for (i1 = 0; i1 < 6; i1++) {
            BetaHF = 0.0;
            for (i2 = 0; i2 < 15; i2++) {
              BetaHF += P[i + 15 * i2] * (double)y_tmp[i2 + 15 * i1];
            }
            K[i + 15 * i1] = BetaHF;
          }
        }
        for (i = 0; i < 6; i++) {
          for (i1 = 0; i1 < 15; i1++) {
            BetaHF = 0.0;
            for (i2 = 0; i2 < 15; i2++) {
              BetaHF += (double)H[i + 6 * i2] * P[i2 + 15 * i1];
            }
            b_H[i + 6 * i1] = BetaHF;
          }
          for (i1 = 0; i1 < 6; i1++) {
            BetaHF = 0.0;
            for (i2 = 0; i2 < 15; i2++) {
              BetaHF += b_H[i + 6 * i2] * (double)y_tmp[i2 + 15 * i1];
            }
            trueCount = i + 6 * i1;
            c_H[trueCount] = BetaHF + R[trueCount];
          }
        }
        mrdiv(K, c_H);
        for (i = 0; i < 90; i++) {
          x[i] = rtIsNaN(K[i]);
        }
        y = true;
        trueCount = 0;
        exitg1 = false;
        while ((!exitg1) && (trueCount < 90)) {
          if (!x[trueCount]) {
            y = false;
            exitg1 = true;
          } else {
            trueCount++;
          }
        }
        if (y) {
          PrevPosHF.re = XhatIMU[0];
          PrevPosHF.im = XhatIMU[1];
          DiffTemp = XhatIMU[9];
          /*  PrevHeadingIMU = HeadingHF; */
          GammHF = XhatIMU[11];
          BetaHF = XhatIMU[10];
        } else {
          double dv4[6];
          DiffTemp = acc[1] - Xbar[7];
          GammHF = (acc[2] - 9.85) - Xbar[8];
          BetaHF = atan((acc[0] - Xbar[6]) /
                        sqrt(DiffTemp * DiffTemp + GammHF * GammHF));
          AngleMatching(Xbar[10], &BetaHF);
          DiffTemp = atan(DiffTemp / GammHF);
          AngleMatching(Xbar[11], &DiffTemp);
          dv4[0] = -PrevHeadingHUWB;
          dv4[1] = BetaHF;
          dv4[2] = DiffTemp;
          dv4[3] = PrevPosHUWB.re;
          dv4[4] = PrevPosHUWB.im;
          dv4[5] = 0.0;
          for (i = 0; i < 6; i++) {
            dv4[i] -= Zv[i];
          }
          for (i = 0; i < 15; i++) {
            BetaHF = 0.0;
            for (i1 = 0; i1 < 6; i1++) {
              BetaHF += K[i + 15 * i1] * dv4[i1];
            }
            XhatUWB[i] = Xbar[i] + BetaHF;
            for (i1 = 0; i1 < 15; i1++) {
              BetaHF = 0.0;
              for (i2 = 0; i2 < 6; i2++) {
                BetaHF += K[i + 15 * i2] * (double)H[i2 + 6 * i1];
              }
              A[i + 15 * i1] = BetaHF;
            }
            for (i1 = 0; i1 < 15; i1++) {
              BetaHF = 0.0;
              for (i2 = 0; i2 < 15; i2++) {
                BetaHF += A[i + 15 * i2] * P[i2 + 15 * i1];
              }
              trueCount = i + 15 * i1;
              Q[trueCount] = P[trueCount] - BetaHF;
            }
          }
          memcpy(&P[0], &Q[0], 225U * sizeof(double));
          prevUWBDataSet = UWBDataSet;
          prev_state = 1.0;
          PrevPosHF.re = XhatUWB[0];
          PrevPosHF.im = XhatUWB[1];
          DiffTemp = XhatUWB[9];
          /*  PrevHeadingIMU = HeadingHF; */
          GammHF = XhatUWB[11];
          BetaHF = XhatUWB[10];
        }
      } else {
        guard1 = true;
      }
    } else {
      guard1 = true;
    }
    if (guard1) {
      if ((UWBDataSet != 0.0) &&
          ((PositionVector_data[0] - UWBDataSet > 0.02) ||
           (UWBAnc_Full == 1.0))) {
        prevUWBDataSet = UWBDataSet;
        XhatIMU[0] = PrevPosHUWB.re;
        XhatIMU[1] = PrevPosHUWB.im;
        XhatIMU[9] = -PrevHeadingHUWB;
        memcpy(&XhatUWB[0], &XhatIMU[0], 15U * sizeof(double));
        prev_state = 1.0;
        PrevPosHF.re = XhatUWB[0];
        PrevPosHF.im = XhatUWB[1];
        DiffTemp = XhatUWB[9];
        /*  PrevHeadingIMU = HeadingHF; */
        GammHF = XhatUWB[11];
        BetaHF = XhatUWB[10];
      } else {
        PrevPosHF.re = XhatIMU[0];
        PrevPosHF.im = XhatIMU[1];
        DiffTemp = XhatIMU[9];
        /*  PrevHeadingIMU = HeadingHF; */
        GammHF = XhatIMU[11];
        BetaHF = XhatIMU[10];
      }
    }
    /*  PosH = Xbar(1)+Xbar(2)*j; */
    /*  HeadingH = Xbar(7); */
    /*  PosH = 0; */
    /*  HeadingH = PrevHeadingHUWB; */
    PrevHeadingHUWB = -DiffTemp;
    UWBAnc_Full = 0.0;
    /*  PrevPosHUWB = PosHF; */
    PrevHeadingIMU = DiffTemp;
    PosH.re = 0.0;
    PosH.im = 0.0;
    HeadingH = 0.0;
  } else if (PositionVector_data[1] == 6.0) {
    /*  GPS */
    PrevPosHF.re = 0.0;
    PrevPosHF.im = 0.0;
    DiffTemp = 0.0;
    GammHF = 0.0;
    BetaHF = 0.0;
    PosH.re = 0.0;
    PosH.im = 0.0;
    HeadingH = 0.0;
  } else if (PositionVector_data[1] == 7.0) {
    /*  CCTV */
    PrevPosHF.re = 0.0;
    PrevPosHF.im = 0.0;
    DiffTemp = 0.0;
    GammHF = 0.0;
    BetaHF = 0.0;
    PosH.re = 0.0;
    PosH.im = 0.0;
    HeadingH = 0.0;
  } else {
    double b_PositionVector_data[50];
    double c_PositionVector_data[50];
    double d_PositionVector_data[50];
    double e_PositionVector_data[50];
    double f_PositionVector_data[50];
    int i3;
    int i4;
    int i5;
    int i6;
    int i7;
    int i8;
    int loop_ub;
    int trueCount;
    s_time_vec[0] = s_time_vec[1];
    s_time_vec[1] = s_time_vec[2];
    s_time_vec[2] = s_time_vec[3];
    s_time_vec[3] = PositionVector_data[0];
    BetaHF = (PositionVector_data[4] + 6.0) - 1.0;
    if (BetaHF < 6.0) {
      i = 0;
      i1 = 0;
    } else {
      i = 5;
      i1 = (int)BetaHF;
    }
    BetaHF = ((PositionVector_data[2] + 6.0) + PositionVector_data[4]) - 1.0;
    if (PositionVector_data[2] + 6.0 > BetaHF) {
      i2 = 0;
      trueCount = 0;
    } else {
      i2 = (int)(PositionVector_data[2] + 6.0) - 1;
      trueCount = (int)BetaHF;
    }
    BetaHF = (PositionVector_data[2] + 6.0) + 4.0;
    DiffTemp = BetaHF + PositionVector_data[3];
    if (BetaHF > DiffTemp - 1.0) {
      b_i = 0;
      i3 = 0;
    } else {
      b_i = (int)BetaHF - 1;
      i3 = (int)(DiffTemp - 1.0);
    }
    BetaHF = DiffTemp + PositionVector_data[3];
    if (DiffTemp > BetaHF - 1.0) {
      i4 = 0;
      i5 = 0;
    } else {
      i4 = (int)DiffTemp - 1;
      i5 = (int)(BetaHF - 1.0);
    }
    DiffTemp = BetaHF + PositionVector_data[3];
    if (BetaHF > DiffTemp - 1.0) {
      i6 = 0;
      i7 = 0;
    } else {
      i6 = (int)BetaHF - 1;
      i7 = (int)(DiffTemp - 1.0);
    }
    if (DiffTemp > (DiffTemp + 4.0) - 1.0) {
      i8 = 0;
      i9 = -1;
    } else {
      i8 = (int)DiffTemp - 1;
      i9 = (int)DiffTemp + 2;
    }
    xt_b.size[0] = 1;
    loop_ub = i9 - i8;
    xt_b.size[1] = loop_ub + 1;
    for (i9 = 0; i9 <= loop_ub; i9++) {
      xt_b.data[i9] = PositionVector_data[i8 + i9];
    }
    if (DiffTemp + 4.0 > ((DiffTemp + 4.0) + 4.0) - 1.0) {
      i8 = 0;
      i9 = -1;
    } else {
      i8 = (int)(DiffTemp + 4.0) - 1;
      i9 = (int)(DiffTemp + 4.0) + 2;
    }
    yt_b.size[0] = 1;
    loop_ub = i9 - i8;
    yt_b.size[1] = loop_ub + 1;
    for (i9 = 0; i9 <= loop_ub; i9++) {
      yt_b.data[i9] = PositionVector_data[i8 + i9];
    }
    /*  [PosH, Heading, UWBAnc_Full] = UWBPosition_V3(s_time, Ln, LnC, Nanchor,
     * SensorNum, RxIDUWB, RxDistOrig, xain, yain, zain, xt_b, yt_b, zt_b); */
    b_PositionVector_size[0] = 1;
    loop_ub = i1 - i;
    b_PositionVector_size[1] = loop_ub;
    for (i1 = 0; i1 < loop_ub; i1++) {
      b_PositionVector_data[i1] = PositionVector_data[i + i1];
    }
    c_PositionVector_size[0] = 1;
    loop_ub = trueCount - i2;
    c_PositionVector_size[1] = loop_ub;
    for (i = 0; i < loop_ub; i++) {
      c_PositionVector_data[i] = PositionVector_data[i2 + i];
    }
    d_PositionVector_size[0] = 1;
    loop_ub = i3 - b_i;
    d_PositionVector_size[1] = loop_ub;
    for (i = 0; i < loop_ub; i++) {
      d_PositionVector_data[i] = PositionVector_data[b_i + i];
    }
    e_PositionVector_size[0] = 1;
    loop_ub = i5 - i4;
    e_PositionVector_size[1] = loop_ub;
    for (i = 0; i < loop_ub; i++) {
      e_PositionVector_data[i] = PositionVector_data[i4 + i];
    }
    loop_ub = i7 - i6;
    for (i = 0; i < loop_ub; i++) {
      f_PositionVector_data[i] = PositionVector_data[i6 + i];
    }
    GammHF = UWBPosition_V3_1(
        PositionVector_data[0], PositionVector_data[2], PositionVector_data[4],
        PositionVector_data[1], b_PositionVector_data, b_PositionVector_size,
        c_PositionVector_data, c_PositionVector_size, d_PositionVector_data,
        d_PositionVector_size, e_PositionVector_data, e_PositionVector_size,
        f_PositionVector_data, xt_b.data, xt_b.size, yt_b.data, yt_b.size,
        PositionVector_data[(int)((DiffTemp + 4.0) + 4.0) - 1], &PosH,
        &UWBAnc_Full);
    DiffTemp = -PrevHeadingIMU - GammHF;
    if (rtIsNaN(DiffTemp)) {
      BetaHF = rtNaN;
    } else if (DiffTemp < 0.0) {
      BetaHF = -1.0;
    } else {
      BetaHF = (DiffTemp > 0.0);
    }
    HeadingH = GammHF + BetaHF * floor(fabs(DiffTemp) / 6.2831853071795862) *
                            2.0 * 3.1415926535897931;
    BetaHF = HeadingH - (-PrevHeadingIMU);
    if (BetaHF > 3.1415926535897931) {
      HeadingH -= 6.2831853071795862;
    } else if (BetaHF < -3.1415926535897931) {
      HeadingH += 6.2831853071795862;
    }
    if ((PosH.re != 0.0) || (PosH.im != 0.0)) {
      for (i = 0; i < 9; i++) {
        PosH_vec[i] = PosH_vec[i + 1];
        HeadingH_vec[i] = HeadingH_vec[i + 1];
        PosH_vec_stime[i] = PosH_vec_stime[i + 1];
      }
      PosH_vec[9] = PosH;
      HeadingH_vec[9] = HeadingH;
      PosH_vec_stime[9] = PositionVector_data[0];
      trueCount = 0;
      for (b_i = 0; b_i < 10; b_i++) {
        if ((PosH_vec[b_i].re == 0.0) && (PosH_vec[b_i].im == 0.0)) {
          trueCount++;
        }
      }
      if (trueCount == 0) {
        double dv3[10];
        double GyroD[3];
        for (i = 0; i < 10; i++) {
          dv3[i] = PosH_vec[i].re;
        }
        polyfit(PosH_vec_stime, dv3, GyroD);
        DiffTemp = PositionVector_data[0] *
                       (PositionVector_data[0] * GyroD[0] + GyroD[1]) +
                   GyroD[2];
        for (i = 0; i < 10; i++) {
          dv3[i] = PosH_vec[i].im;
        }
        polyfit(PosH_vec_stime, dv3, GyroD);
        PosH.re = DiffTemp;
        PosH.im = PositionVector_data[0] *
                      (PositionVector_data[0] * GyroD[0] + GyroD[1]) +
                  GyroD[2];
        polyfit(PosH_vec_stime, HeadingH_vec, GyroD);
        HeadingH = PositionVector_data[0] *
                       (PositionVector_data[0] * GyroD[0] + GyroD[1]) +
                   GyroD[2];
      }
    } else {
      PosH.re = 0.0;
      PosH.im = 0.0;
      HeadingH = 0.0;
    }
    PrevHeadingHUWB = HeadingH;
    if ((PosH.re != 0.0) || (PosH.im != 0.0)) {
      UWBDataSet = PositionVector_data[0];
      PrevPosHUWB = PosH;
      /*  PrevHeadingHUWB = HeadingH; */
    } else {
      UWBDataSet = 0.0;
      /*  PrevPosHUWB = PrevPosIMU; */
      /*  PrevHeadingHUWB = PrevHeadingIMU; */
    }
    /*  PosHF = PrevPosHUWB; */
    /*  HeadingHF = -PrevHeadingHUWB; */
    PrevPosHF.re = 0.0;
    PrevPosHF.im = 0.0;
    DiffTemp = 0.0;
    GammHF = 0.0;
    BetaHF = 0.0;
    /*   */
  }
  PositionOut[0] = PrevPosHF.re;
  PositionOut[1] = PrevPosHF.im;
  PositionOut[2] = 0.0;
  PositionOut[3] = DiffTemp;
  PositionOut[4] = GammHF;
  PositionOut[5] = BetaHF;
  PositionOut[6] = P[0];
  PositionOut[7] = PosH.re;
  PositionOut[8] = PosH.im;
  PositionOut[9] = HeadingH;
}

void PositioningSystem_V2_3_init(void)
{
  int k;
  yt_b.size[1] = 0;
  xt_b.size[1] = 0;
  memset(&IMUacc_c_fifo[0], 0, 12U * sizeof(double));
  memset(&IMUgyro_c_fifo[0], 0, 12U * sizeof(double));
  memset(&XhatUWB[0], 0, 15U * sizeof(double));
  memset(&XhatIMU[0], 0, 15U * sizeof(double));
  prev_state = 0.0;
  /*  1 : UWB, 2 : IMU */
  PrevPosHUWB.re = 0.0;
  PrevPosHUWB.im = 0.0;
  PrevHeadingHUWB = 0.0;
  s_time_vec[0] = 0.0;
  s_time_vec[1] = 0.0;
  s_time_vec[2] = 0.0;
  s_time_vec[3] = 0.0;
  UWBDataSet = 0.0;
  prevUWBDataSet = 0.0;
  /*  xt_b = [-0.525 0.525 -0.525 0.525]; */
  /*  yt_b = [0.505 0.505 -0.505 -0.505]; */
  memset(&P[0], 0, 225U * sizeof(double));
  for (k = 0; k < 15; k++) {
    P[k + 15 * k] = 1.0;
  }
  UWBAnc_Full = 0.0;
  PrevHeadingIMU = 0.0;
  memset(&PosH_vec[0], 0, 10U * sizeof(creal_T));
  memset(&PosH_vec_stime[0], 0, 10U * sizeof(double));
  memset(&HeadingH_vec[0], 0, 10U * sizeof(double));
}

/* End of code generation (PositioningSystem_V2_3.c) */
