//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: UWBpos6_types.h
//
// MATLAB Coder version            : 5.6
// C/C++ source code generated on  : 17-Oct-2023 17:18:56
//

#ifndef UWBPOS6_TYPES_H
#define UWBPOS6_TYPES_H

// Include Files
#include "rtwtypes.h"
#include "coder_array.h"

// Type Definitions
struct UWBpos6PersistentData {
  double r;
  bool r_not_empty;
  coder::array<creal_T, 3U> Tag_Pos_List;
  double heading_est_a[60];
  double centerest_a[120];
  double centerest_a_aver[120];
  double headingest_a_aver[60];
  coder::array<double, 3U> DistList;
  coder::array<double, 3U> DistListTime;
  coder::array<creal_T, 2U> PrevPos;
  double PrevHeading;
  creal_T PrevUWBpos[4];
};

struct UWBpos6StackData {
  UWBpos6PersistentData *pd;
};

#endif
//
// File trailer for UWBpos6_types.h
//
// [EOF]
//
