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
#define EINVAL                  22

#undef ENOSPC
#define ENOSPC                  28

#undef ENOMEM
#define ENOMEM                  12

#undef EIO
#define EIO                     5

#undef ESTRUCT
#define ESTRUCT                 50

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

#define MAX_UINT8  ((uint8_t)~0)
#define MAX_UINT16 ((uint16_t)~0)
#define MAX_UINT32 ((uint32_t)~0)
#define MAX_UINT64 ((uint64_t)~0)

/* Types and macros for working with variable length params. They are needed
   because we don't want use gcc builtins in minimal mode for achive as small
   binary size as possible. */

#ifndef ENABLE_MINIMAL
#  include <stdarg.h>
#else
#ifndef _VA_LIST_
#define _VA_LIST_
typedef char *va_list;
#endif
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

/* Simple type for direction denoting */
enum aal_dir {
	DIR_UP     = 1 << 0,
	DIR_DOWN   = 1 << 1,
	DIR_LEFT   = 1 << 2,
	DIR_RIGHT  = 1 << 3
};

typedef enum aal_direction aal_direction_t;

typedef int bool_t;

/* This type is used for return of result of execution some function. */
typedef int64_t errno_t;

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
typedef uint64_t (*hash_func_t) (void *);

/* Type for callback compare function. It is used in list and hash functions for
   comparing their items. */
typedef int (*comp_func_t) (void *, void *, void *);

/* Key remove functions. */
typedef void (*keyrem_func_t) (void *);
typedef void (*valrem_func_t) (void *);

struct aal_hash_table {
	uint32_t size;
	uint32_t real;

	hash_func_t hash_func;
	comp_func_t comp_func;
	keyrem_func_t keyrem_func;
	valrem_func_t valrem_func;
	
	aal_hash_node_t **nodes;
};

typedef struct aal_hash_table aal_hash_table_t;

/* Type for callback function that is called for each element of list or hash
   table. */
typedef errno_t (*foreach_func_t) (void *, void *);

/* These types is used for keeping the block number and block count value. They
   are needed to increase source code maintainability.

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

/* This is the type of exception. */
enum aal_exception_type {
	EXCEPTION_TYPE_INFO        = 1,
	EXCEPTION_TYPE_MESSAGE     = 2,
	EXCEPTION_TYPE_WARNING     = 3,
	EXCEPTION_TYPE_ERROR	   = 4,
	EXCEPTION_TYPE_FATAL	   = 5,
	EXCEPTION_TYPE_BUG	   = 6,
	EXCEPTION_TYPE_LAST
};

typedef enum aal_exception_type aal_exception_type_t;

enum aal_exception_option {
	EXCEPTION_OPT_UNHANDLED    = 1 << 0,
	EXCEPTION_OPT_YES	   = 1 << 1,
	EXCEPTION_OPT_NO	   = 1 << 2,
	EXCEPTION_OPT_OK	   = 1 << 3,
	EXCEPTION_OPT_RETRY	   = 1 << 4,
	EXCEPTION_OPT_IGNORE       = 1 << 5,
	EXCEPTION_OPT_CANCEL       = 1 << 6,
	EXCEPTION_OPT_LAST         = 1 << 7
};

typedef enum aal_exception_option aal_exception_option_t;

#define EXCEPTION_OPT_YESNO	   (EXCEPTION_OPT_YES | EXCEPTION_OPT_NO)
#define EXCEPTION_OPT_OKCANCEL	   (EXCEPTION_OPT_OK | EXCEPTION_OPT_CANCEL)
#define EXCEPTION_OPT_RETRYIGNORE  (EXCEPTION_OPT_RETRY | EXCEPTION_OPT_IGNORE)

/* Format checking stuff */
#ifdef __GNUC__
#define __aal_check_format(style, format, begin) \
        __attribute__((__format__(style, format, begin)))
#else
#define __aal_check_format(style, format, begin) 
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

/* Gauge related types. */
typedef struct aal_gauge aal_gauge_t;

enum aal_gauge_state {
	GS_START,
	GS_ACTIVE,
	GS_PAUSE,
	GS_RESUME,
	GS_DONE,
	GS_LAST
};

typedef enum aal_gauge_state aal_gauge_state_t;

typedef void (*aal_gauge_handler_t)(aal_gauge_t *);

struct aal_gauge_time {
	/* Time in sec, millisec. */
	uint64_t sec, misec;
	/* Interval in milliseconds gauge should be shown once. */
	uint64_t gap;
};

typedef struct aal_gauge_time aal_gauge_time_t;

struct aal_gauge {
	aal_gauge_handler_t handler;
	aal_gauge_state_t state;
	char label[80];
	
	/* Prepare the value if needed. */
	aal_gauge_handler_t value_func;
	aal_gauge_time_t time;
	int64_t value;
	void *data;
};

/* Stream types. */
typedef struct aal_stream aal_stream_t;

/* Stream protocol. It consist of read(), write() and fini() methods and needed
   for implementing differnt streams with the same interface. There are two
   implementation: memory stream and file stream. */
struct aal_proto {
	int (*eof) (aal_stream_t *);
	void (*fini) (aal_stream_t *);
	int32_t (*read) (aal_stream_t *, void *, uint32_t);
	int32_t (*write) (aal_stream_t *, void *, uint32_t);
};

typedef struct aal_proto aal_proto_t;

/* Stream itself. */
struct aal_stream {
	void *entity;
	uint32_t size;
	uint32_t offset;
	aal_proto_t *proto;
};

/* Public stream protocols. May be used for stream initializing. */
extern aal_proto_t file_stream;
extern aal_proto_t memory_stream;

/* Assert handler type. */
typedef void (*assert_handler_t) (char *, int, char *,
				  char *, int, char *);
#endif
