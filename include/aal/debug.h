/*
  debug.h -- assert implementation.
    
  Copyright (C) 2001, 2002, 2003 by Hans Reiser, licensing governed by
  libaal/COPYING.
*/

#ifndef AAL_DEBUG_H
#define AAL_DEBUG_H

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#if !defined(ENABLE_STAND_ALONE) && defined(ENABLE_DEBUG)

extern void __actual_assert(char *hint, int cond, char *text,
			    char *file, int line, char *func);

/*
  Something like standard assert, but working through exception 
  factory.
*/
#ifdef __GNUC__
#define aal_assert(hint, cond)          \
    	__actual_assert(hint, cond,     \
	   #cond,                       \
	    __FILE__,                   \
	    __LINE__,                   \
	    __PRETTY_FUNCTION__)

#else
#define aal_assert(hint, cond)          \
	__actual_assert(hint, cond,     \
	    #cond,                      \
	    "unknown",                  \
	    0,                          \
	    "unknown")

#endif

#else
#define aal_assert(hint, cond) while (0) {}
#endif

extern assert_handler_t aal_assert_get_handler(void);
extern void aal_assert_set_handler(assert_handler_t handler);

#endif

