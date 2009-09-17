#  $Id: LofarVariants.cmake 13414 2009-06-16 22:15:37Z loose $
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

#  Include a host-specific 'variants.<hostname>' file, if present; and a
#  global 'variants' file. These files must be located in the directory
#  ${LOFAR_ROOT}/CMake/variants.

include(LofarMacros)

if(NOT DEFINED LOFAR_VARIANTS_INCLUDED)

  set(LOFAR_VARIANTS_INCLUDED TRUE)
  
  ## --------------------------------------------------------------------------
  ## First, include host-specific variants file, if present
  ## --------------------------------------------------------------------------
  lofar_get_hostname(hostname)
  set(variants_file ${CMAKE_MODULE_PATH}/variants/variants.${hostname})
  
  if (EXISTS ${variants_file})
    message(STATUS "Loading host-specific variants file")
    include(${variants_file})
  endif (EXISTS ${variants_file})
  
  ## --------------------------------------------------------------------------
  ## Next, include global variants file.
  ## --------------------------------------------------------------------------
  message(STATUS "Loading global variants file")
  include(${CMAKE_MODULE_PATH}/variants/variants)
  
endif(NOT DEFINED LOFAR_VARIANTS_INCLUDED)
