#pragma once

#include "vulkan_types.hpp"
#include "handles.hpp"
#include "math.hpp"

#include <filesystem>

namespace lab {

    // Forward declaration
    class GraphicsDevice;

    class GraphicsPipelineConfig {
    public:
        GraphicsPipelineConfig() = default;
        ~GraphicsPipelineConfig() = default;

        void setShaderPaths(std::filesystem::path vertex_shader_path, std::filesystem::path fragment_shader_path);

        template<VertexInterface T> // inl
        void setVertexInputDescription() {
            m_vertex_input_binding_description = T::bindingDescriptions();
            m_vertex_input_attribute_description = T::attributeDescriptions();
        }

        void setInputAssemblyStateCreateInfo(vk::PrimitiveTopology topology, bool primitive_restart_enable = false);
        void setDepthStencilStateCreateInfo(bool depth_test_enable = true, bool depth_write_enable = true,
                                            vk::CompareOp depth_compare_op = vk::CompareOp::eLess);
        void setRasterizationStateCreateInfo(
                vk::PolygonMode polygon_mode = vk::PolygonMode::eFill,
                vk::CullModeFlags cull_mode = vk::CullModeFlagBits::eBack,
                vk::FrontFace front_face = vk::FrontFace::eCounterClockwise,
                bool depth_clamp_enable = false, bool rasterizer_discard_enable = false,
                bool depth_bias_enable = false, float depth_bias_constant_factor = 0.0f,
                float depth_bias_clamp = 0.0f, float depth_bias_slope_factor = 0.0f,
                float line_width = 1.0f);
        void setMultisampleStateCreateInfo(vk::SampleCountFlagBits sample_count = vk::SampleCountFlagBits::e1,
                                           bool sample_shading_enable = false, float min_sample_shading = 1.0f,
                                           vk::SampleMask *sample_mask = nullptr, bool alpha_to_coverage_enable = false,
                                           bool alpha_to_one_enable = false);
        void setColorBlendAttachmentCreateInfo(bool blend_enable = false, vk::BlendFactor src_color_blend_factor = vk::BlendFactor::eOne,
                                            vk::BlendFactor dst_color_blend_factor = vk::BlendFactor::eZero,
                                            vk::BlendOp color_blend_op = vk::BlendOp::eAdd,
                                            vk::BlendFactor src_alpha_blend_factor = vk::BlendFactor::eOne,
                                            vk::BlendFactor dst_alpha_blend_factor = vk::BlendFactor::eZero,
                                            vk::BlendOp alpha_blend_op = vk::BlendOp::eAdd,
                                            vk::ColorComponentFlags color_write_mask = vk::ColorComponentFlagBits::eR |
                                                                                         vk::ColorComponentFlagBits::eG |
                                                                                         vk::ColorComponentFlagBits::eB |
                                                                                         vk::ColorComponentFlagBits::eA);
        void setDepthFormat(vk::Format depth_format) { m_depth_format = depth_format; }
        void setColorFormat(vk::Format color_format) { m_color_format = color_format; }
        void setViewportAndScissorCount(uint32_t viewport_count = 1, uint32_t scissor_count = 1);

    private:
        std::filesystem::path m_vertex_shader_path, m_fragment_shader_path;
        std::vector<vk::VertexInputAttributeDescription> m_vertex_input_attribute_description;
        std::vector<vk::VertexInputBindingDescription> m_vertex_input_binding_description;
        vk::PipelineInputAssemblyStateCreateInfo m_input_assembly_state_create_info;
        vk::PipelineDepthStencilStateCreateInfo m_depth_stencil_state_create_info;
        vk::PipelineRasterizationStateCreateInfo m_rasterization_state_create_info;
        vk::PipelineMultisampleStateCreateInfo m_multisample_state_create_info;
        vk::PipelineColorBlendStateCreateInfo m_color_blend_state_create_info;
        vk::Format m_depth_format{}, m_color_format{};
        uint32_t m_viewport_count, m_scissor_count;


        friend class GraphicsPipelineManager;
    };


    class GraphicsPipelineManager {
    public:
        explicit GraphicsPipelineManager(std::shared_ptr<GraphicsDevice> device);
        ~GraphicsPipelineManager();

        // dont copy or move graphics pipeline managers
        GraphicsPipelineManager(const GraphicsPipelineManager &) = delete;
        GraphicsPipelineManager &operator=(const GraphicsPipelineManager &) = delete;
        GraphicsPipelineManager(GraphicsPipelineManager &&) = delete;
        GraphicsPipelineManager &operator=(GraphicsPipelineManager &&) = delete;

        [[nodiscard]] vk::Pipeline getPipeline(PipelineHandle handle) const { return m_pipelines[handle.id]; }
        [[nodiscard]] vk::PipelineLayout getPipelineLayout(PipelineLayoutHandle handle) const { return m_pipeline_layouts[handle.id]; }

        [[nodiscard]] PipelineHandle createPipeline(GraphicsPipelineConfig& config, PipelineLayoutHandle layout_handle);
        [[nodiscard]] PipelineLayoutHandle createPipelineLayout(
                const std::vector<vk::DescriptorSetLayout>& descriptor_set_layouts = {},
                const std::vector<vk::PushConstantRange>& push_constant_ranges = {});


    private:
        [[nodiscard]] static vk::ShaderModule createShaderModule(const std::vector<char> &code, vk::Device device);

        // assume we never delete pipelines or their layouts :D
        std::vector<vk::Pipeline> m_pipelines;
        std::vector<vk::PipelineLayout> m_pipeline_layouts;
        std::shared_ptr<GraphicsDevice> m_device;
    };

} // lab

