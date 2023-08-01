/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: find.c
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 01-Aug-2023 14:00:18
 */

/* Include Files */
#include "find.h"
#include "UWBpos_emxutil.h"
#include "UWBpos_types.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Arguments    : const emxArray_boolean_T *x
 *                emxArray_int32_T *i
 *                emxArray_int32_T *j
 * Return Type  : void
 */
void b_eml_find(const emxArray_boolean_T *x, emxArray_int32_T *i,
                emxArray_int32_T *j)
{
  int *i_data;
  int *j_data;
  const bool *x_data;
  x_data = x->data;
  if (x->size[1] == 0) {
    i->size[0] = 1;
    i->size[1] = 0;
    j->size[0] = 1;
    j->size[1] = 0;
  } else {
    int b_i;
    int idx;
    int jj;
    bool exitg1;
    idx = 0;
    b_i = i->size[0] * i->size[1];
    i->size[0] = 1;
    i->size[1] = x->size[1];
    emxEnsureCapacity_int32_T(i, b_i);
    i_data = i->data;
    b_i = j->size[0] * j->size[1];
    j->size[0] = 1;
    j->size[1] = x->size[1];
    emxEnsureCapacity_int32_T(j, b_i);
    j_data = j->data;
    jj = 1;
    exitg1 = false;
    while ((!exitg1) && (jj <= x->size[1])) {
      if (x_data[jj - 1]) {
        idx++;
        i_data[idx - 1] = 1;
        j_data[idx - 1] = jj;
        if (idx >= x->size[1]) {
          exitg1 = true;
        } else {
          jj++;
        }
      } else {
        jj++;
      }
    }
    if (x->size[1] == 1) {
      if (idx == 0) {
        i->size[0] = 1;
        i->size[1] = 0;
        j->size[0] = 1;
        j->size[1] = 0;
      }
    } else {
      if (idx < 1) {
        b_i = 0;
      } else {
        b_i = idx;
      }
      jj = i->size[0] * i->size[1];
      i->size[1] = b_i;
      emxEnsureCapacity_int32_T(i, jj);
      jj = j->size[0] * j->size[1];
      j->size[1] = b_i;
      emxEnsureCapacity_int32_T(j, jj);
    }
  }
}

/*
 * Arguments    : const emxArray_boolean_T *x
 *                emxArray_int32_T *i
 * Return Type  : void
 */
void eml_find(const emxArray_boolean_T *x, emxArray_int32_T *i)
{
  int idx;
  int ii;
  int nx;
  int *i_data;
  const bool *x_data;
  bool exitg1;
  x_data = x->data;
  nx = x->size[0];
  idx = 0;
  ii = i->size[0];
  i->size[0] = x->size[0];
  emxEnsureCapacity_int32_T(i, ii);
  i_data = i->data;
  ii = 0;
  exitg1 = false;
  while ((!exitg1) && (ii <= nx - 1)) {
    if (x_data[ii]) {
      idx++;
      i_data[idx - 1] = ii + 1;
      if (idx >= nx) {
        exitg1 = true;
      } else {
        ii++;
      }
    } else {
      ii++;
    }
  }
  if (x->size[0] == 1) {
    if (idx == 0) {
      i->size[0] = 0;
    }
  } else {
    ii = i->size[0];
    if (idx < 1) {
      i->size[0] = 0;
    } else {
      i->size[0] = idx;
    }
    emxEnsureCapacity_int32_T(i, ii);
  }
}

/*
 * File trailer for find.c
 *
 * [EOF]
 */
