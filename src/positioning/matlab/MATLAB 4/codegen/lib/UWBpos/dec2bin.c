/*
 * dec2bin.c
 *
 * Code generation for function 'dec2bin'
 *
 */

/* Include files */
#include "dec2bin.h"
#include "UWBpos_emxutil.h"
#include "UWBpos_types.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Definitions */
void dec2bin(double d, double numBits, emxArray_char_T *s)
{
  double b_d;
  double olddi;
  int idx;
  int k;
  int nb;
  int nbits;
  int npad;
  char sfull[64];
  char *s_data;
  boolean_T exitg1;
  if ((int)numBits <= 1) {
    nbits = 1;
  } else {
    nbits = (int)numBits;
  }
  b_d = d;
  for (npad = 0; npad < 64; npad++) {
    sfull[npad] = '0';
  }
  npad = 64;
  exitg1 = false;
  while ((!exitg1) && (npad > 0)) {
    olddi = b_d;
    b_d /= 2.0;
    b_d = floor(b_d);
    if (2.0 * b_d < olddi) {
      sfull[npad - 1] = '1';
    }
    if (!(b_d > 0.0)) {
      exitg1 = true;
    } else {
      npad--;
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
  if (nbits >= 65 - idx) {
    npad = s->size[0] * s->size[1];
    s->size[0] = 1;
    s->size[1] = nbits;
    emxEnsureCapacity_char_T(s, npad);
    s_data = s->data;
  } else {
    npad = s->size[0] * s->size[1];
    s->size[0] = 1;
    s->size[1] = 65 - idx;
    emxEnsureCapacity_char_T(s, npad);
    s_data = s->data;
  }
  if (65 - idx < nbits) {
    npad = (nbits + idx) - 64;
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
