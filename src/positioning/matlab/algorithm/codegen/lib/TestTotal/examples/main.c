/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: main.c
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 01-Aug-2023 15:24:45
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
#include "TestTotal.h"
#include "TestTotal_terminate.h"
#include "rt_nonfinite.h"
#include "type.h"
#include <sys/time.h>

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
  main_TestTotal();
  /* Terminate the application.
You do not need to do this more than one time. */
  TestTotal_terminate();
  return 0;
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void main_TestTotal(void)
{
  creal_T tag_pos_est[4];
  creal_T tag_pos_est_aver[4];
  double heading_est;
  double headingest_a_aver_v;
  long long llStartTime;
  long long llEndTime;
  long long llDiffTime;

  PrintEnter("Successfully Start");

  llStartTime = current_timestamp();

  /* Call the entry-point 'TestTotal'. */
  TestTotal(tag_pos_est, &heading_est, tag_pos_est_aver, &headingest_a_aver_v);

  llEndTime = current_timestamp();
  llDiffTime = llEndTime-llStartTime;
  PrintExit("Start-End time of UWBpos (%lld)us", llDiffTime);

  PrintExit("Successfully End");
}

/*
 * File trailer for main.c
 *
 * [EOF]
 */
