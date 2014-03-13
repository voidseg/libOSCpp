dnl process with autoconf

AC_INIT([libOSC++],[0.10.2],[lists@rumori.de])

dnl this is necessary for the AC_CHECK_TYPES macro
AC_PREREQ(2.57)
AC_CONFIG_SRCDIR([src/InetOSC.cc])
AC_CANONICAL_TARGET([])
AM_INIT_AUTOMAKE($PACKAGE_NAME,$PACKAGE_VERSION)
AM_CONFIG_HEADER(config.h)
LT_INIT()

ifdef([m4_pattern_allow], [m4_pattern_allow([AC_PROG_LIBTOOL])])
AC_PROG_LIBTOOL

dnl Here are a set of rules to help you update your library version
dnl information:

dnl   1. Start with version information of `0:0:0' for each libtool library.

dnl   2. Update the version information only immediately before a public
dnl      release of your software.  More frequent updates are unnecessary,
dnl      and only guarantee that the current interface number gets larger
dnl      faster.

dnl   3. If the library source code has changed at all since the last
dnl      update, then increment REVISION (`C:R:A' becomes `C:r+1:A').

dnl   4. If any interfaces have been added, removed, or changed since the
dnl      last update, increment CURRENT, and set REVISION to 0.

dnl   5. If any interfaces have been added since the last public release,
dnl      then increment AGE.

dnl   6. If any interfaces have been removed since the last public release,
dnl      then set AGE to 0.

dnl    *_Never_* try to set the interface numbers so that they correspond
dnl to the release number of your package.  This is an abuse that only
dnl fosters misunderstanding of the purpose of library versions.  Instead,
dnl use the `-release' flag (Note: Release numbers), but be warned that
dnl every release of your package will not be binary compatible with any
dnl other release.
SHARED_VERSION_INFO="0:0:0"

AC_CHECK_PROG(autogen, autogen, yes, no)

dnl check for programs
# Find the compiler
AC_PROG_CC
AC_PROG_CPP
AC_PROG_CXX

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for header files.
AC_HEADER_STDC([])

dnl check for ipv6 stuff
AC_CHECK_HEADERS([arpa/inet.h])
AC_CHECK_HEADERS([inttypes.h])

dnl BSD doesn't have socklen_t, you use ints instead
dnl AC_CHECK_TYPES([socklen_t],,,[#include <sys/socket.h>])
dnl above check appears to be disfunctional: using workaround
AC_C_CONST([])

AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <sys/types.h>
				#include <sys/socket.h>]], [[socklen_t len = 42; return 0;]])],[ac_cv_type_socklen_t=yes],[ac_cv_type_socklen_t=no])
AC_MSG_CHECKING([for socklen_t])
if test $ac_cv_type_socklen_t != yes; then
    AC_DEFINE(socklen_t, int32_t, [if socklen_t is not defined, provide something useful])
    AC_MSG_RESULT([no])
else
    AC_DEFINE(HAVE_SOCKLEN_T, 1, [if socklen_t is defined, make note of it])
    AC_MSG_RESULT([yes])
fi

dnl enable broadcaster classes
AC_ARG_ENABLE(broadcaster_classes,
        AC_HELP_STRING(--disable-broadcaster-classes,compile without Master/QueuedBroadcaster (libpthread not required)),
        enable_broadcaster_classes=no, enable_broadcaster_classes=yes)

if test "${enable_broadcaster_classes}" = yes; then
   AC_CHECK_FUNC(pthread_create, [],
	AC_CHECK_LIB(pthread, pthread_create, [],
        AC_MSG_ERROR([POSIX threads library not found])))
   LIBOSC_LDFLAGS="${LIBOSC_LDFLAGS} -lpthread"   
fi
AM_CONDITIONAL(ENABLE_BROADCASTER_CLASSES, test "${enable_broadcaster_classes}" = yes)

dnl debug flag
AC_ARG_ENABLE(debug,
        AC_HELP_STRING(--enable-debug,compile with debugging symbols),
        enable_debug=yes, enable_debug=no)
AM_CONDITIONAL(LIBOSC_DEBUG, test "${enable_debug}" = yes)

dnl verbose flag
AC_ARG_ENABLE(verbose,
        AC_HELP_STRING(--enable-verbose,enable verbose debugging output),
        enable_verbose=yes, enable_verbose=no)
AM_CONDITIONAL(LIBOSC_VERBOSE, test "${enable_verbose}" = yes)

dnl reply behavior. see InetUDPMaster.cc, InetUDPReplier.cc and ticket #1.
dnl defaults to not disabled -> LIBOSC_REPLY defined
AC_ARG_ENABLE(reply,
        AC_HELP_STRING(--disable-reply,break replying to senders but be faster),
        enable_reply=no, enable_reply=yes)
AM_CONDITIONAL(LIBOSC_REPLY, test "${enable_reply}" = yes)

dnl temporary fix: mikmod-server breaks build when mikmod not installed
AC_ARG_ENABLE(tests,
        AC_HELP_STRING(--enable-tests,enable building of tests),
        enable_tests=yes, enable_tests=no)
AM_CONDITIONAL(LIBOSC_TESTS, test "${enable_tests}" = yes)

AC_SUBST(SHARED_VERSION_INFO)
AC_SUBST(LIBOSC_LDFLAGS)

AC_CONFIG_FILES([
	Makefile
	src/Makefile
	include/Makefile
	examples/Makefile
	tests/Makefile
])
AC_OUTPUT

cat << EOF

libOSC++ configuration summary
------------------------------
libOSC++ version: ${VERSION}
compile with broadcaster classes (pthread): "${enable_broadcaster_classes}"
compile with debugging symbols: "${enable_debug}"
compile with verbose debugging output: "${enable_verbose}"

EOF

# EOF
