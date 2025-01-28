#pragma once

#include "frame.hpp"
#include "material.hpp"

class ImageRenderer
{
  public:
    ImageRenderer();

    void render(const Frame &frame);
    void render_texture(uint32_t texture);

  private:
    uint32_t m_texture;
    uint32_t m_vao;
    uint32_t m_ebo;

    Material m_material;
};
