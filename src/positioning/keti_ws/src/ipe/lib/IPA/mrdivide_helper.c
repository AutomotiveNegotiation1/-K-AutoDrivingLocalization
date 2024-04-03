/*
 * mrdivide_helper.c
 *
 * Code generation for function 'mrdivide_helper'
 *
 */

/* Include files */
#include "mrdivide_helper.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Definitions */
void mrdiv(double A[90], const double B[36])
{
  double b_A[36];
  double smax;
  int b_i;
  int i;
  int j;
  int jA;
  int jBcol;
  int jp1j;
  int k;
  int kBcol;
  signed char ipiv[6];
  memcpy(&b_A[0], &B[0], 36U * sizeof(double));
  for (i = 0; i < 6; i++) {
    ipiv[i] = (signed char)(i + 1);
  }
  for (j = 0; j < 5; j++) {
    int b_tmp;
    int mmj_tmp;
    mmj_tmp = 4 - j;
    b_tmp = j * 7;
    jp1j = b_tmp + 2;
    jA = 6 - j;
    jBcol = 0;
    smax = fabs(b_A[b_tmp]);
    for (k = 2; k <= jA; k++) {
      double s;
      s = fabs(b_A[(b_tmp + k) - 1]);
      if (s > smax) {
        jBcol = k - 1;
        smax = s;
      }
    }
    if (b_A[b_tmp + jBcol] != 0.0) {
      if (jBcol != 0) {
        jA = j + jBcol;
        ipiv[j] = (signed char)(jA + 1);
        for (k = 0; k < 6; k++) {
          kBcol = j + k * 6;
          smax = b_A[kBcol];
          jBcol = jA + k * 6;
          b_A[kBcol] = b_A[jBcol];
          b_A[jBcol] = smax;
        }
      }
      i = (b_tmp - j) + 6;
      for (b_i = jp1j; b_i <= i; b_i++) {
        b_A[b_i - 1] /= b_A[b_tmp];
      }
    }
    jA = b_tmp;
    for (jBcol = 0; jBcol <= mmj_tmp; jBcol++) {
      smax = b_A[(b_tmp + jBcol * 6) + 6];
      if (smax != 0.0) {
        i = jA + 8;
        jp1j = (jA - j) + 12;
        for (kBcol = i; kBcol <= jp1j; kBcol++) {
          b_A[kBcol - 1] += b_A[((b_tmp + kBcol) - jA) - 7] * -smax;
        }
      }
      jA += 6;
    }
  }
  for (j = 0; j < 6; j++) {
    jBcol = 15 * j - 1;
    jA = 6 * j;
    for (k = 0; k < j; k++) {
      kBcol = 15 * k;
      smax = b_A[k + jA];
      if (smax != 0.0) {
        for (b_i = 0; b_i < 15; b_i++) {
          i = (b_i + jBcol) + 1;
          A[i] -= smax * A[b_i + kBcol];
        }
      }
    }
    smax = 1.0 / b_A[j + jA];
    for (b_i = 0; b_i < 15; b_i++) {
      i = (b_i + jBcol) + 1;
      A[i] *= smax;
    }
  }
  for (j = 5; j >= 0; j--) {
    jBcol = 15 * j - 1;
    jA = 6 * j - 1;
    i = j + 2;
    for (k = i; k < 7; k++) {
      kBcol = 15 * (k - 1);
      smax = b_A[k + jA];
      if (smax != 0.0) {
        for (b_i = 0; b_i < 15; b_i++) {
          jp1j = (b_i + jBcol) + 1;
          A[jp1j] -= smax * A[b_i + kBcol];
        }
      }
    }
  }
  for (j = 4; j >= 0; j--) {
    signed char i1;
    i1 = ipiv[j];
    if (i1 != j + 1) {
      for (b_i = 0; b_i < 15; b_i++) {
        kBcol = b_i + 15 * j;
        smax = A[kBcol];
        i = b_i + 15 * (i1 - 1);
        A[kBcol] = A[i];
        A[i] = smax;
      }
    }
  }
}

/* End of code generation (mrdivide_helper.c) */
