/*
 * randperm.c
 *
 * Code generation for function 'randperm'
 *
 */

/* Include files */
#include "randperm.h"
#include "rand.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Definitions */
void randperm(double k, double p_data[], int p_size[2])
{
  double denom;
  double numer;
  double pt;
  double t;
  double u;
  int loop_ub_tmp;
  int m;
  p_size[0] = 1;
  loop_ub_tmp = (int)k;
  p_size[1] = (int)k;
  if (loop_ub_tmp - 1 >= 0) {
    memset(&p_data[0], 0, (unsigned int)loop_ub_tmp * sizeof(double));
  }
  t = 0.0;
  loop_ub_tmp = (int)((k - 1.0) + 1.0);
  for (m = 0; m < loop_ub_tmp; m++) {
    numer = k - (double)m;
    denom = 6.0 - t;
    pt = numer / (6.0 - t);
    u = b_rand();
    while (u > pt) {
      t++;
      denom--;
      pt += (1.0 - pt) * (numer / denom);
    }
    t++;
    numer = b_rand() * ((double)m + 1.0);
    numer = floor(numer);
    p_data[m] = p_data[(int)(numer + 1.0) - 1];
    p_data[(int)(numer + 1.0) - 1] = t;
  }
}

/* End of code generation (randperm.c) */
