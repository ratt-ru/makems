#  $Id: CMakeLists.txt 13474 2009-06-24 14:33:14Z loose $
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

include(LofarPackageVersion)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w -Wno-deprecated -Wno-write-strings")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w -Wno-deprecated -Wno-write-strings")

set(ms_LIB_SRCS
#  Package__Version.cc
  MSCreate.cc
#  VdsMaker.cc
)

set(ms_PROGRAMS
#  versionms
#  MSSplit
#  makevds
#  combinevds
  makems
#  msplay
)

lofar_add_library(ms ${ms_LIB_SRCS})

foreach(prog ${ms_PROGRAMS})
  lofar_add_bin_program(${prog} ${prog}.cc)
  target_link_libraries(${prog} -ldl)
endforeach(prog ${ms_PROGRAMS})

#install(PROGRAMS
#  mssplit
#  makemsdistr
#  makemsdistr-part
#  DESTINATION bin)
