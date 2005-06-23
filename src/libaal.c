/*
  libaal.c -- library whide functions.
    
  Copyright (C) 2001, 2002, 2003 by Hans Reiser, licensing governed by
  libaal/COPYING.
*/

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

/* Returns libaal max supported interface version */
int libaal_max_interface_version(void) {
	return LIBAAL_MAX_INTERFACE_VERSION;
}

/* Returns libaal min supported interface version */
int libaal_min_interface_version(void) {
	return LIBAAL_MIN_INTERFACE_VERSION;
}

/* Returns libaal version */
const char *libaal_version(void) {
	return VERSION;
}

