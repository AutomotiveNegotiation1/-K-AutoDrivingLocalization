/*
 * PositioningSystem_V5_1.c
 *
 * Code generation for function 'PositioningSystem_V5_1'
 *
 */

/* Include files */
#include "PositioningSystem_V5_1.h"
#include "EKF_UWB_SLAM_4.h"
#include "PositioningSystem_V5_1_data.h"
#include "PositioningSystem_V5_1_initialize.h"
#include "PositioningSystem_V5_1_rtwutil.h"
#include "UWBPosition_V4_1.h"
#include "equPlane.h"
#include "exp.h"
#include "interp1.h"
#include "mod.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Type Definitions */
#ifndef struct_emxArray_real_T_1x92
#define struct_emxArray_real_T_1x92
struct emxArray_real_T_1x92 {
  double data[92];
  int size[2];
};
#endif /* struct_emxArray_real_T_1x92 */
#ifndef typedef_emxArray_real_T_1x92
#define typedef_emxArray_real_T_1x92
typedef struct emxArray_real_T_1x92 emxArray_real_T_1x92;
#endif /* typedef_emxArray_real_T_1x92 */

/* Variable Definitions */
static double s_time_vec[4];

static emxArray_real_T_1x92 xt_b;

static emxArray_real_T_1x92 yt_b;

static double P[225];

static creal_T FiFoUWBpos[100];

static double FiFoUWBhead[100];

static double FiFoUWBtime[100];

static double FiFoSLAMpos[300];

static double FiFoSLAMhead[400];

static double FiFoSLAMtime[100];

static double FiFoSLAMEulDiff[300];

static double firstV;

static double SLAMSet;

static double FiFoIMUacc[12];

static double FiFoIMUgyro[12];

static creal_T FiFoSLAMUWBpos[100];

static double FiFoSLAMUWBhead[100];

static double FiFoSLAMUWBtime[100];

static creal_T CAMPos;

static double diff_Heading[3];

/* Function Definitions */
void PositioningSystem_V5_1(const double PositionVector_data[],
                            const int PositionVector_size[2],
                            const double MapParam[16], double PositionOut[10])
{
  creal_T EKFpos;
  creal_T PosH;
  creal_T PrevPosHF;
  creal_T b_prev_pos;
  creal_T dc;
  double b_tmp_data[100];
  double tmp_data[100];
  double b_PositionVector_data[92];
  double c_PositionVector_data[92];
  double d_PositionVector_data[92];
  double e_PositionVector_data[92];
  double f_PositionVector_data[92];
  double sA[9];
  double SLAMorient[4];
  double EKFhead[3];
  double RotEul_diff[3];
  double SLAMpos[3];
  double SLAMposInit[3];
  double curr_pos[3];
  double prev_pos[3];
  double x_tmp[3];
  double HeadingH;
  double HeadingHF;
  double UWBAnc_Full;
  double d;
  double im;
  int b_PositionVector_size[2];
  int c_PositionVector_size[2];
  int d_PositionVector_size[2];
  int e_PositionVector_size[2];
  int FiFoSLAMhead_tmp;
  int FiFoSLAMpos_tmp;
  int i;
  int i1;
  int i2;
  int i3;
  int i4;
  int i5;
  int i6;
  int i7;
  int i8;
  int loop_ub;
  int trueCount;
  boolean_T PosH_tmp;
  boolean_T exitg1;
  boolean_T guard1;
  (void)PositionVector_size;
  if (!isInitialized_PositioningSystem_V5_1) {
    PositioningSystem_V5_1_initialize();
  }
  /*  change (08.21. by joo) Extract the MapParam  */
  sA[0] = MapParam[4];
  sA[3] = MapParam[5];
  sA[6] = MapParam[6];
  sA[1] = MapParam[7];
  sA[4] = MapParam[8];
  sA[7] = MapParam[9];
  sA[2] = MapParam[10];
  sA[5] = MapParam[11];
  sA[8] = MapParam[12];
  SLAMposInit[0] = MapParam[13];
  SLAMposInit[1] = MapParam[14];
  SLAMposInit[2] = MapParam[15];
  /* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
  if (PositionVector_data[1] == 5.0) {
    PrevPosHF.re = 0.0;
    PrevPosHF.im = 0.0;
    HeadingHF = 0.0;
    PosH.re = 0.0;
    PosH.im = 0.0;
    HeadingH = 0.0;
    if (PositionVector_data[2] != 0.0) {
      for (i = 0; i < 3; i++) {
        FiFoSLAMpos_tmp = 3 * (i + 1);
        FiFoIMUacc[3 * i] = FiFoIMUacc[FiFoSLAMpos_tmp];
        FiFoIMUacc[3 * i + 1] = FiFoIMUacc[FiFoSLAMpos_tmp + 1];
        FiFoIMUacc[3 * i + 2] = FiFoIMUacc[FiFoSLAMpos_tmp + 2];
      }
      for (i = 0; i < 3; i++) {
        FiFoIMUacc[i + 9] = PositionVector_data[i + 2];
        FiFoSLAMpos_tmp = 3 * (i + 1);
        FiFoIMUgyro[3 * i] = FiFoIMUgyro[FiFoSLAMpos_tmp];
        FiFoIMUgyro[3 * i + 1] = FiFoIMUgyro[FiFoSLAMpos_tmp + 1];
        FiFoIMUgyro[3 * i + 2] = FiFoIMUgyro[FiFoSLAMpos_tmp + 2];
      }
      FiFoIMUgyro[9] = PositionVector_data[5];
      FiFoIMUgyro[10] = PositionVector_data[6];
      FiFoIMUgyro[11] = PositionVector_data[7];
      /* [PosHF5, HeadingHF5, GammHF5, BetaHF5] = EKF_UWB_SLAM_IMU_1(dt, acc,
       * GyroD, IMUtime, FiFoSLAMUWBpos(end), FiFoSLAMUWBhead(end),
       * FiFoSLAMUWBtime(end), grav); */
    }
  } else if (PositionVector_data[1] == 6.0) {
    /*  GPS */
    PrevPosHF.re = 0.0;
    PrevPosHF.im = 0.0;
    HeadingHF = 0.0;
    PosH.re = 0.0;
    PosH.im = 0.0;
    HeadingH = 0.0;
  } else if (PositionVector_data[1] == 7.0) {
    /*  SLAM */
    PrevPosHF.re = 0.0;
    PrevPosHF.im = 0.0;
    HeadingHF = 0.0;
    PosH.re = 0.0;
    PosH.im = 0.0;
    HeadingH = 0.0;
    SLAMpos[0] = PositionVector_data[2];
    SLAMpos[1] = PositionVector_data[3];
    SLAMpos[2] = PositionVector_data[4];
    SLAMorient[0] = PositionVector_data[5];
    SLAMorient[1] = PositionVector_data[6];
    SLAMorient[2] = PositionVector_data[7];
    SLAMorient[3] = PositionVector_data[8];
    if ((PositionVector_data[2] != 0.0) &&
        (FiFoSLAMtime[99] - PositionVector_data[0] < -0.001)) {
      for (i = 0; i < 99; i++) {
        FiFoSLAMpos_tmp = 3 * (i + 1);
        FiFoSLAMpos[3 * i] = FiFoSLAMpos[FiFoSLAMpos_tmp];
        FiFoSLAMpos[3 * i + 1] = FiFoSLAMpos[FiFoSLAMpos_tmp + 1];
        FiFoSLAMpos[3 * i + 2] = FiFoSLAMpos[FiFoSLAMpos_tmp + 2];
        FiFoSLAMpos_tmp = (i + 1) << 2;
        FiFoSLAMhead_tmp = i << 2;
        FiFoSLAMhead[FiFoSLAMhead_tmp] = FiFoSLAMhead[FiFoSLAMpos_tmp];
        FiFoSLAMhead[FiFoSLAMhead_tmp + 1] = FiFoSLAMhead[FiFoSLAMpos_tmp + 1];
        FiFoSLAMhead[FiFoSLAMhead_tmp + 2] = FiFoSLAMhead[FiFoSLAMpos_tmp + 2];
        FiFoSLAMhead[FiFoSLAMhead_tmp + 3] = FiFoSLAMhead[FiFoSLAMpos_tmp + 3];
        FiFoSLAMtime[i] = FiFoSLAMtime[i + 1];
      }
      FiFoSLAMpos[297] = SLAMpos[0];
      FiFoSLAMpos[298] = SLAMpos[1];
      FiFoSLAMpos[299] = SLAMpos[2];
      FiFoSLAMhead[396] = SLAMorient[0];
      FiFoSLAMhead[397] = SLAMorient[1];
      FiFoSLAMhead[398] = SLAMorient[2];
      FiFoSLAMhead[399] = SLAMorient[3];
      FiFoSLAMtime[99] = PositionVector_data[0];
      trueCount = 0;
      for (FiFoSLAMpos_tmp = 0; FiFoSLAMpos_tmp < 100; FiFoSLAMpos_tmp++) {
        if (FiFoSLAMtime[FiFoSLAMpos_tmp] != 0.0) {
          trueCount++;
        }
      }
      guard1 = false;
      if (FiFoSLAMtime[100 - trueCount] < PositionVector_data[0] - 0.05) {
        trueCount = 0;
        for (FiFoSLAMpos_tmp = 0; FiFoSLAMpos_tmp < 100; FiFoSLAMpos_tmp++) {
          if (FiFoSLAMtime[FiFoSLAMpos_tmp] != 0.0) {
            trueCount++;
          }
        }
        if (trueCount > 2) {
          trueCount = 0;
          for (FiFoSLAMpos_tmp = 0; FiFoSLAMpos_tmp < 100; FiFoSLAMpos_tmp++) {
            if (FiFoSLAMtime[FiFoSLAMpos_tmp] != 0.0) {
              trueCount++;
            }
          }
          b_PositionVector_size[0] = 1;
          b_PositionVector_size[1] = trueCount;
          FiFoSLAMpos_tmp = trueCount - 100;
          c_PositionVector_size[0] = 1;
          c_PositionVector_size[1] = trueCount;
          for (i = 0; i <= FiFoSLAMpos_tmp + 99; i++) {
            FiFoSLAMhead_tmp = (i - trueCount) + 100;
            tmp_data[i] = FiFoSLAMtime[FiFoSLAMhead_tmp];
            b_tmp_data[i] = FiFoSLAMpos[3 * FiFoSLAMhead_tmp];
          }
          curr_pos[0] = interp1(tmp_data, b_PositionVector_size, b_tmp_data,
                                c_PositionVector_size, FiFoSLAMtime[99] - 0.05);
          trueCount = 0;
          for (FiFoSLAMpos_tmp = 0; FiFoSLAMpos_tmp < 100; FiFoSLAMpos_tmp++) {
            if (FiFoSLAMtime[FiFoSLAMpos_tmp] != 0.0) {
              trueCount++;
            }
          }
          b_PositionVector_size[0] = 1;
          b_PositionVector_size[1] = trueCount;
          FiFoSLAMpos_tmp = trueCount - 100;
          c_PositionVector_size[0] = 1;
          c_PositionVector_size[1] = trueCount;
          for (i = 0; i <= FiFoSLAMpos_tmp + 99; i++) {
            FiFoSLAMhead_tmp = (i - trueCount) + 100;
            tmp_data[i] = FiFoSLAMtime[FiFoSLAMhead_tmp];
            b_tmp_data[i] = FiFoSLAMpos[3 * FiFoSLAMhead_tmp + 1];
          }
          curr_pos[1] = interp1(tmp_data, b_PositionVector_size, b_tmp_data,
                                c_PositionVector_size, FiFoSLAMtime[99] - 0.05);
          trueCount = 0;
          for (FiFoSLAMpos_tmp = 0; FiFoSLAMpos_tmp < 100; FiFoSLAMpos_tmp++) {
            if (FiFoSLAMtime[FiFoSLAMpos_tmp] != 0.0) {
              trueCount++;
            }
          }
          b_PositionVector_size[0] = 1;
          b_PositionVector_size[1] = trueCount;
          FiFoSLAMpos_tmp = trueCount - 100;
          c_PositionVector_size[0] = 1;
          c_PositionVector_size[1] = trueCount;
          for (i = 0; i <= FiFoSLAMpos_tmp + 99; i++) {
            FiFoSLAMhead_tmp = (i - trueCount) + 100;
            tmp_data[i] = FiFoSLAMtime[FiFoSLAMhead_tmp];
            b_tmp_data[i] = FiFoSLAMpos[3 * FiFoSLAMhead_tmp + 2];
          }
          curr_pos[2] = interp1(tmp_data, b_PositionVector_size, b_tmp_data,
                                c_PositionVector_size, FiFoSLAMtime[99] - 0.05);
          trueCount = 0;
          for (FiFoSLAMpos_tmp = 0; FiFoSLAMpos_tmp < 100; FiFoSLAMpos_tmp++) {
            if (FiFoSLAMtime[FiFoSLAMpos_tmp] != 0.0) {
              trueCount++;
            }
          }
          if (FiFoSLAMtime[100 - trueCount] < FiFoSLAMtime[98] - 0.05) {
            trueCount = 0;
            for (FiFoSLAMpos_tmp = 0; FiFoSLAMpos_tmp < 100;
                 FiFoSLAMpos_tmp++) {
              if (FiFoSLAMtime[FiFoSLAMpos_tmp] != 0.0) {
                trueCount++;
              }
            }
            b_PositionVector_size[0] = 1;
            b_PositionVector_size[1] = trueCount;
            FiFoSLAMpos_tmp = trueCount - 100;
            c_PositionVector_size[0] = 1;
            c_PositionVector_size[1] = trueCount;
            for (i = 0; i <= FiFoSLAMpos_tmp + 99; i++) {
              FiFoSLAMhead_tmp = (i - trueCount) + 100;
              tmp_data[i] = FiFoSLAMtime[FiFoSLAMhead_tmp];
              b_tmp_data[i] = FiFoSLAMpos[3 * FiFoSLAMhead_tmp];
            }
            prev_pos[0] =
                interp1(tmp_data, b_PositionVector_size, b_tmp_data,
                        c_PositionVector_size, FiFoSLAMtime[98] - 0.05);
            trueCount = 0;
            for (FiFoSLAMpos_tmp = 0; FiFoSLAMpos_tmp < 100;
                 FiFoSLAMpos_tmp++) {
              if (FiFoSLAMtime[FiFoSLAMpos_tmp] != 0.0) {
                trueCount++;
              }
            }
            b_PositionVector_size[0] = 1;
            b_PositionVector_size[1] = trueCount;
            FiFoSLAMpos_tmp = trueCount - 100;
            c_PositionVector_size[0] = 1;
            c_PositionVector_size[1] = trueCount;
            for (i = 0; i <= FiFoSLAMpos_tmp + 99; i++) {
              FiFoSLAMhead_tmp = (i - trueCount) + 100;
              tmp_data[i] = FiFoSLAMtime[FiFoSLAMhead_tmp];
              b_tmp_data[i] = FiFoSLAMpos[3 * FiFoSLAMhead_tmp + 1];
            }
            prev_pos[1] =
                interp1(tmp_data, b_PositionVector_size, b_tmp_data,
                        c_PositionVector_size, FiFoSLAMtime[98] - 0.05);
            trueCount = 0;
            for (FiFoSLAMpos_tmp = 0; FiFoSLAMpos_tmp < 100;
                 FiFoSLAMpos_tmp++) {
              if (FiFoSLAMtime[FiFoSLAMpos_tmp] != 0.0) {
                trueCount++;
              }
            }
            b_PositionVector_size[0] = 1;
            b_PositionVector_size[1] = trueCount;
            FiFoSLAMpos_tmp = trueCount - 100;
            c_PositionVector_size[0] = 1;
            c_PositionVector_size[1] = trueCount;
            for (i = 0; i <= FiFoSLAMpos_tmp + 99; i++) {
              FiFoSLAMhead_tmp = (i - trueCount) + 100;
              tmp_data[i] = FiFoSLAMtime[FiFoSLAMhead_tmp];
              b_tmp_data[i] = FiFoSLAMpos[3 * FiFoSLAMhead_tmp + 2];
            }
            prev_pos[2] =
                interp1(tmp_data, b_PositionVector_size, b_tmp_data,
                        c_PositionVector_size, FiFoSLAMtime[98] - 0.05);
          } else {
            prev_pos[0] = curr_pos[0];
            prev_pos[1] = curr_pos[1];
            prev_pos[2] = curr_pos[2];
          }
        } else {
          guard1 = true;
        }
      } else {
        guard1 = true;
      }
      if (guard1) {
        curr_pos[0] = FiFoSLAMpos[297];
        prev_pos[0] = FiFoSLAMpos[297];
        curr_pos[1] = FiFoSLAMpos[298];
        prev_pos[1] = FiFoSLAMpos[298];
        curr_pos[2] = FiFoSLAMpos[299];
        prev_pos[2] = FiFoSLAMpos[299];
      }
      if (PositionVector_data[2] != 0.0) {
        if (firstV == 0.0) {
          if ((FiFoUWBpos[99].re != 0.0) || (FiFoUWBpos[99].im != 0.0)) {
            firstV = 1.0;
          }
        } else {
          curr_pos[0] -= prev_pos[0];
          curr_pos[1] -= prev_pos[1];
          curr_pos[2] -= prev_pos[2];
          equPlane(SLAMorient, x_tmp);
          equPlane(&FiFoSLAMhead[392], prev_pos);
          UWBAnc_Full = b_mod((x_tmp[0] - prev_pos[0]) + 3.1415926535897931) -
                        3.1415926535897931;
          RotEul_diff[0] = UWBAnc_Full;
          prev_pos[0] = fabs(UWBAnc_Full);
          UWBAnc_Full = b_mod((x_tmp[1] - prev_pos[1]) + 3.1415926535897931) -
                        3.1415926535897931;
          RotEul_diff[1] = UWBAnc_Full;
          prev_pos[1] = fabs(UWBAnc_Full);
          UWBAnc_Full = b_mod((x_tmp[2] - prev_pos[2]) + 3.1415926535897931) -
                        3.1415926535897931;
          RotEul_diff[2] = UWBAnc_Full;
          prev_pos[2] = fabs(UWBAnc_Full);
          PosH_tmp = true;
          FiFoSLAMpos_tmp = 0;
          exitg1 = false;
          while ((!exitg1) && (FiFoSLAMpos_tmp < 3)) {
            if (!(prev_pos[FiFoSLAMpos_tmp] > 0.39269908169872414)) {
              PosH_tmp = false;
              exitg1 = true;
            } else {
              FiFoSLAMpos_tmp++;
            }
          }
          if (PosH_tmp) {
            PosH_tmp = true;
            FiFoSLAMpos_tmp = 0;
            exitg1 = false;
            while ((!exitg1) && (FiFoSLAMpos_tmp < 3)) {
              if (!(RotEul_diff[FiFoSLAMpos_tmp] > 0.0)) {
                PosH_tmp = false;
                exitg1 = true;
              } else {
                FiFoSLAMpos_tmp++;
              }
            }
            if (PosH_tmp) {
              RotEul_diff[0] -= 6.2831853071795862;
              RotEul_diff[1] -= 6.2831853071795862;
              RotEul_diff[2] = UWBAnc_Full - 6.2831853071795862;
            } else {
              RotEul_diff[0] += 6.2831853071795862;
              RotEul_diff[1] += 6.2831853071795862;
              RotEul_diff[2] = UWBAnc_Full + 6.2831853071795862;
            }
          }
          UWBAnc_Full = fabs(curr_pos[0]);
          EKFhead[0] = UWBAnc_Full * UWBAnc_Full;
          UWBAnc_Full = fabs(curr_pos[1]);
          EKFhead[1] = UWBAnc_Full * UWBAnc_Full;
          UWBAnc_Full = fabs(curr_pos[2]);
          if (!(sqrt((EKFhead[0] + EKFhead[1]) + UWBAnc_Full * UWBAnc_Full) <
                1.0)) {
            /* DCM2EULR       Direction cosine matrix to Euler angle */
            /*                vector conversion. */
            /*         */
            /* 	eul_vect = dcm2eulr(DCMbn) */
            /*  */
            /*    INPUTS */
            /*        DCMbn = 3x3 direction cosine matrix providing the */
            /*              transformation from the body frame */
            /*              to the navigation frame */
            /*  */
            /*    OUTPUTS */
            /*        eul_vect(1) = roll angle in radians  */
            /*  */
            /*        eul_vect(2) = pitch angle in radians  */
            /*  */
            /*        eul_vect(3) = yaw angle in radians  */
            /*  */
            /*    NOTE */
            /*        If the pitch angle is vanishingly close to +/- pi/2, */
            /*        the elements of EUL_VECT will be filled with NaN. */
            /*    REFERENCE:  Titterton, D. and J. Weston, STRAPDOWN */
            /*                INERTIAL NAVIGATION TECHNOLOGY, Peter */
            /*                Peregrinus Ltd. on behalf of the Institution */
            /*                of Electrical Engineers, London, 1997. */
            /*  */
            /* 	M. & S. Braasch 12-97 */
            /* 	Copyright (c) 1997 by GPSoft */
            /* 	All Rights Reserved. */
            /*  */
            RotEul_diff[0] = 0.0;
            curr_pos[0] = 0.0;
            RotEul_diff[1] = 0.0;
            curr_pos[1] = 0.0;
            RotEul_diff[2] = 0.0;
            curr_pos[2] = 0.0;
            firstV = 2.0;
          }
          for (i = 0; i < 99; i++) {
            FiFoSLAMpos_tmp = 3 * (i + 1);
            FiFoSLAMEulDiff[3 * i] = FiFoSLAMEulDiff[FiFoSLAMpos_tmp];
            FiFoSLAMEulDiff[3 * i + 1] = FiFoSLAMEulDiff[FiFoSLAMpos_tmp + 1];
            FiFoSLAMEulDiff[3 * i + 2] = FiFoSLAMEulDiff[FiFoSLAMpos_tmp + 2];
          }
          FiFoSLAMEulDiff[297] = RotEul_diff[0];
          FiFoSLAMEulDiff[298] = RotEul_diff[1];
          FiFoSLAMEulDiff[299] = RotEul_diff[2];
          if (FiFoSLAMtime[98] < FiFoUWBtime[99] + 0.05) {
            b_prev_pos.re = FiFoUWBhead[99] * 0.0;
            b_prev_pos.im = FiFoUWBhead[99];
            b_exp(&b_prev_pos);
            UWBAnc_Full = SLAMpos[0];
            d = SLAMpos[2];
            im = SLAMpos[1];
            for (i = 0; i < 3; i++) {
              prev_pos[i] =
                  ((sA[i] * UWBAnc_Full + sA[i + 3] * d) + sA[i + 6] * im) +
                  SLAMposInit[i];
            }
            SLAMpos[0] = x_tmp[0] - diff_Heading[0];
            SLAMpos[1] = x_tmp[1] - diff_Heading[1];
            SLAMpos[2] = x_tmp[2] - diff_Heading[2];
            dc.re = FiFoUWBpos[99].re +
                    (CAMPos.re * b_prev_pos.re - CAMPos.im * b_prev_pos.im);
            dc.im = FiFoUWBpos[99].im +
                    (CAMPos.re * b_prev_pos.im + CAMPos.im * b_prev_pos.re);
            EKF_UWB_SLAM_4(dc, -FiFoUWBhead[99], curr_pos, RotEul_diff,
                           prev_pos, SLAMpos, MapParam, &EKFpos, EKFhead);
            diff_Heading[0] = x_tmp[0] - EKFhead[0];
            diff_Heading[1] = x_tmp[1] - EKFhead[1];
            diff_Heading[2] = x_tmp[2] - EKFhead[2];
          } else {
            UWBAnc_Full = SLAMpos[0];
            d = SLAMpos[2];
            im = SLAMpos[1];
            for (i = 0; i < 3; i++) {
              prev_pos[i] =
                  ((sA[i] * UWBAnc_Full + sA[i + 3] * d) + sA[i + 6] * im) +
                  SLAMposInit[i];
            }
            SLAMpos[0] = x_tmp[0] - diff_Heading[0];
            UWBAnc_Full = x_tmp[1] - diff_Heading[1];
            SLAMpos[1] = UWBAnc_Full;
            SLAMpos[2] = x_tmp[2] - diff_Heading[2];
            b_prev_pos.re = prev_pos[0];
            b_prev_pos.im = prev_pos[1];
            EKF_UWB_SLAM_4(b_prev_pos, UWBAnc_Full, curr_pos, RotEul_diff,
                           prev_pos, SLAMpos, MapParam, &EKFpos, EKFhead);
            diff_Heading[0] = x_tmp[0] - EKFhead[0];
            diff_Heading[1] = x_tmp[1] - EKFhead[1];
            diff_Heading[2] = x_tmp[2] - EKFhead[2];
          }
          if (SLAMSet == 0.0) {
            if ((EKFpos.re != 0.0) || (EKFpos.im != 0.0)) {
              b_prev_pos.re = FiFoUWBhead[99] * 0.0;
              b_prev_pos.im = FiFoUWBhead[99];
              b_exp(&b_prev_pos);
              UWBAnc_Full =
                  CAMPos.re * b_prev_pos.re - CAMPos.im * b_prev_pos.im;
              im = CAMPos.re * b_prev_pos.im + CAMPos.im * b_prev_pos.re;
              if ((rt_hypotd_snf(EKFpos.re - (FiFoUWBpos[99].re + UWBAnc_Full),
                                 EKFpos.im - (FiFoUWBpos[99].im + im)) < 0.5) &&
                  ((FiFoUWBpos[99].re != 0.0) || (FiFoUWBpos[99].im != 0.0))) {
                SLAMSet = 1.0;
                b_prev_pos.re = EKFhead[1] * 0.0;
                b_prev_pos.im = -EKFhead[1];
                b_exp(&b_prev_pos);
                PrevPosHF.re = EKFpos.re - (CAMPos.re * b_prev_pos.re -
                                            CAMPos.im * b_prev_pos.im);
                PrevPosHF.im = EKFpos.im - (CAMPos.re * b_prev_pos.im +
                                            CAMPos.im * b_prev_pos.re);
                HeadingHF = EKFhead[1];
                for (i = 0; i < 99; i++) {
                  FiFoSLAMUWBpos[i] = FiFoSLAMUWBpos[i + 1];
                  FiFoSLAMUWBhead[i] = FiFoSLAMUWBhead[i + 1];
                  FiFoSLAMUWBtime[i] = FiFoSLAMUWBtime[i + 1];
                }
                FiFoSLAMUWBpos[99] = PrevPosHF;
                FiFoSLAMUWBhead[99] = EKFhead[1];
                FiFoSLAMUWBtime[99] = PositionVector_data[0];
              }
            }
          } else {
            b_prev_pos.re = EKFhead[1] * 0.0;
            b_prev_pos.im = -EKFhead[1];
            b_exp(&b_prev_pos);
            PrevPosHF.re = EKFpos.re - (CAMPos.re * b_prev_pos.re -
                                        CAMPos.im * b_prev_pos.im);
            PrevPosHF.im = EKFpos.im - (CAMPos.re * b_prev_pos.im +
                                        CAMPos.im * b_prev_pos.re);
            HeadingHF = EKFhead[1];
            for (i = 0; i < 99; i++) {
              FiFoSLAMUWBpos[i] = FiFoSLAMUWBpos[i + 1];
              FiFoSLAMUWBhead[i] = FiFoSLAMUWBhead[i + 1];
              FiFoSLAMUWBtime[i] = FiFoSLAMUWBtime[i + 1];
            }
            FiFoSLAMUWBpos[99] = PrevPosHF;
            FiFoSLAMUWBhead[99] = EKFhead[1];
            FiFoSLAMUWBtime[99] = PositionVector_data[0];
          }
        }
      }
    }
  } else {
    /*  UWB */
    s_time_vec[0] = s_time_vec[1];
    s_time_vec[1] = s_time_vec[2];
    s_time_vec[2] = s_time_vec[3];
    s_time_vec[3] = PositionVector_data[0];
    UWBAnc_Full = (PositionVector_data[4] + 6.0) - 1.0;
    if (UWBAnc_Full < 6.0) {
      i = 0;
      i1 = 0;
    } else {
      i = 5;
      i1 = (int)UWBAnc_Full;
    }
    UWBAnc_Full =
        ((PositionVector_data[2] + 6.0) + PositionVector_data[4]) - 1.0;
    if (PositionVector_data[2] + 6.0 > UWBAnc_Full) {
      FiFoSLAMpos_tmp = 0;
      FiFoSLAMhead_tmp = 0;
    } else {
      FiFoSLAMpos_tmp = (int)(PositionVector_data[2] + 6.0) - 1;
      FiFoSLAMhead_tmp = (int)UWBAnc_Full;
    }
    UWBAnc_Full = (PositionVector_data[2] + 6.0) + 4.0;
    d = UWBAnc_Full + PositionVector_data[3];
    if (UWBAnc_Full > d - 1.0) {
      trueCount = 0;
      i2 = 0;
    } else {
      trueCount = (int)UWBAnc_Full - 1;
      i2 = (int)(d - 1.0);
    }
    UWBAnc_Full = d + PositionVector_data[3];
    if (d > UWBAnc_Full - 1.0) {
      i3 = 0;
      i4 = 0;
    } else {
      i3 = (int)d - 1;
      i4 = (int)(UWBAnc_Full - 1.0);
    }
    d = UWBAnc_Full + PositionVector_data[3];
    if (UWBAnc_Full > d - 1.0) {
      i5 = 0;
      i6 = 0;
    } else {
      i5 = (int)UWBAnc_Full - 1;
      i6 = (int)(d - 1.0);
    }
    if (d > (d + 4.0) - 1.0) {
      i7 = 0;
      i8 = -1;
    } else {
      i7 = (int)d - 1;
      i8 = (int)d + 2;
    }
    xt_b.size[0] = 1;
    loop_ub = i8 - i7;
    xt_b.size[1] = loop_ub + 1;
    for (i8 = 0; i8 <= loop_ub; i8++) {
      xt_b.data[i8] = PositionVector_data[i7 + i8];
    }
    if (d + 4.0 > ((d + 4.0) + 4.0) - 1.0) {
      i7 = 0;
      i8 = -1;
    } else {
      i7 = (int)(d + 4.0) - 1;
      i8 = (int)(d + 4.0) + 2;
    }
    yt_b.size[0] = 1;
    loop_ub = i8 - i7;
    yt_b.size[1] = loop_ub + 1;
    for (i8 = 0; i8 <= loop_ub; i8++) {
      yt_b.data[i8] = PositionVector_data[i7 + i8];
    }
    b_PositionVector_size[0] = 1;
    loop_ub = i1 - i;
    b_PositionVector_size[1] = loop_ub;
    for (i1 = 0; i1 < loop_ub; i1++) {
      b_PositionVector_data[i1] = PositionVector_data[i + i1];
    }
    c_PositionVector_size[0] = 1;
    loop_ub = FiFoSLAMhead_tmp - FiFoSLAMpos_tmp;
    c_PositionVector_size[1] = loop_ub;
    for (i = 0; i < loop_ub; i++) {
      c_PositionVector_data[i] = PositionVector_data[FiFoSLAMpos_tmp + i];
    }
    d_PositionVector_size[0] = 1;
    loop_ub = i2 - trueCount;
    d_PositionVector_size[1] = loop_ub;
    for (i = 0; i < loop_ub; i++) {
      d_PositionVector_data[i] = PositionVector_data[trueCount + i];
    }
    e_PositionVector_size[0] = 1;
    loop_ub = i4 - i3;
    e_PositionVector_size[1] = loop_ub;
    for (i = 0; i < loop_ub; i++) {
      e_PositionVector_data[i] = PositionVector_data[i3 + i];
    }
    loop_ub = i6 - i5;
    for (i = 0; i < loop_ub; i++) {
      f_PositionVector_data[i] = PositionVector_data[i5 + i];
    }
    HeadingH = UWBPosition_V4_1(
        PositionVector_data[0], PositionVector_data[2], PositionVector_data[4],
        PositionVector_data[1], b_PositionVector_data, b_PositionVector_size,
        c_PositionVector_data, c_PositionVector_size, d_PositionVector_data,
        d_PositionVector_size, e_PositionVector_data, e_PositionVector_size,
        f_PositionVector_data, xt_b.data, xt_b.size, yt_b.data, yt_b.size,
        PositionVector_data[(int)((d + 4.0) + 4.0) - 1], &PosH, &UWBAnc_Full);
    PosH_tmp = ((PosH.re != 0.0) || (PosH.im != 0.0));
    if (PosH_tmp) {
      for (i = 0; i < 99; i++) {
        FiFoUWBpos[i] = FiFoUWBpos[i + 1];
        FiFoUWBhead[i] = FiFoUWBhead[i + 1];
        FiFoUWBtime[i] = FiFoUWBtime[i + 1];
      }
      FiFoUWBpos[99] = PosH;
      FiFoUWBhead[99] = HeadingH;
      FiFoUWBtime[99] = PositionVector_data[0];
    }
    if (SLAMSet == 0.0) {
      PrevPosHF = PosH;
      HeadingHF = -HeadingH;
      if (PosH_tmp) {
        for (i = 0; i < 99; i++) {
          FiFoSLAMUWBpos[i] = FiFoSLAMUWBpos[i + 1];
          FiFoSLAMUWBhead[i] = FiFoSLAMUWBhead[i + 1];
          FiFoSLAMUWBtime[i] = FiFoSLAMUWBtime[i + 1];
        }
        FiFoSLAMUWBpos[99] = PosH;
        FiFoSLAMUWBhead[99] = -HeadingH;
        FiFoSLAMUWBtime[99] = PositionVector_data[0];
      }
    } else {
      PrevPosHF.re = 0.0;
      PrevPosHF.im = 0.0;
      HeadingHF = 0.0;
    }
  }
  PositionOut[0] = PrevPosHF.re;
  PositionOut[1] = PrevPosHF.im;
  PositionOut[2] = 0.0;
  PositionOut[3] = HeadingHF;
  PositionOut[4] = 0.0;
  PositionOut[5] = 0.0;
  PositionOut[6] = P[0];
  PositionOut[7] = PosH.re;
  PositionOut[8] = PosH.im;
  PositionOut[9] = HeadingH;
}

void PositioningSystem_V5_1_init(void)
{
  int k;
  yt_b.size[1] = 0;
  xt_b.size[1] = 0;
  s_time_vec[0] = 0.0;
  s_time_vec[1] = 0.0;
  s_time_vec[2] = 0.0;
  s_time_vec[3] = 0.0;
  memset(&P[0], 0, 225U * sizeof(double));
  for (k = 0; k < 15; k++) {
    P[k + 15 * k] = 1.0;
  }
  memset(&FiFoUWBpos[0], 0, 100U * sizeof(creal_T));
  memset(&FiFoUWBhead[0], 0, 100U * sizeof(double));
  memset(&FiFoUWBtime[0], 0, 100U * sizeof(double));
  memset(&FiFoSLAMpos[0], 0, 300U * sizeof(double));
  memset(&FiFoSLAMhead[0], 0, 400U * sizeof(double));
  memset(&FiFoSLAMtime[0], 0, 100U * sizeof(double));
  memset(&FiFoSLAMUWBpos[0], 0, 100U * sizeof(creal_T));
  memset(&FiFoSLAMUWBhead[0], 0, 100U * sizeof(double));
  memset(&FiFoSLAMUWBtime[0], 0, 100U * sizeof(double));
  memset(&FiFoIMUacc[0], 0, 12U * sizeof(double));
  memset(&FiFoIMUgyro[0], 0, 12U * sizeof(double));
  memset(&FiFoSLAMEulDiff[0], 0, 300U * sizeof(double));
  firstV = 0.0;
  SLAMSet = 0.0;
  diff_Heading[0] = 0.0;
  diff_Heading[1] = 0.0;
  diff_Heading[2] = 0.0;
  CAMPos.re = -0.06;
  CAMPos.im = -0.77;
}

/* End of code generation (PositioningSystem_V5_1.c) */
