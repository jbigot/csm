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
include jacobi/CMakeFiles/JacocpuCore.dir/depend.make

# Include the progress variables for this target.
include jacobi/CMakeFiles/JacocpuCore.dir/progress.make

# Include the compile flags for this target's objects.
include jacobi/CMakeFiles/JacocpuCore.dir/flags.make

jacobi/CMakeFiles/JacocpuCore.dir/src/JacocpuCore/jacocpucore.cpp.o: jacobi/CMakeFiles/JacocpuCore.dir/flags.make
jacobi/CMakeFiles/JacocpuCore.dir/src/JacocpuCore/jacocpucore.cpp.o: src/JacocpuCore/jacocpucore.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object jacobi/CMakeFiles/JacocpuCore.dir/src/JacocpuCore/jacocpucore.cpp.o"
	cd /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/jacobi && /usr/local/bin/mpic++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/JacocpuCore.dir/src/JacocpuCore/jacocpucore.cpp.o -c /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/src/JacocpuCore/jacocpucore.cpp

jacobi/CMakeFiles/JacocpuCore.dir/src/JacocpuCore/jacocpucore.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/JacocpuCore.dir/src/JacocpuCore/jacocpucore.cpp.i"
	cd /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/jacobi && /usr/local/bin/mpic++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/src/JacocpuCore/jacocpucore.cpp > CMakeFiles/JacocpuCore.dir/src/JacocpuCore/jacocpucore.cpp.i

jacobi/CMakeFiles/JacocpuCore.dir/src/JacocpuCore/jacocpucore.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/JacocpuCore.dir/src/JacocpuCore/jacocpucore.cpp.s"
	cd /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/jacobi && /usr/local/bin/mpic++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/src/JacocpuCore/jacocpucore.cpp -o CMakeFiles/JacocpuCore.dir/src/JacocpuCore/jacocpucore.cpp.s

jacobi/CMakeFiles/JacocpuCore.dir/src/JacocpuCore/jacocpucore.cpp.o.requires:
.PHONY : jacobi/CMakeFiles/JacocpuCore.dir/src/JacocpuCore/jacocpucore.cpp.o.requires

jacobi/CMakeFiles/JacocpuCore.dir/src/JacocpuCore/jacocpucore.cpp.o.provides: jacobi/CMakeFiles/JacocpuCore.dir/src/JacocpuCore/jacocpucore.cpp.o.requires
	$(MAKE) -f jacobi/CMakeFiles/JacocpuCore.dir/build.make jacobi/CMakeFiles/JacocpuCore.dir/src/JacocpuCore/jacocpucore.cpp.o.provides.build
.PHONY : jacobi/CMakeFiles/JacocpuCore.dir/src/JacocpuCore/jacocpucore.cpp.o.provides

jacobi/CMakeFiles/JacocpuCore.dir/src/JacocpuCore/jacocpucore.cpp.o.provides.build: jacobi/CMakeFiles/JacocpuCore.dir/src/JacocpuCore/jacocpucore.cpp.o

# Object files for target JacocpuCore
JacocpuCore_OBJECTS = \
"CMakeFiles/JacocpuCore.dir/src/JacocpuCore/jacocpucore.cpp.o"

# External object files for target JacocpuCore
JacocpuCore_EXTERNAL_OBJECTS =

jacobi/libJacocpuCore.so: jacobi/CMakeFiles/JacocpuCore.dir/src/JacocpuCore/jacocpucore.cpp.o
jacobi/libJacocpuCore.so: jacobi/CMakeFiles/JacocpuCore.dir/build.make
jacobi/libJacocpuCore.so: jacobi/CMakeFiles/JacocpuCore.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX shared module libJacocpuCore.so"
	cd /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/jacobi && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/JacocpuCore.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
jacobi/CMakeFiles/JacocpuCore.dir/build: jacobi/libJacocpuCore.so
.PHONY : jacobi/CMakeFiles/JacocpuCore.dir/build

jacobi/CMakeFiles/JacocpuCore.dir/requires: jacobi/CMakeFiles/JacocpuCore.dir/src/JacocpuCore/jacocpucore.cpp.o.requires
.PHONY : jacobi/CMakeFiles/JacocpuCore.dir/requires

jacobi/CMakeFiles/JacocpuCore.dir/clean:
	cd /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/jacobi && $(CMAKE_COMMAND) -P CMakeFiles/JacocpuCore.dir/cmake_clean.cmake
.PHONY : jacobi/CMakeFiles/JacocpuCore.dir/clean

jacobi/CMakeFiles/JacocpuCore.dir/depend:
	cd /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/jacobi /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/jacobi/CMakeFiles/JacocpuCore.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : jacobi/CMakeFiles/JacocpuCore.dir/depend
