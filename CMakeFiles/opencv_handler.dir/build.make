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

# Escaping for special characters.
EQUALS = =

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ka/Desktop/414/Markless-AR-experiments

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ka/Desktop/414/Markless-AR-experiments

# Include any dependencies generated for this target.
include CMakeFiles/opencv_handler.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/opencv_handler.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/opencv_handler.dir/flags.make

CMakeFiles/opencv_handler.dir/src/opencv_handler.cpp.o: CMakeFiles/opencv_handler.dir/flags.make
CMakeFiles/opencv_handler.dir/src/opencv_handler.cpp.o: src/opencv_handler.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/ka/Desktop/414/Markless-AR-experiments/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/opencv_handler.dir/src/opencv_handler.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/opencv_handler.dir/src/opencv_handler.cpp.o -c /home/ka/Desktop/414/Markless-AR-experiments/src/opencv_handler.cpp

CMakeFiles/opencv_handler.dir/src/opencv_handler.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/opencv_handler.dir/src/opencv_handler.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/ka/Desktop/414/Markless-AR-experiments/src/opencv_handler.cpp > CMakeFiles/opencv_handler.dir/src/opencv_handler.cpp.i

CMakeFiles/opencv_handler.dir/src/opencv_handler.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/opencv_handler.dir/src/opencv_handler.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/ka/Desktop/414/Markless-AR-experiments/src/opencv_handler.cpp -o CMakeFiles/opencv_handler.dir/src/opencv_handler.cpp.s

CMakeFiles/opencv_handler.dir/src/opencv_handler.cpp.o.requires:
.PHONY : CMakeFiles/opencv_handler.dir/src/opencv_handler.cpp.o.requires

CMakeFiles/opencv_handler.dir/src/opencv_handler.cpp.o.provides: CMakeFiles/opencv_handler.dir/src/opencv_handler.cpp.o.requires
	$(MAKE) -f CMakeFiles/opencv_handler.dir/build.make CMakeFiles/opencv_handler.dir/src/opencv_handler.cpp.o.provides.build
.PHONY : CMakeFiles/opencv_handler.dir/src/opencv_handler.cpp.o.provides

CMakeFiles/opencv_handler.dir/src/opencv_handler.cpp.o.provides.build: CMakeFiles/opencv_handler.dir/src/opencv_handler.cpp.o

# Object files for target opencv_handler
opencv_handler_OBJECTS = \
"CMakeFiles/opencv_handler.dir/src/opencv_handler.cpp.o"

# External object files for target opencv_handler
opencv_handler_EXTERNAL_OBJECTS =

libopencv_handler.a: CMakeFiles/opencv_handler.dir/src/opencv_handler.cpp.o
libopencv_handler.a: CMakeFiles/opencv_handler.dir/build.make
libopencv_handler.a: CMakeFiles/opencv_handler.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX static library libopencv_handler.a"
	$(CMAKE_COMMAND) -P CMakeFiles/opencv_handler.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/opencv_handler.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/opencv_handler.dir/build: libopencv_handler.a
.PHONY : CMakeFiles/opencv_handler.dir/build

CMakeFiles/opencv_handler.dir/requires: CMakeFiles/opencv_handler.dir/src/opencv_handler.cpp.o.requires
.PHONY : CMakeFiles/opencv_handler.dir/requires

CMakeFiles/opencv_handler.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/opencv_handler.dir/cmake_clean.cmake
.PHONY : CMakeFiles/opencv_handler.dir/clean

CMakeFiles/opencv_handler.dir/depend:
	cd /home/ka/Desktop/414/Markless-AR-experiments && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ka/Desktop/414/Markless-AR-experiments /home/ka/Desktop/414/Markless-AR-experiments /home/ka/Desktop/414/Markless-AR-experiments /home/ka/Desktop/414/Markless-AR-experiments /home/ka/Desktop/414/Markless-AR-experiments/CMakeFiles/opencv_handler.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/opencv_handler.dir/depend

