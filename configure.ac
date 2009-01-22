dnl {{{
dnl
dnl Copyright (C) 2009 Jon-Francis Bhreitheamhnaigh.
dnl All rights reserved.
dnl
dnl This file is part of Secuutus AppServer.
dnl
dnl License:
dnl
dnl 	Secuutus is free software; you can redistribute it and/or modify
dnl	it under the terms of the GNU General Public License, version 2,
dnl	as published by the Free Software Foundation.
dnl 
dnl 	Secuutus is distributed in the hope that it will be useful, but
dnl 	WITHOUT ANY WARRANTY; without even the implied warranty of
dnl 	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl 	GNU General Public License for more details.
dnl 
dnl 	You should have received a copy of the GNU General Public License
dnl 	along with Secuutus; if not, write to:
dnl
dnl		Free Software Foundation, Inc.
dnl		59 Temple Place
dnl		Suite 330
dnl		Boston, MA 02111-1307
dnl		USA
dnl
dnl Authors:
dnl
dnl	Jon-Francis Bhreitheamhnaigh <j.f.vd@adamo-recordings.co.uk>
dnl
dnl }}}

AC_PREREQ([2.59])
AC_INIT([Secuutus], [0.1], [secuutus-bugs@lists.berlios.de])

AC_CONFIG_SRCDIR([bootstrap])
AC_CONFIG_HEADER([config.h])
 
dnl {{{ ---- [ Checks for programs ] -------------------------------------------
echo
echo -e "Checking for programs"
echo

AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_PROG_RANLIB
dnl }}} ---- [ Checks for programs ] -------------------------------------------

dnl {{{ ---- [ Checks for headers ] --------------------------------------------
echo
echo "Checking for headers..."
echo

AC_CHECK_HEADERS([fcntl.h stddef.h stdint.h stdlib.h string.h termios.h unistd.h])
dnl }}} ---- [ Checks for headers ] --------------------------------------------

dnl {{{ ---- [ Checks for typedefs, structures, and compiler characteristics ] -
echo
echo "Checking for typedefs, structures, and compiler characteristics..."
echo

AC_TYPE_INT8_T
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T

AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T

AC_TYPE_SIZE_T

AC_C_CONST
AC_C_INLINE
dnl }}} ---- [ Checks for typedefs, structures, and compiler characteristics ] -

dnl {{{ ---- [ Checks for library functions ] ----------------------------------
echo
echo "Checking for library functions..."
echo

AC_CHECK_FUNCS([asprintf getcwd memset strcasecmp strdup strerror strrchr])

INC='-I${topdir}'
LIBS='-L${topdir}/src'

AC_CHECK_LIB([config], [config_init], [LIBS="$LIBS -lconfig"], [
	AC_MSG_ERROR([Secuutus requires libconfig.])
])

AC_CHECK_LIB([curses], [initscr], [LIBS="$LIBS -lcurses"], [
	AC_MSG_ERROR([Secuutus requires libcurses.])
])


AC_SUBST(DEFS)
AC_SUBST(INC)
AC_SUBST(LIBS)
dnl }}} ---- [ Checks for library functions ] ----------------------------------

dnl {{{ ---- [ Output files ] ----------------------------------------------------
echo
echo "Generating output files..."
echo

AC_OUTPUT([Makefile.top])
dnl }}} ---- [ Output files ] ----------------------------------------------------

echo
echo "Configuration complete"
echo

dnl
dnl vim: ts=8 sw=8 noet fdm=marker :
dnl
