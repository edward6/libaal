/*
  hash.c -- simple hash implementation.
    
  Copyright (C) 2001, 2002, 2003 by Hans Reiser, licensing governed by
  libaal/COPYING.
*/

#if (defined(ENABLE_STAND_ALONE) && defined(ENABLE_HASH_FUNCTIONS)) || !defined(ENABLE_STAND_ALONE)
#include <aal/aal.h>

/* Hash table size */
#define TABLE_SIZE (0x40000)

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
	uint32_t size;
	aal_hash_table_t *table;
	
	aal_assert("umka-2267", hash_func != NULL);

	if (!(table = aal_calloc(sizeof(*table), 0)))
		return NULL;

	table->real = 0;
	table->size = TABLE_SIZE;
	table->hash_func = hash_func;
	table->comp_func = comp_func;

	size = table->size * sizeof(void *);

	if (!(table->nodes = aal_calloc(size, 0)))
		goto error_free_table;

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
		for (node = table->nodes[i];
		     node != NULL; node = next)
		{
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

/* Inserts new node to passed @table */
errno_t aal_hash_table_insert(aal_hash_table_t *table,
			      void *key, void *value)
{
	uint32_t hash;
	aal_hash_node_t **node;
	aal_hash_node_t *new_node;

	hash = table->hash_func(key);
	node = &table->nodes[hash % table->size];

	if (!(new_node = aal_hash_node_alloc(key, value)))
		return -EINVAL;
	
	if (*node) {
		new_node->next = *node;
		(*node)->prev = new_node;
	}
	
	*node = new_node;
	table->real++;

	return 0;
}

/* Removed node from passed @table by @key */
errno_t aal_hash_table_remove(aal_hash_table_t *table,
			      void *key)
{
	aal_hash_node_t *prev;
	aal_hash_node_t *next;
	aal_hash_node_t **node;

	if (!*(node = aal_hash_table_lookup_node(table, key)))
		return -EINVAL;

	prev = (*node)->prev;
	next = (*node)->next;
	
	if (prev)
		prev->next = next;
		
	if (next)
		next->prev = prev;
		
	aal_hash_node_free(*node);
	table->real--;
		
	return 0;
}
#endif
