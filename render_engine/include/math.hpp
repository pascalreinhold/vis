#pragma once

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_RADIANS
#define GLM_FORCE_LEFT_HANDED

#include "vulkan_include.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <vector>

namespace lab {
    using namespace glm;

    template<typename T>
    concept VertexInterface = requires(T v) {
        { v.position } -> std::same_as<vec3&>;
        { T::bindingDescriptions() } -> std::same_as<std::vector<vk::VertexInputBindingDescription>>; // usually only one
        { T::attributeDescriptions() } -> std::same_as<std::vector<vk::VertexInputAttributeDescription>>;
    };

    template<typename T>
    concept VertexInterfaceWithNormal = VertexInterface<T> && requires(T v) {
        { v.normal } -> std::same_as<vec3&>;
    };

    template<typename T>
    concept VertexInterfaceWithTexCoord = VertexInterface<T> && requires(T v) {
        { v.tex_coord } -> std::same_as<vec2&>;
    };

    struct Vertex {
        vec3 position;
        vec3 normal;
        vec2 tex_coord;

        static std::vector<vk::VertexInputBindingDescription> bindingDescriptions(); // usually only one
        static std::vector<vk::VertexInputAttributeDescription> attributeDescriptions();
    };

    struct Transformation {
        vec3 position;
        vec3 yxz_euler_angle;
        float scale;
    };

} // namespace lab