/* Copyright (C) 2001, 2002, 2003 by Hans Reiser, licensing governed by
   libaal/COPYING.
   
   block.h -- block working functions declaration. */

#ifndef AAL_BLOCK_H
#define AAL_BLOCK_H

#include <aal/types.h>

#ifndef ENABLE_STAND_ALONE
extern errno_t aal_block_reread(aal_block_t *block, 
				aal_device_t *device,
				blk_t number);

extern errno_t aal_block_write(aal_block_t *block);

extern void aal_block_move(aal_block_t *block, 
			   blk_t number);
#endif

extern aal_block_t *aal_block_read(aal_device_t *device, 
				   uint32_t size,
				   blk_t number);

extern aal_block_t *aal_block_create(aal_device_t *device, 
				     uint32_t size,
				     blk_t number, char c);

extern void aal_block_free(aal_block_t *block);
extern blk_t aal_block_number(aal_block_t *block);
extern uint32_t aal_block_size(aal_block_t *block);

#endif

