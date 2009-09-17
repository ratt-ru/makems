#  lofar_python.m4
#
#  Copyright (C) 2002
#  ASTRON (Netherlands Foundation for Research in Astronomy)
#  P.O.Box 2, 7990 AA Dwingeloo, The Netherlands, seg@astron.nl
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
#  $Id: lofar_python.m4 12496 2009-01-23 13:03:47Z loose $


# lofar_PYTHON
#
# Macro to check for PYTHON installation
#
# lofar_PYTHON()
# --------------
#
AC_DEFUN([lofar_PYTHON],dnl
[dnl
AC_PREREQ(2.13)dnl
AM_PATH_PYTHON([2.4])
[
# Find python library and include directory and append to paths
lfr_pyincdir=`$PYTHON -c "from distutils import sysconfig; print sysconfig.get_python_inc()"`
lfr_pylibdir=`$PYTHON -c "from distutils import sysconfig; print sysconfig.PREFIX"`/$lofar_libdirext
lfr_pyvers=`$PYTHON -c "from distutils import sysconfig; print sysconfig.get_python_version()"`

lfr_pylibs=
case `uname -s` in
  Darwin*)
    lfr_pylibdir="-framework Python"
    ;;
  *)
    lfr_pylibs="-lpython$lfr_pyvers"
    lfr_pylibdir="-L$lfr_pylibdir"
    ;;
esac

CPPFLAGS="$CPPFLAGS -I$lfr_pyincdir"
LDFLAGS="$LDFLAGS $lfr_pylibdir"
LIBS="$libs $lfr_pylibs"
]
    AC_SUBST(CPPFLAGS)dnl
    AC_SUBST(LDFLAGS)dnl
    AC_SUBST(LIBS)dnl

])
