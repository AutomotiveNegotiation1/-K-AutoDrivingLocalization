//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: UWBpos6.h
//
// MATLAB Coder version            : 5.6
// C/C++ source code generated on  : 17-Oct-2023 17:24:31
//

#ifndef UWBPOS6_H
#define UWBPOS6_H

// Include Files
#include "rtwtypes.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
extern void UWBpos6(double Ln, double Lp, double LnC, double TagNum,
                    double Nanchor, const double RxIDin[6],
                    const double RxDistin[6], double s_time,
                    const creal_T tag_pos_b[4], const double xain[6],
                    const double yain[6], const creal_T prevTagPos[4],
                    double prevTagHeading, double UWBout[21]);

void UWBpos6_init();

void r_not_empty_init();

#endif
//
// File trailer for UWBpos6.h
//
// [EOF]
//
