//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: sort.h
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 30-Nov-2023 16:43:53
//

#ifndef SORT_H
#define SORT_H

// Include Files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
namespace coder {
namespace internal {
void sort(::coder::array<double, 1U> &x, ::coder::array<int, 1U> &idx);

void sort(double x[16], int idx[16]);

} // namespace internal
} // namespace coder

#endif
//
// File trailer for sort.h
//
// [EOF]
//
