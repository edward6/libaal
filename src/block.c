/* Copyright (C) 2001, 2002, 2003 by Hans Reiser, licensing governed by
   libaal/COPYING.
   
   block.c -- data block functions. */

#include <aal/libaal.h>

errno_t aal_block_init(aal_block_t *block,
		       aal_device_t *device,
		       uint32_t size, blk_t nr)
{
	aal_assert("umka-2371", block != NULL);
	aal_assert("umka-2372", device != NULL);

	block->nr = nr;
	block->dirty = 0;
	block->size = size;
	block->device = device;
	    
	if (!(block->data = aal_malloc(size)))
		return -ENOMEM;

	return 0;
}

void aal_block_fini(aal_block_t *block) {
	aal_free(block->data);
	block->data = NULL;
}

/* Allocates one block on specified device. Fills its data field by specified
   char. Marks it as ditry and returns it to caller. This function is widely
   used in libreiser4 for working with disk blocks (node.c, almost all
   plugins). */
aal_block_t *aal_block_alloc(
	aal_device_t *device,	/* device block will eb allocated on */
	uint32_t size,          /* blocksize to be used */
	blk_t nr)	        /* block number for allocating */
{
	aal_block_t *block;

	aal_assert("umka-443", device != NULL);
    
	if (!(block = aal_calloc(sizeof(*block), 0)))
		return NULL;

	if (aal_block_init(block, device, size, nr))
		goto error_free_block;

	return block;
	
 error_free_block:
	aal_free(block);
	return NULL;
}

/* Frees block instance and all assosiated memory */
void aal_block_free(
	aal_block_t *block)		/* block to be released */
{
	aal_assert("umka-451", block != NULL);

	aal_block_fini(block);
	aal_free(block);
}

errno_t aal_block_fill(aal_block_t *block, unsigned char c) {
	aal_assert("umka-2370", block != NULL);

	if (!aal_memset(block->data, c, block->size))
		return -EINVAL;

	block->dirty = 1;
	return 0;
}

errno_t aal_block_read(aal_block_t *block) {
	uint32_t count;
	
	aal_assert("umka-2369", block != NULL);

	block->dirty = 0;
	count = block->size / block->device->blksize;

	return aal_device_read(block->device, block->data,
			       block->nr * count, count);
}

/* Reads one block from specified device. Marks it as clean and returns it to
   caller. For reading is used aal_device_read routine, see above for more
   detailed description. */
aal_block_t *aal_block_load(
	aal_device_t *device,	/* device block will be read from */
	uint32_t size,          /* blocksize to be used */
	blk_t nr)               /* block number for reading */
{
	aal_block_t *block;

	aal_assert("umka-444", device != NULL);

	/* Allocating new block at passed position blk */    
	if (!(block = aal_block_alloc(device, size, nr)))
		return NULL;

	if (aal_block_read(block))
		goto error_free_block;

	return block;

 error_free_block:
	aal_block_free(block);
	return NULL;
}

#ifndef ENABLE_MINIMAL
/* Sets block new number into passed @block */
void aal_block_move(
	aal_block_t *block,		/* block, position will be set to */
	aal_device_t *device,           /* new device to be assigned */
	blk_t nr)                       /* new block number */
{
	aal_assert("umka-450", block != NULL);

	block->nr = nr;
	block->dirty = 1;
	block->device = device;
}

/* Writes specified block onto device. Device reference, block will be wrote
   onto, stored in block->device field. Marks it as clean and returns error code
   to caller. */
errno_t aal_block_write(
	aal_block_t *block)		/* block for writing */
{
	uint32_t count;

	aal_assert("umka-446", block != NULL);

	block->dirty = 0;
	count = block->size / block->device->blksize;
	
	return aal_device_write(block->device, block->data,
				block->nr * count, count);
}
#endif
