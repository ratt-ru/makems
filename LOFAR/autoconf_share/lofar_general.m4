#  lofar_general.m4: contains several general m4 functions
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
#  $Id: lofar_general.m4 12385 2009-01-05 11:14:13Z loose $

#
#  lofar_general.m4 contains the following m4 functions:
#    lofar_DEBUG_OPTIMIZE
#    lofar_FUNCTION_NAME
#    lofar_CHECK_USHORT
#    lofar_CHECK_UINT
#    lofar_CHECK_ULONG
#    lofar_CHECK_LONG_LONG
#

# lofar_GENERAL([rpm_release])
#   The rpm_release defaults to 1 (which is usually okay).
#   Only if the same version is repackaged, the release should be incremented.
#
# Execute the general macros.
#
AC_DEFUN([lofar_GENERAL],dnl
[dnl
ifelse($1, [], [RPM_RELEASE=1], [RPM_RELEASE=$1])
AC_DEFINE([RPM_RELEASE], $RPM_RELEASE, [Define RPM release nr])dnl
AC_SUBST(RPM_RELEASE)
AM_RPM_INIT([])
dnl Enable or disable the rpm making rules in Makefile.am
dnl AM_CONDITIONAL(MAKE_RPMS, test x$make_rpms = xtrue)
MAKE_RPMS=$make_rpms
AC_SUBST(MAKE_RPMS)

lofar_CHECK_USHORT([])
lofar_CHECK_UINT([])
lofar_CHECK_ULONG([])
lofar_CHECK_LONG_LONG([])
lofar_DEBUG_OPTIMIZE([])
lofar_FUNCTION_NAME([])
lofar_BACKTRACE([])
lofar_CHECK_INSTALL_IF_MODIFIED([])
lofar_QATOOLS([])
lofar_DOCXX([])
lofar_LOGGER([])
dnl
])


# lofar_DEBUG_OPTIMIZE
#
# Set debugging or optimisation flags for CC and CXX
#
# Please note that using --with-debug=-g and --with-optimize=-g have
# the same effect for the compiler. However, the first one also defines
# LOFAR_DEBUG in the config.h file, which the latter does not do.
# Also the first one enables DBGASSERT by default.
#
#
AC_DEFUN([lofar_DEBUG_OPTIMIZE],dnl
lofar_SHMEM([])
[dnl
AC_PREREQ(2.13)dnl
AC_ARG_WITH(debug,
	[  --with-debug     enable debugging C(XX)FLAGS (sets -g)],
	[with_debug="$withval"],
	[with_debug=no])dnl
AC_ARG_WITH(optimize,
	[  --with-optimize[=CFLAGS]  enable optimization C(XX)FLAGS (sets -O3 or +K3)],
	[with_optimize="$withval"],
	[with_optimize=no])dnl
AC_ARG_WITH(threads,
	[  --with-threads            enable support of threads],
	[with_threads="$withval"],
	[with_threads=no])dnl
AC_ARG_WITH(sockets,
	[  --with-sockets            enable support of sockets],
	[with_sockets="$withval"],
	[with_sockets=yes])dnl
AC_ARG_WITH(sse,
	[  --with-sse                enable support of sse instructions],
	[with_sse="$withval"],
	[with_sse=no])dnl
AC_ARG_WITH(cppflags,
	[  --with-cppflags=CPPFLAGS  enable extra CPPFLAGS],
	[with_cppflags="$withval"])dnl
AC_ARG_WITH(cflags,
	[  --with-cflags=CFLAGS      enable extra CFLAGS],
	[with_cflags="$withval"])dnl
AC_ARG_WITH(cxxflags,
	[  --with-cxxflags=CXXFLAGS  enable extra CXXFLAGS],
	[with_cxxflags="$withval"])dnl
AC_ARG_WITH(ldflags,
	[  --with-ldflags=LDFLAGS    enable extra LDFLAGS],
	[with_ldflags="$withval"])dnl
AC_ARG_WITH(libs,
	[  --with-libs=LIBS          enable extra LIBS],
	[with_libs="$withval"])dnl
AC_ARG_ENABLE(tracer,
	[  --disable-tracer        en/disable TRACER macros (default is debug/opt)],
	[enable_tracer="$enableval"],
	[enable_tracer="default"])dnl
AC_ARG_ENABLE(dbgassert,
	[  --enable-dbgassert      en/disable DBGASSERT macros (default is debug/opt)],
	[enable_dbgassert="$enableval"],
	[enable_dbgassert="default"])dnl
[
  if test "$with_debug" = "yes"; then
    with_debug="-g";
  fi

  if test "$lofar_compiler" = "gnu"; then
    lofar_warnflags="-W -Wall -Woverloaded-virtual -Wno-unknown-pragmas";
  fi
# Suppress KCC warnings about returning by const value and about double ;
  if test "$lofar_compiler" = "kcc"; then
    lofar_warnflags="--display_error_number --restrict --diag_suppress 815,381";
  fi

# Suppress ICC warnings about "the format string ends before this argument" end unknown pragmas;
  if test "$lofar_compiler" = "icc"; then
    lofar_warnflags="-wd161,268";
  fi

  if test "$lofar_compiler" = "xlc"; then
    lofar_warnflags="-qflag=I:W";   # show Info and up in the list file, Warnings and up on the console
  fi

  enable_debug="no";
  if test "$with_debug" != "no"; then
    enable_debug="yes";
  else
    if test "$with_optimize" = "no"; then
      enable_debug="yes";
    fi
  fi
  if test "$enable_debug" = "yes"; then]
AC_DEFINE(LOFAR_DEBUG,dnl
	1, [Define if we are compiling with debugging information])dnl
[ fi
  lfr_cppflags=
  lfr_cflags=
  lfr_cxxflags=
  lfr_ldflags=
  if test "$enable_debug" != "no"; then
    lfr_cppflags="$lfr_cppflags -DUSE_DEBUG";
    lfr_cflags="$lfr_cflags -g";
    lfr_cxxflags="$lfr_cxxflags -g $lofar_warnflags";
    if test "$enable_tracer" = "default"; then
      enable_tracer="yes";
    fi
    if test "$enable_dbgassert" = "default"; then
      enable_dbgassert="yes";
    fi
  fi

  # KAI CC uses other optimization flags.
  with_optimizecxx="$with_optimize";
  if test "$with_optimize" = "yes"; then
    with_optimize="-O3"
    if test "$lofar_compiler" = "kcc"; then
      with_optimizecxx="+K3"
    else
      with_optimizecxx="-O3"
    fi
  fi

  # Intel CC does not include /usr/local/include, while GCC does.
  if test "$lofar_compiler" = "icc"; then
    lfr_cppflags="$lfr_cppflags -I/usr/local/include"
  fi

  if test "$with_optimize" != "no"; then
    lfr_cflags="$lfr_cflags $with_optimize";
    lfr_cxxflags="$lfr_cxxflags $with_optimizecxx $lofar_warnflags";
    if test "$enable_tracer" = "default"; then
      enable_tracer="no";
    fi
    if test "$enable_dbgassert" = "default"; then
      enable_dbgassert="no";
    fi
  fi

  if test "$with_threads" != "no"; then
    if test "lofar_compiler" != xlc; then  # BG/L doesn't do threads
      lfr_cppflags="$lfr_cppflags -DUSE_THREADS -pthread";
      if test "$lofar_compiler" = "gnu"; then
        lfr_cppflags="$lfr_cppflags -D_GNU_SOURCE";
      fi

      lfr_ldflags="$lfr_ldflags -pthread";
    fi
  fi

  if test "$with_sockets" = "no"; then
    lfr_cppflags="$lfr_cppflags -DUSE_NOSOCKETS";
  fi

  if test "$with_sse" != "no"; then
    lfr_sseflags=
    if test "$lofar_compiler" = "icc"; then
      lfr_sseflags="-xW";
    else
      if test "$lofar_compiler" = "gnu"; then
        lfr_sseflags="-msse2"
      fi
    fi
    lfr_cflags="$lfr_cflags $lfr_sseflags";
    lfr_cxxflags="$lfr_cxxflags $lfr_sseflags";
  fi

  CPPFLAGS="$CPPFLAGS $lfr_cppflags $with_cppflags"
  CFLAGS="$lfr_cflags $with_cflags"
  CXXFLAGS="$lfr_cxxflags $with_cxxflags"
  LDFLAGS="$LDFLAGS $lfr_ldflags $with_ldflags"
  LIBS="$LIBS $with_libs"
]
AC_SUBST(CFLAGS)dnl
AC_SUBST(CXXFLAGS)dnl
AC_SUBST(CPPFLAGS)dnl
AC_SUBST(LDFLAGS)dnl
AC_SUBST(LIBS)dnl
[
  if test "$enable_tracer" != "no"; then]
    AC_DEFINE(ENABLE_TRACER,dnl
	1, [Define if TRACER is enabled])dnl
  [fi
  if test "$enable_dbgassert" != "no"; then]
    AC_DEFINE(ENABLE_DBGASSERT,dnl
	1, [Define if DbgAssert is enabled])dnl
  [fi

  if test "$with_debug" != "no"; then
    if test "$with_optimize" != "no"; then]
AC_MSG_ERROR([Can not have both --with-debug and --with-optimize])
[
    fi
  fi
]])dnl

#
# lofar_FUNCTION_NAME
#
# Define `AUTO_FUNCTION_NAME' as either __PRETTY_FUNCTION__, __FUNCTION__,
# or "<unknown>", depending on compiler support for function name macro.
#
AC_DEFUN([lofar_FUNCTION_NAME],[
  AC_PREREQ(2.13)
  AC_MSG_CHECKING(for function name macro)
  AC_TRY_LINK(
    [#include <stdio.h>],
    [puts(__PRETTY_FUNCTION__)],
    lofar_function_name=__PRETTY_FUNCTION__,
    AC_TRY_LINK(
      [#include <stdio.h>],
      [puts(__FUNCTION__)],
      lofar_function_name=__FUNCTION__,
      lofar_function_name="\"<unknown>\""))
  AC_MSG_RESULT($lofar_function_name)
  AC_DEFINE_UNQUOTED(AUTO_FUNCTION_NAME,$lofar_function_name,[Define as 
                     __PRETTY_FUNCTION__, __FUNCTION__, or "<unknown>"])
])

#
# lofar_BACKTRACE
#
# Check whether the C library provides support for backtrace information.
# The backtrace() function provides you with stack frame return addresses. 
# In order to translate these return addresses to filename, line number
# and function name, we need support from the binutils:
#   - libbfd contains functions to do the address translation
#   - liberty contains a function to demangle C++ function names.
#
AC_DEFUN([lofar_BACKTRACE],
[
  AC_ARG_ENABLE([backtrace],
    AS_HELP_STRING([--disable-backtrace],
                   [use backtrace(3) reporting for exceptions (default=no)]),
    [enable_backtrace="$enableval"],
    [enable_backtrace="no"])
  if test "$enable_backtrace" = "yes"; then
    AC_CHECK_HEADER([execinfo.h],[
      AC_CHECK_FUNCS([backtrace], [
        AC_DEFINE(HAVE_BACKTRACE, 1, [Define if backtrace() is available])
        # Check for -liberty and -lz first, because static library -lbfd
        # depends on them; otherwise library order in $(LIBS) will be wrong.
        AC_SEARCH_LIBS([xexit], [iberty])
        AC_SEARCH_LIBS([inflate], [z])
        AC_CHECK_HEADER([bfd.h],[
          AC_SEARCH_LIBS([bfd_init], [bfd], [
            AC_DEFINE(HAVE_BFD, 1, [Define if libbfd is available])
            AC_CHECK_HEADER([demangle.h],[
              AC_CHECK_FUNCS([cplus_demangle],,[
                AC_MSG_WARN([cplus_demangle not found, please install the GNU binutils])])],[
              AC_MSG_WARN([demangle.h not found, please install the GNU binutils])])],[
            AC_MSG_WARN([bfd_init not found, please install the GNU binutils])])],[
          AC_MSG_WARN([bfd.h not found, please install the GNU binutils])])],[
        AC_MSG_ERROR([backtrace not found in glibc])])],[
      AC_MSG_ERROR([execinfo.h not found, please install glibc-devel])])
  fi
  AM_CONDITIONAL(USE_BACKTRACE, test "$enable_backtrace" = "yes")
])

#
# lofar_CHECK_USHORT
#
# If the sys/types.h supports `ushort' types,  define `HAVE_USHORT'.
#
# Based on ICE and DDD autoconf macros.
#
AC_DEFUN([lofar_CHECK_USHORT],[
  AC_PREREQ(2.13)
  AC_REQUIRE([AC_PROG_CXX])
  AC_MSG_CHECKING(whether sys/types.h defines ushort)
  AC_CACHE_VAL(lofar_cv_have_ushort,[
    AC_LANG_PUSH(C++)
    AC_TRY_COMPILE(
      [#include <sys/types.h>],
      [ushort a;],
      lofar_cv_have_ushort=yes, 
      lofar_cv_have_ushort=no)
    AC_LANG_POP(C++)
  ])
  AC_MSG_RESULT($lofar_cv_have_ushort)
  if test "$lofar_cv_have_ushort" = yes; then
    AC_DEFINE(HAVE_USHORT,1,[Define if `ushort' is supported])
  fi
  echo "USHORT" >> pkgext;
])

#
# lofar_CHECK_UINT
#
# If the sys/types.h supports `uint' types,  define `HAVE_UINT'.
#
# Based on ICE and DDD autoconf macros.
#
AC_DEFUN([lofar_CHECK_UINT],[
  AC_PREREQ(2.13)
  AC_REQUIRE([AC_PROG_CXX])
  AC_MSG_CHECKING(whether sys/types.h defines uint)
  AC_CACHE_VAL(lofar_cv_have_uint,[
    AC_LANG_PUSH(C++)
    AC_TRY_COMPILE(
      [#include <sys/types.h>],
      [uint a;],
      lofar_cv_have_uint=yes, 
      lofar_cv_have_uint=no)
    AC_LANG_POP(C++)
  ])
  AC_MSG_RESULT($lofar_cv_have_uint)
  if test "$lofar_cv_have_uint" = yes; then
    AC_DEFINE(HAVE_UINT,1,[Define if `uint' is supported])
  fi
  echo "UINT" >> pkgext;
])

#
# lofar_CHECK_ULONG
#
# If the sys/types.h supports `ulong' types,  define `HAVE_ULONG'.
#
# Based on ICE and DDD autoconf macros.
#
AC_DEFUN([lofar_CHECK_ULONG],[
  AC_PREREQ(2.13)
  AC_REQUIRE([AC_PROG_CXX])
  AC_MSG_CHECKING(whether sys/types.h defines ulong)
  AC_CACHE_VAL(lofar_cv_have_ulong,[
    AC_LANG_PUSH(C++)
    AC_TRY_COMPILE(
      [#include <sys/types.h>],
      [ulong a;],
      lofar_cv_have_ulong=yes, 
      lofar_cv_have_ulong=no)
    AC_LANG_POP(C++)
  ])
  AC_MSG_RESULT($lofar_cv_have_ulong)
  if test "$lofar_cv_have_ulong" = yes; then
    AC_DEFINE(HAVE_ULONG,1,[Define if `ulong' is supported])
  fi
  echo "ULONG" >> pkgext;
])

#
# lofar_CHECK_LONG_LONG
#
# If the C++ compiler supports `long long' types,  define `HAVE_LONG_LONG'.
#
# Based on ICE and DDD autoconf macros.
#
AC_DEFUN([lofar_CHECK_LONG_LONG],[
  AC_PREREQ(2.13)
  AC_REQUIRE([AC_PROG_CXX])
  AC_MSG_CHECKING(whether ${CXX} supports long long)
  AC_CACHE_VAL(lofar_cv_have_long_long,[
    AC_LANG_PUSH(C++)
    AC_TRY_COMPILE(,[long long a;], 
      lofar_cv_have_long_long=yes, 
      lofar_cv_have_long_long=no)
    AC_LANG_POP(C++)
  ])
  AC_MSG_RESULT($lofar_cv_have_long_long)
  if test "$lofar_cv_have_long_long" = yes; then
    AC_DEFINE(HAVE_LONG_LONG,1,[Define if `long long' is supported])
  fi
  echo "LONG_LONG" >> pkgext;
])

#
# lofar_CHECK_SSTREAM
#
# If the C++ compiler supports the stringstream classes, define `HAVE_SSTREAM'.
#
AC_DEFUN([lofar_CHECK_SSTREAM],[
  AC_PREREQ(2.13)
  AC_REQUIRE([AC_PROG_CXX])
  AC_MSG_CHECKING(whether ${CXX} supports stringstream)
  AC_CACHE_VAL(lofar_cv_have_sstream, [
    AC_LANG_PUSH(C++)
    AC_TRY_COMPILE(
      [#include <sstream>],
      [std::istringstream i;
       std::ostringstream o;],
      lofar_cv_have_sstream=yes,
      lofar_cv_have_sstream=no)
    AC_LANG_POP(C++)
  ])
  AC_MSG_RESULT($lofar_cv_have_sstream)
  if test "$lofar_cv_have_sstream" = yes; then
    AC_DEFINE(HAVE_SSTREAM,1,[Define if `sstream' is supported])
  fi
  echo "SSTREAM" >> pkgext;
])

#
# lofar_CHECK_INSTALL_IF_MODIFIED
#
# Check if the install utility and the script install-sh support the
# BSD install option -C (install if modified). If they do, add the
# option to the command definition.
#
# From the BSD install man page:
#   -C   Install file, unless target already exists and is the same as the
#        new file, in which case the modification time won't be changed.
#
AC_DEFUN([lofar_CHECK_INSTALL_IF_MODIFIED],[
  AC_PREREQ(2.13)
  AC_REQUIRE([AC_PROG_INSTALL])
#
  AC_MSG_CHECKING(whether ${INSTALL} supports -C option)
  touch tmp$$
  if ${INSTALL} -C tmp$$ /tmp 2>/dev/null; then
    lofar_cv_install_if_modified=yes
  else
    lofar_cv_install_if_modified=no
  fi
  rm -f tmp$$ /tmp/tmp$$
  AC_MSG_RESULT($lofar_cv_install_if_modified)
  if test "$lofar_cv_install_if_modified" = yes; then
    INSTALL="${INSTALL} -C"
  fi
#
  AC_MSG_CHECKING(whether ${install_sh} supports -C option)
  touch tmp$$
  if ${install_sh} -C tmp$$ /tmp 2>/dev/null; then
    lofar_cv_install_sh_if_modified=yes
  else
    lofar_cv_install_sh_if_modified=no
  fi
  rm -f tmp$$ /tmp/tmp$$
  AC_MSG_RESULT($lofar_cv_install_sh_if_modified)
  if test "$lofar_cv_install_sh_if_modified" = yes; then
    install_sh="${install_sh} -C"
  fi
])

