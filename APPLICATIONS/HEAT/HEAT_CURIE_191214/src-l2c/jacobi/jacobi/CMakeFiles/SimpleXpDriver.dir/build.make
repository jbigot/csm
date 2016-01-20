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
include jacobi/CMakeFiles/SimpleXpDriver.dir/depend.make

# Include the progress variables for this target.
include jacobi/CMakeFiles/SimpleXpDriver.dir/progress.make

# Include the compile flags for this target's objects.
include jacobi/CMakeFiles/SimpleXpDriver.dir/flags.make

jacobi/CMakeFiles/SimpleXpDriver.dir/src/SimpleXpDriver/simplexpdriver.cpp.o: jacobi/CMakeFiles/SimpleXpDriver.dir/flags.make
jacobi/CMakeFiles/SimpleXpDriver.dir/src/SimpleXpDriver/simplexpdriver.cpp.o: src/SimpleXpDriver/simplexpdriver.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object jacobi/CMakeFiles/SimpleXpDriver.dir/src/SimpleXpDriver/simplexpdriver.cpp.o"
	cd /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/jacobi && /usr/local/bin/mpic++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/SimpleXpDriver.dir/src/SimpleXpDriver/simplexpdriver.cpp.o -c /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/src/SimpleXpDriver/simplexpdriver.cpp

jacobi/CMakeFiles/SimpleXpDriver.dir/src/SimpleXpDriver/simplexpdriver.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SimpleXpDriver.dir/src/SimpleXpDriver/simplexpdriver.cpp.i"
	cd /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/jacobi && /usr/local/bin/mpic++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/src/SimpleXpDriver/simplexpdriver.cpp > CMakeFiles/SimpleXpDriver.dir/src/SimpleXpDriver/simplexpdriver.cpp.i

jacobi/CMakeFiles/SimpleXpDriver.dir/src/SimpleXpDriver/simplexpdriver.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SimpleXpDriver.dir/src/SimpleXpDriver/simplexpdriver.cpp.s"
	cd /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/jacobi && /usr/local/bin/mpic++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/src/SimpleXpDriver/simplexpdriver.cpp -o CMakeFiles/SimpleXpDriver.dir/src/SimpleXpDriver/simplexpdriver.cpp.s

jacobi/CMakeFiles/SimpleXpDriver.dir/src/SimpleXpDriver/simplexpdriver.cpp.o.requires:
.PHONY : jacobi/CMakeFiles/SimpleXpDriver.dir/src/SimpleXpDriver/simplexpdriver.cpp.o.requires

jacobi/CMakeFiles/SimpleXpDriver.dir/src/SimpleXpDriver/simplexpdriver.cpp.o.provides: jacobi/CMakeFiles/SimpleXpDriver.dir/src/SimpleXpDriver/simplexpdriver.cpp.o.requires
	$(MAKE) -f jacobi/CMakeFiles/SimpleXpDriver.dir/build.make jacobi/CMakeFiles/SimpleXpDriver.dir/src/SimpleXpDriver/simplexpdriver.cpp.o.provides.build
.PHONY : jacobi/CMakeFiles/SimpleXpDriver.dir/src/SimpleXpDriver/simplexpdriver.cpp.o.provides

jacobi/CMakeFiles/SimpleXpDriver.dir/src/SimpleXpDriver/simplexpdriver.cpp.o.provides.build: jacobi/CMakeFiles/SimpleXpDriver.dir/src/SimpleXpDriver/simplexpdriver.cpp.o

# Object files for target SimpleXpDriver
SimpleXpDriver_OBJECTS = \
"CMakeFiles/SimpleXpDriver.dir/src/SimpleXpDriver/simplexpdriver.cpp.o"

# External object files for target SimpleXpDriver
SimpleXpDriver_EXTERNAL_OBJECTS =

jacobi/libSimpleXpDriver.so: jacobi/CMakeFiles/SimpleXpDriver.dir/src/SimpleXpDriver/simplexpdriver.cpp.o
jacobi/libSimpleXpDriver.so: jacobi/CMakeFiles/SimpleXpDriver.dir/build.make
jacobi/libSimpleXpDriver.so: jacobi/CMakeFiles/SimpleXpDriver.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX shared module libSimpleXpDriver.so"
	cd /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/jacobi && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/SimpleXpDriver.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
jacobi/CMakeFiles/SimpleXpDriver.dir/build: jacobi/libSimpleXpDriver.so
.PHONY : jacobi/CMakeFiles/SimpleXpDriver.dir/build

jacobi/CMakeFiles/SimpleXpDriver.dir/requires: jacobi/CMakeFiles/SimpleXpDriver.dir/src/SimpleXpDriver/simplexpdriver.cpp.o.requires
.PHONY : jacobi/CMakeFiles/SimpleXpDriver.dir/requires

jacobi/CMakeFiles/SimpleXpDriver.dir/clean:
	cd /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/jacobi && $(CMAKE_COMMAND) -P CMakeFiles/SimpleXpDriver.dir/cmake_clean.cmake
.PHONY : jacobi/CMakeFiles/SimpleXpDriver.dir/clean

jacobi/CMakeFiles/SimpleXpDriver.dir/depend:
	cd /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/jacobi /Users/hcoullon/Documents/Postdoc/HLCM/examples/examples/src-l2c/jacobi/jacobi/CMakeFiles/SimpleXpDriver.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : jacobi/CMakeFiles/SimpleXpDriver.dir/depend

