#pragma once

#include "handles.hpp"
#include "graphics_device.hpp"
#include "math.hpp"

#include <memory>

namespace lab {

// Forward declaration
    class Window;
    class Keyboard;
    class Mouse;
    class GraphicsPipelineManager;
    class GpuResourceManager;
    class Swapchain;
    template <VertexInterface VT> class Model;

    // this
    struct ClosedWindowWhileMinimized : public std::exception {
        [[nodiscard]] const char* what() const noexcept override {
            return "Window was closed while minimized";
        }
    };

    struct FrameData {
        // synchronization for render loop
        vk::Semaphore image_available_semaphore;
        vk::Semaphore render_finished_semaphore;
        vk::Fence render_finished_fence;

        vk::CommandBuffer command_buffer;
    };

    struct Player {
        vec3 position{0.0f, 0.0f, 0.0f};
        vec3 yxz_euler_angle{0.0f, 0.0f, 0.0f};
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
        void drawModel(vk::CommandBuffer cmd, const Model<Vertex>& model) const;
        void recreateSwapchain();

        static void updatePlayer(Player& player, Keyboard& keyboard, Mouse& mouse, float delta_time);
        FrameData& currentFrameData() { return m_frame_data[m_current_frame % FRAMES_IN_FLIGHT]; }

        [[nodiscard]] std::pair<PipelineHandle, PipelineLayoutHandle> createDefaultMeshPipelineAndLayout() const;

        void transitionSwapchainLayoutToPresentSrcKHR(vk::CommandBuffer cmd, size_t image_index) const;
        void transitionSwapchainImageLayoutToColorAttachmentOptimal(vk::CommandBuffer cmd, size_t image_index) const;
        void transitionDepthImageLayoutToDepthStencilAttachment(vk::CommandBuffer cmd) const;

        PipelineHandle default_mesh_pipeline{};
        PipelineLayoutHandle default_mesh_pipeline_layout{};

        std::unique_ptr<Window> m_window{};
        // m_graphics_device ownership is shared between all classes which need the device in their destructor
        std::shared_ptr<GraphicsDevice> m_graphics_device{};
        // m_gpu_resource_manager ownership is shared between all classes which use the BufferResource class
        std::shared_ptr<GpuResourceManager> m_gpu_resource_manager{};
        std::unique_ptr<Swapchain> m_swapchain{};
        std::unique_ptr<GraphicsPipelineManager> m_graphics_pipeline_manager{};
        std::unique_ptr<class GuiManager> m_gui_manager{};

        vk::CommandPool m_command_pool{};

        ModelHandle m_test_model{};
        ModelHandle m_dragon_model{};

        static constexpr size_t FRAMES_IN_FLIGHT = 2;
        size_t m_current_frame = 0;
        FrameData m_frame_data[FRAMES_IN_FLIGHT]{};

        size_t time_out_nanoseconds = 10'000'000'000;

        friend class GuiManager;
    };

    class GuiManager {
        public:
        explicit GuiManager(RenderEngine& render_engine);
        ~GuiManager();

        void showGui();

        static bool wantMouse();
        static bool wantKeyboard();
        static void recordGuiCommands(vk::CommandBuffer& cmd); // call this when recording draw commands [after calling endFrame()]


        // dont copy or move gui managers
        GuiManager(const GuiManager &) = delete;
        GuiManager &operator=(const GuiManager &) = delete;
        GuiManager(GuiManager &&) = delete;
        GuiManager &operator=(GuiManager &&) = delete;
    private:

        void initImGui();
        static void setupImGuiStyle();
        static void newFrame(); // call this before using any imgui drawing functions
        static void endFrame(); // call this after using any imgui drawing functions

        vk::DescriptorPool m_imgui_descriptor_pool{};
        RenderEngine& m_engine;
    };

} // namespace lab
