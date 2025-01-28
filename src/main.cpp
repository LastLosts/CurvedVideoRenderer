#include <iostream>

#include <glad/gl.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

#include "glfw_context.hpp"
#include "image_renderer.hpp"
#include "window.hpp"

constexpr uint32_t window_width = 800;
constexpr uint32_t window_height = 800;

#include "video_reader.hpp"

void resize_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main()
{
    try
    {
        GlfwContext glfw{};
        Window window{window_width, window_height, "Helo"};

        window.make_current_context();

        glfwSetFramebufferSizeCallback(window.window, resize_callback);

        if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
        {
            throw std::runtime_error("Failed to initialize GLAD");
            return -1;
        }

        ImageRenderer image_renderer{};
        VideoReader video_reader{"./assets/test-video.mp4"};

        while (window.is_open())
        {
            Frame frame = video_reader.read_frame();

            static bool first_frame = true;
            if (first_frame)
            {
                glfwSetTime(0.0);
                first_frame = false;
            }

            while (frame.time > glfwGetTime())
            {
                glfwWaitEventsTimeout(frame.time - glfwGetTime());
            }

            image_renderer.render(frame);

            window.swap_buffers();
            glfwPollEvents();
        }
    }
    catch (const std::exception &e)
    {
        std::cout << "Exception thrown:\n" << e.what() << std::endl;
        return -1;
    }
    catch (...)
    {
        std::cout << "Unknown exception thrown terminating" << std::endl;
        return -1;
    }

    return 0;
}
