#version 450 core

layout(location = 0) in vec3 in_position_world;
layout(location = 1) in vec3 in_normal_world;
layout(location = 2) in vec3 in_albedo;

layout(location = 0) out vec4 out_color;

void main () {
    out_color = vec4(in_albedo, 1.f);
}
