/*
  block.c -- data block functions.

  Copyright (C) 2001, 2002, 2003 by Hans Reiser, licensing governed by
  libaal/COPYING.
*/

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <aal/aal.h>

/* 
  Allocates one block on specified device. Fills its data field by specified
  char. Marks it as ditry and returns it to caller. This function is widely used
  in libreiser4 for working with disk blocks (node.c, almost all plugins).
*/
aal_block_t *aal_block_create(
	aal_device_t *device,	/* device block will eb allocated on */
	uint32_t size,          /* blocksize to be used */
	blk_t number,	        /* block number for allocating */
	char c)			/* char for filling allocated block */
{
	aal_block_t *block;

	aal_assert("umka-443", device != NULL);
	aal_assert("umka-2221", size >= device->blocksize);
    
	if (!(block = aal_calloc(sizeof(*block), 0))) {
		aal_exception_error("Out of memory!");
		return NULL;
	}

	block->size = size;
	block->number = number;
	block->device = device;
	    
	if (!(block->data = aal_calloc(size, c))) {
		aal_exception_error("Out of memory!");
		goto error_free_block;
	}

	return block;
	
 error_free_block:
	aal_free(block);
	return NULL;
}

/*
  Reads one block from specified device. Marks it as clean and returns it to
  caller. For reading is used aal_device_read routine, see above for more
  detailed description.
*/
aal_block_t *aal_block_read(
	aal_device_t *device,	/* device block will be read from */
	uint32_t size,          /* blocksize to be used */
	blk_t number)           /* block number for reading */
{
	uint32_t count;
	aal_block_t *block;

	aal_assert("umka-444", device != NULL);
	aal_assert("umka-2220", size >= device->blocksize);

	/* Allocating new block at passed position blk */    
	if (!(block = aal_block_create(device, size, number, 0)))
		return NULL;

	count = size / device->blocksize;
	
	if (aal_device_read(device, block->data,
			    number * count, count))
	{
		goto error_free_block;
	}

	return block;

 error_free_block:
	aal_block_free(block);
	return NULL;
}

#ifndef ENABLE_STAND_ALONE
/* Makes reread of specified block */
errno_t aal_block_reread(
	aal_block_t *block, 	/* block to be reread */
	aal_device_t *device,	/* device, new block should be reread from */
	blk_t number)           /* block number for rereading */
{
	errno_t res;
	uint32_t count;
	
	aal_assert("umka-631", block != NULL);
	aal_assert("umka-632", device != NULL);

	aal_assert("umka-2222", block->size >=
		   device->blocksize);

	count = block->size / device->blocksize;
	
	if ((res = aal_device_read(device, block->data,
				   number * count, count)))
	{
		return res;
	}

	block->device = device;
	aal_block_move(block, number);

	return 0;
}

/* 
  Writes specified block onto device. Device reference, block will be wrote
  onto, stored in block->device field. Marks it as clean and returns error code
  to caller.
*/
errno_t aal_block_write(
	aal_block_t *block)		/* block for writing */
{
	errno_t res;
	uint32_t count;
	aal_device_t *device;

	aal_assert("umka-446", block != NULL);

	device = block->device;

	count = block->size / device->blocksize;
	
	return aal_device_write(device, block->data,
				block->number * count, count);
}

/* Sets block new number into passed @block */
void aal_block_move(
	aal_block_t *block,		/* block, position will be set to */
	blk_t number)                   /* new block number */
{
	aal_assert("umka-450", block != NULL);
	block->number = number;
}
#endif

/*  Returns block number of specified block */
blk_t aal_block_number(
	aal_block_t *block)		/* block, position will be obtained from */
{
	aal_assert("umka-448", block != NULL);
	return block->number;
}

uint32_t aal_block_size(aal_block_t *block) {
	aal_assert("umka-1049", block != NULL);
	return block->size;
}

/* Frees block instance and all assosiated memory */
void aal_block_free(
	aal_block_t *block)		/* block to be released */
{
	aal_assert("umka-451", block != NULL);
	
	aal_free(block->data);
	aal_free(block);
}
