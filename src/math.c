/* Copyright (C) 2001, 2002, 2003 by Hans Reiser, licensing governed by
   libaal/COPYING.
   
   math.c -- some math functions needed by libaal. */

#include <aal/types.h>

/* Retuns log2 of passed value */
int aal_log2(uint32_t n) {
	uint32_t x;

	for (x = 0; (uint32_t)(1 << x) <= n; x++);
	return x - 1;
}

/* Returns TRUE if passed value is power of two, FALSE otherwise. This function
   is used for checking block size for validness. */
int aal_pow2(uint32_t n) {
	return (n & -n) == n;
}

/* Returns absolute value of passed @n */
uint32_t aal_abs(int32_t n) {
	return (n > 0) ? n : -n;
}

int aal_min(int32_t v1, int32_t v2) {
	return v1 < v2 ? v1 : v2;
}

static inline void
aal_divl(uint32_t high, uint32_t low, uint32_t div,
	 uint32_t *q, uint32_t *r)
{
        uint64_t n = (uint64_t) high << 32 | low;
        uint64_t d = (uint64_t) div << 31;
        uint32_t q1 = 0;
        int c = 32;
	
        while (n > 0xffffffffU) {
                q1 <<= 1;
                if (n >= d) {
                        n -= d;
                        q1 |= 1;
                }
                d >>= 1;
                c--;
        }
        q1 <<= c;
        if (n) {
                low = n;
                *q = q1 | (low / div);
                if (r)
                        *r = low % div;
        } else {
                if (r)
                        *r = 0;
                *q = q1;
        }
}
                                                                                        
/* Function for 64 bits numbers division. */
uint64_t aal_div64(uint64_t n, uint32_t div, uint32_t *rem) {
        uint32_t low, high;
	
        low = n & 0xffffffff;
        high = n >> 32;
	
        if (high) {
                uint32_t high1 = high % div;
                uint32_t low1 = low;
                high /= div;
                aal_divl(high1, low1, div, &low, rem);
                return (uint64_t) high << 32 | low;
        } else {
                if (rem)
                        *rem = low % div;
                return low / div;
        }
}

uint32_t aal_mod64(uint64_t n, uint32_t div) {
	uint32_t rem;

	aal_div64(n, div, &rem);
	return rem;
}
