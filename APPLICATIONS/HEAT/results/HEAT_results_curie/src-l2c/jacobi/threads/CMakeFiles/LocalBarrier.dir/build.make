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
include threads/CMakeFiles/LocalBarrier.dir/depend.make

# Include the progress variables for this target.
include threads/CMakeFiles/LocalBarrier.dir/progress.make

# Include the compile flags for this target's objects.
include threads/CMakeFiles/LocalBarrier.dir/flags.make

threads/CMakeFiles/LocalBarrier.dir/src/LocalBarrier/localbarrier.cpp.o: threads/CMakeFiles/LocalBarrier.dir/flags.make
threads/CMakeFiles/LocalBarrier.dir/src/LocalBarrier/localbarrier.cpp.o: ../threads/src/LocalBarrier/localbarrier.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object threads/CMakeFiles/LocalBarrier.dir/src/LocalBarrier/localbarrier.cpp.o"
	cd /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/threads && /usr/local/bin/mpic++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/LocalBarrier.dir/src/LocalBarrier/localbarrier.cpp.o -c /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/threads/src/LocalBarrier/localbarrier.cpp

threads/CMakeFiles/LocalBarrier.dir/src/LocalBarrier/localbarrier.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LocalBarrier.dir/src/LocalBarrier/localbarrier.cpp.i"
	cd /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/threads && /usr/local/bin/mpic++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/threads/src/LocalBarrier/localbarrier.cpp > CMakeFiles/LocalBarrier.dir/src/LocalBarrier/localbarrier.cpp.i

threads/CMakeFiles/LocalBarrier.dir/src/LocalBarrier/localbarrier.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LocalBarrier.dir/src/LocalBarrier/localbarrier.cpp.s"
	cd /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/threads && /usr/local/bin/mpic++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/threads/src/LocalBarrier/localbarrier.cpp -o CMakeFiles/LocalBarrier.dir/src/LocalBarrier/localbarrier.cpp.s

threads/CMakeFiles/LocalBarrier.dir/src/LocalBarrier/localbarrier.cpp.o.requires:
.PHONY : threads/CMakeFiles/LocalBarrier.dir/src/LocalBarrier/localbarrier.cpp.o.requires

threads/CMakeFiles/LocalBarrier.dir/src/LocalBarrier/localbarrier.cpp.o.provides: threads/CMakeFiles/LocalBarrier.dir/src/LocalBarrier/localbarrier.cpp.o.requires
	$(MAKE) -f threads/CMakeFiles/LocalBarrier.dir/build.make threads/CMakeFiles/LocalBarrier.dir/src/LocalBarrier/localbarrier.cpp.o.provides.build
.PHONY : threads/CMakeFiles/LocalBarrier.dir/src/LocalBarrier/localbarrier.cpp.o.provides

threads/CMakeFiles/LocalBarrier.dir/src/LocalBarrier/localbarrier.cpp.o.provides.build: threads/CMakeFiles/LocalBarrier.dir/src/LocalBarrier/localbarrier.cpp.o

# Object files for target LocalBarrier
LocalBarrier_OBJECTS = \
"CMakeFiles/LocalBarrier.dir/src/LocalBarrier/localbarrier.cpp.o"

# External object files for target LocalBarrier
LocalBarrier_EXTERNAL_OBJECTS =

threads/libLocalBarrier.so: threads/CMakeFiles/LocalBarrier.dir/src/LocalBarrier/localbarrier.cpp.o
threads/libLocalBarrier.so: threads/CMakeFiles/LocalBarrier.dir/build.make
threads/libLocalBarrier.so: threads/CMakeFiles/LocalBarrier.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX shared module libLocalBarrier.so"
	cd /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/threads && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/LocalBarrier.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
threads/CMakeFiles/LocalBarrier.dir/build: threads/libLocalBarrier.so
.PHONY : threads/CMakeFiles/LocalBarrier.dir/build

threads/CMakeFiles/LocalBarrier.dir/requires: threads/CMakeFiles/LocalBarrier.dir/src/LocalBarrier/localbarrier.cpp.o.requires
.PHONY : threads/CMakeFiles/LocalBarrier.dir/requires

threads/CMakeFiles/LocalBarrier.dir/clean:
	cd /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/threads && $(CMAKE_COMMAND) -P CMakeFiles/LocalBarrier.dir/cmake_clean.cmake
.PHONY : threads/CMakeFiles/LocalBarrier.dir/clean

threads/CMakeFiles/LocalBarrier.dir/depend:
	cd /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/threads /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/threads /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/threads/CMakeFiles/LocalBarrier.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : threads/CMakeFiles/LocalBarrier.dir/depend

