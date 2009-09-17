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

lofar_add_sbin_program(LimitExec LimitExec.cc)
lofar_add_sbin_program(pkgdep pkgdep.cc)

install(PROGRAMS
  checkrun
  countalllines
  countlines
  countpurecode
  countpurecodecomm
  fillreview
  finddep
  lofarcvsmv
  lofarcvsx
  makecov
  makedoc
  makedep.sh
  makeClass.py
  makePackage.py
  makeTest.py
  processgcov
  DESTINATION sbin)

install(FILES
  templates/header.cc_template
  templates/header.h_template
  templates/main.cc_template  		      
  templates/package_bootstrap_template  		      
  templates/package_configure.in_template  		      
  templates/package_spec.in_template  		      
  templates/package_lofarconf.in_template
  templates/package_makefile.am_template  		      
  templates/include_makefile.am_template  		      
  templates/include_pkg_makefile.am_template  		      
  templates/src_makefile.am_template  	              
  templates/templated_header.cc_template  		      
  templates/templated_header.h_template  		      
  templates/templated_header.tcc_template
  templates/test.cc_template
  templates/test.in_template
  templates/test.out_template
  templates/test.sh_template  		      
  templates/test_makefile.am_template
  DESTINATION share/templates)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/lofarinit.sh.in
               ${CMAKE_BINARY_DIR}/lofarinit.sh @ONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/lofarinit.csh.in
               ${CMAKE_BINARY_DIR}/lofarinit.csh @ONLY)

install(FILES
  ${CMAKE_BINARY_DIR}/lofarinit.sh
  ${CMAKE_BINARY_DIR}/lofarinit.csh
  DESTINATION .)
