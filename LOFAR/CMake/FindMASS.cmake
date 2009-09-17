# $Id: FindMASS.cmake 13640 2009-07-10 15:01:03Z loose $
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

# Try to find MASS, the IBM BG/P Mathematical Acceleration Subsystem
#
# This macro will define:
#  
#  MASS_FOUND        - system has MASS
#  MASS_INCLUDE_DIR  - the MASS include directory (cached)
#  MASS_INCLUDE_DIRS - the MASS include directories
#                      (identical to MASS_INCLUDE_DIR)
#  MASS_LIBRARY      - the scalar math library (cached)
#  MASSV_LIBRARY     - the vector math library (cached)
#  MASS_LIBRARIES    - all MASS libraries
#
# CAUTION: It is not guaranteed that the version of MASS found by this
#  macro can be used on the BG/P, because we need to do cross-compile.
#

if(NOT MASS_FOUND)

  find_path(MASS_INCLUDE_DIR massv.h)
  find_library(MASS_LIBRARY mass)
  find_library(MASSV_LIBRARY massv)
  mark_as_advanced(MASS_INCLUDE_DIR MASS_LIBRARY MASSV_LIBRARY)

  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(MASS DEFAULT_MSG
    MASS_LIBRARY MASSV_LIBRARY MASS_INCLUDE_DIR)

  set(MASS_INCLUDE_DIRS ${MASS_INCLUDE_DIR})
  set(MASS_LIBRARIES ${MASS_LIBRARY} ${MASSV_LIBRARY})

endif(NOT MASS_FOUND)
