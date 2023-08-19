#include "math.hpp"

namespace lab {

    std::vector<vk::VertexInputBindingDescription> Vertex::bindingDescriptions() {
        return {vk::VertexInputBindingDescription{0,sizeof(Vertex),vk::VertexInputRate::eVertex}};
    }

    std::vector<vk::VertexInputAttributeDescription> Vertex::attributeDescriptions() {
        return std::vector<vk::VertexInputAttributeDescription>{
                vk::VertexInputAttributeDescription{0,0,vk::Format::eR32G32B32Sfloat,
                                                    static_cast<uint32_t>(offsetof(Vertex, position))},
                vk::VertexInputAttributeDescription{1,0,vk::Format::eR32G32B32Sfloat,
                                                    static_cast<uint32_t>(offsetof(Vertex, normal))
                }
        };
    }

} // namespace lab