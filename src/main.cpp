#include <iostream>

#include <glad/gl.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "glfw_context.hpp"
#include "image_renderer.hpp"
#include "mesh.hpp"
#include "perspective_camera.hpp"
#include "window.hpp"

constexpr uint32_t initial_window_width = 800;
constexpr uint32_t initial_window_height = 800;

#include "video_reader.hpp"

static float delta_time = 0.016f;
static PerspectiveCamera camera{45.0f, ((float)initial_window_width / (float)initial_window_height), 0.01f, 100.0f};
static bool quit = false;

static void resize_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    camera.set_aspect((float)width / (float)height);
}

static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.position.x += (float)yoffset * camera.move_speed;
}

static void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    static float last_x = initial_window_width / 2.0f, last_y = initial_window_height / 2.0f;

    float xoffset = xpos - last_x;
    float yoffset = ypos - last_y;

    last_x = xpos;
    last_y = ypos;

    if (xoffset >= 100.0f)
        return;
    if (yoffset >= 100.0f)
        return;

    constexpr float sensetivity = 0.1f;

    xoffset *= sensetivity;
    yoffset *= sensetivity;

    camera.yaw += xoffset;
    camera.pitch -= yoffset;
}

static void process_input(PerspectiveCamera &camera, GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        quit = true;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << "Usage:\n renderer [path to video]" << std::endl;
        return -1;
    }

    try
    {
        GlfwContext glfw{};
        Window window{initial_window_width, initial_window_height, "Helo"};

        window.make_current_context();

        glfwSetFramebufferSizeCallback(window.window, resize_callback);
        glfwSetScrollCallback(window.window, scroll_callback);
        glfwSetInputMode(window.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPosCallback(window.window, mouse_callback);

        if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
        {
            throw std::runtime_error("Failed to initialize GLAD");
            return -1;
        }

        ImageRenderer image_renderer{};
        VideoReader video_reader{argv[1]};

        Mesh curved = generate_mesh_curved_plane(1000, 300, 260.0f);

        while (window.is_open() && !quit)
        {
            static bool first_frame = true;
            if (first_frame)
            {
                glfwSetTime(0.0);
                first_frame = false;
            }
            float start = glfwGetTime();

            process_input(camera, window.window);
            Frame frame = video_reader.read_frame();

            while (frame.time > glfwGetTime())
            {
                float timeout_time = frame.time - glfwGetTime();

                glfwWaitEventsTimeout(glm::clamp(timeout_time, 0.001f, std::numeric_limits<float>::max()));
            }

            image_renderer.render(curved, frame, camera);

            window.swap_buffers();
            glfwPollEvents();
            float end = glfwGetTime();

            delta_time = end - start;
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
