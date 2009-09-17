#  lofar_internal.m4
#
#  Copyright (C) 2004
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
#  $Id: lofar_internal.m4 12082 2008-11-03 09:13:35Z diepen $


# lofar_INTERNAL
#
# Macro to check for an INTERNAL installed package
# It does the same as LOFAR_EXTERNAL, but gives a means to distinguish
# internal and external packages.
#
# lofar_INTERNAL(package-path, package, [cvs-versiontag],
#            [option], headerfile, [libraries], [searchpath],
#            [extra_cppflags],[extra_cxxflags],[extra_ldflags],[extra_libs])
#
# E.g.
#   lofar_INTERNAL(LCS/Common, Common, 1.3,, Common/LofarLogger.h)
#   lofar_INTERNAL(MAC/GCF/TM, GCFTM, 0.9,, GCF/TM/file.h)
#
#
AC_DEFUN([lofar_INTERNAL],dnl
[dnl
AC_PREREQ(2.13)dnl
lfr_pkgnam_intv=$2
lfr_hdrfile=$5
lofar_EXTERNAL($2,$4,$5,$6,$7,$8,$9,$10,$11)
[
  # Get the include path of the header file.
  # This will be used for the include path of Package_Version.h.
  if test "$lfr_hdrfile" = ""; then
    lfr_hdrpath=$lfr_pkgnam_intv
  else
    lfr_hdrpath=`dirname $lfr_hdrfile`
  fi
  # If this package was configured in, add the statements to get the
  # version of this package.
  # The Tools abd pytools packages are special and not handled.
  if [ "$enable_external" = "yes"  -a  "${lfr_pkgnam_intv}" != Tools  -a  "${lfr_pkgnam_intv}" != pytools ]; then
    echo "#include \"${lfr_hdrpath}/Package__Version.h\"" >> FillPackage__VersionInc.h
    # If the current FillPackage__VersionFunc.h is the same as the old one, copy the
    # old one back, while preserving creation date and time.
    diff FillPackage__VersionInc.h FillPackage__VersionInc.old-h > /dev/null 2>&1
    if [ $? = 0 ]; then
      cp -p FillPackage__VersionInc.old-h FillPackage__VersionInc.h
    fi

    echo "    ${lfr_pkgnam_intv}Version::fillInfo (level+1, vec);"  >> FillPackage__VersionFunc.h-pkg
    # Do the finalization.
    cp FillPackage__VersionFunc.h-pkg FillPackage__VersionFunc.h
    echo '  }' >> FillPackage__VersionFunc.h
    echo '}' >> FillPackage__VersionFunc.h
    # If the current FillPackage__VersionFunc.h is the same as the old one,
    # copy the old one back, while preserving creation date and time.
    diff FillPackage__VersionFunc.h FillPackage__VersionFunc.old-h > /dev/null 2>&1
    if [ $? = 0 ]; then
      cp -p FillPackage__VersionFunc.old-h FillPackage__VersionFunc.h
    fi
  fi
]
])
