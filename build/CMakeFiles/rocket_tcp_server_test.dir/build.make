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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/chris/myspace/rocket

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/chris/myspace/rocket/build

# Include any dependencies generated for this target.
include CMakeFiles/rocket_tcp_server_test.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/rocket_tcp_server_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/rocket_tcp_server_test.dir/flags.make

CMakeFiles/rocket_tcp_server_test.dir/examples/tcp_server.cc.o: CMakeFiles/rocket_tcp_server_test.dir/flags.make
CMakeFiles/rocket_tcp_server_test.dir/examples/tcp_server.cc.o: ../examples/tcp_server.cc
	$(CMAKE_COMMAND) -E cmake_progress_report /home/chris/myspace/rocket/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/rocket_tcp_server_test.dir/examples/tcp_server.cc.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/rocket_tcp_server_test.dir/examples/tcp_server.cc.o -c /home/chris/myspace/rocket/examples/tcp_server.cc

CMakeFiles/rocket_tcp_server_test.dir/examples/tcp_server.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/rocket_tcp_server_test.dir/examples/tcp_server.cc.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/chris/myspace/rocket/examples/tcp_server.cc > CMakeFiles/rocket_tcp_server_test.dir/examples/tcp_server.cc.i

CMakeFiles/rocket_tcp_server_test.dir/examples/tcp_server.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/rocket_tcp_server_test.dir/examples/tcp_server.cc.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/chris/myspace/rocket/examples/tcp_server.cc -o CMakeFiles/rocket_tcp_server_test.dir/examples/tcp_server.cc.s

CMakeFiles/rocket_tcp_server_test.dir/examples/tcp_server.cc.o.requires:
.PHONY : CMakeFiles/rocket_tcp_server_test.dir/examples/tcp_server.cc.o.requires

CMakeFiles/rocket_tcp_server_test.dir/examples/tcp_server.cc.o.provides: CMakeFiles/rocket_tcp_server_test.dir/examples/tcp_server.cc.o.requires
	$(MAKE) -f CMakeFiles/rocket_tcp_server_test.dir/build.make CMakeFiles/rocket_tcp_server_test.dir/examples/tcp_server.cc.o.provides.build
.PHONY : CMakeFiles/rocket_tcp_server_test.dir/examples/tcp_server.cc.o.provides

CMakeFiles/rocket_tcp_server_test.dir/examples/tcp_server.cc.o.provides.build: CMakeFiles/rocket_tcp_server_test.dir/examples/tcp_server.cc.o

# Object files for target rocket_tcp_server_test
rocket_tcp_server_test_OBJECTS = \
"CMakeFiles/rocket_tcp_server_test.dir/examples/tcp_server.cc.o"

# External object files for target rocket_tcp_server_test
rocket_tcp_server_test_EXTERNAL_OBJECTS =

rocket_tcp_server_test: CMakeFiles/rocket_tcp_server_test.dir/examples/tcp_server.cc.o
rocket_tcp_server_test: librocket.so.1.0
rocket_tcp_server_test: CMakeFiles/rocket_tcp_server_test.dir/build.make
rocket_tcp_server_test: CMakeFiles/rocket_tcp_server_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable rocket_tcp_server_test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/rocket_tcp_server_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/rocket_tcp_server_test.dir/build: rocket_tcp_server_test
.PHONY : CMakeFiles/rocket_tcp_server_test.dir/build

CMakeFiles/rocket_tcp_server_test.dir/requires: CMakeFiles/rocket_tcp_server_test.dir/examples/tcp_server.cc.o.requires
.PHONY : CMakeFiles/rocket_tcp_server_test.dir/requires

CMakeFiles/rocket_tcp_server_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/rocket_tcp_server_test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/rocket_tcp_server_test.dir/clean

CMakeFiles/rocket_tcp_server_test.dir/depend:
	cd /home/chris/myspace/rocket/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/chris/myspace/rocket /home/chris/myspace/rocket /home/chris/myspace/rocket/build /home/chris/myspace/rocket/build /home/chris/myspace/rocket/build/CMakeFiles/rocket_tcp_server_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/rocket_tcp_server_test.dir/depend

