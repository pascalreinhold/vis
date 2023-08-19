#include "window.hpp"

#include "error_logger.hpp"

#include <iostream>
#include <format>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace lab {
    Window::Window(std::string_view window_name, size_t m_width, size_t m_height)
            : m_window_name(window_name), m_width(m_width), m_height(m_height) {

        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API,GLFW_NO_API); // Don't create an OpenGL context
        glfwWindowHint(GLFW_RESIZABLE,GLFW_FALSE); // Disable window resizing [TODO: Remove this later]

        // TODO: if width or height are 0, the window will be fullscreen
        if (m_width == 0 || m_height == 0) {
            ErrorLogger::logFatalError("Window", "Width or height cannot be 0, TODO: make window fullscreen when width or height are 0");
        }

        m_window = glfwCreateWindow(
                static_cast<int>(m_width), static_cast<int>(m_height),
                m_window_name.c_str(), nullptr, nullptr);

        ErrorLogger::log("Window", "Initialized");
        ErrorLogger::log("\t", std::format("Name: {}", m_window_name));
        ErrorLogger::log("\t", std::format("Size: {}x{}", m_width, m_height));
    }

    Window::~Window() {
        glfwDestroyWindow(m_window);
        glfwTerminate();
        ErrorLogger::log("Window", "Destroyed");
    }

    bool Window::shouldClose() const { return glfwWindowShouldClose(m_window); }

    std::pair<uint32_t, uint32_t> Window::getFramebufferSize() const {
        int width, height;
        glfwGetFramebufferSize(m_window, &width, &height);
        if(width == 0 || height == 0) {
            ErrorLogger::logFatalError("Window", "Framebuffer size is 0, this should not happen");
        }
        return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
    }

    void Window::pollEvents() { glfwPollEvents(); }

    void Window::waitEvents() { glfwWaitEvents(); }

    std::vector<const char *> Window::getRequiredExtensions() {
        uint32_t glfw_extension_count = 0;
        const char **glfw_extensions =
                glfwGetRequiredInstanceExtensions(&glfw_extension_count);
        std::vector<const char *> extensions(glfw_extensions,
                                             glfw_extensions + glfw_extension_count);
        return extensions;
    }

    void Window::createSurface(vk::Instance *instance, vk::SurfaceKHR *surface) const {
        VkResult result = glfwCreateWindowSurface(
                *reinterpret_cast<VkInstance *>(instance), m_window, nullptr,
                reinterpret_cast<VkSurfaceKHR *>(surface));
        if (result != VK_SUCCESS) {
            ErrorLogger::logFatalError("Window", "Failed to create surface");
        }
    }

} // namespace lab