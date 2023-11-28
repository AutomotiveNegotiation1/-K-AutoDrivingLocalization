//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: UWBPos6.cpp
//
// MATLAB Coder version            : 5.6
// C/C++ source code generated on  : 17-Oct-2023 17:18:56
//

// Include Files
#include "UWBPos6.h"
#include "GetInitPos4.h"
#include "UWBpos6_data.h"
#include "UWBpos6_types.h"
#include "combineVectorElements.h"
#include "mean.h"
#include "mod.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <algorithm>
#include <cmath>
#include <cstring>

// Function Declarations
static void UWBpos6_init(UWBPos6 *aInstancePtr);

// Function Definitions
//
// [tag_pos_est, heading_est,tag_pos_est_aver,headingest_a_aver_v]
//
// Arguments    : UWBPos6 *aInstancePtr
// Return Type  : void
//
static void UWBpos6_init(UWBPos6 *aInstancePtr)
{
  UWBpos6StackData *localSD;
  localSD = aInstancePtr->getStackData();
  localSD->pd->r = 0.0;
  std::memset(&localSD->pd->heading_est_a[0], 0, 60U * sizeof(double));
  std::memset(&localSD->pd->centerest_a[0], 0, 120U * sizeof(double));
  std::memset(&localSD->pd->centerest_a_aver[0], 0, 120U * sizeof(double));
  std::memset(&localSD->pd->headingest_a_aver[0], 0, 60U * sizeof(double));
  localSD->pd->PrevHeading = 0.0;
}

//
// Arguments    : void
// Return Type  : void
//
UWBPos6::UWBPos6()
{
  SD_.pd = &pd_;
  pd_.r_not_empty = false;
  UWBpos6_init(this);
  GetInitPos4_init(this);
}

//
// Arguments    : void
// Return Type  : void
//
UWBPos6::~UWBPos6() = default;

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
void UWBPos6::UWBpos6(double Ln, double Lp, double LnC, double TagNum,
                      double Nanchor, const double RxIDin[6],
                      const double RxDistin[6], double s_time,
                      const creal_T tag_pos_b[4], const double xain[6],
                      const double yain[6], const creal_T prevTagPos[4],
                      double prevTagHeading, double UWBout[21])
{
  coder::array<creal_T, 3U> r;
  coder::array<creal_T, 3U> x;
  coder::array<double, 2U> DistPredicted;
  coder::array<double, 2U> y;
  double xain_data[6];
  double yain_data[6];
  double ErrSum;
  double tag_center_pos_est_re;
  int xain_size[2];
  int yain_size[2];
  int firstBlockLength;
  int lastBlockLength;
  int loop_ub_tmp;
  int nblocks;
  int npages;
  int xi;
  if (!pd_.r_not_empty) {
    pd_.r_not_empty = true;
    loop_ub_tmp = static_cast<int>(Lp);
    pd_.Tag_Pos_List.set_size(16, 2, loop_ub_tmp);
    firstBlockLength = static_cast<int>(Lp) << 5;
    for (xi = 0; xi < firstBlockLength; xi++) {
      pd_.Tag_Pos_List[xi].re = 0.0;
      pd_.Tag_Pos_List[xi].im = 0.0;
    }
    pd_.DistList.set_size(static_cast<int>(Ln), loop_ub_tmp, 30);
    npages = static_cast<int>(Ln) * static_cast<int>(Lp) * 30;
    pd_.DistListTime.set_size(static_cast<int>(Ln), loop_ub_tmp, 30);
    for (xi = 0; xi < npages; xi++) {
      pd_.DistList[xi] = 0.0;
      pd_.DistListTime[xi] = 0.0;
    }
    pd_.PrevPos.set_size(1, loop_ub_tmp);
    for (xi = 0; xi < loop_ub_tmp; xi++) {
      pd_.PrevPos[xi].re = 0.0;
      pd_.PrevPos[xi].im = 0.0;
    }
  }
  if (pd_.r < 1000.0) {
    pd_.r++;
  }
  if (LnC < 1.0) {
    loop_ub_tmp = 0;
  } else {
    loop_ub_tmp = static_cast<int>(LnC);
  }
  xi = static_cast<int>(Nanchor);
  for (npages = 0; npages < xi; npages++) {
    double dv[29];
    // if RxDistin(q)<20
    tag_center_pos_est_re = RxIDin[npages];
    for (lastBlockLength = 0; lastBlockLength < 29; lastBlockLength++) {
      dv[lastBlockLength] =
          pd_.DistList[((static_cast<int>(tag_center_pos_est_re) +
                         pd_.DistList.size(0) *
                             (static_cast<int>(TagNum) - 1)) +
                        pd_.DistList.size(0) * pd_.DistList.size(1) *
                            (lastBlockLength + 1)) -
                       1];
    }
    pd_.DistList[((static_cast<int>(tag_center_pos_est_re) +
                   pd_.DistList.size(0) * (static_cast<int>(TagNum) - 1)) +
                  pd_.DistList.size(0) * pd_.DistList.size(1) * 29) -
                 1] = RxDistin[npages];
    for (lastBlockLength = 0; lastBlockLength < 29; lastBlockLength++) {
      pd_.DistList[((static_cast<int>(tag_center_pos_est_re) +
                     pd_.DistList.size(0) * (static_cast<int>(TagNum) - 1)) +
                    pd_.DistList.size(0) * pd_.DistList.size(1) *
                        lastBlockLength) -
                   1] = dv[lastBlockLength];
      dv[lastBlockLength] =
          pd_.DistListTime
              [((static_cast<int>(tag_center_pos_est_re) +
                 pd_.DistListTime.size(0) * (static_cast<int>(TagNum) - 1)) +
                pd_.DistListTime.size(0) * pd_.DistListTime.size(1) *
                    (lastBlockLength + 1)) -
               1];
    }
    for (lastBlockLength = 0; lastBlockLength < 29; lastBlockLength++) {
      pd_.DistListTime[((static_cast<int>(tag_center_pos_est_re) +
                         pd_.DistListTime.size(0) *
                             (static_cast<int>(TagNum) - 1)) +
                        pd_.DistListTime.size(0) * pd_.DistListTime.size(1) *
                            lastBlockLength) -
                       1] = dv[lastBlockLength];
    }
    pd_.DistListTime
        [((static_cast<int>(tag_center_pos_est_re) +
           pd_.DistListTime.size(0) * (static_cast<int>(TagNum) - 1)) +
          pd_.DistListTime.size(0) * pd_.DistListTime.size(1) * 29) -
         1] = s_time;
    // end
  }
  xi = static_cast<int>(Ln);
  lastBlockLength = static_cast<int>(Lp);
  DistPredicted.set_size(static_cast<int>(Ln), static_cast<int>(Lp));
  for (npages = 0; npages < xi; npages++) {
    for (firstBlockLength = 0; firstBlockLength < lastBlockLength;
         firstBlockLength++) {
      for (nblocks = 0; nblocks < 30; nblocks++) {
        if (s_time - pd_.DistListTime[(npages + pd_.DistListTime.size(0) *
                                                    firstBlockLength) +
                                      pd_.DistListTime.size(0) *
                                          pd_.DistListTime.size(1) * nblocks] >
            2.0) {
          pd_.DistList[(npages + pd_.DistList.size(0) * firstBlockLength) +
                       pd_.DistList.size(0) * pd_.DistList.size(1) * nblocks] =
              0.0;
          pd_.DistListTime[(npages +
                            pd_.DistListTime.size(0) * firstBlockLength) +
                           pd_.DistListTime.size(0) * pd_.DistListTime.size(1) *
                               nblocks] = 0.0;
        }
      }
      if (s_time - pd_.DistListTime[(npages + pd_.DistListTime.size(0) *
                                                  firstBlockLength) +
                                    pd_.DistListTime.size(0) *
                                        pd_.DistListTime.size(1) * 29] <
          0.3) {
        DistPredicted[npages + DistPredicted.size(0) * firstBlockLength] =
            pd_.DistList[(npages + pd_.DistList.size(0) * firstBlockLength) +
                         pd_.DistList.size(0) * pd_.DistList.size(1) * 29];
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
    double bsum;
    double tag_center_pos_est_im;
    double tag_center_vel_est_im;
    double tag_center_vel_est_re;
    int k;
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
        nblocks = static_cast<int>(
            static_cast<unsigned int>(DistPredicted.size(0)) >> 10);
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
        for (k = 2; k <= firstBlockLength; k++) {
          y[xi] = y[xi] + DistPredicted[(xpageoffset + k) - 1];
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
          for (k = 2; k <= hi; k++) {
            bsum += DistPredicted[(xblockoffset + k) - 1];
          }
          y[xi] = y[xi] + bsum;
        }
      }
    }
    firstBlockLength = y.size(1);
    if (y.size(1) <= 2) {
      if (y.size(1) == 1) {
        bsum = y[0];
      } else {
        bsum = y[y.size(1) - 1];
        if ((!(y[0] > bsum)) && ((!std::isnan(y[0])) || std::isnan(bsum))) {
          bsum = y[0];
        }
      }
    } else {
      if (!std::isnan(y[0])) {
        npages = 1;
      } else {
        bool exitg1;
        npages = 0;
        k = 2;
        exitg1 = false;
        while ((!exitg1) && (k <= firstBlockLength)) {
          if (!std::isnan(y[k - 1])) {
            npages = k;
            exitg1 = true;
          } else {
            k++;
          }
        }
      }
      if (npages == 0) {
        bsum = y[0];
      } else {
        bsum = y[npages - 1];
        xi = npages + 1;
        for (k = xi; k <= firstBlockLength; k++) {
          tag_center_pos_est_re = y[k - 1];
          if (bsum > tag_center_pos_est_re) {
            bsum = tag_center_pos_est_re;
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
      xain_size[0] = 1;
      xain_size[1] = loop_ub_tmp;
      yain_size[0] = 1;
      yain_size[1] = loop_ub_tmp;
      if (loop_ub_tmp - 1 >= 0) {
        std::copy(&xain[0], &xain[loop_ub_tmp], &xain_data[0]);
        std::copy(&yain[0], &yain[loop_ub_tmp], &yain_data[0]);
      }
      npages = DistPredicted.size(1);
      for (xi = 0; xi < npages; xi++) {
        for (lastBlockLength = 0; lastBlockLength < loop_ub_tmp;
             lastBlockLength++) {
          DistPredicted[lastBlockLength + loop_ub_tmp * xi] =
              DistPredicted[lastBlockLength + DistPredicted.size(0) * xi];
        }
      }
      creal_T tag_pos_est_aver[4];
      DistPredicted.set_size(loop_ub_tmp, DistPredicted.size(1));
      pd_.PrevHeading = GetInitPos4(
          this, xain_data, xain_size, yain_data, yain_size, DistPredicted,
          tag_pos_b, LnC, prevTagPos, prevTagHeading, tag_pos_est_aver, ErrSum);
      pd_.PrevPos.set_size(1, 4);
      pd_.PrevPos[0] = tag_pos_est_aver[0];
      pd_.PrevPos[1] = tag_pos_est_aver[1];
      pd_.PrevPos[2] = tag_pos_est_aver[2];
      pd_.PrevPos[3] = tag_pos_est_aver[3];
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
    r.set_size(15, 2, pd_.Tag_Pos_List.size(2));
    firstBlockLength = pd_.Tag_Pos_List.size(2);
    for (xi = 0; xi < firstBlockLength; xi++) {
      for (lastBlockLength = 0; lastBlockLength < 2; lastBlockLength++) {
        for (npages = 0; npages < 15; npages++) {
          r[(npages + 15 * lastBlockLength) + 30 * xi] =
              pd_.Tag_Pos_List[((npages + 16 * lastBlockLength) + 32 * xi) + 1];
        }
      }
    }
    firstBlockLength = r.size(2);
    for (xi = 0; xi < firstBlockLength; xi++) {
      for (lastBlockLength = 0; lastBlockLength < 2; lastBlockLength++) {
        for (npages = 0; npages < 15; npages++) {
          pd_.Tag_Pos_List[(npages + 16 * lastBlockLength) + 32 * xi] =
              r[(npages + 15 * lastBlockLength) + 30 * xi];
        }
      }
    }
    pd_.Tag_Pos_List[15].re = s_time;
    pd_.Tag_Pos_List[15].im = 0.0;
    pd_.Tag_Pos_List[31] = pd_.PrevPos[0];
    pd_.Tag_Pos_List[47].re = s_time;
    pd_.Tag_Pos_List[47].im = 0.0;
    pd_.Tag_Pos_List[63] = pd_.PrevPos[1];
    pd_.Tag_Pos_List[79].re = s_time;
    pd_.Tag_Pos_List[79].im = 0.0;
    pd_.Tag_Pos_List[95] = pd_.PrevPos[2];
    pd_.Tag_Pos_List[111].re = s_time;
    pd_.Tag_Pos_List[111].im = 0.0;
    pd_.Tag_Pos_List[127] = pd_.PrevPos[3];
    tag_center_pos_est_tmp = coder::mean(pd_.PrevPos);
    x.set_size(1, 1, pd_.Tag_Pos_List.size(2));
    firstBlockLength = pd_.Tag_Pos_List.size(2);
    for (xi = 0; xi < firstBlockLength; xi++) {
      x[xi].re =
          pd_.Tag_Pos_List[32 * xi + 11].re - pd_.Tag_Pos_List[32 * xi].re;
      x[xi].im =
          pd_.Tag_Pos_List[32 * xi + 11].im - pd_.Tag_Pos_List[32 * xi].im;
    }
    dc = coder::combineVectorElements(x);
    if ((dc.re != 0.0) || (dc.im != 0.0)) {
      x.set_size(1, 1, pd_.Tag_Pos_List.size(2));
      firstBlockLength = pd_.Tag_Pos_List.size(2);
      for (xi = 0; xi < firstBlockLength; xi++) {
        ar = pd_.Tag_Pos_List[32 * xi + 27].re -
             pd_.Tag_Pos_List[32 * xi + 16].re;
        ai = pd_.Tag_Pos_List[32 * xi + 27].im -
             pd_.Tag_Pos_List[32 * xi + 16].im;
        tag_center_pos_est_re =
            pd_.Tag_Pos_List[32 * xi + 11].re - pd_.Tag_Pos_List[32 * xi].re;
        MeanA_re =
            pd_.Tag_Pos_List[32 * xi + 11].im - pd_.Tag_Pos_List[32 * xi].im;
        if (MeanA_re == 0.0) {
          if (ai == 0.0) {
            x[xi].re = ar / tag_center_pos_est_re;
            x[xi].im = 0.0;
          } else if (ar == 0.0) {
            x[xi].re = 0.0;
            x[xi].im = ai / tag_center_pos_est_re;
          } else {
            x[xi].re = ar / tag_center_pos_est_re;
            x[xi].im = ai / tag_center_pos_est_re;
          }
        } else if (tag_center_pos_est_re == 0.0) {
          if (ar == 0.0) {
            x[xi].re = ai / MeanA_re;
            x[xi].im = 0.0;
          } else if (ai == 0.0) {
            x[xi].re = 0.0;
            x[xi].im = -(ar / MeanA_re);
          } else {
            x[xi].re = ai / MeanA_re;
            x[xi].im = -(ar / MeanA_re);
          }
        } else {
          MeanA_im = std::abs(tag_center_pos_est_re);
          bsum = std::abs(MeanA_re);
          if (MeanA_im > bsum) {
            tag_center_pos_est_im = MeanA_re / tag_center_pos_est_re;
            bsum = tag_center_pos_est_re + tag_center_pos_est_im * MeanA_re;
            x[xi].re = (ar + tag_center_pos_est_im * ai) / bsum;
            x[xi].im = (ai - tag_center_pos_est_im * ar) / bsum;
          } else if (bsum == MeanA_im) {
            if (tag_center_pos_est_re > 0.0) {
              tag_center_pos_est_im = 0.5;
            } else {
              tag_center_pos_est_im = -0.5;
            }
            if (MeanA_re > 0.0) {
              bsum = 0.5;
            } else {
              bsum = -0.5;
            }
            x[xi].re = (ar * tag_center_pos_est_im + ai * bsum) / MeanA_im;
            x[xi].im = (ai * tag_center_pos_est_im - ar * bsum) / MeanA_im;
          } else {
            tag_center_pos_est_im = tag_center_pos_est_re / MeanA_re;
            bsum = MeanA_re + tag_center_pos_est_im * tag_center_pos_est_re;
            x[xi].re = (tag_center_pos_est_im * ar + ai) / bsum;
            x[xi].im = (tag_center_pos_est_im * ai - ar) / bsum;
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
      pd_.heading_est_a[xi] = pd_.heading_est_a[xi + 1];
    }
    tag_center_pos_est_re = pd_.PrevHeading - pd_.heading_est_a[58];
    bsum = std::round(std::abs(tag_center_pos_est_re) / 6.2831853071795862);
    if (bsum < 2.147483648E+9) {
      xi = static_cast<int>(bsum);
    } else if (bsum >= 2.147483648E+9) {
      xi = MAX_int32_T;
    } else {
      xi = 0;
    }
    bsum = std::round(6.2831853071795862 * static_cast<double>(xi));
    if (bsum < 2.147483648E+9) {
      xi = static_cast<int>(bsum);
    } else {
      xi = MAX_int32_T;
    }
    tag_center_pos_est_im =
        std::round(pd_.PrevHeading - static_cast<double>(xi));
    if (tag_center_pos_est_im < 2.147483648E+9) {
      if (tag_center_pos_est_im >= -2.147483648E+9) {
        xi = static_cast<int>(tag_center_pos_est_im);
      } else {
        xi = MIN_int32_T;
      }
    } else if (tag_center_pos_est_im >= 2.147483648E+9) {
      xi = MAX_int32_T;
    } else {
      xi = 0;
    }
    if (bsum < 2.147483648E+9) {
      lastBlockLength = static_cast<int>(bsum);
    } else {
      lastBlockLength = MAX_int32_T;
    }
    bsum = std::round(pd_.PrevHeading + static_cast<double>(lastBlockLength));
    if (bsum < 2.147483648E+9) {
      if (bsum >= -2.147483648E+9) {
        lastBlockLength = static_cast<int>(bsum);
      } else {
        lastBlockLength = MIN_int32_T;
      }
    } else if (bsum >= 2.147483648E+9) {
      lastBlockLength = MAX_int32_T;
    } else {
      lastBlockLength = 0;
    }
    if (tag_center_pos_est_re > 3.1415926535897931) {
      pd_.heading_est_a[59] = xi;
    } else if (pd_.heading_est_a[58] - pd_.PrevHeading > 3.1415926535897931) {
      pd_.heading_est_a[59] = lastBlockLength;
    } else {
      pd_.heading_est_a[59] = pd_.PrevHeading;
    }
    for (xi = 0; xi < 59; xi++) {
      pd_.centerest_a[xi] = pd_.centerest_a[xi + 1];
      pd_.centerest_a_aver[xi] = pd_.centerest_a_aver[xi + 1];
      pd_.centerest_a[xi + 60] = pd_.centerest_a[xi + 61];
      pd_.centerest_a_aver[xi + 60] = pd_.centerest_a_aver[xi + 61];
      pd_.headingest_a_aver[xi] = pd_.headingest_a_aver[xi + 1];
    }
    pd_.centerest_a[59] = tag_center_pos_est_tmp.re;
    pd_.centerest_a[119] = tag_center_pos_est_tmp.im;
    if (pd_.r > 60.0) {
      creal_T b_x[30];
      double b_y;
      double im;
      for (xi = 0; xi < 30; xi++) {
        tag_center_pos_est_re = pd_.centerest_a[xi + 60];
        b_x[xi].re = pd_.centerest_a[xi] + 0.0 * tag_center_pos_est_re;
        b_x[xi].im = tag_center_pos_est_re;
      }
      tag_center_pos_est_re = b_x[0].re;
      tag_center_pos_est_im = b_x[0].im;
      for (k = 0; k < 29; k++) {
        tag_center_pos_est_re += b_x[k + 1].re;
        tag_center_pos_est_im += b_x[k + 1].im;
      }
      if (tag_center_pos_est_im == 0.0) {
        MeanA_re = tag_center_pos_est_re / 30.0;
        MeanA_im = 0.0;
      } else if (tag_center_pos_est_re == 0.0) {
        MeanA_re = 0.0;
        MeanA_im = tag_center_pos_est_im / 30.0;
      } else {
        MeanA_re = tag_center_pos_est_re / 30.0;
        MeanA_im = tag_center_pos_est_im / 30.0;
      }
      for (xi = 0; xi < 30; xi++) {
        tag_center_pos_est_re = pd_.centerest_a[xi + 90];
        b_x[xi].re = pd_.centerest_a[xi + 30] + 0.0 * tag_center_pos_est_re;
        b_x[xi].im = tag_center_pos_est_re;
      }
      tag_center_pos_est_re = b_x[0].re;
      tag_center_pos_est_im = b_x[0].im;
      b_y = pd_.heading_est_a[0];
      ar = pd_.heading_est_a[30];
      for (k = 0; k < 29; k++) {
        tag_center_pos_est_re += b_x[k + 1].re;
        tag_center_pos_est_im += b_x[k + 1].im;
        b_y += pd_.heading_est_a[k + 1];
        ar += pd_.heading_est_a[k + 31];
      }
      if (tag_center_pos_est_im == 0.0) {
        ai = tag_center_pos_est_re / 30.0;
        im = 0.0;
      } else if (tag_center_pos_est_re == 0.0) {
        ai = 0.0;
        im = tag_center_pos_est_im / 30.0;
      } else {
        ai = tag_center_pos_est_re / 30.0;
        im = tag_center_pos_est_im / 30.0;
      }
      bsum = ai - MeanA_re;
      tag_center_pos_est_im = im - MeanA_im;
      if (tag_center_pos_est_im == 0.0) {
        tag_center_pos_est_re = bsum / 2.0;
      } else if (bsum == 0.0) {
        tag_center_pos_est_re = 0.0;
      } else {
        tag_center_pos_est_re = bsum / 2.0;
      }
      pd_.centerest_a_aver[59] = ai + tag_center_pos_est_re;
      if (tag_center_pos_est_im == 0.0) {
        tag_center_pos_est_im = 0.0;
      } else {
        tag_center_pos_est_im /= 2.0;
      }
      pd_.centerest_a_aver[119] = im + tag_center_pos_est_im;
      bsum = ar / 30.0;
      pd_.headingest_a_aver[59] = bsum + (bsum - b_y / 30.0) / 2.0;
    } else {
      pd_.centerest_a_aver[59] = tag_center_pos_est_tmp.re;
      pd_.centerest_a_aver[119] = tag_center_pos_est_tmp.im;
      pd_.headingest_a_aver[59] = pd_.heading_est_a[59];
    }
    //      tag_pos_est_aver =
    //      get_tag_pos(centerest_a_aver(end,1)+j*centerest_a_aver(end,2),
    //      headingest_a_aver(end), tag_pos_b);
    bsum = coder::b_mod(pd_.headingest_a_aver[59]);
    if (bsum * 0.0 == 0.0) {
      tag_center_pos_est_re = std::cos(bsum);
      tag_center_pos_est_im = std::sin(bsum);
    } else if (bsum == 0.0) {
      tag_center_pos_est_re = rtNaN;
      tag_center_pos_est_im = 0.0;
    } else {
      tag_center_pos_est_re = rtNaN;
      tag_center_pos_est_im = rtNaN;
    }
    MeanA_re = pd_.centerest_a_aver[119] * 0.0;
    // tag_pos_est_aver =
    // tag_pos_b*exp(j*(mod(headingest_a_aver(end),2*pi)))+mean(tag_pos_est);
    // [tag_pos_est, heading_est,tag_pos_est_aver,headingest_a_aver_v]
    UWBout[8] = pd_.PrevHeading;
    UWBout[0] = pd_.PrevPos[0].re;
    UWBout[4] = pd_.PrevPos[0].im;
    UWBout[9] = ((tag_pos_b[0].re * tag_center_pos_est_re -
                  tag_pos_b[0].im * tag_center_pos_est_im) +
                 pd_.centerest_a_aver[59]) +
                MeanA_re;
    UWBout[13] = (tag_pos_b[0].re * tag_center_pos_est_im +
                  tag_pos_b[0].im * tag_center_pos_est_re) +
                 pd_.centerest_a_aver[119];
    UWBout[1] = pd_.PrevPos[1].re;
    UWBout[5] = pd_.PrevPos[1].im;
    UWBout[10] = ((tag_pos_b[1].re * tag_center_pos_est_re -
                   tag_pos_b[1].im * tag_center_pos_est_im) +
                  pd_.centerest_a_aver[59]) +
                 MeanA_re;
    UWBout[14] = (tag_pos_b[1].re * tag_center_pos_est_im +
                  tag_pos_b[1].im * tag_center_pos_est_re) +
                 pd_.centerest_a_aver[119];
    UWBout[2] = pd_.PrevPos[2].re;
    UWBout[6] = pd_.PrevPos[2].im;
    UWBout[11] = ((tag_pos_b[2].re * tag_center_pos_est_re -
                   tag_pos_b[2].im * tag_center_pos_est_im) +
                  pd_.centerest_a_aver[59]) +
                 MeanA_re;
    UWBout[15] = (tag_pos_b[2].re * tag_center_pos_est_im +
                  tag_pos_b[2].im * tag_center_pos_est_re) +
                 pd_.centerest_a_aver[119];
    UWBout[3] = pd_.PrevPos[3].re;
    UWBout[7] = pd_.PrevPos[3].im;
    UWBout[12] = ((tag_pos_b[3].re * tag_center_pos_est_re -
                   tag_pos_b[3].im * tag_center_pos_est_im) +
                  pd_.centerest_a_aver[59]) +
                 MeanA_re;
    UWBout[16] = (tag_pos_b[3].re * tag_center_pos_est_im +
                  tag_pos_b[3].im * tag_center_pos_est_re) +
                 pd_.centerest_a_aver[119];
    UWBout[17] = bsum;
    UWBout[18] = tag_center_vel_est_re;
    UWBout[19] = tag_center_vel_est_im;
    UWBout[20] = ErrSum;
  } else {
    UWBout[8] = pd_.PrevHeading;
    UWBout[0] = pd_.PrevPos[0].re;
    UWBout[4] = pd_.PrevPos[0].im;
    UWBout[9] = pd_.PrevPos[0].re;
    UWBout[13] = pd_.PrevPos[0].im;
    UWBout[1] = pd_.PrevPos[1].re;
    UWBout[5] = pd_.PrevPos[1].im;
    UWBout[10] = pd_.PrevPos[1].re;
    UWBout[14] = pd_.PrevPos[1].im;
    UWBout[2] = pd_.PrevPos[2].re;
    UWBout[6] = pd_.PrevPos[2].im;
    UWBout[11] = pd_.PrevPos[2].re;
    UWBout[15] = pd_.PrevPos[2].im;
    UWBout[3] = pd_.PrevPos[3].re;
    UWBout[7] = pd_.PrevPos[3].im;
    UWBout[12] = pd_.PrevPos[3].re;
    UWBout[16] = pd_.PrevPos[3].im;
    UWBout[17] = pd_.PrevHeading;
    UWBout[18] = 0.0;
    UWBout[19] = 0.0;
    UWBout[20] = 100.0;
  }
}

//
// Arguments    : void
// Return Type  : UWBpos6StackData *
//
UWBpos6StackData *UWBPos6::getStackData()
{
  return &SD_;
}

//
// File trailer for UWBPos6.cpp
//
// [EOF]
//
