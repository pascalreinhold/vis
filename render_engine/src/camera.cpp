#include "camera.hpp"

namespace lab {

    mat4 FirstPersonCamera::viewMatrix(vec3 position, vec3 yxz_euler_angle) {
        mat3 rot = eulerAngleYXZ(yxz_euler_angle.x, yxz_euler_angle.y, yxz_euler_angle.z);
        return lookAt(position, position + rot * m_forward, rot * m_up);
    }

    mat4 FirstPersonCamera::projectionMatrix(float aspect) {
        return perspective(m_fov_y, aspect, m_near, m_far);
    }

    mat4 FirstPersonCamera::viewProjectionMatrix(vec3 position, vec3 yxz_euler_angle, float aspect) {
        return projectionMatrix(aspect) * viewMatrix(position, yxz_euler_angle);
    }

} // namespace lab