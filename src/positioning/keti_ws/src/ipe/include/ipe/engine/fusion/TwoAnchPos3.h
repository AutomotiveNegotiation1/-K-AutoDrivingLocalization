//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: TwoAnchPos3.h
//
// MATLAB Coder version            : 5.6
// C/C++ source code generated on  : 17-Oct-2023 17:24:31
//

#ifndef TWOANCHPOS3_H
#define TWOANCHPOS3_H

// Include Files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
void TwoAnchPos3(const double Xa[2], const double Ya[2], const double dist[2],
                 const creal_T anch_pos_data[], const int anch_pos_size[2],
                 const coder::array<double, 1U> &dist_a, double Pos[4],
                 double Prob[2]);

#endif
//
// File trailer for TwoAnchPos3.h
//
// [EOF]
//
