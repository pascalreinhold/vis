SET(SHADER_SOURCE_DIR "${PROJECT_SOURCE_DIR}/shaders/")
SET(SHADER_BINARY_DIR "${PROJECT_SOURCE_DIR}/shaders/")

file(GLOB_RECURSE SHADER_SOURCES FILES
        "${SHADER_SOURCE_DIR}/*.vert"
        "${SHADER_SOURCE_DIR}/*.frag"
)

foreach(SHADER ${SHADER_SOURCES})
    get_filename_component(FILE_NAME ${SHADER} NAME)
    set(SHADER_OUTPUT "${SHADER_BINARY_DIR}/${FILE_NAME}.spv")
    add_custom_command(
            OUTPUT ${SHADER_OUTPUT}
            COMMAND glslc ${SHADER} -o ${SHADER_OUTPUT}
            DEPENDS ${SHADER}
    )
    list(APPEND SHADER_OUTPUTS ${SHADER_OUTPUT})
endforeach(SHADER)

add_custom_target(
        shaders
        DEPENDS ${SHADER_OUTPUTS}
)