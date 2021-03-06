/* Copyright (C) 2001, 2002, 2003 by Hans Reiser, licensing governed by
   libaal/COPYING.
   
   string.h -- memory-working and string-working functions. They are needed in
   order to be independent from specific application. As libreiser4 is used
   string functions, we should provide them for it, because in minimal mode they
   doesn't exist due to libc is not in use. */

#ifndef AAL_STRING_H
#define AAL_STRING_H

#include <aal/types.h>

extern void *aal_memset(void *dest, int c, uint32_t n);
extern void *aal_memcpy(void *dest, const void *src, uint32_t n);
extern void *aal_memmove(void *dest, const void *src, uint32_t n);
extern int aal_memcmp(const void *s1, const void *s2, uint32_t n);

extern char *aal_strncpy(char *dest, const char *src, uint32_t n);
extern char *aal_strncat(char *dest, const char *src, uint32_t n);
extern int aal_strncmp(const char *s1, const char *s2, uint32_t n);
extern int aal_strcmp(const char *s1, const char *s2);
extern uint32_t aal_strlen(const char *s);

extern char *aal_strpbrk(const char *s, const char *accept);
extern char *aal_strchr(const char *s, int c);
extern char *aal_strrchr(const char *s, int c);
extern char *aal_strsep(char **stringp, const char *delim);

extern char *aal_strndup(const char *s, uint32_t n);

#endif

