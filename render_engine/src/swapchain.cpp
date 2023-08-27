#include "swapchain.hpp"

#include "error_logger.hpp"
#include "graphics_device.hpp"

#include <iostream>
#include <limits>

namespace lab {

    Swapchain::Swapchain(std::shared_ptr<GraphicsDevice> device,
                         vk::Extent2D window_framebuffer_extent,
                         bool force_vsync,
                         std::shared_ptr<Swapchain> old_swapchain)
            : m_device{std::move(device)}, m_old_swapchain{
            std::move(old_swapchain)} {
        vk::PhysicalDevice physical_device = m_device->physicalDevice();
        vk::SurfaceKHR surface = m_device->surface();

        vk::SurfaceCapabilitiesKHR surface_capabilities =
                physical_device.getSurfaceCapabilitiesKHR(surface);
        std::vector<vk::SurfaceFormatKHR> available_surface_formats =
                physical_device.getSurfaceFormatsKHR(surface);
        std::vector<vk::PresentModeKHR> available_present_modes =
                physical_device.getSurfacePresentModesKHR(surface);

        vk::SurfaceFormatKHR surface_format = Swapchain::chooseSurfaceFormat(available_surface_formats);
        m_color_format = surface_format.format;
        m_depth_stencil_format = Swapchain::chooseDepthStencilFormat(m_device->physicalDevice());
        vk::PresentModeKHR present_mode = Swapchain::choosePresentMode(available_present_modes, force_vsync);
        m_extent = Swapchain::chooseSwapchainExtent(surface_capabilities, window_framebuffer_extent);

        m_swapchain = createSwapchain(m_extent, surface_format, surface_capabilities, present_mode);
        // reset old swapchain, or you will get a huge memory leak
        if (m_old_swapchain) m_old_swapchain.reset();

        m_images = m_device->get().getSwapchainImagesKHR(m_swapchain);
        m_image_views = createSwapchainImageViews(surface_format.format);
        m_depth_stencil_image = createDepthStencilImage(m_extent, m_depth_stencil_format);

        ErrorLogger::log("Swapchain", "Initialized");
    }

    Swapchain::~Swapchain() {

        // Cleanup the swapchain framebuffer
        m_device->get().destroy(m_depth_stencil_image.image_view);
        vmaDestroyImage(m_device->allocator(),m_depth_stencil_image.image,
                        m_depth_stencil_image.allocation);

        for (auto &image_view : m_image_views) m_device->get().destroy(image_view);
        for (auto &framebuffer : m_framebuffer) m_device->get().destroy(framebuffer);

        m_device->get().destroy(m_swapchain);
        m_device->get().destroy(m_render_pass);

        ErrorLogger::log("Swapchain", "Destroyed");
    }

    vk::SwapchainKHR Swapchain::createSwapchain(vk::Extent2D extent,
                                    vk::SurfaceFormatKHR surface_format,
                                    vk::SurfaceCapabilitiesKHR surface_capabilities,
                                    vk::PresentModeKHR present_mode) const {

        uint32_t image_count = std::max(surface_capabilities.minImageCount, 3u);

        // Clamp image_count to max images if there is no cap on the maximum image [0 means no cap]
        if ((surface_capabilities.maxImageCount > 0) && (image_count > surface_capabilities.maxImageCount)) {
            image_count = surface_capabilities.maxImageCount;
        }

        vk::SharingMode image_sharing_mode = vk::SharingMode::eExclusive;
        std::vector<uint32_t> queue_family_indices{m_device->queue(ePresent).family_index};
        if (m_device->queue(eGraphics).family_index != m_device->queue(ePresent).family_index) {
            queue_family_indices.push_back(m_device->queue(eGraphics).family_index);
            image_sharing_mode = vk::SharingMode::eConcurrent;
        }

        vk::SwapchainCreateInfoKHR swapchain_ci{
                vk::SwapchainCreateFlagsKHR(),
                m_device->surface(),
                image_count,
                surface_format.format,
                surface_format.colorSpace,
                extent,
                1,
                vk::ImageUsageFlagBits::eColorAttachment,
                image_sharing_mode,
                queue_family_indices,
                vk::SurfaceTransformFlagBitsKHR::eIdentity,
                vk::CompositeAlphaFlagBitsKHR::eOpaque,
                present_mode,
                VK_TRUE,
                (m_old_swapchain) ? m_old_swapchain->get() : nullptr};

        return m_device->get().createSwapchainKHR(swapchain_ci);
    }

    std::vector<vk::ImageView> Swapchain::createSwapchainImageViews(vk::Format format) const {

        if(m_images.empty()) {
            ErrorLogger::logFatalError("Swapchain", "Swapchain images not created");
        }

        std::vector<vk::ImageView> image_views(m_images.size());

        vk::ImageViewCreateInfo image_view_info {
                vk::ImageViewCreateFlags(),
                nullptr,
                vk::ImageViewType::e2D,
                format,
                {},
                vk::ImageSubresourceRange{vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1}};

        for (int i = 0; i < m_images.size(); ++i) {
            image_view_info.image = m_images[i];
            image_views[i] = m_device->get().createImageView(image_view_info);
        }
        return image_views;
    }


    AllocatedImage Swapchain::createDepthStencilImage(vk::Extent2D extent, vk::Format depth_format) const {

        AllocatedImage depth_image{};

        auto depth_image_create_info = static_cast<VkImageCreateInfo>(
                vk::ImageCreateInfo{{},
                                    vk::ImageType::e2D,
                                    depth_format,
                                    vk::Extent3D{extent.width, extent.height, 1},
                                    1,
                                    1,
                                    vk::SampleCountFlagBits::e1,
                                    vk::ImageTiling::eOptimal,
                                    vk::ImageUsageFlagBits::eDepthStencilAttachment,
                                    vk::SharingMode::eExclusive,
                                    m_device->queue(ePresent).family_index,
                                    vk::ImageLayout::eUndefined});

        VmaAllocationCreateInfo depth_image_alloc_info = {};
        depth_image_alloc_info.usage = VMA_MEMORY_USAGE_GPU_ONLY;
        depth_image_alloc_info.requiredFlags =
                VkMemoryPropertyFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        vmaCreateImage(m_device->allocator(), &depth_image_create_info,
                       &depth_image_alloc_info, reinterpret_cast<VkImage*>(&depth_image.image), &depth_image.allocation, nullptr);

        VkImageViewCreateInfo depth_image_view_create_info = vk::ImageViewCreateInfo{
                {},
                depth_image.image,
                vk::ImageViewType::e2D,
                depth_format,
                vk::ComponentMapping{
                        vk::ComponentSwizzle::eIdentity, vk::ComponentSwizzle::eIdentity,
                        vk::ComponentSwizzle::eIdentity, vk::ComponentSwizzle::eIdentity},
                vk::ImageSubresourceRange{vk::ImageAspectFlagBits::eDepth, 0, 1, 0, 1}};

        depth_image.image_view = m_device->get().createImageView(depth_image_view_create_info);

        return depth_image;
    }


    vk::ResultValue<uint32_t>
    Swapchain::acquireNextImage(vk::Semaphore image_available_semaphore) {
        uint32_t acquired_image_index = std::numeric_limits<uint32_t>::max();
        // we use the C version as vulkan_hpp throws an exception at
        // VK_SWAPCHAIN_OUT_OF_DATE_KHR
        auto result = static_cast<vk::Result>(vkAcquireNextImageKHR(
                m_device->get(), m_swapchain, 1'000'000'000, image_available_semaphore,
                nullptr, &acquired_image_index));

        if(acquired_image_index >= m_images.size()) {
            ErrorLogger::logFatalError("Swapchain", "Acquired image index is out of range");
        }

        return {result, acquired_image_index};
    }

    vk::RenderingAttachmentInfoKHR Swapchain::colorAttachmentInfo(size_t image_index, vk::ClearColorValue clear_color) const {
        return vk::RenderingAttachmentInfo{
            m_image_views[image_index],
            vk::ImageLayout::eColorAttachmentOptimal,
            vk::ResolveModeFlagBits::eNone, {}, {},
            vk::AttachmentLoadOp::eClear,
            vk::AttachmentStoreOp::eStore,
            clear_color};
    }

    vk::RenderingAttachmentInfoKHR Swapchain::noClearColorAttachmentInfo(size_t image_index) const {
        return vk::RenderingAttachmentInfo{
                m_image_views[image_index],
                vk::ImageLayout::eColorAttachmentOptimal,
                vk::ResolveModeFlagBits::eNone, {}, {},
                vk::AttachmentLoadOp::eDontCare,
                vk::AttachmentStoreOp::eDontCare,
                vk::ClearColorValue{std::array<float, 4>{0.f, 0.f, 0.f, 1.f}}};
    }

    vk::RenderingAttachmentInfoKHR Swapchain::depthStencilAttachmentInfo() const {
        return vk::RenderingAttachmentInfo{
            m_depth_stencil_image.image_view,
            vk::ImageLayout::eDepthStencilAttachmentOptimal,
            vk::ResolveModeFlagBits::eNone, {}, {},
            vk::AttachmentLoadOp::eClear,
            vk::AttachmentStoreOp::eStore,
            vk::ClearDepthStencilValue(1.0f,0)};
    }

    vk::Extent2D Swapchain::chooseSwapchainExtent(const vk::SurfaceCapabilitiesKHR &surface_capabilities, vk::Extent2D window_framebuffer_extent) {

        if(window_framebuffer_extent.width == 0 || window_framebuffer_extent.height == 0) {
            ErrorLogger::logFatalError("Swapchain", "Window framebuffer extent is 0");
        }

        const vk::Extent2D &currentExtent = surface_capabilities.currentExtent;
        const vk::Extent2D &minExtent = surface_capabilities.minImageExtent;
        const vk::Extent2D &maxExtent = surface_capabilities.maxImageExtent;

        // if the current surface extent is valid use it
        if (currentExtent.width != std::numeric_limits<uint32_t>::max()) {
            return currentExtent;
        }

        // clamp Extent to min and max swapchain extent
        vk::Extent2D actualExtent = window_framebuffer_extent;
        actualExtent.width = std::clamp(actualExtent.width, minExtent.width, maxExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, minExtent.height, maxExtent.height);
        return actualExtent;
    }

    vk::SurfaceFormatKHR Swapchain::chooseSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> &available_formats) {
        for (const auto available_format : available_formats) {
            if (((available_format.format == vk::Format::eR8G8B8A8Unorm) ||
                (available_format.format == vk::Format::eB8G8R8A8Unorm)) &&
                available_format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
                return available_format;
            }
        }
        throw std::runtime_error("No suitable swapchain surface format found");
    }

vk::PresentModeKHR Swapchain::choosePresentMode(const std::vector<vk::PresentModeKHR> &available_present_modes, bool enable_vsync) {

    // by default prefer mailbox mode over fifo, as it is the lowest latency non-tearing mode
    // however, if vsync is enabled or mailbox is not available, use fifo
    // [vsync] which is guaranteed to be available

    if (enable_vsync) {
        return vk::PresentModeKHR::eFifo;
    }

    for (const auto &available_present_mode : available_present_modes) {
        if (available_present_mode == vk::PresentModeKHR::eMailbox) {
            return available_present_mode;
        }
    }

    std::cout << "Swapchain: Mailbox Present Mode not available switching to "
                 "FiFo (VSYNC): "
              << std::endl;
    return vk::PresentModeKHR::eFifo;
}

vk::Format Swapchain::chooseDepthStencilFormat(vk::PhysicalDevice physical_device) {
    std::vector<vk::Format> candidates{vk::Format::eD24UnormS8Uint,
                                       vk::Format::eD32SfloatS8Uint};
    for (vk::Format format : candidates) {
        auto formatProperties = physical_device.getFormatProperties(format);
        // check if the candidate has optimal tiling features
        if ((formatProperties.optimalTilingFeatures &
             vk::FormatFeatureFlagBits::eDepthStencilAttachment) ==
            vk::FormatFeatureFlagBits::eDepthStencilAttachment) {
            return format;
        }
    }
    throw std::runtime_error("No appropriate depth format");
}

} // namespace lab