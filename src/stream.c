/* Copyright (C) 2001, 2002, 2003 by Hans Reiser, licensing governed by
   libaal/COPYING.
   
   stream.c -- simple stream implementation. */  

#ifndef ENABLE_STAND_ALONE
#include <stdlib.h>
#include <aal/aal.h>

/* Creates stream and return it to caller. */
aal_stream_t *aal_stream_create(void) {
	aal_stream_t *stream;

	if (!(stream = aal_calloc(sizeof(*stream), 0)))
		return NULL;

	aal_stream_init(stream);
	
	return stream;
}

/* Initializes @stream by flush @handler and water @mark. */
errno_t aal_stream_init(aal_stream_t *stream) {
	aal_assert("umka-1543", stream != NULL);

	stream->size = 0;
	stream->offset = 0;
	stream->data = NULL;
	
	return 0;
}

/* Releases @stream data. */
void aal_stream_fini(aal_stream_t *stream) {
	aal_assert("umka-1549", stream != NULL);
	aal_free(stream->data);
}

/* Releases stream data and frees @stream. */
void aal_stream_close(aal_stream_t *stream) {
	aal_assert("umka-1542", stream != NULL);
	
	aal_stream_fini(stream);
	aal_free(stream);
}

/* Checks if stream is need to be expanded. If so -- expand it. */
static errno_t aal_stream_check(aal_stream_t *stream,
				uint32_t size)
{
	/* Expand stream if needed. */
	if (stream->offset + size + 1 > stream->size) {
		stream->size = stream->offset + size + 1;

		if (!(stream->data = realloc(stream->data,
					     stream->size)))
		{
			return -ENOMEM;
		}
	}
	
	return 0;
}

/* Reset stream offset to zero. */
void aal_stream_reset(aal_stream_t *stream) {
	aal_assert("umka-1711", stream != NULL);
	stream->offset = 0;
}

/* Writes @size bytes of data from @buff to @stream. */
int32_t aal_stream_write(aal_stream_t *stream,
			 void *buff, uint32_t size)
{
	int32_t res;
	
	aal_assert("umka-1544", stream != NULL);
	aal_assert("umka-1545", buff != NULL);

	if ((res = aal_stream_check(stream, size)))
		return res;
	
	aal_memcpy(stream->data + stream->offset,
		   buff, size);
	
	stream->offset += size;
	
	return size;
}

/* Reads @size bytes of data to @buff from @stream. */
int32_t aal_stream_read(aal_stream_t *stream,
			void *buff, uint32_t size)
{
	aal_assert("umka-1546", stream != NULL);
	aal_assert("umka-1547", buff != NULL);

	if (stream->offset + size > stream->size)
		size = stream->size - stream->offset;

	if (size > 0) {
		aal_memcpy(buff, stream->data +
			   stream->offset, size);
		
		stream->offset += size;
	}
	
	return size;
}

/* Writes string formated by using @format and consecuent params. */
int32_t aal_stream_format(aal_stream_t *stream,
			  const char *format, ...)
{
	uint32_t len;
	char buff[4096];
	va_list arg_list;

	aal_memset(buff, 0, sizeof(buff));

	va_start(arg_list, format);
	
	len = aal_vsnprintf(buff, sizeof(buff),
			    format, arg_list);
	
	va_end(arg_list);

	if (!(len = aal_stream_write(stream, buff, len)))
		return len;

	*((char *)stream->data + stream->offset) = '\0';
	
	return len;
}
#endif
