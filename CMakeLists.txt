cmake_minimum_required(VERSION 3.19)
project(lab)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(third_party)
add_subdirectory(render_engine)
add_subdirectory(shaders)
add_subdirectory(app)