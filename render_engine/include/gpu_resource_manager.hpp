#pragma once

#include "math.hpp"
#include "handles.hpp"
#include "vulkan_include.hpp"

#include <vk_mem_alloc.h>

#include <map>
#include <memory>
#include <filesystem>
#include <functional>
#include <variant>

namespace lab {

    // Forward declaration
    class GraphicsDevice;
    template<VertexInterface VT> class Model;

    // use this to make a constructor private to ResourceManager but usable to unique_ptr
    class ResourceManagerConstructorLock {
        ResourceManagerConstructorLock() = default;
        friend class GpuResourceManager;
    };

    struct Buffer{
        Buffer(vk::Buffer buffer, VmaAllocation allocation,
               vk::DeviceSize size, VmaMemoryUsage memory_usage,
               vk::BufferUsageFlags buffer_usage, ResourceManagerConstructorLock lock);
        ~Buffer();

        // don't copy
        Buffer(const Buffer&) = delete;
        Buffer& operator=(const Buffer&) = delete;

        vk::Buffer m_buffer;
        VmaAllocation m_allocation;
        vk::DeviceSize m_size;
        VmaMemoryUsage m_memory_usage;
        vk::BufferUsageFlags m_buffer_usage;
        void *m_mapped_data = nullptr; // always null if buffer not mapped
        uint32_t reference_count = 0;
    };




    // GpuResourceManager is a factory for BufferResources
    struct BufferResource {
    public:
        BufferResource(class GpuResourceManager& parent, BufferHandle handle, vk::DescriptorType descriptor_type,
                vk::DescriptorBufferInfo descriptor_buffer_info, ResourceManagerConstructorLock lock);
        ~BufferResource();

        // for now don't allow copying or moving
        BufferResource& operator=(const BufferResource&) = delete;
        BufferResource(const BufferResource&) = delete;
        BufferResource(BufferResource&&) = delete;
        BufferResource& operator=(BufferResource&&) = delete;

        BufferHandle m_handle = {};
        vk::DescriptorType m_descriptor_type{};
        vk::DescriptorBufferInfo m_descriptor_buffer_info{};

        private:
        // for reference counting - GpuResourceManager asserts that all references are released before it is destroyed
        GpuResourceManager& m_parent;
        friend class GpuResourceManager;
    };

    class GpuResourceManager {
    public:

        explicit GpuResourceManager(std::shared_ptr<GraphicsDevice> graphics_device);
        ~GpuResourceManager();

        [[nodiscard]] ModelHandle loadModel(const std::filesystem::path& path);
        [[nodiscard]] Model<Vertex>& getModel(ModelHandle handle) { return *m_models.at(handle); }

        [[nodiscard]] BufferHandle createBuffer(size_t size, vk::BufferUsageFlags buffer_usage, VmaMemoryUsage memory_usage);
        [[nodiscard]] vk::Buffer getBuffer(BufferHandle handle) const { return m_buffers.at(handle)->m_buffer; }
        [[nodiscard]] std::unique_ptr<BufferResource> createBufferResource(BufferHandle buffer_handle, vk::DeviceSize offset, vk::DeviceSize range, vk::DescriptorType descriptor_type);

        void stageBuffer(const void *src, vk::DeviceSize size, const BufferResource& dest_resource);
        void writeToBuffer(const BufferResource& buffer_resource, const void* data, vk::DeviceSize size);

        void mapBuffer(BufferHandle buffer_handle);
        [[nodiscard]] void* getMappedData(const BufferResource& buffer_resource) const;
        void unmapBuffer(BufferHandle buffer_handle);

        // don't copy or move
        GpuResourceManager(const GpuResourceManager&) = delete;
        GpuResourceManager& operator=(const GpuResourceManager&) = delete;
        GpuResourceManager(GpuResourceManager&&) = delete;
        GpuResourceManager& operator=(GpuResourceManager&&) = delete;

    private:
        // these 3 are called automatically by BufferResource
        [[nodiscard]] bool decreaseBufferReferenceCount(BufferHandle handle); // true if ref_count == 0
        void increaseBufferReferenceCount(BufferHandle handle);
        void destroyBuffer(BufferHandle handle); // force_destroy ignores ref_count

        BufferHandle::underlying_type m_next_buffer_handle_id = 0;
        std::map<BufferHandle, std::unique_ptr<Buffer>> m_buffers;
        ModelHandle ::underlying_type m_next_model_handle_id = 0;
        std::map<ModelHandle, std::unique_ptr<Model<Vertex>>> m_models;

        std::shared_ptr<GraphicsDevice> m_device;

        friend BufferResource;
    };

} // namespace lab