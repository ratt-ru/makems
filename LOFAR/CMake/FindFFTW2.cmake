# $Id: FindFFTW2.cmake 13758 2009-08-13 13:48:33Z loose $
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

# Try to find FFTW.
#
# Usage: find_package(FFTW [COMPONENTS [real] [double|single] [mpi|threads]])
#
# This will define:
#  
#  FFTW2_FOUND           - FFTW is present for the requested components.
#  FFTW2_INCLUDE_DIR     - FFTW include directory for the requested components
#                         (cached)
#  FFTW2_INCLUDE_DIRS    - FFTW include directory 
#                         (identical to FFTW2_INCLUDE_DIR)
#  FFTW2_<YYY>_LIBRARY   - FFTW library for the requested component <YYY>
#                         (cached)
#  FFTW2_LIBRARIES       - FFTW libraries needed to when linking
#
#  FFTW2_PRECISION       - FFTW precision specified (double|single)
#                         (cached internal)
#  FFTW2_PARALLELIZATION - FFTW parallelization specified(off|mpi|threads)
#                         (cached internal)

set(_usage_msg 
  "Usage: find_package(FFTW [COMPONENTS [real] [double|single] [mpi|threads]]")

# -----------------------------------------------------------------------------
# Get the optional `type' component: [real]. 
# The output will be stored in the variable _fftw_type.
# -----------------------------------------------------------------------------
macro(get_fftw_type)
  set(_fftw_type)
  list(FIND FFTW2_FIND_COMPONENTS real _idx)
  if(_idx GREATER -1)
    set(_fftw_type real)
  endif(_idx GREATER -1)
endmacro(get_fftw_type)

# -----------------------------------------------------------------------------
# Get the optional component ${_kind}. Sensible values for ${_kind} are (at
# the moment) precision, or parallelization.
# The variable _options will contain the list of valid components for
# ${_kind}; the first element of _options is used as default value, in case no
# matching component could be found.
# The output will be stored in the variable _fftw_${_kind}.
#
# It is an error if, between multiple calls of FindFFTW(), the currently
# specified component value is different from the cached one.
#
# Usage: get_fftw_component(<kind> <default-option> [option] ...)
# -----------------------------------------------------------------------------
macro(get_fftw_component _kind)
  set(_options ${ARGN})
  if(_options)
    list(GET _options 0 _default_option)
  endif(_options)
  set(_fftw_${_kind})
  foreach(_opt ${_options})
    list(FIND FFTW2_FIND_COMPONENTS ${_opt} _idx)
    if(_idx GREATER -1)
      if(NOT _fftw_${_kind})
        set(_fftw_${_kind} ${_opt})
      else(NOT _fftw_${_kind})
        message(FATAL_ERROR
          "FindFFTW: more than one `${_kind}' component was specified.\n"
          "${_usage_msg}")
      endif(NOT _fftw_${_kind})
    endif(_idx GREATER -1)
  endforeach(_opt ${_options})
  if(NOT _fftw_${_kind})
    set(_fftw_${_kind} "${_default_option}")
  endif(NOT _fftw_${_kind})
  string(TOUPPER "FFTW2_${_kind}" _cached_option)
  if(DEFINED ${_cached_option})
    if(NOT "${_fftw_${_kind}}" STREQUAL "${${_cached_option}}")
      message(FATAL_ERROR
        "FindFFTW: previous call used ${_kind} `${${_cached_option}}', "
        "which is different from `${_fftw_${_kind}}'. This is not supported!")
    endif(NOT "${_fftw_${_kind}}" STREQUAL "${${_cached_option}}")
  endif(DEFINED ${_cached_option})
endmacro(get_fftw_component _kind)

# -----------------------------------------------------------------------------

# Get FFTW type (optional: [real])
get_fftw_type()

# Get FFTW precision (optional: [double|single])
get_fftw_component(precision double single)

# Get FFTW parallelization (optional: [off|mpi|threads])
get_fftw_component(parallelization off mpi threads)

# This is the default: complex transforms
set(_libraries fftw)
set(_headerfile fftw.h)

# The real transforms also require rfftw.h and -lrfftw; i.e. prefixed with 'r'.
if(_fftw_type)
  set(_headerfile rfftw.h)
  set(_libraries rfftw ${_libraries})
endif(_fftw_type)

# Parallelization using mpi or threads also requires fftw_mpi.h and lfftw_mpi
# or fftw_threads.h and -lfftw_threads.
if(_fftw_parallelization)
  set(_headerfile fftw_${_fftw_parallelization}.h)
  set(_libraries fftw_${_fftw_parallelization} ${_libraries})
  # And again, if using real transforms, also add the real transform headers
  # and libraries.
  if(_fftw_type)
    set(_headerfile rfftw_${_fftw_parallelization}.h)
    set(_libraries rfftw_${_fftw_parallelization} ${_libraries})
  endif(_fftw_type)
endif(_fftw_parallelization)

# Some distributions choose to prefix the header files and libraries with
# the precision: (s)ingle or (d)ouble.
string(SUBSTRING ${_fftw_precision} 0 1 _prec)

# Keep a list of variable names that we need to pass on to
# find_package_handle_standard_args().
set(_check_list)

# Search for all required libraries.
set(FFTW2_LIBRARIES)
foreach(_lib ${_libraries})
  string(TOUPPER "${_lib}2" _LIB)
  find_library(${_LIB}_LIBRARY NAMES ${_lib} ${_prec}${_lib})
  mark_as_advanced(${_LIB}_LIBRARY)
  list(APPEND FFTW2_LIBRARIES ${${_LIB}_LIBRARY})
  list(APPEND _check_list ${_LIB}_LIBRARY)
endforeach(_lib ${_libraries})

# Search for header file, with and without prefix.
find_path(FFTW2_INCLUDE_DIR NAMES ${_prec}${_headerfile} ${_headerfile})
mark_as_advanced(FFTW2_INCLUDE_DIR)
list(APPEND _check_list FFTW2_INCLUDE_DIR)

# Handle the QUIETLY and REQUIRED arguments and set FFTW2_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(FFTW2 DEFAULT_MSG ${_check_list})

# If everything was found, put the following variables in the cache. We need
# them to check if a second call to FindFFTW has conflicting options.
if(FFTW2_FOUND)
  set(FFTW2_PRECISION ${_fftw_precision} CACHE INTERNAL
    "FFTW2 precision")
  set(FFTW2_PARALLELIZATION ${_fftw_parallelization} CACHE INTERNAL
    "FFTW2 parallelization")
endif(FFTW2_FOUND)
