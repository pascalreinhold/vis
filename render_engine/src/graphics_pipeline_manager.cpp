#include "graphics_pipeline_manager.hpp"
#include "graphics_device.hpp"
#include "error_logger.hpp"

#include <filesystem>

namespace lab {

namespace {

    std::vector<char> readFile(const std::filesystem::path &filepath) {
        std::ifstream file(filepath, std::ios::ate | std::ios::binary);

        if (!file.is_open()) {
            throw std::runtime_error("failed to open file: " + filepath.string() + "!");
        }

        size_t file_size = static_cast<size_t>(file.tellg());
        std::vector<char> buffer(static_cast<std::streamsize>(file_size));
        file.seekg(0);
        file.read(buffer.data(), static_cast<std::streamsize>(file_size));
        file.close();

        return buffer;
    }

} // namespace

    void GraphicsPipelineConfig::setShaderPaths(std::filesystem::path vertex_shader_path,
                                                std::filesystem::path fragment_shader_path) {
        m_vertex_shader_path = std::move(vertex_shader_path);
        m_fragment_shader_path = std::move(fragment_shader_path);
    }

    void GraphicsPipelineConfig::setInputAssemblyStateCreateInfo(
            vk::PrimitiveTopology topology, bool primitive_restart_enable) {
        m_input_assembly_state_create_info = vk::PipelineInputAssemblyStateCreateInfo{
                {}, topology, primitive_restart_enable};
    }

    void GraphicsPipelineConfig::setDepthStencilStateCreateInfo(bool depth_test_enable, bool depth_write_enable,
                                                                vk::CompareOp depth_compare_op) {
    m_depth_stencil_state_create_info = vk::PipelineDepthStencilStateCreateInfo{
            {}, depth_test_enable,depth_write_enable,depth_compare_op};
    }

    void GraphicsPipelineConfig::setRasterizationStateCreateInfo(vk::PolygonMode polygon_mode, vk::CullModeFlags cull_mode,
                                         vk::FrontFace front_face, bool depth_clamp_enable,
                                         bool rasterizer_discard_enable, bool depth_bias_enable,
                                         float depth_bias_constant_factor, float depth_bias_clamp,
                                         float depth_bias_slope_factor, float line_width) {

        m_rasterization_state_create_info = vk::PipelineRasterizationStateCreateInfo{
                {}, depth_clamp_enable, rasterizer_discard_enable,
                polygon_mode, cull_mode, front_face,depth_bias_enable,
                depth_bias_constant_factor,depth_bias_clamp,
                depth_bias_slope_factor, line_width
        };
    }

    void GraphicsPipelineConfig::setMultisampleStateCreateInfo(
            vk::SampleCountFlagBits sample_count, bool sample_shading_enable, float min_sample_shading,
            vk::SampleMask *sample_mask, bool alpha_to_coverage_enable, bool alpha_to_one_enable) {

        m_multisample_state_create_info = vk::PipelineMultisampleStateCreateInfo{
                {}, sample_count, sample_shading_enable, min_sample_shading,
                sample_mask, alpha_to_coverage_enable, alpha_to_one_enable
        };
    }

    void GraphicsPipelineConfig::setColorBlendAttachmentCreateInfo(
            bool blend_enable, vk::BlendFactor src_color_blend_factor, vk::BlendFactor dst_color_blend_factor,
            vk::BlendOp color_blend_op, vk::BlendFactor src_alpha_blend_factor, vk::BlendFactor dst_alpha_blend_factor,
            vk::BlendOp alpha_blend_op, vk::ColorComponentFlags color_write_mask) {

        vk::PipelineColorBlendAttachmentState color_blend_attachment_state {
            blend_enable, src_color_blend_factor, dst_color_blend_factor,
            color_blend_op, src_alpha_blend_factor, dst_alpha_blend_factor,
            alpha_blend_op, color_write_mask};
        m_color_blend_state_create_info = vk::PipelineColorBlendStateCreateInfo{
                {}, false, vk::LogicOp::eCopy, 1, &color_blend_attachment_state
        };
    }

    void GraphicsPipelineConfig::setViewportAndScissorCount(uint32_t viewport_count, uint32_t scissor_count) {
        m_viewport_count = viewport_count;
        m_scissor_count = scissor_count;
    }

    GraphicsPipelineManager::GraphicsPipelineManager(std::shared_ptr<GraphicsDevice> device)
        : m_device{std::move(device)}
    {
        ErrorLogger::log("GraphicsPipelineManager", "Initialized");
    }

    GraphicsPipelineManager::~GraphicsPipelineManager() {
        for(auto& pipeline : m_pipelines) {
            m_device->get().destroyPipeline(pipeline);
        }
        for(auto& pipeline_layout : m_pipeline_layouts) {
            m_device->get().destroyPipelineLayout(pipeline_layout);
        }
        ErrorLogger::log("GraphicsPipelineManager", "Destroyed");
    }

    vk::ShaderModule GraphicsPipelineManager::createShaderModule(const std::vector<char> &code, vk::Device device) {
        if((code.size() % 4) != 0) {
            ErrorLogger::log("GraphicsPipelineManager", "SpirV code byte length is not a multiple of 4");
        }
        // shader size needs to be given in bytes (not in uint32_t)
        vk::ShaderModuleCreateInfo shader_module_ci{ {}, code.size(), reinterpret_cast<const uint32_t*>(code.data()) };
        return device.createShaderModule(shader_module_ci);
    }

    PipelineHandle GraphicsPipelineManager::createPipeline(GraphicsPipelineConfig& config, PipelineLayoutHandle layout_handle) {

        vk::ShaderModule vertex_shader = createShaderModule(readFile(config.m_vertex_shader_path), m_device->get());
        vk::ShaderModule fragment_shader = createShaderModule(readFile(config.m_fragment_shader_path), m_device->get());

        std::vector<vk::PipelineShaderStageCreateInfo> shader_stage_info = {
                vk::PipelineShaderStageCreateInfo{
                        vk::PipelineShaderStageCreateFlags(),vk::ShaderStageFlagBits::eVertex,
                        vertex_shader,"main",nullptr
                },
                vk::PipelineShaderStageCreateInfo{
                        vk::PipelineShaderStageCreateFlags(),vk::ShaderStageFlagBits::eFragment,
                        fragment_shader,"main",nullptr
                }
        };

        vk::PipelineVertexInputStateCreateInfo vertex_input_state_info{{},
                config.m_vertex_input_binding_description,
                config.m_vertex_input_attribute_description};

        vk::PipelineViewportStateCreateInfo viewport_state_create_info {
                {}, config.m_viewport_count, nullptr, config.m_scissor_count, nullptr};

        std::vector<vk::DynamicState> dynamic_states {
                vk::DynamicState::eViewport, vk::DynamicState::eScissor};
        vk::PipelineDynamicStateCreateInfo dynamic_state_create_info {{}, dynamic_states};

        vk::GraphicsPipelineCreateInfo pipeline_create_info {
            {},
            shader_stage_info,
            &vertex_input_state_info,
            &config.m_input_assembly_state_create_info,
            nullptr,
            &viewport_state_create_info,
            &config.m_rasterization_state_create_info,
            &config.m_multisample_state_create_info,
            &config.m_depth_stencil_state_create_info,
            &config.m_color_blend_state_create_info,
            &dynamic_state_create_info,
            getPipelineLayout(layout_handle),
        };

        vk::PipelineRenderingCreateInfo pipeline_rendering_create_info {
            0, 1, &config.m_color_format,
            config.m_depth_format, vk::Format::eUndefined};
        pipeline_create_info.pNext = &pipeline_rendering_create_info;

        auto result_pipeline = m_device->get().createGraphicsPipeline(nullptr, pipeline_create_info);

        if(result_pipeline.result != vk::Result::eSuccess) {
            ErrorLogger::logFatalError("GraphicsPipelineManager", "Failed to create graphics pipeline");
        }

        m_device->get().destroyShaderModule(vertex_shader);
        m_device->get().destroyShaderModule(fragment_shader);

        m_pipelines.push_back(result_pipeline.value);
        return PipelineHandle{static_cast<uint32_t>(m_pipelines.size() - 1)};
    }

    PipelineLayoutHandle GraphicsPipelineManager::createPipelineLayout(
            const std::vector<vk::DescriptorSetLayout>& descriptor_set_layouts,
            const std::vector<vk::PushConstantRange>& push_constant_ranges) {

        vk::PipelineLayoutCreateInfo pipeline_layout_ci {
                {},
                descriptor_set_layouts,
                push_constant_ranges};

        auto result_pipeline_layout = m_device->get().createPipelineLayout(pipeline_layout_ci);
        if(!result_pipeline_layout) {
            ErrorLogger::logFatalError("GraphicsPipelineManager", "Failed to create pipeline layout");
        }

        m_pipeline_layouts.push_back(result_pipeline_layout);
        return PipelineLayoutHandle{static_cast<uint32_t>(m_pipeline_layouts.size() - 1)};
    }

} // lab
