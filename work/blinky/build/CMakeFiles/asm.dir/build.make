# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/josh/cmpt433/work/blinky

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/josh/cmpt433/work/blinky/build

# Utility rule file for asm.

# Include any custom commands dependencies for this target.
include CMakeFiles/asm.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/asm.dir/progress.make

asm: CMakeFiles/asm.dir/build.make
.PHONY : asm

# Rule to build all files generated by this target.
CMakeFiles/asm.dir/build: asm
.PHONY : CMakeFiles/asm.dir/build

CMakeFiles/asm.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/asm.dir/cmake_clean.cmake
.PHONY : CMakeFiles/asm.dir/clean

CMakeFiles/asm.dir/depend:
	cd /home/josh/cmpt433/work/blinky/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/josh/cmpt433/work/blinky /home/josh/cmpt433/work/blinky /home/josh/cmpt433/work/blinky/build /home/josh/cmpt433/work/blinky/build /home/josh/cmpt433/work/blinky/build/CMakeFiles/asm.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/asm.dir/depend

