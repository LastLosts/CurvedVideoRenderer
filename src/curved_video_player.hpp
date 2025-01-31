#pragma once

#include "glfw_context.hpp"
#include "window.hpp"

class CurvedVideoPlayer
{
  public:
    CurvedVideoPlayer(const char *video_path);

  private:
    GlfwContext m_glfw;
    Window m_window;
};
