# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


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
CMAKE_COMMAND = /Applications/CMake.app/Contents/bin/cmake

# The command to remove a file.
RM = /Applications/CMake.app/Contents/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/Users/jackkilrain/Desktop/Work/Second-Year/COMP3600/Final Project/BSPMinimalRendering"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/Users/jackkilrain/Desktop/Work/Second-Year/COMP3600/Final Project/BSPMinimalRendering"

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/Applications/CMake.app/Contents/bin/ccmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/Applications/CMake.app/Contents/bin/cmake --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start "/Users/jackkilrain/Desktop/Work/Second-Year/COMP3600/Final Project/BSPMinimalRendering/CMakeFiles" "/Users/jackkilrain/Desktop/Work/Second-Year/COMP3600/Final Project/BSPMinimalRendering//CMakeFiles/progress.marks"
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start "/Users/jackkilrain/Desktop/Work/Second-Year/COMP3600/Final Project/BSPMinimalRendering/CMakeFiles" 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named run_tests_wo

# Build rule for target.
run_tests_wo: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 run_tests_wo
.PHONY : run_tests_wo

# fast build rule for target.
run_tests_wo/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run_tests_wo.dir/build.make CMakeFiles/run_tests_wo.dir/build
.PHONY : run_tests_wo/fast

#=============================================================================
# Target rules for targets named BSP

# Build rule for target.
BSP: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 BSP
.PHONY : BSP

# fast build rule for target.
BSP/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/BSP.dir/build.make CMakeFiles/BSP.dir/build
.PHONY : BSP/fast

#=============================================================================
# Target rules for targets named run_tests

# Build rule for target.
run_tests: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 run_tests
.PHONY : run_tests

# fast build rule for target.
run_tests/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run_tests.dir/build.make CMakeFiles/run_tests.dir/build
.PHONY : run_tests/fast

#=============================================================================
# Target rules for targets named build_tests

# Build rule for target.
build_tests: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 build_tests
.PHONY : build_tests

# fast build rule for target.
build_tests/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/build_tests.dir/build.make CMakeFiles/build_tests.dir/build
.PHONY : build_tests/fast

#=============================================================================
# Target rules for targets named archive_logs

# Build rule for target.
archive_logs: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 archive_logs
.PHONY : archive_logs

# fast build rule for target.
archive_logs/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/archive_logs.dir/build.make CMakeFiles/archive_logs.dir/build
.PHONY : archive_logs/fast

#=============================================================================
# Target rules for targets named clean_archives

# Build rule for target.
clean_archives: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 clean_archives
.PHONY : clean_archives

# fast build rule for target.
clean_archives/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/clean_archives.dir/build.make CMakeFiles/clean_archives.dir/build
.PHONY : clean_archives/fast

#=============================================================================
# Target rules for targets named clean_logs

# Build rule for target.
clean_logs: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 clean_logs
.PHONY : clean_logs

# fast build rule for target.
clean_logs/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/clean_logs.dir/build.make CMakeFiles/clean_logs.dir/build
.PHONY : clean_logs/fast

#=============================================================================
# Target rules for targets named destroy

# Build rule for target.
destroy: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 destroy
.PHONY : destroy

# fast build rule for target.
destroy/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/destroy.dir/build.make CMakeFiles/destroy.dir/build
.PHONY : destroy/fast

#=============================================================================
# Target rules for targets named run

# Build rule for target.
run: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 run
.PHONY : run

# fast build rule for target.
run/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/build
.PHONY : run/fast

#=============================================================================
# Target rules for targets named clean_out

# Build rule for target.
clean_out: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 clean_out
.PHONY : clean_out

# fast build rule for target.
clean_out/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/clean_out.dir/build.make CMakeFiles/clean_out.dir/build
.PHONY : clean_out/fast

src/raytracer/Raytracer.o: src/raytracer/Raytracer.cpp.o

.PHONY : src/raytracer/Raytracer.o

# target to build an object file
src/raytracer/Raytracer.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/BSP.dir/build.make CMakeFiles/BSP.dir/src/raytracer/Raytracer.cpp.o
.PHONY : src/raytracer/Raytracer.cpp.o

src/raytracer/Raytracer.i: src/raytracer/Raytracer.cpp.i

.PHONY : src/raytracer/Raytracer.i

# target to preprocess a source file
src/raytracer/Raytracer.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/BSP.dir/build.make CMakeFiles/BSP.dir/src/raytracer/Raytracer.cpp.i
.PHONY : src/raytracer/Raytracer.cpp.i

src/raytracer/Raytracer.s: src/raytracer/Raytracer.cpp.s

.PHONY : src/raytracer/Raytracer.s

# target to generate assembly for a file
src/raytracer/Raytracer.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/BSP.dir/build.make CMakeFiles/BSP.dir/src/raytracer/Raytracer.cpp.s
.PHONY : src/raytracer/Raytracer.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... archive_logs"
	@echo "... build_tests"
	@echo "... clean_archives"
	@echo "... clean_logs"
	@echo "... clean_out"
	@echo "... destroy"
	@echo "... run"
	@echo "... run_tests"
	@echo "... run_tests_wo"
	@echo "... BSP"
	@echo "... src/raytracer/Raytracer.o"
	@echo "... src/raytracer/Raytracer.i"
	@echo "... src/raytracer/Raytracer.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

