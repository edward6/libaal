/* Copyright (C) 2001, 2002, 2003 by Hans Reiser, licensing governed by
   libaal/COPYING.
   
   block.h -- block working functions declaration. */

#ifndef AAL_BLOCK_H
#define AAL_BLOCK_H

#include <aal/types.h>

#ifndef ENABLE_MINIMAL
extern void aal_block_move(aal_block_t *block,
			   aal_device_t *device,
			   blk_t nr);

extern errno_t aal_block_write(aal_block_t *block);
#endif

extern void aal_block_free(aal_block_t *block);
extern void aal_block_fini(aal_block_t *block);
extern errno_t aal_block_read(aal_block_t *block);

extern errno_t aal_block_fill(aal_block_t * block,
			      unsigned char c);

extern errno_t aal_block_init(aal_block_t *block,
			      aal_device_t *device, 
			      uint32_t size, blk_t nr);

extern aal_block_t *aal_block_alloc(aal_device_t *device, 
				    uint32_t size, blk_t nr);

extern aal_block_t *aal_block_load(aal_device_t *device, 
				   uint32_t size, blk_t nr);
#endif

