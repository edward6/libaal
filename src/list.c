/* Copyright (C) 2001, 2002, 2003 by Hans Reiser, licensing governed by
   libaal/COPYING.
   
   list.c -- double-linked list implementation. */

#ifndef ENABLE_MINIMAL
#include <aal/libaal.h>

/* Allocates new aal_list_t instance and assigns passed @data to it */
aal_list_t *aal_list_alloc(void *data) {
	aal_list_t *list;
   
	if (!(list  = (aal_list_t *)aal_calloc(sizeof(*list), 0)))
		return NULL;
    
	list->data = data;
	list->next = NULL;
	list->prev = NULL;
    
	return list;
}

/* Returns last item from the passed @list */
aal_list_t *aal_list_last(aal_list_t *list) {
	if (!list) return NULL;
    
	while (list->next)
		list = list->next;

	return list;
}

/* Returns first item from the passed @list */
aal_list_t *aal_list_first(aal_list_t *list) {
	if (!list) return NULL;
    
	while (list->prev)
		list = list->prev;

	return list;
}

/* Returns list length */
uint32_t aal_list_len(aal_list_t *list) {
	uint32_t len = 0;

	while (list) {
		len++;
		list = list->next;
	}
	
	return len;
}

/* This function walks though the @list and calls passed @func for each list
   item. This may be used for searching something, or performing some per-item
   actions. */
errno_t aal_list_foreach(aal_list_t *list,
			 foreach_func_t func, 
			 void *data) 
{
	errno_t res;
	aal_list_t *walk, *next;

	for (walk = list; walk; ) {
		next = walk->next;

		if ((res = func(walk->data, data)))
			return res;
			
		walk = next;
	}

	return 0;
}

/* Perform lookup inside @list for @data and returns its position */
int32_t aal_list_pos(aal_list_t *list, void *data) {
	int32_t pos = 0;

	while (list) {
		if (list->data == data)
			return pos;
	
		pos++;
		list = list->next;
	}
	return pos;
}

/* Gets list item at @n position */
aal_list_t *aal_list_at(aal_list_t *list, uint32_t n) {

	if (!list)
		return NULL;
	
	while ((n-- > 0) && list->next)
		list = list->next;

	return list;
}

/* Inserts new item at @n position of @list */
aal_list_t *aal_list_insert(aal_list_t *list, 
			    void *data, uint32_t n) 
{
	aal_list_t *at; 
	
	if (n == 0)
		return aal_list_prepend(list, data);
	
	at = aal_list_at(list, n - 1);
	at = aal_list_append(at, data);

	return (list == NULL ? at : list);
}

/* Inserts new item in sorted maner */
aal_list_t *aal_list_insert_sorted(aal_list_t *list, void *data,
				   comp_func_t comp_func, void *user)
{
	aal_list_t *at = list;
	int cmp;

	if (!comp_func)
		return NULL;
    
	if (!list) {
		at = aal_list_alloc(data);
		return at;
	}
  
	cmp = comp_func(at->data, data, user);
  
	while ((at->next) && (cmp < 0)) {
		at = at->next;
		cmp = comp_func(at->data, data, user);
	}

	at = (cmp > 0) ? 
		aal_list_prepend(at, data) : 
		aal_list_append(at, data);
	
	return (list == at->next) ? at : list;
}

/* Inserts new item just before passed @list */
aal_list_t *aal_list_prepend(aal_list_t *list, void *data) {
	aal_list_t *new;
    
	if (!(new = aal_list_alloc(data)))
		return 0;
    
	if (list) {
		if (list->prev) {
			list->prev->next = new;
			new->prev = list->prev;
		}
		list->prev = new;
		new->next = list;
	}
    
	return new;
}

/* Inserts new item just after passed @list */
aal_list_t *aal_list_append(aal_list_t *list, void *data) {
	aal_list_t *new;
    
	if (!(new = aal_list_alloc(data)))
		return 0;
    
	if (list) {
		if (list->next)
			list->next->prev = new;

		new->next = list->next;
		new->prev = list;

		list->next = new;

		return list;
	} else
		return new;
}

/* Removes item from the passed @list and return reffernce to the next or prev
   list item. */
aal_list_t *aal_list_remove(aal_list_t *list, void *data) {
	aal_list_t *temp;
	aal_list_t *result = list;

	if (list && (temp = aal_list_find(list, data))) {
		if (temp->prev)
			temp->prev->next = temp->next;
	    
		if (temp->next)
			temp->next->prev = temp->prev;

		if (temp == list) {
			if (temp->next)
				result = temp->next;
			else if (temp->prev)
				result = temp->prev;
			else
				result = NULL;
		}
		
		aal_free(temp);
	}

	return result;
}

/* Returns list item by its data */
aal_list_t *aal_list_find(aal_list_t *list, void *data) {
	while (list) {
		if (list->data == data)
			return list;
	
		list = list->next;
	}
	return NULL;
}

aal_list_t *aal_list_find_custom(aal_list_t *list, void *needle, 
				 comp_func_t comp_func, void *user) 
{

	if (!comp_func)
		return NULL;
    
	while (list) {
		if (comp_func(list->data, needle, user) == 0)
			return list;

		list = list->next;
	}
	
	return NULL;
}

/* Releases all list items */
void aal_list_free(aal_list_t *list, foreach_func_t func, void *data) {
	aal_list_t *last = list;
    
	if (!list) return;
    
	while (last->next) {
		aal_list_t *temp = last->next;

		if (func) func(last->data, data);
		
		aal_free(last);
		last = temp;
	}
}
#endif
