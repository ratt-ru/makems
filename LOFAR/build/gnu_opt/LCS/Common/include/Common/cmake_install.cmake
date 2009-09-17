# Install script for directory: /home/oms/MakeMS/LOFAR/LCS/Common/include/Common

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/home/oms/MakeMS/LOFAR/installed/gnu_opt")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "DEBUG")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" MATCHES "^(Unspecified)$")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Common" TYPE FILE FILES
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/Allocator.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/Backtrace.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/CheckConfig.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/ComplexBuiltinFP.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/ComplexBuiltinInt.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/ComplexC99.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/ComplexStdFP.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/ComplexStdInt.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/DataConvert.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/DataFormat.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/Exception.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/Exceptions.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/FileLocator.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/hexdump.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/i4complex.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/KVpair.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/lofar_algorithm.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/lofar_bitset.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/LofarConstants.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/lofar_complex.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/lofar_datetime.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/lofar_deque.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/lofar_fstream.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/lofar_iomanip.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/lofar_iosfwd.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/lofar_iostream.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/lofar_list.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/LofarLocators.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/LofarLog4Cplus.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/LofarLog4Cxx.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/LofarLogCout.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/LofarLogger.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/lofar_map.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/lofar_math.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/lofar_numeric.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/lofar_set.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/lofar_smartptr.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/lofar_sstream.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/lofar_stack.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/lofar_string.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/lofar_thread.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/lofar_tribool.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/lofar_typeinfo.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/LofarTypedefs.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/LofarTypes.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/lofar_vector.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/Numeric.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/ObjectFactory.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/ParameterValue.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/ParameterSet.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/ParameterSetImpl.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/PrettyUnits.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/Process.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/ReadLine.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/RunOnNode.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/Semaphore.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/Singleton.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/Stopwatch.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/StreamUtil.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/StringUtil.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/SystemUtil.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/Timer.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/TypeNames.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/TypeNames.tcc"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/Version.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" MATCHES "^(Unspecified)$")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" MATCHES "^(Unspecified)$")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Common/Net" TYPE FILE FILES
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/Net/FdSet.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/Net/Socket.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" MATCHES "^(Unspecified)$")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" MATCHES "^(Unspecified)$")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Common/shmem" TYPE FILE FILES
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/shmem/segbasemap.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/shmem/dlmalloc.h"
    "/home/oms/MakeMS/LOFAR/LCS/Common/include/Common/shmem/shmem_alloc.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" MATCHES "^(Unspecified)$")

