/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: TestTotal_data.c
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 01-Aug-2023 15:24:45
 */

/* Include Files */
#include "TestTotal_data.h"
#include "rt_nonfinite.h"

/* Variable Definitions */
unsigned int state[625];

double r;

creal_T InterpPosition[8];

creal_T Tag_Pos_List[72];

double TagDistInitCount[24];

double TagDistInit[24];

double heading_est_a[20];

double centerest_a[40];

double centerest_a_aver[40];

double headingest_a_aver[20];

const creal_T dcv[4] = {{
                            -0.5, /* re */
                            0.5   /* im */
                        },
                        {
                            0.5, /* re */
                            0.5  /* im */
                        },
                        {
                            -0.5, /* re */
                            -0.5  /* im */
                        },
                        {
                            0.5, /* re */
                            -0.5 /* im */
                        }};

bool isInitialized_TestTotal = false;

/*
 * File trailer for TestTotal_data.c
 *
 * [EOF]
 */
