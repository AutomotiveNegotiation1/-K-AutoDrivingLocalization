//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: UWBpos6.cpp
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 30-Nov-2023 16:43:53
//

// Include Files
#include "UWBpos6.h"
#include "GetInitPos4.h"
#include "combineVectorElements.h"
#include "fusion2_data.h"
#include "fusion2_initialize.h"
#include "mean.h"
#include "mod.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <algorithm>
#include <cmath>
#include <cstring>

// Variable Definitions
static double r;

static bool r_not_empty;

static double heading_est_a[60];

static double centerest_a[120];

static double centerest_a_aver[120];

static double headingest_a_aver[60];

static double PrevHeading;

// Function Definitions
//
// [tag_pos_est, heading_est,tag_pos_est_aver,headingest_a_aver_v]
//
// Arguments    : double Ln
//                double Lp
//                double LnC
//                double TagNum
//                double Nanchor
//                const double RxIDin[6]
//                const double RxDistin[6]
//                double s_time
//                const creal_T tag_pos_b[4]
//                const double xain[6]
//                const double yain[6]
//                const creal_T prevTagPos[4]
//                double prevTagHeading
//                double UWBout[21]
// Return Type  : void
//
void UWBpos6(double Ln, double Lp, double LnC, double TagNum, double Nanchor,
             const double RxIDin[6], const double RxDistin[6], double s_time,
             const creal_T tag_pos_b[4], const double xain[6],
             const double yain[6], const creal_T prevTagPos[4],
             double prevTagHeading, double UWBout[21])
{
  static coder::array<creal_T, 3U> Tag_Pos_List;
  static coder::array<creal_T, 2U> PrevPos;
  static coder::array<double, 3U> DistList;
  static coder::array<double, 3U> DistListTime;
  coder::array<creal_T, 3U> b_r;
  coder::array<creal_T, 3U> x;
  coder::array<double, 2U> DistPredicted;
  coder::array<double, 2U> y;
  double xain_data[6];
  double yain_data[6];
  double ErrSum;
  double s;
  int xain_size[2];
  int yain_size[2];
  int b_loop_ub;
  int firstBlockLength;
  int lastBlockLength;
  int loop_ub;
  int nblocks;
  int npages;
  int xi;
  if (!isInitialized_fusion2) {
    fusion2_initialize();
  }
  if (!r_not_empty) {
    r_not_empty = true;
    firstBlockLength = static_cast<int>(Lp);
    Tag_Pos_List.set_size(16, 2, firstBlockLength);
    loop_ub = static_cast<int>(Lp) << 5;
    for (xi = 0; xi < loop_ub; xi++) {
      Tag_Pos_List[xi].re = 0.0;
      Tag_Pos_List[xi].im = 0.0;
    }
    DistList.set_size(static_cast<int>(Ln), firstBlockLength, 30);
    npages = static_cast<int>(Ln) * static_cast<int>(Lp) * 30;
    DistListTime.set_size(static_cast<int>(Ln), firstBlockLength, 30);
    for (xi = 0; xi < npages; xi++) {
      DistList[xi] = 0.0;
      DistListTime[xi] = 0.0;
    }
    PrevPos.set_size(1, firstBlockLength);
    for (xi = 0; xi < firstBlockLength; xi++) {
      PrevPos[xi].re = 0.0;
      PrevPos[xi].im = 0.0;
    }
  }
  if (r < 1000.0) {
    r++;
  }
  if (LnC < 1.0) {
    loop_ub = 0;
    b_loop_ub = 0;
  } else {
    loop_ub = static_cast<int>(LnC);
    b_loop_ub = static_cast<int>(LnC);
  }
  xi = static_cast<int>(Nanchor);
  for (npages = 0; npages < xi; npages++) {
    double b_dv[29];
    // if RxDistin(q)<20
    s = RxIDin[npages];
    for (lastBlockLength = 0; lastBlockLength < 29; lastBlockLength++) {
      b_dv[lastBlockLength] =
          DistList[((static_cast<int>(s) +
                     DistList.size(0) * (static_cast<int>(TagNum) - 1)) +
                    DistList.size(0) * DistList.size(1) *
                        (lastBlockLength + 1)) -
                   1];
    }
    DistList[((static_cast<int>(s) +
               DistList.size(0) * (static_cast<int>(TagNum) - 1)) +
              DistList.size(0) * DistList.size(1) * 29) -
             1] = RxDistin[npages];
    for (lastBlockLength = 0; lastBlockLength < 29; lastBlockLength++) {
      DistList[((static_cast<int>(s) +
                 DistList.size(0) * (static_cast<int>(TagNum) - 1)) +
                DistList.size(0) * DistList.size(1) * lastBlockLength) -
               1] = b_dv[lastBlockLength];
      b_dv[lastBlockLength] =
          DistListTime[((static_cast<int>(s) +
                         DistListTime.size(0) *
                             (static_cast<int>(TagNum) - 1)) +
                        DistListTime.size(0) * DistListTime.size(1) *
                            (lastBlockLength + 1)) -
                       1];
    }
    for (lastBlockLength = 0; lastBlockLength < 29; lastBlockLength++) {
      DistListTime[((static_cast<int>(s) +
                     DistListTime.size(0) * (static_cast<int>(TagNum) - 1)) +
                    DistListTime.size(0) * DistListTime.size(1) *
                        lastBlockLength) -
                   1] = b_dv[lastBlockLength];
    }
    DistListTime[((static_cast<int>(s) +
                   DistListTime.size(0) * (static_cast<int>(TagNum) - 1)) +
                  DistListTime.size(0) * DistListTime.size(1) * 29) -
                 1] = s_time;
    // end
  }
  xi = static_cast<int>(Ln);
  lastBlockLength = static_cast<int>(Lp);
  DistPredicted.set_size(xi, lastBlockLength);
  for (npages = 0; npages < xi; npages++) {
    for (firstBlockLength = 0; firstBlockLength < lastBlockLength;
         firstBlockLength++) {
      for (nblocks = 0; nblocks < 30; nblocks++) {
        if (s_time - DistListTime[(npages +
                                   DistListTime.size(0) * firstBlockLength) +
                                  DistListTime.size(0) * DistListTime.size(1) *
                                      nblocks] >
            2.0) {
          DistList[(npages + DistList.size(0) * firstBlockLength) +
                   DistList.size(0) * DistList.size(1) * nblocks] = 0.0;
          DistListTime[(npages + DistListTime.size(0) * firstBlockLength) +
                       DistListTime.size(0) * DistListTime.size(1) * nblocks] =
              0.0;
        }
      }
      if (s_time -
              DistListTime[(npages + DistListTime.size(0) * firstBlockLength) +
                           DistListTime.size(0) * DistListTime.size(1) * 29] <
          0.3) {
        DistPredicted[npages + DistPredicted.size(0) * firstBlockLength] =
            DistList[(npages + DistList.size(0) * firstBlockLength) +
                     DistList.size(0) * DistList.size(1) * 29];
      } else {
        DistPredicted[npages + DistPredicted.size(0) * firstBlockLength] = 0.0;
      }
      //              leng = length(find(DistList(q,p,:)~=0));
      //              if leng > 1
      //                  DistPredicted(q,p) = sum((DistList(q,p,:)))/leng;
      //              else
      //                  DistPredicted(q,p) = 0;
      //              end
      //          if length(find(DistList(q,p,:)~=0))>3
      //              DistPredicted(q,p) =
      //              InterpPos3(DistListTime(q,p,:),DistList(q,p,:),s_time,1);
      //
      //              %         elseif (q<=size(prevDist,1)) &&
      //              (p<=size(prevDist,2)) %             DistPredicted(q,p) =
      //              prevDist(q,p);
      //          else
      //              DistPredicted(q,p) = 0;
      //          end
    }
  }
  if (TagNum == 4.0) {
    creal_T dc;
    creal_T tag_center_pos_est_tmp;
    double MeanA_im;
    double MeanA_re;
    double ai;
    double ar;
    double brm;
    double bsum;
    double tag_center_pos_est_re;
    double tag_center_vel_est_im;
    double tag_center_vel_est_re;
    int b_k;
    if ((DistPredicted.size(0) == 0) || (DistPredicted.size(1) == 0)) {
      y.set_size(1, DistPredicted.size(1));
      firstBlockLength = DistPredicted.size(1);
      for (xi = 0; xi < firstBlockLength; xi++) {
        y[xi] = 0.0;
      }
    } else {
      npages = DistPredicted.size(1);
      y.set_size(1, DistPredicted.size(1));
      if (DistPredicted.size(0) <= 1024) {
        firstBlockLength = DistPredicted.size(0);
        lastBlockLength = 0;
        nblocks = 1;
      } else {
        firstBlockLength = 1024;
        nblocks = DistPredicted.size(0) / 1024;
        lastBlockLength = DistPredicted.size(0) - (nblocks << 10);
        if (lastBlockLength > 0) {
          nblocks++;
        } else {
          lastBlockLength = 1024;
        }
      }
      for (xi = 0; xi < npages; xi++) {
        int xpageoffset;
        xpageoffset = xi * DistPredicted.size(0);
        y[xi] = DistPredicted[xpageoffset];
        for (b_k = 2; b_k <= firstBlockLength; b_k++) {
          y[xi] = y[xi] + DistPredicted[(xpageoffset + b_k) - 1];
        }
        for (int ib{2}; ib <= nblocks; ib++) {
          int hi;
          int xblockoffset;
          xblockoffset = xpageoffset + ((ib - 1) << 10);
          bsum = DistPredicted[xblockoffset];
          if (ib == nblocks) {
            hi = lastBlockLength;
          } else {
            hi = 1024;
          }
          for (b_k = 2; b_k <= hi; b_k++) {
            bsum += DistPredicted[(xblockoffset + b_k) - 1];
          }
          y[xi] = y[xi] + bsum;
        }
      }
    }
    firstBlockLength = y.size(1);
    if (y.size(1) <= 2) {
      if (y.size(1) == 1) {
        bsum = y[0];
      } else if ((y[0] > y[y.size(1) - 1]) ||
                 (std::isnan(y[0]) && (!std::isnan(y[y.size(1) - 1])))) {
        bsum = y[y.size(1) - 1];
      } else {
        bsum = y[0];
      }
    } else {
      if (!std::isnan(y[0])) {
        npages = 1;
      } else {
        bool exitg1;
        npages = 0;
        b_k = 2;
        exitg1 = false;
        while ((!exitg1) && (b_k <= firstBlockLength)) {
          if (!std::isnan(y[b_k - 1])) {
            npages = b_k;
            exitg1 = true;
          } else {
            b_k++;
          }
        }
      }
      if (npages == 0) {
        bsum = y[0];
      } else {
        bsum = y[npages - 1];
        xi = npages + 1;
        for (b_k = xi; b_k <= firstBlockLength; b_k++) {
          s = y[b_k - 1];
          if (bsum > s) {
            bsum = s;
          }
        }
      }
    }
    if (bsum > 0.0) {
      //  (length(find(DistPredicted(:)~=0))>3)
      //  if (length(find(DistPredicted(:)~=0))>8)
      //  [tag_pos_est_t, heading_est_t] =  GetUWBPos_v3(xa, ya,
      //  DistPredicted(1:LnC,:), tag_pos_b, tag_pos_est, heading_est);
      //          [tag_pos_est_t3, heading_est_t3] =  GetUWBPos_v2(xa, ya,
      //          DistPredicted(1:LnC,:), tag_pos_b);
      if (LnC < 1.0) {
        firstBlockLength = 0;
      } else {
        firstBlockLength = static_cast<int>(LnC);
      }
      xain_size[0] = 1;
      xain_size[1] = loop_ub;
      if (loop_ub - 1 >= 0) {
        std::copy(&xain[0], &xain[loop_ub], &xain_data[0]);
      }
      yain_size[0] = 1;
      yain_size[1] = b_loop_ub;
      if (b_loop_ub - 1 >= 0) {
        std::copy(&yain[0], &yain[b_loop_ub], &yain_data[0]);
      }
      npages = DistPredicted.size(1) - 1;
      for (xi = 0; xi <= npages; xi++) {
        for (lastBlockLength = 0; lastBlockLength < firstBlockLength;
             lastBlockLength++) {
          DistPredicted[lastBlockLength + firstBlockLength * xi] =
              DistPredicted[lastBlockLength + DistPredicted.size(0) * xi];
        }
      }
      creal_T tag_pos_est_aver[4];
      DistPredicted.set_size(firstBlockLength, npages + 1);
      GetInitPos4(xain_data, xain_size, yain_data, yain_size, DistPredicted,
                  tag_pos_b, LnC, prevTagPos, prevTagHeading, tag_pos_est_aver,
                  &PrevHeading, &ErrSum);
      PrevPos.set_size(1, 4);
      PrevPos[0] = tag_pos_est_aver[0];
      PrevPos[1] = tag_pos_est_aver[1];
      PrevPos[2] = tag_pos_est_aver[2];
      PrevPos[3] = tag_pos_est_aver[3];
      //  %     [tag_pos_est_t3, heading_est_t3] =  GetInitPos(xa,ya,
      //  DistPredicted(1:LnC,:),xa+j*ya,tag_pos_b,LnC,4);
      //
      //          Te = sum(abs(abs(transpose(anch_pos)-(tag_pos_est_t2)) -
      //          prevDist).^2,"all"); Tl =
      //          sum(abs(abs(transpose(anch_pos)-(tag_pos_est_t3)) -
      //          prevDist).^2,"all"); if Te < Tl
      //              tag_pos_est_t3 = tag_pos_est_t2;
      //              heading_est_t3 = heading_est_t2;
      //          end
    } else {
      ErrSum = 10000.0;
    }
    loop_ub = Tag_Pos_List.size(2) - 1;
    b_r.set_size(15, 2, Tag_Pos_List.size(2));
    for (xi = 0; xi <= loop_ub; xi++) {
      for (lastBlockLength = 0; lastBlockLength < 2; lastBlockLength++) {
        for (npages = 0; npages < 15; npages++) {
          b_r[(npages + 15 * lastBlockLength) + 30 * xi] =
              Tag_Pos_List[((npages + 16 * lastBlockLength) + 32 * xi) + 1];
        }
      }
    }
    loop_ub = b_r.size(2);
    for (xi = 0; xi < loop_ub; xi++) {
      for (lastBlockLength = 0; lastBlockLength < 2; lastBlockLength++) {
        for (npages = 0; npages < 15; npages++) {
          Tag_Pos_List[(npages + 16 * lastBlockLength) + 32 * xi] =
              b_r[(npages + 15 * lastBlockLength) + 30 * xi];
        }
      }
    }
    Tag_Pos_List[15].re = s_time;
    Tag_Pos_List[15].im = 0.0;
    Tag_Pos_List[31] = PrevPos[0];
    Tag_Pos_List[47].re = s_time;
    Tag_Pos_List[47].im = 0.0;
    Tag_Pos_List[63] = PrevPos[1];
    Tag_Pos_List[79].re = s_time;
    Tag_Pos_List[79].im = 0.0;
    Tag_Pos_List[95] = PrevPos[2];
    Tag_Pos_List[111].re = s_time;
    Tag_Pos_List[111].im = 0.0;
    Tag_Pos_List[127] = PrevPos[3];
    tag_center_pos_est_tmp = coder::mean(PrevPos);
    loop_ub = Tag_Pos_List.size(2);
    x.set_size(1, 1, Tag_Pos_List.size(2));
    for (xi = 0; xi < loop_ub; xi++) {
      x[xi].re = Tag_Pos_List[32 * xi + 11].re - Tag_Pos_List[32 * xi].re;
      x[xi].im = Tag_Pos_List[32 * xi + 11].im - Tag_Pos_List[32 * xi].im;
    }
    dc = coder::combineVectorElements(x);
    if ((dc.re != 0.0) || (dc.im != 0.0)) {
      loop_ub = Tag_Pos_List.size(2);
      x.set_size(1, 1, Tag_Pos_List.size(2));
      for (xi = 0; xi < loop_ub; xi++) {
        ar = Tag_Pos_List[32 * xi + 27].re - Tag_Pos_List[32 * xi + 16].re;
        ai = Tag_Pos_List[32 * xi + 27].im - Tag_Pos_List[32 * xi + 16].im;
        MeanA_re = Tag_Pos_List[32 * xi + 11].re - Tag_Pos_List[32 * xi].re;
        MeanA_im = Tag_Pos_List[32 * xi + 11].im - Tag_Pos_List[32 * xi].im;
        if (MeanA_im == 0.0) {
          if (ai == 0.0) {
            x[xi].re = ar / MeanA_re;
            x[xi].im = 0.0;
          } else if (ar == 0.0) {
            x[xi].re = 0.0;
            x[xi].im = ai / MeanA_re;
          } else {
            x[xi].re = ar / MeanA_re;
            x[xi].im = ai / MeanA_re;
          }
        } else if (MeanA_re == 0.0) {
          if (ar == 0.0) {
            x[xi].re = ai / MeanA_im;
            x[xi].im = 0.0;
          } else if (ai == 0.0) {
            x[xi].re = 0.0;
            x[xi].im = -(ar / MeanA_im);
          } else {
            x[xi].re = ai / MeanA_im;
            x[xi].im = -(ar / MeanA_im);
          }
        } else {
          brm = std::abs(MeanA_re);
          bsum = std::abs(MeanA_im);
          if (brm > bsum) {
            s = MeanA_im / MeanA_re;
            bsum = MeanA_re + s * MeanA_im;
            x[xi].re = (ar + s * ai) / bsum;
            x[xi].im = (ai - s * ar) / bsum;
          } else if (bsum == brm) {
            if (MeanA_re > 0.0) {
              s = 0.5;
            } else {
              s = -0.5;
            }
            if (MeanA_im > 0.0) {
              bsum = 0.5;
            } else {
              bsum = -0.5;
            }
            x[xi].re = (ar * s + ai * bsum) / brm;
            x[xi].im = (ai * s - ar * bsum) / brm;
          } else {
            s = MeanA_re / MeanA_im;
            bsum = MeanA_im + s * MeanA_re;
            x[xi].re = (s * ar + ai) / bsum;
            x[xi].im = (s * ai - ar) / bsum;
          }
        }
      }
      dc = coder::combineVectorElements(x);
      if (dc.im == 0.0) {
        tag_center_vel_est_re = dc.re / static_cast<double>(x.size(2));
        tag_center_vel_est_im = 0.0;
      } else if (dc.re == 0.0) {
        tag_center_vel_est_re = 0.0;
        tag_center_vel_est_im = dc.im / static_cast<double>(x.size(2));
      } else {
        tag_center_vel_est_re = dc.re / static_cast<double>(x.size(2));
        tag_center_vel_est_im = dc.im / static_cast<double>(x.size(2));
      }
    } else {
      tag_center_vel_est_re = 0.0;
      tag_center_vel_est_im = 0.0;
    }
    //  heading_est_temp = mean(Tag_Pos_List(12,2,[2
    //  4]))-mean(Tag_Pos_List(1,2,[1 3])); heading_est =
    //  angle(heading_est_temp);
    for (xi = 0; xi < 59; xi++) {
      heading_est_a[xi] = heading_est_a[xi + 1];
    }
    s = std::round(std::abs(PrevHeading - heading_est_a[58]) /
                   6.2831853071795862);
    if (s < 2.147483648E+9) {
      xi = static_cast<int>(s);
    } else if (s >= 2.147483648E+9) {
      xi = MAX_int32_T;
    } else {
      xi = 0;
    }
    s = std::round(6.2831853071795862 * static_cast<double>(xi));
    if (s < 2.147483648E+9) {
      xi = static_cast<int>(s);
    } else {
      xi = MAX_int32_T;
    }
    bsum = std::round(PrevHeading - static_cast<double>(xi));
    if (bsum < 2.147483648E+9) {
      if (bsum >= -2.147483648E+9) {
        xi = static_cast<int>(bsum);
      } else {
        xi = MIN_int32_T;
      }
    } else if (bsum >= 2.147483648E+9) {
      xi = MAX_int32_T;
    } else {
      xi = 0;
    }
    if (s < 2.147483648E+9) {
      lastBlockLength = static_cast<int>(s);
    } else {
      lastBlockLength = MAX_int32_T;
    }
    s = std::round(PrevHeading + static_cast<double>(lastBlockLength));
    if (s < 2.147483648E+9) {
      if (s >= -2.147483648E+9) {
        lastBlockLength = static_cast<int>(s);
      } else {
        lastBlockLength = MIN_int32_T;
      }
    } else if (s >= 2.147483648E+9) {
      lastBlockLength = MAX_int32_T;
    } else {
      lastBlockLength = 0;
    }
    if (PrevHeading - heading_est_a[58] > 3.1415926535897931) {
      heading_est_a[59] = xi;
    } else if (heading_est_a[58] - PrevHeading > 3.1415926535897931) {
      heading_est_a[59] = lastBlockLength;
    } else {
      heading_est_a[59] = PrevHeading;
    }
    for (xi = 0; xi < 2; xi++) {
      for (lastBlockLength = 0; lastBlockLength < 59; lastBlockLength++) {
        npages = lastBlockLength + 60 * xi;
        centerest_a[npages] = centerest_a[npages + 1];
        centerest_a_aver[npages] = centerest_a_aver[npages + 1];
      }
    }
    centerest_a[59] = tag_center_pos_est_tmp.re;
    centerest_a[119] = tag_center_pos_est_tmp.im;
    for (xi = 0; xi < 59; xi++) {
      headingest_a_aver[xi] = headingest_a_aver[xi + 1];
    }
    if (r > 60.0) {
      creal_T b_x[30];
      double b_y;
      for (xi = 0; xi < 30; xi++) {
        s = centerest_a[xi + 60];
        b_x[xi].re = centerest_a[xi] + 0.0 * s;
        b_x[xi].im = s;
      }
      tag_center_pos_est_re = b_x[0].re;
      bsum = b_x[0].im;
      for (b_k = 0; b_k < 29; b_k++) {
        tag_center_pos_est_re += b_x[b_k + 1].re;
        bsum += b_x[b_k + 1].im;
      }
      if (bsum == 0.0) {
        MeanA_re = tag_center_pos_est_re / 30.0;
        MeanA_im = 0.0;
      } else if (tag_center_pos_est_re == 0.0) {
        MeanA_re = 0.0;
        MeanA_im = bsum / 30.0;
      } else {
        MeanA_re = tag_center_pos_est_re / 30.0;
        MeanA_im = bsum / 30.0;
      }
      for (xi = 0; xi < 30; xi++) {
        s = centerest_a[xi + 90];
        b_x[xi].re = centerest_a[xi + 30] + 0.0 * s;
        b_x[xi].im = s;
      }
      tag_center_pos_est_re = b_x[0].re;
      bsum = b_x[0].im;
      b_y = heading_est_a[0];
      brm = heading_est_a[30];
      for (b_k = 0; b_k < 29; b_k++) {
        tag_center_pos_est_re += b_x[b_k + 1].re;
        bsum += b_x[b_k + 1].im;
        b_y += heading_est_a[b_k + 1];
        brm += heading_est_a[b_k + 31];
      }
      if (bsum == 0.0) {
        ar = tag_center_pos_est_re / 30.0;
        ai = 0.0;
      } else if (tag_center_pos_est_re == 0.0) {
        ar = 0.0;
        ai = bsum / 30.0;
      } else {
        ar = tag_center_pos_est_re / 30.0;
        ai = bsum / 30.0;
      }
      bsum = ar - MeanA_re;
      s = ai - MeanA_im;
      if (s == 0.0) {
        tag_center_pos_est_re = bsum / 2.0;
      } else if (bsum == 0.0) {
        tag_center_pos_est_re = 0.0;
      } else {
        tag_center_pos_est_re = bsum / 2.0;
      }
      centerest_a_aver[59] = ar + tag_center_pos_est_re;
      if (s == 0.0) {
        bsum = 0.0;
      } else {
        bsum = s / 2.0;
      }
      centerest_a_aver[119] = ai + bsum;
      bsum = brm / 30.0;
      headingest_a_aver[59] = bsum + (bsum - b_y / 30.0) / 2.0;
    } else {
      centerest_a_aver[59] = tag_center_pos_est_tmp.re;
      centerest_a_aver[119] = tag_center_pos_est_tmp.im;
      headingest_a_aver[59] = heading_est_a[59];
    }
    //      tag_pos_est_aver =
    //      get_tag_pos(centerest_a_aver(end,1)+j*centerest_a_aver(end,2),
    //      headingest_a_aver(end), tag_pos_b);
    s = coder::b_mod(headingest_a_aver[59]);
    tag_center_pos_est_re = s * 0.0;
    if (s == 0.0) {
      tag_center_pos_est_re = std::exp(tag_center_pos_est_re);
      bsum = 0.0;
    } else {
      bsum = std::exp(tag_center_pos_est_re / 2.0);
      tag_center_pos_est_re = bsum * (bsum * std::cos(s));
      bsum *= bsum * std::sin(s);
    }
    MeanA_re = centerest_a_aver[119] * 0.0;
    // tag_pos_est_aver =
    // tag_pos_b*exp(j*(mod(headingest_a_aver(end),2*pi)))+mean(tag_pos_est);
    // [tag_pos_est, heading_est,tag_pos_est_aver,headingest_a_aver_v]
    UWBout[8] = PrevHeading;
    UWBout[0] = PrevPos[0].re;
    UWBout[4] = PrevPos[0].im;
    UWBout[9] =
        ((tag_pos_b[0].re * tag_center_pos_est_re - tag_pos_b[0].im * bsum) +
         centerest_a_aver[59]) +
        MeanA_re;
    UWBout[13] =
        (tag_pos_b[0].re * bsum + tag_pos_b[0].im * tag_center_pos_est_re) +
        centerest_a_aver[119];
    UWBout[1] = PrevPos[1].re;
    UWBout[5] = PrevPos[1].im;
    UWBout[10] =
        ((tag_pos_b[1].re * tag_center_pos_est_re - tag_pos_b[1].im * bsum) +
         centerest_a_aver[59]) +
        MeanA_re;
    UWBout[14] =
        (tag_pos_b[1].re * bsum + tag_pos_b[1].im * tag_center_pos_est_re) +
        centerest_a_aver[119];
    UWBout[2] = PrevPos[2].re;
    UWBout[6] = PrevPos[2].im;
    UWBout[11] =
        ((tag_pos_b[2].re * tag_center_pos_est_re - tag_pos_b[2].im * bsum) +
         centerest_a_aver[59]) +
        MeanA_re;
    UWBout[15] =
        (tag_pos_b[2].re * bsum + tag_pos_b[2].im * tag_center_pos_est_re) +
        centerest_a_aver[119];
    UWBout[3] = PrevPos[3].re;
    UWBout[7] = PrevPos[3].im;
    UWBout[12] =
        ((tag_pos_b[3].re * tag_center_pos_est_re - tag_pos_b[3].im * bsum) +
         centerest_a_aver[59]) +
        MeanA_re;
    UWBout[16] =
        (tag_pos_b[3].re * bsum + tag_pos_b[3].im * tag_center_pos_est_re) +
        centerest_a_aver[119];
    UWBout[17] = s;
    UWBout[18] = tag_center_vel_est_re;
    UWBout[19] = tag_center_vel_est_im;
    UWBout[20] = ErrSum;
  } else {
    UWBout[8] = PrevHeading;
    UWBout[0] = PrevPos[0].re;
    UWBout[4] = PrevPos[0].im;
    UWBout[9] = PrevPos[0].re;
    UWBout[13] = PrevPos[0].im;
    UWBout[1] = PrevPos[1].re;
    UWBout[5] = PrevPos[1].im;
    UWBout[10] = PrevPos[1].re;
    UWBout[14] = PrevPos[1].im;
    UWBout[2] = PrevPos[2].re;
    UWBout[6] = PrevPos[2].im;
    UWBout[11] = PrevPos[2].re;
    UWBout[15] = PrevPos[2].im;
    UWBout[3] = PrevPos[3].re;
    UWBout[7] = PrevPos[3].im;
    UWBout[12] = PrevPos[3].re;
    UWBout[16] = PrevPos[3].im;
    UWBout[17] = PrevHeading;
    UWBout[18] = 0.0;
    UWBout[19] = 0.0;
    UWBout[20] = 100.0;
  }
}

//
// [tag_pos_est, heading_est,tag_pos_est_aver,headingest_a_aver_v]
//
// Arguments    : void
// Return Type  : void
//
void UWBpos6_init()
{
  r = 0.0;
  std::memset(&heading_est_a[0], 0, 60U * sizeof(double));
  std::memset(&centerest_a[0], 0, 120U * sizeof(double));
  std::memset(&centerest_a_aver[0], 0, 120U * sizeof(double));
  std::memset(&headingest_a_aver[0], 0, 60U * sizeof(double));
  PrevHeading = 0.0;
}

//
// Arguments    : void
// Return Type  : void
//
void r_not_empty_init()
{
  r_not_empty = false;
}

//
// File trailer for UWBpos6.cpp
//
// [EOF]
//
