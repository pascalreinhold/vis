#pragma once

#include <memory>
#include <utility>
#include <array>
#include <vulkan_types.hpp>

namespace lab {

// Forward declaration
    class GraphicsDevice;

    class Swapchain {
    public:
        Swapchain(std::shared_ptr<GraphicsDevice> device, vk::Extent2D window_framebuffer_extent,
                  std::shared_ptr<Swapchain> old_swapchain = nullptr);
        ~Swapchain();

        [[nodiscard]] const vk::SwapchainKHR& get() const { return m_swapchain; }
        [[nodiscard]] vk::ImageView imageView(uint32_t swapchain_image_index) const {return m_image_views[swapchain_image_index];}
        [[nodiscard]] vk::Image image(uint32_t swapchain_image_index) const {return m_images[swapchain_image_index];}
        [[nodiscard]] vk::Extent2D extent() const { return m_extent; }
        [[nodiscard]] vk::Format colorFormat() const { return m_color_format; }
        [[nodiscard]] vk::Format depthFormat() const { return m_depth_format; }
        [[nodiscard]] vk::RenderingAttachmentInfo colorAttachmentInfo(size_t image_index, vk::ClearColorValue = vk::ClearColorValue{std::array<float, 4>{0.f, 0.f, 0.f, 1.f}}) const;
        [[nodiscard]] vk::RenderingAttachmentInfo depthAttachmentInfo() const;

        [[nodiscard]] vk::ResultValue<uint32_t> acquireNextImage(vk::Semaphore signal_on_acquire);

        // don't copy or move this
        Swapchain(const Swapchain &) = delete;
        Swapchain &operator=(const Swapchain &) = delete;
        Swapchain(Swapchain &&) = delete;
        Swapchain &operator=(Swapchain &&) = delete;

    private:

        // creation functions
        [[nodiscard]] vk::SwapchainKHR createSwapchain(vk::Extent2D extent, vk::SurfaceFormatKHR surface_format,
                                                       vk::SurfaceCapabilitiesKHR surface_capabilities,
                                                       vk::PresentModeKHR present_mode) const;
        [[nodiscard]] std::vector<vk::ImageView> createSwapchainImageViews(vk::Format format) const;
        [[nodiscard]] AllocatedImage createDepthImage(vk::Extent2D extent, vk::Format depth_format) const;

        vk::SwapchainKHR m_swapchain;
        std::shared_ptr<Swapchain> m_old_swapchain;
        std::shared_ptr<GraphicsDevice> m_device;

        static vk::Extent2D chooseSwapchainExtent(const vk::SurfaceCapabilitiesKHR &surface_capabilities,
                                                  vk::Extent2D window_framebuffer_extent);
        static vk::SurfaceFormatKHR chooseSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> &available_formats);
        static vk::PresentModeKHR choosePresentMode(const std::vector<vk::PresentModeKHR> &available_present_modes,
                                                    bool enable_vsync);
        static vk::Format chooseDepthFormat(vk::PhysicalDevice physical_device);

        std::vector<vk::Framebuffer> m_framebuffer;
        // swapchain images (image allocation is handled by the swapchain)
        std::vector<vk::Image> m_images;
        std::vector<vk::ImageView> m_image_views;
        AllocatedImage m_depth_image;
        vk::RenderPass m_render_pass;
        vk::Extent2D m_extent;
        vk::Format m_color_format;
        vk::Format m_depth_format;
    };
} // namespace lab