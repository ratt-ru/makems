# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.6

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canoncical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/oms/MakeMS/LOFAR

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/oms/MakeMS/LOFAR/build/gnu_opt

# Include any dependencies generated for this target.
include CEP/MS/src/CMakeFiles/makems.dir/depend.make

# Include the progress variables for this target.
include CEP/MS/src/CMakeFiles/makems.dir/progress.make

# Include the compile flags for this target's objects.
include CEP/MS/src/CMakeFiles/makems.dir/flags.make

CEP/MS/src/CMakeFiles/makems.dir/makems.cc.o: CEP/MS/src/CMakeFiles/makems.dir/flags.make
CEP/MS/src/CMakeFiles/makems.dir/makems.cc.o: ../../CEP/MS/src/makems.cc
	$(CMAKE_COMMAND) -E cmake_progress_report /home/oms/MakeMS/LOFAR/build/gnu_opt/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CEP/MS/src/CMakeFiles/makems.dir/makems.cc.o"
	cd /home/oms/MakeMS/LOFAR/build/gnu_opt/CEP/MS/src && /usr/bin/g++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/makems.dir/makems.cc.o -c /home/oms/MakeMS/LOFAR/CEP/MS/src/makems.cc

CEP/MS/src/CMakeFiles/makems.dir/makems.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/makems.dir/makems.cc.i"
	cd /home/oms/MakeMS/LOFAR/build/gnu_opt/CEP/MS/src && /usr/bin/g++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/oms/MakeMS/LOFAR/CEP/MS/src/makems.cc > CMakeFiles/makems.dir/makems.cc.i

CEP/MS/src/CMakeFiles/makems.dir/makems.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/makems.dir/makems.cc.s"
	cd /home/oms/MakeMS/LOFAR/build/gnu_opt/CEP/MS/src && /usr/bin/g++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/oms/MakeMS/LOFAR/CEP/MS/src/makems.cc -o CMakeFiles/makems.dir/makems.cc.s

CEP/MS/src/CMakeFiles/makems.dir/makems.cc.o.requires:
.PHONY : CEP/MS/src/CMakeFiles/makems.dir/makems.cc.o.requires

CEP/MS/src/CMakeFiles/makems.dir/makems.cc.o.provides: CEP/MS/src/CMakeFiles/makems.dir/makems.cc.o.requires
	$(MAKE) -f CEP/MS/src/CMakeFiles/makems.dir/build.make CEP/MS/src/CMakeFiles/makems.dir/makems.cc.o.provides.build
.PHONY : CEP/MS/src/CMakeFiles/makems.dir/makems.cc.o.provides

CEP/MS/src/CMakeFiles/makems.dir/makems.cc.o.provides.build: CEP/MS/src/CMakeFiles/makems.dir/makems.cc.o
.PHONY : CEP/MS/src/CMakeFiles/makems.dir/makems.cc.o.provides.build

# Object files for target makems
makems_OBJECTS = \
"CMakeFiles/makems.dir/makems.cc.o"

# External object files for target makems
makems_EXTERNAL_OBJECTS =

CEP/MS/src/makems: CEP/MS/src/CMakeFiles/makems.dir/makems.cc.o
CEP/MS/src/makems: CEP/MS/src/libms.a
CEP/MS/src/makems: LCS/Common/src/libcommon.a
CEP/MS/src/makems: /usr/lib/libpthread.so
CEP/MS/src/makems: /usr/lib/libcasa_ms.a
CEP/MS/src/makems: /usr/lib/libcasa_measures.a
CEP/MS/src/makems: /usr/lib/libcasa_scimath.a
CEP/MS/src/makems: /usr/lib/libcasa_tables.a
CEP/MS/src/makems: /usr/lib/libcasa_scimath_f.a
CEP/MS/src/makems: /usr/lib/libcasa_casa.a
CEP/MS/src/makems: /usr/lib/libm.so
CEP/MS/src/makems: CEP/MS/src/CMakeFiles/makems.dir/build.make
CEP/MS/src/makems: CEP/MS/src/CMakeFiles/makems.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable makems"
	cd /home/oms/MakeMS/LOFAR/build/gnu_opt/CEP/MS/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/makems.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CEP/MS/src/CMakeFiles/makems.dir/build: CEP/MS/src/makems
.PHONY : CEP/MS/src/CMakeFiles/makems.dir/build

CEP/MS/src/CMakeFiles/makems.dir/requires: CEP/MS/src/CMakeFiles/makems.dir/makems.cc.o.requires
.PHONY : CEP/MS/src/CMakeFiles/makems.dir/requires

CEP/MS/src/CMakeFiles/makems.dir/clean:
	cd /home/oms/MakeMS/LOFAR/build/gnu_opt/CEP/MS/src && $(CMAKE_COMMAND) -P CMakeFiles/makems.dir/cmake_clean.cmake
.PHONY : CEP/MS/src/CMakeFiles/makems.dir/clean

CEP/MS/src/CMakeFiles/makems.dir/depend:
	cd /home/oms/MakeMS/LOFAR/build/gnu_opt && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/oms/MakeMS/LOFAR /home/oms/MakeMS/LOFAR/CEP/MS/src /home/oms/MakeMS/LOFAR/build/gnu_opt /home/oms/MakeMS/LOFAR/build/gnu_opt/CEP/MS/src /home/oms/MakeMS/LOFAR/build/gnu_opt/CEP/MS/src/CMakeFiles/makems.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CEP/MS/src/CMakeFiles/makems.dir/depend

