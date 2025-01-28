#pragma once

#include "frame.hpp"
#include "material.hpp"
#include "mesh.hpp"
#include "perspective_camera.hpp"

class ImageRenderer
{
  public:
    ImageRenderer();

    void render(const Mesh &mesh, const Frame &frame, PerspectiveCamera &camera);

  private:
    /*std::vector<uint32_t> m_indices;*/

    uint32_t m_texture;
    /*uint32_t m_vao;*/
    /*uint32_t m_ebo;*/

    Material m_material;
};
