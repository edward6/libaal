/*
  malloc.h -- memory allocation functions. ctualy they are memory allocation
  handlers which may be set by caller. In the allone mode they will point to
  corresponding memory allocation functions, which are used in certain alone
  application (bootloaders, etc). In the standard mode they are pointed to libc
  memory allocation functions (malloc, free, etc).
  
  Copyright (C) 2001, 2002, 2003 by Hans Reiser, licensing governed by
  libaal/COPYING.
*/

#ifndef AAL_MALLOC_H
#define AAL_MALLOC_H

#include <aal/types.h>

extern void aal_mem_fini(void);
extern unsigned int aal_mem_free(void);
extern void aal_mem_init(void *start, uint32_t len);

typedef void (*aal_free_handler_t) (void *);
typedef void *(*aal_malloc_handler_t) (uint32_t);
typedef void *(*aal_realloc_handler_t) (void *, uint32_t);

extern void aal_malloc_set_handler(aal_malloc_handler_t handler);
extern aal_malloc_handler_t aal_malloc_get_handler(void);

extern void *aal_malloc(uint32_t size);
extern void *aal_calloc(uint32_t size, char c);

extern int aal_realloc(void** old, uint32_t size);
extern aal_realloc_handler_t aal_realloc_get_handler(void);
extern void aal_realloc_set_handler(aal_realloc_handler_t handler);

extern void aal_free(void *ptr);
extern aal_free_handler_t aal_free_get_handler(void);
extern void aal_free_set_handler(aal_free_handler_t handler);

#endif

