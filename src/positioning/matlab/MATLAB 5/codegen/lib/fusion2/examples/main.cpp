//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: main.cpp
//
// MATLAB Coder version            : 5.6
// C/C++ source code generated on  : 09-Nov-2023 17:48:28
//

/*************************************************************************/
/* This automatically generated example C++ main file shows how to call  */
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

// Include Files
#include "main.h"
#include "IMUpos.h"
#include "UWBpos6.h"
#include "fusion2.h"
#include "fusion2_terminate.h"
#include "rt_nonfinite.h"

// Function Declarations
static void argInit_1x4_creal_T(creal_T result[4]);

static void argInit_1x5_real_T(double result[5]);

static void argInit_3x1_real_T(double result[3]);

static void argInit_3x20_real_T(double result[60]);

static void argInit_6x1_real_T(double result[6]);

static creal_T argInit_creal_T();

static double argInit_real_T();

// Function Definitions
//
// Arguments    : creal_T result[4]
// Return Type  : void
//
static void argInit_1x4_creal_T(creal_T result[4])
{
  // Loop over the array to initialize each element.
  for (int idx1{0}; idx1 < 4; idx1++) {
    // Set the value of the array element.
    // Change this value to the value that the application requires.
    result[idx1] = argInit_creal_T();
  }
}

//
// Arguments    : double result[5]
// Return Type  : void
//
static void argInit_1x5_real_T(double result[5])
{
  // Loop over the array to initialize each element.
  for (int idx1{0}; idx1 < 5; idx1++) {
    // Set the value of the array element.
    // Change this value to the value that the application requires.
    result[idx1] = argInit_real_T();
  }
}

//
// Arguments    : double result[3]
// Return Type  : void
//
static void argInit_3x1_real_T(double result[3])
{
  // Loop over the array to initialize each element.
  for (int idx0{0}; idx0 < 3; idx0++) {
    // Set the value of the array element.
    // Change this value to the value that the application requires.
    result[idx0] = argInit_real_T();
  }
}

//
// Arguments    : double result[60]
// Return Type  : void
//
static void argInit_3x20_real_T(double result[60])
{
  // Loop over the array to initialize each element.
  for (int i{0}; i < 60; i++) {
    // Set the value of the array element.
    // Change this value to the value that the application requires.
    result[i] = argInit_real_T();
  }
}

//
// Arguments    : double result[6]
// Return Type  : void
//
static void argInit_6x1_real_T(double result[6])
{
  // Loop over the array to initialize each element.
  for (int idx0{0}; idx0 < 6; idx0++) {
    // Set the value of the array element.
    // Change this value to the value that the application requires.
    result[idx0] = argInit_real_T();
  }
}

//
// Arguments    : void
// Return Type  : creal_T
//
static creal_T argInit_creal_T()
{
  creal_T result;
  double re_tmp;
  // Set the value of the complex variable.
  // Change this value to the value that the application requires.
  re_tmp = argInit_real_T();
  result.re = re_tmp;
  result.im = re_tmp;
  return result;
}

//
// Arguments    : void
// Return Type  : double
//
static double argInit_real_T()
{
  return 0.0;
}

//
// Arguments    : int argc
//                char **argv
// Return Type  : int
//
int main(int, char **)
{
  // The initialize function is being called automatically from your entry-point
  // function. So, a call to initialize is not included here. Invoke the
  // entry-point functions.
  // You can call entry-point functions multiple times.
  main_fusion2();
  main_IMUpos();
  main_UWBpos6();
  // Terminate the application.
  // You do not need to do this more than one time.
  fusion2_terminate();
  return 0;
}

//
// Arguments    : void
// Return Type  : void
//
void main_IMUpos()
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
  // Initialize function 'IMUpos' input arguments.
  // Initialize function input argument 'IMUacc_c'.
  argInit_3x1_real_T(b_acc_o);
  // Initialize function input argument 'IMUgyro_c'.
  s_time_tmp = argInit_real_T();
  // Initialize function input argument 'b_acc_o'.
  // Initialize function input argument 'b_gyro'.
  // Initialize function input argument 'cent_pos_est'.
  argInit_3x1_real_T(cent_pos_est_tmp);
  // Initialize function input argument 'cent_vel_est'.
  // Call the entry-point 'IMUpos'.
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

//
// Arguments    : void
// Return Type  : void
//
void main_UWBpos6()
{
  creal_T tag_pos_b_tmp[4];
  double UWBout[21];
  double RxIDin_tmp[6];
  double xain_tmp[6];
  double Ln_tmp;
  // Initialize function 'UWBpos6' input arguments.
  Ln_tmp = argInit_real_T();
  // Initialize function input argument 'RxIDin'.
  argInit_6x1_real_T(RxIDin_tmp);
  // Initialize function input argument 'RxDistin'.
  // Initialize function input argument 'tag_pos_b'.
  argInit_1x4_creal_T(tag_pos_b_tmp);
  // Initialize function input argument 'xain'.
  argInit_6x1_real_T(xain_tmp);
  // Initialize function input argument 'yain'.
  // Initialize function input argument 'prevTagPos'.
  // Call the entry-point 'UWBpos6'.
  UWBpos6(Ln_tmp, Ln_tmp, Ln_tmp, Ln_tmp, Ln_tmp, RxIDin_tmp, RxIDin_tmp,
          Ln_tmp, tag_pos_b_tmp, xain_tmp, xain_tmp, tag_pos_b_tmp, Ln_tmp,
          UWBout);
}

//
// Arguments    : void
// Return Type  : void
//
void main_fusion2()
{
  creal_T dcv[4];
  creal_T IMUposU;
  double Acc_Pos_tmp[60];
  double b_dv[5];
  double b_acc_o[3];
  double cent_pos_est_tmp[3];
  double cent_vel_est[3];
  double gyro_psi;
  double kf_psi;
  double kl_tmp;
  // Initialize function 'fusion2' input arguments.
  kl_tmp = argInit_real_T();
  // Initialize function input argument 'k0'.
  // Initialize function input argument 'tag_pos_est'.
  // Initialize function input argument 'tag_center_vel_est'.
  // Initialize function input argument 'cent_pos_est'.
  argInit_3x1_real_T(cent_pos_est_tmp);
  // Initialize function input argument 'cent_vel_est'.
  // Initialize function input argument 'b_acc_o'.
  // Initialize function input argument 'Acc_Pos'.
  argInit_3x20_real_T(Acc_Pos_tmp);
  // Initialize function input argument 'Acc_Vel'.
  // Call the entry-point 'fusion2'.
  argInit_3x1_real_T(b_acc_o);
  kf_psi = kl_tmp;
  gyro_psi = kl_tmp;
  cent_vel_est[0] = cent_pos_est_tmp[0];
  cent_vel_est[1] = cent_pos_est_tmp[1];
  cent_vel_est[2] = cent_pos_est_tmp[2];
  argInit_1x5_real_T(b_dv);
  argInit_1x4_creal_T(dcv);
  IMUposU = fusion2(kl_tmp, b_dv, dcv, argInit_creal_T(), cent_pos_est_tmp,
                    cent_vel_est, b_acc_o, kl_tmp, Acc_Pos_tmp, Acc_Pos_tmp,
                    kl_tmp, kl_tmp, kl_tmp, &kf_psi, &gyro_psi, kl_tmp, kl_tmp,
                    kl_tmp, kl_tmp, kl_tmp);
}

//
// File trailer for main.cpp
//
// [EOF]
//
