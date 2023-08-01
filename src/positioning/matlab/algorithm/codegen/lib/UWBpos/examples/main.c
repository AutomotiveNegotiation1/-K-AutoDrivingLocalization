/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: main.c
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 01-Aug-2023 14:35:53
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
#include "UWBpos.h"
#include "UWBpos_emxAPI.h"
#include "UWBpos_terminate.h"
#include "UWBpos_types.h"
#include "rt_nonfinite.h"
#include "type.h"
#include <sys/time.h>

/* Function Declarations */
static void argInit_1x4_creal_T(creal_T result[4]);

static void argInit_1x6_real_T(double result[6]);

static void argInit_1xd4_real_T(double result_data[], int result_size[2]);

static creal_T argInit_creal_T(void);

static int argInit_d4x1_real_T(double result_data[]);

static double argInit_real_T(void);

long long current_timestamp()
{
    struct timeval te;
    gettimeofday(&te, NULL); // get current time

#if defined(CONFIG_TIME_CHECK_MS)
    long long llMilliSeconds = te.tv_sec*1000LL + te.tv_usec/1000; // calculate milliseconds
    return llMilliSeconds;
#else
    // default us
    long long llMicroSeconds = te.tv_sec*1000000LL + te.tv_usec; // calculate microseconds
    return llMicroSeconds;
#endif
}

/* Function Definitions */
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
 * Arguments    : double result[6]
 * Return Type  : void
 */
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
  main_UWBpos();
  /* Terminate the application.
You do not need to do this more than one time. */
//  UWBpos_terminate();
  return 0;
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void main_UWBpos(void)
{
  emxArray_creal_T *tag_pos_est;
  emxArray_creal_T *tag_pos_est_aver;
  creal_T dcv[4];
  double xa_tmp[6];
  double RxDist_data[4];
  double RxID_data[4];
  double Ln = 6.0;
  double Lp = 4.0;
  double TagNum = 1.0;
  double Nanchor = 8.0;
  double heading_est;
  double headingest_a_aver_v;
  double s_time = 8.0;
  int RxID_size[2];
  int RxDist_size;
  long long llStartTime;
  long long llEndTime;
  long long llDiffTime;

  PrintEnter("Successfully Start");

  /* Initialize function 'UWBpos' input arguments. */
//  Ln_tmp = argInit_real_T();
  /* Initialize function input argument 'RxID'. */
  argInit_1xd4_real_T(RxID_data, RxID_size);
  /* Initialize function input argument 'RxDist'. */
  RxDist_size = argInit_d4x1_real_T(RxDist_data);
  /* Initialize function input argument 'tag_pos_b'. */
  /* Initialize function input argument 'xa'. */
  argInit_1x6_real_T(xa_tmp);
  /* Initialize function input argument 'ya'. */
  /* Call the entry-point 'UWBpos'. */
  emxInitArray_creal_T(&tag_pos_est, 2);
  emxInitArray_creal_T(&tag_pos_est_aver, 2);
  argInit_1x4_creal_T(dcv);

/*
  double Ln, double Lp, double TagNum, double Nanchor, const double RxID_data[], const int RxID_size[2], const double RxDist_data[],
  const int RxDist_size[1], double s_time, const creal_T tag_pos_b[4], const double xa[6], const double ya[6], emxArray_creal_T *tag_pos_est, double *heading_est,
  emxArray_creal_T *tag_pos_est_aver, double *headingest_a_aver_v)

  */
  llStartTime = current_timestamp();

  UWBpos(Ln, Lp, TagNum, Nanchor, RxID_data, RxID_size, RxDist_data,
         &RxDist_size, s_time, dcv, xa_tmp, xa_tmp, tag_pos_est, &heading_est,
         tag_pos_est_aver, &headingest_a_aver_v);
  llEndTime = current_timestamp();
  llDiffTime = llEndTime-llStartTime;
  PrintExit("Start-End time of UWBpos (%lld)us", llDiffTime);

  emxDestroyArray_creal_T(tag_pos_est);
  emxDestroyArray_creal_T(tag_pos_est_aver);

  PrintExit("Successfully End");
}

/*
 * File trailer for main.c
 *
 * [EOF]
 */
