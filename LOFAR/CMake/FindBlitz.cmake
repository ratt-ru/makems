# $Id: FindBLITZ.cmake 13414 2009-06-16 22:15:37Z loose $
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

# Try to find Blitz: a C++ template class library for scientific computing
#
# This will define:
#  
#  BLITZ_FOUND        - system has Blitz
#  BLITZ_INCLUDE_DIR  - the Blitz include directory (cached)
#  BLITZ_INCLUDE_DIRS - the Blitz include directories 
#                       (identical to BLITZ_INCLUDE_DIR)
#  BLITZ_LIBRARY      - the Blitz library (cached)
#  BLITZ_LIBRARIES    - the Blitz libraries
#                       (identical to BLITZ_LIBRARY)

if(NOT BLITZ_FOUND)

  find_path(BLITZ_INCLUDE_DIR blitz/blitz.h)
  find_library(BLITZ_LIBRARY blitz)
  mark_as_advanced(BLITZ_INCLUDE_DIR BLITZ_LIBRARY)
  
  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(Blitz DEFAULT_MSG
    BLITZ_LIBRARY BLITZ_INCLUDE_DIR)

  set(BLITZ_INCLUDE_DIRS ${BLITZ_INCLUDE_DIR})
  set(BLITZ_LIBRARIES ${BLITZ_LIBRARY})

endif(NOT BLITZ_FOUND)
