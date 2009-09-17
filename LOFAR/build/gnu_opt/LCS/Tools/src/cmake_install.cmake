# Install script for directory: /home/oms/MakeMS/LOFAR/LCS/Tools/src

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
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    IF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/sbin/LimitExec")
      FILE(RPATH_CHECK
           FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/sbin/LimitExec"
           RPATH "/home/oms/MakeMS/LOFAR/installed/gnu_opt/lib")
    ENDIF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/sbin/LimitExec")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/sbin" TYPE EXECUTABLE FILES "/home/oms/MakeMS/LOFAR/build/gnu_opt/LCS/Tools/src/LimitExec")
    IF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/sbin/LimitExec")
      FILE(RPATH_CHANGE
           FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/sbin/LimitExec"
           OLD_RPATH "::::::::::::::::::::::::::::::::::::::::::::"
           NEW_RPATH "/home/oms/MakeMS/LOFAR/installed/gnu_opt/lib")
      IF(CMAKE_INSTALL_DO_STRIP)
        EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/sbin/LimitExec")
      ENDIF(CMAKE_INSTALL_DO_STRIP)
    ENDIF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/sbin/LimitExec")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Oo][Pp][Tt])$")
    IF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/sbin/LimitExec")
      FILE(RPATH_CHECK
           FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/sbin/LimitExec"
           RPATH "/home/oms/MakeMS/LOFAR/installed/gnu_opt/lib")
    ENDIF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/sbin/LimitExec")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/sbin" TYPE EXECUTABLE FILES "/home/oms/MakeMS/LOFAR/build/gnu_opt/LCS/Tools/src/LimitExec")
    IF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/sbin/LimitExec")
      FILE(RPATH_CHANGE
           FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/sbin/LimitExec"
           OLD_RPATH "::::::::::::::::::::::::::::::::::::::::::::"
           NEW_RPATH "/home/oms/MakeMS/LOFAR/installed/gnu_opt/lib")
      IF(CMAKE_INSTALL_DO_STRIP)
        EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/sbin/LimitExec")
      ENDIF(CMAKE_INSTALL_DO_STRIP)
    ENDIF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/sbin/LimitExec")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Oo][Pp][Tt])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" MATCHES "^(Unspecified)$")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" MATCHES "^(Unspecified)$")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    IF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/sbin/pkgdep")
      FILE(RPATH_CHECK
           FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/sbin/pkgdep"
           RPATH "/home/oms/MakeMS/LOFAR/installed/gnu_opt/lib")
    ENDIF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/sbin/pkgdep")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/sbin" TYPE EXECUTABLE FILES "/home/oms/MakeMS/LOFAR/build/gnu_opt/LCS/Tools/src/pkgdep")
    IF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/sbin/pkgdep")
      FILE(RPATH_CHANGE
           FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/sbin/pkgdep"
           OLD_RPATH "::::::::::::::::::::::::::::::::::::::::::::"
           NEW_RPATH "/home/oms/MakeMS/LOFAR/installed/gnu_opt/lib")
      IF(CMAKE_INSTALL_DO_STRIP)
        EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/sbin/pkgdep")
      ENDIF(CMAKE_INSTALL_DO_STRIP)
    ENDIF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/sbin/pkgdep")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Oo][Pp][Tt])$")
    IF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/sbin/pkgdep")
      FILE(RPATH_CHECK
           FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/sbin/pkgdep"
           RPATH "/home/oms/MakeMS/LOFAR/installed/gnu_opt/lib")
    ENDIF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/sbin/pkgdep")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/sbin" TYPE EXECUTABLE FILES "/home/oms/MakeMS/LOFAR/build/gnu_opt/LCS/Tools/src/pkgdep")
    IF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/sbin/pkgdep")
      FILE(RPATH_CHANGE
           FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/sbin/pkgdep"
           OLD_RPATH "::::::::::::::::::::::::::::::::::::::::::::"
           NEW_RPATH "/home/oms/MakeMS/LOFAR/installed/gnu_opt/lib")
      IF(CMAKE_INSTALL_DO_STRIP)
        EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/sbin/pkgdep")
      ENDIF(CMAKE_INSTALL_DO_STRIP)
    ENDIF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/sbin/pkgdep")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Oo][Pp][Tt])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" MATCHES "^(Unspecified)$")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" MATCHES "^(Unspecified)$")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/sbin" TYPE PROGRAM FILES
    "/home/oms/MakeMS/LOFAR/LCS/Tools/src/checkrun"
    "/home/oms/MakeMS/LOFAR/LCS/Tools/src/countalllines"
    "/home/oms/MakeMS/LOFAR/LCS/Tools/src/countlines"
    "/home/oms/MakeMS/LOFAR/LCS/Tools/src/countpurecode"
    "/home/oms/MakeMS/LOFAR/LCS/Tools/src/countpurecodecomm"
    "/home/oms/MakeMS/LOFAR/LCS/Tools/src/fillreview"
    "/home/oms/MakeMS/LOFAR/LCS/Tools/src/finddep"
    "/home/oms/MakeMS/LOFAR/LCS/Tools/src/lofarcvsmv"
    "/home/oms/MakeMS/LOFAR/LCS/Tools/src/lofarcvsx"
    "/home/oms/MakeMS/LOFAR/LCS/Tools/src/makecov"
    "/home/oms/MakeMS/LOFAR/LCS/Tools/src/makedoc"
    "/home/oms/MakeMS/LOFAR/LCS/Tools/src/makedep.sh"
    "/home/oms/MakeMS/LOFAR/LCS/Tools/src/makeClass.py"
    "/home/oms/MakeMS/LOFAR/LCS/Tools/src/makePackage.py"
    "/home/oms/MakeMS/LOFAR/LCS/Tools/src/makeTest.py"
    "/home/oms/MakeMS/LOFAR/LCS/Tools/src/processgcov"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" MATCHES "^(Unspecified)$")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" MATCHES "^(Unspecified)$")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/templates" TYPE FILE FILES
    "/home/oms/MakeMS/LOFAR/LCS/Tools/src/templates/header.cc_template"
    "/home/oms/MakeMS/LOFAR/LCS/Tools/src/templates/header.h_template"
    "/home/oms/MakeMS/LOFAR/LCS/Tools/src/templates/main.cc_template"
    "/home/oms/MakeMS/LOFAR/LCS/Tools/src/templates/package_bootstrap_template"
    "/home/oms/MakeMS/LOFAR/LCS/Tools/src/templates/package_configure.in_template"
    "/home/oms/MakeMS/LOFAR/LCS/Tools/src/templates/package_spec.in_template"
    "/home/oms/MakeMS/LOFAR/LCS/Tools/src/templates/package_lofarconf.in_template"
    "/home/oms/MakeMS/LOFAR/LCS/Tools/src/templates/package_makefile.am_template"
    "/home/oms/MakeMS/LOFAR/LCS/Tools/src/templates/include_makefile.am_template"
    "/home/oms/MakeMS/LOFAR/LCS/Tools/src/templates/include_pkg_makefile.am_template"
    "/home/oms/MakeMS/LOFAR/LCS/Tools/src/templates/src_makefile.am_template"
    "/home/oms/MakeMS/LOFAR/LCS/Tools/src/templates/templated_header.cc_template"
    "/home/oms/MakeMS/LOFAR/LCS/Tools/src/templates/templated_header.h_template"
    "/home/oms/MakeMS/LOFAR/LCS/Tools/src/templates/templated_header.tcc_template"
    "/home/oms/MakeMS/LOFAR/LCS/Tools/src/templates/test.cc_template"
    "/home/oms/MakeMS/LOFAR/LCS/Tools/src/templates/test.in_template"
    "/home/oms/MakeMS/LOFAR/LCS/Tools/src/templates/test.out_template"
    "/home/oms/MakeMS/LOFAR/LCS/Tools/src/templates/test.sh_template"
    "/home/oms/MakeMS/LOFAR/LCS/Tools/src/templates/test_makefile.am_template"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" MATCHES "^(Unspecified)$")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" MATCHES "^(Unspecified)$")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/." TYPE FILE FILES
    "/home/oms/MakeMS/LOFAR/build/gnu_opt/lofarinit.sh"
    "/home/oms/MakeMS/LOFAR/build/gnu_opt/lofarinit.csh"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" MATCHES "^(Unspecified)$")

