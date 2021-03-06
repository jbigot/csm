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
include jacobi/CMakeFiles/SeqDriver.dir/depend.make

# Include the progress variables for this target.
include jacobi/CMakeFiles/SeqDriver.dir/progress.make

# Include the compile flags for this target's objects.
include jacobi/CMakeFiles/SeqDriver.dir/flags.make

jacobi/CMakeFiles/SeqDriver.dir/src/SeqDriver/seqdriver.cpp.o: jacobi/CMakeFiles/SeqDriver.dir/flags.make
jacobi/CMakeFiles/SeqDriver.dir/src/SeqDriver/seqdriver.cpp.o: src/SeqDriver/seqdriver.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object jacobi/CMakeFiles/SeqDriver.dir/src/SeqDriver/seqdriver.cpp.o"
	cd /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/jacobi && /usr/local/bin/mpic++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/SeqDriver.dir/src/SeqDriver/seqdriver.cpp.o -c /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/src/SeqDriver/seqdriver.cpp

jacobi/CMakeFiles/SeqDriver.dir/src/SeqDriver/seqdriver.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SeqDriver.dir/src/SeqDriver/seqdriver.cpp.i"
	cd /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/jacobi && /usr/local/bin/mpic++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/src/SeqDriver/seqdriver.cpp > CMakeFiles/SeqDriver.dir/src/SeqDriver/seqdriver.cpp.i

jacobi/CMakeFiles/SeqDriver.dir/src/SeqDriver/seqdriver.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SeqDriver.dir/src/SeqDriver/seqdriver.cpp.s"
	cd /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/jacobi && /usr/local/bin/mpic++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/src/SeqDriver/seqdriver.cpp -o CMakeFiles/SeqDriver.dir/src/SeqDriver/seqdriver.cpp.s

jacobi/CMakeFiles/SeqDriver.dir/src/SeqDriver/seqdriver.cpp.o.requires:
.PHONY : jacobi/CMakeFiles/SeqDriver.dir/src/SeqDriver/seqdriver.cpp.o.requires

jacobi/CMakeFiles/SeqDriver.dir/src/SeqDriver/seqdriver.cpp.o.provides: jacobi/CMakeFiles/SeqDriver.dir/src/SeqDriver/seqdriver.cpp.o.requires
	$(MAKE) -f jacobi/CMakeFiles/SeqDriver.dir/build.make jacobi/CMakeFiles/SeqDriver.dir/src/SeqDriver/seqdriver.cpp.o.provides.build
.PHONY : jacobi/CMakeFiles/SeqDriver.dir/src/SeqDriver/seqdriver.cpp.o.provides

jacobi/CMakeFiles/SeqDriver.dir/src/SeqDriver/seqdriver.cpp.o.provides.build: jacobi/CMakeFiles/SeqDriver.dir/src/SeqDriver/seqdriver.cpp.o

# Object files for target SeqDriver
SeqDriver_OBJECTS = \
"CMakeFiles/SeqDriver.dir/src/SeqDriver/seqdriver.cpp.o"

# External object files for target SeqDriver
SeqDriver_EXTERNAL_OBJECTS =

jacobi/libSeqDriver.so: jacobi/CMakeFiles/SeqDriver.dir/src/SeqDriver/seqdriver.cpp.o
jacobi/libSeqDriver.so: jacobi/CMakeFiles/SeqDriver.dir/build.make
jacobi/libSeqDriver.so: jacobi/CMakeFiles/SeqDriver.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX shared module libSeqDriver.so"
	cd /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/jacobi && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/SeqDriver.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
jacobi/CMakeFiles/SeqDriver.dir/build: jacobi/libSeqDriver.so
.PHONY : jacobi/CMakeFiles/SeqDriver.dir/build

jacobi/CMakeFiles/SeqDriver.dir/requires: jacobi/CMakeFiles/SeqDriver.dir/src/SeqDriver/seqdriver.cpp.o.requires
.PHONY : jacobi/CMakeFiles/SeqDriver.dir/requires

jacobi/CMakeFiles/SeqDriver.dir/clean:
	cd /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/jacobi && $(CMAKE_COMMAND) -P CMakeFiles/SeqDriver.dir/cmake_clean.cmake
.PHONY : jacobi/CMakeFiles/SeqDriver.dir/clean

jacobi/CMakeFiles/SeqDriver.dir/depend:
	cd /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/jacobi /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/jacobi/CMakeFiles/SeqDriver.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : jacobi/CMakeFiles/SeqDriver.dir/depend

