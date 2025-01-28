#pragma once

#include <glm/mat4x4.hpp>

class PerspectiveCamera
{
  public:
    PerspectiveCamera(float fov, float aspect, float near, float far);

    glm::vec3 position{0.0f, 0.0f, 3.0f};
    float pitch{0.0f};
    float yaw{0.0f};

    float move_speed{1.0f};
    float rotation_speed{50.0f};

    [[nodiscard]] glm::mat4 projection() const noexcept;
    [[nodiscard]] glm::mat4 view() noexcept;

    [[nodiscard]] glm::vec3 forward() const noexcept { return m_forward; }
    [[nodiscard]] glm::vec3 up() const noexcept { return m_up; }

  private:
    float m_aspect;
    float m_near;
    float m_far;
    float m_fov;

    glm::vec3 m_forward{0.0f, 0.0f, -1.0f};
    /*glm::vec3 m_right{1.0f, 0.0f, 1.0f};*/
    glm::vec3 m_up{0.0f, 1.0f, 0.0f};
};
