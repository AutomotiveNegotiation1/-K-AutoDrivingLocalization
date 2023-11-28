//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: main.cpp
//
// MATLAB Coder version            : 5.6
// C/C++ source code generated on  : 17-Oct-2023 17:18:56
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
#include "UWBPos6.h"
#include "rt_nonfinite.h"

// Function Declarations
static void argInit_1x4_creal_T(creal_T result[4]);

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
  UWBPos6 *classInstance;
  classInstance = new UWBPos6;
  // Invoke the entry-point functions.
  // You can call entry-point functions multiple times.
  main_UWBpos6(classInstance);
  delete classInstance;
  return 0;
}

//
// Arguments    : UWBPos6 *instancePtr
// Return Type  : void
//
void main_UWBpos6(UWBPos6 *instancePtr)
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
  instancePtr->UWBpos6(Ln_tmp, Ln_tmp, Ln_tmp, Ln_tmp, Ln_tmp, RxIDin_tmp,
                       RxIDin_tmp, Ln_tmp, tag_pos_b_tmp, xain_tmp, xain_tmp,
                       tag_pos_b_tmp, Ln_tmp, UWBout);
}

//
// File trailer for main.cpp
//
// [EOF]
//
