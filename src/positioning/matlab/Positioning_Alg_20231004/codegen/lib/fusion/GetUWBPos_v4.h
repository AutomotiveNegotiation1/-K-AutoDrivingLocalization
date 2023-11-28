//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: GetUWBPos_v4.h
//
// MATLAB Coder version            : 5.6
// C/C++ source code generated on  : 17-Oct-2023 17:24:31
//

#ifndef GETUWBPOS_V4_H
#define GETUWBPOS_V4_H

// Include Files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
double GetUWBPos_v4(const double xa_data[], const double ya_data[],
                    const coder::array<double, 2U> &dist, double Ln,
                    const creal_T tag_pos_b[4], double &Yc, double &heading,
                    double &flag);

#endif
//
// File trailer for GetUWBPos_v4.h
//
// [EOF]
//
