/* Copyright (C) 2001, 2002, 2003 by Hans Reiser, licensing governed by
   libaal/COPYING.
   
   math.h -- some math functions which are needed by libaal. */

#ifndef AAL_MATH_H
#define AAL_MATH_H

#include "types.h"

extern int aal_log2(uint32_t n);
extern int aal_pow2(uint32_t n);
extern uint32_t aal_abs(int32_t n);

extern int aal_min(int32_t v1, int32_t v2);
uint32_t aal_mod64(uint64_t n, uint32_t div);
uint64_t aal_div64(uint64_t n, uint32_t div, uint32_t *rem);
#endif

