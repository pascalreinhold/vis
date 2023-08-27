#version 450 core

layout(location = 0) in vec3 in_position_model;
layout(location = 1) in vec3 in_normal_model;
layout(location = 2) in vec2 in_texcoord;

layout(location = 0) out vec3 out_position_world;
layout(location = 1) out vec3 out_normal_world;
layout(location = 2) out vec3 out_albedo;

// push constant
layout(push_constant) uniform PushConstants {
    mat4 view_projection_matrix;
} push;

void main() {
    out_position_world = in_position_model;
    out_normal_world = in_normal_model;
    out_albedo = in_normal_model;
    gl_Position = push.view_projection_matrix * vec4(out_position_world, 1.0);
}
