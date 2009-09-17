#  lofar_logger.m4
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
#  $Id: lofar_logger.m4 10500 2007-09-07 13:44:01Z diepen $


# lofar_LOGGER
#
# Macro to check for logger (log4cplus, log4cxx or none)
#
AC_DEFUN([lofar_LOGGER],dnl
[dnl
lofar_HEADER_LOG4CPLUS([])dnl
lofar_HEADER_LOG4CXX([])dnl
[
enable_logger=0
if test "$enable_log4cplus" = "yes"; then
  enable_logger=${enable_logger}1
fi
if test "$enable_log4cxx" = "yes"; then
  enable_logger=${enable_logger}1
fi
if test $enable_logger -gt 1; then
]
    AC_MSG_ERROR([Cannot use more than one LOGGER implementation.])
[fi]
])


# lofar_LOG4CPLUS
#
# Macro to check for LOG4CPLUS installation
#
AC_DEFUN([lofar_HEADER_LOG4CPLUS],dnl
[dnl
AC_PREREQ(2.13)dnl
lofar_EXTERNAL(log4cplus,0,log4cplus/logger.h,,)
enable_log4cplus=$enable_external
])


# lofar_LOG4CXX
#
# Macro to check for LOG4CXX installation
#
AC_DEFUN([lofar_HEADER_LOG4CXX],dnl
[dnl
AC_PREREQ(2.13)dnl
lofar_EXTERNAL(log4cxx,0,log4cxx/logger.h,,)
enable_logger=$enable_external
])
