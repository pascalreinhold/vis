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

# Include any dependencies generated for this target.
include third_party\CMakeFiles\stb_image.dir\depend.make
# Include any dependencies generated by the compiler for this target.
include third_party\CMakeFiles\stb_image.dir\compiler_depend.make

# Include the progress variables for this target.
include third_party\CMakeFiles\stb_image.dir\progress.make

# Include the compile flags for this target's objects.
include third_party\CMakeFiles\stb_image.dir\flags.make

third_party\CMakeFiles\stb_image.dir\stb\stb_image.cpp.obj: third_party\CMakeFiles\stb_image.dir\flags.make
third_party\CMakeFiles\stb_image.dir\stb\stb_image.cpp.obj: C:\code\lab\third_party\stb\stb_image.cpp
third_party\CMakeFiles\stb_image.dir\stb\stb_image.cpp.obj: third_party\CMakeFiles\stb_image.dir\compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\code\lab\cmake-build-release-visual-studio\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object third_party/CMakeFiles/stb_image.dir/stb/stb_image.cpp.obj"
	cd C:\code\lab\cmake-build-release-visual-studio\third_party
	$(CMAKE_COMMAND) -E cmake_cl_compile_depends --dep-file=CMakeFiles\stb_image.dir\stb\stb_image.cpp.obj.d --working-dir=C:\code\lab\cmake-build-release-visual-studio\third_party --filter-prefix="Note: including file: " -- C:\PROGRA~1\MICROS~3\2022\COMMUN~1\VC\Tools\MSVC\1437~1.328\bin\Hostx64\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /showIncludes /FoCMakeFiles\stb_image.dir\stb\stb_image.cpp.obj /FdCMakeFiles\stb_image.dir\stb_image.pdb /FS -c C:\code\lab\third_party\stb\stb_image.cpp
<<
	cd C:\code\lab\cmake-build-release-visual-studio

third_party\CMakeFiles\stb_image.dir\stb\stb_image.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/stb_image.dir/stb/stb_image.cpp.i"
	cd C:\code\lab\cmake-build-release-visual-studio\third_party
	C:\PROGRA~1\MICROS~3\2022\COMMUN~1\VC\Tools\MSVC\1437~1.328\bin\Hostx64\x64\cl.exe > CMakeFiles\stb_image.dir\stb\stb_image.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\code\lab\third_party\stb\stb_image.cpp
<<
	cd C:\code\lab\cmake-build-release-visual-studio

third_party\CMakeFiles\stb_image.dir\stb\stb_image.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/stb_image.dir/stb/stb_image.cpp.s"
	cd C:\code\lab\cmake-build-release-visual-studio\third_party
	C:\PROGRA~1\MICROS~3\2022\COMMUN~1\VC\Tools\MSVC\1437~1.328\bin\Hostx64\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\stb_image.dir\stb\stb_image.cpp.s /c C:\code\lab\third_party\stb\stb_image.cpp
<<
	cd C:\code\lab\cmake-build-release-visual-studio

# Object files for target stb_image
stb_image_OBJECTS = \
"CMakeFiles\stb_image.dir\stb\stb_image.cpp.obj"

# External object files for target stb_image
stb_image_EXTERNAL_OBJECTS =

third_party\stb_image.lib: third_party\CMakeFiles\stb_image.dir\stb\stb_image.cpp.obj
third_party\stb_image.lib: third_party\CMakeFiles\stb_image.dir\build.make
third_party\stb_image.lib: third_party\CMakeFiles\stb_image.dir\objects1.rsp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\code\lab\cmake-build-release-visual-studio\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library stb_image.lib"
	cd C:\code\lab\cmake-build-release-visual-studio\third_party
	$(CMAKE_COMMAND) -P CMakeFiles\stb_image.dir\cmake_clean_target.cmake
	cd C:\code\lab\cmake-build-release-visual-studio
	cd C:\code\lab\cmake-build-release-visual-studio\third_party
	C:\PROGRA~1\MICROS~3\2022\COMMUN~1\VC\Tools\MSVC\1437~1.328\bin\Hostx64\x64\lib.exe /nologo /machine:x64 /out:stb_image.lib @CMakeFiles\stb_image.dir\objects1.rsp 
	cd C:\code\lab\cmake-build-release-visual-studio

# Rule to build all files generated by this target.
third_party\CMakeFiles\stb_image.dir\build: third_party\stb_image.lib
.PHONY : third_party\CMakeFiles\stb_image.dir\build

third_party\CMakeFiles\stb_image.dir\clean:
	cd C:\code\lab\cmake-build-release-visual-studio\third_party
	$(CMAKE_COMMAND) -P CMakeFiles\stb_image.dir\cmake_clean.cmake
	cd C:\code\lab\cmake-build-release-visual-studio
.PHONY : third_party\CMakeFiles\stb_image.dir\clean

third_party\CMakeFiles\stb_image.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" C:\code\lab C:\code\lab\third_party C:\code\lab\cmake-build-release-visual-studio C:\code\lab\cmake-build-release-visual-studio\third_party C:\code\lab\cmake-build-release-visual-studio\third_party\CMakeFiles\stb_image.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : third_party\CMakeFiles\stb_image.dir\depend
