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
#include "TestTotal.h"
#include "TestTotal_terminate.h"
#include "rt_nonfinite.h"

/* Function Definitions */
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

void main_TestTotal(void)
{
  creal_T tag_pos_est[4];
  creal_T tag_pos_est_aver[4];
  double heading_est;
  double headingest_a_aver_v;
  /* Call the entry-point 'TestTotal'. */
  TestTotal(tag_pos_est, &heading_est, tag_pos_est_aver, &headingest_a_aver_v);
}

/* End of code generation (main.c) */
