SUBDIRS         = src include

EXTRA_DIST	= AUTHORS COPYING TODO NEWS BUGS README CREDITS THANKS \
		  libaal.spec.in libaal.spec libaal.m4 depcomp run-ldconfig

aclocaldir 	= $(datadir)/aclocal
aclocal_DATA	= libaal.m4

define all-sources
	( find ./ -name '*.[ch]' -print | grep -v SCCS )
endef

ETAGS:
	$(all-sources) | etags -

install-exec-hook:
	./run-ldconfig $(libdir)

uninstall-hook:
	./run-ldconfig $(libdir)
