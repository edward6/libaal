/* Copyright (C) 2001, 2002, 2003 by Hans Reiser, licensing governed by
   libaal/COPYING.
   
   print.h -- printing and formating strings functions. */

#ifndef AAL_PRINT_H
#define AAL_PRINT_H

#ifndef ENABLE_MINIMAL
#include <aal/types.h>

extern int aal_vsnprintf(char *buff, uint32_t n,
			 const char *format, 
			 va_list arg_list);

extern int aal_snprintf(char *buff, uint32_t n,
			const char *format, 
			...);
#endif

#endif

