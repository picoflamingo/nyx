dnl 
dnl  Nyx: A library with basic functions
dnl  Copyright (c) 2015 David Mart.nez Oliveira
dnl 
dnl  This file is part of Nyx
dnl 
dnl  Nyx is free software: you can redistribute it and/or modify
dnl  it under the terms of the GNU General Public License as published by
dnl  the Free Software Foundation, either version 3 of the License, or
dnl  (at your option) any later version.
dnl 
dnl  Nyx is distributed in the hope that it will be useful,
dnl  but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl  GNU General Public License for more details.
dnl 
dnl  You should have received a copy of the GNU General Public License
dnl  along with Nyx.  If not, see <http://www.gnu.org/licenses/>.
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

AC_SUBST(NYX_BASEDIR)
AC_SUBST(NYX_DATADIR)

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
