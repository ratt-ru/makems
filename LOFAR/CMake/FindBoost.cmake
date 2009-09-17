# $Id: FindBoost.cmake 14027 2009-09-16 12:58:01Z loose $
#
# Copyright (C) 2008-2009
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

# A tiny wrapper around the FindBoost.cmake macro that comes with CMake. 
# Its purpose is twofold:
# - Define all-uppercase variables for the following variables: 
#   Boost_INCLUDE_DIRS, Boost_LIBRARIES, and Boost_FOUND.
# - Set a HAVE_BOOST_<COMPONENT> variable in the cache for each component that
#   was found.

include(${CMAKE_ROOT}/Modules/FindBoost.cmake)

# Define all-uppercase variables for Boost_INCLUDE_DIRS Boost_LIBRARIES and 
# Boost_FOUND.
foreach(_var INCLUDE_DIRS LIBRARIES FOUND)
  if(DEFINED Boost_${_var})
    set(BOOST_${_var} ${Boost_${_var}})
  endif(DEFINED Boost_${_var})
endforeach(_var INCLUDE_DIRS LIBRARIES FOUND)

# Define a HAVE_BOOST_<COMPONENT> variable, for each component that was found.
foreach(_comp ${Boost_FIND_COMPONENTS})
  string(TOUPPER "${_comp}" _comp)
  if(Boost_${_comp}_FOUND)
    set(HAVE_BOOST_${_comp} TRUE CACHE INTERNAL 
      "Set if Boost component ${_comp} was found")
  endif(Boost_${_comp}_FOUND)
endforeach(_comp ${Boost_FIND_COMPONENTS})
