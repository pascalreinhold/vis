add_executable(render_engine_app main.cpp)

target_link_libraries(render_engine_app render_engine_lib)

add_dependencies(render_engine_app shaders)
