#include "image_renderer.hpp"
#include <inttypes.h>

#include <glad/gl.h>

constexpr char *vertex_shader_path = "./shaders/test.vert";
constexpr char *fragment_shader_path = "./shaders/horizontally_flipped_texture.frag";

ImageRenderer::ImageRenderer() : m_material{vertex_shader_path, fragment_shader_path}
{
    float vertices[] = {
        -1.0f, 1.0f,  0.0f, 1.0f, // Top Left
        1.0f,  1.0f,  1.0f, 1.0f, // Top Right
        -1.0f, -1.0f, 0.0f, 0.0f, // Bottom Left
        -1.0f, -1.0f, 0.0f, 0.0f, // Bottom Left
        1.0f,  1.0f,  1.0f, 1.0f, // Top Right
        1.0f,  -1.0f, 1.0f, 0.0f, // Bottom Right
    };

    uint32_t vbo;
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenTextures(1, &m_texture);
}

void ImageRenderer::render(const Frame &frame)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(m_material.id());
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(m_material.id(), "u_texture"), 0);

    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame.width, frame.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, frame.data.data());
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void ImageRenderer::render_texture(uint32_t texture)
{
    glClearColor(0.2f, 0.1f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(m_material.id());
    /*glActiveTexture(GL_TEXTURE0);*/
    glBindTexture(GL_TEXTURE_2D, texture);
    /*glUniform1i(glGetUniformLocation(m_material.id(), "u_texture"), 0);*/

    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
