/* Copyright (C) 2001, 2002, 2003 by Hans Reiser, licensing governed by
   libaal/COPYING.
   
   hash.c -- simple hash implementation. */

#include <aal/libaal.h>

/* Allocates new node and initializes it by @key and @value. */
static aal_hash_node_t *aal_hash_node_alloc(void *key, void *value) {
	aal_hash_node_t *node;

	if (!(node = aal_calloc(sizeof(*node), 0)))
		return NULL;

	node->key = key;
	node->value = value;

	return node;
}

/* Releases passed @node */
static void aal_hash_node_free(aal_hash_table_t *table,
			       aal_hash_node_t *node)
{
	if (table->keyrem_func)
		table->keyrem_func(node->key);

	if (table->valrem_func)
		table->valrem_func(node->value);
	
	aal_free(node);
}

/* Allocates hash table with passed @hash_func used for calculating hashes and
   @comp_func used for comparing keys. */
aal_hash_table_t *aal_hash_table_create(uint32_t size,
					hash_func_t hash_func,
					comp_func_t comp_func,
					keyrem_func_t keyrem_func,
					valrem_func_t valrem_func)
{
	aal_hash_table_t *table;
	
	aal_assert("umka-2267", hash_func != NULL);
	aal_assert("umka-2273", comp_func != NULL);

	if (!(table = aal_calloc(sizeof(*table), 0)))
		return NULL;

	table->real = 0;
	table->size = size;
	
	table->hash_func = hash_func;
	table->comp_func = comp_func;

	table->keyrem_func = keyrem_func;
	table->valrem_func = valrem_func;

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
			aal_hash_node_free(table, node);
		}
	}

	aal_free(table->nodes);
	aal_free(table);
}

/* Makes lookup for node by passed @key */
aal_hash_node_t **aal_hash_table_lookup_node(aal_hash_table_t *table,
					     void *key)
{
	uint64_t hash;
	aal_hash_node_t **node;

	aal_assert("umka-2271", key != NULL);
	aal_assert("umka-2270", table != NULL);
	aal_assert("umka-3118", (table->size & (table->size - 1)) == 0);
	
	hash = table->hash_func(key);
	node = &table->nodes[hash & (table->size - 1)];

	while (*node && table->comp_func((*node)->key, key, NULL))
		node = &(*node)->next;

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
	aal_hash_node_t **node;

	node = aal_hash_table_lookup_node(table, key);
	
	if (*node) {
		(*node)->value = value;
	} else {
		if (!(*node = aal_hash_node_alloc(key, value)))
			return -ENOMEM;
		
		table->real++;
	}

	return 0;
}

/* Removed node from passed @table by @key */
errno_t aal_hash_table_remove(aal_hash_table_t *table,
			      void *key)
{
	aal_hash_node_t *dest;
	aal_hash_node_t **node;

	node = aal_hash_table_lookup_node(table, key);
	
	if (*node) {
		dest = *node;
		*node = dest->next;
		aal_hash_node_free(table, dest);
		table->real--;
		
		return 0;
	}

	return -EINVAL;

}

/* Calls @foreach_func for all hash table nodes. May be used for walking though
   the all nodes registered in hash table. */
errno_t aal_hash_table_foreach(aal_hash_table_t *table,
			       foreach_func_t foreach_func,
			       void *data)
{
	uint32_t i;
	errno_t res;
	
	for (i = 0; i < table->size; i++) {
		aal_hash_node_t *node;
		aal_hash_node_t *next;
		
		for (node = table->nodes[i];
		     node != NULL; node = next)
		{
			next = node->next;

			if ((res = foreach_func(node, data)))
				return res;
		}
	}

	return 0;
}
