#  lofar_aips.m4
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
#  $Id: lofar_aips++.m4 13796 2009-08-18 12:11:15Z schoenmakers $


# lofar_AIPSPP
#
# Macro to check for casacore and/or AIPS++ installation
# If AIPS++ functions are used that use LAPACK, lofar_LAPACK should
# also be made part of the configure.in (after lofar_AIPSPP).
#
# lofar_AIPSPP(option,libraries)
#     option 0 means that AIPS++ is optional, otherwise mandatory.
#     the libraries are optional and can be given as e.g. "-ltables -lcasa"
#
# e.g. lofar_AIPSPP
# -------------------------
#
AC_DEFUN([lofar_AIPSPP],dnl
[dnl
AC_PREREQ(2.13)dnl
ifelse($1, [], [lfr_option=0], [lfr_option=$1])
ifelse($2, [], [lfr_aipslibs="-lms -lmeasures -ltables -lscimath -lscimath_f -lcasa"], [lfr_aipslibs=$2])
AC_ARG_WITH(casacore,
	[  --with-casacore=PFX         enable use of casacore (via explicit path)],
	[with_casacore="$withval"],
	[with_casacore=""])
AC_ARG_WITH(aipspp,
	[  --with-aipspp=PFX           enable use of AIPS++ (via explicit path)],
	[with_aipspp="$withval"],
	[with_aipspp=""])
AC_ARG_WITH(pgplot,
	[  --with-pgplot[=PFX]         enable use of PGPLOT if needed by AIPS++],
	[with_pgplot="$withval"],
	[with_pgplot=""])
AC_ARG_WITH(wcs,
	[  --with-wcs[=PFX]            specific path for wcslib if needed by AIPS++],
	[with_wcs="$withval"],
	[with_wcs=""])
AC_ARG_WITH(wcs-libdir,
	[  --with-wcs-libdir[=PFX]     specific path for wcslib library if needed by AIPS++],
	[with_wcs_libdir="$withval"],
	[with_wcs_libdir=""])
[


if test "$with_casacore" = ""; then
    with_casacore=no;
fi
if test "$with_aipspp" = ""; then
    with_aipspp=no;
fi
if test "$with_pgplot" = ""; then
    with_pgplot=no;
fi
if test "$with_wcs" = ""; then
    with_wcs=yes;
    if test "$with_casacore" = "no" -a "$with_aipspp" = "no"; then
        with_wcs=no;
    else
        lfr_al=`echo "$lfr_aipslibs " | sed -e 's/-lcoordinates //'`
        if test "lfr_al" = "lfr_aipslibs "; then
            with_wcs=no
        fi
    fi
fi

# the path where the libraries can be found
CASAC_LIB_PATH=
AIPSPP_LIB_PATH=
# the path where the include files can be found
CASAC_INC_PATH=
AIPSPP_INC_PATH=

AIPSPP_CPPFLAGS=
AIPSPP_LDFLAGS=
AIPSPP_LIBS=

lfr_use_casa=0
# check the --with-casacore option
if test "$with_casacore" != "no"; then
  lfr_use_casa=1
  AIPSPP=$with_casacore
  CASAC_LIB_PATH=$with_casacore/lib
  CASAC_INC_PATH=$with_casacore/include/casacore
  ]AC_CHECK_FILE([$CASAC_LIB_PATH/libcasa_casa.$lofar_shared_ext],
			[lfr_ext_dir=yes],
			[lfr_ext_dir=no])[
  if test "$lfr_ext_dir" = "no" ; then
    ]AC_CHECK_FILE([$CASAC_LIB_PATH/libcasa_casa.a],
			[lfr_ext_dir=yes],
			[lfr_ext_dir=no])[
  fi
  if test "$lfr_ext_dir" = "no" ; then
    ]AC_MSG_ERROR([Could not find casacore library libcasa_casa in $CASAC_LIB_PATH])[
  fi
  ]AC_CHECK_FILE([$CASAC_INC_PATH/casa/aips.h],
			[lfr_ext_dir=yes],
			[lfr_ext_dir=no])[
  if test "$lfr_ext_dir" = "no" ; then
    ]AC_MSG_ERROR([Could not find casacore header casa/aips.h in $CASAC_INC_PATH])[
  fi
  AIPSPP_CPPFLAGS="$AIPSPP_CPPFLAGS -I$CASAC_INC_PATH"
  AIPSPP_LDFLAGS="$AIPSPP_LDFLAGS -L$CASAC_LIB_PATH"
  if test "$lofar_no_rpath" = 0; then
    AIPSPP_LDFLAGS="$AIPSPP_LDFLAGS -Wl,-rpath,$CASAC_LIB_PATH"
  fi
fi

# check the --with-aipspp option
if test "$with_aipspp" != "no"; then
  lfr_use_casa=1
  AIPSPP=$with_aipspp
  AIPSPP_LIB_PATH=$with_aipspp/lib
  AIPSPP_INC_PATH=`dirname $with_aipspp`/code/include
  # Look for lib directory.
  ]AC_CHECK_FILE([$AIPSPP_LIB_PATH],
			[lfr_ext_dir=yes],
			[lfr_ext_dir=no])[
  if test "$lfr_ext_dir" = "no" ; then
    ]AC_MSG_ERROR([Could not find AIPS++ library directory $AIPSPP_LIB_PATH])[
  fi
  ]AC_CHECK_FILE([$AIPSPP_INC_PATH],
			[lfr_ext_dir=yes],
			[lfr_ext_dir=no])[
  if test "$lfr_ext_dir" = "no" ; then
    ]AC_MSG_ERROR([Could not find AIPS++ include directory $AIPSPP_INC_PATH])[
  fi
  AIPSPP_CPPFLAGS="$AIPSPP_CPPFLAGS -I$AIPSPP_INC_PATH"
  AIPSPP_LDFLAGS="$AIPSPP_LDFLAGS -L$AIPSPP_LIB_PATH"
  if test "$lofar_no_rpath" = 0; then
    AIPSPP_LDFLAGS="$AIPSPP_LDFLAGS -Wl,-rpath,$AIPSPP_LIB_PATH"
  fi
##  AIPSPP_LIBS="$AIPSPP_LIBS $AIPSPP_LIB_PATH/version.o"
fi

# Do we have enough info?
if test "$CASAC_LIB_PATH" = "" -a "$AIPSPP_LIB_PATH" = ""; then
  if test "$lfr_option" != "0"; then
    ]AC_MSG_ERROR([AIPS++ is needed, but neither --with-casacore=path nor --with-aipspp=path has been given])[
  fi
fi

# Set build variables.
if [ "$lfr_use_casa" = 1 ]; then
  case `uname -s` in
    SunOS)   darch="-DAIPS_SOLARIS";;
    Linux)   darch="-DAIPS_LINUX";;
    IRIX32)  darch="-DAIPS_IRIX";;
    IRIX64)  darch="-DAIPS_IRIX";;
    Darwin*) darch="-DAIPS_DARWIN";;
    *)       darch=;;
  esac
  AIPSPP_CPPFLAGS="$AIPSPP_CPPFLAGS $darch -DAIPS_STDLIB"
  if test "$CASAC_LIB_PATH" = ""; then
    AIPSPP_CPPFLAGS="$AIPSPP_CPPFLAGS -DAIPS_NO_TEMPLATE_SRC"
  fi
  # Check if we're on a x86-64 bit system.
  if test "`uname -m`" = "x86_64"; then
    AIPSPP_CPPFLAGS="$AIPSPP_CPPFLAGS -DAIPS_64B"
  fi

  # For casacore, add casa_ to the libraries.
  if test "$CASAC_LIB_PATH" != ""; then
    clibs=`ls $CASAC_LIB_PATH/libcasa_* | sed -e 's%.*/%%g' -e 's%libcasa_%%g' -e 's%\..*%%g' | sort | uniq`
    for LIB in $clibs
    do
      lfr_aipslibs=`echo "$lfr_aipslibs " | sed -e "s/-l$LIB /-lcasa_$LIB /"`
    done
  fi
  AIPSPP_LIBS="$AIPSPP_LIBS $lfr_aipslibs"
fi

# Test for wcs.
if [ "$with_wcs" != "no" ]; then
  if [ "$with_wcs" = "yes" ]; then
    with_wcs=`echo $AIPSPP_INC_PATH | sed -e 's%/include$%/casa%'`
  fi
  ]AC_CHECK_FILE([$with_wcs/wcslib/wcs.h],
	        [lfr_wcs=$with_wcs], [lfr_wcs=no])[
  if test $lfr_wcs = no; then
    ]AC_CHECK_FILE([$with_wcs/include/wcslib/wcs.h],
	          [lfr_wcs=$with_wcs/include], [lfr_wcs=no])[
    if test $lfr_wcs = no; then
      ]AC_MSG_ERROR([wcslib/wcs.h not found in WCS directory $with_wcs])[
    fi
  fi
  AIPSPP_CPPFLAGS="$AIPSPP_CPPFLAGS -I$lfr_wcs"
  if [ "$with_wcs_libdir" = "" ]; then
    if [ "$AIPSPP_LIB_PATH" != "" ]; then
      with_wcs_libdir=$AIPSPP_LIB_PATH
    else
      with_wcs_libdir=$with_wcs/lib
    fi
  fi
  ]AC_CHECK_FILE([$with_wcs_libdir/libwcs.$lofar_shared_ext],
			[lfr_wcs_dir=yes],
			[lfr_wcs_dir=no])[
  if test "$lfr_wcs_dir" = "no" ; then
    ]AC_CHECK_FILE([$with_wcs_libdir/libwcs.a],
			[lfr_wcs_dir=yes],
			[lfr_wcs_dir=no])[
  fi
  if test "$lfr_wcs_dir" = "no" ; then
    ]AC_MSG_ERROR([Could not find libwcs in $with_wcs_libdir])[
  fi
  if [ "$with_wcs_libdir" != "$AIPSPP_LIB_PATH" ]; then
    AIPSPP_LDFLAGS="$AIPSPP_LDFLAGS -L$with_wcs_libdir"
    if test "$lofar_no_rpath" = 0; then
      AIPSPP_LDFLAGS="$AIPSPP_LDFLAGS -Wl,-rpath,$with_wcs_libdir"
    fi
  fi
  AIPSPP_LIBS="$AIPSPP_LIBS -lwcs"
fi

# Test for pgplot.
if test "$with_pgplot" != "no"; then
  ]AC_CHECK_FILE([$with_pgplot],
        [lfr_pg=yes], [lfr_pg=no])[
  if test $lfr_pg = no; then
    ]AC_MSG_ERROR([given PGPLOT directory not found])[
  fi
  AIPSPP_LDFLAGS="$AIPSPP_LDFLAGS -L$with_pgplot"
  if test "$lofar_no_rpath" = 0; then
    AIPSPP_LDFLAGS="$AIPSPP_LDFLAGS -Wl,-rpath,$with_pgplot"
  fi
  AIPSPP_LIBS="$AIPSPP_LIBS -lcpgplot -lpgplot"
fi

# If we're using GCC 4.x, we need to replace -lg2c by -lgfortran.
if test $lofar_gcc_major -ge 4; then
  if test $lofar_have_libg2c = 1; then
    AIPSPP_LIBS=`echo "$AIPSPP_LIBS " | sed -e 's/\(-lgfortran  *\)\?-lg2c /-lgfortran -lg2c /g'`
  else
    AIPSPP_LIBS=`echo "$AIPSPP_LIBS " | sed -e 's/-lg2c /-lgfortran /'`
  fi
fi

if [ "$lfr_use_casa" = 1 ]; then
  echo AIPSPP >> pkgext
  echo "$AIPSPP_CPPFLAGS" >> pkgextcppflags
  echo "$AIPSPP_LDFLAGS" >> pkgextldflags

  CPPFLAGS="$CPPFLAGS $AIPSPP_CPPFLAGS"
  LDFLAGS="$LDFLAGS $AIPSPP_LDFLAGS"
  LIBS="$LIBS $AIPSPP_LIBS -ldl"

  if test "$lofar_compiler" = "gnu"; then
    CXXFLAGS="$CXXFLAGS -Wno-non-template-friend"
    echo "-Wno-non-template-friend" >> pkgextcxxflags
  fi
  ]
   AC_DEFINE(HAVE_AIPSPP, $lfr_use_casa, [Define if AIPS++ is installed])dnl
  [
fi
    ]
dnl
    AC_SUBST(AIPSPP)dnl
    AC_SUBST(CPPFLAGS)dnl
    AC_SUBST(CXXFLAGS)dnl
    AC_SUBST(LDFLAGS)dnl
    AC_SUBST(LIBS)dnl
dnl
])
