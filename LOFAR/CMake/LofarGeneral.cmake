#  LofarGeneral.cmake: 
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
#
#  $Id: LofarGeneral.cmake 13603 2009-07-07 12:45:52Z loose $

if(NOT DEFINED LOFAR_GENERAL_INCLUDED)

  set(LOFAR_GENERAL_INCLUDED TRUE)

  ## --------------------------------------------------------------------------
  ## Handle all LOFAR build configuration options
  ## --------------------------------------------------------------------------
  include(LofarOptions)

  ## --------------------------------------------------------------------------
  ## Configure for testing with CTest/Dart
  ## --------------------------------------------------------------------------
  include(CTest)
  
  ## --------------------------------------------------------------------------
  ## Add include directory in the binary directory to the -I path.
  ## --------------------------------------------------------------------------
  include_directories(${CMAKE_BINARY_DIR}/include)

  ## --------------------------------------------------------------------------
  ## Check for typedefs of primitive types
  ## --------------------------------------------------------------------------
  include(CheckTypeSize)
  check_type_size("ushort"    HAVE_USHORT   )
  check_type_size("uint"      HAVE_UINT     )
  check_type_size("ulong"     HAVE_ULONG    )
  check_type_size("long long" HAVE_LONG_LONG)

  ## --------------------------------------------------------------------------
  ## Check for several system header files
  ## --------------------------------------------------------------------------
  include(CheckIncludeFile)
  check_include_file(dlfcn.h        HAVE_DLFCN_H)
  check_include_file(inttypes.h     HAVE_INTTYPES_H)
  check_include_file(memory.h       HAVE_MEMORY_H)
  check_include_file(net/ethernet.h HAVE_NET_ETHERNET_H)
  check_include_file(netinet/in.h   HAVE_NETINET_IN_H)
  check_include_file(signal.h       HAVE_SIGNAL_H)
  check_include_file(stdint.h       HAVE_STDINT_H)
  check_include_file(stdlib.h       HAVE_STDLIB_H)
  check_include_file(string.h       HAVE_STRING_H)
  check_include_file(strings.h      HAVE_STRINGS_H)
  check_include_file(sys/mman.h     HAVE_SYS_MMAN_H)
  check_include_file(sys/resource.h HAVE_SYS_RESOURCE_H)
  check_include_file(sys/stat.h     HAVE_SYS_STAT_H)
  check_include_file(sys/time.h     HAVE_SYS_TIME_H)
  check_include_file(sys/timepps.h  HAVE_SYS_TIMEPPS_H)
  check_include_file(sys/types.h    HAVE_SYS_TYPES_H)
  check_include_file(unistd.h       HAVE_UNISTD_H)
  
  ## --------------------------------------------------------------------------
  ## Check endianess
  ## --------------------------------------------------------------------------
  include(TestBigEndian)
  test_big_endian(WORDS_BIGENDIAN)

  ## --------------------------------------------------------------------------
  ## Define `AUTO_FUNCTION_NAME' as either __PRETTY_FUNCTION__, __FUNCTION__,
  ## or "<unknown>", depending on compiler support for function name macro.
  ## --------------------------------------------------------------------------
  include(CheckCSourceCompiles)
  foreach(func_name __PRETTY_FUNCTION__ __FUNCTION__ "\"<unkown>\"")
    check_c_source_compiles("
    #include <stdio.h> 
    int main() { puts(${func_name}); }
    " HAVE_${func_name})
    if(HAVE_${func_name})
      set(AUTO_FUNCTION_NAME ${func_name} CACHE INTERNAL 
        "Define as __PRETTY_FUNCTION__, __FUNCTION__, or \"<unknown>\"")
      break()
    endif(HAVE_${func_name})
  endforeach(func_name)

  ## --------------------------------------------------------------------------
  ## Locate the Doxygen documentation tool
  ## --------------------------------------------------------------------------
  find_package(Doxygen)

endif(NOT DEFINED LOFAR_GENERAL_INCLUDED)
