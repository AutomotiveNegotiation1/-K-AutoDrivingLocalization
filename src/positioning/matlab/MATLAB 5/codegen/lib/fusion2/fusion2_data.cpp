//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: fusion2_data.cpp
//
// MATLAB Coder version            : 5.6
// C/C++ source code generated on  : 09-Nov-2023 17:48:28
//

// Include Files
#include "fusion2_data.h"
#include "rt_nonfinite.h"

// Variable Definitions
double b_H[36];

double b_Q[144];

double b_P[144];

const double dv[9]{0.01, 0.0, 0.0, 0.0, 0.01, 0.0, 0.0, 0.0, 0.01};

const signed char iv[144]{
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};

const signed char iv1[9]{0, 1, 0, 1, 0, 0, 0, 0, 1};

bool isInitialized_fusion2{false};

//
// File trailer for fusion2_data.cpp
//
// [EOF]
//
