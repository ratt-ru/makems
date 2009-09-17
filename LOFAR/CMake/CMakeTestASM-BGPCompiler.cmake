#  $Id: CMakeTestASM-BGPCompiler.cmake 13414 2009-06-16 22:15:37Z loose $
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
## Test whether the assembler works. This is not really possible. Instead it
## just tests whether the assembler has been found, i.e. whether the variable
## CMAKE_ASM-BGP_COMPILER has been set.
##
## See http://www.vtk.org/Wiki/CMake/Assembler for details
## ---------------------------------------------------------------------------
set(ASM_DIALECT "-BGP")
include(CMakeTestASMCompiler)
set(ASM_DIALECT)
