# This file is a part of libaal package
# Copyright (C) 2001, 2002, 2003 by Hans Reiser, licensing governed by
# libaal/COPYING.

dnl Usage:
dnl AC_CHECK_LIBAAL([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]]])
dnl
dnl Example:
dnl AC_CHECK_LIBAAL(0.3.0, , [AC_MSG_ERROR([libaal >= 0.3.0 not installed - please install first])])
dnl
dnl Adds the required libraries to $AAL_LIBS and does an
dnl AC_SUBST(AAL_LIBS)

AC_DEFUN([AC_CHECK_LIBAAL],
[

dnl save LIBS
saved_LIBS="$LIBS"

dnl Check for headers and library
AC_CHECK_HEADER(aal/libaal.h, ,
    [AC_MSG_ERROR([<aal/libaal.h> not found; install libaal])] 
$3)

AC_CHECK_LIB(aal, aal_device_open, ,
    [AC_MSG_ERROR([libaal not found; install libaal available at \
http://www.namesys.com/snapshots/])]
$3)

AC_MSG_CHECKING(for libaal version >= $1)

AC_TRY_LINK_FUNC(libaal_version,,
    AC_MSG_RESULT(failed)
    AC_MSG_ERROR([libaal can't execute test]))

dnl Get major, minor, and micro version from arg MINIMUM-VERSION
libaal_config_major_version=`echo $1 | \
    sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
libaal_config_minor_version=`echo $1 | \
    sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
libaal_config_micro_version=`echo $1 | \
    sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`

dnl Compare MINIMUM-VERSION with libaal version
AC_TRY_RUN([
#include <stdio.h>
#include <stdlib.h>
#include <aal/libaal.h>

int main() {
    const char *version;    
    int major, minor, micro;
	
    if (!(version = libaal_version()))
	exit(1);
		
    if (sscanf(version, "%d.%d.%d", &major, &minor, &micro) != 3) {
	printf("%s, bad version string\n", version);
	exit(1);
    }
	
    if ((major >= $libaal_config_major_version) &&
	((major == $libaal_config_major_version) && 
	(minor >= $libaal_config_minor_version)) &&
	((major == $libaal_config_major_version) && 
	(minor == $libaal_config_minor_version) && 
	(micro >= $libaal_config_micro_version))) 
    {
	return 0;
    } else {
	printf("\nAn old version of libaal (%s) was found.\n",
	    version);
	printf("You need a version of libaal newer than or "
            "equal to %d.%d.%d.\n", $libaal_config_major_version, 
	    $libaal_config_minor_version, $libaal_config_micro_version);

	printf("You can get it at http://www.namesys.com/snapshots\n");
	return 1;
    }
}
], 
    AC_MSG_RESULT(yes),
    AC_MSG_RESULT(no) ; $3,
    [echo $ac_n "cross compiling; assumed OK... $ac_c"])

dnl restore orignial LIBS and set @AAL_LIBS@
AAL_LIBS="$LIBS"
LIBS="$saved_LIBS"
AC_SUBST(AAL_LIBS)

dnl Execute ACTION-IF-FOUND
$2

])

