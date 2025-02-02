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
CMAKE_SOURCE_DIR = /home/josh/cmpt433/work/as1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/josh/cmpt433/work/as1/build

# Include any dependencies generated for this target.
include app/CMakeFiles/reaction_timer.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include app/CMakeFiles/reaction_timer.dir/compiler_depend.make

# Include the progress variables for this target.
include app/CMakeFiles/reaction_timer.dir/progress.make

# Include the compile flags for this target's objects.
include app/CMakeFiles/reaction_timer.dir/flags.make

app/CMakeFiles/reaction_timer.dir/src/main.c.o: app/CMakeFiles/reaction_timer.dir/flags.make
app/CMakeFiles/reaction_timer.dir/src/main.c.o: /home/josh/cmpt433/work/as1/app/src/main.c
app/CMakeFiles/reaction_timer.dir/src/main.c.o: app/CMakeFiles/reaction_timer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/josh/cmpt433/work/as1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object app/CMakeFiles/reaction_timer.dir/src/main.c.o"
	cd /home/josh/cmpt433/work/as1/build/app && /usr/bin/aarch64-linux-gnu-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT app/CMakeFiles/reaction_timer.dir/src/main.c.o -MF CMakeFiles/reaction_timer.dir/src/main.c.o.d -o CMakeFiles/reaction_timer.dir/src/main.c.o -c /home/josh/cmpt433/work/as1/app/src/main.c

app/CMakeFiles/reaction_timer.dir/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/reaction_timer.dir/src/main.c.i"
	cd /home/josh/cmpt433/work/as1/build/app && /usr/bin/aarch64-linux-gnu-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/josh/cmpt433/work/as1/app/src/main.c > CMakeFiles/reaction_timer.dir/src/main.c.i

app/CMakeFiles/reaction_timer.dir/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/reaction_timer.dir/src/main.c.s"
	cd /home/josh/cmpt433/work/as1/build/app && /usr/bin/aarch64-linux-gnu-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/josh/cmpt433/work/as1/app/src/main.c -o CMakeFiles/reaction_timer.dir/src/main.c.s

# Object files for target reaction_timer
reaction_timer_OBJECTS = \
"CMakeFiles/reaction_timer.dir/src/main.c.o"

# External object files for target reaction_timer
reaction_timer_EXTERNAL_OBJECTS =

app/reaction_timer: app/CMakeFiles/reaction_timer.dir/src/main.c.o
app/reaction_timer: app/CMakeFiles/reaction_timer.dir/build.make
app/reaction_timer: hal/libhal.a
app/reaction_timer: app/CMakeFiles/reaction_timer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/josh/cmpt433/work/as1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable reaction_timer"
	cd /home/josh/cmpt433/work/as1/build/app && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/reaction_timer.dir/link.txt --verbose=$(VERBOSE)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Copying ARM executable to public NFS directory"
	cd /home/josh/cmpt433/work/as1/build/app && /usr/bin/cmake -E copy /home/josh/cmpt433/work/as1/build/app/reaction_timer ~/cmpt433/public/myApps/reaction_timer

# Rule to build all files generated by this target.
app/CMakeFiles/reaction_timer.dir/build: app/reaction_timer
.PHONY : app/CMakeFiles/reaction_timer.dir/build

app/CMakeFiles/reaction_timer.dir/clean:
	cd /home/josh/cmpt433/work/as1/build/app && $(CMAKE_COMMAND) -P CMakeFiles/reaction_timer.dir/cmake_clean.cmake
.PHONY : app/CMakeFiles/reaction_timer.dir/clean

app/CMakeFiles/reaction_timer.dir/depend:
	cd /home/josh/cmpt433/work/as1/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/josh/cmpt433/work/as1 /home/josh/cmpt433/work/as1/app /home/josh/cmpt433/work/as1/build /home/josh/cmpt433/work/as1/build/app /home/josh/cmpt433/work/as1/build/app/CMakeFiles/reaction_timer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : app/CMakeFiles/reaction_timer.dir/depend

