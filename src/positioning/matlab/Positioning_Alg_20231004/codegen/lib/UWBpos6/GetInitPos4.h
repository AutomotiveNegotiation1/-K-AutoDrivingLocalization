//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: GetInitPos4.h
//
// MATLAB Coder version            : 5.6
// C/C++ source code generated on  : 17-Oct-2023 17:18:56
//

#ifndef GETINITPOS4_H
#define GETINITPOS4_H

// Include Files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Type Declarations
class UWBPos6;

// Function Declarations
double GetInitPos4(UWBPos6 *aInstancePtr, const double xa_data[],
                   const int xa_size[2], const double ya_data[],
                   const int ya_size[2], const coder::array<double, 2U> &dist,
                   const creal_T tag_pos_b[4], double Ln,
                   const creal_T PrevPos[4], double PrevHeading,
                   creal_T tag_pos_est[4], double &ErrSum);

void GetInitPos4_init(UWBPos6 *aInstancePtr);

#endif
//
// File trailer for GetInitPos4.h
//
// [EOF]
//
