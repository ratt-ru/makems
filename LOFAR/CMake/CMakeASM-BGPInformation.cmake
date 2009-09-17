#  $Id: CMakeASM-BGPInformation.cmake 13661 2009-07-14 12:36:05Z loose $
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

## ---------------------------------------------------------------------------
## Set all the assembler specific variables, like filename suffixes, compile
## rules, etc. If necessary you can override the assembler specific variables
## by setting them before including the generic file.
##
## See http://www.vtk.org/Wiki/CMake/Assembler for details.
## ---------------------------------------------------------------------------
set(ASM_DIALECT "-BGP")
set(CMAKE_ASM${ASM_DIALECT}_SOURCE_FILE_EXTENSIONS S)
set(CMAKE_ASM${ASM_DIALECT}_COMPILE_OBJECT "<CMAKE_ASM${ASM_DIALECT}_COMPILER> <DEFINES> <FLAGS> -c -o <OBJECT> <SOURCE>")
set(CMAKE_ASM${ASM_DIALECT}_LINK_EXECUTABLE "<CMAKE_ASM${ASM_DIALECT}_COMPILER> <FLAGS> <LINK_FLAGS> <OBJECTS>  -o <TARGET> <LINK_LIBRARIES>")
include(CMakeASMInformation)
set(ASM_DIALECT)
