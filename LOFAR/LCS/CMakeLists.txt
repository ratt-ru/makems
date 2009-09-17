#  $Id: CMakeLists.txt 14027 2009-09-16 12:58:01Z loose $
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
project(LCS)
include(LofarGeneral)

## ---------------------------------------------------------------------------
## Subdirectories
## ---------------------------------------------------------------------------
lofar_add_subdirectory(Tools)          # Useful tools
lofar_add_subdirectory(Common)         # Common stuff
lofar_add_subdirectory(Blob)           # Binary Large Objects
lofar_add_subdirectory(Stream)         # Low-level support for streaming data
lofar_add_subdirectory(Transport)      # Low-level transport library
lofar_add_subdirectory(ACC)            # Application Configuration & Control
lofar_add_subdirectory(AMC)            # Astronomical Measures Conversions
lofar_add_subdirectory(ApplCommon)     # Application common stuff

lofar_add_subdirectory(pytools)        # Python tools
lofar_add_subdirectory(pyparameterset) # Python ParameterSet bindings

## ---------------------------------------------------------------------------
## Generate configuration header file.
## ---------------------------------------------------------------------------
include(LofarConfig)
