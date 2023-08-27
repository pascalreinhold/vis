#include "render_engine.hpp"

#include "camera.hpp"
#include "error_logger.hpp"
#include "graphics_device.hpp"
#include "graphics_pipeline_manager.hpp"
#include "gpu_resource_manager.hpp"
#include "model.hpp"
#include "swapchain.hpp"
#include "window.hpp"

#include <imgui.h>
#include <imgui_impl_vulkan.h>
#include <imgui_impl_glfw.h>

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

        m_gpu_resource_manager = std::make_shared<GpuResourceManager>(m_graphics_device);
        m_test_model = m_gpu_resource_manager->loadModel("./assets/models/unit_plate/unit_plate.obj");
        m_dragon_model = m_gpu_resource_manager->loadModel("./assets/models/alduin/scene.gltf");

        m_gui_manager = std::make_unique<GuiManager>(*this);

        ErrorLogger::log("Engine", "Initialized");
    }

    RenderEngine::~RenderEngine() {
        m_graphics_device->get().waitIdle();

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
            m_window->keyboard().prepareNextFrame(); // needs to come before pollEvents
            m_window->mouse().prepareNextFrame(); // needs to come before pollEvents
            Window::pollEvents();

            if(m_window->keyboard().isPressed(eTab).first) {
                m_window->mouse().toggleCursor();
            }

            m_gui_manager->showGui();

            try {
                m_current_frame += render(); // only increment the frame if it was rendered successfully
            } catch (const ClosedWindowWhileMinimized&) {
                break; // break out of the render loop if the window was closed while minimized
            }
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
        } else if (image_result_value.result != vk::Result::eSuccess && image_result_value.result != vk::Result::eSuboptimalKHR) {
            ErrorLogger::logFatalError("Engine", "Failed to acquire next image");
        }
        uint32_t image_index = image_result_value.value;

        cmd.reset(vk::CommandBufferResetFlagBits::eReleaseResources);

        // begin command buffer recording
        vk::CommandBufferBeginInfo command_buffer_begin_info{
            vk::CommandBufferUsageFlagBits::eOneTimeSubmit};
        cmd.begin(command_buffer_begin_info);

        // swapchain layout needs to be done manually when dynamic rendering is used
        transitionSwapchainImageLayoutToColorAttachmentOptimal(cmd, image_index);
        transitionDepthImageLayoutToDepthStencilAttachment(cmd);

        const float clear_grey_scale = 0.2f * abs(sinf((float)m_current_frame / 1000.f));
        std::array<float, 4> clear_color{clear_grey_scale, clear_grey_scale, clear_grey_scale, 1.f};
        auto color_attachment = m_swapchain->colorAttachmentInfo(image_index, vk::ClearColorValue{clear_color});
        auto depth_stencil_attachment = m_swapchain->depthStencilAttachmentInfo();

        auto rendering_info = vk::RenderingInfoKHR {
                {},vk::Rect2D{{0,0},m_swapchain->extent()}, 1, 0,
                color_attachment, &depth_stencil_attachment, &depth_stencil_attachment};
        cmd.beginRenderingKHR(rendering_info);
        recordDrawCommands(cmd);
        cmd.endRenderingKHR();

        auto no_clear_color_attachment = m_swapchain->noClearColorAttachmentInfo(image_index);
        auto gui_rendering_info = vk::RenderingInfoKHR {{},vk::Rect2D{{0,0},
                                                        m_swapchain->extent()},
                                                        1, 0,no_clear_color_attachment};
        cmd.beginRenderingKHR(gui_rendering_info);
        m_gui_manager->recordGuiCommands(cmd);
        cmd.endRenderingKHR();

        // let thread sleep for 10 ms
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        // end command buffer recording and submit it


        transitionSwapchainLayoutToPresentSrcKHR(cmd, image_index);
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
        while((framebuffer_size.first == 0 || framebuffer_size.second == 0)) {
            framebuffer_size = m_window->getFramebufferSize();
            Window::waitEvents();

            if(m_window->shouldClose()) {
                throw ClosedWindowWhileMinimized();
            }
        }

        m_graphics_device->get().waitIdle();
        std::shared_ptr<Swapchain> old_swapchain = std::move(m_swapchain);
        m_swapchain = std::make_unique<Swapchain>(
                m_graphics_device,
                vk::Extent2D(framebuffer_size.first,framebuffer_size.second),
                false,
                old_swapchain);
    }

    void RenderEngine::transitionSwapchainLayoutToPresentSrcKHR(vk::CommandBuffer cmd, size_t image_index) const {
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

    void RenderEngine::transitionSwapchainImageLayoutToColorAttachmentOptimal(
            vk::CommandBuffer cmd, size_t image_index) const {
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

    void RenderEngine::transitionDepthImageLayoutToDepthStencilAttachment(vk::CommandBuffer cmd) const {
        vk::ImageMemoryBarrier layout_transition_barrier {
                vk::AccessFlagBits::eNone,
                vk::AccessFlagBits::eDepthStencilAttachmentWrite,
                vk::ImageLayout::eUndefined,
                vk::ImageLayout::eDepthStencilAttachmentOptimal,
                VK_QUEUE_FAMILY_IGNORED,
                VK_QUEUE_FAMILY_IGNORED,
                m_swapchain->depthStencilImage(),
                vk::ImageSubresourceRange{vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil,0,1,0,1}
        };

        cmd.pipelineBarrier(vk::PipelineStageFlagBits::eEarlyFragmentTests | vk::PipelineStageFlagBits::eLateFragmentTests,
                            vk::PipelineStageFlagBits::eEarlyFragmentTests | vk::PipelineStageFlagBits::eLateFragmentTests,
                            vk::DependencyFlags(),
                            nullptr,
                            nullptr,
                            layout_transition_barrier);
    }

    std::pair<PipelineHandle, PipelineLayoutHandle> RenderEngine::createDefaultMeshPipelineAndLayout() const {
        GraphicsPipelineConfig config{};
        config.setShaderPaths("./shaders/default_mesh_shader.vert.spv",
                              "./shaders/default_mesh_shader.frag.spv");
        config.setColorFormat(m_swapchain->colorFormat());
        config.setDepthStencilFormat(m_swapchain->depthStencilFormat());
        config.setDepthStencilStateCreateInfo();
        config.setRasterizationStateCreateInfo();
        config.setMultisampleStateCreateInfo();
        config.setInputAssemblyStateCreateInfo(vk::PrimitiveTopology::eTriangleList);
        config.setColorBlendAttachmentCreateInfo();
        config.setVertexInputDescription<Vertex>();
        config.setViewportAndScissorCount();

        vk::PushConstantRange push_constant_range{vk::ShaderStageFlagBits::eVertex, 0, sizeof(mat4)};
        auto pipeline_layout_handle = m_graphics_pipeline_manager->createPipelineLayout({}, {push_constant_range});
        auto pipeline_handle = m_graphics_pipeline_manager->createPipeline(config, pipeline_layout_handle);

        return {pipeline_handle, pipeline_layout_handle};
    }

    void RenderEngine::recordDrawCommands(vk::CommandBuffer cmd) const {
        vk::Viewport viewport{0.f, 0.f, static_cast<float>(m_swapchain->extent().width),
                              static_cast<float>(m_swapchain->extent().height), 0.f, 1.f};
        vk::Rect2D scissor{{0, 0}, m_swapchain->extent()};
        cmd.setViewport(0,viewport);
        cmd.setScissor(0, scissor);

        cmd.bindPipeline(vk::PipelineBindPoint::eGraphics, m_graphics_pipeline_manager->getPipeline(default_mesh_pipeline));

        static Player player{vec3(0.f, -1.f, 0.f), vec3(0.f, 0.f, 0.f)};
        updatePlayer(player, m_window->keyboard(), m_window->mouse(), 0.01f);

        mat4 view_projection_matrix = FirstPersonCamera::viewProjectionMatrix(player.position, player.yxz_euler_angle, m_window->aspect());

        cmd.pushConstants(m_graphics_pipeline_manager->getPipelineLayout(default_mesh_pipeline_layout),
                          vk::ShaderStageFlagBits::eVertex, 0, sizeof(mat4), &view_projection_matrix[0][0]);

        drawModel(cmd, m_gpu_resource_manager->getModel(m_test_model));
        drawModel(cmd, m_gpu_resource_manager->getModel(m_dragon_model));
    }

    void RenderEngine::drawModel(vk::CommandBuffer cmd, const Model<Vertex>& model) const {
        for(const auto& mesh : model.meshes()) {
            const BufferResource& vertex_buffer_resource = mesh.vertexBuffer();
            vk::DeviceSize vertex_buffer_offset = vertex_buffer_resource.m_descriptor_buffer_info.offset;
            const BufferResource& index_buffer_resource = mesh.indexBuffer();
            vk::DeviceSize index_buffer_offset = index_buffer_resource.m_descriptor_buffer_info.offset;

            cmd.bindVertexBuffers(0, m_gpu_resource_manager->getBuffer(vertex_buffer_resource.m_handle), vertex_buffer_offset);
            cmd.bindIndexBuffer(m_gpu_resource_manager->getBuffer(index_buffer_resource.m_handle), index_buffer_offset, vk::IndexType::eUint32);
            cmd.drawIndexed(static_cast<uint32_t>(mesh.indexCount()), 1, 0, 0, 0);
        }
    }

    void RenderEngine::updatePlayer(Player &player, Keyboard &keyboard, Mouse &mouse, float delta_time) {
        constexpr vec3 default_forward = vec3(0.f, 0.f, -1.f);
        constexpr vec3 default_up = vec3(0.f, -1.f, 0.f);

        mat3 rot = eulerAngleYXZ(player.yxz_euler_angle.x, player.yxz_euler_angle.y, player.yxz_euler_angle.z);
        vec3 forward = rot * default_forward;
        vec3 up = rot * default_up;
        vec3 right = cross(forward, up);

        auto& position = player.position;
        float forward_speed = 200.f;
        float right_speed = 200.f;
        float up_speed = 200.f;

        vec3 forward_delta = forward * forward_speed * delta_time;
        vec3 right_delta = right * right_speed * delta_time;
        vec3 up_delta = -up * up_speed * delta_time;

        if (keyboard.isHeld(KeyboardButtons::eW).first) {
            position += forward_delta;
        }
        if (keyboard.isHeld(KeyboardButtons::eS).first) {
            position -= forward_delta;
        }
        if (keyboard.isHeld(KeyboardButtons::eD).first) {
            position -= right_delta;
        }
        if (keyboard.isHeld(KeyboardButtons::eA).first) {
            position += right_delta;
        }
        if (keyboard.isHeld(KeyboardButtons::eSpace).first) {
            position += up_delta;
        }
        if (keyboard.isHeld(KeyboardButtons::eLeftShift).first) {
            position -= up_delta;
        }

        if(!mouse.isCursorEnabled()) {
            vec2 mouse_delta = {mouse.delta()[0], mouse.delta()[1]};
            player.yxz_euler_angle.y = glm::clamp(player.yxz_euler_angle.y, -1.5f, 1.5f);
            player.yxz_euler_angle.x = glm::mod(player.yxz_euler_angle.x, glm::two_pi<float>());
            player.yxz_euler_angle -= vec3(mouse_delta.x, mouse_delta.y, 0.f) * 0.002f;
        }
    }

    GuiManager::GuiManager(RenderEngine &render_engine) : m_engine(render_engine) {
        initImGui();
        setupImGuiStyle();
        ErrorLogger::log("GuiManager", "Initialized");
    }

    void GuiManager::initImGui() {
        vk::DescriptorPoolSize pool_sizes[] =
                {
                        {vk::DescriptorType::eSampler, 500},
                        {vk::DescriptorType::eCombinedImageSampler, 500},
                        {vk::DescriptorType::eSampledImage, 500},
                        {vk::DescriptorType::eStorageImage, 500},
                        {vk::DescriptorType::eUniformBuffer, 500},
                        {vk::DescriptorType::eStorageTexelBuffer, 500},
                        {vk::DescriptorType::eUniformBuffer, 500},
                        {vk::DescriptorType::eStorageBuffer, 500},
                        {vk::DescriptorType::eUniformBufferDynamic, 500},
                        {vk::DescriptorType::eStorageBufferDynamic, 500},
                        {vk::DescriptorType::eInputAttachment, 500}
                };

        vk::DescriptorPoolCreateInfo pool_info = vk::DescriptorPoolCreateInfo(
                vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet,
                500, static_cast<uint32_t>(std::size(pool_sizes)), pool_sizes);
        m_imgui_descriptor_pool = m_engine.m_graphics_device->get().createDescriptorPool(pool_info);
        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForVulkan(m_engine.m_window->glfwWindow(), true);
        ImGui_ImplVulkan_InitInfo init_info = {};

        init_info.Instance = m_engine.m_graphics_device->instance();
        init_info.PhysicalDevice = m_engine.m_graphics_device->physicalDevice();
        init_info.Device = m_engine.m_graphics_device->get();
        //init_info.QueueFamily = graphics_queue_family_;
        init_info.Queue = m_engine.m_graphics_device->queue(QueueType::eGraphics).queue;
        //init_info.PipelineCache = nullptr;
        init_info.DescriptorPool = m_imgui_descriptor_pool;
        //init_info.Subpass = 0;
        init_info.UseDynamicRendering = true;
        init_info.ColorAttachmentFormat = static_cast<VkFormat>(m_engine.m_swapchain->colorFormat());
        init_info.MinImageCount = static_cast<uint32_t>(m_engine.m_swapchain->imageCount());
        init_info.ImageCount = static_cast<uint32_t>(m_engine.m_swapchain->imageCount());
        init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
        init_info.CheckVkResultFn = [](VkResult result) {
            if (static_cast<vk::Result>(result) != vk::Result::eSuccess) ErrorLogger::logFatalError("ImGui", "Failed to initialize ImGui");};

        ImGui_ImplVulkan_Init(&init_info, VK_NULL_HANDLE);

        ImGuiIO &io = ImGui::GetIO();
        std::string font_filepath = "./assets/fonts/Roboto/Roboto-Medium.ttf";
        io.Fonts->AddFontFromFileTTF(font_filepath.c_str(), 18);

        m_engine.m_graphics_device->immediateSubmit(
                [&](vk::CommandBuffer cmd) {ImGui_ImplVulkan_CreateFontsTexture(cmd);});
        ImGui_ImplVulkan_DestroyFontUploadObjects();

    }

    void GuiManager::setupImGuiStyle() {
        //Light Mode standard style
        ImGuiStyle &style = ImGui::GetStyle();

         auto normalizeColor = [](int r, int g, int b, int a) ->ImVec4 {
            return ImVec4{
                    static_cast<float>(r)/255.f,
                    static_cast<float>(g)/255.f,
                    static_cast<float>(b)/255.f,
                    static_cast<float>(a)/255.f};
        };

        style.Alpha = 1.0f;
        style.DisabledAlpha = 0.6000000238418579f;
        style.WindowPadding = ImVec2(8.0f, 8.0f);
        style.WindowRounding = 0.0f;
        style.WindowBorderSize = 0.0f;
        style.WindowMinSize = ImVec2(32.0f, 32.0f);
        style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
        style.WindowMenuButtonPosition = ImGuiDir_Right;
        style.ChildRounding = 0.0f;
        style.ChildBorderSize = 1.0f;
        style.PopupRounding = 8.0f;
        style.PopupBorderSize = 0.0f;
        style.FramePadding = ImVec2(4.0f, 3.0f);
        style.FrameRounding = 8.0f;
        style.FrameBorderSize = 0.0f;
        style.ItemSpacing = ImVec2(12.0f, 6.0f);
        style.ItemInnerSpacing = ImVec2(8.0f, 4.0f);
        style.CellPadding = ImVec2(4.0f, 2.0f);
        style.IndentSpacing = 20.0f;
        style.ColumnsMinSpacing = 6.0f;
        style.ScrollbarSize = 11.0f;
        style.ScrollbarRounding = 2.0f;
        style.GrabMinSize = 10.0f;
        style.GrabRounding = 2.0f;
        style.TabRounding = 8.0f;
        style.TabBorderSize = 0.0f;
        style.TabMinWidthForCloseButton = 0.0f;
        style.ColorButtonPosition = ImGuiDir_Right;
        style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
        style.SelectableTextAlign = ImVec2(0.0f, 0.0f);
        style.Colors[ImGuiCol_Text] = normalizeColor(255, 255, 255, 255);
        style.Colors[ImGuiCol_TextDisabled] = normalizeColor(180, 180, 180, 255);
        style.Colors[ImGuiCol_WindowBg] = normalizeColor(60, 60, 63, 255);
        style.Colors[ImGuiCol_ChildBg] = normalizeColor(105, 105, 105, 0);
        style.Colors[ImGuiCol_PopupBg] = normalizeColor(45, 45, 48, 255);
        style.Colors[ImGuiCol_Border] = normalizeColor(155, 155, 155, 255);
        style.Colors[ImGuiCol_BorderShadow] = normalizeColor(0, 0, 0, 0);
        style.Colors[ImGuiCol_FrameBg] = normalizeColor(120, 120, 120, 99);
        style.Colors[ImGuiCol_FrameBgHovered] = normalizeColor(140, 140, 140, 102);
        style.Colors[ImGuiCol_FrameBgActive] = normalizeColor(120, 120, 120, 176);
        style.Colors[ImGuiCol_TitleBg] = normalizeColor(45, 45, 48, 255);
        style.Colors[ImGuiCol_TitleBgActive] = normalizeColor(45, 45, 48, 255);
        style.Colors[ImGuiCol_TitleBgCollapsed] = normalizeColor(45, 45, 48, 120);
        style.Colors[ImGuiCol_MenuBarBg] = normalizeColor(40, 40, 43, 204);
        style.Colors[ImGuiCol_ScrollbarBg] = normalizeColor(45, 45, 48, 150);
        style.Colors[ImGuiCol_ScrollbarGrab] = normalizeColor(120, 120, 120, 97);
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = normalizeColor(130, 130, 130, 102);
        style.Colors[ImGuiCol_ScrollbarGrabActive] = normalizeColor(160, 160, 160, 153);
        style.Colors[ImGuiCol_CheckMark] = normalizeColor(255, 185, 20, 255);
        style.Colors[ImGuiCol_SliderGrab] = normalizeColor(255, 185, 20, 255);
        style.Colors[ImGuiCol_SliderGrabActive] = normalizeColor(247, 215, 14, 255);
        style.Colors[ImGuiCol_Button] = normalizeColor(120, 120, 120, 99);
        style.Colors[ImGuiCol_ButtonHovered] = normalizeColor(255, 185, 20, 180);
        style.Colors[ImGuiCol_ButtonActive] = normalizeColor(255, 185, 20, 255);
        style.Colors[ImGuiCol_Header] = normalizeColor(140, 140, 140, 99);
        style.Colors[ImGuiCol_HeaderHovered] = normalizeColor(255, 185, 20, 180);
        style.Colors[ImGuiCol_HeaderActive] = normalizeColor(255, 185, 20, 255);
        style.Colors[ImGuiCol_Separator] = normalizeColor(170, 170, 170, 80);
        style.Colors[ImGuiCol_SeparatorHovered] = normalizeColor(170, 170, 170, 80);
        style.Colors[ImGuiCol_SeparatorActive] = normalizeColor(170, 170, 170, 80);
        style.Colors[ImGuiCol_ResizeGrip] = normalizeColor(170, 170, 170, 99);
        style.Colors[ImGuiCol_ResizeGripHovered] = normalizeColor(255, 185, 20, 180);
        style.Colors[ImGuiCol_ResizeGripActive] = normalizeColor(255, 185, 20, 255);
        style.Colors[ImGuiCol_Tab] = normalizeColor(120, 120, 120, 99);
        style.Colors[ImGuiCol_TabHovered] = normalizeColor(255, 185, 20, 180);
        style.Colors[ImGuiCol_TabActive] = normalizeColor(255, 185, 20, 255);
        style.Colors[ImGuiCol_TabUnfocused] = normalizeColor(255, 185, 20, 90);
        style.Colors[ImGuiCol_TabUnfocusedActive] = normalizeColor(255, 185, 20, 125);
        style.Colors[ImGuiCol_PlotLines] = normalizeColor(255, 255, 255, 255);
        style.Colors[ImGuiCol_PlotLinesHovered] = normalizeColor(230, 179, 0, 255);
        style.Colors[ImGuiCol_PlotHistogram] = normalizeColor(230, 179, 0, 255);
        style.Colors[ImGuiCol_PlotHistogramHovered] = normalizeColor(255, 153, 0, 255);
        style.Colors[ImGuiCol_TableHeaderBg] = normalizeColor(140, 140, 140, 99);
        style.Colors[ImGuiCol_TableBorderStrong] = normalizeColor(0, 0, 0, 255);
        style.Colors[ImGuiCol_TableBorderLight] = normalizeColor(0, 0, 0, 50);
        style.Colors[ImGuiCol_TableRowBg] = normalizeColor(0, 0, 0, 15);
        style.Colors[ImGuiCol_TableRowBgAlt] = normalizeColor(255, 255, 255, 18);
        style.Colors[ImGuiCol_TextSelectedBg] = normalizeColor(35, 132, 255, 99);
        style.Colors[ImGuiCol_DragDropTarget] = normalizeColor(255, 255, 0, 230);
        style.Colors[ImGuiCol_NavHighlight] = normalizeColor(115, 115, 230, 240);
        style.Colors[ImGuiCol_NavWindowingHighlight] = normalizeColor(255, 255, 255, 179);
        style.Colors[ImGuiCol_NavWindowingDimBg] = normalizeColor(204, 204, 204, 51);
        style.Colors[ImGuiCol_ModalWindowDimBg] = normalizeColor(51, 51, 51, 89);
    }

    GuiManager::~GuiManager() {
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        m_engine.m_graphics_device->get().destroyDescriptorPool(m_imgui_descriptor_pool);
        
        ErrorLogger::log("GuiManager", "Destroyed");
    }

    void GuiManager::showGui() {
        GuiManager::newFrame();
        ImGui::ShowDemoWindow();

        //show Mouse Debugging Info
        if(ImGui::Begin("Mouse Debugging Info")) {
            auto mouse_pos = m_engine.m_window->mouse().position();
            ImGui::Text("Mouse Position: (%f, %f)", mouse_pos[0], mouse_pos[1]);
            auto mouse_pos_delta = m_engine.m_window->mouse().delta();
            ImGui::Text("Mouse Delta: (%f, %f)", mouse_pos_delta[0], mouse_pos_delta[1]);
            float wheel_delta = m_engine.m_window->mouse().wheelDelta();
            ImGui::Text("Mouse Scroll Delta: %f", wheel_delta);
            auto [mouse_left_button, mouse_left_button_pressed] = m_engine.m_window->mouse().isPressed(MouseButtons::eLeft);
            ImGui::Text("Mouse Left Button: %s", mouse_left_button ? "Pressed" : "Not Pressed");
            auto [mouse_right_button, mouse_right_button_pressed] = m_engine.m_window->mouse().isPressed(MouseButtons::eRight);
            ImGui::Text("Mouse Right Button: %s", mouse_right_button ? "Pressed" : "Not Pressed");
            auto [mouse_middle_button, mouse_middle_button_pressed] = m_engine.m_window->mouse().isPressed(MouseButtons::eMiddle);
            ImGui::Text("Mouse Middle Button: %s", mouse_middle_button ? "Pressed" : "Not Pressed");
        }
        ImGui::End();

        GuiManager::endFrame();
    }

    void GuiManager::newFrame() {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    bool GuiManager::wantMouse() {
        return ImGui::GetIO().WantCaptureMouse;
    }

    bool GuiManager::wantKeyboard() {
        return ImGui::GetIO().WantCaptureKeyboard;
    }

    void GuiManager::recordGuiCommands(vk::CommandBuffer &cmd) {
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmd);
    }

    void GuiManager::endFrame() {
        ImGui::Render();
    }


} // namespace lab
