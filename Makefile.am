SUBDIRS         = src include

EXTRA_DIST	= AUTHORS COPYING TODO NEWS BUGS README CREDITS THANKS \
		  libaal.spec.in libaal.spec libaal.m4 ltconfig

aclocaldir 	= $(datadir)/aclocal
aclocal_DATA	= libaal.m4

define all-sources
	( find ./ -name '*.[ch]' -print | grep -v SCCS )
endef

ETAGS:
	$(all-sources) | etags -
