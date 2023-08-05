#pragma once

#include <vulkan/vulkan.hpp>
#include <memory>

namespace lab {

// Forward declaration
class Window;

class RenderEngine {
    public:
        RenderEngine();
        ~RenderEngine();
    private:
        std::unique_ptr<Window> m_window{};
};

} // namespace lab
