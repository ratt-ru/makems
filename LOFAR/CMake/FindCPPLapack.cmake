# $Id: FindCPPLapack.cmake 13415 2009-06-17 12:51:37Z loose $
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

# Try to find CPPLapack, a C++ class wrapper for BLAS and LAPACK.
#
# This will define:
#  
#  CPPLAPACK_FOUND        - system has CPPLapack
#  CPPLAPACK_INCLUDE_DIR  - the CPPLapack include directory (cached)
#  CPPLAPACK_INCLUDE_DIRS - the CPPLapack include directories
#                           (identical to CPPLAPACK_INCLUDE_DIR)
#  CPPLAPACK_LIBRARIES    - libraries that CPPLapack depends upon

if(NOT CPPLAPACK_FOUND)

  find_path(CPPLAPACK_INCLUDE_DIR cpplapack.h)
  find_library(LAPACK_LIBRARY lapack)
  find_library(BLAS_LIBRARY blas)
  mark_as_advanced(CPPLAPACK_INCLUDE_DIR LAPACK_LIBRARY BLAS_LIBRARY)

  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(CPPLapack DEFAULT_MSG
    CPPLAPACK_INCLUDE_DIR LAPACK_LIBRARY BLAS_LIBRARY)

  set(CPPLAPACK_INCLUDE_DIRS ${CPPLAPACK_INCLUDE_DIR})
  set(CPPLAPACK_LIBRARIES ${LAPACK_LIBRARY} ${BLAS_LIBRARY})

endif(NOT CPPLAPACK_FOUND)
