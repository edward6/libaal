/* Copyright (C) 2001, 2002, 2003 by Hans Reiser, licensing governed by
   libaal/COPYING.
   
   list.h -- double-linked list implementation. */

#ifndef AAL_LIST_H
#define AAL_LIST_H

#ifndef ENABLE_MINIMAL
#include <aal/types.h>

extern aal_list_t *aal_list_alloc(void *data);
extern aal_list_t *aal_list_last(aal_list_t *list);
extern aal_list_t *aal_list_first(aal_list_t *list);

extern uint32_t aal_list_len(aal_list_t *list);

extern errno_t aal_list_foreach(aal_list_t *list, 
				foreach_func_t func,
				void *data);

extern aal_list_t *aal_list_prepend(aal_list_t *list,
				    void *data);

extern int32_t aal_list_pos(aal_list_t *list, void *data);
extern aal_list_t *aal_list_at(aal_list_t *list, uint32_t n);

extern aal_list_t *aal_list_insert(aal_list_t *list, 
				   void *data, uint32_t n);

extern aal_list_t *aal_list_insert_sorted(aal_list_t *list,
					  void *data,
					  comp_func_t comp_func,
					  void *user);
    
extern aal_list_t *aal_list_append(aal_list_t *list, void *data);
extern aal_list_t *aal_list_remove(aal_list_t *list, void *data);
extern aal_list_t *aal_list_find(aal_list_t *list, void *data);

extern aal_list_t *aal_list_find_custom(aal_list_t *list, void *needle, 
					comp_func_t comp_func, void *data);

extern void aal_list_free(aal_list_t *list, foreach_func_t func, void *data);

/* Macros for walking through the list in both directions (forward and
   backward). They are used for simple search, etc. */
#define aal_list_foreach_forward(list, walk)  \
        for (walk = list; walk; walk = walk->next)

#define aal_list_foreach_backward(list, walk) \
        for (walk = list; walk; walk = walk->prev)
    
#endif

#endif
