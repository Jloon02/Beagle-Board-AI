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

# Utility rule file for attach.

# Include any custom commands dependencies for this target.
include zephyr/cmake/flash/CMakeFiles/attach.dir/compiler_depend.make

# Include the progress variables for this target.
include zephyr/cmake/flash/CMakeFiles/attach.dir/progress.make

zephyr/cmake/flash/CMakeFiles/attach:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/josh/cmpt433/work/blinky/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Debugging beagley_ai"
	/usr/bin/cmake -DTARGET=attach -DDEPENDENCIES="" -P /home/josh/zephyrproject/zephyr/cmake/flash/check_runner_dependencies.cmake
	/usr/bin/cmake -E env /home/josh/zephyrproject/.venv/bin/python -m west attach

attach: zephyr/cmake/flash/CMakeFiles/attach
attach: zephyr/cmake/flash/CMakeFiles/attach.dir/build.make
.PHONY : attach

# Rule to build all files generated by this target.
zephyr/cmake/flash/CMakeFiles/attach.dir/build: attach
.PHONY : zephyr/cmake/flash/CMakeFiles/attach.dir/build

zephyr/cmake/flash/CMakeFiles/attach.dir/clean:
	cd /home/josh/cmpt433/work/blinky/build/zephyr/cmake/flash && $(CMAKE_COMMAND) -P CMakeFiles/attach.dir/cmake_clean.cmake
.PHONY : zephyr/cmake/flash/CMakeFiles/attach.dir/clean

zephyr/cmake/flash/CMakeFiles/attach.dir/depend:
	cd /home/josh/cmpt433/work/blinky/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/josh/cmpt433/work/blinky /home/josh/zephyrproject/zephyr/cmake/flash /home/josh/cmpt433/work/blinky/build /home/josh/cmpt433/work/blinky/build/zephyr/cmake/flash /home/josh/cmpt433/work/blinky/build/zephyr/cmake/flash/CMakeFiles/attach.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : zephyr/cmake/flash/CMakeFiles/attach.dir/depend

