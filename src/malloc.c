/*
  malloc.c -- hanlders for memory allocation functions.
    
  Copyright (C) 2001, 2002, 2003 by Hans Reiser, licensing governed by
  libaal/COPYING.
*/

#include <aal/aal.h>

/*
  Checking whether allone mode is in use. If so, initializes memory working
  handlers as NULL, because application that is use libreiser4 and libaal must
  set it up.
*/
#if defined(ENABLE_STAND_ALONE) && defined(ENABLE_MEMORY_MANAGER)
static aal_malloc_handler_t malloc_handler = NULL;
static aal_realloc_handler_t realloc_handler = NULL;
static aal_free_handler_t free_handler = NULL;
#else
#include <stdlib.h>
static aal_malloc_handler_t malloc_handler = malloc;
static aal_realloc_handler_t realloc_handler = realloc;
static aal_free_handler_t free_handler = free;
#endif

/* 
  Sets new handler for malloc function. This is useful for alone mode, because
  all application which are working in alone mode (without libc, probably in
  real mode of processor, etc) have own memory allocation factory. That factory
  usualy operates on some static memory heap. And all allocation function just
  mark some piece of heap as used. And all deallocation function marks
  corresponding piece as unused.
*/
void aal_malloc_set_handler(
	aal_malloc_handler_t handler)  /* new handler to be set */
{
	malloc_handler = handler;
}

/* Returns allocation handler */
aal_malloc_handler_t aal_malloc_get_handler(void) {
	return malloc_handler;
}

/* 
  Sets new handler for "realloc" operation. The same as in malloc case. See
  above for details.
*/
void aal_realloc_set_handler(
	aal_realloc_handler_t handler)   /* new handler for realloc */
{
	realloc_handler = handler;
}

/* Returns realloc handler */
aal_realloc_handler_t aal_realloc_get_handler(void) {
	return realloc_handler;
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

/*
  Memory manager stuff. Simple memory manager is needed for appliances where
  libc cannot be used but libreiser4 must be working.
*/
#if defined(ENABLE_STAND_ALONE) && defined(ENABLE_MEMORY_MANAGER)
typedef struct chunk chunk_t;
typedef enum chunk_state chunk_state_t;

#define ptr2chunk(ptr) \
        ((chunk_t *)((int)ptr - sizeof(chunk_t)))

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

static uint32_t mem_len = 0;
static uint32_t mem_free = 0;
static void *mem_start = NULL;

static void __chunk_init(void *ptr, uint32_t len,
			 chunk_state_t state,
			 void *prev, void *next)
{
	((chunk_t *)ptr)->len = len;
	((chunk_t *)ptr)->next = next;
	((chunk_t *)ptr)->prev = prev;
	((chunk_t *)ptr)->state = state;
}

static void __chunk_fuse(chunk_t *chunk) {
	chunk_t *next = chunk->next;
	chunk_t *prev = chunk->prev;
	chunk_t *first = (chunk_t *)mem_start;
	
	/*
	  Trying to fuse currect chunk with next one if it is free. This is
	  needed for getting fragmentation smaller.
	*/
	if (next != first && next->state == ST_FREE) {
		chunk->next = next->next;

		if (next->next != first)
			next->next->prev = chunk;

		mem_free += sizeof(chunk_t);
		chunk->len += (next->len + sizeof(chunk_t));
	}

	/* Trying to fuse current chunk with the prvious one */
	if (prev != first && prev->state == ST_FREE) {
		prev->next = chunk->next;

		if (chunk->next != first)
			chunk->next->prev = prev;

		mem_free += sizeof(chunk_t);
		prev->len += (chunk->len + sizeof(chunk_t));
	}
}

static void *__chunk_split(chunk_t *chunk, uint32_t size) {
	chunk_t *first = (chunk_t *)mem_start;

	void *new = (void *)((int)chunk + size +
			     sizeof(chunk_t));

	/*
	  Check if we have enough free space for split the found
	  chunk.
	*/
	if ((int)new + sizeof(chunk_t) >= ((int)mem_start + mem_len))
		return NULL;

	/*
	  Okay, we have found good enough chunk. And now we
	  split into onto two.
	*/
	__chunk_init(new, chunk->len - size - sizeof(chunk_t),
		     ST_FREE, chunk, chunk->next);

	/* Setting up prev, next pointers */
	if (chunk->next != first)
		chunk->next->prev = new;
			
	__chunk_init(chunk, size, ST_USED, chunk->prev, new);

	mem_free -= (size + sizeof(chunk_t));
	return (void *)((int)chunk + sizeof(chunk_t));
}

static inline int __chunk_exact(chunk_t *chunk,
				uint32_t size)
{
	return chunk->len == size;
}

static inline int __chunk_proper(chunk_t *chunk,
				 uint32_t size)
{
	if (chunk->state != ST_FREE)
		return 0;

	if (__chunk_exact(chunk, size))
		return 1;
	
	return chunk->len >= size + sizeof(chunk_t);
}

/*
  Makes search for proper memory chunk in list of chunks. If found, split it in
  order to allocate requested amount of memory.
*/
static void *__chunk_alloc(uint32_t size) {
	chunk_t *walk;

	if (size == 0)
		return NULL;

	walk = (chunk_t *)mem_start;
	
	/* The loop though the all chunks in list */
	while (1) {

		if (__chunk_proper(walk, size)) {

			/* Check if we need to split @walk chunk */
			if (__chunk_exact(walk, size)) {
				walk->state = ST_USED;
				mem_free -= walk->len;
				return (void *)walk + sizeof(chunk_t);
			}
			
			return __chunk_split(walk, size);
		}

		if ((walk = walk->next) == mem_start)
			break;
	}

	return NULL;
}

/* Frees passed memory pointer */
static void __chunk_free(void *ptr) {
	chunk_t *chunk = ptr2chunk(ptr);

	chunk->state = ST_FREE;
	mem_free += chunk->len;

	/*
	  Fusing both left and right neighbour chunks if they are not used. This
	  is needed for keep memory manager area in optimal state.
	*/
	__chunk_fuse(chunk);
}

/* Initializes memory manager on passed memory area */
void aal_mem_init(void *start, uint32_t len) {
	uint32_t size = len - sizeof(chunk_t);

	__chunk_init(start, size, ST_FREE,
		     start, start);

	mem_len = len;
	mem_free = size;
	mem_start = start;

	free_handler = __chunk_free;
	malloc_handler = __chunk_alloc;
}

void aal_mem_fini(void) {
	mem_len = 0;
	mem_free = 0;
	mem_start = NULL;
}

uint32_t aal_mem_free(void) {
	return mem_free;
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

/*
  The wrapper for realloc function. It checks for result memory allocation and
  if it failed then reports about this.
*/
errno_t aal_realloc(
	void **old,		    /* pointer to previously allocated piece */
	uint32_t size)              /* new size */
{
	void *mem;

	if (!realloc_handler)
		return -EINVAL;

	if (!(mem = (void *)realloc_handler(*old, size)))
		return -ENOMEM;
    
	*old = mem;
	return 0;
}

/*
  The wrapper for free function. It checks for passed memory pointer and if it
  is invalid then reports about this.
*/
void aal_free(
	void *ptr)		    /* pointer onto memory to be released */
{
	if (!free_handler)
		return;

	free_handler(ptr);
}

/*
  The wrapper for malloc function. It checks for result memory allocation and if
  it failed then reports about this.
*/
void *aal_malloc(
	uint32_t size)              /* size of memory piece to be allocated */
{
	void *mem;

	/* 
	  We are using simple printf function instead of exception, because
	  exception initialization is needed correctly worked memory allocation
	  handler.
	*/
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
