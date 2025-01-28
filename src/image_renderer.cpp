#include "image_renderer.hpp"
#include "glm/ext/matrix_transform.hpp"
#include <inttypes.h>

#include <glad/gl.h>

#include <glm/gtc/type_ptr.hpp>

constexpr char *vertex_shader_path = "./shaders/test.vert";
constexpr char *fragment_shader_path = "./shaders/horizontally_flipped_texture.frag";

ImageRenderer::ImageRenderer() : m_material{vertex_shader_path, fragment_shader_path}
{
    /*float vertices[] = {*/
    /*    // Pos              // Tex coords*/
    /*    1.0f,  1.0f,  0.0f, 1.0f, 1.0f, // Top Right*/
    /*    1.0f,  -1.0f, 0.0f, 1.0f, 0.0f, // Bottom Right*/
    /*    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // Bottom Left*/
    /*    -1.0f, 1.0f,  0.0f, 0.0f, 1.0f, // Top Left*/
    /*};*/
    /**/
    /*m_indices = std::vector<uint32_t>{0, 1, 3, 1, 2, 3};*/

    /*uint32_t vbo;*/
    /*glGenVertexArrays(1, &m_vao);*/
    /*glGenBuffers(1, &vbo);*/
    /*glGenBuffers(1, &m_ebo);*/

    /*glBindVertexArray(m_vao);*/

    /*glBindBuffer(GL_ARRAY_BUFFER, vbo);*/
    /*glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);*/

    /*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);*/
    /*glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * m_indices.size(), m_indices.data(), GL_STATIC_DRAW);*/

    /*glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);*/
    /*glEnableVertexAttribArray(0);*/
    /**/
    /*glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));*/
    /*glEnableVertexAttribArray(1);*/

    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glEnable(GL_DEPTH_TEST);
}

void ImageRenderer::render(const Mesh &mesh, const Frame &frame, PerspectiveCamera &camera)
{
    glm::mat4 model{1.0f};
    glm::mat4 projection = camera.projection();
    glm::mat4 view = camera.view();

    model = glm::scale(model, {2.0f, 1.0f, 2.0f});

    int projection_loc = glGetUniformLocation(m_material.id(), "camera_projection");
    int model_loc = glGetUniformLocation(m_material.id(), "model");
    int view_loc = glGetUniformLocation(m_material.id(), "camera_view");

    glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection));

    glClearColor(0.2f, 0.1f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(m_material.id());
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(m_material.id(), "u_texture"), 0);

    glBindTexture(GL_TEXTURE_2D, m_texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame.width, frame.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, frame.data.data());
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindVertexArray(mesh.vertex_array());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.element_buffer());

    glDrawElements(GL_TRIANGLES, mesh.indices_count(), GL_UNSIGNED_INT, 0);
}
