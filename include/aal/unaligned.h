/* Copyright (C) 2001-2005 by Hans Reiser, licensing governed by libaal/COPYING.

   unaligned.h -- libaal unalignment declaration. */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#ifndef AAL_UNALIGNED_H
#define AAL_UNALIGNED_H

/* unaligned access is allowed on cris, i386, ppc, ppc64, s390, x86_64 */
#if defined(i386)    || defined (__i386__)   ||		\
    defined(ppc)     || defined(__ppc__)     ||		\
    defined(PPC)     || defined(__ppc)       ||		\
    defined(__PPC__) || defined(__powerpc__) ||		\
    defined(powerpc) || defined(__x86_64__)  ||		\
    defined(__s390__) || defined(__cris__)

#define get_unaligned(ptr) (*(ptr))
#define put_unaligned(val, ptr) ((void)( *(ptr) = (val) ))

#else

#define get_unaligned(ptr)				\
({							\
	__typeof__(*(ptr)) __tmp;			\
	aal_memcpy(&__tmp, (ptr), sizeof(*(ptr)));	\
	__tmp;						\
})

#define put_unaligned(val, ptr)				\
({							\
	__typeof__(*(ptr)) __tmp = (val);		\
	aal_memcpy((ptr), &__tmp, sizeof(*(ptr)));	\
	(void)0;					\
})
#endif

#endif
