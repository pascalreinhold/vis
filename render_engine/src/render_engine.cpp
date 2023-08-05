#include "render_engine.hpp"
#include "window.hpp"

#include <iostream>

namespace lab {
    RenderEngine::RenderEngine() {
        std::cout << "Initializing Engine" << std::endl;
        m_window = std::make_unique<Window>("My Window", 800, 600);
    }

    RenderEngine::~RenderEngine() {
        std::cout << "Destroying Engine" << std::endl;
    }
}
