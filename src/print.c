/*
  print.c -- output functions and some useful utilities.
    
  Copyright (C) 2001, 2002, 2003 by Hans Reiser, licensing governed by
  libaal/COPYING.
*/

#include <aal/aal.h>

#ifdef ENABLE_STAND_ALONE

enum format_modifier {
	MOD_EMPTY,
	MOD_LONG,
	MOD_LONGER
};

typedef enum format_modifier format_modifier_t;

/* Converts string denoted as size into digits */
#define DCONV_RANGE_DEC (1000000000)
#define DCONV_RANGE_HEX (0x10000000)
#define DCONV_RANGE_OCT (01000000000)

/* 
  Macro for providing function for converting the passed digital into string.
  It supports converting of decimal, hexadecimal and octal digits. It is used by
  aal_vsnprintf function.
*/
#define DEFINE_DCONV(name, type)			                \
int aal_##name##toa(type d, uint32_t n, char *a, int base, int flags) { \
    type s;							        \
    type range;							        \
    char *p = a;						        \
				                                        \
    switch (base) {						        \
	    case 10: range = DCONV_RANGE_DEC; break;			\
	    case 16: range = DCONV_RANGE_HEX; break;			\
	    case 8: range = DCONV_RANGE_OCT; break;			\
	    default: return 0;					        \
    }								        \
    aal_memset(p, 0, n);					        \
								        \
    if (d == 0) {						        \
	    *p++ = '0';						        \
	    return 1;						        \
    }								        \
								        \
    for (s = range; s > 0; s /= base) {                                 \
	    type v = d / s;                                             \
								        \
	    if ((uint32_t)(p - a) >= n)				        \
	            break;			                        \
                                                                        \
	    if (v <= 0)                                                 \
		    continue;                                           \
								        \
	    if (v >= (type)base) {				        \
                    type ds = d / s;                                    \
                    type vb = v / base;                                 \
		    v = ds - (vb * base);		                \
            }                                                           \
	    switch (base) {					        \
		    case 10:                                            \
	            case 8:                                             \
			    *p++ = '0' + v;                             \
			    break;		                        \
		    case 16:					        \
		            if (flags == 0)				\
		                    *p++ = '0' + (v > 9 ? 39 : 0) + v;	\
		            else					\
			            *p++ = '0' + (v > 9 ? 7 : 0) + v;	\
		            break;					\
            }							        \
    }								        \
    return p - a;						        \
}

/*
  Defining the digital to alpha convertiion functions for all supported types
  (%u, %lu, %llu)
*/
DEFINE_DCONV(u, unsigned int)
DEFINE_DCONV(lu, unsigned long int)

DEFINE_DCONV(s, int)
DEFINE_DCONV(ls, long int)

/*
  This function is used for forming a string by passed format string and
  arguments. It is widely used in exception handling and in other places, where
  format string is needed. It is almost full clone of standard libc function. It
  was introduced in order to provide formating strings ability in the allone
  mode.
*/
int aal_vsnprintf(
	char *buff,			    /* buffer string will be formed in */
	uint32_t n,			    /* size of the buffer */
	const char *format,		    /* format string */
	va_list arg_list)		    /* list of parameters */
{
	int i;
	long int li;
	long long int lli;
			
	unsigned int u;
	unsigned long int lu;
	unsigned long long llu;
    
	const char *old = format;
	const char *fmt = format;
    
	format_modifier_t modifier = MOD_EMPTY;

	aal_memset(buff, 0, n);
	
	while (*fmt) {
		if (fmt - format + 1 >= (int)n)
			break;

		modifier = MOD_EMPTY;
		switch (*fmt) {
		case '%': {
			if (aal_strlen(fmt) < 2)
				break;
			
			if (fmt - format > 0)
				aal_memcpy(buff + aal_strlen(buff), old, fmt - old);
		repeat:		
			fmt++;
			switch (*fmt) {
			case 's': {
				char *s;
			
				if (modifier != MOD_EMPTY)
					break;
			
				s = va_arg(arg_list, char *);
				aal_strncat(buff, s, n - aal_strlen(buff));
				fmt++;
				break;
			}
			case 'c' : {
				char c = va_arg(arg_list, int);
				*buff = c; fmt++;
				break;
			}
			case '%' : {
				aal_strncat(buff, "% ", n - aal_strlen(buff));
				fmt++;
				break;
			}
			case 'l': {
				modifier = (modifier == MOD_LONG ? MOD_LONGER : MOD_LONG);
				old++;
				goto repeat;
			}
			case 'd':
			case 'o':
			case 'i':
			case 'u':
			case 'p':
			case 'P':
			case 'X':
			case 'x': {
				char s[32];
			
				aal_memset(s, 0, sizeof(s));
			
				if (*fmt == 'd' || *fmt == 'i') {
					if (modifier == MOD_EMPTY) {
						i = va_arg(arg_list, int);
						aal_stoa(i, sizeof(s), s, 10, 0);
					} else if (modifier == MOD_LONG) {
						li = va_arg(arg_list, long int);
						aal_lstoa(li, sizeof(s), s, 10, 0);
					} else {
						li = va_arg(arg_list, long long int);
						aal_lstoa(li, sizeof(s), s, 10, 0);
					}
					aal_strncat(buff, s, n - aal_strlen(buff));
				} else {
					if (modifier == MOD_EMPTY) {
						u = va_arg(arg_list, unsigned int);
						
						switch (*fmt) {
						case 'u':
							aal_utoa(u, sizeof(s), s, 10, 0);
							break;
						case 'p':
						case 'x':
							aal_utoa(u, sizeof(s), s, 16, 0);
							break;
						case 'P':
						case 'X':
							aal_utoa(u, sizeof(s), s, 16, 1);
							break;
						case 'o':
							aal_utoa(u, sizeof(s), s, 8, 0);
							break;
						}
					} else if (modifier == MOD_LONG) {
						lu = va_arg(arg_list, unsigned long int);
						
						switch (*fmt) {
						case 'u':
							aal_lutoa(lu, sizeof(s), s, 10, 0);
							break;
						case 'x':
							aal_lutoa(lu, sizeof(s), s, 16, 0);
							break;
						case 'X':
							aal_lutoa(lu, sizeof(s), s, 16, 1);
							break;
						case 'o':
							aal_lutoa(lu, sizeof(s), s, 8, 0);
							break;
						}
					} else {
						llu = va_arg(arg_list, unsigned long long);
						
						switch (*fmt) {
						case 'u':
							aal_lutoa(llu, sizeof(s), s, 10, 0);
							break;
						case 'x':
							aal_lutoa(llu, sizeof(s), s, 16, 0);
							break;
						case 'X':
							aal_lutoa(llu, sizeof(s), s, 16, 1);
							break;
						case 'o':
							aal_lutoa(llu, sizeof(s), s, 8, 0);
							break;
						}
					}
					aal_strncat(buff, s, n - aal_strlen(buff));
				}
				fmt++;
			}
			}
			old = fmt;
			break;
		}
		default: fmt++;
		}
	}

	if (fmt - format > 0)
		aal_memcpy(buff + aal_strlen(buff), old, fmt - old);

	return aal_strlen(buff);
}

#else

#include <stdio.h>
int aal_vsnprintf(
	char *buff,			    /* buffer string will be formed in */
	uint32_t n,			    /* size of the buffer */
	const char *format,		    /* format string */
	va_list arg_list)		    /* list of parameters */
{
	return vsnprintf(buff, n, format, arg_list);
}

#endif

/* Forms string in passed buffer by using format string */
int aal_snprintf(
	char *buff,			    /* buffer string will be formed in */
	uint32_t n,			    /* size of the buffer */
	const char *format,		    /* format string */
	...)				    /* variable list of parametsrs */
{
	int len;
	va_list arg_list;
	
	va_start(arg_list, format);
	len = aal_vsnprintf(buff, n, format, arg_list);
	va_end(arg_list);

	return len;
}
