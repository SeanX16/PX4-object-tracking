# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/xsh/catkin_ws/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/xsh/catkin_ws/build

# Utility rule file for ContinuousBuild.

# Include the progress variables for this target.
include Firmware/CMakeFiles/ContinuousBuild.dir/progress.make

Firmware/CMakeFiles/ContinuousBuild:
	cd /home/xsh/catkin_ws/build/Firmware && /usr/bin/ctest -D ContinuousBuild

ContinuousBuild: Firmware/CMakeFiles/ContinuousBuild
ContinuousBuild: Firmware/CMakeFiles/ContinuousBuild.dir/build.make

.PHONY : ContinuousBuild

# Rule to build all files generated by this target.
Firmware/CMakeFiles/ContinuousBuild.dir/build: ContinuousBuild

.PHONY : Firmware/CMakeFiles/ContinuousBuild.dir/build

Firmware/CMakeFiles/ContinuousBuild.dir/clean:
	cd /home/xsh/catkin_ws/build/Firmware && $(CMAKE_COMMAND) -P CMakeFiles/ContinuousBuild.dir/cmake_clean.cmake
.PHONY : Firmware/CMakeFiles/ContinuousBuild.dir/clean

Firmware/CMakeFiles/ContinuousBuild.dir/depend:
	cd /home/xsh/catkin_ws/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/xsh/catkin_ws/src /home/xsh/catkin_ws/src/Firmware /home/xsh/catkin_ws/build /home/xsh/catkin_ws/build/Firmware /home/xsh/catkin_ws/build/Firmware/CMakeFiles/ContinuousBuild.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Firmware/CMakeFiles/ContinuousBuild.dir/depend

