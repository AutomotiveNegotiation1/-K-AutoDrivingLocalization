//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: fusion2_initialize.cpp
//
// MATLAB Coder version            : 5.6
// C/C++ source code generated on  : 09-Nov-2023 17:48:28
//

// Include Files
#include "fusion2_initialize.h"
#include "EulerKalman_2.h"
#include "GetInitPos4.h"
#include "IMUpos.h"
#include "PosKalman2.h"
#include "UWBpos6.h"
#include "fusion2.h"
#include "fusion2_data.h"
#include "rt_nonfinite.h"

// Function Definitions
//
// Arguments    : void
// Return Type  : void
//
void fusion2_initialize()
{
  r_not_empty_init();
  fusion2_init();
  EulerKalman_2_init();
  PosKalman2_init();
  IMUpos_init();
  UWBpos6_init();
  GetInitPos4_init();
  isInitialized_fusion2 = true;
}

//
// File trailer for fusion2_initialize.cpp
//
// [EOF]
//
