#include "perspective_camera.hpp"
#include "glm/ext/matrix_transform.hpp"

#include <glm/gtc/matrix_transform.hpp>

PerspectiveCamera::PerspectiveCamera(float fov, float aspect, float near, float far)
    : m_aspect{aspect}, m_fov{fov}, m_near{near}, m_far{far}
{
}

glm::mat4 PerspectiveCamera::projection() const noexcept
{
    return glm::perspective(glm::radians(m_fov), m_aspect, m_near, m_far);
}

glm::mat4 PerspectiveCamera::view() noexcept
{
    pitch = glm::clamp(pitch, -89.0f, 89.0f);

    glm::vec3 dir;
    dir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    dir.y = sin(glm::radians(pitch));
    dir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    m_forward = glm::normalize(dir);

    return glm::lookAt(position, position + m_forward, m_up);
}
