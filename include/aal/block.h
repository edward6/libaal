/*
  block.h -- block working functions declaration.
    
  Copyright (C) 2001, 2002, 2003 by Hans Reiser, licensing governed by
  libaal/COPYING.
*/

#ifndef AAL_BLOCK_H
#define AAL_BLOCK_H

#include <aal/types.h>

#ifndef ENABLE_STAND_ALONE
extern errno_t aal_block_reread(aal_block_t *block, 
				aal_device_t *device,
				blk_t blk);

extern errno_t aal_block_write(aal_block_t *block);

extern void aal_block_move(aal_block_t *block, 
			   blk_t blk);
#endif

extern aal_block_t *aal_block_read(aal_device_t *device, 
				   blk_t blk);

extern aal_block_t *aal_block_create(aal_device_t *device, 
				     blk_t blk, char c);

extern void aal_block_free(aal_block_t *block);
extern blk_t aal_block_number(aal_block_t *block);
extern uint32_t aal_block_size(aal_block_t *block);

#endif

