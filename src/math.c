/*
  math.c -- some math functions needed by libaal.
    
  Copyright (C) 2001, 2002, 2003 by Hans Reiser, licensing governed by
  libaal/COPYING.
*/

/* Retuns log2 of passed value */
int aal_log2(unsigned long n) {
	unsigned long x;

	for (x = 0; (unsigned long)(1 << x) <= n; x++);
	return x - 1;
}

/*
  Returns TRUE if passed value is power of two, FALSE otherwise. This function
  is used for checking block size for validness.
*/
int aal_pow2(unsigned long n) {
	return (n & -n) == n;
}
