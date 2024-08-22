/*
 * equPlane.c
 *
 * Code generation for function 'equPlane'
 *
 */

/* Include files */
#include "equPlane.h"
#include "PositioningSystem_V5_1_rtwutil.h"
#include "mod.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Definitions */
void equPlane(const double quav[4], double EulCurr[3])
{
  static const signed char C1[9] = {1, 0, 0, 0, 1, 0, 0, 0, 1};
  static const signed char c_C3[9] = {1, 0, 0, 0, 1, 0, 0, 0, 1};
  double EulPi[24];
  double C3[9];
  double DCMTempO[9];
  double DCMTempPi[9];
  double b_C3[9];
  double Qq[8];
  double DCMTempO_tmp;
  double b_DCMTempO_tmp;
  double cthe;
  double sthe;
  int i;
  int idx;
  int k;
  signed char i1;
  signed char i2;
  signed char i3;
  boolean_T exitg1;
  /*  if abs(abs(eulv(1))-pi)<pi/4 */
  /*      EulCurr = eulv; */
  /*  else */
  /*      eulvT = [eulv(1) -eulv(2) eulv(3)-pi]; */
  /*      EulCurr = [-eulvT(1) eulvT(2)+pi eulvT(3)]; */
  /*  end */
  /* QUA2DCM       Quaternion to direction cosine matrix conversion. */
  /*         */
  /* 	 DCMbn = qua2dcm(qua_vec) */
  /*  */
  /*    INPUT */
  /*        qua_vec = 4 element quaternion vector */
  /*                = [a b c d] */
  /*        where: a = cos(MU/2) */
  /*               b = (MUx/MU)*sin(MU/2) */
  /*               c = (MUy/MU)*sin(MU/2) */
  /*               d = (MUz/MU)*sin(MU/2) */
  /*        where: MUx, MUy, MUz are the components of the angle vector */
  /*               MU is the magnitude of the angle vector */
  /*  */
  /*    OUTPUT */
  /*        DCMbn = 3x3 direction cosine matrix providing the */
  /*              transformation from the body frame */
  /*              to the navigation frame */
  /*  */
  /*    REFERENCE:  Titterton, D. and J. Weston, STRAPDOWN */
  /*                INERTIAL NAVIGATION TECHNOLOGY, Peter */
  /*                Peregrinus Ltd. on behalf of the Institution */
  /*                of Electrical Engineers, London, 1997. */
  /*  */
  /* 	M. & S. Braasch 12-97 */
  /* 	Copyright (c) 1997 by GPSoft */
  /* 	All Rights Reserved. */
  /*  */
  cthe = quav[0] * quav[0];
  sthe = quav[1] * quav[1];
  DCMTempO_tmp = quav[2] * quav[2];
  b_DCMTempO_tmp = quav[3] * quav[3];
  DCMTempO[0] = ((cthe + sthe) - DCMTempO_tmp) - b_DCMTempO_tmp;
  cthe -= sthe;
  DCMTempO[4] = (cthe + DCMTempO_tmp) - b_DCMTempO_tmp;
  DCMTempO[8] = (cthe - DCMTempO_tmp) + b_DCMTempO_tmp;
  cthe = quav[1] * quav[2];
  sthe = quav[0] * quav[3];
  DCMTempO[3] = 2.0 * (cthe - sthe);
  DCMTempO_tmp = quav[1] * quav[3];
  b_DCMTempO_tmp = quav[0] * quav[2];
  DCMTempO[6] = 2.0 * (DCMTempO_tmp + b_DCMTempO_tmp);
  DCMTempO[1] = 2.0 * (cthe + sthe);
  cthe = quav[2] * quav[3];
  sthe = quav[0] * quav[1];
  DCMTempO[7] = 2.0 * (cthe - sthe);
  DCMTempO[2] = 2.0 * (DCMTempO_tmp - b_DCMTempO_tmp);
  DCMTempO[5] = 2.0 * (cthe + sthe);
  for (k = 0; k < 8; k++) {
    b_DCMTempO_tmp = ((double)k - 4.0) * 3.1415926535897931 / 4.0;
    /* EULR2DCM       Euler angle vector to direction cosine */
    /*                matrix conversion. */
    /*         */
    /* 	DCMnb = eulr2dcm(eul_vect) */
    /*  */
    /*    INPUTS */
    /*        eul_vect(1) = roll angle in radians  */
    /*  */
    /*        eul_vect(2) = pitch angle in radians  */
    /*  */
    /*        eul_vect(3) = yaw angle in radians  */
    /*  */
    /*    OUTPUTS */
    /*        DCMnb = 3x3 direction cosine matrix providing the */
    /*              transformation from the navigation frame */
    /*              to the body frame */
    /*  */
    /*    REFERENCE:  Titterton, D. and J. Weston, STRAPDOWN */
    /*                INERTIAL NAVIGATION TECHNOLOGY, Peter */
    /*                Peregrinus Ltd. on behalf of the Institution */
    /*                of Electrical Engineers, London, 1997. */
    /*  */
    /* 	M. & S. Braasch 12-97 */
    /* 	Copyright (c) 1997 by GPSoft */
    /* 	All Rights Reserved. */
    /*  */
    cthe = cos(b_DCMTempO_tmp);
    sthe = sin(b_DCMTempO_tmp);
    DCMTempPi[0] = cthe;
    DCMTempPi[3] = 0.0;
    DCMTempPi[6] = -sthe;
    DCMTempPi[1] = 0.0;
    DCMTempPi[4] = 1.0;
    DCMTempPi[7] = 0.0;
    DCMTempPi[2] = sthe;
    DCMTempPi[5] = 0.0;
    DCMTempPi[8] = cthe;
    for (i = 0; i < 3; i++) {
      i1 = c_C3[i];
      i2 = c_C3[i + 3];
      i3 = c_C3[i + 6];
      for (idx = 0; idx < 3; idx++) {
        C3[i + 3 * idx] = ((double)i1 * DCMTempPi[3 * idx] +
                           (double)i2 * DCMTempPi[3 * idx + 1]) +
                          (double)i3 * DCMTempPi[3 * idx + 2];
      }
      cthe = C3[i];
      sthe = C3[i + 3];
      DCMTempO_tmp = C3[i + 6];
      for (idx = 0; idx < 3; idx++) {
        b_C3[i + 3 * idx] =
            (cthe * (double)C1[3 * idx] + sthe * (double)C1[3 * idx + 1]) +
            DCMTempO_tmp * (double)C1[3 * idx + 2];
      }
    }
    for (i = 0; i < 3; i++) {
      cthe = DCMTempO[i];
      sthe = DCMTempO[i + 3];
      DCMTempO_tmp = DCMTempO[i + 6];
      for (idx = 0; idx < 3; idx++) {
        DCMTempPi[i + 3 * idx] =
            (cthe * b_C3[3 * idx] + sthe * b_C3[3 * idx + 1]) +
            DCMTempO_tmp * b_C3[3 * idx + 2];
      }
    }
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
    cthe = rt_atan2d_snf(DCMTempPi[5], DCMTempPi[8]);
    sthe = rt_atan2d_snf(DCMTempPi[1], DCMTempPi[0]);
    EulPi[k] = cthe;
    if (DCMTempPi[2] > 1.0) {
      DCMTempO_tmp = -1.5707963267948966;
    } else if (DCMTempPi[2] < -1.0) {
      DCMTempO_tmp = 1.5707963267948966;
    } else {
      DCMTempO_tmp = asin(-DCMTempPi[2]);
    }
    EulPi[k + 8] = DCMTempO_tmp + b_DCMTempO_tmp;
    EulPi[k + 16] = sthe;
    Qq[k] = fabs(b_mod(cthe + 3.1415926535897931) - 3.1415926535897931) +
            fabs(b_mod(sthe + 3.1415926535897931) - 3.1415926535897931);
  }
  if (!rtIsNaN(Qq[0])) {
    idx = 1;
  } else {
    idx = 0;
    k = 2;
    exitg1 = false;
    while ((!exitg1) && (k < 9)) {
      if (!rtIsNaN(Qq[k - 1])) {
        idx = k;
        exitg1 = true;
      } else {
        k++;
      }
    }
  }
  if (idx == 0) {
    idx = 1;
  } else {
    cthe = Qq[idx - 1];
    i = idx + 1;
    for (k = i; k < 9; k++) {
      b_DCMTempO_tmp = Qq[k - 1];
      if (cthe > b_DCMTempO_tmp) {
        cthe = b_DCMTempO_tmp;
        idx = k;
      }
    }
  }
  EulCurr[0] = b_mod(EulPi[idx - 1] + 3.1415926535897931) - 3.1415926535897931;
  EulCurr[1] = b_mod(EulPi[idx + 7] + 3.1415926535897931) - 3.1415926535897931;
  EulCurr[2] = b_mod(EulPi[idx + 15] + 3.1415926535897931) - 3.1415926535897931;
}

/* End of code generation (equPlane.c) */
