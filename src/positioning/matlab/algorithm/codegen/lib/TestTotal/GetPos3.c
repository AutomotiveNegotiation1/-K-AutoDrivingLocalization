/*
 * GetPos3.c
 *
 * Code generation for function 'GetPos3'
 *
 */

/* Include files */
#include "GetPos3.h"
#include "GetMultiTagPosGen2.h"
#include "TestTotal_data.h"
#include "TestTotal_rtwutil.h"
#include "TwoAnchPos3.h"
#include "dec2bin.h"
#include "rt_nonfinite.h"
#include "sort.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Declarations */
static double rt_powd_snf(double u0, double u1);

/* Function Definitions */
static double rt_powd_snf(double u0, double u1)
{
  double d;
  double d1;
  double y;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = rtNaN;
  } else {
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

void GetPos3(const double dist_data[], int dist_size, const double RxID_data[],
             const int RxID_size[2], double Ln, double PP,
             const creal_T PredPos[4], const double DistPrev[24],
             const double b_RxIDprev[24], const double b_RxIDprevLen[4],
             const double b_PPprev[4], creal_T tag_pos_est[4],
             double *heading_est, creal_T *cand_tag_pos)
{
  static const signed char iv[6] = {10, 10, -10, -10, 20, -20};
  static const signed char iv1[6] = {10, -10, -10, 10, 15, -15};
  creal_T Pos2C[8];
  creal_T Est_P[4];
  creal_T tmp_data[4];
  creal_T Est_C[2];
  cint8_T anch_pos_data[4];
  double ia_data[64];
  double Est_Ft_data[4];
  double Est_F[2];
  double Est_H[2];
  double Prob2[2];
  double dist[2];
  double Est_H_p_im;
  double Est_H_p_re;
  double Pos2C_re;
  double Sel_C_im;
  double Sel_C_re;
  double Sel_F;
  double ai;
  double ai_tmp;
  double ar;
  double ar_tmp;
  double b_ai_tmp;
  double b_ar_tmp;
  double b_d;
  double b_r;
  double brm;
  double c_ai_tmp;
  double c_ar_tmp;
  double d;
  double d1;
  double d2;
  double d3;
  double d_ai_tmp;
  double d_ar_tmp;
  double im;
  double re;
  int Va_size[2];
  int tmp_size[2];
  int L1;
  int L2;
  int P;
  int Qn;
  int b_loop_ub;
  int i;
  int i1;
  int i2;
  int i3;
  int idx;
  int j2;
  int loop_ub;
  char Va_data[64];
  signed char Va_ind[4];
  signed char j_data[4];
  char xtmp;
  boolean_T exitg1;
  memset(&Pos2C[0], 0, 8U * sizeof(creal_T));
  Pos2C[0] = PredPos[0];
  Va_ind[0] = 0;
  Pos2C[1] = PredPos[1];
  Va_ind[1] = 0;
  Pos2C[2] = PredPos[2];
  Va_ind[2] = 0;
  Pos2C[3] = PredPos[3];
  Va_ind[3] = 0;
  Sel_F = 1.0E+7;
  Sel_C_re = 0.0;
  Sel_C_im = 0.0;
  *heading_est = 0.0;
  cand_tag_pos->re = 0.0;
  cand_tag_pos->im = 0.0;
  i = (int)Ln;
  for (L1 = 0; L1 < i; L1++) {
    i1 = (int)(Ln + (1.0 - (((double)L1 + 1.0) + 1.0)));
    if (i1 - 1 >= 0) {
      i2 = (int)(PP + (1.0 - PP));
      Va_ind[(int)PP - 1] = 1;
    }
    for (L2 = 0; L2 < i1; L2++) {
      b_r = (((double)L1 + 1.0) + 1.0) + (double)L2;
      if (i2 - 1 >= 0) {
        j2 = (int)RxID_data[L1] - 1;
        Est_F[0] = iv[j2];
        idx = (int)RxID_data[(int)b_r - 1] - 1;
        Est_F[1] = iv[idx];
        Est_H[0] = iv1[j2];
        Est_H[1] = iv1[idx];
        dist[0] = dist_data[L1];
        dist[1] = dist_data[(int)b_r - 1];
        loop_ub = RxID_size[1];
      }
      if (i2 - 1 >= 0) {
        tmp_size[0] = 1;
        tmp_size[1] = RxID_size[1];
        for (i3 = 0; i3 < loop_ub; i3++) {
          j2 = (int)RxID_data[i3] - 1;
          tmp_data[i3].re = iv[j2];
          tmp_data[i3].im = iv1[j2];
        }
      }
      for (j2 = 0; j2 < i2; j2++) {
        b_r = PP + (double)j2;
        TwoAnchPos3(Est_F, Est_H, dist, tmp_data, tmp_size, dist_data,
                    dist_size, Est_Ft_data, Prob2);
        Pos2C[(int)b_r - 1].re = Est_Ft_data[0];
        Pos2C[(int)b_r - 1].im = Est_Ft_data[2];
        Pos2C[(int)b_r + 3].re = Est_Ft_data[1];
        Pos2C[(int)b_r + 3].im = Est_Ft_data[3];
      }
      for (Qn = 0; Qn < 2; Qn++) {
        dec2bin(rt_powd_snf(2.0, PP * (double)Qn) - 1.0, Va_data, Va_size);
        i3 = Va_size[1] >> 1;
        for (idx = 0; idx < i3; idx++) {
          j2 = (Va_size[1] - idx) - 1;
          xtmp = Va_data[idx];
          Va_data[idx] = Va_data[j2];
          Va_data[j2] = xtmp;
        }
        b_loop_ub = Va_size[1];
        /*  AnchIDList = [1:NP]; */
        /*  for kk = 1 : 4 */
        /*      if kk == 1 */
        /*          A = RxIDprev(kk,1:RxIDprevLen(kk)); */
        /*      else */
        /*          for ll = 1 : (RxIDprevLen(kk)) */
        /*              if length(find(RxIDprev(kk,ll)==A))==0 */
        /*                   A = [A RxIDprev(kk,ll)]; */
        /*              end */
        /*          end */
        /*      end */
        /*  end */
        for (j2 = 0; j2 < b_loop_ub; j2++) {
          ia_data[j2] = 0.0;
          xtmp = Va_data[j2];
          if ((xtmp == '0') && (Va_ind[j2] != 0)) {
            ia_data[j2] = 1.0;
          } else if ((xtmp == '1') && (Va_ind[j2] != 0)) {
            ia_data[j2] = 2.0;
          } else {
            ia_data[j2] = 1.0;
          }
        }
        /*  NB = length(tag_pos_b); */
        j2 = ((int)ia_data[0] - 1) << 2;
        ar_tmp = Pos2C[j2].re;
        ai_tmp = Pos2C[j2].im;
        if (ai_tmp == 0.0) {
          i3 = Va_size[1];
          Pos2C_re = ar_tmp / (double)Va_size[1];
          b_r = 0.0;
        } else if (ar_tmp == 0.0) {
          Pos2C_re = 0.0;
          i3 = Va_size[1];
          b_r = ai_tmp / (double)Va_size[1];
        } else {
          i3 = Va_size[1];
          Pos2C_re = ar_tmp / (double)Va_size[1];
          b_r = ai_tmp / (double)Va_size[1];
        }
        re = Pos2C_re;
        im = b_r;
        j2 = (((int)ia_data[1] - 1) << 2) + 1;
        b_ar_tmp = Pos2C[j2].re;
        b_ai_tmp = Pos2C[j2].im;
        if (b_ai_tmp == 0.0) {
          Pos2C_re = b_ar_tmp / (double)i3;
          b_r = 0.0;
        } else if (b_ar_tmp == 0.0) {
          Pos2C_re = 0.0;
          b_r = b_ai_tmp / (double)i3;
        } else {
          Pos2C_re = b_ar_tmp / (double)i3;
          b_r = b_ai_tmp / (double)i3;
        }
        re += Pos2C_re;
        im += b_r;
        j2 = (((int)ia_data[2] - 1) << 2) + 2;
        c_ar_tmp = Pos2C[j2].re;
        c_ai_tmp = Pos2C[j2].im;
        if (c_ai_tmp == 0.0) {
          Pos2C_re = c_ar_tmp / (double)i3;
          b_r = 0.0;
        } else if (c_ar_tmp == 0.0) {
          Pos2C_re = 0.0;
          b_r = c_ai_tmp / (double)i3;
        } else {
          Pos2C_re = c_ar_tmp / (double)i3;
          b_r = c_ai_tmp / (double)i3;
        }
        re += Pos2C_re;
        im += b_r;
        j2 = (((int)ia_data[3] - 1) << 2) + 3;
        d_ar_tmp = Pos2C[j2].re;
        d_ai_tmp = Pos2C[j2].im;
        if (d_ai_tmp == 0.0) {
          Pos2C_re = d_ar_tmp / (double)i3;
          b_r = 0.0;
        } else if (d_ar_tmp == 0.0) {
          Pos2C_re = 0.0;
          b_r = d_ai_tmp / (double)i3;
        } else {
          Pos2C_re = d_ar_tmp / (double)i3;
          b_r = d_ai_tmp / (double)i3;
        }
        re += Pos2C_re;
        im += b_r;
        ar = ar_tmp - re;
        ai = ai_tmp - im;
        if (dcv[0].im == 0.0) {
          if (ai == 0.0) {
            d = dcv[0].re;
            Pos2C_re = ar / dcv[0].re;
            b_r = 0.0;
          } else if (ar == 0.0) {
            Pos2C_re = 0.0;
            d = dcv[0].re;
            b_r = ai / dcv[0].re;
          } else {
            d = dcv[0].re;
            Pos2C_re = ar / dcv[0].re;
            b_r = ai / dcv[0].re;
          }
        } else {
          d = dcv[0].re;
          if (dcv[0].re == 0.0) {
            if (ar == 0.0) {
              Pos2C_re = ai / dcv[0].im;
              b_r = 0.0;
            } else if (ai == 0.0) {
              Pos2C_re = 0.0;
              b_r = -(ar / dcv[0].im);
            } else {
              Pos2C_re = ai / dcv[0].im;
              b_r = -(ar / dcv[0].im);
            }
          } else {
            brm = fabs(dcv[0].re);
            b_r = fabs(dcv[0].im);
            if (brm > b_r) {
              b_r = dcv[0].im / dcv[0].re;
              b_d = dcv[0].re + b_r * dcv[0].im;
              Pos2C_re = (ar + b_r * ai) / b_d;
              b_r = (ai - b_r * ar) / b_d;
            } else if (b_r == brm) {
              if (dcv[0].re > 0.0) {
                b_r = 0.5;
              } else {
                b_r = -0.5;
              }
              if (dcv[0].im > 0.0) {
                b_d = 0.5;
              } else {
                b_d = -0.5;
              }
              Pos2C_re = (ar * b_r + ai * b_d) / brm;
              b_r = (ai * b_r - ar * b_d) / brm;
            } else {
              b_r = dcv[0].re / dcv[0].im;
              b_d = dcv[0].im + b_r * dcv[0].re;
              Pos2C_re = (b_r * ar + ai) / b_d;
              b_r = (b_r * ai - ar) / b_d;
            }
          }
        }
        Est_H_p_re = Pos2C_re;
        Est_H_p_im = b_r;
        ar = b_ar_tmp - re;
        ai = b_ai_tmp - im;
        if (dcv[1].im == 0.0) {
          if (ai == 0.0) {
            d1 = dcv[1].re;
            Pos2C_re = ar / dcv[1].re;
            b_r = 0.0;
          } else if (ar == 0.0) {
            Pos2C_re = 0.0;
            d1 = dcv[1].re;
            b_r = ai / dcv[1].re;
          } else {
            d1 = dcv[1].re;
            Pos2C_re = ar / dcv[1].re;
            b_r = ai / dcv[1].re;
          }
        } else {
          d1 = dcv[1].re;
          if (dcv[1].re == 0.0) {
            if (ar == 0.0) {
              Pos2C_re = ai / dcv[1].im;
              b_r = 0.0;
            } else if (ai == 0.0) {
              Pos2C_re = 0.0;
              b_r = -(ar / dcv[1].im);
            } else {
              Pos2C_re = ai / dcv[1].im;
              b_r = -(ar / dcv[1].im);
            }
          } else {
            brm = fabs(dcv[1].re);
            b_r = fabs(dcv[1].im);
            if (brm > b_r) {
              b_r = dcv[1].im / dcv[1].re;
              b_d = dcv[1].re + b_r * dcv[1].im;
              Pos2C_re = (ar + b_r * ai) / b_d;
              b_r = (ai - b_r * ar) / b_d;
            } else if (b_r == brm) {
              if (dcv[1].re > 0.0) {
                b_r = 0.5;
              } else {
                b_r = -0.5;
              }
              if (dcv[1].im > 0.0) {
                b_d = 0.5;
              } else {
                b_d = -0.5;
              }
              Pos2C_re = (ar * b_r + ai * b_d) / brm;
              b_r = (ai * b_r - ar * b_d) / brm;
            } else {
              b_r = dcv[1].re / dcv[1].im;
              b_d = dcv[1].im + b_r * dcv[1].re;
              Pos2C_re = (b_r * ar + ai) / b_d;
              b_r = (b_r * ai - ar) / b_d;
            }
          }
        }
        Est_H_p_re += Pos2C_re;
        Est_H_p_im += b_r;
        ar = c_ar_tmp - re;
        ai = c_ai_tmp - im;
        if (dcv[2].im == 0.0) {
          if (ai == 0.0) {
            d2 = dcv[2].re;
            Pos2C_re = ar / dcv[2].re;
            b_r = 0.0;
          } else if (ar == 0.0) {
            Pos2C_re = 0.0;
            d2 = dcv[2].re;
            b_r = ai / dcv[2].re;
          } else {
            d2 = dcv[2].re;
            Pos2C_re = ar / dcv[2].re;
            b_r = ai / dcv[2].re;
          }
        } else {
          d2 = dcv[2].re;
          if (dcv[2].re == 0.0) {
            if (ar == 0.0) {
              Pos2C_re = ai / dcv[2].im;
              b_r = 0.0;
            } else if (ai == 0.0) {
              Pos2C_re = 0.0;
              b_r = -(ar / dcv[2].im);
            } else {
              Pos2C_re = ai / dcv[2].im;
              b_r = -(ar / dcv[2].im);
            }
          } else {
            brm = fabs(dcv[2].re);
            b_r = fabs(dcv[2].im);
            if (brm > b_r) {
              b_r = dcv[2].im / dcv[2].re;
              b_d = dcv[2].re + b_r * dcv[2].im;
              Pos2C_re = (ar + b_r * ai) / b_d;
              b_r = (ai - b_r * ar) / b_d;
            } else if (b_r == brm) {
              if (dcv[2].re > 0.0) {
                b_r = 0.5;
              } else {
                b_r = -0.5;
              }
              if (dcv[2].im > 0.0) {
                b_d = 0.5;
              } else {
                b_d = -0.5;
              }
              Pos2C_re = (ar * b_r + ai * b_d) / brm;
              b_r = (ai * b_r - ar * b_d) / brm;
            } else {
              b_r = dcv[2].re / dcv[2].im;
              b_d = dcv[2].im + b_r * dcv[2].re;
              Pos2C_re = (b_r * ar + ai) / b_d;
              b_r = (b_r * ai - ar) / b_d;
            }
          }
        }
        Est_H_p_re += Pos2C_re;
        Est_H_p_im += b_r;
        ar = d_ar_tmp - re;
        ai = d_ai_tmp - im;
        if (dcv[3].im == 0.0) {
          if (ai == 0.0) {
            d3 = dcv[3].re;
            Pos2C_re = ar / dcv[3].re;
            b_r = 0.0;
          } else if (ar == 0.0) {
            Pos2C_re = 0.0;
            d3 = dcv[3].re;
            b_r = ai / dcv[3].re;
          } else {
            d3 = dcv[3].re;
            Pos2C_re = ar / dcv[3].re;
            b_r = ai / dcv[3].re;
          }
        } else {
          d3 = dcv[3].re;
          if (dcv[3].re == 0.0) {
            if (ar == 0.0) {
              Pos2C_re = ai / dcv[3].im;
              b_r = 0.0;
            } else if (ai == 0.0) {
              Pos2C_re = 0.0;
              b_r = -(ar / dcv[3].im);
            } else {
              Pos2C_re = ai / dcv[3].im;
              b_r = -(ar / dcv[3].im);
            }
          } else {
            brm = fabs(dcv[3].re);
            b_r = fabs(dcv[3].im);
            if (brm > b_r) {
              b_r = dcv[3].im / dcv[3].re;
              b_d = dcv[3].re + b_r * dcv[3].im;
              Pos2C_re = (ar + b_r * ai) / b_d;
              b_r = (ai - b_r * ar) / b_d;
            } else if (b_r == brm) {
              if (dcv[3].re > 0.0) {
                b_r = 0.5;
              } else {
                b_r = -0.5;
              }
              if (dcv[3].im > 0.0) {
                b_d = 0.5;
              } else {
                b_d = -0.5;
              }
              Pos2C_re = (ar * b_r + ai * b_d) / brm;
              b_r = (ai * b_r - ar * b_d) / brm;
            } else {
              b_r = dcv[3].re / dcv[3].im;
              b_d = dcv[3].im + b_r * dcv[3].re;
              Pos2C_re = (b_r * ar + ai) / b_d;
              b_r = (b_r * ai - ar) / b_d;
            }
          }
        }
        Est_H_p_re += Pos2C_re;
        Est_H_p_im += b_r;
        brm = rt_atan2d_snf(Est_H_p_im, Est_H_p_re);
        /*      [v,indp] = find(PPprev==P); */
        /*      if length(indp)>1 */
        /*          indp = indp(end); */
        /*      end */
        /*      anch_pos = xa(RxIDprev{indp})+j*ya(RxIDprev{indp}); */
        Pos2C_re = brm * 0.0;
        if (brm == 0.0) {
          Est_H_p_re = exp(Pos2C_re);
          Est_H_p_im = 0.0;
        } else {
          b_r = exp(Pos2C_re / 2.0);
          Est_H_p_re = b_r * (b_r * cos(brm));
          Est_H_p_im = b_r * (b_r * sin(brm));
        }
        b_d = re + (d * Est_H_p_re - dcv[0].im * Est_H_p_im);
        b_r = im + (d * Est_H_p_im + dcv[0].im * Est_H_p_re);
        Est_P[0].re = b_d;
        Est_P[0].im = b_r;
        b_r = rt_hypotd_snf(b_d - ar_tmp, b_r - ai_tmp);
        d = b_r * b_r;
        /*      for q = 1 : length(RxIDprev{indp}) */
        /*          Est_F = Est_F +
         * abs(abs(anch_pos(q)-Pos2C(P,ia(P)))-PrevDist(RxIDprev{indp}(q),P))^2;
         */
        /*      end */
        /*      [v,indp] = find(PPprev==P); */
        /*      if length(indp)>1 */
        /*          indp = indp(end); */
        /*      end */
        /*      anch_pos = xa(RxIDprev{indp})+j*ya(RxIDprev{indp}); */
        if (brm == 0.0) {
          Est_H_p_re = exp(Pos2C_re);
          Est_H_p_im = 0.0;
        } else {
          b_r = exp(Pos2C_re / 2.0);
          Est_H_p_re = b_r * (b_r * cos(brm));
          Est_H_p_im = b_r * (b_r * sin(brm));
        }
        b_d = re + (d1 * Est_H_p_re - dcv[1].im * Est_H_p_im);
        b_r = im + (d1 * Est_H_p_im + dcv[1].im * Est_H_p_re);
        Est_P[1].re = b_d;
        Est_P[1].im = b_r;
        b_r = rt_hypotd_snf(b_d - b_ar_tmp, b_r - b_ai_tmp);
        d += b_r * b_r;
        /*      for q = 1 : length(RxIDprev{indp}) */
        /*          Est_F = Est_F +
         * abs(abs(anch_pos(q)-Pos2C(P,ia(P)))-PrevDist(RxIDprev{indp}(q),P))^2;
         */
        /*      end */
        /*      [v,indp] = find(PPprev==P); */
        /*      if length(indp)>1 */
        /*          indp = indp(end); */
        /*      end */
        /*      anch_pos = xa(RxIDprev{indp})+j*ya(RxIDprev{indp}); */
        if (brm == 0.0) {
          Est_H_p_re = exp(Pos2C_re);
          Est_H_p_im = 0.0;
        } else {
          b_r = exp(Pos2C_re / 2.0);
          Est_H_p_re = b_r * (b_r * cos(brm));
          Est_H_p_im = b_r * (b_r * sin(brm));
        }
        b_d = re + (d2 * Est_H_p_re - dcv[2].im * Est_H_p_im);
        b_r = im + (d2 * Est_H_p_im + dcv[2].im * Est_H_p_re);
        Est_P[2].re = b_d;
        Est_P[2].im = b_r;
        b_r = rt_hypotd_snf(b_d - c_ar_tmp, b_r - c_ai_tmp);
        d += b_r * b_r;
        /*      for q = 1 : length(RxIDprev{indp}) */
        /*          Est_F = Est_F +
         * abs(abs(anch_pos(q)-Pos2C(P,ia(P)))-PrevDist(RxIDprev{indp}(q),P))^2;
         */
        /*      end */
        /*      [v,indp] = find(PPprev==P); */
        /*      if length(indp)>1 */
        /*          indp = indp(end); */
        /*      end */
        /*      anch_pos = xa(RxIDprev{indp})+j*ya(RxIDprev{indp}); */
        if (brm == 0.0) {
          Est_H_p_re = exp(Pos2C_re);
          Est_H_p_im = 0.0;
        } else {
          b_r = exp(Pos2C_re / 2.0);
          Est_H_p_re = b_r * (b_r * cos(brm));
          Est_H_p_im = b_r * (b_r * sin(brm));
        }
        b_d = re + (d3 * Est_H_p_re - dcv[3].im * Est_H_p_im);
        b_r = im + (d3 * Est_H_p_im + dcv[3].im * Est_H_p_re);
        Est_P[3].re = b_d;
        Est_P[3].im = b_r;
        b_r = rt_hypotd_snf(b_d - d_ar_tmp, b_r - d_ai_tmp);
        d += b_r * b_r;
        /*      for q = 1 : length(RxIDprev{indp}) */
        /*          Est_F = Est_F +
         * abs(abs(anch_pos(q)-Pos2C(P,ia(P)))-PrevDist(RxIDprev{indp}(q),P))^2;
         */
        /*      end */
        /*  Est_F = 0; */
        for (P = 0; P < 4; P++) {
          idx = 0;
          j2 = 1;
          exitg1 = false;
          while ((!exitg1) && (j2 <= 4)) {
            if (b_PPprev[j2 - 1] == (double)P + 1.0) {
              idx++;
              j_data[idx - 1] = (signed char)j2;
              if (idx >= 4) {
                exitg1 = true;
              } else {
                j2++;
              }
            } else {
              j2++;
            }
          }
          if (idx < 1) {
            j2 = 0;
          } else {
            j2 = idx;
          }
          if (j2 > 1) {
            idx = j_data[j2 - 1] - 1;
          } else {
            idx = 0;
          }
          if (b_RxIDprevLen[idx] < 1.0) {
            b_loop_ub = 0;
            i3 = 0;
          } else {
            b_loop_ub = (int)b_RxIDprevLen[idx];
            i3 = (int)b_RxIDprevLen[idx];
          }
          if (b_loop_ub == i3) {
            Va_size[0] = 1;
            Va_size[1] = b_loop_ub;
            for (i3 = 0; i3 < b_loop_ub; i3++) {
              j2 = (int)b_RxIDprev[idx + (i3 << 2)] - 1;
              anch_pos_data[i3].re = iv[j2];
              anch_pos_data[i3].im = iv1[j2];
            }
          } else {
            b_binary_expand_op(anch_pos_data, Va_size, iv, b_RxIDprev, idx,
                               b_loop_ub - 1, iv1, i3 - 1);
          }
          b_loop_ub = (int)b_RxIDprevLen[idx];
          if (b_loop_ub - 1 >= 0) {
            memset(&Est_Ft_data[0], 0,
                   (unsigned int)b_loop_ub * sizeof(double));
          }
          for (j2 = 0; j2 < b_loop_ub; j2++) {
            b_r =
                fabs(rt_hypotd_snf((double)anch_pos_data[j2].re - Est_P[P].re,
                                   (double)anch_pos_data[j2].im - Est_P[P].im) -
                     DistPrev[((int)b_RxIDprev[idx + (j2 << 2)] + 6 * P) - 1]);
            Est_Ft_data[j2] = b_r * b_r;
            /*          Est_Ft(q) =
             * abs(abs(anch_pos(q)-Pos2C(P,ia(P)))-PrevDist(RxIDprev{indp}(q),P))^2;
             */
          }
          sort(Est_Ft_data, &b_loop_ub);
          d += Est_Ft_data[0] + Est_Ft_data[1];
        }
        Est_F[Qn] = d;
        Est_C[Qn].re = re;
        Est_C[Qn].im = im;
        Est_H[Qn] = brm;
      }
      if ((Est_F[0] > Est_F[1]) ||
          (rtIsNaN(Est_F[0]) && (!rtIsNaN(Est_F[1])))) {
        b_r = Est_F[1];
        j2 = 1;
      } else {
        b_r = Est_F[0];
        j2 = 0;
      }
      if (Sel_F > b_r) {
        Sel_F = b_r;
        Sel_C_re = Est_C[j2].re;
        Sel_C_im = Est_C[j2].im;
        *heading_est = Est_H[j2];
        *cand_tag_pos = Pos2C[((int)PP + (j2 << 2)) - 1];
      }
    }
  }
  Est_H_p_re = *heading_est * 0.0;
  if (*heading_est == 0.0) {
    Est_H_p_re = exp(Est_H_p_re);
    Est_H_p_im = 0.0;
  } else {
    b_r = exp(Est_H_p_re / 2.0);
    Est_H_p_re = b_r * (b_r * cos(*heading_est));
    Est_H_p_im = b_r * (b_r * sin(*heading_est));
  }
  tag_pos_est[0].re =
      Sel_C_re + (dcv[0].re * Est_H_p_re - dcv[0].im * Est_H_p_im);
  tag_pos_est[0].im =
      Sel_C_im + (dcv[0].re * Est_H_p_im + dcv[0].im * Est_H_p_re);
  tag_pos_est[1].re =
      Sel_C_re + (dcv[1].re * Est_H_p_re - dcv[1].im * Est_H_p_im);
  tag_pos_est[1].im =
      Sel_C_im + (dcv[1].re * Est_H_p_im + dcv[1].im * Est_H_p_re);
  tag_pos_est[2].re =
      Sel_C_re + (dcv[2].re * Est_H_p_re - dcv[2].im * Est_H_p_im);
  tag_pos_est[2].im =
      Sel_C_im + (dcv[2].re * Est_H_p_im + dcv[2].im * Est_H_p_re);
  tag_pos_est[3].re =
      Sel_C_re + (dcv[3].re * Est_H_p_re - dcv[3].im * Est_H_p_im);
  tag_pos_est[3].im =
      Sel_C_im + (dcv[3].re * Est_H_p_im + dcv[3].im * Est_H_p_re);
}

/* End of code generation (GetPos3.c) */
