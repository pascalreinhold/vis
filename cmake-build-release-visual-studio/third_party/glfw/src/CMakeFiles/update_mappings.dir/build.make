﻿# CMAKE generated file: DO NOT EDIT!
# Generated by "NMake Makefiles" Generator, CMake Version 3.26

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

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

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE
NULL=nul
!ENDIF
SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = C:\Users\ich\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\232.8660.186\bin\cmake\win\x64\bin\cmake.exe

# The command to remove a file.
RM = C:\Users\ich\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\232.8660.186\bin\cmake\win\x64\bin\cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\code\lab

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\code\lab\cmake-build-release-visual-studio

# Utility rule file for update_mappings.

# Include any custom commands dependencies for this target.
include third_party\glfw\src\CMakeFiles\update_mappings.dir\compiler_depend.make

# Include the progress variables for this target.
include third_party\glfw\src\CMakeFiles\update_mappings.dir\progress.make

third_party\glfw\src\CMakeFiles\update_mappings:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=C:\code\lab\cmake-build-release-visual-studio\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Updating gamepad mappings from upstream repository"
	cd C:\code\lab\third_party\glfw\src
	C:\Users\ich\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\232.8660.186\bin\cmake\win\x64\bin\cmake.exe -P C:/code/lab/third_party/glfw/CMake/GenerateMappings.cmake mappings.h.in mappings.h
	cd C:\code\lab\cmake-build-release-visual-studio

update_mappings: third_party\glfw\src\CMakeFiles\update_mappings
update_mappings: third_party\glfw\src\CMakeFiles\update_mappings.dir\build.make
.PHONY : update_mappings

# Rule to build all files generated by this target.
third_party\glfw\src\CMakeFiles\update_mappings.dir\build: update_mappings
.PHONY : third_party\glfw\src\CMakeFiles\update_mappings.dir\build

third_party\glfw\src\CMakeFiles\update_mappings.dir\clean:
	cd C:\code\lab\cmake-build-release-visual-studio\third_party\glfw\src
	$(CMAKE_COMMAND) -P CMakeFiles\update_mappings.dir\cmake_clean.cmake
	cd C:\code\lab\cmake-build-release-visual-studio
.PHONY : third_party\glfw\src\CMakeFiles\update_mappings.dir\clean

third_party\glfw\src\CMakeFiles\update_mappings.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" C:\code\lab C:\code\lab\third_party\glfw\src C:\code\lab\cmake-build-release-visual-studio C:\code\lab\cmake-build-release-visual-studio\third_party\glfw\src C:\code\lab\cmake-build-release-visual-studio\third_party\glfw\src\CMakeFiles\update_mappings.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : third_party\glfw\src\CMakeFiles\update_mappings.dir\depend
