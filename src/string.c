/* Copyright (C) 2001, 2002, 2003 by Hans Reiser, licensing governed by
   libaal/COPYING.
   
   string.c -- memory-working and string-working functions. */

#include <aal/libaal.h>

#ifdef ENABLE_MINIMAL

/* Memory and string working functions. They are full analog of standard ones.
   See corresponding man page for details. */
void *aal_memset(void *dest, int c, uint32_t n) {
	char *dest_p = (char *)dest;

	for (; dest_p < (char *)dest + n; dest_p++)
		*dest_p = c;

	return dest;
}

void *aal_memcpy(void *dest, const void *src, uint32_t n) {
	char *src_p;
	char *dest_p; 

	if (dest < src) {
		src_p = (char *)src;
		dest_p = (char *)dest; 

		for (; src_p < (char *)src + n; src_p++, dest_p++)
			*dest_p = *src_p;
		
	} else {
		src_p = (char *)src + n - 1;
		dest_p = (char *)dest + n - 1;

		for (; src_p >= (char *)src; src_p--, dest_p--)
			*dest_p = *src_p;
	}
    
	return dest;
}

void *aal_memmove(void *dest, const void *src, uint32_t n) {
	return aal_memcpy(dest, src, n);
}

int aal_memcmp(const void *s1, const void *s2, uint32_t n) {
	const char *p_s1 = (const char *)s1;
	const char *p_s2 = (const char *)s2;
	
	for (; p_s1 < (char *)s1 + n; p_s1++, p_s2++) {
		if (*p_s1 < *p_s2) 
			return -1;
	
		if (*p_s1 > *p_s2)
			return 1;
	}
	
	return p_s1 != s1 ? 0 : -1;
}

uint32_t aal_strlen(const char *s) {
	uint32_t len = 0;

	while (*s++) len++;
	return len;
}

int aal_strncmp(const char *s1, const char *s2, uint32_t n) {
	return aal_memcmp((const void *)s1, (const void *)s2, n);
}

int aal_strcmp(const char *s1, const char *s2) {
	uint32_t len1 = aal_strlen(s1);
	uint32_t len2 = aal_strlen(s2);
	uint32_t len = len1 < len2 ? len1 : len2;
	int comp;
	
	if ((comp = aal_memcmp((const void *)s1, 
			       (const void *)s2, len)))
	{
		return comp;
	}
	
	return len1 < len2 ? -1 : (len1 > len2 ? 1 : 0);
}

char *aal_strncpy(char *dest, const char *src, uint32_t n) {
	uint32_t src_len = aal_strlen(src);
	uint32_t len = src_len < n ? src_len : n;
	
	aal_memcpy((void *)dest, (const void *)src, len);

	if (len < n)
		*(dest + len) = '\0';
	
	return dest;
}

char *aal_strncat(char *dest, const char *src, uint32_t n) {
	uint32_t src_len = aal_strlen(src);
	uint32_t dest_len = aal_strlen(dest);
	uint32_t used_len = src_len < n ? src_len : n;
	
	aal_memcpy(dest + dest_len, src, used_len);
	
	if (used_len < n)
		*(dest + dest_len) = '\0';
	
	return dest;
}

char *aal_strpbrk(const char *s, const char *accept) {
	char *p_s = (char *)s;
	char *p_a = (char *)accept;
    
	while (*p_s) {
		while (*p_a) {
			if (*p_a == *p_s)
				return p_s;
			p_a++;
		}
		p_s++;
	}
	return NULL;
}

char *aal_strchr(const char *s, int c) {
	char *p_s = (char *)s;
	while (*p_s) {
		if (*p_s == c)
			return p_s;
		p_s++;
	}
	return NULL;
}

char *aal_strrchr(const char *s, int c) {
	char *p_s = ((char *)s + aal_strlen(s) - 1);
	while (p_s != s) {
		if (*p_s == c)
			return p_s;
		p_s--;
	}

	if (*p_s == c)
		return p_s;
	
	return NULL;
}

char *aal_strsep(char **stringp, const char *delim) {
	char *begin, *end;

	begin = *stringp;
    
	if (begin == NULL)
		return NULL;
    
	if (delim[0] == '\0' || delim[1] == '\0') {
		char ch = delim[0];
	
		if (ch == '\0')
			end = NULL;
		else {
			if (*begin == ch)
				end = begin;
			else if (*begin == '\0')
				end = NULL;
			else
				end = aal_strchr(begin + 1, ch);
		}
	} else
		end = aal_strpbrk(begin, delim);
    
	if (end) {
		*end++ = '\0';
		*stringp = end;
	} else
		*stringp = NULL;
    
	return begin;
}

#else

#include <string.h>

void *aal_memset(void *dest, int c, uint32_t n) {
	return memset(dest, c, n);
}

void *aal_memcpy(void *dest, const void *src, uint32_t n) {
	return memcpy(dest, src, n);
}

void *aal_memmove(void *dest, const void *src, uint32_t n) {
	return memmove(dest, src, n);
}

int aal_memcmp(const void *s1, const void *s2, uint32_t n) {
	return memcmp(s1, s2, n);
}

uint32_t aal_strlen(const char *s) {
	return strlen(s);
}

int aal_strncmp(const char *s1, const char *s2, uint32_t n) {
	return strncmp(s1, s2, n);
}

int aal_strcmp(const char *s1, const char *s2) {
	return strcmp(s1, s2);
}

char *aal_strncpy(char *dest, const char *src, uint32_t n) {
	return strncpy(dest, src, n);
}

char *aal_strncat(char *dest, const char *src, uint32_t n) {
	return strncat(dest, src, n);
}

char *aal_strpbrk(const char *s, const char *accept) {
	return strpbrk(s, accept);
}

char *aal_strchr(const char *s, int c) {
	return strchr(s, c);
}

char *aal_strrchr(const char *s, int c) {
	return strrchr(s, c);
}

char *aal_strsep(char **stringp, const char *delim) {
	return strsep(stringp, delim);
}

#endif

char *aal_strndup(const char *s, uint32_t n) {
	char *str = (char *)aal_calloc(n + 1, 0);
	aal_strncpy(str, s, n);

	return str;
}
