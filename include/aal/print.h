/*
  print.h -- printing and formating strings functions.

  Copyright (C) 2001, 2002, 2003 by Hans Reiser, licensing governed by
  libaal/COPYING.
*/

#ifndef AAL_PRINT_H
#define AAL_PRINT_H

#include <aal/types.h>

#if defined(ENABLE_STAND_ALONE) && defined(ENABLE_PRINT_FUNCTIONS)

extern int aal_vsnprintf(char *buff, uint32_t n,
			 const char *format, 
			 va_list arg_list);

extern int aal_snprintf(char *buff, uint32_t n,
			const char *format, 
			...);

extern int aal_stoa(int d, uint32_t n, char *a,
		    int base, int flags);

extern int aal_lstoa(long int d, uint32_t n, char *a,
		     int base, int flags);


extern int aal_utoa(unsigned int d, uint32_t n, char *a,
		    int base, int flags);

extern int aal_lutoa(unsigned long int d, uint32_t n, char *a,
		     int base, int flags);

#else

#include <stdio.h>
#define aal_vsnprintf vsnprintf
#define aal_snprintf snprintf

#endif

#endif

