# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.0

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi

# Include any dependencies generated for this target.
include jacobi/CMakeFiles/MpiConcentrator.dir/depend.make

# Include the progress variables for this target.
include jacobi/CMakeFiles/MpiConcentrator.dir/progress.make

# Include the compile flags for this target's objects.
include jacobi/CMakeFiles/MpiConcentrator.dir/flags.make

jacobi/CMakeFiles/MpiConcentrator.dir/src/MpiConcentrator/mpiconcentrator.cpp.o: jacobi/CMakeFiles/MpiConcentrator.dir/flags.make
jacobi/CMakeFiles/MpiConcentrator.dir/src/MpiConcentrator/mpiconcentrator.cpp.o: src/MpiConcentrator/mpiconcentrator.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object jacobi/CMakeFiles/MpiConcentrator.dir/src/MpiConcentrator/mpiconcentrator.cpp.o"
	cd /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/jacobi && /usr/local/bin/mpic++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/MpiConcentrator.dir/src/MpiConcentrator/mpiconcentrator.cpp.o -c /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/src/MpiConcentrator/mpiconcentrator.cpp

jacobi/CMakeFiles/MpiConcentrator.dir/src/MpiConcentrator/mpiconcentrator.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MpiConcentrator.dir/src/MpiConcentrator/mpiconcentrator.cpp.i"
	cd /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/jacobi && /usr/local/bin/mpic++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/src/MpiConcentrator/mpiconcentrator.cpp > CMakeFiles/MpiConcentrator.dir/src/MpiConcentrator/mpiconcentrator.cpp.i

jacobi/CMakeFiles/MpiConcentrator.dir/src/MpiConcentrator/mpiconcentrator.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MpiConcentrator.dir/src/MpiConcentrator/mpiconcentrator.cpp.s"
	cd /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/jacobi && /usr/local/bin/mpic++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/src/MpiConcentrator/mpiconcentrator.cpp -o CMakeFiles/MpiConcentrator.dir/src/MpiConcentrator/mpiconcentrator.cpp.s

jacobi/CMakeFiles/MpiConcentrator.dir/src/MpiConcentrator/mpiconcentrator.cpp.o.requires:
.PHONY : jacobi/CMakeFiles/MpiConcentrator.dir/src/MpiConcentrator/mpiconcentrator.cpp.o.requires

jacobi/CMakeFiles/MpiConcentrator.dir/src/MpiConcentrator/mpiconcentrator.cpp.o.provides: jacobi/CMakeFiles/MpiConcentrator.dir/src/MpiConcentrator/mpiconcentrator.cpp.o.requires
	$(MAKE) -f jacobi/CMakeFiles/MpiConcentrator.dir/build.make jacobi/CMakeFiles/MpiConcentrator.dir/src/MpiConcentrator/mpiconcentrator.cpp.o.provides.build
.PHONY : jacobi/CMakeFiles/MpiConcentrator.dir/src/MpiConcentrator/mpiconcentrator.cpp.o.provides

jacobi/CMakeFiles/MpiConcentrator.dir/src/MpiConcentrator/mpiconcentrator.cpp.o.provides.build: jacobi/CMakeFiles/MpiConcentrator.dir/src/MpiConcentrator/mpiconcentrator.cpp.o

# Object files for target MpiConcentrator
MpiConcentrator_OBJECTS = \
"CMakeFiles/MpiConcentrator.dir/src/MpiConcentrator/mpiconcentrator.cpp.o"

# External object files for target MpiConcentrator
MpiConcentrator_EXTERNAL_OBJECTS =

jacobi/libMpiConcentrator.so: jacobi/CMakeFiles/MpiConcentrator.dir/src/MpiConcentrator/mpiconcentrator.cpp.o
jacobi/libMpiConcentrator.so: jacobi/CMakeFiles/MpiConcentrator.dir/build.make
jacobi/libMpiConcentrator.so: /usr/local/lib/libmpi_cxx.dylib
jacobi/libMpiConcentrator.so: /usr/local/lib/libmpi.dylib
jacobi/libMpiConcentrator.so: jacobi/CMakeFiles/MpiConcentrator.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX shared module libMpiConcentrator.so"
	cd /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/jacobi && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/MpiConcentrator.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
jacobi/CMakeFiles/MpiConcentrator.dir/build: jacobi/libMpiConcentrator.so
.PHONY : jacobi/CMakeFiles/MpiConcentrator.dir/build

jacobi/CMakeFiles/MpiConcentrator.dir/requires: jacobi/CMakeFiles/MpiConcentrator.dir/src/MpiConcentrator/mpiconcentrator.cpp.o.requires
.PHONY : jacobi/CMakeFiles/MpiConcentrator.dir/requires

jacobi/CMakeFiles/MpiConcentrator.dir/clean:
	cd /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/jacobi && $(CMAKE_COMMAND) -P CMakeFiles/MpiConcentrator.dir/cmake_clean.cmake
.PHONY : jacobi/CMakeFiles/MpiConcentrator.dir/clean

jacobi/CMakeFiles/MpiConcentrator.dir/depend:
	cd /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/jacobi /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/jacobi/CMakeFiles/MpiConcentrator.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : jacobi/CMakeFiles/MpiConcentrator.dir/depend

