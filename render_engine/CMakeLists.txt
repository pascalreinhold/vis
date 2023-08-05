project(render_engine)

# build render_engine_lib
set(RENDER_ENGINE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(RENDER_ENGINE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

set(RENDER_ENGINE_SOURCES
        "${RENDER_ENGINE_SOURCE_DIR}/render_engine.cpp"
        "${RENDER_ENGINE_SOURCE_DIR}/window.cpp")
set(RENDER_ENGINE_HEADERS
        "${RENDER_ENGINE_INCLUDE_DIR}/render_engine.hpp"
        "${RENDER_ENGINE_INCLUDE_DIR}/window.hpp")

add_library(render_engine_lib STATIC "${RENDER_ENGINE_SOURCES}")
target_include_directories("render_engine_lib" PUBLIC "${RENDER_ENGINE_INCLUDE_DIR}")


# get vulkan
find_package(Vulkan REQUIRED)
target_link_libraries(render_engine_lib Vulkan::Vulkan)
target_include_directories(render_engine_lib PUBLIC ${Vulkan_INCLUDE_DIRS})

# get glfw
