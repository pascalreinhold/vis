# build glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/glfw") # the libraries target name is glfw
#target_include_directories(glfw PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/glfw/include")

# build vulkan memory allocator
# disable tests
set(VMA_BUILD_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/vulkan_memory_allocator")

# build glm
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/glm")
