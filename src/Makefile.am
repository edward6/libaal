aalincludedir 		 = -I$(top_srcdir)/include

libaal_sources  	 = device.c file.c exception.c list.c malloc.c \
			   print.c string.c math.c debug.c bitops.c \
			   gauge.c block.c ui.c stream.c hash.c libaal.c

if ENABLE_MINIMAL
MINIMAL_LIBS               = libaal-minimal.la 
else
MINIMAL_LIBS               = 
endif

lib_LTLIBRARIES		 = libaal.la $(MINIMAL_LIBS)

libaal_la_LDFLAGS 	 = -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) \
			   -release $(LT_RELEASE)

libaal_la_SOURCES  	 = $(libaal_sources)

libaal_la_CFLAGS         = @GENERIC_CFLAGS@

if ENABLE_MINIMAL
libaal_minimal_la_SOURCES  = $(libaal_sources)
libaal_minimal_la_CFLAGS   = @MINIMAL_CFLAGS@
endif

INCLUDES = $(aalincludedir)

