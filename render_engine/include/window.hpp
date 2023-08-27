#pragma once

#include <cstddef>
#include <string_view>
#include <memory>
#include <utility>
#include <vector>
#include <array>
#include <map>

// Forward declaration
struct GLFWwindow;

namespace vk {
    class Instance;
    class SurfaceKHR;
} // namespace vk

namespace lab {

    enum MouseButtons : size_t {
        eLeft = 0,
        eRight = 1,
        eMiddle = 2,
        eMouseButtonCount = eMiddle + 1
    };

    enum KeyboardButtons : size_t {
        eW = 0,
        eA = 1,
        eS = 2,
        eD = 3,
        eQ = 4,
        eE = 5,
        eR = 6,
        eF = 7,
        eLeftShift = 8,
        eSpace = 9,
        eLeftControl = 10,
        eEscape = 11,
        eTab = 12,
        eKeyboardButtonCount = eTab + 1
    };

    class Mouse {
    public:
        explicit Mouse(GLFWwindow* window);
        ~Mouse();

        // don't copy or move mice
        Mouse(const Mouse &) = delete;
        Mouse &operator=(const Mouse &) = delete;
        Mouse(Mouse &&) = delete;
        Mouse &operator=(Mouse &&) = delete;

        void prepareNextFrame(); // call this once per frame, resets the pressed and released keys and the fetched state

        using button_state = std::pair<bool, bool>; // first is pressed, second is if state was fetched this frame
        [[nodiscard]] button_state isHeld(MouseButtons key);
        [[nodiscard]] button_state isPressed(MouseButtons key);
        [[nodiscard]] button_state isReleased(MouseButtons key);

        [[nodiscard]] std::array<float, 2> position() const { return m_cursor_position; }
        [[nodiscard]] std::array<float, 2> delta() const { return m_cursor_delta; }
        [[nodiscard]] float wheelDelta() const { return m_wheel_delta; }

        void enableCursor();
        void disableCursor();
        void toggleCursor();
        [[nodiscard]] bool isCursorEnabled() const { return m_cursor_enabled; }

    private:
        bool m_cursor_enabled{true};
        std::array<button_state, eMouseButtonCount> m_pressed{button_state{false, false}};
        std::array<button_state, eMouseButtonCount> m_held{button_state{false, false}};
        std::array<button_state, eMouseButtonCount> m_released{button_state{false, false}};
        std::map<int, MouseButtons> m_glfw_button_map;

        std::array<float, 2> m_cursor_position{};
        std::array<float, 2> m_cursor_delta{};
        float m_wheel_delta{};

        GLFWwindow* m_window{}; // does not own the window
    };

    class Keyboard {
    public:
        explicit Keyboard(GLFWwindow* window); // set up all the glfw callbacks and key map
        ~Keyboard();

        // don't copy or move keyboards
        Keyboard(const Keyboard &) = delete;
        Keyboard &operator=(const Keyboard &) = delete;
        Keyboard(Keyboard &&) = delete;
        Keyboard &operator=(Keyboard &&) = delete;

        void prepareNextFrame(); // call this once per frame, resets the pressed and released keys and the fetched state

        using button_state = std::pair<bool, bool>; // first is pressed, second is if state was fetched this frame
        [[nodiscard]] button_state isHeld(KeyboardButtons key);
        [[nodiscard]] button_state isPressed(KeyboardButtons key);
        [[nodiscard]] button_state isReleased(KeyboardButtons key);
    private:
        std::array<button_state, eKeyboardButtonCount> m_pressed{button_state{false, false}};
        std::array<button_state, eKeyboardButtonCount> m_held{button_state{false, false}};
        std::array<button_state, eKeyboardButtonCount> m_released{button_state{false, false}};
        std::map<int, KeyboardButtons> m_glfw_key_map;
    };

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
        [[nodiscard]] float aspect() const { return width_f() / height_f(); }

        GLFWwindow *glfwWindow() { return m_window; }
        Keyboard& keyboard() { return *m_keyboard; }
        Mouse& mouse() { return *m_mouse; }
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

        std::unique_ptr<Keyboard> m_keyboard;
        std::unique_ptr<Mouse> m_mouse;

        friend class Keyboard; // so the keyboard can access itself from the glfw callbacks
        friend class Mouse; // so the mouse can access itself from the glfw callbacks
    };

} // namespace lab