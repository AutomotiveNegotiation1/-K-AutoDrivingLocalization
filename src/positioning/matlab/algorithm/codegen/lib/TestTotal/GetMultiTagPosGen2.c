/*
 * GetMultiTagPosGen2.c
 *
 * Code generation for function 'GetMultiTagPosGen2'
 *
 */

/* Include files */
#include "GetMultiTagPosGen2.h"
#include "rt_nonfinite.h"

/* Function Definitions */
void b_binary_expand_op(cint8_T in1_data[], int in1_size[2],
                        const signed char in2[6], const double in3[24], int in4,
                        int in5, const signed char in7[6], int in8)
{
  int i;
  int loop_ub;
  int stride_0_1;
  int stride_1_1;
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
    in1_data[i].re = in2[(int)in3[in4 + ((i * stride_0_1) << 2)] - 1];
    in1_data[i].im = in7[(int)in3[in4 + ((i * stride_1_1) << 2)] - 1];
  }
}

/* End of code generation (GetMultiTagPosGen2.c) */
