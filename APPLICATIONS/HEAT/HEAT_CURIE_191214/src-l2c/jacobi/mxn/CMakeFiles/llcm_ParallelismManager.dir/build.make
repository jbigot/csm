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
include mxn/CMakeFiles/llcm_ParallelismManager.dir/depend.make

# Include the progress variables for this target.
include mxn/CMakeFiles/llcm_ParallelismManager.dir/progress.make

# Include the compile flags for this target's objects.
include mxn/CMakeFiles/llcm_ParallelismManager.dir/flags.make

mxn/CMakeFiles/llcm_ParallelismManager.dir/src/ParallelismManager/ParallelismManager.cpp.o: mxn/CMakeFiles/llcm_ParallelismManager.dir/flags.make
mxn/CMakeFiles/llcm_ParallelismManager.dir/src/ParallelismManager/ParallelismManager.cpp.o: ../mxn/src/ParallelismManager/ParallelismManager.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object mxn/CMakeFiles/llcm_ParallelismManager.dir/src/ParallelismManager/ParallelismManager.cpp.o"
	cd /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/mxn && /usr/local/bin/mpic++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/llcm_ParallelismManager.dir/src/ParallelismManager/ParallelismManager.cpp.o -c /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/mxn/src/ParallelismManager/ParallelismManager.cpp

mxn/CMakeFiles/llcm_ParallelismManager.dir/src/ParallelismManager/ParallelismManager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/llcm_ParallelismManager.dir/src/ParallelismManager/ParallelismManager.cpp.i"
	cd /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/mxn && /usr/local/bin/mpic++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/mxn/src/ParallelismManager/ParallelismManager.cpp > CMakeFiles/llcm_ParallelismManager.dir/src/ParallelismManager/ParallelismManager.cpp.i

mxn/CMakeFiles/llcm_ParallelismManager.dir/src/ParallelismManager/ParallelismManager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/llcm_ParallelismManager.dir/src/ParallelismManager/ParallelismManager.cpp.s"
	cd /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/mxn && /usr/local/bin/mpic++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/mxn/src/ParallelismManager/ParallelismManager.cpp -o CMakeFiles/llcm_ParallelismManager.dir/src/ParallelismManager/ParallelismManager.cpp.s

mxn/CMakeFiles/llcm_ParallelismManager.dir/src/ParallelismManager/ParallelismManager.cpp.o.requires:
.PHONY : mxn/CMakeFiles/llcm_ParallelismManager.dir/src/ParallelismManager/ParallelismManager.cpp.o.requires

mxn/CMakeFiles/llcm_ParallelismManager.dir/src/ParallelismManager/ParallelismManager.cpp.o.provides: mxn/CMakeFiles/llcm_ParallelismManager.dir/src/ParallelismManager/ParallelismManager.cpp.o.requires
	$(MAKE) -f mxn/CMakeFiles/llcm_ParallelismManager.dir/build.make mxn/CMakeFiles/llcm_ParallelismManager.dir/src/ParallelismManager/ParallelismManager.cpp.o.provides.build
.PHONY : mxn/CMakeFiles/llcm_ParallelismManager.dir/src/ParallelismManager/ParallelismManager.cpp.o.provides

mxn/CMakeFiles/llcm_ParallelismManager.dir/src/ParallelismManager/ParallelismManager.cpp.o.provides.build: mxn/CMakeFiles/llcm_ParallelismManager.dir/src/ParallelismManager/ParallelismManager.cpp.o

# Object files for target llcm_ParallelismManager
llcm_ParallelismManager_OBJECTS = \
"CMakeFiles/llcm_ParallelismManager.dir/src/ParallelismManager/ParallelismManager.cpp.o"

# External object files for target llcm_ParallelismManager
llcm_ParallelismManager_EXTERNAL_OBJECTS =

mxn/libllcm_ParallelismManager.so: mxn/CMakeFiles/llcm_ParallelismManager.dir/src/ParallelismManager/ParallelismManager.cpp.o
mxn/libllcm_ParallelismManager.so: mxn/CMakeFiles/llcm_ParallelismManager.dir/build.make
mxn/libllcm_ParallelismManager.so: mxn/CMakeFiles/llcm_ParallelismManager.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX shared module libllcm_ParallelismManager.so"
	cd /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/mxn && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/llcm_ParallelismManager.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
mxn/CMakeFiles/llcm_ParallelismManager.dir/build: mxn/libllcm_ParallelismManager.so
.PHONY : mxn/CMakeFiles/llcm_ParallelismManager.dir/build

mxn/CMakeFiles/llcm_ParallelismManager.dir/requires: mxn/CMakeFiles/llcm_ParallelismManager.dir/src/ParallelismManager/ParallelismManager.cpp.o.requires
.PHONY : mxn/CMakeFiles/llcm_ParallelismManager.dir/requires

mxn/CMakeFiles/llcm_ParallelismManager.dir/clean:
	cd /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/mxn && $(CMAKE_COMMAND) -P CMakeFiles/llcm_ParallelismManager.dir/cmake_clean.cmake
.PHONY : mxn/CMakeFiles/llcm_ParallelismManager.dir/clean

mxn/CMakeFiles/llcm_ParallelismManager.dir/depend:
	cd /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/mxn /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/mxn /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/mxn/CMakeFiles/llcm_ParallelismManager.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : mxn/CMakeFiles/llcm_ParallelismManager.dir/depend

