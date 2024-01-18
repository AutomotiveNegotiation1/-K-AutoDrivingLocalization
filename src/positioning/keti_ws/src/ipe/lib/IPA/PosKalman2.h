//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: PosKalman2.h
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 30-Nov-2023 16:43:53
//

#ifndef POSKALMAN2_H
#define POSKALMAN2_H

// Include Files
#include "rtwtypes.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
extern void PosKalman2(double x[12], const double A[144], const double z[3],
                       const double b_R[9]);

void PosKalman2_init();

#endif
//
// File trailer for PosKalman2.h
//
// [EOF]
//
