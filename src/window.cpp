#include "window.hpp"
#include "GLFW/glfw3.h"
#include <stdexcept>

Window::Window(uint32_t width, uint32_t height, const char *title)
{
    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (window == nullptr)
    {
        throw std::runtime_error("Failed to create glfw window");
    }
}
Window::~Window()
{
    glfwDestroyWindow(window);
}

void Window::make_current_context()
{
    glfwMakeContextCurrent(window);
}

void Window::swap_buffers()
{
    glfwSwapBuffers(window);
}
