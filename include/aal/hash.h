/*
  hash.h -- hash table implementation.
    
  Copyright (C) 2001, 2002, 2003 by Hans Reiser, licensing governed by
  libaal/COPYING.
*/

#ifndef AAL_HASH_H
#define AAL_HASH_H

#if (defined(ENABLE_STAND_ALONE) && defined(ENABLE_HASH_FUNCTIONS)) || !defined(ENABLE_STAND_ALONE)
#include <aal/types.h>

extern void aal_hash_table_free(aal_hash_table_t *table);

extern int aal_hash_table_remove(aal_hash_table_t *table,
				 void *key);

extern void aal_hash_table_insert(aal_hash_table_t *table,
				  void *key, void *value);

extern void *aal_hash_table_lookup(aal_hash_table_t *table,
				   void *key);

extern aal_hash_table_t *aal_hash_table_alloc(hash_func_t hash_func,
					      comp_func_t comp_func);

extern aal_hash_node_t **aal_hash_table_lookup_node(aal_hash_table_t *table,
						    void *key);
#endif

#endif
