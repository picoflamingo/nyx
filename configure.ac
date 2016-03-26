dnl libnyx. The working stuff in the shadows
dnl Author: David Martinez Oliveira <dmo@papermint-designs.com>
dnl
dnl __LICENSE__
dnl


m4_include([version.m4])
AC_INIT([nyx], VERSION_NUMBER, [dmo@papermint-designs.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

AC_CONFIG_SRCDIR([src/nyx.c])
AC_CONFIG_MACRO_DIR([m4])


# Checks for programs
AC_PROG_CC
AM_PROG_AR
AC_LIBTOOL_DLOPEN
AC_PROG_LIBTOOL

# Checks for libraries
AC_CHECK_LIB(dl, main)

# Checks for header files
AC_CONFIG_HEADERS([config.h])
AM_MAINTAINER_MODE

# Cheks for typedefs, structures and compiler characteristics
# Checks for library functions

# Configure options
AC_ARG_ENABLE(debug,
              [  --enable-debug          Enable debugging information],
              USE_DEBUG="$enableval", USE_DEBUG="no")


if test $USE_DEBUG = yes ; then 
  DEBUG=1
  BUILD_FLAGS="-Wall -g -O0 -DDEBUG "
else
  BUILD_FLAGS="-Wall -O3 "
  DEBUG=0
fi


CFLAGS=" ${BUILD_FLAGS}"
AC_SUBST(BUILD_FLAGS)

MY_TOP_DIR="`pwd`"

AC_SUBST(MY_TOP_DIR)

dnl Uncomment for special management of prefix variable
dnl 
if test "$prefix" = "NONE"; then
	NYX_BASEDIR="/usr/local"
else
	NYX_BASEDIR="$prefix"
fi

NYX_DATADIR="$NYX_BASEDIR/share/nyx"

AC_SUBST(JONES_BASEDIR)
AC_SUBST(JONES_DATADIR)

# Output Files
AC_CONFIG_FILES([Makefile \
		doc/Makefile \
		tests/Makefile \
		src/Makefile])
AC_OUTPUT


AC_MSG_RESULT(--- Building for $build_cpu in a $build_os)
AC_MSG_RESULT(--- OS          : ${build_os})
AC_MSG_RESULT(--- HOST        : ${host})
AC_MSG_RESULT(--- Prefix      : ${NYX_BASEDIR})
AC_MSG_RESULT(--- Data dir    : ${NYX_DATADIR})
AC_MSG_RESULT(--- BUILD_FLAGS : ${BUILD_FLAGS})
