/* Copyright (C) 2001, 2002, 2003 by Hans Reiser, licensing governed by
   libaal/COPYING.
   
   exception.h -- exception factory functions. */

#ifndef AAL_EXCEPTION_H
#define AAL_EXCEPTION_H

#include <aal/types.h>

#ifndef ENABLE_MINIMAL
extern void aal_exception_on(void);
extern void aal_exception_off(void);

extern char *aal_exception_type_name(aal_exception_type_t type);
extern char *aal_exception_option_name(aal_exception_option_t opt);

extern char *aal_exception_message(aal_exception_t *ex);
extern aal_exception_type_t aal_exception_type(aal_exception_t *ex);
extern aal_exception_option_t aal_exception_option(aal_exception_t *ex);

extern aal_exception_handler_t aal_exception_get_handler(void);
extern void aal_exception_set_handler(aal_exception_handler_t handler);

extern aal_exception_option_t aal_exception_throw(aal_exception_type_t type, 
						  aal_exception_option_t opt,
						  const char *message, ...)
                                                  __aal_check_format(printf, 3, 4);

#define aal_fatal(msg, list...)       \
        aal_exception_throw(EXCEPTION_TYPE_FATAL, EXCEPTION_OPT_OK, msg, ##list)
	
#define aal_error(msg, list...)       \
        aal_exception_throw(EXCEPTION_TYPE_ERROR, EXCEPTION_OPT_OK, msg, ##list)
	
#define aal_warn(msg, list...)        \
        aal_exception_throw(EXCEPTION_TYPE_WARNING, EXCEPTION_OPT_OK, msg, ##list)
	
#define aal_info(msg, list...)        \
        aal_exception_throw(EXCEPTION_TYPE_INFO, EXCEPTION_OPT_OK, msg, ##list)

#define aal_mess(msg, list...)        \
        aal_exception_throw(EXCEPTION_TYPE_MESSAGE, EXCEPTION_OPT_OK, msg, ##list)

#define aal_yesno(msg, list...)       \
        aal_exception_throw(EXCEPTION_TYPE_MESSAGE, EXCEPTION_OPT_YESNO, msg, ##list)

#define aal_okcancel(msg, list...)    \
        aal_exception_throw(EXCEPTION_TYPE_MESSAGE, EXCEPTION_OPT_OKCANCEL, msg, ##list)

#define aal_retryignore(msg, list...) \
        aal_exception_throw(EXCEPTION_TYPE_ERROR, EXCEPTION_OPT_RETRYIGNORE, msg, ##list)

#else
#define aal_warn(msg, list...)
#define aal_info(msg, list...)
#define aal_mess(msg, list...)
#define aal_fatal(msg, list...)
#define aal_error(msg, list...)
#define aal_yesno(msg, list...)
#define aal_okcancel(msg, list...)
#define aal_retryignore(msg, list...)
#endif

#endif

