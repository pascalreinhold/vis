#pragma once

#include <vk_mem_alloc.h>
#include <vulkan/vulkan.hpp>

namespace lab {

    struct AllocatedImage {
        vk::Image image{};
        vk::ImageView image_view{};
        VmaAllocation allocation{};
    };

    struct AllocatedBuffer {
        vk::Buffer buffer{};
        VmaAllocation allocation{};
    };

} // namespace lab