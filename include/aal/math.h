/*
  math.h -- some math functions which are needed by libaal.
  
  Copyright (C) 2001, 2002, 2003 by Hans Reiser, licensing governed by
  libaal/COPYING.
*/

#ifndef AAL_MATH_H
#define AAL_MATH_H

#include <aal/types.h>

extern int aal_log2(unsigned long n);

extern long long int aal_fact(int64_t n);
extern int aal_pow_of_two(unsigned long n);

extern unsigned int aal_adler32(char *buff,
				unsigned int n);

#endif

