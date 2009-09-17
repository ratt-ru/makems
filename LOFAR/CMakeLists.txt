#  $Id: CMakeLists.txt 13772 2009-08-14 12:15:05Z loose $
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

cmake_minimum_required(VERSION 2.6)

## ---------------------------------------------------------------------------
## Preamble: set compilers
## ---------------------------------------------------------------------------
include(LofarInit)

## ---------------------------------------------------------------------------
## Project characteristics
## ---------------------------------------------------------------------------
project(LOFAR)
include(LofarGeneral)

## ---------------------------------------------------------------------------
## Subdirectories
## ---------------------------------------------------------------------------
lofar_add_subdirectory(LCS)
lofar_add_subdirectory(CEP)
lofar_add_subdirectory(RTCP)
lofar_add_subdirectory(SAS)
lofar_add_subdirectory(MAC)

## ---------------------------------------------------------------------------
## Generate configuration header file.
## ---------------------------------------------------------------------------
include(LofarConfig)

get_directory_property(def DEFINITIONS)
message(STATUS "COMPILE_DEFINITIONS = ${def}")

if(0)
  get_cmake_property(cache_vars CACHE_VARIABLES)
  message(STATUS "vvvvvvvv Cache variables vvvvvvvv")
  foreach(var ${cache_vars})
    message(STATUS "[cached] ${var} = ${${var}}")
  endforeach(var ${cache_vars})
  message(STATUS "^^^^^^^^ Cache variables ^^^^^^^^")
  message(STATUS "vvvvvvvv Variables vvvvvvvv")
  include(AllVariables)
  all_variables()
  message(STATUS "^^^^^^^^ Variables ^^^^^^^^")
endif(0)
