# $Id: FindReadline.cmake 13603 2009-07-07 12:45:52Z loose $
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

# Try to find readline, a library for easy editing of command lines.
#
# This will define:
#  
#  READLINE_FOUND        - system has Readline
#  READLINE_INCLUDE_DIR  - the Readline include directory (cached)
#  READLINE_INCLUDE_DIRS - the Readline include directories
#                           (identical to READLINE_INCLUDE_DIR)
#  READLINE_LIBRARY      - the Readline library (cached)
#  READLINE_LIBRARIES    - the Readline library plus the libraries it depends on

if(NOT READLINE_FOUND)

  find_path(READLINE_INCLUDE_DIR readline/readline.h)
  find_library(READLINE_LIBRARY readline)
  find_library(NCURSES_LIBRARY ncurses)   # readline depends on libncurses
  mark_as_advanced(READLINE_INCLUDE_DIR READLINE_LIBRARY NCURSES_LIBRARY)

  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(Readline DEFAULT_MSG
    READLINE_LIBRARY NCURSES_LIBRARY READLINE_INCLUDE_DIR)

  set(READLINE_INCLUDE_DIRS ${READLINE_INCLUDE_DIR})
  set(READLINE_LIBRARIES ${READLINE_LIBRARY} ${NCURSES_LIBRARY})

endif(NOT READLINE_FOUND)
