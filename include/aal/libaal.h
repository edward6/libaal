/* Copyright (C) 2001, 2002, 2003 by Hans Reiser, licensing governed by
   libaal/COPYING.
   
   libaal.h -- the central libaal header. aal - application abstraction library. It
   contains functions which help to work in any environment, out of the box. For
   now libaal supports two envinments: standard (userspace, libc, etc.) and so
   called "minimal" mode - the mode, bootloaders work in (real mode of processor,
   no libc, etc). */

#ifndef AAL_H
#define AAL_H

#ifdef __cplusplus
extern "C" {
#endif
	
#include "types.h"
#include "device.h"
#include "file.h"
#include "exception.h"
#include "list.h"
#include "hash.h"
#include "malloc.h"
#include "print.h"
#include "string.h"
#include "math.h"
#include "bitops.h"
#include "endian.h"
#include "unaligned.h"
#include "debug.h"
#include "gauge.h"
#include "block.h"
#include "stream.h"
#include "ui.h"

extern const char *libaal_version(void);
extern int libaal_max_interface_version(void);
extern int libaal_min_interface_version(void);
	
#ifdef __cplusplus
}
#endif

#endif

