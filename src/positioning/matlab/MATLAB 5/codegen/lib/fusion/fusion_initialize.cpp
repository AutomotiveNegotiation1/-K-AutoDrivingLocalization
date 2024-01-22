//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: fusion_initialize.cpp
//
// MATLAB Coder version            : 5.6
// C/C++ source code generated on  : 17-Oct-2023 17:24:31
//

// Include Files
#include "fusion_initialize.h"
#include "EulerKalman_2.h"
#include "GetInitPos4.h"
#include "IMUpos.h"
#include "PosKalman2.h"
#include "UWBpos6.h"
#include "fusion.h"
#include "fusion_data.h"
#include "rt_nonfinite.h"

// Function Definitions
//
// Arguments    : void
// Return Type  : void
//
void fusion_initialize()
{
  r_not_empty_init();
  fusion_init();
  EulerKalman_2_init();
  PosKalman2_init();
  IMUpos_init();
  UWBpos6_init();
  GetInitPos4_init();
  isInitialized_fusion = true;
}

//
// File trailer for fusion_initialize.cpp
//
// [EOF]
//
