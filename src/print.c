/* Copyright (C) 2001, 2002, 2003 by Hans Reiser, licensing governed by
   libaal/COPYING.
   
   print.c -- printf and friends. */

#ifndef ENABLE_MINIMAL
#include <aal/libaal.h>
#include <stdio.h>

int aal_vsnprintf(
	char *buff,			    /* buffer string will be formed in */
	uint32_t n,			    /* size of the buffer */
	const char *format,		    /* format string */
	va_list arg_list)		    /* list of parameters */
{
	return vsnprintf(buff, n, format, arg_list);
}

/* Forms string in passed buffer by using format string */
int aal_snprintf(
	char *buff,			    /* buffer string will be formed in */
	uint32_t n,			    /* size of the buffer */
	const char *format,		    /* format string */
	...)				    /* variable list of parametsrs */
{
	int len;
	va_list arg_list;
	
	va_start(arg_list, format);
	len = aal_vsnprintf(buff, n, format, arg_list);
	va_end(arg_list);

	return len;
}
#endif
