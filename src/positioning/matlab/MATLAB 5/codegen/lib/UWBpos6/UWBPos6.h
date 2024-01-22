//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: UWBPos6.h
//
// MATLAB Coder version            : 5.6
// C/C++ source code generated on  : 17-Oct-2023 17:18:56
//

#ifndef UWBPOS6_H
#define UWBPOS6_H

// Include Files
#include "UWBpos6_types.h"
#include "rtwtypes.h"
#include <cstddef>
#include <cstdlib>

// Type Definitions
class UWBPos6 {
public:
  UWBPos6();
  ~UWBPos6();
  void UWBpos6(double Ln, double Lp, double LnC, double TagNum, double Nanchor,
               const double RxIDin[6], const double RxDistin[6], double s_time,
               const creal_T tag_pos_b[4], const double xain[6],
               const double yain[6], const creal_T prevTagPos[4],
               double prevTagHeading, double UWBout[21]);
  UWBpos6StackData *getStackData();

private:
  UWBpos6PersistentData pd_;
  UWBpos6StackData SD_;
};

#endif
//
// File trailer for UWBPos6.h
//
// [EOF]
//
