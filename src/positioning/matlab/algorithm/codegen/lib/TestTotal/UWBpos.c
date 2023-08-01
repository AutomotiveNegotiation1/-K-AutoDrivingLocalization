/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: UWBpos.c
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 01-Aug-2023 15:24:45
 */

/* Include Files */
#include "UWBpos.h"
#include "TestTotal_data.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Definitions */
/*
 * Arguments    : void
 * Return Type  : void
 */
void UWBpos_init(void)
{
  r = 0.0;
  memset(&InterpPosition[0], 0, 8U * sizeof(creal_T));
  memset(&Tag_Pos_List[0], 0, 72U * sizeof(creal_T));
  memset(&TagDistInitCount[0], 0, 24U * sizeof(double));
  memset(&TagDistInit[0], 0, 24U * sizeof(double));
  memset(&heading_est_a[0], 0, 20U * sizeof(double));
  memset(&centerest_a[0], 0, 40U * sizeof(double));
  memset(&centerest_a_aver[0], 0, 40U * sizeof(double));
  memset(&headingest_a_aver[0], 0, 20U * sizeof(double));
}

/*
 * File trailer for UWBpos.c
 *
 * [EOF]
 */
