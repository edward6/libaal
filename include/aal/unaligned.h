/* Copyright (C) 2001-2005 by Hans Reiser, licensing governed by libaal/COPYING.
         
   unaligned.h -- libaal unalignment declaration. */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#ifndef AAL_UNALIGNED_H
#define AAL_UNALIGNED_H

#define get_unaligned(ptr)				\
({							\
	__typeof__(*(ptr)) __tmp;			\
	aal_memcpy(&__tmp, (ptr), sizeof(*(ptr)));	\
	__tmp;						\
})

#define put_unaligned(ptr, val)				\
({							\
	__typeof__(*(ptr)) __tmp = (val);		\
	aal_memcpy((ptr), &__tmp, sizeof(*(ptr)));	\
	(void)0;					\
})

#endif
