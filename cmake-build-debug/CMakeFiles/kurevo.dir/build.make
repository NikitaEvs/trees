# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


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
CMAKE_COMMAND = /snap/clion/92/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/92/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/hutu/trees

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/hutu/trees/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/kurevo.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/kurevo.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/kurevo.dir/flags.make

CMakeFiles/kurevo.dir/trees/kurevo.cpp.o: CMakeFiles/kurevo.dir/flags.make
CMakeFiles/kurevo.dir/trees/kurevo.cpp.o: ../trees/kurevo.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hutu/trees/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/kurevo.dir/trees/kurevo.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/kurevo.dir/trees/kurevo.cpp.o -c /home/hutu/trees/trees/kurevo.cpp

CMakeFiles/kurevo.dir/trees/kurevo.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/kurevo.dir/trees/kurevo.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/hutu/trees/trees/kurevo.cpp > CMakeFiles/kurevo.dir/trees/kurevo.cpp.i

CMakeFiles/kurevo.dir/trees/kurevo.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/kurevo.dir/trees/kurevo.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/hutu/trees/trees/kurevo.cpp -o CMakeFiles/kurevo.dir/trees/kurevo.cpp.s

# Object files for target kurevo
kurevo_OBJECTS = \
"CMakeFiles/kurevo.dir/trees/kurevo.cpp.o"

# External object files for target kurevo
kurevo_EXTERNAL_OBJECTS =

kurevo: CMakeFiles/kurevo.dir/trees/kurevo.cpp.o
kurevo: CMakeFiles/kurevo.dir/build.make
kurevo: CMakeFiles/kurevo.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/hutu/trees/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable kurevo"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/kurevo.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/kurevo.dir/build: kurevo

.PHONY : CMakeFiles/kurevo.dir/build

CMakeFiles/kurevo.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/kurevo.dir/cmake_clean.cmake
.PHONY : CMakeFiles/kurevo.dir/clean

CMakeFiles/kurevo.dir/depend:
	cd /home/hutu/trees/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/hutu/trees /home/hutu/trees /home/hutu/trees/cmake-build-debug /home/hutu/trees/cmake-build-debug /home/hutu/trees/cmake-build-debug/CMakeFiles/kurevo.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/kurevo.dir/depend
