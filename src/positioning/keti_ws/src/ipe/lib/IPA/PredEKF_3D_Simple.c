/*
 * PredEKF_3D_Simple.c
 *
 * Code generation for function 'PredEKF_3D_Simple'
 *
 */

/* Include files */
#include "PredEKF_3D_Simple.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Definitions */
void PredEKF_3D_Simple(const double Xhat[15], const double acc[3],
                       const double p[3], double dT, double Xbar[15])
{
  double RotMat[9];
  double albega[3];
  double b_RotMat[3];
  double RotMat_tmp;
  double Vxyzb_idx_0;
  double Vxyzb_idx_1;
  double Vxyzb_idx_2;
  double b_RotMat_tmp;
  double c_RotMat_tmp;
  double d_RotMat_tmp;
  double e_RotMat_tmp;
  double f_RotMat_tmp;
  double g_RotMat_tmp;
  int i;
  RotMat_tmp = sin(Xhat[9]);
  b_RotMat_tmp = cos(Xhat[11]);
  c_RotMat_tmp = cos(Xhat[9]);
  d_RotMat_tmp = sin(Xhat[11]);
  e_RotMat_tmp = cos(Xhat[10]);
  f_RotMat_tmp = sin(Xhat[10]);
  RotMat[0] = RotMat_tmp * e_RotMat_tmp;
  g_RotMat_tmp = RotMat_tmp * f_RotMat_tmp;
  RotMat[3] = g_RotMat_tmp * d_RotMat_tmp + c_RotMat_tmp * b_RotMat_tmp;
  RotMat[6] = g_RotMat_tmp * b_RotMat_tmp - c_RotMat_tmp * d_RotMat_tmp;
  RotMat[1] = c_RotMat_tmp * e_RotMat_tmp;
  c_RotMat_tmp *= f_RotMat_tmp;
  RotMat[4] = c_RotMat_tmp * d_RotMat_tmp - RotMat_tmp * b_RotMat_tmp;
  RotMat[7] = c_RotMat_tmp * b_RotMat_tmp + RotMat_tmp * d_RotMat_tmp;
  RotMat[2] = -f_RotMat_tmp;
  RotMat[5] = e_RotMat_tmp * d_RotMat_tmp;
  RotMat[8] = e_RotMat_tmp * b_RotMat_tmp;
  RotMat_tmp = tan(Xhat[10]);
  Vxyzb_idx_0 = acc[0] - Xhat[6];
  Vxyzb_idx_1 = acc[1] - Xhat[7];
  Vxyzb_idx_2 = acc[2] - Xhat[8];
  c_RotMat_tmp = dT * dT;
  for (i = 0; i < 3; i++) {
    double d;
    f_RotMat_tmp = RotMat[i];
    g_RotMat_tmp = RotMat[i + 3];
    d = RotMat[i + 6];
    albega[i] =
        ((0.5 * f_RotMat_tmp * Vxyzb_idx_0 + 0.5 * g_RotMat_tmp * Vxyzb_idx_1) +
         0.5 * d * Vxyzb_idx_2) *
        c_RotMat_tmp;
    b_RotMat[i] = ((f_RotMat_tmp * Vxyzb_idx_0 + g_RotMat_tmp * Vxyzb_idx_1) +
                   d * Vxyzb_idx_2) *
                  dT;
  }
  Vxyzb_idx_0 = b_RotMat[0] + Xhat[3];
  Vxyzb_idx_1 = b_RotMat[1] + Xhat[4];
  Vxyzb_idx_2 = b_RotMat[2] + Xhat[5];
  RotMat[0] = 1.0;
  RotMat[3] = d_RotMat_tmp * RotMat_tmp;
  RotMat[6] = b_RotMat_tmp * RotMat_tmp;
  RotMat[1] = 0.0;
  RotMat[4] = b_RotMat_tmp;
  RotMat[7] = -d_RotMat_tmp;
  RotMat[2] = 0.0;
  RotMat[5] = d_RotMat_tmp / e_RotMat_tmp;
  RotMat[8] = b_RotMat_tmp / e_RotMat_tmp;
  RotMat_tmp = p[0] - Xhat[12];
  c_RotMat_tmp = p[1] - Xhat[13];
  f_RotMat_tmp = p[2] - Xhat[14];
  for (i = 0; i < 3; i++) {
    b_RotMat[i] = ((RotMat[i] * RotMat_tmp + RotMat[i + 3] * c_RotMat_tmp) +
                   RotMat[i + 6] * f_RotMat_tmp) *
                  dT;
  }
  /*  %%% X Y change */
  /*  Pxb = Pxh + Vxh*dT +
   * 0.5*((acc(1)-abxh)*sin(alph)+(acc(2)-abyh)*cos(alph))*dT^2; */
  /*  Pyb = Pyh + Vyh*dT +
   * 0.5*((acc(1)-abxh)*cos(alph)+(acc(2)-abyh)*(-sin(alph)))*dT^2; */
  /*   */
  /*  Vxb = Vxh + ((acc(1)-abxh)*sin(alph)+(acc(2)-abyh)*cos(alph))*dT; */
  /*  Vyb = Vyh + ((acc(1)-abxh)*cos(alph)+(acc(2)-abyh)*(-sin(alph)))*dT; */
  /* %% X Y original */
  /*  Pxb = Pxh + Vxh*dT +
   * 0.5*((acc(1)-abxh)*cos(alph)*cos(betah)+(acc(2)-abyh)*(cos(alph)*sin(betah)*sin(gammah)-sin(alph)*cos(gammah))+(acc(3)-abzh)*(cos(alph)*sin(betah)*cos(gammah)+sin(alph)*sin(gammah)))*dT^2;
   */
  /*  Pyb = Pyh + Vyh*dT +
   * 0.5*((acc(1)-abxh)*sin(alph)*cos(betah)+(acc(2)-abyh)*(sin(alph)*sin(betah)*sin(gammah)+cos(alph)*cos(gammah))+(acc(3)-abzh)*(sin(alph)*sin(betah)*cos(gammah)-cos(alph)*sin(gammah)))*dT^2;
   */
  /*  Pzb = Pzh + Vzh*dT +
   * 0.5*(-1*(acc(1)-abxh)*sin(betah)+(acc(2)-abyh)*(cos(betah)*sin(gammah))+(acc(3)-abzh)*(cos(betah)*cos(gammah)))*dT^2;
   */
  /*   */
  /*  Vxb = Vxh +
   * ((acc(1)-abxh)*cos(alph)*cos(betah)+(acc(2)-abyh)*(cos(alph)*sin(betah)*sin(gammah)-sin(alph)*cos(gammah))+(acc(3)-abzh)*(cos(alph)*sin(betah)*cos(gammah)+sin(alph)*sin(gammah)))*dT;
   */
  /*  Vyb = Vyh +
   * ((acc(1)-abxh)*sin(alph)*cos(betah)+(acc(2)-abyh)*(sin(alph)*sin(betah)*sin(gammah)+cos(alph)*cos(gammah))+(acc(3)-abzh)*(sin(alph)*sin(betah)*cos(gammah)-cos(alph)*sin(gammah)))*dT;
   */
  /*  Vzb = Vzh +
   * (-1*(acc(1)-abxh)*sin(betah)+(acc(2)-abyh)*(cos(betah)*sin(gammah))+(acc(3)-abzh)*(cos(betah)*cos(gammah)))*dT;
   */
  /* %%%%%%% 발산 방지%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
  if ((fabs(Vxyzb_idx_0) > 6.0) || (Vxyzb_idx_1 > 6.0) || (Vxyzb_idx_2 > 6.0)) {
    memset(&Xbar[0], 0, 15U * sizeof(double));
  } else {
    Xbar[0] = (Xhat[0] + Xhat[3] * dT) + albega[0];
    Xbar[1] = (Xhat[1] + Xhat[4] * dT) + albega[1];
    Xbar[2] = (Xhat[2] + Xhat[5] * dT) + albega[2];
    Xbar[3] = Vxyzb_idx_0;
    Xbar[4] = Vxyzb_idx_1;
    Xbar[5] = Vxyzb_idx_2;
    Xbar[6] = Xhat[6];
    Xbar[7] = Xhat[7];
    Xbar[8] = Xhat[8];
    Xbar[9] = b_RotMat[2] + Xhat[9];
    Xbar[10] = b_RotMat[1] + Xhat[10];
    Xbar[11] = b_RotMat[0] + Xhat[11];
    Xbar[12] = Xhat[12];
    Xbar[13] = Xhat[13];
    Xbar[14] = Xhat[14];
  }
  /* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
  /*  Xbar = [Pxb Pyb Pzb Vxb Vyb Vzb abxb abyb abzb alpb betb gamb pbb qbb
   * rbb]; */
}

void b_PredEKF_3D_Simple(const double Xhat[15], const double acc[3],
                         const double p[3], double Xbar[15])
{
  double RotMat[9];
  double albega[3];
  double b_RotMat[3];
  double Pxyzb_idx_0;
  double Pxyzb_idx_1;
  double Pxyzb_idx_2;
  double RotMat_tmp;
  double Vxyzb_idx_0;
  double Vxyzb_idx_1;
  double Vxyzb_idx_2;
  double b_RotMat_tmp;
  double c_RotMat_tmp;
  double d;
  double d1;
  double d2;
  double d_RotMat_tmp;
  int i;
  RotMat_tmp = sin(Xhat[9]);
  b_RotMat_tmp = cos(Xhat[11]);
  Vxyzb_idx_0 = cos(Xhat[9]);
  c_RotMat_tmp = sin(Xhat[11]);
  d_RotMat_tmp = cos(Xhat[10]);
  Vxyzb_idx_1 = sin(Xhat[10]);
  RotMat[0] = RotMat_tmp * d_RotMat_tmp;
  Vxyzb_idx_2 = RotMat_tmp * Vxyzb_idx_1;
  RotMat[3] = Vxyzb_idx_2 * c_RotMat_tmp + Vxyzb_idx_0 * b_RotMat_tmp;
  RotMat[6] = Vxyzb_idx_2 * b_RotMat_tmp - Vxyzb_idx_0 * c_RotMat_tmp;
  RotMat[1] = Vxyzb_idx_0 * d_RotMat_tmp;
  Vxyzb_idx_0 *= Vxyzb_idx_1;
  RotMat[4] = Vxyzb_idx_0 * c_RotMat_tmp - RotMat_tmp * b_RotMat_tmp;
  RotMat[7] = Vxyzb_idx_0 * b_RotMat_tmp + RotMat_tmp * c_RotMat_tmp;
  RotMat[2] = -Vxyzb_idx_1;
  RotMat[5] = d_RotMat_tmp * c_RotMat_tmp;
  RotMat[8] = d_RotMat_tmp * b_RotMat_tmp;
  RotMat_tmp = tan(Xhat[10]);
  Vxyzb_idx_0 = acc[0] - Xhat[6];
  Vxyzb_idx_1 = acc[1] - Xhat[7];
  Vxyzb_idx_2 = acc[2] - Xhat[8];
  for (i = 0; i < 3; i++) {
    d = RotMat[i];
    d1 = RotMat[i + 3];
    d2 = RotMat[i + 6];
    albega[i] = ((0.5 * d * Vxyzb_idx_0 + 0.5 * d1 * Vxyzb_idx_1) +
                 0.5 * d2 * Vxyzb_idx_2) *
                0.0001;
    b_RotMat[i] =
        ((d * Vxyzb_idx_0 + d1 * Vxyzb_idx_1) + d2 * Vxyzb_idx_2) * 0.01;
  }
  Pxyzb_idx_0 = (Xhat[0] + Xhat[3] * 0.01) + albega[0];
  Pxyzb_idx_1 = (Xhat[1] + Xhat[4] * 0.01) + albega[1];
  Pxyzb_idx_2 = (Xhat[2] + Xhat[5] * 0.01) + albega[2];
  Vxyzb_idx_0 = b_RotMat[0] + Xhat[3];
  Vxyzb_idx_1 = b_RotMat[1] + Xhat[4];
  Vxyzb_idx_2 = b_RotMat[2] + Xhat[5];
  RotMat[0] = 1.0;
  RotMat[3] = c_RotMat_tmp * RotMat_tmp;
  RotMat[6] = b_RotMat_tmp * RotMat_tmp;
  RotMat[1] = 0.0;
  RotMat[4] = b_RotMat_tmp;
  RotMat[7] = -c_RotMat_tmp;
  RotMat[2] = 0.0;
  RotMat[5] = c_RotMat_tmp / d_RotMat_tmp;
  RotMat[8] = b_RotMat_tmp / d_RotMat_tmp;
  d = p[0] - Xhat[12];
  d1 = p[1] - Xhat[13];
  d2 = p[2] - Xhat[14];
  for (i = 0; i < 3; i++) {
    b_RotMat[i] =
        ((RotMat[i] * d + RotMat[i + 3] * d1) + RotMat[i + 6] * d2) * 0.01;
  }
  /*  %%% X Y change */
  /*  Pxb = Pxh + Vxh*dT +
   * 0.5*((acc(1)-abxh)*sin(alph)+(acc(2)-abyh)*cos(alph))*dT^2; */
  /*  Pyb = Pyh + Vyh*dT +
   * 0.5*((acc(1)-abxh)*cos(alph)+(acc(2)-abyh)*(-sin(alph)))*dT^2; */
  /*   */
  /*  Vxb = Vxh + ((acc(1)-abxh)*sin(alph)+(acc(2)-abyh)*cos(alph))*dT; */
  /*  Vyb = Vyh + ((acc(1)-abxh)*cos(alph)+(acc(2)-abyh)*(-sin(alph)))*dT; */
  /* %% X Y original */
  /*  Pxb = Pxh + Vxh*dT +
   * 0.5*((acc(1)-abxh)*cos(alph)*cos(betah)+(acc(2)-abyh)*(cos(alph)*sin(betah)*sin(gammah)-sin(alph)*cos(gammah))+(acc(3)-abzh)*(cos(alph)*sin(betah)*cos(gammah)+sin(alph)*sin(gammah)))*dT^2;
   */
  /*  Pyb = Pyh + Vyh*dT +
   * 0.5*((acc(1)-abxh)*sin(alph)*cos(betah)+(acc(2)-abyh)*(sin(alph)*sin(betah)*sin(gammah)+cos(alph)*cos(gammah))+(acc(3)-abzh)*(sin(alph)*sin(betah)*cos(gammah)-cos(alph)*sin(gammah)))*dT^2;
   */
  /*  Pzb = Pzh + Vzh*dT +
   * 0.5*(-1*(acc(1)-abxh)*sin(betah)+(acc(2)-abyh)*(cos(betah)*sin(gammah))+(acc(3)-abzh)*(cos(betah)*cos(gammah)))*dT^2;
   */
  /*   */
  /*  Vxb = Vxh +
   * ((acc(1)-abxh)*cos(alph)*cos(betah)+(acc(2)-abyh)*(cos(alph)*sin(betah)*sin(gammah)-sin(alph)*cos(gammah))+(acc(3)-abzh)*(cos(alph)*sin(betah)*cos(gammah)+sin(alph)*sin(gammah)))*dT;
   */
  /*  Vyb = Vyh +
   * ((acc(1)-abxh)*sin(alph)*cos(betah)+(acc(2)-abyh)*(sin(alph)*sin(betah)*sin(gammah)+cos(alph)*cos(gammah))+(acc(3)-abzh)*(sin(alph)*sin(betah)*cos(gammah)-cos(alph)*sin(gammah)))*dT;
   */
  /*  Vzb = Vzh +
   * (-1*(acc(1)-abxh)*sin(betah)+(acc(2)-abyh)*(cos(betah)*sin(gammah))+(acc(3)-abzh)*(cos(betah)*cos(gammah)))*dT;
   */
  /* %%%%%%% 발산 방지%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
  if ((fabs(Vxyzb_idx_0) > 6.0) || (Vxyzb_idx_1 > 6.0) || (Vxyzb_idx_2 > 6.0)) {
    memset(&Xbar[0], 0, 15U * sizeof(double));
  } else {
    Xbar[0] = Pxyzb_idx_0;
    Xbar[1] = Pxyzb_idx_1;
    Xbar[2] = Pxyzb_idx_2;
    Xbar[3] = Vxyzb_idx_0;
    Xbar[4] = Vxyzb_idx_1;
    Xbar[5] = Vxyzb_idx_2;
    Xbar[6] = Xhat[6];
    Xbar[7] = Xhat[7];
    Xbar[8] = Xhat[8];
    Xbar[9] = b_RotMat[2] + Xhat[9];
    Xbar[10] = b_RotMat[1] + Xhat[10];
    Xbar[11] = b_RotMat[0] + Xhat[11];
    Xbar[12] = Xhat[12];
    Xbar[13] = Xhat[13];
    Xbar[14] = Xhat[14];
  }
  /* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
  /*  Xbar = [Pxb Pyb Pzb Vxb Vyb Vzb abxb abyb abzb alpb betb gamb pbb qbb
   * rbb]; */
}

/* End of code generation (PredEKF_3D_Simple.c) */
