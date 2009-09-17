#  $Id: CMakeLists.txt 13813 2009-08-20 09:49:38Z loose $
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

set(common_LIB_SRCS
  Package__Version.cc
  Allocator.cc
  DataConvert.cc
  Exception.cc
  FileLocator.cc
  hexdump.cc
  KVpair.cc
  lofar_bitset.cc
  LofarLogCout.cc
  LofarLogger.cc
  Numeric.cc
  ParameterSet.cc
  ParameterSetImpl.cc
  ParameterValue.cc
  PrettyUnits.cc
  Process.cc
  ReadLine.cc
  RunOnNode.cc
  Stopwatch.cc
  StreamUtil.cc
  StringUtil.cc
  SystemUtil.cc
  Timer.cc
  TypeNames.cc
  Version.cc
  Net/FdSet.cc
  Net/Socket.cc)

set(common_PROGRAMS
  getparsetvalue
  versioncommon
)

if(HAVE_BACKTRACE)
  list(APPEND common_LIB_SRCS 
    AddressTranslator.cc 
    Backtrace.cc
    SymbolTable.cc)
endif(HAVE_BACKTRACE)

if(HAVE_SHMEM)
  set(shmem_LIB_SRCS
    shmem/segbasemap.cc
    shmem/dlmalloc.c
    shmem/shmem_alloc.cc)
  set(shmem_COMPILE_FLAGS
    -DUSE_PUBLIC_MALLOC_WRAPPERS
    -DUSE_DL_PREFIX
    -DHAVE_MMAP=0
    -DMORECORE=shmbrk
    -DMORECORE_CONTIGUOUS=0
    -DMORECORE_CANNOT_TRIM=1
    -DSHMEM_ALLOC
    -DHAVE_USR_INCLUDE_MALLOC_H)
  join_arguments(shmem_COMPILE_FLAGS)
  set_source_files_properties(${shmem_LIB_SRCS} 
    PROPERTIES COMPILE_FLAGS ${shmem_COMPILE_FLAGS})
  list(APPEND common_LIB_SRCS ${shmem_LIB_SRCS})
endif(HAVE_SHMEM)

lofar_add_library(common ${common_LIB_SRCS})

foreach(prog ${common_PROGRAMS})
  lofar_add_bin_program(${prog} ${prog}.cc)
endforeach(prog ${common_PROGRAMS})
