/*
  print.h -- printing and formating strings functions.

  Copyright (C) 2001, 2002, 2003 by Hans Reiser, licensing governed by
  libaal/COPYING.
*/

#ifndef AAL_PRINT_H
#define AAL_PRINT_H

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <aal/types.h>

#ifdef ENABLE_PRINT_FUNCTIONS

extern int aal_vsnprintf(char *buff, uint32_t n,
			 const char *format, 
			 va_list arg_list);

extern int aal_snprintf(char *buff, uint32_t n,
			const char *format, 
			...);
#else

#include <stdio.h>

#define aal_vsnprintf vsnprintf
#define aal_snprintf snprintf

#endif

#endif

