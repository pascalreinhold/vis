# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.26

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
CMAKE_BINARY_DIR = C:\code\lab\cmake-build-mingw

# Include any dependencies generated for this target.
include third_party/CMakeFiles/stb_image.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include third_party/CMakeFiles/stb_image.dir/compiler_depend.make

# Include the progress variables for this target.
include third_party/CMakeFiles/stb_image.dir/progress.make

# Include the compile flags for this target's objects.
include third_party/CMakeFiles/stb_image.dir/flags.make

third_party/CMakeFiles/stb_image.dir/stb/stb_image.cpp.obj: third_party/CMakeFiles/stb_image.dir/flags.make
third_party/CMakeFiles/stb_image.dir/stb/stb_image.cpp.obj: third_party/CMakeFiles/stb_image.dir/includes_CXX.rsp
third_party/CMakeFiles/stb_image.dir/stb/stb_image.cpp.obj: C:/code/lab/third_party/stb/stb_image.cpp
third_party/CMakeFiles/stb_image.dir/stb/stb_image.cpp.obj: third_party/CMakeFiles/stb_image.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\code\lab\cmake-build-mingw\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object third_party/CMakeFiles/stb_image.dir/stb/stb_image.cpp.obj"
	cd /d C:\code\lab\cmake-build-mingw\third_party && C:\Users\ich\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\232.8660.186\bin\mingw\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT third_party/CMakeFiles/stb_image.dir/stb/stb_image.cpp.obj -MF CMakeFiles\stb_image.dir\stb\stb_image.cpp.obj.d -o CMakeFiles\stb_image.dir\stb\stb_image.cpp.obj -c C:\code\lab\third_party\stb\stb_image.cpp

third_party/CMakeFiles/stb_image.dir/stb/stb_image.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/stb_image.dir/stb/stb_image.cpp.i"
	cd /d C:\code\lab\cmake-build-mingw\third_party && C:\Users\ich\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\232.8660.186\bin\mingw\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\code\lab\third_party\stb\stb_image.cpp > CMakeFiles\stb_image.dir\stb\stb_image.cpp.i

third_party/CMakeFiles/stb_image.dir/stb/stb_image.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/stb_image.dir/stb/stb_image.cpp.s"
	cd /d C:\code\lab\cmake-build-mingw\third_party && C:\Users\ich\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\232.8660.186\bin\mingw\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\code\lab\third_party\stb\stb_image.cpp -o CMakeFiles\stb_image.dir\stb\stb_image.cpp.s

# Object files for target stb_image
stb_image_OBJECTS = \
"CMakeFiles/stb_image.dir/stb/stb_image.cpp.obj"

# External object files for target stb_image
stb_image_EXTERNAL_OBJECTS =

third_party/libstb_imaged.a: third_party/CMakeFiles/stb_image.dir/stb/stb_image.cpp.obj
third_party/libstb_imaged.a: third_party/CMakeFiles/stb_image.dir/build.make
third_party/libstb_imaged.a: third_party/CMakeFiles/stb_image.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\code\lab\cmake-build-mingw\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libstb_imaged.a"
	cd /d C:\code\lab\cmake-build-mingw\third_party && $(CMAKE_COMMAND) -P CMakeFiles\stb_image.dir\cmake_clean_target.cmake
	cd /d C:\code\lab\cmake-build-mingw\third_party && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\stb_image.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
third_party/CMakeFiles/stb_image.dir/build: third_party/libstb_imaged.a
.PHONY : third_party/CMakeFiles/stb_image.dir/build

third_party/CMakeFiles/stb_image.dir/clean:
	cd /d C:\code\lab\cmake-build-mingw\third_party && $(CMAKE_COMMAND) -P CMakeFiles\stb_image.dir\cmake_clean.cmake
.PHONY : third_party/CMakeFiles/stb_image.dir/clean

third_party/CMakeFiles/stb_image.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\code\lab C:\code\lab\third_party C:\code\lab\cmake-build-mingw C:\code\lab\cmake-build-mingw\third_party C:\code\lab\cmake-build-mingw\third_party\CMakeFiles\stb_image.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : third_party/CMakeFiles/stb_image.dir/depend
