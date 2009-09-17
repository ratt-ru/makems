#  LofarFindPackage.cmake: 
#
#  Copyright (C) 2008-2009
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
#  $Id: LofarFindPackage.cmake 13984 2009-09-11 08:02:57Z loose $

include(LofarSearchPath)
include(FindPackageHandleStandardArgs)

# ----------------------------------------------------------------------------
# function lofar_find_package(package)
#
# Find a package like find_package() does.
# It uses the LOFAR search path (defined in the variants file), preprended
# with <PKG>_PREFIX_PATH (if defined) as CMAKE_PREFIX_PATH.
#
# Furthermore:
# - Add preprocessor definitions that are defined in <PKG>_DEFINITIONS.
# - Add include directories that are defined <PKG>_INCLUDE_DIRS.
# - Add <PKG>_LIBRARIES to the list of LOFAR_EXTRA_LIBRARIES, needed for
#   linking.
# - Add cache variable HAVE_<PKG>, which indicates whether the package was 
#   found. It can be used with #cmakedefine.
# Note: <PKG> equals <package> in uppercase.
# ----------------------------------------------------------------------------
function(lofar_find_package _package)

  string(TOLOWER ${_package} _pkg)
  string(TOUPPER ${_package} _PKG)

  if(NOT ${_PKG}_FOUND)

    # Set CMAKE_PREFIX_PATH; used by the find_xxx() commands for searching.
    lofar_search_path(_prefix_path ${_pkg})
    set(CMAKE_PREFIX_PATH ${${_PKG}_PREFIX_PATH} ${_prefix_path})

    # If package has been disabled explicitly, but is required, raise an
    # error.
    if(DEFINED USE_${_PKG} AND NOT USE_${_PKG})
      list(FIND ARGN REQUIRED is_required)
      if(is_required GREATER -1)
        message(SEND_ERROR 
          "Package ${_package} is required, but has been disabled explicitly!")
      endif(is_required GREATER -1)
    endif(DEFINED USE_${_PKG} AND NOT USE_${_PKG})

    # Use the Find${_package}.cmake module.
    if(LOFAR_VERBOSE_CONFIGURE)
      find_package(${ARGV})
    else(LOFAR_VERBOSE_CONFIGURE)
      find_package(${ARGV} QUIET)
    endif(LOFAR_VERBOSE_CONFIGURE)

    # Add include directories and libraries, if package was found;
    # set HAVE_<PACKAGE> variable in the cache.
    if(${_PKG}_FOUND)
      if(NOT DEFINED HAVE_${_PKG})
        set(HAVE_${_PKG} TRUE CACHE INTERNAL "Have ${_package}?")
      endif(NOT DEFINED HAVE_${_PKG})
      add_definitions(${${_PKG}_DEFINITIONS})
      include_directories(${${_PKG}_INCLUDE_DIRS})
      set(LOFAR_EXTRA_LIBRARIES ${LOFAR_EXTRA_LIBRARIES} ${${_PKG}_LIBRARIES}
        PARENT_SCOPE)
    else(${_PKG}_FOUND)
      set(HAVE_${_PKG} FALSE CACHE INTERNAL "Have ${_package}?")
    endif(${_PKG}_FOUND)
    set(${_PKG}_FOUND ${${_PKG}_FOUND} PARENT_SCOPE)

  endif(NOT ${_PKG}_FOUND)

endfunction(lofar_find_package _package)
