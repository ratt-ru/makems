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

# Create symbolic link to include directory.
execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink 
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_BINARY_DIR}/include/${PROJECT_NAME})

# Install header files.
install(FILES 
  Allocator.h
  Backtrace.h
  CheckConfig.h
  ComplexBuiltinFP.h
  ComplexBuiltinInt.h
  ComplexC99.h
  ComplexStdFP.h
  ComplexStdInt.h
  DataConvert.h
  DataFormat.h
  Exception.h
  Exceptions.h
  FileLocator.h
  hexdump.h
  i4complex.h
  KVpair.h
  lofar_algorithm.h
  lofar_bitset.h
  LofarConstants.h
  lofar_complex.h
  lofar_datetime.h
  lofar_deque.h
  lofar_fstream.h
  lofar_iomanip.h
  lofar_iosfwd.h
  lofar_iostream.h
  lofar_list.h
  LofarLocators.h
  LofarLog4Cplus.h
  LofarLog4Cxx.h
  LofarLogCout.h
  LofarLogger.h
  lofar_map.h
  lofar_math.h
  lofar_numeric.h
  lofar_set.h
  lofar_smartptr.h
  lofar_sstream.h
  lofar_stack.h
  lofar_string.h
  lofar_thread.h
  lofar_tribool.h
  lofar_typeinfo.h
  LofarTypedefs.h
  LofarTypes.h
  lofar_vector.h
  Numeric.h
  ObjectFactory.h
  ParameterValue.h
  ParameterSet.h
  ParameterSetImpl.h
  PrettyUnits.h
  Process.h
  ReadLine.h
  RunOnNode.h
  Semaphore.h
  Singleton.h
  Stopwatch.h
  StreamUtil.h
  StringUtil.h
  SystemUtil.h
  Timer.h
  TypeNames.h
  TypeNames.tcc
  Version.h
  DESTINATION include/${PROJECT_NAME})

install(FILES 
  Net/FdSet.h
  Net/Socket.h
  DESTINATION include/${PROJECT_NAME}/Net)

install(FILES
  shmem/segbasemap.h
  shmem/dlmalloc.h
  shmem/shmem_alloc.h
  DESTINATION include/${PROJECT_NAME}/shmem)
