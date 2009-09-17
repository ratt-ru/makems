#!/bin/sh

# runtest.sh: script to assay a test program
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
#  $Id: runtest.sh 14028 2009-09-17 07:54:50Z loose $

# Set default options.
NEEDOUTFIL=0
PYTHONPKG=

# Set srcdir if not defined (in case run by hand).
if test "$srcdir" = ""; then
  srcdir="../../../test"
fi

# Handle possible options.
while [ $# != 0 ]
do
  # Special cvs command?
  if [ "$1" = "-stdout" ]; then
    NEEDOUTFIL=1
    shift
  elif [ "$1" = "-nostdout" ]; then
    NEEDOUTFIL=0
    shift
  elif [ "$1" = "-pythonpkg" ]; then
    shift
    PYTHONPKG=$1
    shift
  elif [ "$1" = "-nopythonpkg" ]; then
    PYTHONPKG=
    shift
  else
    break
  fi
done

if test $# -lt 1 || test $# -gt 3; then
  echo "usage: runtest.sh [-stdout] [-pythonpkg package] <testname> [<max run-time>] [<precision>]"
  exit 1
fi

if [ "$2" != "" ]; then
  MAXTIME=$2
else
  MAXTIME=300  # 300 seconds == 5 minutes
fi

if [ "$3" != "" ]; then
  PREC=$3
else
  PREC=1e-5
fi

# Get directory of this script.
lfr_share_dir=`dirname $0`
lfr_share_dir=`cd $lfr_share_dir && pwd`

findvars=`cd .. && $lfr_share_dir/findpkg -l`
LOFAR_BUILDCOMP=`echo $findvars | awk '{print $1}'`
export LOFAR_BUILDCOMP
LOFAR_BUILDVAR=`echo $findvars | awk '{print $2}'`
export LOFAR_BUILDVAR

# Set source directory if not defined.
curwd=`pwd`
if [ "$srcdir" = "" ]; then
  basenm=`basename $curwd`
  srcdir=$lfr_share_dir/../`echo $findvars | awk '{print $5}'`/$basenm
fi

# Make directory writable (needed for make distcheck).
chmod +w .

# Initialize LOFAR (if needed). Use make variable $prefix, if defined;
# otherwise use fallback LOFAR/LCS/Tools/src
# Note that if LOFARROOT is already set, it won't be changed because the
# Tools/src directory does not have a bin directory.
if test -f "$prefix/lofarinit.sh"; then
  lofarinitdir=$prefix
else
  lofarinitdir=`pwd | sed -e 's%\(.*/LOFAR\)/.*%\1/LCS/Tools/src%'`
fi

if test -r "$lofarinitdir/lofarinit.sh"; then
  cd "$lofarinitdir"
  . ./lofarinit.sh
fi
cd $curwd

# Initialize AIPS++.
if test "$AIPSPP" != ""; then
  aipsroot=`dirname $AIPSPP`
  if test -r $aipsroot/aipsinit.sh; then
    . $aipsroot/aipsinit.sh
    # Set correct AIPS++ variant.
    aipsd=`basename $AIPSPP`
    aipsv=`echo $AIPSPATH | awk '{print $2}'`
    AIPSPATH=`echo $AIPSPATH | sed -e "s% $aipsv % $aipsd %"`
  else
    AIPSPATH=`dirname $AIPSPP`
  fi
  export AIPSPATH
fi

# Add the current directory to the path. We don't care if it's already in.
PATH=.:$PATH
export PATH

# Delete all possible files from previous test runs.
\rm -f $1.err $1.valgrind*

#
# Copy expected files to current directory
#
if test -f "$srcdir/$1.in"; then
  \rm -f $1.in
  \cp $srcdir/$1.in  .
fi
\rm -rf $1.in_*
\cp -r $srcdir/$1.in_* . > /dev/null 2>&1
chmod -R +w $1.in_* > /dev/null 2>&1    # Make writable (for make distcheck).
if test -f "$srcdir/$1.stdout"; then
  \rm -f $1.stdout
  \cp $srcdir/$1.stdout .
elif test -f "$srcdir/$1.out"; then
  \rm -f $1.stdout
  \cp $srcdir/$1.out $1.stdout
fi
if test -f "$srcdir/$1.run"; then
  \rm -f $1.run
  \cp $srcdir/$1.run .
fi
if test -f "$srcdir/$1.py"; then
  \rm -f $1.py
  \cp $srcdir/$1.py .
fi
if test -f "$srcdir/$1.parset"; then
  \rm -f $1.parset
  \cp $srcdir/$1.parset .
fi
if test -f "$srcdir/$1.log_prop"; then
  \rm -f $1.log_prop
  \cp $srcdir/$1.log_prop .
else
  if test ! -f "$1.log_prop"; then
    sed -e "s/<LOGFILENAME>/$1_tmp.log/" $lfr_share_dir/default.log_prop > $1.log_prop
  fi
fi

# For python files create the lofar/package directory, so the tests can
# be run as if the python files were installed.
# Note that python looks in . before PYTHONPATH.
if [ "$PYTHONPKG" != "" ]; then
  PYTHONDIR=${1}_tmp_pythonpkg_dir           # will be removed automatically
  mkdir -p $PYTHONDIR/lofar/$PYTHONPKG
  touch $PYTHONDIR/lofar/__init__.py
  rm -f $PYTHONDIR/lofar/$PYTHONPKG/*
  for FIL in $srcdir/../src/*.py
  do
      ln -s $FIL $PYTHONDIR/lofar/$PYTHONPKG
  done
  for FIL in `pwd`/../src/.libs/*.so
  do
      ln -s $FIL $PYTHONDIR/lofar/$PYTHONPKG
  done
  PYTHONPATH=`pwd`/$PYTHONDIR/:$PYTHONPATH
  export PYTHONPATH
  echo "PYTHONPATH=$PYTHONPATH"
fi

# Define source directory and run assay
LOFAR_PKGSRCDIR=$srcdir
export LOFAR_PKGSRCDIR
$lfr_share_dir/assay $1 $MAXTIME $PREC $NEEDOUTFIL
STS=$?

# Cleanup (mainly for make distcheck).
\rm -f $1.stdout $1.run $1.py $1.in $1.parset $1.log_prop
\rm -rf $1.in_*
exit $STS
