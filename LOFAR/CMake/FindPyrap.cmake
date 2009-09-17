# $Id: FindPyrap.cmake 14027 2009-09-16 12:58:01Z loose $
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

# Try to find Pyrap.
#
# This will define:
#  
#  PYRAP_FOUND        - system has Pyrap
#  PYRAP_INCLUDE_DIR  - the Pyrap include directory (cached)
#  PYRAP_INCLUDE_DIRS - the Pyrap include directories
#                       (identical to PYRAP_INCLUDE_DIR)
#  PYRAP_LIBRARY      - the Pyrap library (cached)
#  PYRAP_LIBRARIES    - the Pyrap libraries
#                       (identical to PYRAP_LIBRARY)

if(NOT PYRAP_FOUND)

  find_path(PYRAP_INCLUDE_DIR pyrap/Converters.h)
  find_library(PYRAP_LIBRARY pyrap)
  mark_as_advanced(PYRAP_INCLUDE_DIR PYRAP_LIBRARY)

  # Pyrap also depends on Casacore
  find_package(Casacore REQUIRED COMPONENTS casa)

  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(Pyrap DEFAULT_MSG 
    PYRAP_LIBRARY PYRAP_INCLUDE_DIR CASACORE_INCLUDE_DIR)

  set(PYRAP_INCLUDE_DIRS ${PYRAP_INCLUDE_DIR} ${CASACORE_INCLUDE_DIR})
  set(PYRAP_LIBRARIES ${PYRAP_LIBRARY})

endif(NOT PYRAP_FOUND)
