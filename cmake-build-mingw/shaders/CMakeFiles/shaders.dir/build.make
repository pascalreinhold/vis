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

# Utility rule file for shaders.

# Include any custom commands dependencies for this target.
include shaders/CMakeFiles/shaders.dir/compiler_depend.make

# Include the progress variables for this target.
include shaders/CMakeFiles/shaders.dir/progress.make

shaders/CMakeFiles/shaders: C:/code/lab/shaders/default_mesh_shader.frag.spv
shaders/CMakeFiles/shaders: C:/code/lab/shaders/default_mesh_shader.vert.spv

C:/code/lab/shaders/default_mesh_shader.frag.spv: C:/code/lab/shaders/default_mesh_shader.frag
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=C:\code\lab\cmake-build-mingw\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating C:/code/lab/shaders/default_mesh_shader.frag.spv"
	cd /d C:\code\lab\cmake-build-mingw\shaders && glslc C:/code/lab/shaders//default_mesh_shader.frag -o C:/code/lab/shaders//default_mesh_shader.frag.spv

C:/code/lab/shaders/default_mesh_shader.vert.spv: C:/code/lab/shaders/default_mesh_shader.vert
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=C:\code\lab\cmake-build-mingw\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Generating C:/code/lab/shaders/default_mesh_shader.vert.spv"
	cd /d C:\code\lab\cmake-build-mingw\shaders && glslc C:/code/lab/shaders//default_mesh_shader.vert -o C:/code/lab/shaders//default_mesh_shader.vert.spv

shaders: shaders/CMakeFiles/shaders
shaders: C:/code/lab/shaders/default_mesh_shader.frag.spv
shaders: C:/code/lab/shaders/default_mesh_shader.vert.spv
shaders: shaders/CMakeFiles/shaders.dir/build.make
.PHONY : shaders

# Rule to build all files generated by this target.
shaders/CMakeFiles/shaders.dir/build: shaders
.PHONY : shaders/CMakeFiles/shaders.dir/build

shaders/CMakeFiles/shaders.dir/clean:
	cd /d C:\code\lab\cmake-build-mingw\shaders && $(CMAKE_COMMAND) -P CMakeFiles\shaders.dir\cmake_clean.cmake
.PHONY : shaders/CMakeFiles/shaders.dir/clean

shaders/CMakeFiles/shaders.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\code\lab C:\code\lab\shaders C:\code\lab\cmake-build-mingw C:\code\lab\cmake-build-mingw\shaders C:\code\lab\cmake-build-mingw\shaders\CMakeFiles\shaders.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : shaders/CMakeFiles/shaders.dir/depend
