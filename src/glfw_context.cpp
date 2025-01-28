#include "glfw_context.hpp"

#include <GLFW/glfw3.h>
#include <stdexcept>

GlfwContext::GlfwContext()
{
    if (!glfwInit())
    {
        throw std::runtime_error("Failed to initialize glfw");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}
GlfwContext::~GlfwContext()
{
    glfwTerminate();
}
