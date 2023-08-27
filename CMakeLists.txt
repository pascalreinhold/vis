cmake_minimum_required(VERSION 3.19)
project(lab)

if (MSVC)
    # warning level 4
    add_compile_options(/W4)
else()
    # additional warnings
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(CMAKE_CXX_STANDARD 20)
set(LAB_BINARY_DIR "${PROJECT_BINARY_DIR}/out")

add_subdirectory(third_party)
add_subdirectory(render_engine)
add_subdirectory(shaders)
add_subdirectory(app)