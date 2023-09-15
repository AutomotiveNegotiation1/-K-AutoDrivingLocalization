/*
 * main.c
 *
 * Code generation for function 'main'
 *
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

/* Include files */
#include "main.h"
#include "UWBpos2.h"
#include "UWBpos2_emxAPI.h"
#include "UWBpos2_terminate.h"
#include "UWBpos2_types.h"
#include "rt_nonfinite.h"

/* Function Declarations */
static void argInit_1x4_creal_T(creal_T result[4]);

static void argInit_6x1_real_T(double result[6]);

static creal_T argInit_creal_T(void);

static double argInit_real_T(void);

/* Function Definitions */
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

static void argInit_6x1_real_T(double result[6])
{
  int idx0;
  /* Loop over the array to initialize each element. */
  for (idx0 = 0; idx0 < 6; idx0++) {
    /* Set the value of the array element.
Change this value to the value that the application requires. */
    result[idx0] = argInit_real_T();
  }
}

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

static double argInit_real_T(void)
{
  return 0.0;
}

int main(int argc, char **argv)
{
  (void)argc;
  (void)argv;
  /* The initialize function is being called automatically from your entry-point
   * function. So, a call to initialize is not included here. */
  /* Invoke the entry-point functions.
You can call entry-point functions multiple times. */
  main_UWBpos2();
  /* Terminate the application.
You do not need to do this more than one time. */
  UWBpos2_terminate();
  return 0;
}

void main_UWBpos2(void)
{
  emxArray_real_T *UWBout;
  creal_T dcv[4];
  double RxIDin_tmp[6];
  double xain_tmp[6];
  double Ln_tmp;
  /* Initialize function 'UWBpos2' input arguments. */
  Ln_tmp = argInit_real_T();
  /* Initialize function input argument 'RxIDin'. */
  argInit_6x1_real_T(RxIDin_tmp);
  /* Initialize function input argument 'RxDistin'. */
  /* Initialize function input argument 'tag_pos_b'. */
  /* Initialize function input argument 'xain'. */
  argInit_6x1_real_T(xain_tmp);
  /* Initialize function input argument 'yain'. */
  /* Call the entry-point 'UWBpos2'. */
  emxInitArray_real_T(&UWBout, 2);
  argInit_1x4_creal_T(dcv);
  UWBpos2(Ln_tmp, Ln_tmp, Ln_tmp, Ln_tmp, Ln_tmp, RxIDin_tmp, RxIDin_tmp,
          Ln_tmp, dcv, xain_tmp, xain_tmp, UWBout);
  emxDestroyArray_real_T(UWBout);
}

/* End of code generation (main.c) */
