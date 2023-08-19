#version 450 core

layout(location = 0) in vec3 in_position_model;
layout(location = 1) in vec3 in_normal_model;

layout(location = 0) out vec3 out_position_world;
layout(location = 1) out vec3 out_normal_world;

void main() {
    out_position_world = in_position_model;
    out_normal_world = in_normal_model;
    gl_Position = vec4(in_position_model, 1.0);
}
