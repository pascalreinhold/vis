#pragma once

#include "vulkan_include.hpp"
#include <vk_mem_alloc.h>

#include <array>
#include <functional>
#include <iostream>
#include <vector>
#include <functional>
#include <memory>

namespace lab {

    // Forward declaration
    class Window;
    class GpuResourceManager;

    enum QueueType {
        eGraphics = 0,
        eTransfer = 1,
        eCompute = 2,
        ePresent = 3
    };

    struct DeviceQueue {
        uint32_t family_index;
        vk::Queue queue;
    };

    struct AllocatedImage {
        vk::Image image{};
        vk::ImageView image_view{};
        VmaAllocation allocation{};
    };

    struct UploadContext {
        vk::Fence upload_fence;
        vk::CommandPool command_pool;
        vk::CommandBuffer command_buffer;
    };

    class GraphicsDevice {
    public:
        explicit GraphicsDevice(const std::unique_ptr<Window>& window);
        ~GraphicsDevice();

        [[nodiscard]] vk::Device get() const { return m_device; }
        [[nodiscard]] vk::PhysicalDevice physicalDevice() const { return m_physical_device; }
        [[nodiscard]] vk::Instance instance() const { return m_instance; }
        [[nodiscard]] vk::SurfaceKHR surface() const { return m_surface; }
        [[nodiscard]] VmaAllocator allocator() const { return m_allocator; };
        [[nodiscard]] const DeviceQueue& queue(QueueType queue_type) const { return m_device_queues[queue_type]; }

        void immediateSubmit(std::function<void(vk::CommandBuffer cmd)> &&function) const;

        // dont copy or move graphics devices
        GraphicsDevice(const GraphicsDevice &) = delete;
        GraphicsDevice &operator=(const GraphicsDevice &) = delete;
        GraphicsDevice(GraphicsDevice &&) = delete;
        GraphicsDevice &operator=(GraphicsDevice &&) = delete;

    private:
        [[nodiscard]] vk::Instance createInstance() const;
        [[nodiscard]] vk::DebugUtilsMessengerEXT createDebugMessenger() const;
        [[nodiscard]] vk::PhysicalDevice pickPhysicalDevice() const;
        [[nodiscard]] std::array<uint32_t, 4> getQueueFamilyIndices() const;
        [[nodiscard]] std::pair<vk::Device, std::array<vk::Queue, 4>> createDeviceAndQueues() const;
        [[nodiscard]] VmaAllocator createAllocator() const;

        vk::Instance m_instance{};
        vk::DebugUtilsMessengerEXT m_debug_messenger{};
        vk::PhysicalDevice m_physical_device{};
        std::array<DeviceQueue, 4> m_device_queues{};
        vk::SurfaceKHR m_surface{};
        vk::Device m_device{};
        VmaAllocator m_allocator{};
        UploadContext m_upload_context{};

        std::vector<const char *> m_required_instance_layers{};
        std::vector<const char *> m_required_instance_extensions{};
        std::vector<const char *> m_required_device_extensions{};
        vk::PhysicalDeviceFeatures m_required_device_features{};

        static constexpr bool m_enable_validation_layers = true;

        // helpers
        static bool checkInstanceLayers(const std::vector<const char *> &layers);
        static bool checkInstanceExtensions(const std::vector<const char *> &extensions);
        static bool checkPhysicalDevice(
                vk::PhysicalDevice physical_device,
                const std::vector<const char *> &required_device_extensions,
                const vk::PhysicalDeviceFeatures &required_device_features);

        static bool
        checkDeviceExtensions(vk::PhysicalDevice physical_device,
                              const std::vector<const char *> &extensions);

        static bool checkDeviceFeatures(vk::PhysicalDevice physical_device,
                                        vk::PhysicalDeviceFeatures device_features);

        static int ratePhysicalDevice(vk::PhysicalDevice physical_device);
    };

} // namespace lab