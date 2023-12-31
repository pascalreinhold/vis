# build render_engine_lib
set(RENDER_ENGINE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(RENDER_ENGINE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

set(RENDER_ENGINE_SOURCES
        "${RENDER_ENGINE_SOURCE_DIR}/camera.cpp"
        "${RENDER_ENGINE_SOURCE_DIR}/error_logger.cpp"
        "${RENDER_ENGINE_SOURCE_DIR}/graphics_device.cpp"
        "${RENDER_ENGINE_SOURCE_DIR}/graphics_pipeline_manager.cpp"
        "${RENDER_ENGINE_SOURCE_DIR}/gpu_resource_manager.cpp"
        "${RENDER_ENGINE_SOURCE_DIR}/math.cpp"
        "${RENDER_ENGINE_SOURCE_DIR}/model.cpp"
        "${RENDER_ENGINE_SOURCE_DIR}/render_engine.cpp"
        "${RENDER_ENGINE_SOURCE_DIR}/swapchain.cpp"
        "${RENDER_ENGINE_SOURCE_DIR}/vma_implementation.cpp"
        "${RENDER_ENGINE_SOURCE_DIR}/window.cpp")
set(RENDER_ENGINE_HEADERS
        "${RENDER_ENGINE_INCLUDE_DIR}/camera.hpp"
        "${RENDER_ENGINE_INCLUDE_DIR}/error_logger.hpp"
        "${RENDER_ENGINE_INCLUDE_DIR}/graphics_device.hpp"
        "${RENDER_ENGINE_INCLUDE_DIR}/graphics_pipeline_manager.hpp"
        "${RENDER_ENGINE_INCLUDE_DIR}/gpu_resource_manager.hpp"
        "${RENDER_ENGINE_INCLUDE_DIR}/handles.hpp"
        "${RENDER_ENGINE_INCLUDE_DIR}/math.hpp"
        "${RENDER_ENGINE_INCLUDE_DIR}/model.hpp"
        "${RENDER_ENGINE_INCLUDE_DIR}/render_engine.hpp"
        "${RENDER_ENGINE_INCLUDE_DIR}/swapchain.hpp"
        "${RENDER_ENGINE_INCLUDE_DIR}/utilities.hpp"
        "${RENDER_ENGINE_INCLUDE_DIR}/vulkan_include.hpp"
        "${RENDER_ENGINE_INCLUDE_DIR}/window.hpp")

add_library(render_engine_lib STATIC "${RENDER_ENGINE_SOURCES}")
target_include_directories(render_engine_lib PUBLIC "${RENDER_ENGINE_INCLUDE_DIR}")
target_compile_definitions(render_engine_lib PUBLIC VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1)
set_target_properties(render_engine_lib PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${LAB_BINARY_DIR}")

# copy assets to build directory
add_custom_command(TARGET render_engine_lib POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets" "${LAB_BINARY_DIR}/assets"
)

# vulkan
find_package(Vulkan REQUIRED)
target_link_libraries(render_engine_lib PUBLIC Vulkan::Vulkan)

# glfw [submodule]
target_link_libraries(render_engine_lib PUBLIC glfw)

# Vulkan Memory Allocator [submodule]
target_link_libraries(render_engine_lib PUBLIC VulkanMemoryAllocator)

# glm [submodule]
target_link_libraries(render_engine_lib PUBLIC glm)

# meshoptimizer [submodule]
target_link_libraries(render_engine_lib PUBLIC meshoptimizer)

# assimp [submodule]
target_link_libraries(render_engine_lib PUBLIC assimp)

# imgui [submodule]
target_link_libraries(render_engine_lib PUBLIC imgui)

# stb [submodule]
target_link_libraries(render_engine_lib PUBLIC stb)
