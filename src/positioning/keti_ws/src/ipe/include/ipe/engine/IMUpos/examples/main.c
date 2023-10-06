/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: main.c
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 04-Oct-2023 14:13:20
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
#include "IMUpos.h"
#include "IMUpos_terminate.h"
#include "rt_nonfinite.h"

/* Function Declarations */
static void argInit_1x3_real_T(double result[3]);

static double argInit_real_T(void);

/* Function Definitions */
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
  main_IMUpos();
  /* Terminate the application.
You do not need to do this more than one time. */
  IMUpos_terminate();
  return 0;
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void main_IMUpos(void)
{
  double b_acc_o[3];
  double b_b_acc_o[3];
  double b_gyro[3];
  double cent_pos_est_tmp[3];
  double cent_vel_est[3];
  double acc_b_phi;
  double acc_b_theta;
  double gyro_psi;
  double kf_psi;
  double s_time_tmp;
  double state_o;
  /* Initialize function 'IMUpos' input arguments. */
  /* Initialize function input argument 'IMUacc_c'. */
  argInit_1x3_real_T(b_acc_o);
  /* Initialize function input argument 'IMUgyro_c'. */
  s_time_tmp = argInit_real_T();
  /* Initialize function input argument 'b_acc_o'. */
  /* Initialize function input argument 'b_gyro'. */
  /* Initialize function input argument 'cent_pos_est'. */
  argInit_1x3_real_T(cent_pos_est_tmp);
  /* Initialize function input argument 'cent_vel_est'. */
  /* Call the entry-point 'IMUpos'. */
  kf_psi = s_time_tmp;
  gyro_psi = s_time_tmp;
  cent_vel_est[0] = cent_pos_est_tmp[0];
  b_gyro[0] = b_acc_o[0];
  cent_vel_est[1] = cent_pos_est_tmp[1];
  b_gyro[1] = b_acc_o[1];
  cent_vel_est[2] = cent_pos_est_tmp[2];
  b_gyro[2] = b_acc_o[2];
  b_b_acc_o[0] = b_acc_o[0];
  b_b_acc_o[1] = b_acc_o[1];
  b_b_acc_o[2] = b_acc_o[2];
  IMUpos(b_b_acc_o, b_b_acc_o, s_time_tmp, b_acc_o, b_gyro, s_time_tmp, &kf_psi,
         &gyro_psi, cent_pos_est_tmp, cent_vel_est, &state_o, &acc_b_phi,
         &acc_b_theta);
}

/*
 * File trailer for main.c
 *
 * [EOF]
 */
