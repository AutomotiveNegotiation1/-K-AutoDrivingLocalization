/*
 * dec2bin.c
 *
 * Code generation for function 'dec2bin'
 *
 */

/* Include files */
#include "dec2bin.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Definitions */
void dec2bin(double d, char s_data[], int s_size[2])
{
  double b_d;
  double olddi;
  int idx;
  int k;
  int nb;
  int npad;
  char sfull[64];
  boolean_T exitg1;
  b_d = d;
  for (idx = 0; idx < 64; idx++) {
    sfull[idx] = '0';
  }
  idx = 64;
  exitg1 = false;
  while ((!exitg1) && (idx > 0)) {
    olddi = b_d;
    b_d /= 2.0;
    b_d = floor(b_d);
    if (2.0 * b_d < olddi) {
      sfull[idx - 1] = '1';
    }
    if (!(b_d > 0.0)) {
      exitg1 = true;
    } else {
      idx--;
    }
  }
  idx = 0;
  k = 0;
  exitg1 = false;
  while ((!exitg1) && (k < 64)) {
    if (sfull[k] == '1') {
      idx = k + 1;
      exitg1 = true;
    } else {
      k++;
    }
  }
  if (idx == 0) {
    idx = 64;
  }
  nb = 64 - idx;
  if (65 - idx <= 4) {
    s_size[0] = 1;
    s_size[1] = 4;
  } else {
    s_size[0] = 1;
    s_size[1] = 65 - idx;
  }
  if (65 - idx < 4) {
    npad = idx - 60;
    for (k = 0; k <= npad - 2; k++) {
      s_data[k] = '0';
    }
  } else {
    npad = 1;
  }
  for (k = 0; k <= nb; k++) {
    s_data[(npad + k) - 1] = sfull[(idx + k) - 1];
  }
}

/* End of code generation (dec2bin.c) */
