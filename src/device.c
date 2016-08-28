/* Copyright (C) 2001, 2002, 2003 by Hans Reiser, licensing governed by
   libaal/COPYING.
   
   device.c -- device independent interface and block-working functions. */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <aal/libaal.h>
#include <fcntl.h>

/* This macro is used for checking whether specified routine from the device
   operations is implemented or not. If not, it throws exception and performs
   specified action.
   
   It is using in the following maner:

   aal_device_check_routine(some_devive_instance, read_operation, goto
   error_processing);
    
   This macro was introdused to decrease source code by removing a lot of common
   pieces and replace them by just one line of macro. */
#ifndef ENABLE_MINIMAL
#define aal_device_check_routine(device, routine, action)		\
    do {								\
	    if (!device->ops->routine) {				\
	        aal_fatal("Device operation \""                         \
                           #routine "\" isn't implemented.");           \
	        action;							\
	    }								\
    } while (0)
#else
#define aal_device_check_routine(device, routine, action)
#endif

/* Initializes device instance, checks and sets all device attributes
   (blocksize, flags, etc) and returns initialized instance to caller. */
aal_device_t *aal_device_open(
	struct aal_device_ops *ops, /* pointer to device operations */
	void *person,               /* device personality (filename, etc) */
	uint32_t blksize,           /* block size device is working with */
	int flags)		    /* flags device opened with (O_RDONLY, etc) */
{
	aal_device_t *device;

	aal_assert("umka-429", ops != NULL);
    
#ifndef ENABLE_MINIMAL
	/* Rough check for blocksize validness */
	if (!aal_pow2(blksize)) {
		aal_error("Block size %u isn't power of two.", blksize);
		return NULL;
	}	
    
	if (blksize < 512) {
		aal_error("Block size can't be less than 512 bytes.");
		return NULL;
	}
#endif
	
	/* Allocating memory for device instance and initializing all fields */
	if (!(device = (aal_device_t *)aal_calloc(sizeof(*device), 0)))
		return NULL;

	device->ops = ops;
	device->flags = flags;
	device->person = person;
	device->blksize = blksize;

	if (ops->open) {
		if (ops->open(device, person, blksize, flags))
			goto error_free_device;
	}

	return device;
	
 error_free_device:
	aal_free(device);
	return NULL;
}

#ifndef ENABLE_MINIMAL
errno_t aal_device_reopen(
	aal_device_t *device,       /* device for reopening */
	uint32_t blksize,           /* block size device is working with */
	int flags)		    /* flags device opened with (READ, WRITE...) */
{
	device->flags = flags;
	device->blksize = blksize;

	return device->ops->open(device, device->person,
				 blksize, flags);
}

bool_t aal_device_readonly(aal_device_t *device) {
	aal_assert("umka-1291", device != NULL);
	return ((device->flags & 7) == O_RDONLY);
}

/* Performs write operation on specified device. Actualqy it calls corresponding
   operation (write) from assosiated with device operations. Returns error code,
   see types.h for more detailed description of errno_t. */
errno_t aal_device_write(
	aal_device_t *device,	/* device instance we will write into */
	void *buff,		/* buffer with data to be wrote */
	blk_t block,		/* block we will write to */
	count_t count)		/* number of blocks to be wrote */
{
	aal_assert("umka-434", device != NULL);
	aal_assert("umka-435", buff != NULL);
	
	aal_device_check_routine(device, write, return -EINVAL);
	return device->ops->write(device, buff, block, count);
}

/* Performs discard operation on specified device. Actualqy it calls corresponding
   operation (discard) from assosiated with device operations. Returns error code,
   see types.h for more detailed description of errno_t. */
errno_t aal_device_discard(
	aal_device_t *device,	/* device instance we will discard */
	blk_t block,		/* block we will begin discarding from */
	count_t count)		/* number of blocks to be discarded */
{
	aal_assert("intelfx-1", device != NULL);

	aal_device_check_routine(device, discard, return -EINVAL);
	return device->ops->discard(device, block, count);
}

/* Performs sync operation on specified device. Actualy it calls corresponding
   operation (sync) from assosiated with device operations. Returns error code,
   see types.h for more detailed description of errno_t. */
errno_t aal_device_sync(
	aal_device_t *device)	/* device instance that will be synchronized */
{
	aal_assert("umka-436", device != NULL);
    
	aal_device_check_routine(device, sync, return -EINVAL);
	return device->ops->sync(device);
}

/* Compares two devices. Returns TRUE for equal devices and FALSE for different
   ones. This function is needed in order to be aware is host device user has
   specified corresponds journal one. And in some other cases. */
bool_t aal_device_equals(
	aal_device_t *device1,	/* first device for comparing */
	aal_device_t *device2)	/* second one */
{
	aal_assert("umka-438", device1 != NULL);
	aal_assert("umka-439", device2 != NULL);
	
	aal_device_check_routine(device1, equals, return 0);
	return device1->ops->equals(device1, device2);
}
#endif

/* Checks and sets new block size for specified device. Returns error code, see
   types.h for more detailed description of errno_t. */
errno_t aal_device_set_bs(
	aal_device_t *device,	/* device to be set with passed blocksize */
	uint32_t blksize)       /* new blocksize value */
{
	aal_assert("umka-431", device != NULL);

#ifndef ENABLE_MINIMAL
	if (!aal_pow2(blksize)) {
		aal_error("Block size %u isn't power of two.", blksize);
		return -EINVAL;
	}	
    
	if (blksize < 512) {
		aal_error("Block size can't be less than 512 bytes.");
		return -EINVAL;
	}
#endif

	device->blksize = blksize;
	return 0;
}

/* Closes device. Frees all assosiated memory */
void aal_device_close(
	aal_device_t *device)	/* device to be closed */
{
	aal_assert("umka-430", device != NULL);

	if (device->ops->close)
		device->ops->close(device);
	
	aal_free(device);
}

/* Returns current block size from specified device */
uint32_t aal_device_get_bs(
	aal_device_t *device)	/* device blocksize will be received from */
{
	aal_assert("umka-432", device != NULL);
	return device->blksize;
}

/* Performs read operation on specified device. Actualy it calls corresponding
   operation (read) from assosiated with device operations. Returns error code,
   see types.h for more detailed description of errno_t. */
errno_t aal_device_read(
	aal_device_t *device,	/* device instance we will read from */
	void *buff,		/* buffer we will read into */
	blk_t block,		/* block number to be read from */
	count_t count)		/* count of blocks to be read */
{
	aal_assert("umka-433", device != NULL);

	aal_device_check_routine(device, read, return -EINVAL);
	return device->ops->read(device, buff, block, count);
}

/* Returns device length in blocks */
count_t aal_device_len(
	aal_device_t *device)	/* device, length in blocks will be get from */
{
	aal_assert("vpf-216", device != NULL);

	aal_device_check_routine(device, len,
				 return INVAL_BLK);
	
	return device->ops->len(device);
}
