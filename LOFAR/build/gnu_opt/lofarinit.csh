#!/bin/echo Usage: source
#-----------------------------------------------------------------------------
# lofarinit.csh: Define the LOFAR environment for C-like shells
#-----------------------------------------------------------------------------
#
# Copyright (C) 2005
# ASTRON (Netherlands Foundation for Research in Astronomy)
# P.O.Box 2, 7990 AA Dwingeloo, The Netherlands, seg@astron.nl
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
# $Id: lofarinit.csh.in 13415 2009-06-17 12:51:37Z loose $

# Define root here.
# This is a placeholder, so do NOT change the line.
# The actual value is filled in by make install (see Makefile.am).
set a_root=/home/oms/MakeMS/LOFAR/installed/gnu_opt        # lfr_root=/home/oms/MakeMS/LOFAR/installed/gnu_opt

# Get python version.
set a_pyv=`python -V |& cat --`
set a_pyvv=`echo $a_pyv | sed -e "s/Python \([0-9]\.[0-9]\).*/\1/"`
# Fall-back: if python cannot be found assume Python 2.4
if ("$a_pyv" == "$a_pyvv") then
  set a_pyvv=2.4
endif

# Only modify path variables if $a_root is an existing directory.
if (! (-d $a_root) ) then
    echo "LOFAR root directory $a_root does not exist"
else
    # First strip the current LOFARROOT from PATH, LD_LIBRARY_PATH, PYTHONPATH.
    # Take care that a possible . is preceeded by a backslash.
    if ($?LOFARROOT) then
        set a_path = `echo $LOFARROOT | sed -e 's/\./\\\./g'`
	if ($?PATH) then
	    foreach bintp (bin sbin)
		set a_bin = "$a_path/$bintp"
		setenv PATH `echo $PATH | sed -e "s%:${a_bin}:%:%g" -e "s%^${a_bin}:%%"  -e "s%:${a_bin}"'$%%' -e "s%^${a_bin}"'$%%'`
	    end
        endif
        if ($?LD_LIBRARY_PATH) then
            set a_lib = "$a_path/lib"
            setenv LD_LIBRARY_PATH `echo $LD_LIBRARY_PATH | sed -e "s%:${a_lib}:%:%g" -e "s%^${a_lib}:%%"  -e "s%:${a_lib}"'$%%' -e "s%^${a_lib}"'$%%'`
        endif
        if ($?PYTHONPATH) then
            set a_pyt = "$a_path/lib/python$a_pyvv/site-packages"
            setenv PYTHONPATH `echo $PYTHONPATH | sed -e "s%:${a_pyt}:%:%g" -e "s%^${a_pyt}:%%"  -e "s%:${a_pyt}"'$%%' -e "s%^${a_pyt}"'$%%'`
        endif
    endif

    # Now define the new LOFARROOT
    setenv LOFARROOT `cd >/dev/null $a_root; pwd`      # make path absolute

    # Also strip this path from the current paths (in case it is contained in
    # it).
    set a_path = `echo $LOFARROOT | sed -e 's/\./\\\./g'`
    if ($?PATH) then
	foreach bintp (bin sbin)
	    set a_bin = "$a_path/$bintp"
            setenv PATH `echo $PATH | sed -e "s%:${a_bin}:%:%g" -e "s%^${a_bin}:%%"  -e "s%:${a_bin}"'$%%' -e "s%^${a_bin}"'$%%'`
	end
    endif
    if ($?LD_LIBRARY_PATH) then
        set a_lib = "$a_path/lib"
        setenv LD_LIBRARY_PATH `echo $LD_LIBRARY_PATH | sed -e "s%:${a_lib}:%:%g" -e "s%^${a_lib}:%%"  -e "s%:${a_lib}"'$%%' -e "s%^${a_lib}"'$%%'`
    endif
    if ($?PYTHONPATH) then
        set a_pyt = "$a_path/lib/python$a_pyvv/site-packages"
        setenv PYTHONPATH `echo $PYTHONPATH | sed -e "s%:${a_pyt}:%:%g" -e "s%^${a_pyt}:%%"  -e "s%:${a_pyt}"'$%%' -e "s%^${a_pyt}"'$%%'`
    endif

    # Add the path to the standard paths.
    if (! $?PATH) then
        setenv PATH $LOFARROOT/bin:$LOFARROOT/sbin
    else
        setenv PATH $LOFARROOT/bin:$LOFARROOT/sbin:$PATH
    endif
    if (! $?LD_LIBRARY_PATH) then
        setenv LD_LIBRARY_PATH $LOFARROOT/lib
    else
        setenv LD_LIBRARY_PATH $LOFARROOT/lib:$LD_LIBRARY_PATH
    endif
    set a_pyt = "$LOFARROOT/lib/python$a_pyvv/site-packages"
    if (! $?PYTHONPATH) then
        setenv PYTHONPATH $a_pyt
    else
        setenv PYTHONPATH ${a_pyt}:$PYTHONPATH
    endif

    # Now define the new LOFARDATAROOT (if possible).
    # First try as data directory of the LOFAR install directory.
    set data_path = `echo $LOFARROOT | sed -e 's%/installed.*%%'`
    if ( "$data_path" != ""  &&  -d $data_path/data ) then
        setenv LOFARDATAROOT $data_path/data
    else
        # Try it as the LOFARDATA directory (part of the source tree).
        set data_path = `echo $LOFARROOT | sed -e 's%/LOFAR/.*%/LOFAR%'`
        if ( "$data_path" != ""  &&  -d ${data_path}DATA ) then
            setenv LOFARDATAROOT ${data_path}DATA
        else
            setenv LOFARDATAROOT /opt/lofar/data
        endif
    endif

    # Create a .glishrc.post in the HOME directory to append
    # the LOFAR libexec to the glish path.
    if ( -f $HOME/.glishrc.post ) then
        mv $HOME/.glishrc.post $HOME/.glishrc.post-old
    endif
    echo "system.path.include := [system.path.include, '$LOFARROOT/libexec/glish']" > $HOME/.glishrc.post

endif
