/* Copyright (C) 2001, 2002, 2003 by Hans Reiser, licensing governed by
   libaal/COPYING.
   
   hash.h -- hash table implementation. */

#ifndef AAL_HASH_H
#define AAL_HASH_H

#include <aal/types.h>

extern void aal_hash_table_free(aal_hash_table_t *table);

extern void *aal_hash_table_lookup(aal_hash_table_t *table,
				   void *key);

extern errno_t aal_hash_table_remove(aal_hash_table_t *table,
				     void *key);

extern errno_t aal_hash_table_insert(aal_hash_table_t *table,
				     void *key, void *value);

extern errno_t aal_hash_table_foreach(aal_hash_table_t *table,
				      foreach_func_t foreach_func,
				      void *data);

extern aal_hash_table_t *aal_hash_table_create(uint32_t size,
					       hash_func_t hash_func,
					       comp_func_t comp_func,
					       keyrem_func_t keyrem_func,
					       valrem_func_t valrem_func);

extern aal_hash_node_t **aal_hash_table_lookup_node(aal_hash_table_t *table,
						    void *key);
#endif
