/* Copyright (C) 2001, 2002, 2003 by Hans Reiser, licensing governed by
   libaal/COPYING.
   
   malloc.c -- hanlders for memory allocation functions. */

#include <aal/libaal.h>

/* Checking whether allone mode is in use. If so, initializes memory working
   handlers as NULL, because application that is use libreiser4 and libaal must
   set it up. */
#if defined(ENABLE_MINIMAL) && defined(ENABLE_MEMORY_MANAGER)
static aal_free_handler_t free_handler = NULL;
static aal_malloc_handler_t malloc_handler = NULL;
#else
#include <stdlib.h>
static aal_free_handler_t free_handler = (aal_free_handler_t)free;
static aal_malloc_handler_t malloc_handler = (aal_malloc_handler_t)malloc;
#endif

#ifndef ENABLE_MINIMAL
/* Sets new handler for malloc function. This is useful for minimal mode, because
   all application which are working in minimal mode (without libc, probably in
   real mode of processor, etc) have own memory allocation factory. That factory
   usualy operates on some static memory heap. And all allocation function just
   mark some piece of heap as used. And all deallocation function marks
   corresponding piece as unused. */
void aal_malloc_set_handler(
	aal_malloc_handler_t handler)  /* new handler to be set */
{
	malloc_handler = handler;
}

/* Returns allocation handler */
aal_malloc_handler_t aal_malloc_get_handler(void) {
	return malloc_handler;
}

/* Sets new handle for "free" operation */
void aal_free_set_handler(
	aal_free_handler_t handler)    /* new "free" operation handler */
{
	free_handler = handler;
}

/* Returns hanlder for "free" opetration */
aal_free_handler_t aal_free_get_handler(void) {
	return free_handler;
}
#endif

/* Memory manager stuff. Simple memory manager is needed for appliances where
   libc cannot be used but libreiser4 must be working. */
#if defined(ENABLE_MINIMAL) && defined(ENABLE_MEMORY_MANAGER)
typedef struct chunk chunk_t;
typedef enum chunk_state chunk_state_t;

enum chunk_state {
	ST_FREE = 1 << 0,
	ST_USED = 1 << 1 
};

struct chunk {
	uint32_t len;
	chunk_t *next;
	chunk_t *prev;

	chunk_state_t state;
} __attribute__((packed));

static uint32_t area_len = 0;
static uint32_t area_free = 0;
static void *area_start = NULL;

static void __chunk_init(void *ptr, uint32_t len,
			 chunk_state_t state,
			 void *prev, void *next)
{
	((chunk_t *)ptr)->len = len;
	((chunk_t *)ptr)->next = next;
	((chunk_t *)ptr)->prev = prev;
	((chunk_t *)ptr)->state = state;
}

/* Tries to fuse @passed chunk with @neig if it is free */
static int __chunk_fuse(chunk_t *chunk, chunk_t *right) {

	if (right == area_start || right->state != ST_FREE)
		return 0;

	if (chunk == area_start || chunk->state != ST_FREE)
		return 0;
	
	chunk->next = right->next;
	right->next->prev = chunk;

	area_free += sizeof(chunk_t);
	chunk->len += (right->len + sizeof(chunk_t));

	return 1;
}

static void *__chunk_split(chunk_t *chunk, uint32_t size, int forw) {
	uint32_t s;
	void *new;

	s = chunk->len - size - sizeof(chunk_t);
	new = forw ? 
		(char *)chunk + sizeof(chunk_t) + size :
		(char *)chunk + sizeof(chunk_t) + s;
	
	/* Okay, we have found chunk good enough. And now we split it onto two
	   chunks. */
	__chunk_init(new, forw ? s : size, forw ? ST_FREE : ST_USED, 
		     chunk, chunk->next);

	/* Setting up prev, next pointers */
	chunk->next->prev = new;
			
	__chunk_init(chunk, forw ? size : s, forw ? ST_USED : ST_FREE, 
		     chunk->prev, new);

	area_free -= (size + sizeof(chunk_t));
	return forw ?
		(char *)chunk + sizeof(chunk_t):
		(char *)new + sizeof(chunk_t);
}

/* Makes search for proper memory chunk in list of chunks. If found, split it in
   order to allocate requested amount of memory. */
static void *__chunk_alloc(uint32_t size) {
	chunk_t *walk = (chunk_t *)area_start;
	int forw = 1;
	
	if (size >= 4096) {
		forw = 0;
		walk = walk->prev;
	}
		
	/* The loop though the all chunks in list */
	while (1) {
		if (walk->state == ST_FREE) {
			if (walk->len >= size + sizeof(chunk_t) + 1) {
				return __chunk_split(walk, size, forw);
			} else if (walk->len == size) {
				walk->state = ST_USED;
				area_free -= walk->len;
				return (void *)walk + sizeof(chunk_t);
			}
		}

		/* Getting next chunk from list */
		if ((walk = forw ? walk->next : walk->prev) == area_start)
			break;
	};

	return NULL;
}

#define ptr2chunk(ptr) \
        ((chunk_t *)((int)ptr - sizeof(chunk_t)))

/* Frees passed memory pointer */
static void __chunk_free(void *ptr) {
	chunk_t *chunk = ptr2chunk(ptr);

	chunk->state = ST_FREE;
	area_free += chunk->len;

	/* Fusing both left and right neighbour chunks if they are not
	   used. This is needed for keep memory manager area not fragmented. */
	__chunk_fuse(chunk, chunk->next);
	__chunk_fuse(chunk->prev, chunk);
}

/* Initializes memory manager on passed memory area */
void aal_mem_init(void *start, uint32_t len) {
	__chunk_init(start, len - sizeof(chunk_t),
		     ST_FREE, start, start);

	area_len = len;
	area_start = start;
	area_free = len - sizeof(chunk_t);

	free_handler = __chunk_free;
	malloc_handler = __chunk_alloc;
}

void aal_mem_fini(void) {
	area_len = 0;
	area_free = 0;
	area_start = NULL;
	
	free_handler = NULL;
	malloc_handler = NULL;
}

uint32_t aal_mem_free(void) {
	return area_free;
}
#else
void aal_mem_init(void *start, uint32_t len) {
}

void aal_mem_fini(void) {
}

uint32_t aal_mem_free(void) {
	return 0;
}
#endif

/* The wrapper for free function. It checks for passed memory pointer and if it
   is invalid then reports about this. */
void aal_free(
	void *ptr)		    /* pointer onto memory to be released */
{
	if (!free_handler)
		return;

	free_handler(ptr);
}

/* The wrapper for malloc function. It checks for result memory allocation and
   if it failed then reports about this. */
void *aal_malloc(
	uint32_t size)              /* size of memory piece to be allocated */
{
	void *mem;

	/* We are using simple printf function instead of exception, because
	   exception initialization is needed correctly worked memory allocation
	   handler. */
	if (!malloc_handler)
		return NULL;

	if (!(mem = malloc_handler(size)))
		return NULL;

	return mem;
}

/* Allocates memory piese and fills it by specified byte */
void *aal_calloc(
	uint32_t size,              /* size of memory piece to be allocated */
	char c)
{
	void *mem;

	if (!(mem = aal_malloc(size)))
		return NULL;

	aal_memset(mem, c, size);
	return mem;
}
