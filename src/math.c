/* Copyright (C) 2001, 2002, 2003 by Hans Reiser, licensing governed by
   libaal/COPYING.
   
   math.c -- some math functions needed by libaal. */

/* Retuns log2 of passed value */
int aal_log2(unsigned long n) {
	unsigned long x;

	for (x = 0; (unsigned long)(1 << x) <= n; x++);
	return x - 1;
}

/* Returns TRUE if passed value is power of two, FALSE otherwise. This function
   is used for checking block size for validness. */
int aal_pow2(unsigned long n) {
	return (n & -n) == n;
}

/* Returns absolute value of passed @n */
unsigned long aal_abs(long n) {
	return (n > 0) ? n : -n;
}

int aal_min(int v1, int v2) {
	return v1 < v2 ? v1 : v2;
}
