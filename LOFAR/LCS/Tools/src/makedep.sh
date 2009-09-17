#!/bin/sh
#
#  makedep.sh: generate dependency trees (uses and used-by) for LOFAR packages
#
#  Copyright (C) 2002-2004
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
#  $Id: makedep.sh 6905 2005-10-24 14:18:17Z diepen $


# Get LOFAR root directory from this script's name.
a_dir=`dirname $0`
a_dir=`cd $a_dir > /dev/null 2>&1; pwd`
a_root=`echo $a_dir | sed -e 's%/LOFAR/.*%/LOFAR%'`

# Find out where finddep and pkgdep are located.
if [ -f $a_dir/pkgdep ]; then
  a_pkgdep=$a_dir/pkgdep
  a_finddep=$a_dir/finddep
else
  a_pkgdep=$a_root/LCS/Tools/build/gnu_debug/src/pkgdep
  a_finddep=$a_root/LCS/Tools/src/finddep
fi
if [ "$a_pkgdep" = ""  -o  ! -x "$a_pkgdep" ]; then
  a_pkgdep=`which pkgdep`
  if [ "$a_pkgdep" = ""  -o  ! -x "$a_pkgdep" ]; then
    echo "Fatal error: could not locate program \`pkgdep'"
    exit 1
  fi
  a_finddep=`which finddep`
  if [ "$a_finddep" = ""  -o  ! -x "$a_finddep" ]; then
    echo "Fatal error: could not locate program \`finddep'"
    exit 1
  fi
fi

# Execute finddep.
echo "Executing $a_finddep ..."
$a_finddep

# Execute pkgdep
echo "Executing $a_pkgdep ..."
$a_pkgdep finddep.pkg top strip xhtml hdrtxt="%pkg% Package Directory Tree" href='<a href="../scripts/makepage.php?name=%pkg%" target="description">' > finddep-pkg.html
$a_pkgdep finddep.used xhtml > finddep-used.html
$a_pkgdep finddep.uses xhtml > finddep-uses.html
$a_pkgdep finddep.used-all xhtml hdrtxt="%pkg% Cross Reference Tree<br>(shows in a recursive way the packages where %pkg% is used)" split=".used.html" 
$a_pkgdep finddep.uses-all xhtml hdrtxt="%pkg% Uses Dependency Tree<br>(shows in a recursive way the packages used by %pkg%)" split=".uses.html" 
$a_pkgdep finddep.uses-all xhtml flat hdrtxt="%pkg% Flat Dependency Tree<br>(shows the packages used by %pkg%)" split=".flat.html" 
