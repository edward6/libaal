/* Copyright (C) 2001, 2002, 2003 by Hans Reiser, licensing governed by
   libaal/COPYING.
   
   debug.h -- asserts implementation. */

#ifndef AAL_DEBUG_H
#define AAL_DEBUG_H

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

extern void __actual_bug(char *hint, char *file, int line, 
			 char *func, char *text, ...);

extern void __actual_assert(char *hint, int cond, char *text,
			    char *file, int line, char *func);

#if !defined(ENABLE_MINIMAL) && defined(ENABLE_DEBUG)
#ifdef __GNUC__
#define aal_bug(hint, text, list...)         \
    	__actual_bug(hint,                   \
		     __FILE__,		     \
		     __LINE__,		     \
		     __PRETTY_FUNCTION__,    \
		     text,		     \
		     ##list)

#define aal_assert(hint, cond)               \
    	__actual_assert(hint,                \
			cond,		     \
			#cond,		     \
			__FILE__,	     \
			__LINE__,	     \
			__PRETTY_FUNCTION__)

#else
#define aal_bug(hint, text, list...)         \
    	__actual_bug(hint,                   \
		     "unknown",		     \
		     0,		             \
		     "unknown",              \
		     text,		     \
		     ##list)

#define aal_assert(hint, cond)               \
	__actual_assert(hint, cond,          \
			#cond,		     \
			"unknown",	     \
			0,		     \
			"unknown")

#endif

#else
#define aal_bug(hint, text, list...)
#define aal_assert(hint, cond)
#endif

extern assert_handler_t aal_assert_get_handler(void);
extern void aal_assert_set_handler(assert_handler_t handler);
#endif

