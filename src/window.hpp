#pragma once

#include <GLFW/glfw3.h>

class Window
{
  public:
    Window(uint32_t width, uint32_t height, const char *title);
    ~Window();

    Window(const Window &) = delete;
    Window(Window &&) = delete;
    Window &operator=(const Window &) = delete;
    Window &operator=(Window &&) = delete;

    bool is_open() const { return !glfwWindowShouldClose(window); }

    void make_current_context();
    void swap_buffers();

    GLFWwindow *window;
};
