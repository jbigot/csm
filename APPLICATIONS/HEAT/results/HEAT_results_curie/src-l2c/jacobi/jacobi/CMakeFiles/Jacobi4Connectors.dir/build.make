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
include jacobi/CMakeFiles/Jacobi4Connectors.dir/depend.make

# Include the progress variables for this target.
include jacobi/CMakeFiles/Jacobi4Connectors.dir/progress.make

# Include the compile flags for this target's objects.
include jacobi/CMakeFiles/Jacobi4Connectors.dir/flags.make

jacobi/CMakeFiles/Jacobi4Connectors.dir/src/Jacobi4Connectors/jacobi4connectors.cpp.o: jacobi/CMakeFiles/Jacobi4Connectors.dir/flags.make
jacobi/CMakeFiles/Jacobi4Connectors.dir/src/Jacobi4Connectors/jacobi4connectors.cpp.o: src/Jacobi4Connectors/jacobi4connectors.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object jacobi/CMakeFiles/Jacobi4Connectors.dir/src/Jacobi4Connectors/jacobi4connectors.cpp.o"
	cd /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/jacobi && /usr/local/bin/mpic++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Jacobi4Connectors.dir/src/Jacobi4Connectors/jacobi4connectors.cpp.o -c /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/src/Jacobi4Connectors/jacobi4connectors.cpp

jacobi/CMakeFiles/Jacobi4Connectors.dir/src/Jacobi4Connectors/jacobi4connectors.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Jacobi4Connectors.dir/src/Jacobi4Connectors/jacobi4connectors.cpp.i"
	cd /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/jacobi && /usr/local/bin/mpic++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/src/Jacobi4Connectors/jacobi4connectors.cpp > CMakeFiles/Jacobi4Connectors.dir/src/Jacobi4Connectors/jacobi4connectors.cpp.i

jacobi/CMakeFiles/Jacobi4Connectors.dir/src/Jacobi4Connectors/jacobi4connectors.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Jacobi4Connectors.dir/src/Jacobi4Connectors/jacobi4connectors.cpp.s"
	cd /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/jacobi && /usr/local/bin/mpic++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/src/Jacobi4Connectors/jacobi4connectors.cpp -o CMakeFiles/Jacobi4Connectors.dir/src/Jacobi4Connectors/jacobi4connectors.cpp.s

jacobi/CMakeFiles/Jacobi4Connectors.dir/src/Jacobi4Connectors/jacobi4connectors.cpp.o.requires:
.PHONY : jacobi/CMakeFiles/Jacobi4Connectors.dir/src/Jacobi4Connectors/jacobi4connectors.cpp.o.requires

jacobi/CMakeFiles/Jacobi4Connectors.dir/src/Jacobi4Connectors/jacobi4connectors.cpp.o.provides: jacobi/CMakeFiles/Jacobi4Connectors.dir/src/Jacobi4Connectors/jacobi4connectors.cpp.o.requires
	$(MAKE) -f jacobi/CMakeFiles/Jacobi4Connectors.dir/build.make jacobi/CMakeFiles/Jacobi4Connectors.dir/src/Jacobi4Connectors/jacobi4connectors.cpp.o.provides.build
.PHONY : jacobi/CMakeFiles/Jacobi4Connectors.dir/src/Jacobi4Connectors/jacobi4connectors.cpp.o.provides

jacobi/CMakeFiles/Jacobi4Connectors.dir/src/Jacobi4Connectors/jacobi4connectors.cpp.o.provides.build: jacobi/CMakeFiles/Jacobi4Connectors.dir/src/Jacobi4Connectors/jacobi4connectors.cpp.o

# Object files for target Jacobi4Connectors
Jacobi4Connectors_OBJECTS = \
"CMakeFiles/Jacobi4Connectors.dir/src/Jacobi4Connectors/jacobi4connectors.cpp.o"

# External object files for target Jacobi4Connectors
Jacobi4Connectors_EXTERNAL_OBJECTS =

jacobi/libJacobi4Connectors.so: jacobi/CMakeFiles/Jacobi4Connectors.dir/src/Jacobi4Connectors/jacobi4connectors.cpp.o
jacobi/libJacobi4Connectors.so: jacobi/CMakeFiles/Jacobi4Connectors.dir/build.make
jacobi/libJacobi4Connectors.so: /usr/local/lib/libmpi_cxx.dylib
jacobi/libJacobi4Connectors.so: /usr/local/lib/libmpi.dylib
jacobi/libJacobi4Connectors.so: jacobi/CMakeFiles/Jacobi4Connectors.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX shared module libJacobi4Connectors.so"
	cd /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/jacobi && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Jacobi4Connectors.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
jacobi/CMakeFiles/Jacobi4Connectors.dir/build: jacobi/libJacobi4Connectors.so
.PHONY : jacobi/CMakeFiles/Jacobi4Connectors.dir/build

jacobi/CMakeFiles/Jacobi4Connectors.dir/requires: jacobi/CMakeFiles/Jacobi4Connectors.dir/src/Jacobi4Connectors/jacobi4connectors.cpp.o.requires
.PHONY : jacobi/CMakeFiles/Jacobi4Connectors.dir/requires

jacobi/CMakeFiles/Jacobi4Connectors.dir/clean:
	cd /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/jacobi && $(CMAKE_COMMAND) -P CMakeFiles/Jacobi4Connectors.dir/cmake_clean.cmake
.PHONY : jacobi/CMakeFiles/Jacobi4Connectors.dir/clean

jacobi/CMakeFiles/Jacobi4Connectors.dir/depend:
	cd /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/jacobi /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/jacobi/CMakeFiles/Jacobi4Connectors.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : jacobi/CMakeFiles/Jacobi4Connectors.dir/depend
