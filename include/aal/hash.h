/*
  hash.h -- hash table implementation.
    
  Copyright (C) 2001, 2002, 2003 by Hans Reiser, licensing governed by
  libaal/COPYING.
*/

#ifndef AAL_HASH_H
#define AAL_HASH_H

#ifndef ENABLE_STAND_ALONE
#include <aal/types.h>

extern void aal_hash_table_free(aal_hash_table_t *table);

extern errno_t aal_hash_table_remove(aal_hash_table_t *table,
				     void *key);

extern errno_t aal_hash_table_insert(aal_hash_table_t *table,
				     void *key, void *value);

extern void *aal_hash_table_lookup(aal_hash_table_t *table,
				   void *key);

extern errno_t aal_hash_table_foreach(aal_hash_table_t *table,
				      foreach_func_t foreach_func,
				      void *data);

extern aal_hash_table_t *aal_hash_table_alloc(hash_func_t hash_func,
					      comp_func_t comp_func);

extern aal_hash_node_t **aal_hash_table_lookup_node(aal_hash_table_t *table,
						    void *key);
#endif

#endif
