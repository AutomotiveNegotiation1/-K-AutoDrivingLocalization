/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: EulerToQuaternion.c
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 22-Sep-2023 10:38:00
 */

/* Include Files */
#include "EulerToQuaternion.h"
#include "EulerKalman_2_data.h"
#include "EulerKalman_2_initialize.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Definitions */
/*
 * Arguments    : double phi
 *                double theta
 *                double psi
 *                double z[4]
 * Return Type  : void
 */
void EulerToQuaternion(double phi, double theta, double psi, double z[4])
{
  double b_z_tmp;
  double cosPhi;
  double cosPsi;
  double cosTheta;
  double sinPhi;
  double sinPsi;
  double sinTheta;
  double z_tmp;
  if (!isInitialized_EulerKalman_2) {
    EulerKalman_2_initialize();
  }
  sinPhi = sin(phi / 2.0);
  cosPhi = cos(phi / 2.0);
  sinTheta = sin(theta / 2.0);
  cosTheta = cos(theta / 2.0);
  sinPsi = sin(psi / 2.0);
  cosPsi = cos(psi / 2.0);
  z_tmp = cosPhi * cosTheta;
  b_z_tmp = sinPhi * sinTheta;
  z[0] = z_tmp * cosPsi + b_z_tmp * sinPsi;
  sinTheta *= cosPhi;
  cosPhi = sinPhi * cosTheta;
  z[1] = cosPhi * cosPsi - sinTheta * sinPsi;
  z[2] = sinTheta * cosPsi + cosPhi * sinPsi;
  z[3] = z_tmp * sinPsi - b_z_tmp * cosPsi;
}

/*
 * File trailer for EulerToQuaternion.c
 *
 * [EOF]
 */
