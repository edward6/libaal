/* Copyright (C) 2001, 2002, 2003 by Hans Reiser, licensing governed by
   libaal/COPYING.
   
   gauge.c -- progress-bar functions. Gauge is supporting three gauge kinds:

   (1) percentage gauge - for operations, whose completion time may be foreseen;
   looks like, "initializing: 14%"
  
   (2) indicator gauge - for operations, whose completion time may not be
   foreseen; for example, "traversing: /"
    
   (3) silent gauge - for operations, without any indication of progress; for
   example, "synchronizing..."
    
   The all kinds of gauges will report about operation result (done/failed) in
   maner like this:

   "initializing: done" or "initializing: failed"

   In the case some exception occurs durring gauge running, it will be stoped
   and failing report will be made. */

#ifndef ENABLE_MINIMAL

#include <aal/libaal.h>

/* Gauge constructor. */
aal_gauge_t *aal_gauge_create(aal_gauge_handler_t handler,
			      aal_gauge_handler_t value_func,
			      void *data, uint64_t gap, 
			      char *label, ...) 
{
	aal_gauge_t *gauge;
	va_list arg_list;
	
	if (!handler) 
		return NULL;
    
	if (!(gauge = aal_calloc(sizeof(*gauge), 0)))
		return NULL;

	gauge->handler = handler;
	gauge->value_func = value_func;
	gauge->data = data;
	gauge->value = -1;
	gauge->time.gap = gap;

	va_start(arg_list, label);
	aal_gauge_rename(gauge, label, arg_list);
	va_end(arg_list);
	
	return gauge;
}

/* Init guage parameters. */
void aal_gauge_rename(aal_gauge_t *gauge, char *label, ...) {
	va_list arg_list;
	int len;

	if (!gauge || !label) return;
	
	gauge->state = GS_DONE;
	
	va_start(arg_list, label);
	len = aal_vsnprintf(gauge->label, sizeof(gauge->label), 
			    label, arg_list);
	va_end(arg_list);
	
	gauge->label[len] = '\0';
}

/* Gauge is running. Next step. */
void aal_gauge_touch(aal_gauge_t *gauge) {
	if (!gauge) return;

	if (gauge->state == GS_DONE) {
		gauge->state = GS_START;
	} else if (gauge->state == GS_PAUSE) {
		gauge->state = GS_RESUME;
	}

	gauge->handler(gauge);
	gauge->state = GS_ACTIVE;
}

/* Finish. */
void aal_gauge_done(aal_gauge_t *gauge) {
	if (!gauge || gauge->state == GS_DONE)
		return;
	
	if (gauge->state == GS_PAUSE)
		aal_gauge_touch(gauge);

	gauge->state = GS_DONE;
	gauge->handler(gauge);
}

/* Pause the gauge. Will be resumed later. */
void aal_gauge_pause(aal_gauge_t *gauge) {
	if (!gauge) return;
    
	if (gauge->state != GS_ACTIVE && 
	    gauge->state != GS_START) 
	{
		return;
	}
	
	gauge->state = GS_PAUSE;
	gauge->handler(gauge);
}

/* Gauge destructor. */
void aal_gauge_free(aal_gauge_t *gauge) {
	if (!gauge) return;
	aal_free(gauge);
}

void aal_gauge_set_value(aal_gauge_t *gauge, int64_t value) {
	if (!gauge) return;
	gauge->value = value;
}

int64_t aal_gauge_get_value(aal_gauge_t *gauge) {
	if (!gauge) 
		return -EINVAL;

	return gauge->value;
}

void aal_gauge_set_data(aal_gauge_t *gauge, void *data) {
	if (!gauge) return;
	gauge->data = data;
}

#endif
