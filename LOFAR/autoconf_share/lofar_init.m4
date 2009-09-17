#  lofar_init.m4
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
#  $Id: lofar_init.m4 12210 2008-11-17 11:16:38Z loose $


# lofar_INIT
#
# Macro to initialize common LOFAR make variables and to
# define CXX if only CC is defined.
# It should be invoked before AC_PROG_CXX.
#
# It sets the following variables:
#  lofar_top_srcdir  user's LOFAR root (e.g. $HOME/sim/LOFAR)
#  lofar_sharedir    $lofar_top_srcdir/autoconf_share
#  lofar_root        LOFAR root which is default tree for package sources
#                    (e.g. /lofar/stable/LOFAR)
#  lofar_root_libdir LOFAR root which is default tree for package libraries
#                    It contains the string <package> which should be replaced
#                    by the actual package name.
#  lofar_use_root    0 = root is only a fallback
#                    1 = always use root (except for package to build)
#  lofar_variant     variant being built (e.g. gnu_opt or xxx)
#  lofar_compiler    compiler type used (gnu, kcc, or icc) derived from CXX
#                    Other compilers are not recognized and result in
#                    a warning.
#  lofar_gcc_major   major version of gcc compiler
#  lofar_have_libg2c 1 = libg2c is available
#  lofar_shared_ext  the extension name of a shared library (so, sl, dylib)
#  lofar_libdirext   lib or lib64 depending on enable_lib64 setting
#  lofar_no_rpath    1 = rpath cannot be used when linking
#  LOFAR_DEPEND      all lofar libraries this package is dependent on
#
# A warning is given if the compiler type mismatches the compiler part
# of the variant.
#
# The configure option --with-lofar=version:variant can be used to specify
# that a particular version and variant should be used when building
# a package. So all include files and libraries are taken from there.
# In this way a system-wide LOFAR tree can be used, so a user does not need
# to build the packages the package to built is dependent on.
# The version can be given as a path, where /home/lofar is added if only
# a version name like weekly is given.
# Default version is stable.
# Default variant is the name of the variant being configured.
# E.g. if configuring BaseSim/build/gnu_debug
#   --with-lofar=weekly:opt   -> /home/lofar/weekly  gnu_opt
#   --with-lofar=:opt         -> /home/lofar/stable  gnu_opt
#   --with-lofar=~            -> $HOME          gnu_debug
#
# It is checked whether the version and variant exist.
#
# Instead of --with-lofar, it is possible to use --with-lofar-default.
# The difference is that a package is only taken from the given lofar tree
# if not found in the user's tree. A package is not found in the user's
# tree if there is no configure file for the package in the user's tree,
# thus if the package has not been bootstrapped.
# 
#
#
AC_DEFUN([lofar_INIT],dnl
[dnl
AC_PREREQ(2.13)dnl
AC_ARG_WITH(lofar,
	[  --with-lofar[=version:variant]    root version:variant to use (default=user tree)],
	[with_lofar=$withval;
         lofar_use_root=1])

AC_ARG_WITH(lofar-default,
	[  --with-lofar-default[=version:variant]    root version:variant to use (default=user tree)],
	[with_lofar_def=$withval;
         lfr_use_root_def=1])

AC_ARG_WITH(lofar-libdir,
  [  --with-lofar-libdir=PFX   specific tree for lofar libraries],
  [lofar_root_libdir="$withval"])

AC_ARG_ENABLE(lib64,
	[  --enable-lib64          use lib or lib64 (default is system dependent)],
	[enable_lib64="$enableval"],
	[enable_lib64="default"])dnl

[
  # Determine the shared library extension.
  lofar_shared_ext=so
  case `uname -s` in
    Darwin*) lofar_shared_ext=dylib;;
    HP-UX)   lofar_shared_ext=sl;;
  esac

  # Determine if -rpath cannot be given.
  lofar_no_rpath=0
  if test -n "$CATAMOUNT_DIR"; then
    lofar_no_rpath=1       # Cray XT
  else
    case `uname -s` in
      Darwin*) lofar_no_rpath=1;;
    esac
  fi

  # Determine if lib64 has to be used. This is somewhat tricky for the default
  # case, because: Debian puts 64-bit libs in /lib and 32-bit libs in /lib32,
  # but SuSE and RedHat put 64-bit libs in /lib64 and 32-bit libs in /lib. 
  # We cannot use `uname -s`, since all distros return "Linux", hence we must
  # test for the presence of these lib directories instead. To further 
  # complicate matters, Debian distros may create a symlink /lib64; therefore
  # we should make sure that /lib64 is a "real" directory, not a symlink.
  lofar_libdirext=lib
  if test "$enable_lib64" = "yes" ; then
    lofar_libdirext=lib64
  elif test "$enable_lib64" = "default" ; then
    if test -d /lib64 -a ! -L /lib64 ; then
      lofar_libdirext=lib64
    fi
  else
    if test -d /lib32 -a ! -L /lib32 ; then
      lofar_libdirext=lib32
    fi
  fi

  # Handle lofar root
  LOFARROOT=$prefix  
  if test "$with_lofar" != ""  -a  "$with_lofar" != "no"  -a \
          "$with_lofar" != "yes"; then
    LOFARROOT=$with_lofar
  fi
  # The old with_lofar is obsolete now, thus always clear.
  with_lofar=
  lofar_use_root=0
  if test "$with_lofar_def" = no; then
    with_lofar=
    lfr_use_root_def=0
  fi
  if test "$lfr_use_root_def" = "1"; then
    if test "$lofar_use_root" = "1"; then
      ]AC_MSG_ERROR([--with-lofar and --with-lofar-default should not be used together])[
    fi
    with_lofar="$with_lofar_def"
    lofar_use_root=2
  fi
  if test "$with_lofar" = yes; then
    with_lofar=
  fi
  if test "x$lofar_use_root" = "x"; then
    lofar_use_root=0;
  fi
  if test "$lofar_use_root" != 0  -a  "$with_lofar" = ""; then
    with_lofar=stable
  fi
  if test "$lofar_use_root" = "2"; then
    lofar_use_root=0;
  fi
  # Find root of user LOFAR directory tree.
  lfr_top=`(cd $srcdir && pwd) | sed -e "s%\(.*\)/LOFAR/.*%\1%"`
  lofar_top_srcdir=$lfr_top/LOFAR;
  lfr_pkg=`(cd $srcdir && pwd) | sed -e "s%$lofar_top_srcdir/%%"`
  lofar_sharedir=$lofar_top_srcdir/autoconf_share
  # Determine if the build area has the compiler name in it
  # and get the root of the build tree.
  # The build area can be
  # like   something/build/variant/package  --> root = something/build/variant
  # or     something/package/build/variant  --> root = something
  lfr_curwd=`pwd`;
  lfr_curvar1=`pwd | sed -e "s%.*/build/%%"`;
  lfr_curvar=`echo $lfr_curvar1 | sed -e "s%/.*%%g"`;
  case "$lfr_curvar1" in
  */*)
    # root = something/build/variant
    lfr_curroot=`pwd | sed -e "s%/build/.*%/build/$lfr_curvar%"`
    ;;
  *)
    # root = something
    # Strip the package name from the build path
    lfr_curroot=`pwd | sed -e "s%$lfr_pkg.*%%"`
    ;;
  esac
  lfr_buildcomp=
  case "$lfr_curvar" in
  *_*)
    lfr_buildcomp=`echo $lfr_curvar | sed -e "s/_.*//"`
    ;;
  esac

  # If the C++ compiler is not given, set it to the C compiler (if given).
  if test "x$CXX" = "x"; then
    if test "x$CC" != "x"; then
      lfr_cxx=`basename $CC`;
      lfr_ccdir="";
      if test "x$lfr_cxx" != "x$CC"; then
        lfr_ccdir=`dirname $CC`/;
      elif test "x$lfr_cxx" = "xgcc"; then
        lfr_cxx="g++";
      fi
      CXX="$lfr_ccdir$lfr_cxx";
      ]AC_SUBST(CXX)[
    fi
  fi

  # Find the compiler type. Note that the default compiler is gnu g++.
  # Set the special AR needed for the KAI C++ compiler.
  lofar_compiler="gnu";
  if test "x$CXX" != "x"; then
    lfr_cxx=`basename $CXX`;
    if test "x$lfr_cxx" = "xg++"; then
      lofar_compiler="gnu";
    elif test "x$lfr_cxx" = "xCC"; then
      if test "$PE_ENV" = "PGI"; then
        lofar_compiler="pgi";             # Cray XT3
      fi
    elif test "x$lfr_cxx" = "xKCC"; then
      lofar_compiler="kcc";
      AR="$CXX";
      AR_FLAGS="-o";
      ]AC_SUBST(AR)
       AC_SUBST(AR_FLAGS)[
    elif test "x$lfr_cxx" = "xicc"; then
      lofar_compiler="icc";
    elif test "x$lfr_cxx" = "xblrts_xlC"; then
      lofar_compiler="xlc";
    else
      ]AC_MSG_WARN([$CXX is an unknown compiler for LOFAR; assuming gnu])[
    fi
  fi

  # Get the major gcc version.
  lofar_gcc_major=0
  if [ "$lofar_compiler" = "gnu" ]; then
    version=`$CXX -v 2>&1 | tail -1`
    # A typical version strings looks like this:
    #   gcc version 4.0.2 20050901 (prerelease) (SUSE Linux)
    if echo $version | grep -iq gcc; then
      # We'll assume that the first word starting with one or more digits is
      # the version number, so strip off the rest.
      version=`echo $version | sed -e 's,^[^0-9]*,,' -e 's,[ \t].*$,,'`
      # We only need the major version number.
      lofar_gcc_major=`echo $version | cut -s -d'.' -f1`
    fi
  fi

  # Find out if libg2c exists.
  lofar_have_libg2c=0
  lfr_flib=`echo $CXX | sed -e 's%bin/.*%lib%'`
  if [ -e $lfr_flib/libg2c.$lofar_shared_ext ]; then
    lofar_have_libg2c=1
  elif [ -e $lfr_flib/libg2c.a ]; then
    lofar_have_libg2c=1
  fi

  # Check if compiler matches build variant.
  if test "x$lfr_buildcomp" != "x"; then
    if test "x$lfr_buildcomp" != "x$lofar_compiler"; then
      ]AC_MSG_WARN([compiler $CXX mismatches build variant $lfr_curvar])[
    fi
  fi

  # Get the possible version:variant given. If no version is given, 
  # assume stable.
  # Empty variant means the current variant.
  # Remove trailing / and /LOFAR if user has given that (redundantly).
  lofar_root=
  lofar_variant=
  if test "x$with_lofar" != "x"; then
    lofar_root=$with_lofar;
    case "$with_lofar" in
    *:*)
      lofar_root=`echo ${with_lofar} | sed -e "s/:.*//"`
      lofar_variant=`echo ${with_lofar} | sed -e "s/.*://"`
      ;;
    esac
    lofar_root=`echo $lofar_root | sed -e 's%/$%%' -e 's%/LOFAR$%%'`;
    if test "x$lofar_root" = "x"; then
      lofar_root="stable"
    fi
  fi
  # If variant has no _, add compiler_ to it.
  if test "x$lofar_variant" = "x"; then
    lofar_variant="$lfr_curvar";
  else
    case "$lofar_variant" in
    *_*)
      ;;
    *)
      lofar_variant=${lofar_compiler}_$lofar_variant
      ;;
    esac
  fi
  # If root has no / or ~, use /data/LOFAR/installed/$lofar_root/$lofar_variant
  # as default.
  # Replace ~ by home directory.
  lfr_libdir=;
  if test "x$lofar_root" = "x"; then
    lofar_root=$lfr_top;
    lfr_libdir=$lfr_curroot;
  else
    case "$lofar_root" in
    ~*)
      lofar_root=`echo $lofar_root | sed -e "s%~%$HOME%"`
      ;;
    */*)
      ;;
    ?*)
      lofar_root=/data/LOFAR/installed/$lofar_root/$lofar_variant
      ;;
    esac
  fi
  LOFARROOT=$lofar_root
  if test "x$lfr_libdir" = "x"; then
    lfr_libdir=$lofar_root/$lofar_libdirext;
  fi
  if test "x$lofar_root_libdir" = "x"; then
    lofar_root_libdir=$lfr_libdir;
  fi

  # Create the pkgext files to remember configure settings.
  \rm -f pkgext*
  touch pkgext pkgextcppflags pkgextcxxflags pkgextldflags pkgextobjs

  # Create a symlink from the buld directory to srcdir/src in case
  # header files are in src instead of include/PACKAGE.
  # Note that the build directory is part of the -I path.
  \rm -f ${PACKAGE}
  ln -s $srcdir/src ${PACKAGE}

  # We have to deal with creating a file lofar_config.h which is the
  # common include file to be used in LOFAR software.
  # It includes config.h.
  # The reason for having lofar_config.h is that variables created
  # by lofar_package are not included in config.h by the autotools,
  # because their names are too variable and therefore not handled
  # by config.status.
  # A first attempt was to create lofar_config.h in the lofarconf script
  # which worked fine. However, sometimes make forced a new configure
  # which does not use the lofarconf script. Hence the include files
  # were not correct.
  # Therefore everything is now done in the m4 scripts.
  # There are 2 problems to be solved:
  # 1. An #endif has to be inserted at the end.
  # 2. If the new lofar_config.h file is the same as the old one,
  #    the old one should be used to avoid recompilations.
  # Both problems are tackled in lofar_init and lofar_package by
  # having a lofar_config.old-h for the old lofar_config.h file
  # and a lofar_config.h-pkg to be added to.

  # Save the current lofar_config.h
  touch lofar_config.old-h
  if [ -f lofar_config.h ]; then
    mv lofar_config.h lofar_config.old-h
  fi
  # Create the lofar_config.h file.
  \rm -f lofar_config.h*
  echo "/* Generated by lofar_init.m4 */" >> lofar_config.h-pkg
  echo "" >> lofar_config.h-pkg
  echo "#ifndef LOFAR_CONFIG_H" >> lofar_config.h-pkg
  echo "#define LOFAR_CONFIG_H" >> lofar_config.h-pkg
  echo "" >> lofar_config.h-pkg
  echo "#if defined(HAVE_CONFIG_H)" >> lofar_config.h-pkg
  echo "#include <config.h>" >> lofar_config.h-pkg
  echo "#endif" >> lofar_config.h-pkg
  echo "" >> lofar_config.h-pkg
  # PACKAGE is the package name which is defined by the auto tools.
  # Define a line for the package being configured.
  lfr_upkg=`echo $PACKAGE | tr a-z A-Z`
  echo "#define HAVE_LOFAR_$lfr_upkg 1" >> lofar_config.h-pkg
  echo "" >> lofar_config.h-pkg;
  # Define the package also for the lofarlogger.
  # Strip after /build which is added by make distcheck.
  lfr_llpkg=`echo $lfr_pkg | sed -e "s%/build/.*%%" -e "s%/%.%g"`
  echo "#define LOFARLOGGER_PACKAGE \"$lfr_llpkg\"" >> lofar_config.h-pkg;
  # Do the finalization (in case no lofar_package is used).
  cp lofar_config.h-pkg lofar_config.h
  echo "" >> lofar_config.h
  echo "#endif" >> lofar_config.h
  # If the current lofar_config.h is the same as the old one, copy the
  # old one back, while preserving creation date and time.
  diff lofar_config.h lofar_config.old-h > /dev/null 2>&1
  if [ $? = 0 ]; then
    cp -p lofar_config.old-h lofar_config.h
  fi

  # Similarly files FillPackage__VersionInc/Func.h are created to contain
  # the body of the fillInfo function.
  # It is included in Package__Version.cc created by the makeversion script.
  touch FillPackage__VersionInc.old-h
  if [ -f FillPackage__VersionInc.h ]; then
    mv FillPackage__VersionInc.h FillPackage__VersionInc.old-h
  fi
  # Create the FillPackage__VersionInc.h file.
  \rm -f FillPackage__VersionInc.h*
  echo "/* Generated by lofar_init.m4 */" >> FillPackage__VersionInc.h
  echo "" >> FillPackage__VersionInc.h
  # If the current FillPackage__VersionInc.h is the same as the old one,
  # copy the old one back, while preserving creation date and time.
  diff FillPackage__VersionInc.h FillPackage__VersionInc.old-h > /dev/null 2>&1
  if [ $? = 0 ]; then
    cp -p FillPackage__VersionInc.old-h FillPackage__VersionInc.h
  fi

  touch FillPackage__VersionFunc.old-h
  if [ -f FillPackage__VersionFunc.h ]; then
    mv FillPackage__VersionFunc.h FillPackage__VersionFunc.old-h
  fi
  # Create the FillPackage__VersionFunc.h file.
  \rm -f FillPackage__VersionFunc.h*
  cat > FillPackage__VersionFunc.h-pkg <<EOF
/* Generated by lofar_init.m4 */

namespace LOFAR {
  std::string ${PACKAGE}Version::getBuildTime()
    { return "`date`"; }
  std::string ${PACKAGE}Version::getBuildUser()
    { return "`whoami`"; }
  std::string ${PACKAGE}Version::getBuildMachine()
    { return "`uname -n`"; }

  void ${PACKAGE}Version::fillInfo
                        (int level, std::vector<Version::PkgInfo>& vec)
  {
    // Add info of this package if needed.
    vec.push_back (Version::PkgInfo(level, &getInfo));
    // Add info of packages being used.
EOF
  # Do the finalization (in case no lofar_internal is used).
  cp FillPackage__VersionFunc.h-pkg FillPackage__VersionFunc.h
  echo '  }' >> FillPackage__VersionFunc.h
  echo '}' >> FillPackage__VersionFunc.h
  # If the current FillPackage__VersionFunc.h is the same as the old one,
  # copy the old one back, while preserving creation date and time.
  diff FillPackage__VersionFunc.h FillPackage__VersionFunc.old-h > /dev/null 2>&1
  if [ $? = 0 ]; then
    cp -p FillPackage__VersionFunc.old-h FillPackage__VersionFunc.h
  fi

  # Use absolute path (make distcheck sets srcdir to ..).
  srcdirx=`cd $srcdir && pwd`
  CPPFLAGS="$CPPFLAGS -I$lfr_curwd -I$srcdirx/include"
  LOFAR_DEPEND=

  case $lofar_root_libdir in
  */build/*)
    lfr_find=$lofar_root_libdir/LCS/Common
    lofar_root_libdir="$lofar_root_libdir/<package>"
    ;;
  *)
    lfr_find=$lofar_root_libdir/LCS/Common/build/$lofar_variant
    lofar_root_libdir="$lofar_root_libdir/<package>/build/$lofar_variant"
    ;;
  esac

]
  # Make sure that, if needed, libraries are installed in lib64.
  if test "$libdir" = '${exec_prefix}/lib'; then
    libdir='${exec_prefix}/'$lofar_libdirext
    AC_SUBST(libdir,"$libdir")
  fi

  AC_SUBST(lofar_root)
  AC_SUBST(lofar_root_libdir)
  AC_SUBST(lofar_use_root)
  AC_SUBST(lofar_compiler)
  AC_SUBST(lofar_variant)
  AC_SUBST(lofar_top_srcdir)
  AC_SUBST(lofar_sharedir)
  AC_SUBST(LOFAR_DEPEND)
  AC_SUBST(CPPFLAGS)
  AC_SUBST(LDFLAGS)
  AC_SUBST(LOFARROOT)

# Check for endianness. 
# If the system is big-endian WORDS_BIGENDIAN will be defined. 
  AC_C_BIGENDIAN

# Initially RPM builds are not possible.
  MAKE_RPMS=false
  AC_SUBST(MAKE_RPMS)
#  AM_CONDITIONAL(MAKE_RPMS, test xfalse = xtrue)
])
