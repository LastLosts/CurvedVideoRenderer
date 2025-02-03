#pragma once

#include "frame.hpp"
#include "material.hpp"
#include "mesh.hpp"
#include "perspective_camera.hpp"

class ImageRenderer
{
  public:
    ImageRenderer();

    void render(const Mesh &mesh, const Frame &frame, PerspectiveCamera &camera, bool same_frame);

  private:
    uint32_t m_texture;

    Material m_material;
};
