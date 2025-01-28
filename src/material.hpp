#pragma once

#include <cstdint>

class Material
{
  public:
    Material(const char *vertex_shader_path, const char *fragment_shader_path);
    ~Material();

    Material(const Material &) = delete;
    Material(Material &&) = delete;
    Material &operator=(const Material &) = delete;
    Material &operator=(Material &&) = delete;

    [[nodiscard]] uint32_t id() const noexcept { return m_id; }

  private:
    uint32_t m_id;
};
