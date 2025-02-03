#pragma once

#include <inttypes.h>
#include <vector>

#include "glm/trigonometric.hpp"
#include "vertex.hpp"

class Mesh
{
  public:
    Mesh();
    Mesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices);
    ~Mesh();

    Mesh(const Mesh &) = delete;
    Mesh(Mesh &&other);
    Mesh &operator=(const Mesh &) = delete;
    Mesh &operator=(Mesh &&other);

    [[nodiscard]] uint32_t vertex_array() const noexcept { return m_vertex_array; }
    [[nodiscard]] uint32_t element_buffer() const noexcept { return m_element_buffer; }
    [[nodiscard]] uint32_t indices_count() const noexcept { return m_indices_count; }

  private:
    uint32_t m_vertex_array;
    uint32_t m_vertex_buffer;
    uint32_t m_element_buffer;

    size_t m_indices_count;
};

// Angles are in degree not radians
static Mesh generate_mesh_curved_plane(uint32_t width, uint32_t height, float curve_angle)
{
    std::vector<Vertex> vertices;
    vertices.reserve(height * width);
    std::vector<uint32_t> indices;
    indices.reserve((height - 1) * (width - 1) * 6);

    int32_t half_height = height / 2;
    int32_t half_width = width / 2;

    float angle_spacing = curve_angle / (float)width;

    for (int32_t y = -half_height; y < half_height; ++y)
    {
        for (int32_t x = -half_width; x < half_width; ++x)
        {
            Vertex v;
            v.position = {glm::cos(glm::radians(angle_spacing * x)), y * 0.005f,
                          glm::sin(glm::radians(angle_spacing * x))};

            v.uv = {(float)(x + half_width) / (float)width, (float)(y + half_height) / (float)height};

            vertices.push_back(v);
        }
    }

    for (uint32_t x = 0; x < width - 1; ++x)
    {
        for (uint32_t y = 0; y < height - 1; ++y)
        {
            indices.push_back(y * width + x);
            indices.push_back((y + 1) * width + x + 1);
            indices.push_back(y * width + x + 1);

            indices.push_back(y * width + x);
            indices.push_back((y + 1) * width + x);
            indices.push_back((y + 1) * width + x + 1);
        }
    }

    return {vertices, indices};
}
