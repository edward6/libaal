/* Copyright (C) 2001, 2002, 2003 by Hans Reiser, licensing governed by
   libaal/COPYING.
   
   debug.c -- implements assertions through exceptions factory. That is if some
   exception occurs, user will have the ability to make choise, continue running
   or not. */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#if !defined(ENABLE_MINIMAL) && defined(ENABLE_DEBUG)

#include <stdlib.h>
#include <aal/libaal.h>

static void default_assert_handler(char *hint, int cond, char *text,
				   char *file, int line, char *func)
{
	/* Actual exception throwing. Messages will contain hint for owner,
	   file, line and function assertion was failed in. */ 
	aal_exception_throw(EXCEPTION_TYPE_BUG, EXCEPTION_OPT_OK,
			    "%s: Assertion (%s) at %s:%d in function "
			    "%s() failed.", hint, text, file, line, func);

	abort();
}

static assert_handler_t assert_handler = default_assert_handler;

assert_handler_t aal_assert_get_handler(void) {
	return assert_handler;
}

void aal_assert_set_handler(assert_handler_t handler) {
	if (!handler)
		handler = default_assert_handler;
	
	assert_handler = handler;
}
#endif

void __actual_bug(
	char *hint,	     /* person owner of assert */
	char *file,	     /* source file assertion was failed in */
	int line,	     /* line of code assertion was failed in */
	char *func,          /* function in code assertion was failed in */
	char *text,	     /* text of the assertion */
	...)
{
#if !defined(ENABLE_MINIMAL) && defined(ENABLE_DEBUG)
	char desc[512];
	va_list arg_list;
	
	va_start(arg_list, text);
	aal_memset(desc, 0, sizeof(desc));
	aal_vsnprintf(desc, 256, text, arg_list);
	va_end(arg_list);
	
	aal_exception_throw(EXCEPTION_TYPE_BUG, EXCEPTION_OPT_OK,
			    "%s: Bug at %s:%d in function %s(). "
			    "%s.", hint, file, line, func, desc);
	abort();
#endif
}

/* This function is used to provide asserts via exceptions. It is used by macro
   aal_assert(). */
void __actual_assert(
	char *hint,	     /* person owner of assert */
	int cond,	     /* condition of assertion */
	char *text,	     /* text of the assertion */
	char *file,	     /* source file assertion was failed in */
	int line,	     /* line of code assertion was failed in */
	char *func)          /* function in code assertion was failed in */
{
#if !defined(ENABLE_MINIMAL) && defined(ENABLE_DEBUG)
	/* Checking the condition and assert handler validness */
	if (!cond && assert_handler)
		assert_handler(hint, cond, text, file, line, func);
#endif
}

