/* Copyright (C) 2001, 2002, 2003 by Hans Reiser, licensing governed by
   libaal/COPYING.
   
   stream.c -- stream implementation. */  

#ifndef ENABLE_MINIMAL
#include <stdio.h>
#include <stdlib.h>
#include <aal/libaal.h>

aal_stream_t *aal_stream_create(void *entity,
				aal_proto_t *proto)
{
	aal_stream_t *stream;

	aal_assert("umka-2639", proto != NULL);
	
	if (!(stream = aal_calloc(sizeof(*stream), 0)))
		return NULL;

	aal_stream_init(stream, entity, proto);
	return stream;
}

void aal_stream_init(aal_stream_t *stream, void *entity, aal_proto_t *proto) {
	aal_assert("umka-2640", proto != NULL);
	aal_assert("umka-1543", stream != NULL);

	stream->size = 0;
	stream->offset = 0;
	stream->proto = proto;
	stream->entity = entity;
}

void aal_stream_fini(aal_stream_t *stream) {
	aal_assert("umka-1549", stream != NULL);

	if (stream->proto->fini)
		stream->proto->fini(stream);
}

void aal_stream_close(aal_stream_t *stream) {
	aal_assert("umka-1542", stream != NULL);
	
	aal_stream_fini(stream);
	aal_free(stream);
}

void aal_stream_reset(aal_stream_t *stream) {
	aal_assert("umka-1711", stream != NULL);
	stream->offset = 0;
}

int32_t aal_stream_write(aal_stream_t *stream,
			 void *buff, uint32_t n)
{
	aal_assert("umka-1544", stream != NULL);
	aal_assert("umka-1545", buff != NULL);

	if (!stream->proto->write)
		return -EINVAL;

	return stream->proto->write(stream, buff, n);
}

int32_t aal_stream_read(aal_stream_t *stream,
			void *buff, uint32_t n)
{
	aal_assert("umka-1546", stream != NULL);
	aal_assert("umka-1547", buff != NULL);

	if (!stream->proto->read)
		return -EINVAL;

	return stream->proto->read(stream, buff, n);
}

int aal_stream_eof(aal_stream_t *stream) {
	aal_assert("umka-2643", stream != NULL);
	return stream->proto->eof(stream);
}

int32_t aal_stream_format(aal_stream_t *stream,
			  const char *format, ...)
{
	uint32_t n;
	char buff[256];
	va_list arg_list;

	aal_assert("umka-2644", stream != NULL);
	aal_assert("umka-2645", format != NULL);

	va_start(arg_list, format);
	
	n = aal_vsnprintf(buff, sizeof(buff),
			  format, arg_list);
	
	va_end(arg_list);
	
	return aal_stream_write(stream, buff, n);
}

/* Callback for reading data from memory area. */
static int32_t read_memory(aal_stream_t *stream,
			   void *buff, uint32_t n)
{
	if (n > stream->size - stream->offset)
		n = stream->size - stream->offset;
	
	aal_memcpy(buff, stream->entity +
		   stream->offset, n);
	
	stream->offset += n;
	return n;
}

/* Callback for writing data to memory area. */
static int32_t write_memory(aal_stream_t *stream,
			    void *buff, uint32_t n)
{
	/* Expanding memory erea to fit yet another @n bytes. */
	if (stream->offset + n + 1 > stream->size) {
		stream->size = stream->size + n + 1;
		
		if (!(stream->entity = realloc(stream->entity,
					       stream->size)))
		{
			return -ENOMEM;
		}
	}

	/* Copying data to memory area attached to @print_stream. */
	aal_memcpy(stream->entity + stream->offset, buff, n);
	stream->offset += n;
	*(char *)(stream->entity + stream->offset) = 0;
	
	return n;
}

static int eof_memory(aal_stream_t *stream) {
	return stream->offset >= stream->size;
}

static void fini_memory(aal_stream_t *stream) {
	if (stream->entity) {
		aal_free(stream->entity);
		stream->entity = NULL;
	}
}

static int32_t read_file(aal_stream_t *stream,
			 void *buff, uint32_t n)
{
	FILE *file;
	uint32_t res;
	uint32_t read;

	file = stream->entity ? (FILE *)stream->entity : stdin;

	for (read = 0; read < n; read += res) {
		res = fread(buff + read, 1,
			    n - read, file);
		
		if (res < n - read) {
			read += res;
			goto error_update_offset;
		}
	}

 error_update_offset:
	stream->offset += read;
	return read;
}

static int32_t write_file(aal_stream_t *stream,
			  void *buff, uint32_t n)
{
	FILE *file;
	uint32_t res;
	uint32_t write;

	file = stream->entity ? (FILE *)stream->entity : stdout;

	for (write = 0; write < n; write += res) {
		res = fwrite(buff + write, 1,
			     n - write, file);
		
		if (res < n - write) {
			write += res;
			goto error_update_offset;
		}
	}

 error_update_offset:
	stream->offset += write;
	return write;
}

static int eof_file(aal_stream_t *stream) {
	FILE *file = stream->entity ? (FILE *)stream->entity : stdin;
	return feof(file);
}

aal_proto_t memory_stream = {
	.eof   = eof_memory, 
	.fini  = fini_memory,
	.read  = read_memory,
	.write = write_memory
};

aal_proto_t file_stream = {
	.fini  = NULL,
	.eof   = eof_file,
	.read  = read_file,
	.write = write_file
};
#endif
