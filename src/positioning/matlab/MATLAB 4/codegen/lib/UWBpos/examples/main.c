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
#include "UWBpos.h"
#include "UWBpos_emxAPI.h"
#include "UWBpos_terminate.h"
#include "UWBpos_types.h"
#include "rt_nonfinite.h"

/* Function Declarations */
static void argInit_1x4_creal_T(creal_T result[4]);

static void argInit_1x6_real_T(double result[6]);

static void argInit_1xd4_real_T(double result_data[], int result_size[2]);

static creal_T argInit_creal_T(void);

static void argInit_d4x1_real_T(double result_data[], int *result_size);

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

static void argInit_1x6_real_T(double result[6])
{
  int idx1;
  /* Loop over the array to initialize each element. */
  for (idx1 = 0; idx1 < 6; idx1++) {
    /* Set the value of the array element.
Change this value to the value that the application requires. */
    result[idx1] = argInit_real_T();
  }
}

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

static void argInit_d4x1_real_T(double result_data[], int *result_size)
{
  int idx0;
  /* Set the size of the array.
Change this size to the value that the application requires. */
  *result_size = 2;
  /* Loop over the array to initialize each element. */
  for (idx0 = 0; idx0 < 2; idx0++) {
    /* Set the value of the array element.
Change this value to the value that the application requires. */
    result_data[idx0] = argInit_real_T();
  }
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
  main_UWBpos();
  /* Terminate the application.
You do not need to do this more than one time. */
  UWBpos_terminate();
  return 0;
}

void main_UWBpos(void)
{
  emxArray_creal_T *tag_pos_est;
  emxArray_creal_T *tag_pos_est_aver;
  creal_T dcv[4];
  double xa_tmp[6];
  double RxDist_data[4];
  double RxID_data[4];
  double Ln_tmp;
  double heading_est;
  double headingest_a_aver_v;
  int RxID_size[2];
  int RxDist_size;
  /* Initialize function 'UWBpos' input arguments. */
  Ln_tmp = argInit_real_T();
  /* Initialize function input argument 'RxID'. */
  argInit_1xd4_real_T(RxID_data, RxID_size);
  /* Initialize function input argument 'RxDist'. */
  argInit_d4x1_real_T(RxDist_data, &RxDist_size);
  /* Initialize function input argument 'tag_pos_b'. */
  /* Initialize function input argument 'xa'. */
  argInit_1x6_real_T(xa_tmp);
  /* Initialize function input argument 'ya'. */
  /* Call the entry-point 'UWBpos'. */
  emxInitArray_creal_T(&tag_pos_est, 2);
  emxInitArray_creal_T(&tag_pos_est_aver, 2);
  argInit_1x4_creal_T(dcv);
  UWBpos(Ln_tmp, Ln_tmp, Ln_tmp, Ln_tmp, RxID_data, RxID_size, RxDist_data,
         &RxDist_size, Ln_tmp, dcv, xa_tmp, xa_tmp, tag_pos_est, &heading_est,
         tag_pos_est_aver, &headingest_a_aver_v);
  emxDestroyArray_creal_T(tag_pos_est);
  emxDestroyArray_creal_T(tag_pos_est_aver);
}

/* End of code generation (main.c) */
