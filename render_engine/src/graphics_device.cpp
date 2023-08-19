#include "graphics_device.hpp"
#include "error_logger.hpp"
#include "window.hpp"

#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>

#include <iostream>
#include <set>
#include <utility>

namespace lab {

// helper functions in anonymous namespace
    namespace {

        // RT = Required Container Type, AT = Available Container Type
        template<typename RT, typename AT>
        bool check_required_available_strings(
                const std::vector<RT> &required,
                std::function<const char *(const RT &)> get_required_string,
                const std::vector<AT> &available,
                std::function<const char *(const AT &)> get_available_string) {
            for (const auto &required_string : required) {
                bool found = false;
                for (const auto &available_string : available) {
                    if (strcmp(get_required_string(required_string),
                               get_available_string(available_string)) == 0) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    return false;
                }
            }
            return true;
        }

    } // namespace

    static VKAPI_ATTR VkBool32 VKAPI_CALL validation_layer_callback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT /*messageType*/,
            const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
            void * /*pUserData*/) {
        if (messageSeverity < VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
            return VK_TRUE;
        }
        ErrorLogger::log("Validation Layer", pCallbackData->pMessage);
        return VK_FALSE;
    }

    VkResult CreateDebugUtilsMessengerEXT(
            VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
            const VkAllocationCallbacks *pAllocator,
            VkDebugUtilsMessengerEXT *pDebugMessenger) {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(
                instance, "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr) {
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        } else {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    void DestroyDebugUtilsMessengerEXT(VkInstance instance,
                                       VkDebugUtilsMessengerEXT debugMessenger,
                                       const VkAllocationCallbacks *pAllocator) {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(
                instance, "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr) {
            func(instance, debugMessenger, pAllocator);
        }
    }

    GraphicsDevice::GraphicsDevice(const std::unique_ptr<Window>& window) {

        // populate required extensions, layers and features fields
        m_required_instance_extensions = window->getRequiredExtensions();

        if (m_enable_validation_layers) {
            m_required_instance_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
            m_required_instance_layers.push_back("VK_LAYER_KHRONOS_validation");
        }

        m_required_device_extensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
        m_required_device_features.geometryShader = true;

        // create instance, debug messenger, surface, physical device
        m_instance = createInstance();
        if (m_enable_validation_layers) {
            m_debug_messenger = createDebugMessenger();
        }

        window->createSurface(&m_instance, &m_surface);
        m_physical_device = pickPhysicalDevice();

        // create device and queues
        std::array<uint32_t, 4> queue_family_indices = getQueueFamilyIndices();
        for (size_t i = 0; i < m_device_queues.size(); ++i) {
            m_device_queues[i].family_index = queue_family_indices[i];
        }
        std::array<vk::Queue, 4> queue_holder{};
        std::tie(m_device, queue_holder) = createDeviceAndQueues();
        for (size_t i = 0; i < m_device_queues.size(); ++i) {
            m_device_queues[i].queue = queue_holder[i];
        }

        m_allocator = createAllocator();

        ErrorLogger::log("GraphicsDevice", "Initialized");
    }

    GraphicsDevice::~GraphicsDevice() {
        vmaDestroyAllocator(m_allocator);
        m_device.destroy();
        m_instance.destroySurfaceKHR(m_surface);
        if (m_debug_messenger) {
            DestroyDebugUtilsMessengerEXT(
                    m_instance, static_cast<VkDebugUtilsMessengerEXT>(m_debug_messenger),
                    nullptr);
        }
        m_instance.destroy();

        ErrorLogger::log("GraphicsDevice", "Destroyed");
    }

    bool GraphicsDevice::checkInstanceExtensions(
            const std::vector<const char *> &extensions) {
        return check_required_available_strings<const char *,
                vk::ExtensionProperties>(
                extensions, [](const char *extension) { return extension; },
                vk::enumerateInstanceExtensionProperties(),
                [](const vk::ExtensionProperties &extension) {
                    return extension.extensionName;
                });
    }

    bool GraphicsDevice::checkInstanceLayers(
            const std::vector<const char *> &layers) {
        return check_required_available_strings<const char *, vk::LayerProperties>(
                layers, [](const char *layer) { return layer; },
                vk::enumerateInstanceLayerProperties(),
                [](const vk::LayerProperties &layer) { return layer.layerName; });
    }

    bool GraphicsDevice::checkDeviceExtensions(
            vk::PhysicalDevice device, const std::vector<const char *> &extensions) {
        return check_required_available_strings<const char *,
                vk::ExtensionProperties>(
                extensions, [](const char *extension) { return extension; },
                device.enumerateDeviceExtensionProperties(),
                [](const vk::ExtensionProperties &extension) {
                    return extension.extensionName;
                });
    }

    vk::Instance GraphicsDevice::createInstance() const {

        vk::ApplicationInfo app_info{"Lab Engine", VK_MAKE_VERSION(1, 0, 0),
                                     "No Engine", VK_MAKE_VERSION(1, 0, 0),
                                     VK_API_VERSION_1_3, nullptr};

        const bool layers_available = checkInstanceLayers(m_required_instance_layers);
        const bool extensions_available = checkInstanceExtensions(m_required_instance_extensions);

        if (!layers_available || !extensions_available) {
            ErrorLogger::logFatalError("GraphicsDevice","Required layers or extensions are not available");
        }

        vk::InstanceCreateInfo instance_info{{},
                                             &app_info,
                                             m_required_instance_layers,
                                             m_required_instance_extensions};
        return vk::createInstance(instance_info);
    }

    vk::DebugUtilsMessengerEXT GraphicsDevice::createDebugMessenger() const {

        vk::DebugUtilsMessengerCreateInfoEXT create_info{
                {},
                vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
                vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
                vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
                vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
                vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
                vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
                validation_layer_callback,
                nullptr};

        VkDebugUtilsMessengerEXT debug_messenger;
        VkResult result = CreateDebugUtilsMessengerEXT(
                m_instance,
                reinterpret_cast<VkDebugUtilsMessengerCreateInfoEXT *>(&create_info),
                nullptr, &debug_messenger);

        if (result != VK_SUCCESS) {
            ErrorLogger::logFatalError("GraphicsDevice","Failed to set up debug messenger");
        }

        return debug_messenger;
    }

    vk::PhysicalDevice GraphicsDevice::pickPhysicalDevice() const {

        // get available GPUs
        std::vector<vk::PhysicalDevice> physical_devices = m_instance.enumeratePhysicalDevices();

        std::vector<vk::PhysicalDevice> discrete_devices;
        std::vector<vk::PhysicalDevice> integrated_devices;

        for (auto &physical_device : physical_devices) {
            vk::PhysicalDeviceProperties properties = physical_device.getProperties();

            if (!checkPhysicalDevice(physical_device, m_required_device_extensions,
                                     m_required_device_features))
                continue;

            if (properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu) {
                discrete_devices.push_back(physical_device);
            } else if (properties.deviceType ==
                       vk::PhysicalDeviceType::eIntegratedGpu) {
                integrated_devices.push_back(physical_device);
            }
        }

        auto sort_func = [](const vk::PhysicalDevice &a,
                            const vk::PhysicalDevice &b) {
            return ratePhysicalDevice(a) > ratePhysicalDevice(b);
        };

        if (!discrete_devices.empty()) {
            std::sort(discrete_devices.begin(), discrete_devices.end(), sort_func);
            return discrete_devices[0];
        } else if (!integrated_devices.empty()) {
            std::sort(integrated_devices.begin(), integrated_devices.end(), sort_func);
            return integrated_devices[0];
        }

        ErrorLogger::logFatalError("GraphicsDevice","No suitable GPU found");
        return VK_NULL_HANDLE;
    }

    bool GraphicsDevice::checkDeviceFeatures(
            vk::PhysicalDevice physical_device,
            vk::PhysicalDeviceFeatures required_device_features) {
        VkPhysicalDeviceFeatures supported_features = physical_device.getFeatures();
        auto check = [](bool required, bool supported) {
            return !required || supported;
        };

        bool all_features_supported =
                check(required_device_features.alphaToOne,
                      supported_features.alphaToOne) &&
                check(required_device_features.depthBiasClamp,
                      supported_features.depthBiasClamp) &&
                check(required_device_features.depthBounds,
                      supported_features.depthBounds) &&
                check(required_device_features.depthClamp,
                      supported_features.depthClamp) &&
                check(required_device_features.drawIndirectFirstInstance,
                      supported_features.drawIndirectFirstInstance) &&
                check(required_device_features.dualSrcBlend,
                      supported_features.dualSrcBlend) &&
                check(required_device_features.fillModeNonSolid,
                      supported_features.fillModeNonSolid) &&
                check(required_device_features.fragmentStoresAndAtomics,
                      supported_features.fragmentStoresAndAtomics) &&
                check(required_device_features.fullDrawIndexUint32,
                      supported_features.fullDrawIndexUint32) &&
                check(required_device_features.geometryShader,
                      supported_features.geometryShader) &&
                check(required_device_features.imageCubeArray,
                      supported_features.imageCubeArray) &&
                check(required_device_features.independentBlend,
                      supported_features.independentBlend) &&
                check(required_device_features.inheritedQueries,
                      supported_features.inheritedQueries) &&
                check(required_device_features.largePoints,
                      supported_features.largePoints) &&
                check(required_device_features.logicOp, supported_features.logicOp) &&
                check(required_device_features.multiDrawIndirect,
                      supported_features.multiDrawIndirect) &&
                check(required_device_features.multiViewport,
                      supported_features.multiViewport) &&
                check(required_device_features.occlusionQueryPrecise,
                      supported_features.occlusionQueryPrecise) &&
                check(required_device_features.pipelineStatisticsQuery,
                      supported_features.pipelineStatisticsQuery) &&
                check(required_device_features.robustBufferAccess,
                      supported_features.robustBufferAccess) &&
                check(required_device_features.samplerAnisotropy,
                      supported_features.samplerAnisotropy) &&
                check(required_device_features.sampleRateShading,
                      supported_features.sampleRateShading) &&
                check(required_device_features.shaderClipDistance,
                      supported_features.shaderClipDistance) &&
                check(required_device_features.shaderCullDistance,
                      supported_features.shaderCullDistance) &&
                check(required_device_features.shaderFloat64,
                      supported_features.shaderFloat64) &&
                check(required_device_features.shaderImageGatherExtended,
                      supported_features.shaderImageGatherExtended) &&
                check(required_device_features.shaderInt16,
                      supported_features.shaderInt16) &&
                check(required_device_features.shaderInt64,
                      supported_features.shaderInt64) &&
                check(required_device_features.shaderResourceMinLod,
                      supported_features.shaderResourceMinLod) &&
                check(required_device_features.shaderResourceResidency,
                      supported_features.shaderResourceResidency) &&
                check(required_device_features.shaderSampledImageArrayDynamicIndexing,
                      supported_features.shaderSampledImageArrayDynamicIndexing) &&
                check(required_device_features.shaderStorageBufferArrayDynamicIndexing,
                      supported_features.shaderStorageBufferArrayDynamicIndexing) &&
                check(required_device_features.shaderStorageImageArrayDynamicIndexing,
                      supported_features.shaderStorageImageArrayDynamicIndexing) &&
                check(required_device_features.shaderStorageImageExtendedFormats,
                      supported_features.shaderStorageImageExtendedFormats) &&
                check(required_device_features.shaderStorageImageMultisample,
                      supported_features.shaderStorageImageMultisample) &&
                check(required_device_features.shaderStorageImageReadWithoutFormat,
                      supported_features.shaderStorageImageReadWithoutFormat) &&
                check(required_device_features.shaderStorageImageWriteWithoutFormat,
                      supported_features.shaderStorageImageWriteWithoutFormat) &&
                check(required_device_features.shaderTessellationAndGeometryPointSize,
                      supported_features.shaderTessellationAndGeometryPointSize) &&
                check(required_device_features.shaderUniformBufferArrayDynamicIndexing,
                      supported_features.shaderUniformBufferArrayDynamicIndexing) &&
                check(required_device_features.sparseBinding,
                      supported_features.sparseBinding) &&
                check(required_device_features.sparseResidency16Samples,
                      supported_features.sparseResidency16Samples) &&
                check(required_device_features.sparseResidency2Samples,
                      supported_features.sparseResidency2Samples) &&
                check(required_device_features.sparseResidency4Samples,
                      supported_features.sparseResidency4Samples) &&
                check(required_device_features.sparseResidency8Samples,
                      supported_features.sparseResidency8Samples) &&
                check(required_device_features.sparseResidencyAliased,
                      supported_features.sparseResidencyAliased) &&
                check(required_device_features.sparseResidencyBuffer,
                      supported_features.sparseResidencyBuffer) &&
                check(required_device_features.sparseResidencyImage2D,
                      supported_features.sparseResidencyImage2D) &&
                check(required_device_features.sparseResidencyImage3D,
                      supported_features.sparseResidencyImage3D) &&
                check(required_device_features.tessellationShader,
                      supported_features.tessellationShader) &&
                check(required_device_features.textureCompressionASTC_LDR,
                      supported_features.textureCompressionASTC_LDR) &&
                check(required_device_features.textureCompressionBC,
                      supported_features.textureCompressionBC) &&
                check(required_device_features.textureCompressionETC2,
                      supported_features.textureCompressionETC2) &&
                check(required_device_features.variableMultisampleRate,
                      supported_features.variableMultisampleRate) &&
                check(required_device_features.vertexPipelineStoresAndAtomics,
                      supported_features.vertexPipelineStoresAndAtomics) &&
                check(required_device_features.wideLines, supported_features.wideLines);

        return all_features_supported;
    }

// TODO: implement check for swapchain support [but it is not really necessary]
    bool GraphicsDevice::checkPhysicalDevice(
            vk::PhysicalDevice physical_device,
            const std::vector<const char *> &required_device_extensions,
            const vk::PhysicalDeviceFeatures &required_device_features) {
        bool suitable = true;
        suitable &= checkDeviceFeatures(physical_device, required_device_features);
        suitable &= checkDeviceExtensions(physical_device, required_device_extensions);
        return suitable;
    }

// TODO: implement [higher score is a better device]
    int GraphicsDevice::ratePhysicalDevice(vk::PhysicalDevice) {
        return 0;
    }

    std::array<uint32_t, 4> GraphicsDevice::getQueueFamilyIndices() const {
        std::vector<vk::QueueFamilyProperties> queue_family_properties =
                m_physical_device.getQueueFamilyProperties();

        std::array<uint32_t, 4> queue_indices = {0, 0, 0, 0};

        bool graphics_queue_found = false, compute_queue_found = false,
                transfer_queue_found = false, present_queue_found = false;

        // try to find dedicated queues first
        for (int i = 0; i < queue_family_properties.size(); ++i) {
            if (((queue_family_properties[i].queueFlags |
                  vk::QueueFlagBits::eGraphics) == vk::QueueFlagBits::eGraphics) &&
                !graphics_queue_found) {
                queue_indices[eGraphics] = i;
                graphics_queue_found = true;
            }
            if (((queue_family_properties[i].queueFlags |
                  vk::QueueFlagBits::eTransfer) == vk::QueueFlagBits::eTransfer) &&
                !transfer_queue_found) {
                queue_indices[eTransfer] = i;
                transfer_queue_found = true;
            }
            if (((queue_family_properties[i].queueFlags |
                  vk::QueueFlagBits::eCompute) == vk::QueueFlagBits::eCompute) &&
                !compute_queue_found) {
                queue_indices[eCompute] = i;
                compute_queue_found = true;
            }
            if (m_physical_device.getSurfaceSupportKHR(i, m_surface) &&
                !present_queue_found) {
                queue_indices[ePresent] = i;
                present_queue_found = true;
            }
        }

        // if no dedicated queues are found, try to find a queue that supports
        // multiple operations
        for (int i = 0; i < queue_family_properties.size(); ++i) {
            if ((queue_family_properties[i].queueFlags &
                 vk::QueueFlagBits::eGraphics) &&
                !graphics_queue_found) {
                queue_indices[eGraphics] = i;
                graphics_queue_found = true;
            }
            if ((queue_family_properties[i].queueFlags &
                 vk::QueueFlagBits::eTransfer) &&
                !transfer_queue_found) {
                queue_indices[eTransfer] = i;
                transfer_queue_found = true;
            }
            if ((queue_family_properties[i].queueFlags & vk::QueueFlagBits::eCompute) &&
                !compute_queue_found) {
                queue_indices[eCompute] = i;
                compute_queue_found = true;
            }
            if (m_physical_device.getSurfaceSupportKHR(i, m_surface) &&
                !present_queue_found) {
                queue_indices[ePresent] = i;
                present_queue_found = true;
            }
        }

        if (!graphics_queue_found)
            ErrorLogger::logFatalError("GraphicsDevice","No graphics queue found");
        if (!compute_queue_found)
            ErrorLogger::logFatalError("GraphicsDevice","No compute queue found");
        if (!transfer_queue_found)
            ErrorLogger::logFatalError("GraphicsDevice","No transfer queue found");
        if (!present_queue_found)
            ErrorLogger::logFatalError("GraphicsDevice","No present queue found");

        return queue_indices;
    }

    std::pair<vk::Device, std::array<vk::Queue, 4>>
    GraphicsDevice::createDeviceAndQueues() const {

        std::set<uint32_t> unique_family_indices{
                m_device_queues[eGraphics].family_index,
                m_device_queues[ePresent].family_index,
                m_device_queues[eCompute].family_index,
                m_device_queues[eTransfer].family_index};
        std::vector<vk::DeviceQueueCreateInfo> queue_create_infos(unique_family_indices.size());
        std::vector<float> queue_priorities{1.0f};

        int i = 0;
        for (uint32_t family_index : unique_family_indices) {
            queue_create_infos[i++] = vk::DeviceQueueCreateInfo{{}, family_index, queue_priorities};
        }

        // TODO: implement checking for 1.3 features

        vk::PhysicalDeviceVulkan13Features required_vulkan_13_features{};
        required_vulkan_13_features.dynamicRendering = VK_TRUE;
        vk::PhysicalDeviceFeatures2 device_features{};
        device_features.features = m_required_device_features;
        device_features.pNext = &required_vulkan_13_features;

        vk::DeviceCreateInfo device_info{{},
                                         queue_create_infos,
                                         m_required_instance_layers,
                                         m_required_device_extensions,
                                         nullptr};
        device_info.pNext = &device_features;
        vk::Device device = m_physical_device.createDevice(device_info);

        std::array<vk::Queue, 4> queues{};
        queues[eGraphics] =
                device.getQueue(m_device_queues[eGraphics].family_index, 0);
        queues[ePresent] = device.getQueue(m_device_queues[ePresent].family_index, 0);
        queues[eCompute] = device.getQueue(m_device_queues[eCompute].family_index, 0);
        queues[eTransfer] =
                device.getQueue(m_device_queues[eTransfer].family_index, 0);

        return {device, queues};
    }

    VmaAllocator GraphicsDevice::createAllocator() const {
        VmaAllocatorCreateInfo allocatorInfo{};
        allocatorInfo.physicalDevice = m_physical_device;
        allocatorInfo.device = m_device;
        allocatorInfo.instance = m_instance;

        VmaVulkanFunctions func = {};
        func.vkGetInstanceProcAddr = vkGetInstanceProcAddr;
        func.vkGetDeviceProcAddr = vkGetDeviceProcAddr;
        allocatorInfo.pVulkanFunctions = &func;

        VmaAllocator allocator;
        vmaCreateAllocator(&allocatorInfo, &allocator);
        return allocator;
    }

} // namespace lab