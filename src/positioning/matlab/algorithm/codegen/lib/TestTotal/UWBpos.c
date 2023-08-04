/*
 * UWBpos.c
 *
 * Code generation for function 'UWBpos'
 *
 */

/* Include files */
#include "UWBpos.h"
#include "TestTotal_data.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Definitions */
void UWBpos_init(void)
{
  r = 0.0;
  memset(&InterpPosition[0], 0, 8U * sizeof(creal_T));
  memset(&Tag_Pos_List[0], 0, 96U * sizeof(creal_T));
  memset(&TagDistInitCount[0], 0, 24U * sizeof(double));
  memset(&TagDistInit[0], 0, 24U * sizeof(double));
  memset(&heading_est_a[0], 0, 20U * sizeof(double));
  memset(&centerest_a[0], 0, 40U * sizeof(double));
  memset(&centerest_a_aver[0], 0, 40U * sizeof(double));
  memset(&headingest_a_aver[0], 0, 20U * sizeof(double));
  memset(&RxIDprev[0], 0, 24U * sizeof(double));
  RxIDprevLen[0] = 0.0;
  PPprev[0] = 0.0;
  RxIDprevLen[1] = 0.0;
  PPprev[1] = 0.0;
  RxIDprevLen[2] = 0.0;
  PPprev[2] = 0.0;
  RxIDprevLen[3] = 0.0;
  PPprev[3] = 0.0;
}

/* End of code generation (UWBpos.c) */
