#include "gpu_resource_manager.hpp"
#include "graphics_device.hpp"
#include "error_logger.hpp"
#include "model.hpp"

#include <utility>

namespace lab {

    Buffer::Buffer(vk::Buffer buffer, VmaAllocation allocation, vk::DeviceSize size,
                   VmaMemoryUsage memory_usage, vk::BufferUsageFlags buffer_usage,
                   ResourceManagerConstructorLock /*lock*/)
    : m_buffer(buffer), m_allocation(allocation), m_size(size), m_memory_usage(memory_usage), m_buffer_usage(buffer_usage) {
        ErrorLogger::log("Buffer", "Buffer created");
    }

    Buffer::~Buffer() {
        ErrorLogger::log("Buffer", "Buffer destroyed");
    }

    BufferResource::BufferResource(class GpuResourceManager &parent, BufferHandle handle,
                                    vk::DescriptorType descriptor_type,
                                    vk::DescriptorBufferInfo descriptor_buffer_info,
                                    ResourceManagerConstructorLock /*lock*/)
                                    : m_parent(parent), m_handle(handle), m_descriptor_type(descriptor_type), m_descriptor_buffer_info(descriptor_buffer_info)
                                    {
                                        ErrorLogger::log("BufferResource", "BufferResource created");
                                    }

    BufferResource::~BufferResource() {
        if(m_parent.decreaseBufferReferenceCount(m_handle)){
            m_parent.destroyBuffer(m_handle);
        }
        ErrorLogger::log("BufferResource", "BufferResource destroyed");
    }

    GpuResourceManager::GpuResourceManager(std::shared_ptr<GraphicsDevice> device) : m_device(std::move(device)) {
        ErrorLogger::log("GpuResourceManager", "GpuResourceManager created");
    }

    GpuResourceManager::~GpuResourceManager() {

        m_models.clear();

        while (!m_buffers.empty()) {
            auto it = m_buffers.begin();
            destroyBuffer(it->first);
        }
        ErrorLogger::log("GpuResourceManager", "GpuResourceManager destroyed");
    }

    bool GpuResourceManager::decreaseBufferReferenceCount(BufferHandle handle) {
        auto it = m_buffers.find(handle);
        if (it == m_buffers.end()) {
            ErrorLogger::logFatalError("GpuResourceManager", "Buffer not found. BufferResource tried to decrease the ref_count");
        }
        --it->second->reference_count;
        return it->second->reference_count == 0;
    }

    void GpuResourceManager::increaseBufferReferenceCount(BufferHandle handle) {
        auto it = m_buffers.find(handle);
        if (it == m_buffers.end()) {
            throw std::runtime_error("Buffer not found. BufferResource tried to increase the ref_count");
        }
        ++it->second->reference_count;
    }

    void GpuResourceManager::destroyBuffer(BufferHandle handle) {
        auto it = m_buffers.find(handle);
        if (it == m_buffers.end()) {
            ErrorLogger::logFatalError("GpuResourceManager", "Buffer not found. Tried to destroy a buffer that doesn't exist");
        }
        if(it->second->reference_count != 0 ){
            ErrorLogger::logFatalError("GpuResourceManager", "Buffer still has references. Tried to destroy a buffer that still has references");
        }
        if(it->second->m_mapped_data) {
            unmapBuffer(handle);
        }
        vmaDestroyBuffer(m_device->allocator(), it->second->m_buffer, it->second->m_allocation);
        m_buffers.erase(it);
    }

    BufferHandle GpuResourceManager::createBuffer(size_t size, vk::BufferUsageFlags buffer_usage, VmaMemoryUsage memory_usage) {
        vk::BufferCreateInfo buffer_create_info{ {}, size, buffer_usage};
        VmaAllocationCreateInfo alloc_create_info{ {}, memory_usage };

        vk::Buffer vk_buffer;
        VmaAllocation allocation;
        vmaCreateBuffer(m_device->allocator(), reinterpret_cast<VkBufferCreateInfo *>(&buffer_create_info),
                        &alloc_create_info,reinterpret_cast<VkBuffer*>(&vk_buffer),
                        &allocation, nullptr);

        BufferHandle handle{m_next_buffer_handle_id++};
        m_buffers.emplace(handle, std::make_unique<Buffer>(vk_buffer, allocation, size, memory_usage, buffer_usage, ResourceManagerConstructorLock{}));
        return handle;
    }

    std::unique_ptr<BufferResource> GpuResourceManager::createBufferResource(
            BufferHandle buffer_handle, vk::DeviceSize offset,
            vk::DeviceSize range, vk::DescriptorType descriptor_type)
    {
        increaseBufferReferenceCount(buffer_handle);
        return std::make_unique<BufferResource>(
                *this, buffer_handle, descriptor_type,
                vk::DescriptorBufferInfo{m_buffers.at(buffer_handle)->m_buffer,
                offset, range}, ResourceManagerConstructorLock{});
    }

    void GpuResourceManager::writeToBuffer(const BufferResource& buffer_resource, const void *data, vk::DeviceSize size) {
        auto& buffer = m_buffers.at(buffer_resource.m_handle);
        if(buffer->m_mapped_data == nullptr) {
            ErrorLogger::logFatalError("GpuResourceManager", "Tried to write to a buffer that is not mapped");
        }
        void* final_location = reinterpret_cast<char*>(buffer->m_mapped_data) + buffer_resource.m_descriptor_buffer_info.offset;
        memcpy(final_location, data, size);
    }

    void GpuResourceManager::stageBuffer(const void *src, vk::DeviceSize size, const BufferResource& dest_resource) {

        auto staging_buffer_handle = createBuffer(size, vk::BufferUsageFlagBits::eTransferSrc, VMA_MEMORY_USAGE_CPU_ONLY);
        auto staging_buffer_resource = createBufferResource(staging_buffer_handle, 0, size, vk::DescriptorType::eStorageBuffer);
        auto& staging_buffer = m_buffers.at(staging_buffer_resource->m_handle);

        mapBuffer(staging_buffer_resource->m_handle);

        writeToBuffer(*staging_buffer_resource, src, size);
        auto& dest_buffer = m_buffers.at(dest_resource.m_handle);

        if(dest_buffer->m_size < dest_resource.m_descriptor_buffer_info.offset + size){
            ErrorLogger::logFatalError("GpuResourceManager", "Tried to stage a buffer that is too big");
        }

        m_device->immediateSubmit([&](vk::CommandBuffer cmd) {
            vk::BufferCopy copy{0, dest_resource.m_descriptor_buffer_info.offset, size};
            cmd.copyBuffer(staging_buffer->m_buffer, dest_buffer->m_buffer, 1, &copy);
        });
    }

    void* GpuResourceManager::getMappedData(const BufferResource& buffer_resource) const{
        auto& buffer = m_buffers.at(buffer_resource.m_handle);
        return reinterpret_cast<char*>(buffer->m_mapped_data) + buffer_resource.m_descriptor_buffer_info.offset;
    }

    void GpuResourceManager::mapBuffer(BufferHandle buffer_handle) {
        auto& buffer = m_buffers.at(buffer_handle);
        auto map_result = static_cast<vk::Result>(vmaMapMemory(m_device->allocator(), buffer->m_allocation,
                                                                     &buffer->m_mapped_data));
        if(map_result != vk::Result::eSuccess) {
            ErrorLogger::logFatalError("GpuResourceManager", "Failed to map buffer");
        }
    }

    void GpuResourceManager::unmapBuffer(BufferHandle buffer_handle) {
        auto& buffer = m_buffers.at(buffer_handle);
        if(buffer->m_mapped_data == nullptr) {
            ErrorLogger::logFatalError("GpuResourceManager", "Tried to unmap a buffer that is not mapped");
        }
        vmaUnmapMemory(m_device->allocator(), buffer->m_allocation);
        buffer->m_mapped_data = nullptr;
    }

    ModelHandle GpuResourceManager::loadModel(const std::filesystem::path& path) {
        ModelHandle handle{m_next_model_handle_id++};
        m_models.insert({handle, std::make_unique<Model<Vertex>>(path, *this)});
        return handle;
    }


} // namespace lab