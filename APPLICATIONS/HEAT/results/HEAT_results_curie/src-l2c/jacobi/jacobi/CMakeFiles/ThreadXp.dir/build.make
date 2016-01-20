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
include jacobi/CMakeFiles/ThreadXp.dir/depend.make

# Include the progress variables for this target.
include jacobi/CMakeFiles/ThreadXp.dir/progress.make

# Include the compile flags for this target's objects.
include jacobi/CMakeFiles/ThreadXp.dir/flags.make

jacobi/CMakeFiles/ThreadXp.dir/src/ThreadXp/threadxp.cpp.o: jacobi/CMakeFiles/ThreadXp.dir/flags.make
jacobi/CMakeFiles/ThreadXp.dir/src/ThreadXp/threadxp.cpp.o: src/ThreadXp/threadxp.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object jacobi/CMakeFiles/ThreadXp.dir/src/ThreadXp/threadxp.cpp.o"
	cd /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/jacobi && /usr/local/bin/mpic++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/ThreadXp.dir/src/ThreadXp/threadxp.cpp.o -c /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/src/ThreadXp/threadxp.cpp

jacobi/CMakeFiles/ThreadXp.dir/src/ThreadXp/threadxp.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ThreadXp.dir/src/ThreadXp/threadxp.cpp.i"
	cd /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/jacobi && /usr/local/bin/mpic++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/src/ThreadXp/threadxp.cpp > CMakeFiles/ThreadXp.dir/src/ThreadXp/threadxp.cpp.i

jacobi/CMakeFiles/ThreadXp.dir/src/ThreadXp/threadxp.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ThreadXp.dir/src/ThreadXp/threadxp.cpp.s"
	cd /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/jacobi && /usr/local/bin/mpic++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/src/ThreadXp/threadxp.cpp -o CMakeFiles/ThreadXp.dir/src/ThreadXp/threadxp.cpp.s

jacobi/CMakeFiles/ThreadXp.dir/src/ThreadXp/threadxp.cpp.o.requires:
.PHONY : jacobi/CMakeFiles/ThreadXp.dir/src/ThreadXp/threadxp.cpp.o.requires

jacobi/CMakeFiles/ThreadXp.dir/src/ThreadXp/threadxp.cpp.o.provides: jacobi/CMakeFiles/ThreadXp.dir/src/ThreadXp/threadxp.cpp.o.requires
	$(MAKE) -f jacobi/CMakeFiles/ThreadXp.dir/build.make jacobi/CMakeFiles/ThreadXp.dir/src/ThreadXp/threadxp.cpp.o.provides.build
.PHONY : jacobi/CMakeFiles/ThreadXp.dir/src/ThreadXp/threadxp.cpp.o.provides

jacobi/CMakeFiles/ThreadXp.dir/src/ThreadXp/threadxp.cpp.o.provides.build: jacobi/CMakeFiles/ThreadXp.dir/src/ThreadXp/threadxp.cpp.o

# Object files for target ThreadXp
ThreadXp_OBJECTS = \
"CMakeFiles/ThreadXp.dir/src/ThreadXp/threadxp.cpp.o"

# External object files for target ThreadXp
ThreadXp_EXTERNAL_OBJECTS =

jacobi/libThreadXp.so: jacobi/CMakeFiles/ThreadXp.dir/src/ThreadXp/threadxp.cpp.o
jacobi/libThreadXp.so: jacobi/CMakeFiles/ThreadXp.dir/build.make
jacobi/libThreadXp.so: /opt/local/lib/libboost_thread-mt.dylib
jacobi/libThreadXp.so: /opt/local/lib/libboost_system-mt.dylib
jacobi/libThreadXp.so: /opt/local/lib/libboost_filesystem-mt.dylib
jacobi/libThreadXp.so: jacobi/CMakeFiles/ThreadXp.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX shared module libThreadXp.so"
	cd /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/jacobi && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ThreadXp.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
jacobi/CMakeFiles/ThreadXp.dir/build: jacobi/libThreadXp.so
.PHONY : jacobi/CMakeFiles/ThreadXp.dir/build

jacobi/CMakeFiles/ThreadXp.dir/requires: jacobi/CMakeFiles/ThreadXp.dir/src/ThreadXp/threadxp.cpp.o.requires
.PHONY : jacobi/CMakeFiles/ThreadXp.dir/requires

jacobi/CMakeFiles/ThreadXp.dir/clean:
	cd /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/jacobi && $(CMAKE_COMMAND) -P CMakeFiles/ThreadXp.dir/cmake_clean.cmake
.PHONY : jacobi/CMakeFiles/ThreadXp.dir/clean

jacobi/CMakeFiles/ThreadXp.dir/depend:
	cd /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/jacobi /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/jacobi/CMakeFiles/ThreadXp.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : jacobi/CMakeFiles/ThreadXp.dir/depend

