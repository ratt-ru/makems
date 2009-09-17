# $Id: FindPQXX.cmake 13622 2009-07-09 09:06:10Z loose $
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

# Try to find libpqxx: the official C++ client API for PostgreSQL
#
# This will define:
#  
#  PQXX_FOUND        - system has PQXX
#  PQXX_INCLUDE_DIR  - the PQXX include directory (cached)
#  PQXX_INCLUDE_DIRS - the PQXX include directories 
#                      (identical to PQXX_INCLUDE_DIR)
#  PQXX_LIBRARY      - the PQXX library (cached)
#  PQXX_LIBRARIES    - the PQXX libraries
#                      (identical to PQXX_LIBRARY)

if(NOT PQXX_FOUND)

  find_path(PQXX_INCLUDE_DIR pqxx/pqxx)
  find_library(PQXX_LIBRARY pqxx)
  find_library(PQ_LIBRARY pq)
  mark_as_advanced(PQXX_INCLUDE_DIR PQXX_LIBRARY PQ_LIBRARY)
  
  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(PQXX DEFAULT_MSG
    PQXX_LIBRARY PQ_LIBRARY PQXX_INCLUDE_DIR)

  set(PQXX_INCLUDE_DIRS ${PQXX_INCLUDE_DIR})
  set(PQXX_LIBRARIES ${PQXX_LIBRARY} ${PQ_LIBRARY})

endif(NOT PQXX_FOUND)
