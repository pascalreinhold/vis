#include "window.hpp"

#include "error_logger.hpp"

#include <format>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace lab {
    Window::Window(std::string_view window_name, size_t m_width, size_t m_height)
            : m_window_name(window_name), m_width(m_width), m_height(m_height) {

        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API,GLFW_NO_API); // Don't create an OpenGL context

        // TODO: if width or height are 0, the window will be fullscreen
        if (m_width == 0 || m_height == 0) {
            ErrorLogger::logFatalError("Window", "Width or height cannot be 0, TODO: make window fullscreen when width or height are 0");
        }

        m_window = glfwCreateWindow(
                static_cast<int>(m_width), static_cast<int>(m_height),
                m_window_name.c_str(), nullptr,nullptr);

        // the glfw needs this pointer so we can use it in the callback functions
        glfwSetWindowUserPointer(m_window, this);

        m_keyboard = std::make_unique<Keyboard>(m_window);
        m_mouse = std::make_unique<Mouse>(m_window);

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

    Mouse::Mouse(GLFWwindow *window)
        : m_window(window)
    {
        ErrorLogger::log("Mouse", "Initialized");

        m_glfw_button_map = {
                {GLFW_MOUSE_BUTTON_LEFT, MouseButtons::eLeft},
                {GLFW_MOUSE_BUTTON_RIGHT, MouseButtons::eRight},
                {GLFW_MOUSE_BUTTON_MIDDLE, MouseButtons::eMiddle}
        };

        auto glfw_mouse_button_callback = [](GLFWwindow* window, int button, int action, int /*mods*/) {
            auto& mouse_parent_window = *reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
            auto& mouse = *mouse_parent_window.m_mouse;

            // check if the key is in the map
            auto button_it = mouse.m_glfw_button_map.find(button);
            if(button_it == mouse.m_glfw_button_map.end()) {
                return;
            }

            if (action == GLFW_PRESS) {
                mouse.m_pressed[button_it->second] = {true, false};
                mouse.m_held[button_it->second] = {true, false};
            } else if (action == GLFW_RELEASE) {
                mouse.m_released[button_it->second] = {true, false};
                mouse.m_held[button_it->second] = {false, false};
            }
        };
        glfwSetMouseButtonCallback(m_window, glfw_mouse_button_callback);

        auto glfw_cursor_pos_callback = [](GLFWwindow* window, double x_pos, double y_pos) {
            auto& mouse_parent_window = *reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
            auto& mouse = *mouse_parent_window.m_mouse;

            mouse.m_cursor_delta = {static_cast<float>(x_pos) - mouse.m_cursor_position[0],
                                    static_cast<float>(y_pos) - mouse.m_cursor_position[1]};
            mouse.m_cursor_position = {static_cast<float>(x_pos), static_cast<float>(y_pos)};
        };
        glfwSetCursorPosCallback(m_window, glfw_cursor_pos_callback);

        auto glfw_scroll_callback = [](GLFWwindow* window, double /*x_offset*/, double y_offset) {
            auto& mouse_parent_window = *reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
            auto& mouse = *mouse_parent_window.m_mouse;
            mouse.m_wheel_delta = static_cast<float>(y_offset);
        };
        glfwSetScrollCallback(m_window, glfw_scroll_callback);
    }

    void Mouse::enableCursor() {
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        m_cursor_enabled = true;
    }

    void Mouse::disableCursor() {
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        m_cursor_enabled = false;
    }

    void Mouse::toggleCursor() {
        if(isCursorEnabled()) {
            disableCursor();
        } else {
            enableCursor();
        }
    }

    void Mouse::prepareNextFrame() {
        // reset pressed and released keys and the fetched state
        std::fill(m_pressed.begin(), m_pressed.end(), button_state{false, false});
        std::fill(m_released.begin(), m_released.end(), button_state{false, false});
        for(auto& button : m_held) {
            button.second = false;
        }

        // reset cursor delta
        m_cursor_delta = {0.0f, 0.0f};
        m_wheel_delta = 0.0f;
    }

    Mouse::button_state Mouse::isHeld(MouseButtons button) {
        auto state = m_held[button];
        m_held[button].second = true;
        return state;
    }

    Mouse::button_state Mouse::isPressed(MouseButtons button) {
        auto state = m_pressed[button];
        m_pressed[button].second = true;
        return state;
    }

    Mouse::button_state Mouse::isReleased(MouseButtons button) {
        auto state = m_released[button];
        m_released[button].second = true;
        return state;
    }

    Mouse::~Mouse() {
        ErrorLogger::log("Mouse", "Destroyed");
    }


    Keyboard::Keyboard(GLFWwindow* window) {
        ErrorLogger::log("Keyboard", "Initialized");

        m_glfw_key_map = {
                {GLFW_KEY_W, KeyboardButtons::eW},
                {GLFW_KEY_A, KeyboardButtons::eA},
                {GLFW_KEY_S, KeyboardButtons::eS},
                {GLFW_KEY_D, KeyboardButtons::eD},
                {GLFW_KEY_Q, KeyboardButtons::eQ},
                {GLFW_KEY_E, KeyboardButtons::eE},
                {GLFW_KEY_R, KeyboardButtons::eR},
                {GLFW_KEY_F, KeyboardButtons::eF},
                {GLFW_KEY_LEFT_SHIFT, KeyboardButtons::eLeftShift},
                {GLFW_KEY_SPACE, KeyboardButtons::eSpace},
                {GLFW_KEY_LEFT_CONTROL, KeyboardButtons::eLeftControl},
                {GLFW_KEY_ESCAPE, KeyboardButtons::eEscape},
                {GLFW_KEY_TAB, KeyboardButtons::eTab}
        };

        if(m_glfw_key_map.size() != eKeyboardButtonCount) {
                ErrorLogger::logFatalError("Keyboard", "m_glfw_key_map size is not equal to KeyboardButtons::eCount");
        }

        auto glfw_key_callback = [](GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/) {
            auto& keyboard_parent_window = *reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
            auto& keyboard = *keyboard_parent_window.m_keyboard;

            // check if the key is in the map
            auto button_it = keyboard.m_glfw_key_map.find(key);
            if(button_it == keyboard.m_glfw_key_map.end()) {
                return;
            }

            if (action == GLFW_PRESS) {
                keyboard.m_pressed[button_it->second] = {true, false};
                keyboard.m_held[button_it->second] = {true, false};
            } else if (action == GLFW_RELEASE) {
                keyboard.m_released[button_it->second] = {true, false};
                keyboard.m_held[button_it->second] = {false, false};
            }
        };

        glfwSetKeyCallback(window, glfw_key_callback);
    }

    Keyboard::~Keyboard() {
        ErrorLogger::log("Keyboard", "Destroyed");
    }

    void Keyboard::prepareNextFrame() {
        // reset pressed and released keys and the fetched state
        std::fill(m_pressed.begin(), m_pressed.end(), button_state{false, false});
        std::fill(m_released.begin(), m_released.end(), button_state{false, false});
        for(auto& button : m_held) {
            button.second = false;
        }
    }

    Keyboard::button_state Keyboard::isHeld(KeyboardButtons key) {
        auto state = m_held[key];
        m_held[key].second = true;
        return state;
    }

    Keyboard::button_state Keyboard::isPressed(KeyboardButtons key) {
        auto state = m_pressed[key];
        m_pressed[key].second = true;
        return state;
    }

    Keyboard::button_state Keyboard::isReleased(KeyboardButtons key) {
        auto state = m_released[key];
        m_released[key].second = true;
        return state;
    }

} // namespace lab