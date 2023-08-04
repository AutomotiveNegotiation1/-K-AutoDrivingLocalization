/*
 * GetMultiTagPosGen2.c
 *
 * Code generation for function 'GetMultiTagPosGen2'
 *
 */

/* Include files */
#include "GetMultiTagPosGen2.h"
#include "UWBpos_types.h"
#include "rt_nonfinite.h"

/* Function Definitions */
void c_binary_expand_op(creal_T in1_data[], int in1_size[2],
                        const double in2[6], const emxArray_real_T *in3,
                        int in4, int in5, const double in7[6], int in8)
{
  const double *in3_data;
  double d;
  int i;
  int loop_ub;
  int stride_0_1;
  int stride_1_1;
  in3_data = in3->data;
  in1_size[0] = 1;
  if (in8 + 1 == 1) {
    in1_size[1] = in5 + 1;
  } else {
    in1_size[1] = in8 + 1;
  }
  stride_0_1 = (in5 + 1 != 1);
  stride_1_1 = (in8 + 1 != 1);
  if (in8 + 1 == 1) {
    loop_ub = in5 + 1;
  } else {
    loop_ub = in8 + 1;
  }
  for (i = 0; i < loop_ub; i++) {
    d = in7[(int)in3_data[in4 + 4 * (i * stride_1_1)] - 1];
    in1_data[i].re =
        in2[(int)in3_data[in4 + 4 * (i * stride_0_1)] - 1] + 0.0 * d;
    in1_data[i].im = d;
  }
}

/* End of code generation (GetMultiTagPosGen2.c) */
