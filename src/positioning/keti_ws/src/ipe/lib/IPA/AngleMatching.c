/*
 * AngleMatching.c
 *
 * Code generation for function 'AngleMatching'
 *
 */

/* Include files */
#include "AngleMatching.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Definitions */
void AngleMatching(double RefAng, double *TargetAng)
{
  double DiffTemp;
  double d;
  DiffTemp = RefAng - *TargetAng;
  if (rtIsNaN(DiffTemp)) {
    d = rtNaN;
  } else if (DiffTemp < 0.0) {
    d = -1.0;
  } else {
    d = (DiffTemp > 0.0);
  }
  *TargetAng +=
      d * floor(fabs(DiffTemp) / 6.2831853071795862) * 2.0 * 3.1415926535897931;
  DiffTemp = *TargetAng - RefAng;
  if (DiffTemp > 3.1415926535897931) {
    *TargetAng -= 6.2831853071795862;
  } else if (DiffTemp < -3.1415926535897931) {
    *TargetAng += 6.2831853071795862;
  }
}

/* End of code generation (AngleMatching.c) */
