//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: EulerKalman_2.h
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 30-Nov-2023 16:43:53
//

#ifndef EULERKALMAN_2_H
#define EULERKALMAN_2_H

// Include Files
#include "rtwtypes.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
extern void EulerKalman_2(double x[4], const double A[16], const double z[4],
                          double *phi, double *theta, double *psi);

void EulerKalman_2_init();

#endif
//
// File trailer for EulerKalman_2.h
//
// [EOF]
//
