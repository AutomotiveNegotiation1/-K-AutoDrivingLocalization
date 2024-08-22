/*
 * EKF_UWB_SLAM_4.c
 *
 * Code generation for function 'EKF_UWB_SLAM_4'
 *
 */

/* Include files */
#include "EKF_UWB_SLAM_4.h"
#include "mod.h"
#include "mod1.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Variable Definitions */
static double PosP[3];

static double H[96];

static double Hroll;

static double Hpitch;

static double Hyaw;

static boolean_T firstRun_not_empty;

static creal_T totPosP[5];

static double UWBless;

/* Function Definitions */
void EKF_UWB_SLAM_4(const creal_T PosUWB, double HeadUWB, double dPosSLAM[3],
                    double dHeadSLAM[3], const double PosSLAMc[3],
                    const double HeadSLAMc[3], const double MapParam[16],
                    creal_T *Posn, double Headn[3])
{
  static double Q[256];
  static double b_P[256];
  static double R[36];
  static double pitch;
  static double pitchOrg;
  static double roll;
  static double rollOrg;
  static double s;
  static double sOrg;
  static double yaw;
  static double yawOrg;
  static const unsigned char uv[217] = {
      1U,   2U,   3U,   4U,   5U,   6U,   7U,   8U,   9U,   10U,  11U,  12U,
      13U,  14U,  15U,  16U,  18U,  19U,  20U,  21U,  22U,  23U,  24U,  25U,
      26U,  27U,  28U,  29U,  30U,  31U,  32U,  33U,  35U,  36U,  37U,  38U,
      39U,  40U,  41U,  42U,  43U,  44U,  45U,  46U,  47U,  52U,  53U,  54U,
      55U,  56U,  57U,  58U,  59U,  60U,  61U,  62U,  63U,  67U,  69U,  70U,
      71U,  72U,  73U,  74U,  75U,  76U,  77U,  78U,  79U,  83U,  84U,  86U,
      87U,  88U,  89U,  90U,  91U,  92U,  93U,  94U,  95U,  98U,  99U,  100U,
      101U, 103U, 104U, 105U, 106U, 107U, 108U, 109U, 110U, 111U, 115U, 116U,
      117U, 118U, 120U, 121U, 122U, 123U, 124U, 125U, 126U, 127U, 131U, 132U,
      133U, 134U, 135U, 137U, 138U, 139U, 140U, 141U, 142U, 143U, 147U, 148U,
      149U, 150U, 151U, 152U, 154U, 155U, 156U, 157U, 158U, 159U, 160U, 161U,
      162U, 163U, 164U, 165U, 166U, 167U, 168U, 169U, 171U, 172U, 173U, 174U,
      175U, 176U, 177U, 178U, 179U, 180U, 181U, 182U, 183U, 184U, 185U, 186U,
      188U, 189U, 190U, 191U, 192U, 193U, 194U, 195U, 196U, 197U, 198U, 199U,
      200U, 201U, 202U, 203U, 205U, 206U, 207U, 208U, 209U, 210U, 211U, 212U,
      213U, 214U, 215U, 216U, 217U, 219U, 220U, 222U, 223U, 224U, 225U, 226U,
      227U, 228U, 229U, 230U, 231U, 232U, 233U, 234U, 236U, 237U, 239U, 240U,
      241U, 242U, 243U, 244U, 245U, 246U, 247U, 248U, 249U, 250U, 251U, 253U,
      254U};
  static const signed char iv[36] = {
      100, 0, 0, 0,   0, 0, 0, 100, 0, 0, 0,   0, 0, 0, 100, 0, 0, 0,
      0,   0, 0, 100, 0, 0, 0, 0,   0, 0, 100, 0, 0, 0, 0,   0, 0, 100};
  double A[256];
  double Pp[256];
  double b_A[256];
  double K[96];
  double b_Pp[96];
  double x[36];
  double y[36];
  double xp[16];
  double b_PosUWB[6];
  double b_xn_tmp_tmp;
  double b_xp_tmp;
  double b_xp_tmp_tmp;
  double c_xn_tmp_tmp;
  double c_xp_tmp;
  double c_xp_tmp_tmp;
  double d_xn_tmp_tmp;
  double d_xp_tmp;
  double e_xn_tmp_tmp;
  double e_xp_tmp;
  double f_xp_tmp;
  double g_xp_tmp;
  double h_xp_tmp;
  double i_xp_tmp;
  double j_xp_tmp;
  double k_xp_tmp;
  double l_xp_tmp;
  double m_xp_tmp;
  double n_xp_tmp;
  double o_xp_tmp;
  double p_xp_tmp;
  double smax;
  double xn_tmp_tmp;
  double xp_tmp;
  double xp_tmp_tmp;
  int b_i;
  int b_tmp;
  int i;
  int j;
  int jA;
  int jp1j;
  int k;
  int kAcol;
  int mmj_tmp;
  int x_tmp;
  signed char ipiv[6];
  signed char p[6];
  signed char i1;
  boolean_T PosUWB_tmp;
  if (!firstRun_not_empty) {
    for (i = 0; i < 256; i++) {
      Q[i] = 1.0E-5;
    }
    for (i = 0; i < 217; i++) {
      Q[uv[i]] = 0.0;
    }
    for (i = 0; i < 36; i++) {
      R[i] = iv[i];
    }
    for (i = 0; i < 256; i++) {
      b_P[i] = 1.0;
    }
    for (i = 0; i < 217; i++) {
      b_P[uv[i]] = 0.0;
    }
    firstRun_not_empty = true;
    /* MapParam = [s roll pitch yaw sA_00 sA_01 sA_02 sA_10 sA_11 sA_12 sA_20
     * sA_21 sA_22 SLAMposInit_0 SLAMposInit_1 SLAMposInit_2]; */
    sOrg = MapParam[0];
    rollOrg = MapParam[1];
    pitchOrg = MapParam[2];
    yawOrg = MapParam[3];
    s = sOrg;
    roll = rollOrg;
    pitch = pitchOrg;
    yaw = yawOrg;
  }
  PosUWB_tmp = ((PosUWB.re != 0.0) || (PosUWB.im != 0.0));
  if (PosUWB_tmp) {
    if (PosP[0] == 0.0) {
      PosP[0] = PosSLAMc[0];
      PosP[1] = PosSLAMc[1];
      PosP[2] = PosSLAMc[2];
      s = sOrg;
      roll = rollOrg;
      pitch = pitchOrg;
      yaw = yawOrg;
      Hroll = roll;
      Hpitch = pitch;
      Hyaw = yaw;
    } else if (dPosSLAM[0] == 0.0) {
      s = sOrg;
      roll = rollOrg;
      pitch = pitchOrg;
      yaw = yawOrg;
    } else if ((sqrt((dHeadSLAM[0] * dHeadSLAM[0] +
                      dHeadSLAM[1] * dHeadSLAM[1]) +
                     dHeadSLAM[2] * dHeadSLAM[2]) > 0.5) ||
               (sqrt((dPosSLAM[0] * dPosSLAM[0] + dPosSLAM[1] * dPosSLAM[1]) +
                     dPosSLAM[2] * dPosSLAM[2]) > 0.1)) {
      dPosSLAM[0] = 0.0;
      dHeadSLAM[0] = 0.0;
      dPosSLAM[1] = 0.0;
      dHeadSLAM[1] = 0.0;
      dPosSLAM[2] = 0.0;
      dHeadSLAM[2] = 0.0;
      s = sOrg;
      roll = rollOrg;
      pitch = pitchOrg;
      yaw = yawOrg;
    }
  }
  smax = cos(roll);
  xn_tmp_tmp = sin(roll);
  b_xn_tmp_tmp = sin(yaw);
  c_xn_tmp_tmp = cos(yaw);
  d_xn_tmp_tmp = cos(pitch);
  e_xn_tmp_tmp = sin(pitch);
  xp_tmp = smax * e_xn_tmp_tmp;
  b_xp_tmp = xp_tmp * b_xn_tmp_tmp;
  c_xp_tmp = xn_tmp_tmp * b_xn_tmp_tmp;
  xp_tmp *= c_xn_tmp_tmp;
  d_xp_tmp = smax * d_xn_tmp_tmp;
  e_xp_tmp = xn_tmp_tmp * c_xn_tmp_tmp;
  f_xp_tmp = (((d_xp_tmp * dPosSLAM[0] - e_xp_tmp * dPosSLAM[1]) +
               b_xp_tmp * dPosSLAM[1]) +
              c_xp_tmp * dPosSLAM[2]) +
             xp_tmp * dPosSLAM[2];
  g_xp_tmp = s * f_xp_tmp;
  xp[0] = PosP[0] + g_xp_tmp;
  h_xp_tmp = xn_tmp_tmp * e_xn_tmp_tmp;
  xp_tmp_tmp = smax * c_xn_tmp_tmp;
  i_xp_tmp = xp_tmp_tmp * dPosSLAM[1];
  b_xp_tmp_tmp = h_xp_tmp * b_xn_tmp_tmp;
  j_xp_tmp = b_xp_tmp_tmp * dPosSLAM[1];
  k_xp_tmp = smax * b_xn_tmp_tmp * dPosSLAM[2];
  c_xp_tmp_tmp = h_xp_tmp * c_xn_tmp_tmp;
  h_xp_tmp = c_xp_tmp_tmp * dPosSLAM[2];
  l_xp_tmp = xn_tmp_tmp * d_xn_tmp_tmp;
  m_xp_tmp =
      (((l_xp_tmp * dPosSLAM[0] + i_xp_tmp) + j_xp_tmp) - k_xp_tmp) + h_xp_tmp;
  xp[1] = PosP[1] + s * m_xp_tmp;
  n_xp_tmp = d_xn_tmp_tmp * b_xn_tmp_tmp;
  o_xp_tmp = d_xn_tmp_tmp * c_xn_tmp_tmp;
  p_xp_tmp = (-e_xn_tmp_tmp * dPosSLAM[0] + n_xp_tmp * dPosSLAM[1]) +
             o_xp_tmp * dPosSLAM[2];
  xp[2] = PosP[2] + s * p_xp_tmp;
  xp[3] = dPosSLAM[0];
  xp[4] = dPosSLAM[1];
  xp[5] = dPosSLAM[2];
  xp[6] = roll;
  xp[7] = pitch;
  xp[8] = yaw;
  xp[9] = s;
  xp[10] =
      (Hroll + c_mod(dHeadSLAM[0] + 1.5707963267948966)) - 1.5707963267948966;
  xp[11] =
      (Hpitch + b_mod(dHeadSLAM[1] + 3.1415926535897931)) - 3.1415926535897931;
  xp[12] =
      (Hyaw + c_mod(dHeadSLAM[2] + 1.5707963267948966)) - 1.5707963267948966;
  xp[13] = dHeadSLAM[0];
  xp[14] = dHeadSLAM[1];
  xp[15] = dHeadSLAM[2];
  if (PosUWB_tmp) {
    /* ------------------------ */
    memset(&A[0], 0, 256U * sizeof(double));
    for (k = 0; k < 16; k++) {
      A[k + (k << 4)] = 1.0;
    }
    A[48] = s * smax * d_xn_tmp_tmp;
    A[64] = s * (-xn_tmp_tmp * c_xn_tmp_tmp + b_xp_tmp);
    A[80] = s * (c_xp_tmp + xp_tmp);
    A[96] =
        s *
        ((((-xn_tmp_tmp * d_xn_tmp_tmp * dPosSLAM[0] - i_xp_tmp) - j_xp_tmp) +
          k_xp_tmp) -
         h_xp_tmp);
    A[112] = s * ((smax * -e_xn_tmp_tmp * dPosSLAM[0] +
                   d_xp_tmp * b_xn_tmp_tmp * dPosSLAM[1]) +
                  d_xp_tmp * c_xn_tmp_tmp * dPosSLAM[2]);
    A[128] = s * (((c_xp_tmp * dPosSLAM[1] + xp_tmp * dPosSLAM[1]) +
                   e_xp_tmp * dPosSLAM[2]) -
                  b_xp_tmp * dPosSLAM[2]);
    A[144] = f_xp_tmp;
    A[49] = s * l_xp_tmp;
    A[65] = s * (xp_tmp_tmp + b_xp_tmp_tmp);
    smax = -smax * b_xn_tmp_tmp;
    A[81] = s * (smax + c_xp_tmp_tmp);
    A[97] = g_xp_tmp;
    A[113] = s * ((xn_tmp_tmp * -e_xn_tmp_tmp * dPosSLAM[0] +
                   l_xp_tmp * b_xn_tmp_tmp * dPosSLAM[1]) +
                  l_xp_tmp * c_xn_tmp_tmp * dPosSLAM[2]);
    A[129] = s * (((smax * dPosSLAM[1] + c_xp_tmp_tmp * dPosSLAM[1]) -
                   xp_tmp_tmp * dPosSLAM[2]) -
                  b_xp_tmp_tmp * dPosSLAM[2]);
    A[145] = m_xp_tmp;
    A[50] = s * -e_xn_tmp_tmp;
    A[66] = s * n_xp_tmp;
    A[82] = s * o_xp_tmp;
    A[114] = s * ((-d_xn_tmp_tmp * dPosSLAM[0] -
                   e_xn_tmp_tmp * b_xn_tmp_tmp * dPosSLAM[1]) -
                  e_xn_tmp_tmp * c_xn_tmp_tmp * dPosSLAM[2]);
    A[130] = s * (o_xp_tmp * dPosSLAM[1] - n_xp_tmp * dPosSLAM[2]);
    A[146] = p_xp_tmp;
    A[218] = 1.0;
    A[235] = 1.0;
    A[252] = 1.0;
    smax = HeadUWB - Hpitch;
    if (smax > 6.2831853071795862) {
      HeadUWB -= 6.2831853071795862 * floor(smax / 6.2831853071795862);
    } else {
      smax = Hpitch - HeadUWB;
      if (smax > 6.2831853071795862) {
        HeadUWB += 6.2831853071795862 * floor(smax / 6.2831853071795862);
      }
    }
    smax = HeadUWB - Hpitch;
    if (smax > 3.1415926535897931) {
      HeadUWB -= 6.2831853071795862;
    } else if (smax < -3.1415926535897931) {
      HeadUWB += 6.2831853071795862;
    }
    for (i = 0; i < 16; i++) {
      for (jp1j = 0; jp1j < 16; jp1j++) {
        smax = 0.0;
        for (kAcol = 0; kAcol < 16; kAcol++) {
          smax += A[i + (kAcol << 4)] * b_P[kAcol + (jp1j << 4)];
        }
        b_A[i + (jp1j << 4)] = smax;
      }
      for (jp1j = 0; jp1j < 16; jp1j++) {
        smax = 0.0;
        for (kAcol = 0; kAcol < 16; kAcol++) {
          jA = kAcol << 4;
          smax += b_A[i + jA] * A[jp1j + jA];
        }
        jA = i + (jp1j << 4);
        Pp[jA] = smax + Q[jA];
      }
    }
    for (i = 0; i < 6; i++) {
      for (jp1j = 0; jp1j < 16; jp1j++) {
        jA = i + 6 * jp1j;
        K[jp1j + (i << 4)] = H[jA];
        smax = 0.0;
        for (kAcol = 0; kAcol < 16; kAcol++) {
          smax += H[i + 6 * kAcol] * Pp[kAcol + (jp1j << 4)];
        }
        b_Pp[jA] = smax;
      }
    }
    for (i = 0; i < 6; i++) {
      for (jp1j = 0; jp1j < 6; jp1j++) {
        smax = 0.0;
        for (kAcol = 0; kAcol < 16; kAcol++) {
          smax += b_Pp[i + 6 * kAcol] * K[kAcol + (jp1j << 4)];
        }
        x_tmp = i + 6 * jp1j;
        x[x_tmp] = smax + R[x_tmp];
      }
    }
    memset(&y[0], 0, 36U * sizeof(double));
    for (i = 0; i < 6; i++) {
      ipiv[i] = (signed char)(i + 1);
    }
    for (j = 0; j < 5; j++) {
      mmj_tmp = 4 - j;
      b_tmp = j * 7;
      jp1j = b_tmp + 2;
      jA = 6 - j;
      kAcol = 0;
      smax = fabs(x[b_tmp]);
      for (k = 2; k <= jA; k++) {
        s = fabs(x[(b_tmp + k) - 1]);
        if (s > smax) {
          kAcol = k - 1;
          smax = s;
        }
      }
      if (x[b_tmp + kAcol] != 0.0) {
        if (kAcol != 0) {
          jA = j + kAcol;
          ipiv[j] = (signed char)(jA + 1);
          for (k = 0; k < 6; k++) {
            kAcol = j + k * 6;
            smax = x[kAcol];
            x_tmp = jA + k * 6;
            x[kAcol] = x[x_tmp];
            x[x_tmp] = smax;
          }
        }
        i = (b_tmp - j) + 6;
        for (b_i = jp1j; b_i <= i; b_i++) {
          x[b_i - 1] /= x[b_tmp];
        }
      }
      jA = b_tmp;
      for (kAcol = 0; kAcol <= mmj_tmp; kAcol++) {
        smax = x[(b_tmp + kAcol * 6) + 6];
        if (smax != 0.0) {
          i = jA + 8;
          jp1j = (jA - j) + 12;
          for (x_tmp = i; x_tmp <= jp1j; x_tmp++) {
            x[x_tmp - 1] += x[((b_tmp + x_tmp) - jA) - 7] * -smax;
          }
        }
        jA += 6;
      }
    }
    for (i = 0; i < 6; i++) {
      p[i] = (signed char)(i + 1);
    }
    for (k = 0; k < 5; k++) {
      i1 = ipiv[k];
      if (i1 > k + 1) {
        jA = p[i1 - 1];
        p[i1 - 1] = p[k];
        p[k] = (signed char)jA;
      }
    }
    for (k = 0; k < 6; k++) {
      x_tmp = 6 * (p[k] - 1);
      y[k + x_tmp] = 1.0;
      for (j = k + 1; j < 7; j++) {
        i = (j + x_tmp) - 1;
        if (y[i] != 0.0) {
          jp1j = j + 1;
          for (b_i = jp1j; b_i < 7; b_i++) {
            jA = (b_i + x_tmp) - 1;
            y[jA] -= y[i] * x[(b_i + 6 * (j - 1)) - 1];
          }
        }
      }
    }
    for (j = 0; j < 6; j++) {
      jA = 6 * j;
      for (k = 5; k >= 0; k--) {
        kAcol = 6 * k;
        i = k + jA;
        smax = y[i];
        if (smax != 0.0) {
          y[i] = smax / x[k + kAcol];
          for (b_i = 0; b_i < k; b_i++) {
            x_tmp = b_i + jA;
            y[x_tmp] -= y[i] * x[b_i + kAcol];
          }
        }
      }
    }
    for (i = 0; i < 16; i++) {
      for (jp1j = 0; jp1j < 6; jp1j++) {
        smax = 0.0;
        for (kAcol = 0; kAcol < 16; kAcol++) {
          smax += Pp[i + (kAcol << 4)] * K[kAcol + (jp1j << 4)];
        }
        b_Pp[i + (jp1j << 4)] = smax;
      }
    }
    for (i = 0; i < 16; i++) {
      for (jp1j = 0; jp1j < 6; jp1j++) {
        smax = 0.0;
        for (kAcol = 0; kAcol < 6; kAcol++) {
          smax += b_Pp[i + (kAcol << 4)] * y[kAcol + 6 * jp1j];
        }
        K[i + (jp1j << 4)] = smax;
      }
    }
    b_PosUWB[0] = PosUWB.re;
    b_PosUWB[1] = 0.0;
    b_PosUWB[2] = PosUWB.im;
    b_PosUWB[3] = 0.0;
    b_PosUWB[4] = HeadUWB;
    b_PosUWB[5] = 0.0;
    for (i = 0; i < 6; i++) {
      smax = 0.0;
      for (jp1j = 0; jp1j < 16; jp1j++) {
        smax += H[i + 6 * jp1j] * xp[jp1j];
      }
      b_PosUWB[i] -= smax;
    }
    for (i = 0; i < 16; i++) {
      smax = 0.0;
      for (jp1j = 0; jp1j < 6; jp1j++) {
        smax += K[i + (jp1j << 4)] * b_PosUWB[jp1j];
      }
      xp[i] += smax;
      for (jp1j = 0; jp1j < 16; jp1j++) {
        smax = 0.0;
        for (kAcol = 0; kAcol < 6; kAcol++) {
          smax += K[i + (kAcol << 4)] * H[kAcol + 6 * jp1j];
        }
        A[i + (jp1j << 4)] = smax;
      }
      for (jp1j = 0; jp1j < 16; jp1j++) {
        smax = 0.0;
        for (kAcol = 0; kAcol < 16; kAcol++) {
          smax += A[i + (kAcol << 4)] * Pp[kAcol + (jp1j << 4)];
        }
        jA = i + (jp1j << 4);
        b_P[jA] = Pp[jA] - smax;
      }
    }
    UWBless = 0.0;
  } else {
    UWBless++;
    if (UWBless > 3.0) {
      xp[0] = PosSLAMc[0];
      xp[1] = PosSLAMc[2];
      xp[2] = PosSLAMc[1];
      xp[10] = HeadSLAMc[0];
      xp[11] = HeadSLAMc[1];
      xp[12] = HeadSLAMc[2];
    }
  }
  PosP[0] = xp[0];
  PosP[1] = xp[1];
  PosP[2] = xp[2];
  roll = xp[6];
  pitch = xp[7];
  yaw = xp[8];
  s = xp[9];
  Hroll = xp[10];
  Hpitch = xp[11];
  Hyaw = xp[12];
  smax = PosP[0] + PosP[2] * 0.0;
  Posn->re = smax;
  Posn->im = PosP[2];
  Headn[0] = Hroll;
  Headn[1] = Hpitch;
  Headn[2] = Hyaw;
  totPosP[0] = totPosP[1];
  totPosP[1] = totPosP[2];
  totPosP[2] = totPosP[3];
  totPosP[3] = totPosP[4];
  totPosP[4].re = smax;
  totPosP[4].im = PosP[2];
}

void EKF_UWB_SLAM_4_init(void)
{
  static const signed char iv[96] = {
      1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0,
      0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int i;
  for (i = 0; i < 96; i++) {
    H[i] = iv[i];
  }
  firstRun_not_empty = false;
  Hroll = 0.0;
  Hpitch = 0.0;
  Hyaw = 0.0;
  UWBless = 0.0;
  memset(&totPosP[0], 0, 5U * sizeof(creal_T));
  PosP[0] = 0.0;
  PosP[1] = 0.0;
  PosP[2] = 0.0;
}

/* End of code generation (EKF_UWB_SLAM_4.c) */
