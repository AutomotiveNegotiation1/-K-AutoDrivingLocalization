/*
 * TestTotal_data.c
 *
 * Code generation for function 'TestTotal_data'
 *
 */

/* Include files */
#include "TestTotal_data.h"
#include "rt_nonfinite.h"

/* Variable Definitions */
unsigned int state[625];

double freq;

boolean_T freq_not_empty;

double r;

creal_T InterpPosition[8];

creal_T Tag_Pos_List[96];

double TagDistInitCount[24];

double TagDistInit[24];

double heading_est_a[20];

double centerest_a[40];

double centerest_a_aver[40];

double headingest_a_aver[20];

double RxIDprev[24];

double PPprev[4];

double RxIDprevLen[4];

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

boolean_T isInitialized_TestTotal = false;

/* End of code generation (TestTotal_data.c) */
