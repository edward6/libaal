/*
  hash.c -- hash implementation.
    
  Copyright (C) 2001, 2002, 2003 by Hans Reiser, licensing governed by
  libaal/COPYING.
*/

#if (defined(ENABLE_STAND_ALONE) && defined(ENABLE_HASH_FUNCTIONS)) || !defined(ENABLE_STAND_ALONE)
#include <aal/aal.h>

/* Minimal and maximal hash table size */
#define MIN_SIZE (11)
#define MAX_SIZE (13845163)

/* Possible hash table size values */
static const uint32_t primes[] = {
	11,
	19,
	37,
	73,
	109,
	163,
	251,
	367,
	557,
	823,
	1237,
	1861,
	2777,
	4177,
	6247,
	9371,
	14057,
	21089,
	31627,
	47431,
	71143,
	106721,
	160073,
	240101,
	360163,
	540217,
	810343,
	1215497,
	1823231,
	2734867,
	4102283,
	6153409,
	9230113,
	13845163,
};

static const uint32_t nprimes = sizeof(primes) / sizeof(primes[0]);

/* Returns closest known prime number */
static uint32_t aal_spaced_primes_closest(uint32_t num) {
	uint32_t i;
                                                                                          
	for (i = 0; i < nprimes; i++) {
		if (primes[i] > num)
			return primes[i];
	}
                                                                                          
	return primes[nprimes - 1];
}

/* Allocates new node and initializes it by @key and @value */
static aal_hash_node_t *aal_hash_node_alloc(void *key, void *value) {
	aal_hash_node_t *node;

	if (!(node = aal_calloc(sizeof(*node), 0)))
		return NULL;

	node->key = key;
	node->value = value;

	return node;
}

/* Releases passed @node */
static void aal_hash_node_free(aal_hash_node_t *node) {
	aal_free(node);
}

/*
  Allocates hash table with passed @hash_func used for calculating hashes and
  @comp_func used for comparing keys.
*/
aal_hash_table_t *aal_hash_table_alloc(hash_func_t hash_func,
				       comp_func_t comp_func)
{
	aal_hash_table_t *table;
	
	aal_assert("umka-2267", hash_func != NULL);

	if (!(table = aal_calloc(sizeof(*table), 0)))
		return NULL;

	table->hash_func = hash_func;
	table->comp_func = comp_func;

	table->real = 0;
	table->size = MIN_SIZE;
	
	if (!(table->nodes = aal_calloc(sizeof(void *),
					table->size)))
	{
		goto error_free_table;
	}

	return table;
	
 error_free_table:
	aal_free(table);
	return NULL;
}

/* Releases all nodes and hash @table itself */
void aal_hash_table_free(aal_hash_table_t *table) {
	uint32_t i;
	aal_hash_node_t *node;
	aal_hash_node_t *next;
	
	aal_assert("umka-2269", table != NULL);

	for (i = 0; i < table->size; i++) {
		for (node = table->nodes[i]; node; node = next) {
			next = node->next;
			aal_hash_node_free(node);
		}
	}

	aal_free(table->nodes);
	aal_free(table);
}

/* Makes lookup for node by passed @key */
aal_hash_node_t **aal_hash_table_lookup_node(aal_hash_table_t *table,
					     void *key)
{
	uint32_t hash;
	aal_hash_node_t **node;

	aal_assert("umka-2271", key != NULL);
	aal_assert("umka-2270", table != NULL);
	
	hash = table->hash_func(key);
	node = &table->nodes[hash % table->size];

	if (table->comp_func) {
		while (*node && table->comp_func((*node)->key, key, NULL))
			node = &(*node)->next;
	} else {
		while (node && (*node)->key != key)
			node = &(*node)->next;
	}

	return node;
}

/* Makes lookup for value by passed @key */
void *aal_hash_table_lookup(aal_hash_table_t *table,
			    void *key)
{
	aal_hash_node_t *node;

	node = *aal_hash_table_lookup_node(table, key);
	return node ? node->value : NULL;
}

/* Resizes hash @table if it is needed */
static void aal_hash_table_resize(aal_hash_table_t *table) {
	if ((table->size >= table->real * 3 && table->size > MIN_SIZE) ||
	    (table->size * 3 <= table->real && table->size < _MAX_SIZE))
	{
		uint32_t i, new_size;
		aal_hash_node_t **new_nodes;
		aal_hash_node_t *node, *next;

		new_size = aal_spaced_primes_closest(table->real);
		new_size = CLAMP(new_size, MIN_SIZE, MAX_SIZE);

		new_nodes = aal_calloc(sizeof(void *), new_size);

		for (i = 0; i < table->size; i++) {
			uint32_t hash;
			
			for (node = table->nodes[i]; node; node = next) {
				next = node->next;

				hash = table->hash_func(node->key) %
					new_size;
				
				node->next = new_nodes[hash];
				new_nodes[hash] = node;
			}
		}

		aal_free(table->nodes);
		table->size = new_size;
		table->nodes = new_nodes;
	}
}

/* Inserts new node to passed @table */
void aal_hash_table_insert(aal_hash_table_t *table,
			   void *key, void *value)
{
	aal_hash_node_t **node;

	node = aal_hash_table_lookup_node(table, key);
	
	if (*node)
		(*node)->value = value;
	else {
		*node = aal_hash_node_alloc(key, value);
		table->real++;
		aal_hash_table_resize(table);
	}
}

/* Removed node from passed @table by @key */
int aal_hash_table_remove(aal_hash_table_t *table,
			  void *key)
{
	aal_hash_node_t **node, *dest;
                                                                                          
	node = aal_hash_table_lookup_node(table, key);
	
	if (*node) {
		dest = *node;

		(*node) = dest->next;
		aal_hash_node_free(dest);
		table->real--;

		aal_hash_table_resize(table);
		return 1;
	}
	
	return 0;
}
#endif
