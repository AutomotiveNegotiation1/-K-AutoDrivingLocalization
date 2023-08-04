/*
 * inv.c
 *
 * Code generation for function 'inv'
 *
 */

/* Include files */
#include "inv.h"
#include "UWBpos_data.h"
#include "UWBpos_rtwutil.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>

/* Function Definitions */
void b_inv(const creal_T x[9], creal_T y[9])
{
  creal_T b_x[9];
  double absx11;
  double absx21;
  double absx31;
  double brm;
  double d;
  double im;
  double re;
  double t1_im;
  double t1_re;
  double t2_im;
  double t2_re;
  int itmp;
  int p1;
  int p2;
  int p3;
  memcpy(&b_x[0], &x[0], 9U * sizeof(creal_T));
  p1 = 0;
  p2 = 3;
  p3 = 6;
  absx11 = rt_hypotd_snf(x[0].re, x[0].im);
  absx21 = rt_hypotd_snf(x[1].re, x[1].im);
  absx31 = rt_hypotd_snf(x[2].re, x[2].im);
  if ((absx21 > absx11) && (absx21 > absx31)) {
    p1 = 3;
    p2 = 0;
    b_x[0] = x[1];
    b_x[1] = x[0];
    b_x[3] = x[4];
    b_x[4] = x[3];
    b_x[6] = x[7];
    b_x[7] = x[6];
  } else if (absx31 > absx11) {
    p1 = 6;
    p3 = 0;
    b_x[0] = x[2];
    b_x[2] = x[0];
    b_x[3] = x[5];
    b_x[5] = x[3];
    b_x[6] = x[8];
    b_x[8] = x[6];
  }
  if (b_x[0].im == 0.0) {
    if (b_x[1].im == 0.0) {
      re = b_x[1].re / b_x[0].re;
      im = 0.0;
    } else if (b_x[1].re == 0.0) {
      re = 0.0;
      im = b_x[1].im / b_x[0].re;
    } else {
      re = b_x[1].re / b_x[0].re;
      im = b_x[1].im / b_x[0].re;
    }
  } else if (b_x[0].re == 0.0) {
    if (b_x[1].re == 0.0) {
      re = b_x[1].im / b_x[0].im;
      im = 0.0;
    } else if (b_x[1].im == 0.0) {
      re = 0.0;
      im = -(b_x[1].re / b_x[0].im);
    } else {
      re = b_x[1].im / b_x[0].im;
      im = -(b_x[1].re / b_x[0].im);
    }
  } else {
    brm = fabs(b_x[0].re);
    absx31 = fabs(b_x[0].im);
    if (brm > absx31) {
      absx31 = b_x[0].im / b_x[0].re;
      d = b_x[0].re + absx31 * b_x[0].im;
      re = (b_x[1].re + absx31 * b_x[1].im) / d;
      im = (b_x[1].im - absx31 * b_x[1].re) / d;
    } else if (absx31 == brm) {
      if (b_x[0].re > 0.0) {
        absx31 = 0.5;
      } else {
        absx31 = -0.5;
      }
      if (b_x[0].im > 0.0) {
        d = 0.5;
      } else {
        d = -0.5;
      }
      re = (b_x[1].re * absx31 + b_x[1].im * d) / brm;
      im = (b_x[1].im * absx31 - b_x[1].re * d) / brm;
    } else {
      absx31 = b_x[0].re / b_x[0].im;
      d = b_x[0].im + absx31 * b_x[0].re;
      re = (absx31 * b_x[1].re + b_x[1].im) / d;
      im = (absx31 * b_x[1].im - b_x[1].re) / d;
    }
  }
  b_x[1].re = re;
  b_x[1].im = im;
  if (b_x[0].im == 0.0) {
    if (b_x[2].im == 0.0) {
      absx21 = b_x[2].re / b_x[0].re;
      absx11 = 0.0;
    } else if (b_x[2].re == 0.0) {
      absx21 = 0.0;
      absx11 = b_x[2].im / b_x[0].re;
    } else {
      absx21 = b_x[2].re / b_x[0].re;
      absx11 = b_x[2].im / b_x[0].re;
    }
  } else if (b_x[0].re == 0.0) {
    if (b_x[2].re == 0.0) {
      absx21 = b_x[2].im / b_x[0].im;
      absx11 = 0.0;
    } else if (b_x[2].im == 0.0) {
      absx21 = 0.0;
      absx11 = -(b_x[2].re / b_x[0].im);
    } else {
      absx21 = b_x[2].im / b_x[0].im;
      absx11 = -(b_x[2].re / b_x[0].im);
    }
  } else {
    brm = fabs(b_x[0].re);
    absx31 = fabs(b_x[0].im);
    if (brm > absx31) {
      absx31 = b_x[0].im / b_x[0].re;
      d = b_x[0].re + absx31 * b_x[0].im;
      absx21 = (b_x[2].re + absx31 * b_x[2].im) / d;
      absx11 = (b_x[2].im - absx31 * b_x[2].re) / d;
    } else if (absx31 == brm) {
      if (b_x[0].re > 0.0) {
        absx31 = 0.5;
      } else {
        absx31 = -0.5;
      }
      if (b_x[0].im > 0.0) {
        d = 0.5;
      } else {
        d = -0.5;
      }
      absx21 = (b_x[2].re * absx31 + b_x[2].im * d) / brm;
      absx11 = (b_x[2].im * absx31 - b_x[2].re * d) / brm;
    } else {
      absx31 = b_x[0].re / b_x[0].im;
      d = b_x[0].im + absx31 * b_x[0].re;
      absx21 = (absx31 * b_x[2].re + b_x[2].im) / d;
      absx11 = (absx31 * b_x[2].im - b_x[2].re) / d;
    }
  }
  b_x[2].re = absx21;
  b_x[2].im = absx11;
  b_x[4].re -= re * b_x[3].re - im * b_x[3].im;
  b_x[4].im -= re * b_x[3].im + im * b_x[3].re;
  b_x[5].re -= absx21 * b_x[3].re - absx11 * b_x[3].im;
  b_x[5].im -= absx21 * b_x[3].im + absx11 * b_x[3].re;
  b_x[7].re -= re * b_x[6].re - im * b_x[6].im;
  b_x[7].im -= re * b_x[6].im + im * b_x[6].re;
  b_x[8].re -= absx21 * b_x[6].re - absx11 * b_x[6].im;
  b_x[8].im -= absx21 * b_x[6].im + absx11 * b_x[6].re;
  if (rt_hypotd_snf(b_x[5].re, b_x[5].im) >
      rt_hypotd_snf(b_x[4].re, b_x[4].im)) {
    itmp = p2;
    p2 = p3;
    p3 = itmp;
    b_x[1] = b_x[2];
    b_x[2].re = re;
    b_x[2].im = im;
    t1_re = b_x[4].re;
    t1_im = b_x[4].im;
    b_x[4] = b_x[5];
    b_x[5].re = t1_re;
    b_x[5].im = t1_im;
    t1_re = b_x[7].re;
    t1_im = b_x[7].im;
    b_x[7] = b_x[8];
    b_x[8].re = t1_re;
    b_x[8].im = t1_im;
  }
  if (b_x[4].im == 0.0) {
    if (b_x[5].im == 0.0) {
      re = b_x[5].re / b_x[4].re;
      im = 0.0;
    } else if (b_x[5].re == 0.0) {
      re = 0.0;
      im = b_x[5].im / b_x[4].re;
    } else {
      re = b_x[5].re / b_x[4].re;
      im = b_x[5].im / b_x[4].re;
    }
  } else if (b_x[4].re == 0.0) {
    if (b_x[5].re == 0.0) {
      re = b_x[5].im / b_x[4].im;
      im = 0.0;
    } else if (b_x[5].im == 0.0) {
      re = 0.0;
      im = -(b_x[5].re / b_x[4].im);
    } else {
      re = b_x[5].im / b_x[4].im;
      im = -(b_x[5].re / b_x[4].im);
    }
  } else {
    brm = fabs(b_x[4].re);
    absx31 = fabs(b_x[4].im);
    if (brm > absx31) {
      absx31 = b_x[4].im / b_x[4].re;
      d = b_x[4].re + absx31 * b_x[4].im;
      re = (b_x[5].re + absx31 * b_x[5].im) / d;
      im = (b_x[5].im - absx31 * b_x[5].re) / d;
    } else if (absx31 == brm) {
      if (b_x[4].re > 0.0) {
        absx31 = 0.5;
      } else {
        absx31 = -0.5;
      }
      if (b_x[4].im > 0.0) {
        d = 0.5;
      } else {
        d = -0.5;
      }
      re = (b_x[5].re * absx31 + b_x[5].im * d) / brm;
      im = (b_x[5].im * absx31 - b_x[5].re * d) / brm;
    } else {
      absx31 = b_x[4].re / b_x[4].im;
      d = b_x[4].im + absx31 * b_x[4].re;
      re = (absx31 * b_x[5].re + b_x[5].im) / d;
      im = (absx31 * b_x[5].im - b_x[5].re) / d;
    }
  }
  b_x[8].re -= re * b_x[7].re - im * b_x[7].im;
  b_x[8].im -= re * b_x[7].im + im * b_x[7].re;
  absx11 = (b_x[1].re * re - b_x[1].im * im) - b_x[2].re;
  absx21 = (b_x[1].re * im + b_x[1].im * re) - b_x[2].im;
  if (b_x[8].im == 0.0) {
    if (absx21 == 0.0) {
      t1_re = absx11 / b_x[8].re;
      t1_im = 0.0;
    } else if (absx11 == 0.0) {
      t1_re = 0.0;
      t1_im = absx21 / b_x[8].re;
    } else {
      t1_re = absx11 / b_x[8].re;
      t1_im = absx21 / b_x[8].re;
    }
  } else if (b_x[8].re == 0.0) {
    if (absx11 == 0.0) {
      t1_re = absx21 / b_x[8].im;
      t1_im = 0.0;
    } else if (absx21 == 0.0) {
      t1_re = 0.0;
      t1_im = -(absx11 / b_x[8].im);
    } else {
      t1_re = absx21 / b_x[8].im;
      t1_im = -(absx11 / b_x[8].im);
    }
  } else {
    brm = fabs(b_x[8].re);
    absx31 = fabs(b_x[8].im);
    if (brm > absx31) {
      absx31 = b_x[8].im / b_x[8].re;
      d = b_x[8].re + absx31 * b_x[8].im;
      t1_re = (absx11 + absx31 * absx21) / d;
      t1_im = (absx21 - absx31 * absx11) / d;
    } else if (absx31 == brm) {
      if (b_x[8].re > 0.0) {
        absx31 = 0.5;
      } else {
        absx31 = -0.5;
      }
      if (b_x[8].im > 0.0) {
        d = 0.5;
      } else {
        d = -0.5;
      }
      t1_re = (absx11 * absx31 + absx21 * d) / brm;
      t1_im = (absx21 * absx31 - absx11 * d) / brm;
    } else {
      absx31 = b_x[8].re / b_x[8].im;
      d = b_x[8].im + absx31 * b_x[8].re;
      t1_re = (absx31 * absx11 + absx21) / d;
      t1_im = (absx31 * absx21 - absx11) / d;
    }
  }
  absx11 = -(b_x[1].re + (b_x[7].re * t1_re - b_x[7].im * t1_im));
  absx21 = -(b_x[1].im + (b_x[7].re * t1_im + b_x[7].im * t1_re));
  if (b_x[4].im == 0.0) {
    if (absx21 == 0.0) {
      t2_re = absx11 / b_x[4].re;
      t2_im = 0.0;
    } else if (absx11 == 0.0) {
      t2_re = 0.0;
      t2_im = absx21 / b_x[4].re;
    } else {
      t2_re = absx11 / b_x[4].re;
      t2_im = absx21 / b_x[4].re;
    }
  } else if (b_x[4].re == 0.0) {
    if (absx11 == 0.0) {
      t2_re = absx21 / b_x[4].im;
      t2_im = 0.0;
    } else if (absx21 == 0.0) {
      t2_re = 0.0;
      t2_im = -(absx11 / b_x[4].im);
    } else {
      t2_re = absx21 / b_x[4].im;
      t2_im = -(absx11 / b_x[4].im);
    }
  } else {
    brm = fabs(b_x[4].re);
    absx31 = fabs(b_x[4].im);
    if (brm > absx31) {
      absx31 = b_x[4].im / b_x[4].re;
      d = b_x[4].re + absx31 * b_x[4].im;
      t2_re = (absx11 + absx31 * absx21) / d;
      t2_im = (absx21 - absx31 * absx11) / d;
    } else if (absx31 == brm) {
      if (b_x[4].re > 0.0) {
        absx31 = 0.5;
      } else {
        absx31 = -0.5;
      }
      if (b_x[4].im > 0.0) {
        d = 0.5;
      } else {
        d = -0.5;
      }
      t2_re = (absx11 * absx31 + absx21 * d) / brm;
      t2_im = (absx21 * absx31 - absx11 * d) / brm;
    } else {
      absx31 = b_x[4].re / b_x[4].im;
      d = b_x[4].im + absx31 * b_x[4].re;
      t2_re = (absx31 * absx11 + absx21) / d;
      t2_im = (absx31 * absx21 - absx11) / d;
    }
  }
  absx11 = (1.0 - (b_x[3].re * t2_re - b_x[3].im * t2_im)) -
           (b_x[6].re * t1_re - b_x[6].im * t1_im);
  absx21 = (0.0 - (b_x[3].re * t2_im + b_x[3].im * t2_re)) -
           (b_x[6].re * t1_im + b_x[6].im * t1_re);
  if (b_x[0].im == 0.0) {
    if (absx21 == 0.0) {
      y[p1].re = absx11 / b_x[0].re;
      y[p1].im = 0.0;
    } else if (absx11 == 0.0) {
      y[p1].re = 0.0;
      y[p1].im = absx21 / b_x[0].re;
    } else {
      y[p1].re = absx11 / b_x[0].re;
      y[p1].im = absx21 / b_x[0].re;
    }
  } else if (b_x[0].re == 0.0) {
    if (absx11 == 0.0) {
      y[p1].re = absx21 / b_x[0].im;
      y[p1].im = 0.0;
    } else if (absx21 == 0.0) {
      y[p1].re = 0.0;
      y[p1].im = -(absx11 / b_x[0].im);
    } else {
      y[p1].re = absx21 / b_x[0].im;
      y[p1].im = -(absx11 / b_x[0].im);
    }
  } else {
    brm = fabs(b_x[0].re);
    absx31 = fabs(b_x[0].im);
    if (brm > absx31) {
      absx31 = b_x[0].im / b_x[0].re;
      d = b_x[0].re + absx31 * b_x[0].im;
      y[p1].re = (absx11 + absx31 * absx21) / d;
      y[p1].im = (absx21 - absx31 * absx11) / d;
    } else if (absx31 == brm) {
      if (b_x[0].re > 0.0) {
        absx31 = 0.5;
      } else {
        absx31 = -0.5;
      }
      if (b_x[0].im > 0.0) {
        d = 0.5;
      } else {
        d = -0.5;
      }
      y[p1].re = (absx11 * absx31 + absx21 * d) / brm;
      y[p1].im = (absx21 * absx31 - absx11 * d) / brm;
    } else {
      absx31 = b_x[0].re / b_x[0].im;
      d = b_x[0].im + absx31 * b_x[0].re;
      y[p1].re = (absx31 * absx11 + absx21) / d;
      y[p1].im = (absx31 * absx21 - absx11) / d;
    }
  }
  y[p1 + 1].re = t2_re;
  y[p1 + 1].im = t2_im;
  y[p1 + 2].re = t1_re;
  y[p1 + 2].im = t1_im;
  if (b_x[8].im == 0.0) {
    if (-im == 0.0) {
      t1_re = -re / b_x[8].re;
      t1_im = 0.0;
    } else if (-re == 0.0) {
      t1_re = 0.0;
      t1_im = -im / b_x[8].re;
    } else {
      t1_re = -re / b_x[8].re;
      t1_im = -im / b_x[8].re;
    }
  } else if (b_x[8].re == 0.0) {
    if (-re == 0.0) {
      t1_re = -im / b_x[8].im;
      t1_im = 0.0;
    } else if (-im == 0.0) {
      t1_re = 0.0;
      t1_im = -(-re / b_x[8].im);
    } else {
      t1_re = -im / b_x[8].im;
      t1_im = -(-re / b_x[8].im);
    }
  } else {
    brm = fabs(b_x[8].re);
    absx31 = fabs(b_x[8].im);
    if (brm > absx31) {
      absx31 = b_x[8].im / b_x[8].re;
      d = b_x[8].re + absx31 * b_x[8].im;
      t1_re = (-re + absx31 * -im) / d;
      t1_im = (-im - absx31 * -re) / d;
    } else if (absx31 == brm) {
      if (b_x[8].re > 0.0) {
        absx31 = 0.5;
      } else {
        absx31 = -0.5;
      }
      if (b_x[8].im > 0.0) {
        d = 0.5;
      } else {
        d = -0.5;
      }
      t1_re = (-re * absx31 + -im * d) / brm;
      t1_im = (-im * absx31 - -re * d) / brm;
    } else {
      absx31 = b_x[8].re / b_x[8].im;
      d = b_x[8].im + absx31 * b_x[8].re;
      t1_re = (absx31 * -re - im) / d;
      t1_im = (absx31 * -im - (-re)) / d;
    }
  }
  absx11 = b_x[7].re * t1_re - b_x[7].im * t1_im;
  absx21 = b_x[7].re * t1_im + b_x[7].im * t1_re;
  if (b_x[4].im == 0.0) {
    if (0.0 - absx21 == 0.0) {
      t2_re = (1.0 - absx11) / b_x[4].re;
      t2_im = 0.0;
    } else if (1.0 - absx11 == 0.0) {
      t2_re = 0.0;
      t2_im = (0.0 - absx21) / b_x[4].re;
    } else {
      t2_re = (1.0 - absx11) / b_x[4].re;
      t2_im = (0.0 - absx21) / b_x[4].re;
    }
  } else if (b_x[4].re == 0.0) {
    if (1.0 - absx11 == 0.0) {
      t2_re = (0.0 - absx21) / b_x[4].im;
      t2_im = 0.0;
    } else if (0.0 - absx21 == 0.0) {
      t2_re = 0.0;
      t2_im = -((1.0 - absx11) / b_x[4].im);
    } else {
      t2_re = (0.0 - absx21) / b_x[4].im;
      t2_im = -((1.0 - absx11) / b_x[4].im);
    }
  } else {
    brm = fabs(b_x[4].re);
    absx31 = fabs(b_x[4].im);
    if (brm > absx31) {
      absx31 = b_x[4].im / b_x[4].re;
      d = b_x[4].re + absx31 * b_x[4].im;
      t2_re = ((1.0 - absx11) + absx31 * (0.0 - absx21)) / d;
      t2_im = ((0.0 - absx21) - absx31 * (1.0 - absx11)) / d;
    } else if (absx31 == brm) {
      if (b_x[4].re > 0.0) {
        absx31 = 0.5;
      } else {
        absx31 = -0.5;
      }
      if (b_x[4].im > 0.0) {
        d = 0.5;
      } else {
        d = -0.5;
      }
      t2_re = ((1.0 - absx11) * absx31 + (0.0 - absx21) * d) / brm;
      t2_im = ((0.0 - absx21) * absx31 - (1.0 - absx11) * d) / brm;
    } else {
      absx31 = b_x[4].re / b_x[4].im;
      d = b_x[4].im + absx31 * b_x[4].re;
      t2_re = (absx31 * (1.0 - absx11) + (0.0 - absx21)) / d;
      t2_im = (absx31 * (0.0 - absx21) - (1.0 - absx11)) / d;
    }
  }
  absx11 = -((b_x[3].re * t2_re - b_x[3].im * t2_im) +
             (b_x[6].re * t1_re - b_x[6].im * t1_im));
  absx21 = -((b_x[3].re * t2_im + b_x[3].im * t2_re) +
             (b_x[6].re * t1_im + b_x[6].im * t1_re));
  if (b_x[0].im == 0.0) {
    if (absx21 == 0.0) {
      y[p2].re = absx11 / b_x[0].re;
      y[p2].im = 0.0;
    } else if (absx11 == 0.0) {
      y[p2].re = 0.0;
      y[p2].im = absx21 / b_x[0].re;
    } else {
      y[p2].re = absx11 / b_x[0].re;
      y[p2].im = absx21 / b_x[0].re;
    }
  } else if (b_x[0].re == 0.0) {
    if (absx11 == 0.0) {
      y[p2].re = absx21 / b_x[0].im;
      y[p2].im = 0.0;
    } else if (absx21 == 0.0) {
      y[p2].re = 0.0;
      y[p2].im = -(absx11 / b_x[0].im);
    } else {
      y[p2].re = absx21 / b_x[0].im;
      y[p2].im = -(absx11 / b_x[0].im);
    }
  } else {
    brm = fabs(b_x[0].re);
    absx31 = fabs(b_x[0].im);
    if (brm > absx31) {
      absx31 = b_x[0].im / b_x[0].re;
      d = b_x[0].re + absx31 * b_x[0].im;
      y[p2].re = (absx11 + absx31 * absx21) / d;
      y[p2].im = (absx21 - absx31 * absx11) / d;
    } else if (absx31 == brm) {
      if (b_x[0].re > 0.0) {
        absx31 = 0.5;
      } else {
        absx31 = -0.5;
      }
      if (b_x[0].im > 0.0) {
        d = 0.5;
      } else {
        d = -0.5;
      }
      y[p2].re = (absx11 * absx31 + absx21 * d) / brm;
      y[p2].im = (absx21 * absx31 - absx11 * d) / brm;
    } else {
      absx31 = b_x[0].re / b_x[0].im;
      d = b_x[0].im + absx31 * b_x[0].re;
      y[p2].re = (absx31 * absx11 + absx21) / d;
      y[p2].im = (absx31 * absx21 - absx11) / d;
    }
  }
  y[p2 + 1].re = t2_re;
  y[p2 + 1].im = t2_im;
  y[p2 + 2].re = t1_re;
  y[p2 + 2].im = t1_im;
  if (b_x[8].im == 0.0) {
    t1_re = 1.0 / b_x[8].re;
    t1_im = 0.0;
  } else if (b_x[8].re == 0.0) {
    t1_re = 0.0;
    t1_im = -(1.0 / b_x[8].im);
  } else {
    brm = fabs(b_x[8].re);
    absx31 = fabs(b_x[8].im);
    if (brm > absx31) {
      absx31 = b_x[8].im / b_x[8].re;
      d = b_x[8].re + absx31 * b_x[8].im;
      t1_re = (absx31 * 0.0 + 1.0) / d;
      t1_im = (0.0 - absx31) / d;
    } else if (absx31 == brm) {
      if (b_x[8].re > 0.0) {
        absx31 = 0.5;
      } else {
        absx31 = -0.5;
      }
      if (b_x[8].im > 0.0) {
        d = 0.5;
      } else {
        d = -0.5;
      }
      t1_re = (absx31 + 0.0 * d) / brm;
      t1_im = (0.0 * absx31 - d) / brm;
    } else {
      absx31 = b_x[8].re / b_x[8].im;
      d = b_x[8].im + absx31 * b_x[8].re;
      t1_re = absx31 / d;
      t1_im = (absx31 * 0.0 - 1.0) / d;
    }
  }
  absx11 = -b_x[7].re * t1_re - -b_x[7].im * t1_im;
  absx21 = -b_x[7].re * t1_im + -b_x[7].im * t1_re;
  if (b_x[4].im == 0.0) {
    if (absx21 == 0.0) {
      t2_re = absx11 / b_x[4].re;
      t2_im = 0.0;
    } else if (absx11 == 0.0) {
      t2_re = 0.0;
      t2_im = absx21 / b_x[4].re;
    } else {
      t2_re = absx11 / b_x[4].re;
      t2_im = absx21 / b_x[4].re;
    }
  } else if (b_x[4].re == 0.0) {
    if (absx11 == 0.0) {
      t2_re = absx21 / b_x[4].im;
      t2_im = 0.0;
    } else if (absx21 == 0.0) {
      t2_re = 0.0;
      t2_im = -(absx11 / b_x[4].im);
    } else {
      t2_re = absx21 / b_x[4].im;
      t2_im = -(absx11 / b_x[4].im);
    }
  } else {
    brm = fabs(b_x[4].re);
    absx31 = fabs(b_x[4].im);
    if (brm > absx31) {
      absx31 = b_x[4].im / b_x[4].re;
      d = b_x[4].re + absx31 * b_x[4].im;
      t2_re = (absx11 + absx31 * absx21) / d;
      t2_im = (absx21 - absx31 * absx11) / d;
    } else if (absx31 == brm) {
      if (b_x[4].re > 0.0) {
        absx31 = 0.5;
      } else {
        absx31 = -0.5;
      }
      if (b_x[4].im > 0.0) {
        d = 0.5;
      } else {
        d = -0.5;
      }
      t2_re = (absx11 * absx31 + absx21 * d) / brm;
      t2_im = (absx21 * absx31 - absx11 * d) / brm;
    } else {
      absx31 = b_x[4].re / b_x[4].im;
      d = b_x[4].im + absx31 * b_x[4].re;
      t2_re = (absx31 * absx11 + absx21) / d;
      t2_im = (absx31 * absx21 - absx11) / d;
    }
  }
  absx11 = -((b_x[3].re * t2_re - b_x[3].im * t2_im) +
             (b_x[6].re * t1_re - b_x[6].im * t1_im));
  absx21 = -((b_x[3].re * t2_im + b_x[3].im * t2_re) +
             (b_x[6].re * t1_im + b_x[6].im * t1_re));
  if (b_x[0].im == 0.0) {
    if (absx21 == 0.0) {
      y[p3].re = absx11 / b_x[0].re;
      y[p3].im = 0.0;
    } else if (absx11 == 0.0) {
      y[p3].re = 0.0;
      y[p3].im = absx21 / b_x[0].re;
    } else {
      y[p3].re = absx11 / b_x[0].re;
      y[p3].im = absx21 / b_x[0].re;
    }
  } else if (b_x[0].re == 0.0) {
    if (absx11 == 0.0) {
      y[p3].re = absx21 / b_x[0].im;
      y[p3].im = 0.0;
    } else if (absx21 == 0.0) {
      y[p3].re = 0.0;
      y[p3].im = -(absx11 / b_x[0].im);
    } else {
      y[p3].re = absx21 / b_x[0].im;
      y[p3].im = -(absx11 / b_x[0].im);
    }
  } else {
    brm = fabs(b_x[0].re);
    absx31 = fabs(b_x[0].im);
    if (brm > absx31) {
      absx31 = b_x[0].im / b_x[0].re;
      d = b_x[0].re + absx31 * b_x[0].im;
      y[p3].re = (absx11 + absx31 * absx21) / d;
      y[p3].im = (absx21 - absx31 * absx11) / d;
    } else if (absx31 == brm) {
      if (b_x[0].re > 0.0) {
        absx31 = 0.5;
      } else {
        absx31 = -0.5;
      }
      if (b_x[0].im > 0.0) {
        d = 0.5;
      } else {
        d = -0.5;
      }
      y[p3].re = (absx11 * absx31 + absx21 * d) / brm;
      y[p3].im = (absx21 * absx31 - absx11 * d) / brm;
    } else {
      absx31 = b_x[0].re / b_x[0].im;
      d = b_x[0].im + absx31 * b_x[0].re;
      y[p3].re = (absx31 * absx11 + absx21) / d;
      y[p3].im = (absx31 * absx21 - absx11) / d;
    }
  }
  y[p3 + 1].re = t2_re;
  y[p3 + 1].im = t2_im;
  y[p3 + 2].re = t1_re;
  y[p3 + 2].im = t1_im;
}

void inv(const double x[4], double y[4])
{
  double b_r;
  double t;
  if (fabs(x[1]) > fabs(x[0])) {
    b_r = x[0] / x[1];
    t = 1.0 / (b_r * x[3] - x[2]);
    y[0] = x[3] / x[1] * t;
    y[1] = -t;
    y[2] = -x[2] / x[1] * t;
    y[3] = b_r * t;
  } else {
    b_r = x[1] / x[0];
    t = 1.0 / (x[3] - b_r * x[2]);
    y[0] = x[3] / x[0] * t;
    y[1] = -b_r * t;
    y[2] = -x[2] / x[0] * t;
    y[3] = t;
  }
}

/* End of code generation (inv.c) */
