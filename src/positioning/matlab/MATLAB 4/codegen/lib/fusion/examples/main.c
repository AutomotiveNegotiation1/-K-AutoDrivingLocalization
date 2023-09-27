/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: main.c
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 22-Sep-2023 10:33:23
 */

/*************************************************************************/
/* This automatically generated example C main file shows how to call    */
/* entry-point functions that MATLAB Coder generated. You must customize */
/* this file for your application. Do not modify this file directly.     */
/* Instead, make a copy of this file, modify it, and integrate it into   */
/* your development environment.                                         */
/*                                                                       */
/* This file initializes entry-point function arguments to a default     */
/* size and value before calling the entry-point functions. It does      */
/* not store or use any values returned from the entry-point functions.  */
/* If necessary, it does pre-allocate memory for returned values.        */
/* You can use this file as a starting point for a main function that    */
/* you can deploy in your application.                                   */
/*                                                                       */
/* After you copy the file, and before you deploy it, you must make the  */
/* following changes:                                                    */
/* * For variable-size function arguments, change the example sizes to   */
/* the sizes that your application requires.                             */
/* * Change the example values of function arguments to the values that  */
/* your application requires.                                            */
/* * If the entry-point functions return values, store these values or   */
/* otherwise use them as required by your application.                   */
/*                                                                       */
/*************************************************************************/

/* Include Files */
#include "main.h"
#include "PosKalman2.h"
#include "ReliCalc.h"
#include "fusion.h"
#include "fusion_terminate.h"
#include "rt_nonfinite.h"

/* Function Declarations */
static void argInit_12x12_real_T(double result[144]);

static void argInit_12x1_real_T(double result[12]);

static void argInit_1x3_real_T(double result[3]);

static void argInit_1x4_creal_T(creal_T result[4]);

static void argInit_1x5_real_T(double result[5]);

static void argInit_1x6_creal_T(creal_T result[6]);

static void argInit_1xd4_real_T(double result_data[], int result_size[2]);

static void argInit_3x3_real_T(double result[9]);

static creal_T argInit_creal_T(void);

static int argInit_d4x1_real_T(double result_data[]);

static double argInit_real_T(void);

/* Function Definitions */
/*
 * Arguments    : double result[144]
 * Return Type  : void
 */
static void argInit_12x12_real_T(double result[144])
{
  int i;
  /* Loop over the array to initialize each element. */
  for (i = 0; i < 144; i++) {
    /* Set the value of the array element.
Change this value to the value that the application requires. */
    result[i] = argInit_real_T();
  }
}

/*
 * Arguments    : double result[12]
 * Return Type  : void
 */
static void argInit_12x1_real_T(double result[12])
{
  int idx0;
  /* Loop over the array to initialize each element. */
  for (idx0 = 0; idx0 < 12; idx0++) {
    /* Set the value of the array element.
Change this value to the value that the application requires. */
    result[idx0] = argInit_real_T();
  }
}

/*
 * Arguments    : double result[3]
 * Return Type  : void
 */
static void argInit_1x3_real_T(double result[3])
{
  int idx1;
  /* Loop over the array to initialize each element. */
  for (idx1 = 0; idx1 < 3; idx1++) {
    /* Set the value of the array element.
Change this value to the value that the application requires. */
    result[idx1] = argInit_real_T();
  }
}

/*
 * Arguments    : creal_T result[4]
 * Return Type  : void
 */
static void argInit_1x4_creal_T(creal_T result[4])
{
  int idx1;
  /* Loop over the array to initialize each element. */
  for (idx1 = 0; idx1 < 4; idx1++) {
    /* Set the value of the array element.
Change this value to the value that the application requires. */
    result[idx1] = argInit_creal_T();
  }
}

/*
 * Arguments    : double result[5]
 * Return Type  : void
 */
static void argInit_1x5_real_T(double result[5])
{
  int idx1;
  /* Loop over the array to initialize each element. */
  for (idx1 = 0; idx1 < 5; idx1++) {
    /* Set the value of the array element.
Change this value to the value that the application requires. */
    result[idx1] = argInit_real_T();
  }
}

/*
 * Arguments    : creal_T result[6]
 * Return Type  : void
 */
static void argInit_1x6_creal_T(creal_T result[6])
{
  int idx1;
  /* Loop over the array to initialize each element. */
  for (idx1 = 0; idx1 < 6; idx1++) {
    /* Set the value of the array element.
Change this value to the value that the application requires. */
    result[idx1] = argInit_creal_T();
  }
}

/*
 * Arguments    : double result_data[]
 *                int result_size[2]
 * Return Type  : void
 */
static void argInit_1xd4_real_T(double result_data[], int result_size[2])
{
  int idx1;
  /* Set the size of the array.
Change this size to the value that the application requires. */
  result_size[0] = 1;
  result_size[1] = 2;
  /* Loop over the array to initialize each element. */
  for (idx1 = 0; idx1 < 2; idx1++) {
    /* Set the value of the array element.
Change this value to the value that the application requires. */
    result_data[idx1] = argInit_real_T();
  }
}

/*
 * Arguments    : double result[9]
 * Return Type  : void
 */
static void argInit_3x3_real_T(double result[9])
{
  int i;
  /* Loop over the array to initialize each element. */
  for (i = 0; i < 9; i++) {
    /* Set the value of the array element.
Change this value to the value that the application requires. */
    result[i] = argInit_real_T();
  }
}

/*
 * Arguments    : void
 * Return Type  : creal_T
 */
static creal_T argInit_creal_T(void)
{
  creal_T result;
  double re_tmp;
  /* Set the value of the complex variable.
Change this value to the value that the application requires. */
  re_tmp = argInit_real_T();
  result.re = re_tmp;
  result.im = re_tmp;
  return result;
}

/*
 * Arguments    : double result_data[]
 * Return Type  : int
 */
static int argInit_d4x1_real_T(double result_data[])
{
  int idx0;
  int result_size;
  /* Set the size of the array.
Change this size to the value that the application requires. */
  result_size = 2;
  /* Loop over the array to initialize each element. */
  for (idx0 = 0; idx0 < 2; idx0++) {
    /* Set the value of the array element.
Change this value to the value that the application requires. */
    result_data[idx0] = argInit_real_T();
  }
  return result_size;
}

/*
 * Arguments    : void
 * Return Type  : double
 */
static double argInit_real_T(void)
{
  return 0.0;
}

/*
 * Arguments    : int argc
 *                char **argv
 * Return Type  : int
 */
int main(int argc, char **argv)
{
  (void)argc;
  (void)argv;
  /* The initialize function is being called automatically from your entry-point
   * function. So, a call to initialize is not included here. */
  /* Invoke the entry-point functions.
You can call entry-point functions multiple times. */
  main_fusion();
  main_PosKalman2();
  main_ReliCalc();
  /* Terminate the application.
You do not need to do this more than one time. */
  fusion_terminate();
  return 0;
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void main_PosKalman2(void)
{
  double dv[144];
  double x[12];
  double dv2[9];
  double dv1[3];
  /* Initialize function 'PosKalman2' input arguments. */
  /* Initialize function input argument 'x'. */
  /* Initialize function input argument 'A'. */
  /* Initialize function input argument 'z'. */
  /* Initialize function input argument 'R'. */
  /* Call the entry-point 'PosKalman2'. */
  argInit_12x1_real_T(x);
  argInit_12x12_real_T(dv);
  argInit_1x3_real_T(dv1);
  argInit_3x3_real_T(dv2);
  PosKalman2(x, dv, dv1, dv2);
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void main_ReliCalc(void)
{
  creal_T dcv1[6];
  creal_T dcv[4];
  double RxDist_data[4];
  double RxID_data[4];
  double Reliability;
  int RxID_size[2];
  int RxDist_size;
  /* Initialize function 'ReliCalc' input arguments. */
  /* Initialize function input argument 'tag_pos_est'. */
  /* Initialize function input argument 'anchor_pos'. */
  /* Initialize function input argument 'RxDist'. */
  RxDist_size = argInit_d4x1_real_T(RxDist_data);
  /* Initialize function input argument 'RxID'. */
  argInit_1xd4_real_T(RxID_data, RxID_size);
  /* Call the entry-point 'ReliCalc'. */
  argInit_1x4_creal_T(dcv);
  argInit_1x6_creal_T(dcv1);
  Reliability = ReliCalc(dcv, dcv1, RxDist_data, &RxDist_size, RxID_data,
                         RxID_size, argInit_real_T());
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void main_fusion(void)
{
  creal_T dcv[4];
  creal_T IMUposU;
  creal_T tag_center_vel_est;
  double dv1[5];
  double cent_pos_est_tmp[3];
  double cent_vel_est[3];
  double dv[3];
  double kf_psi;
  double state_IMU_tmp;
  /* Initialize function 'fusion' input arguments. */
  /* Initialize function input argument 'tag_center_vel_est'. */
  state_IMU_tmp = argInit_real_T();
  /* Initialize function input argument 'b_acc_o'. */
  /* Initialize function input argument 'k0'. */
  /* Initialize function input argument 'cent_pos_est'. */
  argInit_1x3_real_T(cent_pos_est_tmp);
  /* Initialize function input argument 'cent_vel_est'. */
  /* Initialize function input argument 'tag_pos_est'. */
  /* Call the entry-point 'fusion'. */
  cent_vel_est[0] = cent_pos_est_tmp[0];
  cent_vel_est[1] = cent_pos_est_tmp[1];
  cent_vel_est[2] = cent_pos_est_tmp[2];
  kf_psi = state_IMU_tmp;
  tag_center_vel_est = argInit_creal_T();
  argInit_1x3_real_T(dv);
  argInit_1x5_real_T(dv1);
  argInit_1x4_creal_T(dcv);
  IMUposU = fusion(&tag_center_vel_est, state_IMU_tmp, state_IMU_tmp, dv,
                   state_IMU_tmp, &state_IMU_tmp, state_IMU_tmp, state_IMU_tmp,
                   state_IMU_tmp, dv1, cent_pos_est_tmp, cent_vel_est, &kf_psi,
                   dcv, state_IMU_tmp, state_IMU_tmp);
}

/*
 * File trailer for main.c
 *
 * [EOF]
 */
