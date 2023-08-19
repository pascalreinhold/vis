#pragma once

#include "handles.hpp"
#include "vulkan_types.hpp"

#include <memory>

namespace lab {

// Forward declaration
    class Window;
    class GraphicsDevice;
    class GraphicsPipelineManager;
    class Swapchain;

    struct FrameData {
        // synchronization for render loop
        vk::Semaphore image_available_semaphore;
        vk::Semaphore render_finished_semaphore;
        vk::Fence render_finished_fence;

        vk::CommandBuffer command_buffer;
    };



    class RenderEngine {
    public:
        RenderEngine();
        ~RenderEngine();

        // dont copy or move render engines
        RenderEngine(const RenderEngine &) = delete;
        RenderEngine &operator=(const RenderEngine &) = delete;
        RenderEngine(RenderEngine &&) = delete;
        RenderEngine &operator=(RenderEngine &&) = delete;

        void run();
    private:
        bool render();
        void recordDrawCommands(vk::CommandBuffer cmd) const;
        void recreateSwapchain();
        [[nodiscard]] AllocatedBuffer createTestVertexBuffer() const;

        FrameData& currentFrameData() { return m_frame_data[m_current_frame % FRAMES_IN_FLIGHT]; }

        void transitionSwapchainLayoutToPresent(vk::CommandBuffer cmd, size_t image_index) const;
        void transitionSwapchainLayoutToColorAttachmentWrite(vk::CommandBuffer cmd, size_t image_index) const;

        [[nodiscard]] std::pair<PipelineHandle, PipelineLayoutHandle> createDefaultMeshPipelineAndLayout() const;

        PipelineHandle default_mesh_pipeline{};
        PipelineLayoutHandle default_mesh_pipeline_layout{};

        AllocatedBuffer test_vertex_buffer{};

        std::unique_ptr<Window> m_window{};
        // m_graphics_device ownership is shared between all classes which need the device in their destructor
        std::shared_ptr<GraphicsDevice> m_graphics_device{};
        std::unique_ptr<Swapchain> m_swapchain{};
        std::unique_ptr<GraphicsPipelineManager> m_graphics_pipeline_manager{};
        vk::CommandPool m_command_pool{};

        static constexpr size_t FRAMES_IN_FLIGHT = 2;
        size_t m_current_frame = 0;
        FrameData m_frame_data[FRAMES_IN_FLIGHT]{};

        size_t time_out_nanoseconds = 10'000'000'000;
    };

} // namespace lab
