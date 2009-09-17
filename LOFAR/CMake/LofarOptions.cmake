#  LofarOptions.cmake: Parse CMake options and set associated variables
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
#  $Id: LofarOptions.cmake 13985 2009-09-11 08:05:35Z loose $

if(NOT DEFINED LOFAR_OPTIONS_INCLUDED)

  set(LOFAR_OPTIONS_INCLUDED TRUE)

  ## --------------------------------------------------------------------------
  ## Include wrapper macro for find_package()
  ## --------------------------------------------------------------------------
  include(LofarFindPackage)

  ## --------------------------------------------------------------------------
  ## Handle contradicting options
  ## --------------------------------------------------------------------------
  if(USE_LOG4CXX AND USE_LOG4CPLUS)
    message(FATAL_ERROR 
      "You cannot use more than one logger implementation. "
      "Please check your variants file!")
  endif(USE_LOG4CXX AND USE_LOG4CPLUS)

  if(BUILD_STATIC_EXECUTABLES AND BUILD_SHARED_LIBS)
    message(FATAL_ERROR 
      "Cannot create static executables, when creating shared libraries. "
      "Please check your variants file!")
  endif(BUILD_STATIC_EXECUTABLES AND BUILD_SHARED_LIBS)

  ## --------------------------------------------------------------------------
  ## Handle each option
  ## --------------------------------------------------------------------------
  if(BUILD_STATIC_EXECUTABLES)
    set(CMAKE_EXE_LINKER_FLAGS -static)
    set(CMAKE_FIND_LIBRARY_SUFFIXES .a)
    set(CMAKE_EXE_LINK_DYNAMIC_C_FLAGS)       # remove -Wl,-Bdynamic
    set(CMAKE_EXE_LINK_DYNAMIC_CXX_FLAGS)
    set(CMAKE_SHARED_LIBRARY_C_FLAGS)         # remove -fPIC
    set(CMAKE_SHARED_LIBRARY_CXX_FLAGS)
    set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)    # remove -rdynamic
    set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS)
    # Maybe this works as well, haven't tried yet.
    # set_property(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS FALSE)
  endif(BUILD_STATIC_EXECUTABLES)
  
#  if(USE_AIPSPP)
#    lofar_find_package(CasaCore)
#  endif(USE_AIPSPP)
  
  if(USE_BACKTRACE)
    lofar_find_package(Backtrace REQUIRED)
  endif(USE_BACKTRACE)

  if(USE_LOG4CXX)
    lofar_find_package(Log4Cxx REQUIRED)
  endif(USE_LOG4CXX)

  if(USE_LOG4CPLUS)
    lofar_find_package(Log4Cplus REQUIRED)
  endif(USE_LOG4CPLUS)

  if(USE_MPI)
    lofar_find_package(MPI REQUIRED)
  endif(USE_MPI)
  
  if(USE_PYTHON)
#    lofar_find_package(Python)
  endif(USE_PYTHON)

  if(USE_SSE)
    set(GNU_SSE_FLAGS "-msse2")      # DOES NOT YET WORK !
    set(ICC_SSE_FLAGS "-xW")         # DOES NOT YET WORK !
  endif(USE_SSE)

  if(USE_SHMEM)
    set(HAVE_SHMEM 1)
  endif(USE_SHMEM)

  if(USE_SOCKETS)
    #
  endif(USE_SOCKETS)

  if(USE_THREADS)
    lofar_find_package(Pthreads REQUIRED)
    if(CMAKE_COMPILER_IS_GNUCC)
      set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")
    endif(CMAKE_COMPILER_IS_GNUCC)
    if(CMAKE_COMPILER_IS_GNUCXX)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
    endif(CMAKE_COMPILER_IS_GNUCXX)
  endif(USE_THREADS)

  if(USE_ZOID)
    #
  endif(USE_ZOID)

endif(NOT DEFINED LOFAR_OPTIONS_INCLUDED)
