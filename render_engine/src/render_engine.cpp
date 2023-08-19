#include "render_engine.hpp"

#include "error_logger.hpp"
#include "graphics_device.hpp"
#include "graphics_pipeline_manager.hpp"
#include "swapchain.hpp"
#include "window.hpp"

#include <iostream>

namespace lab {
    RenderEngine::RenderEngine() {
        m_window = std::make_unique<Window>("My Window", 800, 600);
        m_graphics_device = std::make_shared<GraphicsDevice>(m_window);
        m_graphics_pipeline_manager = std::make_unique<GraphicsPipelineManager>(m_graphics_device);
        auto framebuffer_size = m_window->getFramebufferSize();
        m_swapchain = std::make_unique<Swapchain>(
                m_graphics_device,
                vk::Extent2D(framebuffer_size.first, framebuffer_size.second));
        std::tie(default_mesh_pipeline, default_mesh_pipeline_layout) = createDefaultMeshPipelineAndLayout();
        test_vertex_buffer = createTestVertexBuffer();




        // setup per frame data
        // command pool and buffers
        vk::CommandPoolCreateInfo command_pool_create_info{
            vk::CommandPoolCreateFlagBits::eResetCommandBuffer,m_graphics_device->queue(QueueType::eGraphics).family_index};
        m_command_pool = m_graphics_device->get().createCommandPool(command_pool_create_info);
        vk::CommandBufferAllocateInfo command_buffer_allocate_info{
            m_command_pool,vk::CommandBufferLevel::ePrimary,FRAMES_IN_FLIGHT};
        std::vector<vk::CommandBuffer> command_buffers =
                m_graphics_device->get().allocateCommandBuffers(command_buffer_allocate_info);
        // fences and semaphores for the render loop synchronization
        vk::FenceCreateInfo render_fence_create_info{vk::FenceCreateFlagBits::eSignaled};
        vk::SemaphoreCreateInfo semaphore_create_info{vk::SemaphoreCreateFlags()};

        for (size_t i = 0; i < FRAMES_IN_FLIGHT; ++i) {
            m_frame_data[i].render_finished_fence = m_graphics_device->get().createFence(render_fence_create_info);
            m_frame_data[i].image_available_semaphore = m_graphics_device->get().createSemaphore(semaphore_create_info);
            m_frame_data[i].render_finished_semaphore = m_graphics_device->get().createSemaphore(semaphore_create_info);
            m_frame_data[i].command_buffer = command_buffers[i];
        }

        ErrorLogger::log("Engine", "Initialized");
    }

    RenderEngine::~RenderEngine() {
        m_graphics_device->get().waitIdle();

        vmaDestroyBuffer(m_graphics_device->allocator(), test_vertex_buffer.buffer, test_vertex_buffer.allocation);

        for (auto& frame_data : m_frame_data) {
            m_graphics_device->get().destroyFence(frame_data.render_finished_fence);
            m_graphics_device->get().destroySemaphore(frame_data.image_available_semaphore);
            m_graphics_device->get().destroySemaphore(frame_data.render_finished_semaphore);
        }
        m_graphics_device->get().destroyCommandPool(m_command_pool);

        ErrorLogger::log("Engine", "Destroyed");
    }

    void RenderEngine::run() {
        while (!m_window->shouldClose()) {
            Window::pollEvents();

            // if it fails to render, it means that the swapchain was out of date ,and it was recreated
            // i.e. we try to render the same frame again
            bool success = render();
            m_current_frame += success;
        }
    }

    bool RenderEngine::render() {
        FrameData& frame_data = currentFrameData();
        vk::CommandBuffer& cmd = frame_data.command_buffer;

        // wait on fence and reset it
        vk::Result wait_result = m_graphics_device->get().waitForFences(
                frame_data.render_finished_fence,VK_TRUE,time_out_nanoseconds);
        if(wait_result != vk::Result::eSuccess) {
            ErrorLogger::logFatalError("Engine", "Failed to wait for fence");
        }
        m_graphics_device->get().resetFences(frame_data.render_finished_fence);


        // acquire image from swapchain
        auto image_result_value = m_swapchain->acquireNextImage(frame_data.image_available_semaphore);
        if(image_result_value.result == vk::Result::eErrorOutOfDateKHR) {
            recreateSwapchain();
            return false;
        } else if (image_result_value.result != vk::Result::eSuccess) {
            ErrorLogger::logFatalError("Engine", "Failed to acquire next image");
        }
        uint32_t image_index = image_result_value.value;

        cmd.reset(vk::CommandBufferResetFlagBits::eReleaseResources);

        // begin command buffer recording
        vk::CommandBufferBeginInfo command_buffer_begin_info{
            vk::CommandBufferUsageFlagBits::eOneTimeSubmit};
        cmd.begin(command_buffer_begin_info);

        // swapchain layout needs to be done manually when dynamic rendering is used
        transitionSwapchainLayoutToColorAttachmentWrite(cmd, image_index);

        static std::array<float, 4> clear_color = {0.f, 0.f, 0.f, 1.f};
        clear_color[0] = std::sinf(static_cast<float>(m_current_frame) / 10.f);
        clear_color[1] = std::cosf(static_cast<float>(m_current_frame) / 10.f);
        auto color_attachment = m_swapchain->colorAttachmentInfo(image_index, vk::ClearColorValue{clear_color});
        auto depth_attachment = m_swapchain->depthAttachmentInfo();

        auto rendering_info = vk::RenderingInfo {
                {},vk::Rect2D{{0,0},m_swapchain->extent()}, 1, 0,
                color_attachment, &depth_attachment, {}};
        cmd.beginRendering(rendering_info);

        // draw commands
        recordDrawCommands(cmd);

        // end command buffer recording and submit it
        cmd.endRendering();
        transitionSwapchainLayoutToPresent(cmd, image_index);
        cmd.end();

        vk::PipelineStageFlags wait_stage = vk::PipelineStageFlagBits::eColorAttachmentOutput;
        vk::SubmitInfo submit_info{frame_data.image_available_semaphore,
                                   wait_stage,
                                   frame_data.command_buffer,
                                   frame_data.render_finished_semaphore};
        m_graphics_device->queue(QueueType::eGraphics).queue.submit(submit_info,frame_data.render_finished_fence);

        // present image
        vk::PresentInfoKHR present_info{frame_data.render_finished_semaphore,
                                        m_swapchain->get(),image_index};
        auto present_result = m_graphics_device->queue(QueueType::ePresent).queue.presentKHR(present_info);
        if(present_result == vk::Result::eErrorOutOfDateKHR || present_result == vk::Result::eSuboptimalKHR) {
            recreateSwapchain();
        } else if (present_result != vk::Result::eSuccess) {
            ErrorLogger::logFatalError("Engine", "Failed to present image");
        }

        return true;
    }

    void RenderEngine::recreateSwapchain() {
        auto framebuffer_size = m_window->getFramebufferSize();
        while(framebuffer_size.first == 0 || framebuffer_size.second == 0) {
            framebuffer_size = m_window->getFramebufferSize();
            Window::waitEvents();
        }

        m_graphics_device->get().waitIdle();
        m_swapchain = std::make_unique<Swapchain>(
                m_graphics_device,
                vk::Extent2D(framebuffer_size.first,
                framebuffer_size.second));
    }

    void RenderEngine::transitionSwapchainLayoutToPresent(vk::CommandBuffer cmd, size_t image_index) const {
        vk::ImageMemoryBarrier layout_transition_barrier {
                vk::AccessFlagBits::eColorAttachmentWrite,
                vk::AccessFlagBits::eNone,
                vk::ImageLayout::eColorAttachmentOptimal,
                vk::ImageLayout::ePresentSrcKHR,
                VK_QUEUE_FAMILY_IGNORED,
                VK_QUEUE_FAMILY_IGNORED,
                m_swapchain->image(image_index),
                vk::ImageSubresourceRange{vk::ImageAspectFlagBits::eColor,0,1,0,1}
        };

        cmd.pipelineBarrier(vk::PipelineStageFlagBits::eColorAttachmentOutput,
                            vk::PipelineStageFlagBits::eBottomOfPipe,
                            vk::DependencyFlags(),
                            nullptr,
                            nullptr,
                            layout_transition_barrier);
    }

    void RenderEngine::transitionSwapchainLayoutToColorAttachmentWrite(vk::CommandBuffer cmd, size_t image_index) const {
        vk::ImageMemoryBarrier layout_transition_barrier {
                vk::AccessFlagBits::eNone,
                vk::AccessFlagBits::eColorAttachmentWrite,
                vk::ImageLayout::eUndefined,
                vk::ImageLayout::eColorAttachmentOptimal,
                VK_QUEUE_FAMILY_IGNORED,
                VK_QUEUE_FAMILY_IGNORED,
                m_swapchain->image(image_index),
                vk::ImageSubresourceRange{vk::ImageAspectFlagBits::eColor,0,1,0,1}
        };

        cmd.pipelineBarrier(vk::PipelineStageFlagBits::eTopOfPipe,
                            vk::PipelineStageFlagBits::eColorAttachmentOutput,
                            vk::DependencyFlags(),
                            nullptr,
                            nullptr,
                            layout_transition_barrier);
    }

    std::pair<PipelineHandle, PipelineLayoutHandle> RenderEngine::createDefaultMeshPipelineAndLayout() const {
        GraphicsPipelineConfig config{};
        config.setShaderPaths("C:/code/lab/shaders/default_mesh_shader.vert.spv",
                              "C:/code/lab/shaders/default_mesh_shader.frag.spv");
        config.setColorFormat(m_swapchain->colorFormat());
        config.setDepthFormat(m_swapchain->depthFormat());
        config.setDepthStencilStateCreateInfo();
        config.setRasterizationStateCreateInfo();
        config.setMultisampleStateCreateInfo();
        config.setInputAssemblyStateCreateInfo(vk::PrimitiveTopology::eTriangleList);
        config.setColorBlendAttachmentCreateInfo();
        config.setVertexInputDescription<Vertex>();
        config.setViewportAndScissorCount();

        auto pipeline_layout_handle = m_graphics_pipeline_manager->createPipelineLayout();
        auto pipeline_handle = m_graphics_pipeline_manager->createPipeline(config, pipeline_layout_handle);

        return {pipeline_handle, pipeline_layout_handle};
    }

    AllocatedBuffer RenderEngine::createTestVertexBuffer() const {
        std::vector<Vertex> vertices = {
                {{-0.5f, -0.5f, -0.5f}, {1.f, 0.f, 0.f}},
                {{0.5f, -0.5f, -0.5f}, {0.f, 1.f, 0.f}},
                {{0.5f, 0.5f, -0.5f}, {0.f, 0.f, 1.f}},
                {{-0.5f, 0.5f, -0.5f}, {1.f, 1.f, 0.f}},
                {{-0.5f, -0.5f, 0.5f}, {0.f, 1.f, 1.f}},
                {{0.5f, -0.5f, 0.5f}, {1.f, 0.f, 1.f}}
        };
        for(auto& vertex : vertices) vertex.position *= 10.f;

        AllocatedBuffer vertex_buffer{};

        vk::BufferCreateInfo buffer_create_info{
                {}, sizeof(Vertex) * vertices.size(), vk::BufferUsageFlagBits::eVertexBuffer};
        VmaAllocationCreateInfo allocation_create_info{
                {}, VMA_MEMORY_USAGE_CPU_TO_GPU, VMA_ALLOCATION_CREATE_MAPPED_BIT};
        auto result = vmaCreateBuffer(m_graphics_device->allocator(),
                                      reinterpret_cast<const VkBufferCreateInfo *>(&buffer_create_info),
                                      &allocation_create_info, reinterpret_cast<VkBuffer*>(&vertex_buffer.buffer), &vertex_buffer.allocation, nullptr);
        if(result != VK_SUCCESS) {
            ErrorLogger::logFatalError("Engine", "Failed to create vertex buffer");
        }
        return vertex_buffer;
    }

    void RenderEngine::recordDrawCommands(vk::CommandBuffer cmd) const {
        vk::Viewport viewport{0.f, 0.f, static_cast<float>(m_swapchain->extent().width),
                              static_cast<float>(m_swapchain->extent().height), 0.f, 1.f};
        vk::Rect2D scissor{{0, 0}, m_swapchain->extent()};
        cmd.setViewport(0,viewport);
        cmd.setScissor(0, scissor);

        cmd.bindPipeline(vk::PipelineBindPoint::eGraphics, m_graphics_pipeline_manager->getPipeline(default_mesh_pipeline));
        vk::DeviceSize offset = 0;
        cmd.bindVertexBuffers(0, test_vertex_buffer.buffer, offset);
        cmd.draw(6, 1, 0, 0);
    }

} // namespace lab
