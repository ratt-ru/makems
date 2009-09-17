# $Id: FindPVSS.cmake 13622 2009-07-09 09:06:10Z loose $
#
# Copyright (C) 2008-2009
# ASTRON (Netherlands Foundation for Research in Astronomy)
# P.O.Box 2, 7990 AA Dwingeloo, The Netherlands, seg@astron.nl
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

# Try to find PVSS (Prozessvisualisierungs- und Steuerungssystem)
#
# Variables defined by this module:
#  
#  PVSS_FOUND        - System has PVSS
#  PVSS_DEFINITIONS  - Compiler definitions required for PVSS 
#  PVSS_INCLUDE_DIR  - "Top-level" PVSS include directory (cached)
#  PVSS_INCLUDE_DIRS - List of PVSS include directories
#  PVSS_LIBRARY_DIR  - PVSS library directory (cached)
#  PVSS_LIBRARIES    - List of all PVSS libraries

# Compiler definitions required for PVSS
set(pvss_definitions
  -D__UNIX
  -D__PC
  -DHAS_TEMPLATES=1 
  -DBC_DESTRUCTOR 
  -Dbcm_boolean_h 
  -DOS_LINUX 
  -DLINUX 
  -DLINUX2 
  -DDLLEXP_BASICS= 
  -DDLLEXP_CONFIGS= 
  -DDLLEXP_DATAPOINT= 
  -DDLLEXP_MESSAGES= 
  -DDLLEXP_MANAGER= 
  -DDLLEXP_CTRL=)
  
# Define PVSS include directories, relative to top-level include directory.
set(pvss_include_dirs 
  Basics/DpBasics 
  Basics/Variables 
  Basics/Utilities 
  Basics/NoPosix 
  BCMNew 
  Configs 
  Datapoint 
  Messages 
  Manager)

# Define PVSS libraries.
set(pvss_libraries 
  Manager
  Messages 
  Datapoint 
  Basics 
  bcm)
  
if(NOT PVSS_FOUND)

  # Get the PVSS version information. 
  # Use hard-coded value if version information file cannot be found.
  find_file(PVSS_VERSINFO_MK
    NAMES VersInfo.mk
    PATHS ENV PVSSHOME
    PATH_SUFFIXES api)
  mark_as_advanced(PVSS_VERSINFO_MK)
  if(NOT PVSS_VERSINFO_MK)
    set(pvss_version "V37_304")
  else()
    file(STRINGS ${PVSS_VERSINFO_MK} match REGEX "^PVSS_VERSION_MAIN")
    string(REGEX REPLACE "^.*= *([^ ]+)$" "\\1" pvss_version_main ${match})
    file(STRINGS ${PVSS_VERSINFO_MK} match REGEX "^PVSS_VERSION_BUILD")
    string(REGEX REPLACE "^.*= *([^ ]+)$" "\\1" pvss_version_build ${match})
    set(pvss_version "V${pvss_version_main}_${pvss_version_build}")
  endif(NOT PVSS_VERSINFO_MK)

  # Search for the PVSS include directory
  find_path(PVSS_INCLUDE_DIR
    NAMES Basics/Utilities/Util.hxx
    PATHS ENV PVSSHOME
    PATH_SUFFIXES api/include)
  set(PVSS_INCLUDE_DIRS ${PVSS_INCLUDE_DIR})
  set(pvss_check_list PVSS_INCLUDE_DIR)
  
  # Search for the PVSS libraries
  string(TOLOWER "${CMAKE_SYSTEM_NAME}" osname)
  foreach(lib ${pvss_libraries})
    find_library(PVSS_${lib}_LIBRARY
      NAMES ${lib}${pvss_version}
      PATHS ENV PVSSHOME
      PATH_SUFFIXES api/lib.${osname})
    list(APPEND pvss_check_list PVSS_${lib}_LIBRARY)
  endforeach(lib Manager Messages Datapoint Basics bcm)

  # Mark all variables in pvss_check_list as advanced
  mark_as_advanced(${pvss_check_list})
  
  # Handle the QUIETLY and REQUIRED arguments and set PVSS_FOUND to TRUE if
  # all elements of pvss_check_list are TRUE.
  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(PVSS DEFAULT_MSG ${pvss_check_list})
  
  # Now it's time to fill the non-cached variables
  set(PVSS_DEFINITIONS)
  foreach(def ${pvss_definitions})
    set(PVSS_DEFINITIONS "${PVSS_DEFINITIONS} ${def}")
  endforeach(def ${pvss_definitions})
  set(PVSS_INCLUDE_DIRS)
  foreach(dir ${pvss_include_dirs})
    list(APPEND PVSS_INCLUDE_DIRS ${PVSS_INCLUDE_DIR}/${dir})
  endforeach(dir ${pvss_include_dirs})
  set(PVSS_LIBRARIES)
  foreach(lib ${pvss_libraries})
    list(APPEND PVSS_LIBRARIES ${PVSS_${lib}_LIBRARY})
  endforeach(lib ${pvss_libraries})
   
endif(NOT PVSS_FOUND)
