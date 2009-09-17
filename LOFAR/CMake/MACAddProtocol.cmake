#  $Id: MACAddProtocol.cmake 13622 2009-07-09 09:06:10Z loose $
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
# mac_add_protocol(protocol templ_dir)
#
# Add a MAC protocol source file. 
# Use autogen to generate a C++ header and source file from the protocol
# definition file ${protocol}.prot, using a template in the template directory
# ${templ_dir}.
#
# The generated C++ header file will have an extension '.ph', to distinguish
# it from ordinary header files. Both C++ header and source file will be
# generated in the directory ${CMAKE_CURRENT_BINARY_DIR}.
# --------------------------------------------------------------------------
macro(mac_add_protocol _protocol _templ_dir)

  # Search for the autogen program
  find_program(AUTOGEN_EXECUTABLE autogen)
  mark_as_advanced(AUTOGEN_EXECUTABLE)
  if(NOT AUTOGEN_EXECUTABLE)
    message(FATAL_ERROR 
      "Could not find autogen - the Automated Program Generator")
  endif(NOT AUTOGEN_EXECUTABLE)

  # Get the full path to the protocol file and the template directory.
  get_filename_component(_proto_file "${_protocol}.prot" ABSOLUTE)
  get_filename_component(_templ_dir "${_templ_dir}" ABSOLUTE)

  # Determine the destination directory for the generated C++ header file. 
  # This directory is chosen such that other MAC packages only have to
  # add <binary-root>/include/MAC to their -I path.
  if(DEFINED ${PROJECT_NAME}_INCLUDE_PATH_SUFFIX)
    set(_dest_dir
      ${CMAKE_BINARY_DIR}/include/MAC/${${PROJECT_NAME}_INCLUDE_PATH_SUFFIX})
  else()
    string(REGEX REPLACE "${LOFAR_ROOT}" "${CMAKE_BINARY_DIR}/include" 
      _dest_dir ${${PROJECT_NAME}_SOURCE_DIR})
  endif(DEFINED ${PROJECT_NAME}_INCLUDE_PATH_SUFFIX)

  # Define the command to generate the C++ header and source files. The header
  # file will be copied to the correct destination directory.
  set(_cxx_hdr "${_protocol}.ph")
  set(_cxx_src "${_protocol}.cc")
  add_custom_command(OUTPUT "${_cxx_hdr}" "${_cxx_src}"
    COMMAND ${AUTOGEN_EXECUTABLE} 
    ARGS --writable -L "${_templ_dir}" "${_proto_file}"
    COMMAND ${CMAKE_COMMAND}
    ARGS -E copy "${_cxx_hdr}" "${_dest_dir}/${_cxx_hdr}"
    DEPENDS "${_proto_file}")

endmacro(mac_add_protocol _protocol _templ_dir)
