/* Copyright (C) 2001, 2002, 2003 by Hans Reiser, licensing governed by
   libaal/COPYING.
   
   types.h -- libaal types declaration. */

#ifndef AAL_TYPES_H
#define AAL_TYPES_H

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

/* Some usefull return codes */
#undef EINVAL
#define EINVAL                  0x1

#undef ENOSPC
#define ENOSPC                  0x2

#undef ENOMEM
#define ENOMEM                  0x3

#undef EIO
#define EIO                     0x4

#undef ESTRUCT
#define ESTRUCT                 0x5

#ifndef __int8_t_defined
#define __int8_t_defined
typedef signed char             int8_t;
typedef short int               int16_t;
typedef int                     int32_t;
__extension__
typedef long long int           int64_t;
#endif
 
typedef unsigned char           uint8_t;
typedef unsigned short int      uint16_t;
#ifndef __uint32_t_defined
#define __uint32_t_defined
typedef unsigned int            uint32_t;
__extension__
typedef unsigned long long int  uint64_t;
#endif

#define MAX_UINT8 (~(uint8_t)0)
#define MAX_UINT16 (~(uint16_t)0)
#define MAX_UINT32 (~(uint32_t)0)
#define MAX_UINT64 (~(uint64_t)0)

/* Types and macros for working with variable length params. They are needed
   because we don't want use gcc builtins in alone mode for achive as small
   binary size as possible. */

#ifndef ENABLE_STAND_ALONE
#  include <stdarg.h>
#else
typedef char *va_list;
#undef va_arg
#undef va_end
#undef va_start

#define va_end(ap)       \
        do {} while(0);

#define va_start(ap, p)  \
        (ap = (char *)(&(p)+1))

#define va_arg(ap, type) \
        ((type *)(ap += sizeof(type)))[-1]
#endif

/* As libaal may be used without any standard headers, we need to declare NULL
   macro here in order to avoid compilation errors. */
#undef NULL

#if defined(__cplusplus)
#  define NULL 0
#else
#  define NULL ((void *)0)
#endif

/* Here we define FALSE and TRUE macros in order to make sources more clean for
   understanding. I mean, that there where we need some boolean value, we will
   use these two macro. */
#if !defined(FALSE)
#  define FALSE 0
#endif

#if !defined(TRUE)
#  define TRUE 1
#endif

/* Simple type for direction denoting */
enum aal_direction {
	D_TOP    = 1 << 0,
	D_BOTTOM = 1 << 1,
	D_LEFT   = 1 << 2,
	D_RIGHT  = 1 << 3
};

typedef enum aal_direction aal_direction_t;

typedef int bool_t;

/* This type is used for return of result of execution some function. */
typedef int errno_t;

typedef struct aal_list aal_list_t;

/* This is the struct that describes one list element. It contains: pointer to
   data assosiated with this item of list, pointer to next element of list and
   pointer to prev element of list. */
struct aal_list {
	void *data;
    
	aal_list_t *next;
	aal_list_t *prev;
};

typedef struct aal_hash_node aal_hash_node_t;

struct aal_hash_node {
	void *key;
	void *value;
	
	aal_hash_node_t *next;
};

/* Type for hash fucntion */
typedef uint64_t (*hash_func_t) (const void *);

/* Type for callback compare function. It is used in list and hash functions for
   comparing their items. */
typedef int (*comp_func_t) (const void *, const void *, void *);

struct aal_hash_table {
	uint32_t size;
	uint32_t real;

	hash_func_t hash_func;
	comp_func_t comp_func;
	aal_hash_node_t **nodes;
};

typedef struct aal_hash_table aal_hash_table_t;

/* Type for callback function that is called for each element of list. Usage is
   the same as previous one. */
typedef int (*foreach_func_t) (const void *, void *);

/* This types is used for keeping the block number and block count value. They
   are needed to be increase source code maintainability.

   For instance, there is some function:

   blk_t some_func(void);
    
   It is clear to any reader, that this function is working with block number,
   it returns block number.

   Yet another variant of this function:

   uint64_t some_func(void);
    
   This variant of the function may return anything. This may be bytes, blocks,
   etc. */

typedef uint64_t blk_t;
typedef uint64_t count_t;
#define INVAL_BLK MAX_UINT64

struct aal_device_ops;

/* Abstract device structure. It consists of flags device opened with, user
   specified data, some opaque entity (for standard file it is file descriptor),
   name of device (for instance, /dev/hda2), block size of device and device
   operations. */
struct aal_device {
	
	int flags;
	
	void *data;
	void *entity;
	void *person;
	
	char name[256];
	char error[256];

	uint32_t blksize;
	struct aal_device_ops *ops;
};

typedef struct aal_device aal_device_t;

/* Operations which may be performed on the device. Some of them may not be
   implemented. */
struct aal_device_ops {
	errno_t (*open) (aal_device_t *, void *,
			 uint32_t, int);
	
	errno_t (*read) (aal_device_t *, 
			 void *, blk_t, count_t);
    
	errno_t (*write) (aal_device_t *, 
			 void *, blk_t, count_t);
    
	errno_t (*sync) (aal_device_t *);
    
	errno_t (*equals) (aal_device_t *, 
			   aal_device_t *);
    
	count_t (*len) (aal_device_t *);
	void (*close) (aal_device_t *);
};

/* Disk block structure. It is a replica of struct buffer_head from the linux
   kernel. It consists of flags (dirty, clean, etc), data (pointer to data of
   block), block size, offset (offset in bytes where block is placed on device),
   and pointer to device, block opened on. */
struct aal_block {
	blk_t nr;
	int dirty;
	void *data;
	uint32_t size;
	aal_device_t *device;
};

typedef struct aal_block aal_block_t;

/* This is the type of exception */
enum aal_exception_type {
	EXCEPTION_INFORMATION   = 1,
	EXCEPTION_MESSAGE       = 2,
	EXCEPTION_WARNING       = 3,
	EXCEPTION_ERROR	        = 4,
	EXCEPTION_FATAL	        = 5,
	EXCEPTION_BUG	        = 6
};

typedef enum aal_exception_type aal_exception_type_t;

enum aal_exception_option {
	EXCEPTION_UNHANDLED     = 1 << 0,
	EXCEPTION_YES	        = 1 << 1,
	EXCEPTION_NO	        = 1 << 2,
	EXCEPTION_OK	        = 1 << 3,
	EXCEPTION_RETRY	        = 1 << 4,
	EXCEPTION_IGNORE        = 1 << 5,
	EXCEPTION_CANCEL        = 1 << 6,
	EXCEPTION_LAST		= 1 << 7
};

typedef enum aal_exception_option aal_exception_option_t;

#define EXCEPTION_YESNO		(EXCEPTION_YES | EXCEPTION_NO)
#define EXCEPTION_OKCANCEL	(EXCEPTION_OK | EXCEPTION_CANCEL)
#define EXCEPTION_RETRYIGNORE	(EXCEPTION_RETRY | EXCEPTION_IGNORE)

/* Format checking stuff */
#ifdef __GNUC__
#define __aal_check_format(style, format, begin) \
        __attribute__((__format__(style, format, begin)))
#else
        __aal_check_format(style, format, begin)
#endif

/* This is exception structure. It contains: exception message, exception type,
   exception options. Usualy, the life cycle of exception is very short.
   Exception instance created by aal_exception_throw function and passed t
   exception handler. After exception processed, it is destroyed by exception
   factory. */
struct aal_exception {
	char *message;
	aal_exception_type_t type;
	aal_exception_option_t options;
};

typedef struct aal_exception aal_exception_t;

typedef aal_exception_option_t (*aal_exception_handler_t) (aal_exception_t *ex);

typedef struct aal_gauge aal_gauge_t;

enum aal_gauge_state {
	GAUGE_STARTED,
	GAUGE_RUNNING,
	GAUGE_PAUSED,
	GAUGE_DONE,
};

typedef enum aal_gauge_state aal_gauge_state_t;

typedef void (*aal_gauge_handler_t)(aal_gauge_t *);

struct aal_gauge_type {
	uint32_t type;
	aal_gauge_handler_t handler;
};

typedef struct aal_gauge_type aal_gauge_type_t;

#define MAX_GAUGES 10

struct aal_gauge {
	void *data;
    
	uint32_t type;
	uint32_t value;
	char name[256];

	aal_gauge_state_t state;
};

struct aal_stream {
	int size;
	int offset;
	void *data;
};

typedef struct aal_stream aal_stream_t;

typedef void (*assert_handler_t) (char *, int, char *,
				  char *, int, char *);

#endif
