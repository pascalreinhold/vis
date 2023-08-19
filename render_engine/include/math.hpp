#pragma once

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_RADIANS
#define GLM_FORCE_LEFT_HANDED

#include <glm/glm.hpp>
#include <vulkan_types.hpp>
#include <vector>

namespace lab {
    using namespace glm;

    template<typename T>
    concept VertexInterface = requires(T v) {
        { v.position } -> std::same_as<vec3&>;
        { T::bindingDescriptions() } -> std::same_as<std::vector<vk::VertexInputBindingDescription>>; // usually only one
        { T::attributeDescriptions() } -> std::same_as<std::vector<vk::VertexInputAttributeDescription>>;
    };

    struct Vertex {
        vec3 position;
        vec3 normal;
        vec2 tex_coord;

        static std::vector<vk::VertexInputBindingDescription> bindingDescriptions(); // usually only one
        static std::vector<vk::VertexInputAttributeDescription> attributeDescriptions();
    };

} // namespace lab