#include "window.hpp"
#include <iostream>

#include <GLFW/glfw3.h>

namespace lab {
    Window::Window(std::string_view window_name, size_t m_width, size_t m_height)
    : m_window_name(window_name) ,m_width(m_width), m_height(m_height) {
        std::cout << "Initializing Window " << m_width << "x" << m_height << std::endl;
    }

    Window::~Window() {
        std::cout << "Destroying Window" << std::endl;
    }

} // namespace lab