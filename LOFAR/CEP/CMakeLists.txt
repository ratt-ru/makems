#  $Id: CMakeLists.txt 14028 2009-09-17 07:54:50Z loose $
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
project(CEP)
include(LofarGeneral)

## ---------------------------------------------------------------------------
## Subdirectories
## ---------------------------------------------------------------------------
lofar_add_subdirectory(MWCommon)
lofar_add_subdirectory(MS)
lofar_add_subdirectory(ParmDB)
lofar_add_subdirectory(Calibration)
lofar_add_subdirectory(DP3)
lofar_add_subdirectory(Imager)
lofar_add_subdirectory(Pipeline)
lofar_add_subdirectory(pyparmdb)

## ---------------------------------------------------------------------------
## Generate configuration header file.
## ---------------------------------------------------------------------------
include(LofarConfig)
