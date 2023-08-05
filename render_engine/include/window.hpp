#pragma once

#include <cstddef>
#include <string_view>
#include <string>

namespace lab {

class Window {
    public:
        // if width or height are 0, the window will be fullscreen
        explicit Window(std::string_view window_name, size_t m_width = 0, size_t m_height = 0);
        ~Window();

        [[nodiscard]] size_t width() const { return m_width;}
        [[nodiscard]] size_t height() const { return m_height;}
        [[nodiscard]] float width_f() const { return static_cast<float>(m_width);}
        [[nodiscard]] float height_f() const { return static_cast<float>(m_height);}
        [[nodiscard]] float aspect_ratio() const { return width_f() / height_f();}

    private:
        size_t m_width;
        size_t m_height;
        std::string m_window_name;
};

}