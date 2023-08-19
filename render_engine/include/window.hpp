#pragma once

#include <cstddef>
#include <string_view>
#include <utility>
#include <vector>

// Forward declaration
struct GLFWwindow;
namespace vk {
    class Instance;
    class SurfaceKHR;
} // namespace vk

namespace lab {

    class Window {
    public:
        // if width or height are 0, the window will be fullscreen
        explicit Window(std::string_view window_name, size_t m_width = 0, size_t m_height = 0);

        ~Window();

        // don't copy or move windows
        Window(const Window &) = delete;
        Window &operator=(const Window &) = delete;
        Window(Window &&) = delete;
        Window &operator=(Window &&) = delete;

        // caller is responsible for destroying the surface
        void createSurface(vk::Instance *instance, vk::SurfaceKHR *surface) const;

        [[nodiscard]] size_t width() const { return m_width; }
        [[nodiscard]] size_t height() const { return m_height; }
        [[nodiscard]] float width_f() const { return static_cast<float>(m_width); }
        [[nodiscard]] float height_f() const { return static_cast<float>(m_height); }
        [[nodiscard]] std::pair<uint32_t, uint32_t> getFramebufferSize() const;
        [[nodiscard]] float aspectRatio() const { return width_f() / height_f(); }

        [[nodiscard]] bool shouldClose() const;
        // static because this is related to the context of glfw and not the window itself
        static void pollEvents();
        static void waitEvents();
        static std::vector<const char *> getRequiredExtensions();

    private:
        size_t m_width;
        size_t m_height;
        std::string m_window_name;
        GLFWwindow *m_window{};
    };

} // namespace lab