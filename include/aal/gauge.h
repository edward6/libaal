/* Copyright (C) 2001, 2002, 2003 by Hans Reiser, licensing governed by
   libaal/COPYING.
   
   gauge.h -- progress-bar functions. */

#ifndef AAL_GAUGE_H
#define AAL_GAUGE_H

#ifndef ENABLE_MINIMAL
#include <aal/types.h>

extern aal_gauge_t *aal_gauge_create(aal_gauge_handler_t handler, 
				     aal_gauge_handler_t value_func,
				     void *data, uint64_t gap, 
				     char *label, ...);

extern void aal_gauge_rename(aal_gauge_t *gauge, char *label, ...);

extern void aal_gauge_touch(aal_gauge_t *gauge);
extern void aal_gauge_done(aal_gauge_t *gauge);
extern void aal_gauge_pause(aal_gauge_t *gauge);
extern void aal_gauge_free(aal_gauge_t *gauge);

extern void aal_gauge_set_value(aal_gauge_t *gauge, int64_t value);
extern int64_t aal_gauge_get_value(aal_gauge_t *gauge);
extern void aal_gauge_set_data(aal_gauge_t *gauge, void *data);

#endif

#endif

