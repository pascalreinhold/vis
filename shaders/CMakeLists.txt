SET(SHADER_SOURCE_DIR "${PROJECT_SOURCE_DIR}/shaders/")
SET(SHADER_BINARY_DIR "${LAB_BINARY_DIR}/shaders/")

# print all the different binary dirs
message(STATUS "PROJECT_BINARY_DIR: ${PROJECT_BINARY_DIR}")
message(STATUS "SHADER_BINARY_DIR: ${SHADER_BINARY_DIR}")
message(STATUS "CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")
message(STATUS "CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")

# create the binary dir
add_custom_command(
        OUTPUT ${SHADER_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${SHADER_BINARY_DIR}
)

# collect all the shaders
file(GLOB_RECURSE SHADER_SOURCES FILES
        "${SHADER_SOURCE_DIR}/*.vert"
        "${SHADER_SOURCE_DIR}/*.frag"
)

# compile all the shaders
foreach(SHADER ${SHADER_SOURCES})
    get_filename_component(FILE_NAME ${SHADER} NAME)
    set(SHADER_OUTPUT "${SHADER_BINARY_DIR}/${FILE_NAME}.spv")
    add_custom_command(
            OUTPUT ${SHADER_OUTPUT}
            COMMAND glslc ${SHADER} -o ${SHADER_OUTPUT}
            DEPENDS ${SHADER} ${SHADER_BINARY_DIR}
    )
    list(APPEND SHADER_OUTPUTS ${SHADER_OUTPUT})
endforeach(SHADER)

add_custom_target(
        shaders
        DEPENDS ${SHADER_OUTPUTS}
)