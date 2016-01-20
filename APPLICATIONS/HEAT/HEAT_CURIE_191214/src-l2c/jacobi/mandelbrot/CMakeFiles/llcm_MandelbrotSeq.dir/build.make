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
include mandelbrot/CMakeFiles/llcm_MandelbrotSeq.dir/depend.make

# Include the progress variables for this target.
include mandelbrot/CMakeFiles/llcm_MandelbrotSeq.dir/progress.make

# Include the compile flags for this target's objects.
include mandelbrot/CMakeFiles/llcm_MandelbrotSeq.dir/flags.make

mandelbrot/CMakeFiles/llcm_MandelbrotSeq.dir/src/MandelbrotSeq/MandelbrotSeq.cpp.o: mandelbrot/CMakeFiles/llcm_MandelbrotSeq.dir/flags.make
mandelbrot/CMakeFiles/llcm_MandelbrotSeq.dir/src/MandelbrotSeq/MandelbrotSeq.cpp.o: ../mandelbrot/src/MandelbrotSeq/MandelbrotSeq.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object mandelbrot/CMakeFiles/llcm_MandelbrotSeq.dir/src/MandelbrotSeq/MandelbrotSeq.cpp.o"
	cd /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/mandelbrot && /usr/local/bin/mpic++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/llcm_MandelbrotSeq.dir/src/MandelbrotSeq/MandelbrotSeq.cpp.o -c /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/mandelbrot/src/MandelbrotSeq/MandelbrotSeq.cpp

mandelbrot/CMakeFiles/llcm_MandelbrotSeq.dir/src/MandelbrotSeq/MandelbrotSeq.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/llcm_MandelbrotSeq.dir/src/MandelbrotSeq/MandelbrotSeq.cpp.i"
	cd /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/mandelbrot && /usr/local/bin/mpic++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/mandelbrot/src/MandelbrotSeq/MandelbrotSeq.cpp > CMakeFiles/llcm_MandelbrotSeq.dir/src/MandelbrotSeq/MandelbrotSeq.cpp.i

mandelbrot/CMakeFiles/llcm_MandelbrotSeq.dir/src/MandelbrotSeq/MandelbrotSeq.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/llcm_MandelbrotSeq.dir/src/MandelbrotSeq/MandelbrotSeq.cpp.s"
	cd /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/mandelbrot && /usr/local/bin/mpic++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/mandelbrot/src/MandelbrotSeq/MandelbrotSeq.cpp -o CMakeFiles/llcm_MandelbrotSeq.dir/src/MandelbrotSeq/MandelbrotSeq.cpp.s

mandelbrot/CMakeFiles/llcm_MandelbrotSeq.dir/src/MandelbrotSeq/MandelbrotSeq.cpp.o.requires:
.PHONY : mandelbrot/CMakeFiles/llcm_MandelbrotSeq.dir/src/MandelbrotSeq/MandelbrotSeq.cpp.o.requires

mandelbrot/CMakeFiles/llcm_MandelbrotSeq.dir/src/MandelbrotSeq/MandelbrotSeq.cpp.o.provides: mandelbrot/CMakeFiles/llcm_MandelbrotSeq.dir/src/MandelbrotSeq/MandelbrotSeq.cpp.o.requires
	$(MAKE) -f mandelbrot/CMakeFiles/llcm_MandelbrotSeq.dir/build.make mandelbrot/CMakeFiles/llcm_MandelbrotSeq.dir/src/MandelbrotSeq/MandelbrotSeq.cpp.o.provides.build
.PHONY : mandelbrot/CMakeFiles/llcm_MandelbrotSeq.dir/src/MandelbrotSeq/MandelbrotSeq.cpp.o.provides

mandelbrot/CMakeFiles/llcm_MandelbrotSeq.dir/src/MandelbrotSeq/MandelbrotSeq.cpp.o.provides.build: mandelbrot/CMakeFiles/llcm_MandelbrotSeq.dir/src/MandelbrotSeq/MandelbrotSeq.cpp.o

# Object files for target llcm_MandelbrotSeq
llcm_MandelbrotSeq_OBJECTS = \
"CMakeFiles/llcm_MandelbrotSeq.dir/src/MandelbrotSeq/MandelbrotSeq.cpp.o"

# External object files for target llcm_MandelbrotSeq
llcm_MandelbrotSeq_EXTERNAL_OBJECTS =

mandelbrot/libllcm_MandelbrotSeq.so: mandelbrot/CMakeFiles/llcm_MandelbrotSeq.dir/src/MandelbrotSeq/MandelbrotSeq.cpp.o
mandelbrot/libllcm_MandelbrotSeq.so: mandelbrot/CMakeFiles/llcm_MandelbrotSeq.dir/build.make
mandelbrot/libllcm_MandelbrotSeq.so: mandelbrot/CMakeFiles/llcm_MandelbrotSeq.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX shared module libllcm_MandelbrotSeq.so"
	cd /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/mandelbrot && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/llcm_MandelbrotSeq.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
mandelbrot/CMakeFiles/llcm_MandelbrotSeq.dir/build: mandelbrot/libllcm_MandelbrotSeq.so
.PHONY : mandelbrot/CMakeFiles/llcm_MandelbrotSeq.dir/build

mandelbrot/CMakeFiles/llcm_MandelbrotSeq.dir/requires: mandelbrot/CMakeFiles/llcm_MandelbrotSeq.dir/src/MandelbrotSeq/MandelbrotSeq.cpp.o.requires
.PHONY : mandelbrot/CMakeFiles/llcm_MandelbrotSeq.dir/requires

mandelbrot/CMakeFiles/llcm_MandelbrotSeq.dir/clean:
	cd /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/mandelbrot && $(CMAKE_COMMAND) -P CMakeFiles/llcm_MandelbrotSeq.dir/cmake_clean.cmake
.PHONY : mandelbrot/CMakeFiles/llcm_MandelbrotSeq.dir/clean

mandelbrot/CMakeFiles/llcm_MandelbrotSeq.dir/depend:
	cd /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/mandelbrot /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/mandelbrot /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/mandelbrot/CMakeFiles/llcm_MandelbrotSeq.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : mandelbrot/CMakeFiles/llcm_MandelbrotSeq.dir/depend

