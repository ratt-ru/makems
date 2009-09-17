#  $Id: LofarAddPackage.cmake 13983 2009-09-11 07:58:03Z loose $
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

# --------------------------------------------------------------------------
# lofar_add_package(<name> [version] [DEPENDS <depend> [depend] ...])
#
# Add a LOFAR package. 
#
# This function performs the following actions:
# - Create a custom target for package <name>, using add_custom_target().
# - If package <name> has dependencies, then, for each dependency <dep>:
#   - add a dependency of package <name> on package <dep>
#   - add the list of include directories of <dep> to that of <name>
# - Add a preprocessor definition for LOFARLOGGER_PACKAGE
#
# This function sets the following variables in the parent scope:
#   ${name}_VERSION       Version number of package <name>
#   ${name}_DEPENDENCIES  List of packages that package <name> depends on.
#
# --------------------------------------------------------------------------
function(lofar_add_package _name)

  set(_errmsg 
    "Wrong arguments supplied to lofar_add_package().\n"
    "Usage: lofar_add_package(name [version] [DEPENDS depend ...])\n")

  # Print message to console if configuring verbose.
  if(LOFAR_VERBOSE_CONFIGURE)
    message(STATUS "Adding package ${_name}...")
  endif(LOFAR_VERBOSE_CONFIGURE)

  # If arguments were given, then the first could be a version number.
  if(ARGC GREATER 1)
    list(GET ARGV 1 _version)
    string(REGEX MATCH "^[0-9]+(\\.[0-9]+)*$" _version ${_version})
  endif(ARGC GREATER 1)
  
  # If two arguments were given, then the second must be a version number.
  if(ARGC EQUAL 2 AND NOT _version)
    message(FATAL_ERROR ${_errmsg})
  endif(ARGC EQUAL 2 AND NOT _version)

  # If more than two arguments were given, then search for the DEPENDS
  # keyword, which must be present.
  if(ARGC GREATER 2)
    list(FIND ARGV DEPENDS _idx)
    if(_idx EQUAL -1)
      message(FATAL_ERROR ${_errmsg})
    endif(_idx EQUAL -1)
    # Let _idx point to the first argument of DEPENDS
    math(EXPR _idx "${_idx}+1")
    if(NOT _idx LESS ARGC)
      # Missing arguments after DEPENDS
      message(FATAL_ERROR ${_errmsg})
    endif(NOT _idx LESS ARGC)
    # Add dependencies to the dependency list
    while(_idx LESS ARGC)
      list(GET ARGV ${_idx} _dep)
      list(APPEND _dependencies ${_dep})
      math(EXPR _idx "${_idx}+1")
    endwhile(_idx LESS ARGC)
  endif(ARGC GREATER 2)

  # Create a custom target for package <name>.
  add_custom_target(${_name})

  # For each package <pkg> that this package depends on:
  # - add a dependency of package <name> on package <pkg>
  # - add the list of include directories of <pkg> to that of package <name>
  foreach(_dep ${_dependencies})
    get_directory_property(_dirs 
      DIRECTORY ${${_dep}_SOURCE_DIR} INCLUDE_DIRECTORIES)
    add_dependencies(${_name} ${_dep})
#    message(STATUS "set(BUILD_${_dep} TRUE CACHE BOOL \"Build ${_dep}?\"")
#    set(BUILD_${_dep} TRUE CACHE BOOL "Build ${_dep}?")
    include_directories(${_dirs})
  endforeach(_dep ${_dependencies})

  # Set LOFAR Logger package name to the current project
  string(REGEX REPLACE "^${LOFAR_ROOT}" "" _pkg "${${_name}_SOURCE_DIR}")
  string(REGEX REPLACE "^/" "" _pkg "${_pkg}")
  string(REPLACE "/" "." _pkg "${_pkg}")
  add_definitions(-DLOFARLOGGER_PACKAGE="${_pkg}")

  # Export variables to the parent scope
  set(LOFAR_PACKAGE_NAME ${_name} PARENT_SCOPE)
  set(${_name}_VERSION ${_version} PARENT_SCOPE)
  set(${_name}_DEPENDENCIES ${_dependencies} PARENT_SCOPE)

endfunction(lofar_add_package _name)
  
