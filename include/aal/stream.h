/* Copyright (C) 2001, 2002, 2003 by Hans Reiser, licensing governed by
   libaal/COPYING.
   
   stream.h -- simple stream implementation. */  

#ifndef AAL_STREAM_H
#define AAL_STREAM_H

#ifndef ENABLE_MINIMAL
#include <aal/types.h>

extern int aal_stream_eof(aal_stream_t *stream);
extern void aal_stream_fini(aal_stream_t *stream);
extern void aal_stream_close(aal_stream_t *stream);
extern void aal_stream_reset(aal_stream_t *stream);

extern aal_stream_t *aal_stream_create(void *entity,
				       aal_proto_t *proto);

extern int32_t aal_stream_read(aal_stream_t *stream,
			       void *buff, uint32_t n);

extern int32_t aal_stream_write(aal_stream_t *stream,
				void *buff, uint32_t n);

extern int32_t aal_stream_format(aal_stream_t *stream,
				 const char *format, ...);
extern void aal_stream_init(aal_stream_t *stream,
			    void *entity,
			    aal_proto_t *proto);
				       
#endif

#endif
