/* Copyright (C) 2001, 2002, 2003 by Hans Reiser, licensing governed by
   libaal/COPYING.
   
   bitops.h -- bitops functions. */

#ifndef AAL_BITOPS_H
#define AAL_BITOPS_H

#ifndef ENABLE_MINIMAL
typedef uint64_t bit_t;

extern int aal_set_bit(void *map, bit_t nr);
extern int aal_clear_bit(void *map, bit_t nr);
extern int aal_test_bit(void *map, bit_t nr);

extern bit_t aal_find_first_zero_bit(void *map, bit_t size);

extern bit_t aal_find_next_zero_bit(void *map, bit_t size,
				    bit_t offset);

extern bit_t aal_find_next_set_bit(void *map, bit_t size,
				   bit_t offset);

extern bit_t aal_find_zero_bits(void *map, bit_t size,
				bit_t *start, bit_t count);

extern bit_t aal_find_set_bits(void *map, bit_t size,
			       bit_t *start, bit_t count);

extern void aal_clear_bits(void *map, bit_t start,
			   bit_t count);

extern void aal_set_bits(void *map, bit_t start,
			 bit_t count);
#endif

#endif
