#pragma once

#include "graphics_device.hpp"
#include <memory>
#include <utility>
#include <array>

namespace lab {

// Forward declaration
    class GraphicsDevice;

    class Swapchain {
    public:
        Swapchain(std::shared_ptr<GraphicsDevice> device, vk::Extent2D window_framebuffer_extent,
                  bool force_vsync = false, std::shared_ptr<Swapchain> old_swapchain = nullptr);
        ~Swapchain();

        [[nodiscard]] const vk::SwapchainKHR& get() const { return m_swapchain; }
        [[nodiscard]] vk::ImageView imageView(size_t swapchain_image_index) const {return m_image_views[swapchain_image_index];}
        [[nodiscard]] vk::Image image(size_t swapchain_image_index) const {return m_images[swapchain_image_index];}
        [[nodiscard]] size_t imageCount() const { return m_images.size(); }
        [[nodiscard]] vk::Image depthStencilImage() const { return m_depth_stencil_image.image; }
        [[nodiscard]] vk::Extent2D extent() const { return m_extent; }
        [[nodiscard]] vk::Format colorFormat() const { return m_color_format; }
        [[nodiscard]] vk::Format depthStencilFormat() const { return m_depth_stencil_format; }
        [[nodiscard]] vk::RenderingAttachmentInfoKHR colorAttachmentInfo(size_t image_index, vk::ClearColorValue = vk::ClearColorValue{std::array<float, 4>{0.f, 0.f, 0.f, 1.f}}) const;
        [[nodiscard]] vk::RenderingAttachmentInfoKHR noClearColorAttachmentInfo(size_t image_index) const;
        [[nodiscard]] vk::RenderingAttachmentInfoKHR depthStencilAttachmentInfo() const;

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
        [[nodiscard]] AllocatedImage createDepthStencilImage(vk::Extent2D extent, vk::Format depth_format) const;

        vk::SwapchainKHR m_swapchain;
        std::shared_ptr<Swapchain> m_old_swapchain;
        std::shared_ptr<GraphicsDevice> m_device;

        static vk::Extent2D chooseSwapchainExtent(const vk::SurfaceCapabilitiesKHR &surface_capabilities,
                                                  vk::Extent2D window_framebuffer_extent);
        static vk::SurfaceFormatKHR chooseSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> &available_formats);
        static vk::PresentModeKHR choosePresentMode(const std::vector<vk::PresentModeKHR> &available_present_modes,
                                                    bool enable_vsync);
        static vk::Format chooseDepthStencilFormat(vk::PhysicalDevice physical_device);

        std::vector<vk::Framebuffer> m_framebuffer;
        // swapchain images (image allocation is handled by the swapchain)
        std::vector<vk::Image> m_images;
        std::vector<vk::ImageView> m_image_views;
        AllocatedImage m_depth_stencil_image;
        vk::RenderPass m_render_pass;
        vk::Extent2D m_extent;
        vk::Format m_color_format;
        vk::Format m_depth_stencil_format;
    };
} // namespace lab