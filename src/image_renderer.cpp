#include "image_renderer.hpp"
#include "glm/ext/matrix_transform.hpp"
#include <inttypes.h>

#include <glad/gl.h>

#include <glm/gtc/type_ptr.hpp>

constexpr char *vertex_shader_path = "./shaders/test.vert";
constexpr char *fragment_shader_path = "./shaders/horizontally_flipped_texture.frag";

ImageRenderer::ImageRenderer() : m_material{vertex_shader_path, fragment_shader_path}
{
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glEnable(GL_DEPTH_TEST);
}

void ImageRenderer::render(const Mesh &mesh, const Frame &frame, PerspectiveCamera &camera, bool same_frame)
{
    glm::mat4 model{1.0f};
    glm::mat4 projection = camera.projection();
    glm::mat4 view = camera.view();

    model = glm::scale(model, {2.0f, 1.0f, 2.0f});
    model = glm::translate(model, {3.0f, 0.0f, 1.5f});

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

    if (!same_frame)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame.width, frame.height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                     frame.data.data());
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    glBindVertexArray(mesh.vertex_array());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.element_buffer());

    glDrawElements(GL_TRIANGLES, mesh.indices_count(), GL_UNSIGNED_INT, 0);
}
