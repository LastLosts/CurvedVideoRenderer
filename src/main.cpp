#include <iostream>

#include <glad/gl.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

#include "glfw_context.hpp"
#include "image_renderer.hpp"
#include "mesh.hpp"
#include "perspective_camera.hpp"
#include "window.hpp"

constexpr uint32_t window_width = 800;
constexpr uint32_t window_height = 800;

#include "video_reader.hpp"

float delta_time = 0.016f;

static void resize_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

static void process_input(PerspectiveCamera &camera, GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.position += camera.move_speed * camera.forward() * delta_time;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.position -= camera.move_speed * camera.forward() * delta_time;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.position -= glm::normalize(glm::cross(camera.forward(), camera.up())) * camera.move_speed * delta_time;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.position += glm::normalize(glm::cross(camera.forward(), camera.up())) * camera.move_speed * delta_time;

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        camera.yaw -= camera.rotation_speed * delta_time;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        camera.yaw += camera.rotation_speed * delta_time;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        camera.pitch += camera.rotation_speed * delta_time;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        camera.pitch -= camera.rotation_speed * delta_time;
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
        PerspectiveCamera camera{45.0f, ((float)window_width / (float)window_height), 0.01f, 100.0f};

        Mesh curved = generate_mesh_curved_plane(10000, 300, 160.0f);

        while (window.is_open())
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
