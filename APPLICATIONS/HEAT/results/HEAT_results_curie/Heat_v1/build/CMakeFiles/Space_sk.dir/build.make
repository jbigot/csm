# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_COMMAND = /ccc/cont003/home/gen7317/richarje/use-local/install/cmake/bin/cmake

# The command to remove a file.
RM = /ccc/cont003/home/gen7317/richarje/use-local/install/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /ccc/cont003/home/gen7317/richarje/use-local/install/cmake/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /ccc/cont003/home/gen7317/richarje/helene/HEAT/Heat_v1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /ccc/cont003/home/gen7317/richarje/helene/HEAT/Heat_v1/build

# Include any dependencies generated for this target.
include CMakeFiles/Space_sk.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Space_sk.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Space_sk.dir/flags.make

CMakeFiles/Space_sk.dir/src/Space/Space_sk.cpp.o: CMakeFiles/Space_sk.dir/flags.make
CMakeFiles/Space_sk.dir/src/Space/Space_sk.cpp.o: ../src/Space/Space_sk.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /ccc/cont003/home/gen7317/richarje/helene/HEAT/Heat_v1/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/Space_sk.dir/src/Space/Space_sk.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Space_sk.dir/src/Space/Space_sk.cpp.o -c /ccc/cont003/home/gen7317/richarje/helene/HEAT/Heat_v1/src/Space/Space_sk.cpp

CMakeFiles/Space_sk.dir/src/Space/Space_sk.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Space_sk.dir/src/Space/Space_sk.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /ccc/cont003/home/gen7317/richarje/helene/HEAT/Heat_v1/src/Space/Space_sk.cpp > CMakeFiles/Space_sk.dir/src/Space/Space_sk.cpp.i

CMakeFiles/Space_sk.dir/src/Space/Space_sk.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Space_sk.dir/src/Space/Space_sk.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /ccc/cont003/home/gen7317/richarje/helene/HEAT/Heat_v1/src/Space/Space_sk.cpp -o CMakeFiles/Space_sk.dir/src/Space/Space_sk.cpp.s

CMakeFiles/Space_sk.dir/src/Space/Space_sk.cpp.o.requires:
.PHONY : CMakeFiles/Space_sk.dir/src/Space/Space_sk.cpp.o.requires

CMakeFiles/Space_sk.dir/src/Space/Space_sk.cpp.o.provides: CMakeFiles/Space_sk.dir/src/Space/Space_sk.cpp.o.requires
	$(MAKE) -f CMakeFiles/Space_sk.dir/build.make CMakeFiles/Space_sk.dir/src/Space/Space_sk.cpp.o.provides.build
.PHONY : CMakeFiles/Space_sk.dir/src/Space/Space_sk.cpp.o.provides

CMakeFiles/Space_sk.dir/src/Space/Space_sk.cpp.o.provides.build: CMakeFiles/Space_sk.dir/src/Space/Space_sk.cpp.o

# Object files for target Space_sk
Space_sk_OBJECTS = \
"CMakeFiles/Space_sk.dir/src/Space/Space_sk.cpp.o"

# External object files for target Space_sk
Space_sk_EXTERNAL_OBJECTS =

libSpace_sk.so: CMakeFiles/Space_sk.dir/src/Space/Space_sk.cpp.o
libSpace_sk.so: CMakeFiles/Space_sk.dir/build.make
libSpace_sk.so: /opt/mpi/bullxmpi/1.2.7.2/lib/libmpi_cxx.so
libSpace_sk.so: /opt/mpi/bullxmpi/1.2.7.2/lib/libmpi.so
libSpace_sk.so: /usr/lib64/libdl.so
libSpace_sk.so: /usr/lib64/libm.so
libSpace_sk.so: /usr/lib64/libnuma.so
libSpace_sk.so: /usr/lib64/librt.so
libSpace_sk.so: /usr/lib64/libnsl.so
libSpace_sk.so: /usr/lib64/libutil.so
libSpace_sk.so: /usr/lib64/libm.so
libSpace_sk.so: /usr/lib64/libdl.so
libSpace_sk.so: /usr/lib64/libnuma.so
libSpace_sk.so: /usr/lib64/librt.so
libSpace_sk.so: /usr/lib64/libnsl.so
libSpace_sk.so: /usr/lib64/libutil.so
libSpace_sk.so: CMakeFiles/Space_sk.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX shared module libSpace_sk.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Space_sk.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Space_sk.dir/build: libSpace_sk.so
.PHONY : CMakeFiles/Space_sk.dir/build

CMakeFiles/Space_sk.dir/requires: CMakeFiles/Space_sk.dir/src/Space/Space_sk.cpp.o.requires
.PHONY : CMakeFiles/Space_sk.dir/requires

CMakeFiles/Space_sk.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Space_sk.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Space_sk.dir/clean

CMakeFiles/Space_sk.dir/depend:
	cd /ccc/cont003/home/gen7317/richarje/helene/HEAT/Heat_v1/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /ccc/cont003/home/gen7317/richarje/helene/HEAT/Heat_v1 /ccc/cont003/home/gen7317/richarje/helene/HEAT/Heat_v1 /ccc/cont003/home/gen7317/richarje/helene/HEAT/Heat_v1/build /ccc/cont003/home/gen7317/richarje/helene/HEAT/Heat_v1/build /ccc/cont003/home/gen7317/richarje/helene/HEAT/Heat_v1/build/CMakeFiles/Space_sk.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Space_sk.dir/depend
