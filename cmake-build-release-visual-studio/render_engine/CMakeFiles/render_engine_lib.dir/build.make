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
include render_engine\CMakeFiles\render_engine_lib.dir\depend.make
# Include any dependencies generated by the compiler for this target.
include render_engine\CMakeFiles\render_engine_lib.dir\compiler_depend.make

# Include the progress variables for this target.
include render_engine\CMakeFiles\render_engine_lib.dir\progress.make

# Include the compile flags for this target's objects.
include render_engine\CMakeFiles\render_engine_lib.dir\flags.make

render_engine\CMakeFiles\render_engine_lib.dir\src\camera.cpp.obj: render_engine\CMakeFiles\render_engine_lib.dir\flags.make
render_engine\CMakeFiles\render_engine_lib.dir\src\camera.cpp.obj: C:\code\lab\render_engine\src\camera.cpp
render_engine\CMakeFiles\render_engine_lib.dir\src\camera.cpp.obj: render_engine\CMakeFiles\render_engine_lib.dir\compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\code\lab\cmake-build-release-visual-studio\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object render_engine/CMakeFiles/render_engine_lib.dir/src/camera.cpp.obj"
	cd C:\code\lab\cmake-build-release-visual-studio\render_engine
	$(CMAKE_COMMAND) -E cmake_cl_compile_depends --dep-file=CMakeFiles\render_engine_lib.dir\src\camera.cpp.obj.d --working-dir=C:\code\lab\cmake-build-release-visual-studio\render_engine --filter-prefix="Note: including file: " -- C:\PROGRA~1\MICROS~3\2022\COMMUN~1\VC\Tools\MSVC\1437~1.328\bin\Hostx64\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /showIncludes /FoCMakeFiles\render_engine_lib.dir\src\camera.cpp.obj /FdCMakeFiles\render_engine_lib.dir\render_engine_lib.pdb /FS -c C:\code\lab\render_engine\src\camera.cpp
<<
	cd C:\code\lab\cmake-build-release-visual-studio

render_engine\CMakeFiles\render_engine_lib.dir\src\camera.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/render_engine_lib.dir/src/camera.cpp.i"
	cd C:\code\lab\cmake-build-release-visual-studio\render_engine
	C:\PROGRA~1\MICROS~3\2022\COMMUN~1\VC\Tools\MSVC\1437~1.328\bin\Hostx64\x64\cl.exe > CMakeFiles\render_engine_lib.dir\src\camera.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\code\lab\render_engine\src\camera.cpp
<<
	cd C:\code\lab\cmake-build-release-visual-studio

render_engine\CMakeFiles\render_engine_lib.dir\src\camera.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/render_engine_lib.dir/src/camera.cpp.s"
	cd C:\code\lab\cmake-build-release-visual-studio\render_engine
	C:\PROGRA~1\MICROS~3\2022\COMMUN~1\VC\Tools\MSVC\1437~1.328\bin\Hostx64\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\render_engine_lib.dir\src\camera.cpp.s /c C:\code\lab\render_engine\src\camera.cpp
<<
	cd C:\code\lab\cmake-build-release-visual-studio

render_engine\CMakeFiles\render_engine_lib.dir\src\error_logger.cpp.obj: render_engine\CMakeFiles\render_engine_lib.dir\flags.make
render_engine\CMakeFiles\render_engine_lib.dir\src\error_logger.cpp.obj: C:\code\lab\render_engine\src\error_logger.cpp
render_engine\CMakeFiles\render_engine_lib.dir\src\error_logger.cpp.obj: render_engine\CMakeFiles\render_engine_lib.dir\compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\code\lab\cmake-build-release-visual-studio\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object render_engine/CMakeFiles/render_engine_lib.dir/src/error_logger.cpp.obj"
	cd C:\code\lab\cmake-build-release-visual-studio\render_engine
	$(CMAKE_COMMAND) -E cmake_cl_compile_depends --dep-file=CMakeFiles\render_engine_lib.dir\src\error_logger.cpp.obj.d --working-dir=C:\code\lab\cmake-build-release-visual-studio\render_engine --filter-prefix="Note: including file: " -- C:\PROGRA~1\MICROS~3\2022\COMMUN~1\VC\Tools\MSVC\1437~1.328\bin\Hostx64\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /showIncludes /FoCMakeFiles\render_engine_lib.dir\src\error_logger.cpp.obj /FdCMakeFiles\render_engine_lib.dir\render_engine_lib.pdb /FS -c C:\code\lab\render_engine\src\error_logger.cpp
<<
	cd C:\code\lab\cmake-build-release-visual-studio

render_engine\CMakeFiles\render_engine_lib.dir\src\error_logger.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/render_engine_lib.dir/src/error_logger.cpp.i"
	cd C:\code\lab\cmake-build-release-visual-studio\render_engine
	C:\PROGRA~1\MICROS~3\2022\COMMUN~1\VC\Tools\MSVC\1437~1.328\bin\Hostx64\x64\cl.exe > CMakeFiles\render_engine_lib.dir\src\error_logger.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\code\lab\render_engine\src\error_logger.cpp
<<
	cd C:\code\lab\cmake-build-release-visual-studio

render_engine\CMakeFiles\render_engine_lib.dir\src\error_logger.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/render_engine_lib.dir/src/error_logger.cpp.s"
	cd C:\code\lab\cmake-build-release-visual-studio\render_engine
	C:\PROGRA~1\MICROS~3\2022\COMMUN~1\VC\Tools\MSVC\1437~1.328\bin\Hostx64\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\render_engine_lib.dir\src\error_logger.cpp.s /c C:\code\lab\render_engine\src\error_logger.cpp
<<
	cd C:\code\lab\cmake-build-release-visual-studio

render_engine\CMakeFiles\render_engine_lib.dir\src\graphics_device.cpp.obj: render_engine\CMakeFiles\render_engine_lib.dir\flags.make
render_engine\CMakeFiles\render_engine_lib.dir\src\graphics_device.cpp.obj: C:\code\lab\render_engine\src\graphics_device.cpp
render_engine\CMakeFiles\render_engine_lib.dir\src\graphics_device.cpp.obj: render_engine\CMakeFiles\render_engine_lib.dir\compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\code\lab\cmake-build-release-visual-studio\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object render_engine/CMakeFiles/render_engine_lib.dir/src/graphics_device.cpp.obj"
	cd C:\code\lab\cmake-build-release-visual-studio\render_engine
	$(CMAKE_COMMAND) -E cmake_cl_compile_depends --dep-file=CMakeFiles\render_engine_lib.dir\src\graphics_device.cpp.obj.d --working-dir=C:\code\lab\cmake-build-release-visual-studio\render_engine --filter-prefix="Note: including file: " -- C:\PROGRA~1\MICROS~3\2022\COMMUN~1\VC\Tools\MSVC\1437~1.328\bin\Hostx64\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /showIncludes /FoCMakeFiles\render_engine_lib.dir\src\graphics_device.cpp.obj /FdCMakeFiles\render_engine_lib.dir\render_engine_lib.pdb /FS -c C:\code\lab\render_engine\src\graphics_device.cpp
<<
	cd C:\code\lab\cmake-build-release-visual-studio

render_engine\CMakeFiles\render_engine_lib.dir\src\graphics_device.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/render_engine_lib.dir/src/graphics_device.cpp.i"
	cd C:\code\lab\cmake-build-release-visual-studio\render_engine
	C:\PROGRA~1\MICROS~3\2022\COMMUN~1\VC\Tools\MSVC\1437~1.328\bin\Hostx64\x64\cl.exe > CMakeFiles\render_engine_lib.dir\src\graphics_device.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\code\lab\render_engine\src\graphics_device.cpp
<<
	cd C:\code\lab\cmake-build-release-visual-studio

render_engine\CMakeFiles\render_engine_lib.dir\src\graphics_device.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/render_engine_lib.dir/src/graphics_device.cpp.s"
	cd C:\code\lab\cmake-build-release-visual-studio\render_engine
	C:\PROGRA~1\MICROS~3\2022\COMMUN~1\VC\Tools\MSVC\1437~1.328\bin\Hostx64\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\render_engine_lib.dir\src\graphics_device.cpp.s /c C:\code\lab\render_engine\src\graphics_device.cpp
<<
	cd C:\code\lab\cmake-build-release-visual-studio

render_engine\CMakeFiles\render_engine_lib.dir\src\graphics_pipeline_manager.cpp.obj: render_engine\CMakeFiles\render_engine_lib.dir\flags.make
render_engine\CMakeFiles\render_engine_lib.dir\src\graphics_pipeline_manager.cpp.obj: C:\code\lab\render_engine\src\graphics_pipeline_manager.cpp
render_engine\CMakeFiles\render_engine_lib.dir\src\graphics_pipeline_manager.cpp.obj: render_engine\CMakeFiles\render_engine_lib.dir\compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\code\lab\cmake-build-release-visual-studio\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object render_engine/CMakeFiles/render_engine_lib.dir/src/graphics_pipeline_manager.cpp.obj"
	cd C:\code\lab\cmake-build-release-visual-studio\render_engine
	$(CMAKE_COMMAND) -E cmake_cl_compile_depends --dep-file=CMakeFiles\render_engine_lib.dir\src\graphics_pipeline_manager.cpp.obj.d --working-dir=C:\code\lab\cmake-build-release-visual-studio\render_engine --filter-prefix="Note: including file: " -- C:\PROGRA~1\MICROS~3\2022\COMMUN~1\VC\Tools\MSVC\1437~1.328\bin\Hostx64\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /showIncludes /FoCMakeFiles\render_engine_lib.dir\src\graphics_pipeline_manager.cpp.obj /FdCMakeFiles\render_engine_lib.dir\render_engine_lib.pdb /FS -c C:\code\lab\render_engine\src\graphics_pipeline_manager.cpp
<<
	cd C:\code\lab\cmake-build-release-visual-studio

render_engine\CMakeFiles\render_engine_lib.dir\src\graphics_pipeline_manager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/render_engine_lib.dir/src/graphics_pipeline_manager.cpp.i"
	cd C:\code\lab\cmake-build-release-visual-studio\render_engine
	C:\PROGRA~1\MICROS~3\2022\COMMUN~1\VC\Tools\MSVC\1437~1.328\bin\Hostx64\x64\cl.exe > CMakeFiles\render_engine_lib.dir\src\graphics_pipeline_manager.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\code\lab\render_engine\src\graphics_pipeline_manager.cpp
<<
	cd C:\code\lab\cmake-build-release-visual-studio

render_engine\CMakeFiles\render_engine_lib.dir\src\graphics_pipeline_manager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/render_engine_lib.dir/src/graphics_pipeline_manager.cpp.s"
	cd C:\code\lab\cmake-build-release-visual-studio\render_engine
	C:\PROGRA~1\MICROS~3\2022\COMMUN~1\VC\Tools\MSVC\1437~1.328\bin\Hostx64\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\render_engine_lib.dir\src\graphics_pipeline_manager.cpp.s /c C:\code\lab\render_engine\src\graphics_pipeline_manager.cpp
<<
	cd C:\code\lab\cmake-build-release-visual-studio

render_engine\CMakeFiles\render_engine_lib.dir\src\gpu_resource_manager.cpp.obj: render_engine\CMakeFiles\render_engine_lib.dir\flags.make
render_engine\CMakeFiles\render_engine_lib.dir\src\gpu_resource_manager.cpp.obj: C:\code\lab\render_engine\src\gpu_resource_manager.cpp
render_engine\CMakeFiles\render_engine_lib.dir\src\gpu_resource_manager.cpp.obj: render_engine\CMakeFiles\render_engine_lib.dir\compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\code\lab\cmake-build-release-visual-studio\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object render_engine/CMakeFiles/render_engine_lib.dir/src/gpu_resource_manager.cpp.obj"
	cd C:\code\lab\cmake-build-release-visual-studio\render_engine
	$(CMAKE_COMMAND) -E cmake_cl_compile_depends --dep-file=CMakeFiles\render_engine_lib.dir\src\gpu_resource_manager.cpp.obj.d --working-dir=C:\code\lab\cmake-build-release-visual-studio\render_engine --filter-prefix="Note: including file: " -- C:\PROGRA~1\MICROS~3\2022\COMMUN~1\VC\Tools\MSVC\1437~1.328\bin\Hostx64\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /showIncludes /FoCMakeFiles\render_engine_lib.dir\src\gpu_resource_manager.cpp.obj /FdCMakeFiles\render_engine_lib.dir\render_engine_lib.pdb /FS -c C:\code\lab\render_engine\src\gpu_resource_manager.cpp
<<
	cd C:\code\lab\cmake-build-release-visual-studio

render_engine\CMakeFiles\render_engine_lib.dir\src\gpu_resource_manager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/render_engine_lib.dir/src/gpu_resource_manager.cpp.i"
	cd C:\code\lab\cmake-build-release-visual-studio\render_engine
	C:\PROGRA~1\MICROS~3\2022\COMMUN~1\VC\Tools\MSVC\1437~1.328\bin\Hostx64\x64\cl.exe > CMakeFiles\render_engine_lib.dir\src\gpu_resource_manager.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\code\lab\render_engine\src\gpu_resource_manager.cpp
<<
	cd C:\code\lab\cmake-build-release-visual-studio

render_engine\CMakeFiles\render_engine_lib.dir\src\gpu_resource_manager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/render_engine_lib.dir/src/gpu_resource_manager.cpp.s"
	cd C:\code\lab\cmake-build-release-visual-studio\render_engine
	C:\PROGRA~1\MICROS~3\2022\COMMUN~1\VC\Tools\MSVC\1437~1.328\bin\Hostx64\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\render_engine_lib.dir\src\gpu_resource_manager.cpp.s /c C:\code\lab\render_engine\src\gpu_resource_manager.cpp
<<
	cd C:\code\lab\cmake-build-release-visual-studio

render_engine\CMakeFiles\render_engine_lib.dir\src\math.cpp.obj: render_engine\CMakeFiles\render_engine_lib.dir\flags.make
render_engine\CMakeFiles\render_engine_lib.dir\src\math.cpp.obj: C:\code\lab\render_engine\src\math.cpp
render_engine\CMakeFiles\render_engine_lib.dir\src\math.cpp.obj: render_engine\CMakeFiles\render_engine_lib.dir\compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\code\lab\cmake-build-release-visual-studio\CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object render_engine/CMakeFiles/render_engine_lib.dir/src/math.cpp.obj"
	cd C:\code\lab\cmake-build-release-visual-studio\render_engine
	$(CMAKE_COMMAND) -E cmake_cl_compile_depends --dep-file=CMakeFiles\render_engine_lib.dir\src\math.cpp.obj.d --working-dir=C:\code\lab\cmake-build-release-visual-studio\render_engine --filter-prefix="Note: including file: " -- C:\PROGRA~1\MICROS~3\2022\COMMUN~1\VC\Tools\MSVC\1437~1.328\bin\Hostx64\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /showIncludes /FoCMakeFiles\render_engine_lib.dir\src\math.cpp.obj /FdCMakeFiles\render_engine_lib.dir\render_engine_lib.pdb /FS -c C:\code\lab\render_engine\src\math.cpp
<<
	cd C:\code\lab\cmake-build-release-visual-studio

render_engine\CMakeFiles\render_engine_lib.dir\src\math.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/render_engine_lib.dir/src/math.cpp.i"
	cd C:\code\lab\cmake-build-release-visual-studio\render_engine
	C:\PROGRA~1\MICROS~3\2022\COMMUN~1\VC\Tools\MSVC\1437~1.328\bin\Hostx64\x64\cl.exe > CMakeFiles\render_engine_lib.dir\src\math.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\code\lab\render_engine\src\math.cpp
<<
	cd C:\code\lab\cmake-build-release-visual-studio

render_engine\CMakeFiles\render_engine_lib.dir\src\math.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/render_engine_lib.dir/src/math.cpp.s"
	cd C:\code\lab\cmake-build-release-visual-studio\render_engine
	C:\PROGRA~1\MICROS~3\2022\COMMUN~1\VC\Tools\MSVC\1437~1.328\bin\Hostx64\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\render_engine_lib.dir\src\math.cpp.s /c C:\code\lab\render_engine\src\math.cpp
<<
	cd C:\code\lab\cmake-build-release-visual-studio

render_engine\CMakeFiles\render_engine_lib.dir\src\model.cpp.obj: render_engine\CMakeFiles\render_engine_lib.dir\flags.make
render_engine\CMakeFiles\render_engine_lib.dir\src\model.cpp.obj: C:\code\lab\render_engine\src\model.cpp
render_engine\CMakeFiles\render_engine_lib.dir\src\model.cpp.obj: render_engine\CMakeFiles\render_engine_lib.dir\compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\code\lab\cmake-build-release-visual-studio\CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object render_engine/CMakeFiles/render_engine_lib.dir/src/model.cpp.obj"
	cd C:\code\lab\cmake-build-release-visual-studio\render_engine
	$(CMAKE_COMMAND) -E cmake_cl_compile_depends --dep-file=CMakeFiles\render_engine_lib.dir\src\model.cpp.obj.d --working-dir=C:\code\lab\cmake-build-release-visual-studio\render_engine --filter-prefix="Note: including file: " -- C:\PROGRA~1\MICROS~3\2022\COMMUN~1\VC\Tools\MSVC\1437~1.328\bin\Hostx64\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /showIncludes /FoCMakeFiles\render_engine_lib.dir\src\model.cpp.obj /FdCMakeFiles\render_engine_lib.dir\render_engine_lib.pdb /FS -c C:\code\lab\render_engine\src\model.cpp
<<
	cd C:\code\lab\cmake-build-release-visual-studio

render_engine\CMakeFiles\render_engine_lib.dir\src\model.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/render_engine_lib.dir/src/model.cpp.i"
	cd C:\code\lab\cmake-build-release-visual-studio\render_engine
	C:\PROGRA~1\MICROS~3\2022\COMMUN~1\VC\Tools\MSVC\1437~1.328\bin\Hostx64\x64\cl.exe > CMakeFiles\render_engine_lib.dir\src\model.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\code\lab\render_engine\src\model.cpp
<<
	cd C:\code\lab\cmake-build-release-visual-studio

render_engine\CMakeFiles\render_engine_lib.dir\src\model.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/render_engine_lib.dir/src/model.cpp.s"
	cd C:\code\lab\cmake-build-release-visual-studio\render_engine
	C:\PROGRA~1\MICROS~3\2022\COMMUN~1\VC\Tools\MSVC\1437~1.328\bin\Hostx64\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\render_engine_lib.dir\src\model.cpp.s /c C:\code\lab\render_engine\src\model.cpp
<<
	cd C:\code\lab\cmake-build-release-visual-studio

render_engine\CMakeFiles\render_engine_lib.dir\src\render_engine.cpp.obj: render_engine\CMakeFiles\render_engine_lib.dir\flags.make
render_engine\CMakeFiles\render_engine_lib.dir\src\render_engine.cpp.obj: C:\code\lab\render_engine\src\render_engine.cpp
render_engine\CMakeFiles\render_engine_lib.dir\src\render_engine.cpp.obj: render_engine\CMakeFiles\render_engine_lib.dir\compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\code\lab\cmake-build-release-visual-studio\CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object render_engine/CMakeFiles/render_engine_lib.dir/src/render_engine.cpp.obj"
	cd C:\code\lab\cmake-build-release-visual-studio\render_engine
	$(CMAKE_COMMAND) -E cmake_cl_compile_depends --dep-file=CMakeFiles\render_engine_lib.dir\src\render_engine.cpp.obj.d --working-dir=C:\code\lab\cmake-build-release-visual-studio\render_engine --filter-prefix="Note: including file: " -- C:\PROGRA~1\MICROS~3\2022\COMMUN~1\VC\Tools\MSVC\1437~1.328\bin\Hostx64\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /showIncludes /FoCMakeFiles\render_engine_lib.dir\src\render_engine.cpp.obj /FdCMakeFiles\render_engine_lib.dir\render_engine_lib.pdb /FS -c C:\code\lab\render_engine\src\render_engine.cpp
<<
	cd C:\code\lab\cmake-build-release-visual-studio

render_engine\CMakeFiles\render_engine_lib.dir\src\render_engine.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/render_engine_lib.dir/src/render_engine.cpp.i"
	cd C:\code\lab\cmake-build-release-visual-studio\render_engine
	C:\PROGRA~1\MICROS~3\2022\COMMUN~1\VC\Tools\MSVC\1437~1.328\bin\Hostx64\x64\cl.exe > CMakeFiles\render_engine_lib.dir\src\render_engine.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\code\lab\render_engine\src\render_engine.cpp
<<
	cd C:\code\lab\cmake-build-release-visual-studio

render_engine\CMakeFiles\render_engine_lib.dir\src\render_engine.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/render_engine_lib.dir/src/render_engine.cpp.s"
	cd C:\code\lab\cmake-build-release-visual-studio\render_engine
	C:\PROGRA~1\MICROS~3\2022\COMMUN~1\VC\Tools\MSVC\1437~1.328\bin\Hostx64\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\render_engine_lib.dir\src\render_engine.cpp.s /c C:\code\lab\render_engine\src\render_engine.cpp
<<
	cd C:\code\lab\cmake-build-release-visual-studio

render_engine\CMakeFiles\render_engine_lib.dir\src\swapchain.cpp.obj: render_engine\CMakeFiles\render_engine_lib.dir\flags.make
render_engine\CMakeFiles\render_engine_lib.dir\src\swapchain.cpp.obj: C:\code\lab\render_engine\src\swapchain.cpp
render_engine\CMakeFiles\render_engine_lib.dir\src\swapchain.cpp.obj: render_engine\CMakeFiles\render_engine_lib.dir\compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\code\lab\cmake-build-release-visual-studio\CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object render_engine/CMakeFiles/render_engine_lib.dir/src/swapchain.cpp.obj"
	cd C:\code\lab\cmake-build-release-visual-studio\render_engine
	$(CMAKE_COMMAND) -E cmake_cl_compile_depends --dep-file=CMakeFiles\render_engine_lib.dir\src\swapchain.cpp.obj.d --working-dir=C:\code\lab\cmake-build-release-visual-studio\render_engine --filter-prefix="Note: including file: " -- C:\PROGRA~1\MICROS~3\2022\COMMUN~1\VC\Tools\MSVC\1437~1.328\bin\Hostx64\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /showIncludes /FoCMakeFiles\render_engine_lib.dir\src\swapchain.cpp.obj /FdCMakeFiles\render_engine_lib.dir\render_engine_lib.pdb /FS -c C:\code\lab\render_engine\src\swapchain.cpp
<<
	cd C:\code\lab\cmake-build-release-visual-studio

render_engine\CMakeFiles\render_engine_lib.dir\src\swapchain.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/render_engine_lib.dir/src/swapchain.cpp.i"
	cd C:\code\lab\cmake-build-release-visual-studio\render_engine
	C:\PROGRA~1\MICROS~3\2022\COMMUN~1\VC\Tools\MSVC\1437~1.328\bin\Hostx64\x64\cl.exe > CMakeFiles\render_engine_lib.dir\src\swapchain.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\code\lab\render_engine\src\swapchain.cpp
<<
	cd C:\code\lab\cmake-build-release-visual-studio

render_engine\CMakeFiles\render_engine_lib.dir\src\swapchain.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/render_engine_lib.dir/src/swapchain.cpp.s"
	cd C:\code\lab\cmake-build-release-visual-studio\render_engine
	C:\PROGRA~1\MICROS~3\2022\COMMUN~1\VC\Tools\MSVC\1437~1.328\bin\Hostx64\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\render_engine_lib.dir\src\swapchain.cpp.s /c C:\code\lab\render_engine\src\swapchain.cpp
<<
	cd C:\code\lab\cmake-build-release-visual-studio

render_engine\CMakeFiles\render_engine_lib.dir\src\vma_implementation.cpp.obj: render_engine\CMakeFiles\render_engine_lib.dir\flags.make
render_engine\CMakeFiles\render_engine_lib.dir\src\vma_implementation.cpp.obj: C:\code\lab\render_engine\src\vma_implementation.cpp
render_engine\CMakeFiles\render_engine_lib.dir\src\vma_implementation.cpp.obj: render_engine\CMakeFiles\render_engine_lib.dir\compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\code\lab\cmake-build-release-visual-studio\CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object render_engine/CMakeFiles/render_engine_lib.dir/src/vma_implementation.cpp.obj"
	cd C:\code\lab\cmake-build-release-visual-studio\render_engine
	$(CMAKE_COMMAND) -E cmake_cl_compile_depends --dep-file=CMakeFiles\render_engine_lib.dir\src\vma_implementation.cpp.obj.d --working-dir=C:\code\lab\cmake-build-release-visual-studio\render_engine --filter-prefix="Note: including file: " -- C:\PROGRA~1\MICROS~3\2022\COMMUN~1\VC\Tools\MSVC\1437~1.328\bin\Hostx64\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /showIncludes /FoCMakeFiles\render_engine_lib.dir\src\vma_implementation.cpp.obj /FdCMakeFiles\render_engine_lib.dir\render_engine_lib.pdb /FS -c C:\code\lab\render_engine\src\vma_implementation.cpp
<<
	cd C:\code\lab\cmake-build-release-visual-studio

render_engine\CMakeFiles\render_engine_lib.dir\src\vma_implementation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/render_engine_lib.dir/src/vma_implementation.cpp.i"
	cd C:\code\lab\cmake-build-release-visual-studio\render_engine
	C:\PROGRA~1\MICROS~3\2022\COMMUN~1\VC\Tools\MSVC\1437~1.328\bin\Hostx64\x64\cl.exe > CMakeFiles\render_engine_lib.dir\src\vma_implementation.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\code\lab\render_engine\src\vma_implementation.cpp
<<
	cd C:\code\lab\cmake-build-release-visual-studio

render_engine\CMakeFiles\render_engine_lib.dir\src\vma_implementation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/render_engine_lib.dir/src/vma_implementation.cpp.s"
	cd C:\code\lab\cmake-build-release-visual-studio\render_engine
	C:\PROGRA~1\MICROS~3\2022\COMMUN~1\VC\Tools\MSVC\1437~1.328\bin\Hostx64\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\render_engine_lib.dir\src\vma_implementation.cpp.s /c C:\code\lab\render_engine\src\vma_implementation.cpp
<<
	cd C:\code\lab\cmake-build-release-visual-studio

render_engine\CMakeFiles\render_engine_lib.dir\src\window.cpp.obj: render_engine\CMakeFiles\render_engine_lib.dir\flags.make
render_engine\CMakeFiles\render_engine_lib.dir\src\window.cpp.obj: C:\code\lab\render_engine\src\window.cpp
render_engine\CMakeFiles\render_engine_lib.dir\src\window.cpp.obj: render_engine\CMakeFiles\render_engine_lib.dir\compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\code\lab\cmake-build-release-visual-studio\CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object render_engine/CMakeFiles/render_engine_lib.dir/src/window.cpp.obj"
	cd C:\code\lab\cmake-build-release-visual-studio\render_engine
	$(CMAKE_COMMAND) -E cmake_cl_compile_depends --dep-file=CMakeFiles\render_engine_lib.dir\src\window.cpp.obj.d --working-dir=C:\code\lab\cmake-build-release-visual-studio\render_engine --filter-prefix="Note: including file: " -- C:\PROGRA~1\MICROS~3\2022\COMMUN~1\VC\Tools\MSVC\1437~1.328\bin\Hostx64\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /showIncludes /FoCMakeFiles\render_engine_lib.dir\src\window.cpp.obj /FdCMakeFiles\render_engine_lib.dir\render_engine_lib.pdb /FS -c C:\code\lab\render_engine\src\window.cpp
<<
	cd C:\code\lab\cmake-build-release-visual-studio

render_engine\CMakeFiles\render_engine_lib.dir\src\window.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/render_engine_lib.dir/src/window.cpp.i"
	cd C:\code\lab\cmake-build-release-visual-studio\render_engine
	C:\PROGRA~1\MICROS~3\2022\COMMUN~1\VC\Tools\MSVC\1437~1.328\bin\Hostx64\x64\cl.exe > CMakeFiles\render_engine_lib.dir\src\window.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\code\lab\render_engine\src\window.cpp
<<
	cd C:\code\lab\cmake-build-release-visual-studio

render_engine\CMakeFiles\render_engine_lib.dir\src\window.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/render_engine_lib.dir/src/window.cpp.s"
	cd C:\code\lab\cmake-build-release-visual-studio\render_engine
	C:\PROGRA~1\MICROS~3\2022\COMMUN~1\VC\Tools\MSVC\1437~1.328\bin\Hostx64\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\render_engine_lib.dir\src\window.cpp.s /c C:\code\lab\render_engine\src\window.cpp
<<
	cd C:\code\lab\cmake-build-release-visual-studio

# Object files for target render_engine_lib
render_engine_lib_OBJECTS = \
"CMakeFiles\render_engine_lib.dir\src\camera.cpp.obj" \
"CMakeFiles\render_engine_lib.dir\src\error_logger.cpp.obj" \
"CMakeFiles\render_engine_lib.dir\src\graphics_device.cpp.obj" \
"CMakeFiles\render_engine_lib.dir\src\graphics_pipeline_manager.cpp.obj" \
"CMakeFiles\render_engine_lib.dir\src\gpu_resource_manager.cpp.obj" \
"CMakeFiles\render_engine_lib.dir\src\math.cpp.obj" \
"CMakeFiles\render_engine_lib.dir\src\model.cpp.obj" \
"CMakeFiles\render_engine_lib.dir\src\render_engine.cpp.obj" \
"CMakeFiles\render_engine_lib.dir\src\swapchain.cpp.obj" \
"CMakeFiles\render_engine_lib.dir\src\vma_implementation.cpp.obj" \
"CMakeFiles\render_engine_lib.dir\src\window.cpp.obj"

# External object files for target render_engine_lib
render_engine_lib_EXTERNAL_OBJECTS =

render_engine\render_engine_lib.lib: render_engine\CMakeFiles\render_engine_lib.dir\src\camera.cpp.obj
render_engine\render_engine_lib.lib: render_engine\CMakeFiles\render_engine_lib.dir\src\error_logger.cpp.obj
render_engine\render_engine_lib.lib: render_engine\CMakeFiles\render_engine_lib.dir\src\graphics_device.cpp.obj
render_engine\render_engine_lib.lib: render_engine\CMakeFiles\render_engine_lib.dir\src\graphics_pipeline_manager.cpp.obj
render_engine\render_engine_lib.lib: render_engine\CMakeFiles\render_engine_lib.dir\src\gpu_resource_manager.cpp.obj
render_engine\render_engine_lib.lib: render_engine\CMakeFiles\render_engine_lib.dir\src\math.cpp.obj
render_engine\render_engine_lib.lib: render_engine\CMakeFiles\render_engine_lib.dir\src\model.cpp.obj
render_engine\render_engine_lib.lib: render_engine\CMakeFiles\render_engine_lib.dir\src\render_engine.cpp.obj
render_engine\render_engine_lib.lib: render_engine\CMakeFiles\render_engine_lib.dir\src\swapchain.cpp.obj
render_engine\render_engine_lib.lib: render_engine\CMakeFiles\render_engine_lib.dir\src\vma_implementation.cpp.obj
render_engine\render_engine_lib.lib: render_engine\CMakeFiles\render_engine_lib.dir\src\window.cpp.obj
render_engine\render_engine_lib.lib: render_engine\CMakeFiles\render_engine_lib.dir\build.make
render_engine\render_engine_lib.lib: render_engine\CMakeFiles\render_engine_lib.dir\objects1.rsp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\code\lab\cmake-build-release-visual-studio\CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Linking CXX static library render_engine_lib.lib"
	cd C:\code\lab\cmake-build-release-visual-studio\render_engine
	$(CMAKE_COMMAND) -P CMakeFiles\render_engine_lib.dir\cmake_clean_target.cmake
	cd C:\code\lab\cmake-build-release-visual-studio
	cd C:\code\lab\cmake-build-release-visual-studio\render_engine
	C:\PROGRA~1\MICROS~3\2022\COMMUN~1\VC\Tools\MSVC\1437~1.328\bin\Hostx64\x64\lib.exe /nologo /machine:x64 /out:render_engine_lib.lib @CMakeFiles\render_engine_lib.dir\objects1.rsp 
	cd C:\code\lab\cmake-build-release-visual-studio
	cd C:\code\lab\cmake-build-release-visual-studio\render_engine
	C:\Users\ich\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\232.8660.186\bin\cmake\win\x64\bin\cmake.exe -E copy_directory C:/code/lab/assets C:/code/lab/cmake-build-release-visual-studio/out/assets
	cd C:\code\lab\cmake-build-release-visual-studio

# Rule to build all files generated by this target.
render_engine\CMakeFiles\render_engine_lib.dir\build: render_engine\render_engine_lib.lib
.PHONY : render_engine\CMakeFiles\render_engine_lib.dir\build

render_engine\CMakeFiles\render_engine_lib.dir\clean:
	cd C:\code\lab\cmake-build-release-visual-studio\render_engine
	$(CMAKE_COMMAND) -P CMakeFiles\render_engine_lib.dir\cmake_clean.cmake
	cd C:\code\lab\cmake-build-release-visual-studio
.PHONY : render_engine\CMakeFiles\render_engine_lib.dir\clean

render_engine\CMakeFiles\render_engine_lib.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" C:\code\lab C:\code\lab\render_engine C:\code\lab\cmake-build-release-visual-studio C:\code\lab\cmake-build-release-visual-studio\render_engine C:\code\lab\cmake-build-release-visual-studio\render_engine\CMakeFiles\render_engine_lib.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : render_engine\CMakeFiles\render_engine_lib.dir\depend

