#pragma once

#include "math.hpp"

namespace lab {

class FirstPersonCamera {
public:
    [[nodiscard]] static mat4 viewMatrix(vec3 position, vec3 yxz_euler_angle);
    [[nodiscard]] static mat4 projectionMatrix(float aspect);
    [[nodiscard]] static mat4 viewProjectionMatrix(vec3 position, vec3 yxz_euler_angle, float aspect);
private:
    static constexpr vec3 m_forward{0.f, 0.f, -1.f};
    static constexpr vec3 m_up{0.f, -1.f, 0.f};
    static constexpr float m_fov_y = 70.f;
    static constexpr float m_near = 30.f;
    static constexpr float m_far = 5000.f;
};

} // namespace lab