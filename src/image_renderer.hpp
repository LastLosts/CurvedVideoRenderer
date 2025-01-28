#pragma once

#include "material.hpp"
#include <inttypes.h>
#include <vector>

struct Frame
{
    uint32_t width;
    uint32_t height;
    std::vector<uint8_t> data;
};

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
