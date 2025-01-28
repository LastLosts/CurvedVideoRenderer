#include "mesh.hpp"

#include <glad/gl.h>

Mesh::Mesh() : m_element_buffer{0}, m_vertex_array{0}, m_vertex_buffer{0}, m_indices_count{0}
{
}

Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices) : m_indices_count{indices.size()}
{
    glGenVertexArrays(1, &m_vertex_array);
    glGenBuffers(1, &m_element_buffer);
    glGenBuffers(1, &m_vertex_buffer);

    glBindVertexArray(m_vertex_array);

    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_element_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indices.size(), indices.data(), GL_STATIC_DRAW);

    // TODO: Automate this???
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
Mesh::~Mesh()
{
    glDeleteBuffers(1, &m_vertex_buffer);
    glDeleteBuffers(1, &m_element_buffer);
    glDeleteVertexArrays(1, &m_vertex_array);
}
Mesh &Mesh::operator=(Mesh &&other)
{
    std::swap(m_vertex_buffer, other.m_vertex_buffer);
    std::swap(m_vertex_array, other.m_vertex_array);
    std::swap(m_element_buffer, other.m_element_buffer);
    std::swap(m_indices_count, other.m_indices_count);
    return *this;
}
Mesh::Mesh(Mesh &&other)
{
    std::swap(m_vertex_buffer, other.m_vertex_buffer);
    std::swap(m_vertex_array, other.m_vertex_array);
    std::swap(m_element_buffer, other.m_element_buffer);
    std::swap(m_indices_count, other.m_indices_count);
}
