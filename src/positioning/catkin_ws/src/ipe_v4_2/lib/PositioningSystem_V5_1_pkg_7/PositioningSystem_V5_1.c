/*
 * PositioningSystem_V5_1.c
 *
 * Code generation for function 'PositioningSystem_V5_1'
 *
 */

/* Include files */
#include "PositioningSystem_V5_1.h"
#include "EKF_UWB_SLAM_4.h"
#include "EKF_UWB_SLAM_IMU_1.h"
#include "PositioningSystem_V5_1_data.h"
#include "PositioningSystem_V5_1_initialize.h"
#include "PositioningSystem_V5_1_rtwutil.h"
#include "UWBPosition_V4_1.h"
#include "equPlane.h"
#include "exp.h"
#include "ifWhileCond.h"
#include "interp1.h"
#include "mod.h"
#include "mod1.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Type Definitions */
#ifndef struct_emxArray_real_T_1x65
#define struct_emxArray_real_T_1x65
struct emxArray_real_T_1x65 {
  double data[65];
  int size[2];
};
#endif /* struct_emxArray_real_T_1x65 */
#ifndef typedef_emxArray_real_T_1x65
#define typedef_emxArray_real_T_1x65
typedef struct emxArray_real_T_1x65 emxArray_real_T_1x65;
#endif /* typedef_emxArray_real_T_1x65 */

/* Variable Definitions */
static double s_time_vec[4];

static emxArray_real_T_1x65 xt_b;

static emxArray_real_T_1x65 yt_b;

static double P[225];

static creal_T FiFoUWBpos[100];

static double FiFoUWBhead[100];

static double FiFoUWBtime[100];

static double FiFoSLAMpos[300];

static double FiFoSLAMhead[400];

static double FiFoSLAMtime[100];

static double FiFoSLAMEulDiff[300];

static double firstV;

static double sA[9];

static double SLAMposInit[3];

static double SLAMSet;

static double FiFoIMUacc[12];

static double FiFoIMUgyro[12];

static creal_T FiFoSLAMUWBpos[100];

static double FiFoSLAMUWBhead[100];

static double FiFoSLAMUWBtime[100];

static creal_T CAMPos;

/* Function Definitions */
void PositioningSystem_V5_1(const double PositionVector_data[],
                            const int PositionVector_size[2],
                            double PositionOut[10])
{
  creal_T EKFpos;
  creal_T PosH;
  creal_T PrevPosHF;
  creal_T TT1;
  double curr_pos[3];
  double prev_pos[3];
  double HeadingH;
  double HeadingHF;
  double SLAMpos_idx_0;
  double SLAMpos_idx_1;
  double SLAMpos_idx_2;
  int b_PositionVector_size[2];
  int c_PositionVector_size[2];
  int d_PositionVector_size[2];
  int e_PositionVector_size[2];
  int FiFoSLAMpos_tmp;
  int i;
  int i1;
  int i8;
  (void)PositionVector_size;
  if (!isInitialized_PositioningSystem_V5_1) {
    PositioningSystem_V5_1_initialize();
  }
  /*  dT = 0.001; */
  if (PositionVector_data[1] == 5.0) {
    PrevPosHF.re = 0.0;
    PrevPosHF.im = 0.0;
    HeadingHF = 0.0;
    PosH.re = 0.0;
    PosH.im = 0.0;
    HeadingH = 0.0;
    /*  dt = 10e-3; */
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
      /*  GyroD = mean(FiFoIMUgyro)*pi/180; */
      /*  acc = mean(FiFoIMUacc); */
      /*  GyroD = (FiFoIMUgyro(:,end))*pi/180; */
      FiFoIMUgyro[9] = PositionVector_data[5];
      prev_pos[0] = FiFoIMUacc[9];
      FiFoIMUgyro[10] = PositionVector_data[6];
      prev_pos[1] = FiFoIMUacc[10];
      FiFoIMUgyro[11] = PositionVector_data[7];
      prev_pos[2] = FiFoIMUacc[11] + 9.85;
      /*  [PosHF5, HeadingHF5, GammHF5, BetaHF5] = EKF_UWB_IMU_1(dt, acc, GyroD,
       * IMUtime, FiFoUWBpos(end), -FiFoUWBhead(end), FiFoUWBtime(end), grav);
       */
      EKF_UWB_SLAM_IMU_1(prev_pos, &FiFoIMUgyro[9], PositionVector_data[0],
                         FiFoSLAMUWBpos[99], FiFoSLAMUWBhead[99],
                         FiFoSLAMUWBtime[99], &TT1, &SLAMpos_idx_1,
                         &SLAMpos_idx_2);
      /*  [PosHF5, HeadingHF5, GammHF5, BetaHF5] = EKF_UWB_SLAM_IMU_1(dt, acc,
       * GyroD, IMUtime,
       * FiFoSLAMUWBpos(end)-(-0.065+2.5*j)*exp(j*(FiFoSLAMUWBhead(end))),
       * FiFoSLAMUWBhead(end), FiFoSLAMUWBtime(end), grav); */
      /*          PosUWBIMU = [PosUWBIMU PosHF5]; */
      /*          HeadUWBIMU = [HeadUWBIMU HeadingHF5]; */
      /*          GammUWBIMU = [GammUWBIMU GammHF5]; */
      /*          BetaUWBIMU = [BetaUWBIMU BetaHF5]; */
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
    double SLAMorient[4];
    /*  SLAM */
    PrevPosHF.re = 0.0;
    PrevPosHF.im = 0.0;
    HeadingHF = 0.0;
    PosH.re = 0.0;
    PosH.im = 0.0;
    HeadingH = 0.0;
    SLAMpos_idx_0 = PositionVector_data[2];
    SLAMpos_idx_1 = PositionVector_data[3];
    SLAMpos_idx_2 = PositionVector_data[4];
    SLAMorient[0] = PositionVector_data[5];
    SLAMorient[1] = PositionVector_data[6];
    SLAMorient[2] = PositionVector_data[7];
    SLAMorient[3] = PositionVector_data[8];
    /*  SLAMorient = SLAMorient([4 1 2 3]); */
    if ((PositionVector_data[2] != 0.0) &&
        (FiFoSLAMtime[99] - PositionVector_data[0] < -0.001)) {
      int FiFoSLAMhead_tmp;
      int trueCount;
      boolean_T guard1;
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
      FiFoSLAMpos[297] = SLAMpos_idx_0;
      FiFoSLAMpos[298] = SLAMpos_idx_1;
      FiFoSLAMpos[299] = SLAMpos_idx_2;
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
          double b_tmp_data[100];
          double tmp_data[100];
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
          double RotEul_diff[3];
          double x[3];
          double xn_tmp;
          boolean_T b_prev_pos[3];
          curr_pos[0] -= prev_pos[0];
          curr_pos[1] -= prev_pos[1];
          curr_pos[2] -= prev_pos[2];
          /*  DCMTemp = qua2dcm(prev_quat); */
          /*  Yaixs = DCMTemp*[1;0;0]; */
          /*  HeadingEul = zeros(1,2); */
          /*  HeadingEul(1) = atan2(Yaixs(3),Yaixs(1)); */
          /*  RotEul_diff = [0;HeadingEul(2)-HeadingEul(1);0]; */
          equPlane(SLAMorient, x);
          equPlane(&FiFoSLAMhead[392], prev_pos);
          xn_tmp = b_mod((x[0] - prev_pos[0]) + 3.1415926535897931) -
                   3.1415926535897931;
          RotEul_diff[0] = xn_tmp;
          b_prev_pos[0] = (fabs(xn_tmp) > 0.39269908169872414);
          xn_tmp = b_mod((x[1] - prev_pos[1]) + 3.1415926535897931) -
                   3.1415926535897931;
          RotEul_diff[1] = xn_tmp;
          b_prev_pos[1] = (fabs(xn_tmp) > 0.39269908169872414);
          xn_tmp = b_mod((x[2] - prev_pos[2]) + 3.1415926535897931) -
                   3.1415926535897931;
          RotEul_diff[2] = xn_tmp;
          b_prev_pos[2] = (fabs(xn_tmp) > 0.39269908169872414);
          if (ifWhileCond(b_prev_pos)) {
            b_prev_pos[0] = (RotEul_diff[0] > 0.0);
            b_prev_pos[1] = (RotEul_diff[1] > 0.0);
            b_prev_pos[2] = (xn_tmp > 0.0);
            if (ifWhileCond(b_prev_pos)) {
              RotEul_diff[0] -= 6.2831853071795862;
              RotEul_diff[1] -= 6.2831853071795862;
              RotEul_diff[2] = xn_tmp - 6.2831853071795862;
            } else {
              RotEul_diff[0] += 6.2831853071795862;
              RotEul_diff[1] += 6.2831853071795862;
              RotEul_diff[2] = xn_tmp + 6.2831853071795862;
            }
          }
          /*  RotEul_diff = -RotEul_diff; */
          /*  DCMTemp = qua2dcm(curr_quat); */
          /*  Yaixs = DCMTemp*[1;0;0]; */
          /*  HeadingEul(2) = atan2(Yaixs(3),Yaixs(1)); */
          xn_tmp = fabs(curr_pos[0]);
          x[0] = xn_tmp * xn_tmp;
          xn_tmp = fabs(curr_pos[1]);
          x[1] = xn_tmp * xn_tmp;
          xn_tmp = fabs(curr_pos[2]);
          if (!(sqrt((x[0] + x[1]) + xn_tmp * xn_tmp) < 1.0)) {
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
            RotEul_diff[1] = -0.0;
            RotEul_diff[2] = 0.0;
            curr_pos[0] = 0.0;
            curr_pos[1] = 0.0;
            curr_pos[2] = 0.0;
            firstV = 2.0;
          } else {
            /*  RotMat_diff = (qua2dcm(curr_quat))/(qua2dcm(prev_quat)); */
            /*  RotMat_diff = (quat2rotm(curr_quat))/(quat2rotm(prev_quat)); */
            /*   */
            /*  EulCurr = quat2eul(curr_quat); */
            /*  EulPrev = quat2eul(prev_quat); */
            /*  TT = dcm2eulr(RotMat_diff); */
            /*  if abs(TT(3)) > pi/4 */
            /*      OffsetEulr = [pi/2; pi/2; 0]; */
            /*  else */
            /*      OffsetEulr = [0; 0; 0]; */
            /*  end */
            /*  TempOffDcm = eulr2dcm(OffsetEulr); */
            /*  RotEul_diff = dcm2eulr(RotMat_diff*TempOffDcm)-OffsetEulr; */
          }
          /*  if Front == 1 */
          for (i = 0; i < 99; i++) {
            FiFoSLAMpos_tmp = 3 * (i + 1);
            FiFoSLAMEulDiff[3 * i] = FiFoSLAMEulDiff[FiFoSLAMpos_tmp];
            FiFoSLAMEulDiff[3 * i + 1] = FiFoSLAMEulDiff[FiFoSLAMpos_tmp + 1];
            FiFoSLAMEulDiff[3 * i + 2] = FiFoSLAMEulDiff[FiFoSLAMpos_tmp + 2];
          }
          FiFoSLAMEulDiff[297] = RotEul_diff[0];
          FiFoSLAMEulDiff[298] = RotEul_diff[1];
          FiFoSLAMEulDiff[299] = RotEul_diff[2];
          /*  else */
          /*      FiFoSLAMEulDiff = [FiFoSLAMEulDiff(:,2:end) (-RotEul_diff)];
           */
          /*  end */
          if (FiFoSLAMtime[98] < FiFoUWBtime[99] + 0.05) {
            /*  [EKFpos,EKFhead] =
             * EKF_UWB_SLAM_4(FiFoUWBpos(end)+(-0.065+0.74*j)*exp(j*(FiFoUWBhead(end))),
             * -FiFoUWBhead(end), pos_diff_org, RotEul_diff, QuatInit, PosSLAM,
             * tempEulr); */
            /*  if Front == 1 */
            TT1.re = FiFoUWBhead[99] * 0.0;
            TT1.im = FiFoUWBhead[99];
            b_exp(&TT1);
            for (i = 0; i < 3; i++) {
              x[i] = ((sA[i] * SLAMpos_idx_0 + sA[i + 3] * SLAMpos_idx_1) +
                      sA[i + 6] * SLAMpos_idx_2) +
                     SLAMposInit[i];
            }
            creal_T dc;
            dc.re =
                FiFoUWBpos[99].re + (CAMPos.re * TT1.re - CAMPos.im * TT1.im);
            dc.im =
                FiFoUWBpos[99].im + (CAMPos.re * TT1.im + CAMPos.im * TT1.re);
            EKF_UWB_SLAM_4(dc, -FiFoUWBhead[99], curr_pos, RotEul_diff, x,
                           &EKFpos, prev_pos);
            /*  else */
            /*      [EKFpos,EKFhead] =
             * EKF_UWB_SLAM_4(FiFoUWBpos(end)+CAMPos*exp(j*(FiFoUWBhead(end))),
             * -FiFoUWBhead(end), pos_diff_org, -RotEul_diff, QuatInit, PosSLAM,
             * tempEulr); */
            /*  end */
          } else {
            double PosP_tmp;
            double b_xn_tmp;
            double c_xn_tmp;
            /*  if Front == 1 */
            /* ------------------------ */
            SLAMpos_idx_0 = cos(roll);
            SLAMpos_idx_1 = sin(roll);
            SLAMpos_idx_2 = sin(yaw);
            xn_tmp = cos(yaw);
            b_xn_tmp = cos(pitch);
            c_xn_tmp = sin(pitch);
            Hpitch = (Hpitch + b_mod(RotEul_diff[1] + 3.1415926535897931)) -
                     3.1415926535897931;
            UWBless++;
            /*  TempAA = eulr2dcm(HeadSLAMc)*[1;0;0]; */
            /*  Eulr_r = quaternionToEulerYXZ(QuatInit, eulr2dcm([roll  pitch
             * yaw])); */
            /*  x(1) = PosSLAMc(1); */
            /*  x(2) = PosSLAMc(2); */
            /*  x(3) = PosSLAMc(3); */
            /*  x(11) = 0; */
            /*  x(12) = atan2(TempAA(1),TempAA(3)); */
            /*  x(13) = 0; */
            /*  x(1) = PosSLAMc(1); */
            /*  x(2) = PosSLAMc(2); */
            /*  x(3) = PosSLAMc(3); */
            /*   */
            /*  x(11) = HeadSLAMc(1); */
            /*  x(12) = HeadSLAMc(2); */
            /*  x(13) = HeadSLAMc(3); */
            if (UWBless > 30.0) {
              /*  x(11:13) = Eulr_r; */
              Hpitch = b_mod(rt_atan2d_snf(totPosP[4].re - totPosP[0].re,
                                           totPosP[4].im - totPosP[0].im) +
                             6.2831853071795862);
            }
            PosP_tmp = SLAMpos_idx_0 * c_xn_tmp;
            PosP[0] += s * ((((SLAMpos_idx_0 * b_xn_tmp * curr_pos[0] -
                               SLAMpos_idx_1 * xn_tmp * curr_pos[1]) +
                              PosP_tmp * SLAMpos_idx_2 * curr_pos[1]) +
                             SLAMpos_idx_1 * SLAMpos_idx_2 * curr_pos[2]) +
                            PosP_tmp * xn_tmp * curr_pos[2]);
            PosP_tmp = SLAMpos_idx_1 * c_xn_tmp;
            PosP[1] += s * ((((SLAMpos_idx_1 * b_xn_tmp * curr_pos[0] +
                               SLAMpos_idx_0 * xn_tmp * curr_pos[1]) +
                              PosP_tmp * SLAMpos_idx_2 * curr_pos[1]) -
                             SLAMpos_idx_0 * SLAMpos_idx_2 * curr_pos[2]) +
                            PosP_tmp * xn_tmp * curr_pos[2]);
            PosP[2] += s * ((-c_xn_tmp * curr_pos[0] +
                             b_xn_tmp * SLAMpos_idx_2 * curr_pos[1]) +
                            b_xn_tmp * xn_tmp * curr_pos[2]);
            /*   */
            /*  Tep = dcm2eulr(sA); */
            /*  roll = Tep(1); */
            /*  pitch = -Tep(3); */
            /*  yaw = Tep(2); */
            Hroll = (Hroll + c_mod(RotEul_diff[0] + 1.5707963267948966)) -
                    1.5707963267948966;
            Hyaw = (Hyaw + c_mod(RotEul_diff[2] + 1.5707963267948966)) -
                   1.5707963267948966;
            SLAMpos_idx_0 = PosP[0] + PosP[2] * 0.0;
            EKFpos.re = SLAMpos_idx_0;
            EKFpos.im = PosP[2];
            prev_pos[1] = Hpitch;
            /*   */
            /*  totHead = [totHead [Hroll;Hpitch;Hyaw]]; */
            /*  totA = [totA [roll;pitch;yaw]]; */
            /*  totHUWB = [totHUWB HeadUWB]; */
            /*   */
            totPosP[0] = totPosP[1];
            totPosP[1] = totPosP[2];
            totPosP[2] = totPosP[3];
            totPosP[3] = totPosP[4];
            totPosP[4].re = SLAMpos_idx_0;
            totPosP[4].im = PosP[2];
            /*  totPosUWB = [totPosUWB PosUWB]; */
            /*   */
            /*  totdHeadSLAM = [totdHeadSLAM reshape(dHeadSLAM(:),3,1)]; */
            /*  totdPosSLAM = [totdPosSLAM dPosSLAM]; */
            /*  else */
            /*      [EKFpos,EKFhead] = EKF_UWB_SLAM_4(0,0, pos_diff_org,
             * -RotEul_diff, QuatInit, PosSLAM, tempEulr); */
            /*  end */
          }
          if (SLAMSet == 0.0) {
            if ((EKFpos.re != 0.0) || (EKFpos.im != 0.0)) {
              TT1.re = FiFoUWBhead[99] * 0.0;
              TT1.im = FiFoUWBhead[99];
              b_exp(&TT1);
              SLAMpos_idx_0 = CAMPos.re * TT1.re - CAMPos.im * TT1.im;
              SLAMpos_idx_1 = CAMPos.re * TT1.im + CAMPos.im * TT1.re;
              if ((rt_hypotd_snf(
                       EKFpos.re - (FiFoUWBpos[99].re + SLAMpos_idx_0),
                       EKFpos.im - (FiFoUWBpos[99].im + SLAMpos_idx_1)) <
                   0.5) &&
                  ((FiFoUWBpos[99].re != 0.0) || (FiFoUWBpos[99].im != 0.0))) {
                /*  &&
                 * (min(abs(mod((FiFoUWBhead(end)-EKFhead(2)),2*pi)),abs(mod((FiFoUWBhead(end)-EKFhead(2)),2*pi)-2*pi))<0.1)
                 */
                /*  if (EKFpos~=0) &&
                 * (abs(EKFpos-(FiFoUWBpos(end)+(-0.065+0.74*j)*exp(j*(FiFoUWBhead(end)))))<0.5)
                 * && (FiFoUWBpos(end)~=0) % &&
                 * (min(abs(mod((FiFoUWBhead(end)-EKFhead(2)),2*pi)),abs(mod((FiFoUWBhead(end)-EKFhead(2)),2*pi)-2*pi))<0.1)
                 */
                SLAMSet = 1.0;
                /*  PosHF = EKFpos; */
                /*  HeadingHF = EKFhead(2); */
                TT1.re = prev_pos[1] * 0.0;
                TT1.im = -prev_pos[1];
                b_exp(&TT1);
                PrevPosHF.re =
                    EKFpos.re - (CAMPos.re * TT1.re - CAMPos.im * TT1.im);
                PrevPosHF.im =
                    EKFpos.im - (CAMPos.re * TT1.im + CAMPos.im * TT1.re);
                HeadingHF = prev_pos[1];
                for (i = 0; i < 99; i++) {
                  FiFoSLAMUWBpos[i] = FiFoSLAMUWBpos[i + 1];
                  FiFoSLAMUWBhead[i] = FiFoSLAMUWBhead[i + 1];
                  FiFoSLAMUWBtime[i] = FiFoSLAMUWBtime[i + 1];
                }
                FiFoSLAMUWBpos[99] = PrevPosHF;
                FiFoSLAMUWBhead[99] = prev_pos[1];
                FiFoSLAMUWBtime[99] = PositionVector_data[0];
              }
            }
          } else {
            /*  PosHF = EKFpos; */
            /*  HeadingHF = EKFhead(2); */
            TT1.re = prev_pos[1] * 0.0;
            TT1.im = -prev_pos[1];
            b_exp(&TT1);
            PrevPosHF.re =
                EKFpos.re - (CAMPos.re * TT1.re - CAMPos.im * TT1.im);
            PrevPosHF.im =
                EKFpos.im - (CAMPos.re * TT1.im + CAMPos.im * TT1.re);
            HeadingHF = prev_pos[1];
            for (i = 0; i < 99; i++) {
              FiFoSLAMUWBpos[i] = FiFoSLAMUWBpos[i + 1];
              FiFoSLAMUWBhead[i] = FiFoSLAMUWBhead[i + 1];
              FiFoSLAMUWBtime[i] = FiFoSLAMUWBtime[i + 1];
            }
            FiFoSLAMUWBpos[99] = PrevPosHF;
            FiFoSLAMUWBhead[99] = prev_pos[1];
            FiFoSLAMUWBtime[99] = PositionVector_data[0];
          }
        }
      }
    }
  } else {
    double b_PositionVector_data[65];
    double c_PositionVector_data[65];
    double d_PositionVector_data[65];
    double e_PositionVector_data[65];
    double f_PositionVector_data[65];
    double xn_tmp;
    int FiFoSLAMhead_tmp;
    int i2;
    int i3;
    int i4;
    int i5;
    int i6;
    int i7;
    int loop_ub;
    int trueCount;
    boolean_T PosH_tmp;
    /*  UWB */
    s_time_vec[0] = s_time_vec[1];
    s_time_vec[1] = s_time_vec[2];
    s_time_vec[2] = s_time_vec[3];
    s_time_vec[3] = PositionVector_data[0];
    xn_tmp = (PositionVector_data[4] + 6.0) - 1.0;
    if (xn_tmp < 6.0) {
      i = 0;
      i1 = 0;
    } else {
      i = 5;
      i1 = (int)xn_tmp;
    }
    xn_tmp = ((PositionVector_data[2] + 6.0) + PositionVector_data[4]) - 1.0;
    if (PositionVector_data[2] + 6.0 > xn_tmp) {
      FiFoSLAMpos_tmp = 0;
      FiFoSLAMhead_tmp = 0;
    } else {
      FiFoSLAMpos_tmp = (int)(PositionVector_data[2] + 6.0) - 1;
      FiFoSLAMhead_tmp = (int)xn_tmp;
    }
    xn_tmp = (PositionVector_data[2] + 6.0) + 4.0;
    SLAMpos_idx_1 = xn_tmp + PositionVector_data[3];
    if (xn_tmp > SLAMpos_idx_1 - 1.0) {
      trueCount = 0;
      i2 = 0;
    } else {
      trueCount = (int)xn_tmp - 1;
      i2 = (int)(SLAMpos_idx_1 - 1.0);
    }
    xn_tmp = SLAMpos_idx_1 + PositionVector_data[3];
    if (SLAMpos_idx_1 > xn_tmp - 1.0) {
      i3 = 0;
      i4 = 0;
    } else {
      i3 = (int)SLAMpos_idx_1 - 1;
      i4 = (int)(xn_tmp - 1.0);
    }
    SLAMpos_idx_1 = xn_tmp + PositionVector_data[3];
    if (xn_tmp > SLAMpos_idx_1 - 1.0) {
      i5 = 0;
      i6 = 0;
    } else {
      i5 = (int)xn_tmp - 1;
      i6 = (int)(SLAMpos_idx_1 - 1.0);
    }
    if (SLAMpos_idx_1 > (SLAMpos_idx_1 + 4.0) - 1.0) {
      i7 = 0;
      i8 = -1;
    } else {
      i7 = (int)SLAMpos_idx_1 - 1;
      i8 = (int)SLAMpos_idx_1 + 2;
    }
    xt_b.size[0] = 1;
    loop_ub = i8 - i7;
    xt_b.size[1] = loop_ub + 1;
    for (i8 = 0; i8 <= loop_ub; i8++) {
      xt_b.data[i8] = PositionVector_data[i7 + i8];
    }
    if (SLAMpos_idx_1 + 4.0 > ((SLAMpos_idx_1 + 4.0) + 4.0) - 1.0) {
      i7 = 0;
      i8 = -1;
    } else {
      i7 = (int)(SLAMpos_idx_1 + 4.0) - 1;
      i8 = (int)(SLAMpos_idx_1 + 4.0) + 2;
    }
    yt_b.size[0] = 1;
    loop_ub = i8 - i7;
    yt_b.size[1] = loop_ub + 1;
    for (i8 = 0; i8 <= loop_ub; i8++) {
      yt_b.data[i8] = PositionVector_data[i7 + i8];
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
        PositionVector_data[(int)((SLAMpos_idx_1 + 4.0) + 4.0) - 1], &PosH,
        &SLAMpos_idx_0);
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
      /*  PosHF = PosH + (-0.065+0.74*j)*exp(j*HeadingH); */
    } else {
      PrevPosHF.re = 0.0;
      PrevPosHF.im = 0.0;
      HeadingHF = 0.0;
    }
    /*  */
  }
  /*  figure(123412);hold on;plot(PosHF,'ro'); */
  /*  PositionOut = [real(PosHF), imag(PosHF), 0, HeadingHF, GammHF, BetaHF,
   * Uncertainty(1,1), real(PosHF5), imag(PosHF5), HeadingHF5  ]; */
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
  /*  PositionOut = [real(PosHF5), imag(PosHF5), 0, -HeadingHF5, GammHF, BetaHF,
   * Uncertainty(1,1), real(PosH), imag(PosH), HeadingH  ]; */
  /*  */
}

void PositioningSystem_V5_1_init(void)
{
  static const double dv[9] = {
      -0.06611020732818,  -3.30859080761202,   -0.0075904315774107,
      3.30239075120085,   -0.213460995745502,  0.0339592364423471,
      -0.112716735233562, -0.0178754852105826, 0.999394394425305};
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
  /*  load("MapParams2.mat"); */
  SLAMSet = 0.0;
  CAMPos.re = -0.06;
  CAMPos.im = -0.77;
  memcpy(&sA[0], &dv[0], 9U * sizeof(double));
  SLAMposInit[0] = 48.2402110895784;
  SLAMposInit[1] = 5.22590479979198;
  SLAMposInit[2] = 0.0;
}

/* End of code generation (PositioningSystem_V5_1.c) */
