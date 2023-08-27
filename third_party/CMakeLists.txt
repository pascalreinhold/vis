
# build glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/glfw") # the libraries target name is glfw

# build vulkan memory allocator
# disable tests
set(VMA_BUILD_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/vulkan_memory_allocator")

# build stb
add_library(stb "${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_image.cpp")
target_include_directories(stb PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/stb")

# build meshoptimizer
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/meshoptimizer")

# build glm
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/glm")

# build imgui
set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/imgui")
set(IMGUI_SOURCES
        "${IMGUI_DIR}/imgui.h"
        "${IMGUI_DIR}/imgui.cpp"
        "${IMGUI_DIR}/imgui_demo.cpp"
        "${IMGUI_DIR}/imgui_draw.cpp"
        "${IMGUI_DIR}/imgui_widgets.cpp"
        "${IMGUI_DIR}/imgui_tables.cpp"
        "${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp"
        "${IMGUI_DIR}/backends/imgui_impl_glfw.cpp"
)

add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC ${IMGUI_DIR} ${IMGUI_DIR}/backends)
find_package(Vulkan REQUIRED)
target_link_libraries(imgui PUBLIC glfw Vulkan::Vulkan)

# build assimp
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ZLIB ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE) # needed to use assimp as a submodule
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/assimp")

# ASSIMP_WARNINGS_AS_ERRORS (default ON): Treat all warnings as errors.
# ASSIMP_ASAN (default OFF): Enable AddressSanitizer.
# ASSIMP_UBSAN (default OFF): Enable Undefined Behavior sanitizer.
# ASSIMP_BUILD_DOCS (default OFF): Build documentation using Doxygen. OBSOLETE, see https://github.com/assimp/assimp-docs
# ASSIMP_INJECT_DEBUG_POSTFIX (default ON): Inject debug postfix in .a/.so/.lib/.dll lib names
# ASSIMP_INSTALL_PDB (default ON): Install MSVC debug files.
# ASSIMP_BUILD_DRACO (default OFF): Build Draco libraries. Primarily for glTF.
