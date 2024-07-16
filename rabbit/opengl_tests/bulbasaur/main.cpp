#include <rabbit/mesh/mesh.h>
#include <rabbit/opengl/drawer.h>
#include <rabbit/opengl/opengl_shader_program.h>
#include <rabbit/opengl/projection.h>
#include <rabbit/opengl/shader_collection.h>
#include <rabbit/util.h>
#include <ralgo/space/pose3.h>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <chrono>
#include <rabbit/camera.h>
#include <rabbit/opengl/drawer.h>
#include <rabbit/opengl/util.h>
#include <thread>

// Window dimensions
const GLuint WIDTH = 1600, HEIGHT = 800;

int main()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow *window =
        glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    glewInit();

    glViewport(0, 0, WIDTH, HEIGHT);

    rabbit::opengl_drawer drawer;

    drawer.init_opengl_context();

    auto surf = rabbit::sphere_surface(0.5);

    auto mesh = rabbit::mesh_from_file<float>("bulbasaur_dual_body.STL");
    mesh.correct_center();

    float aspect = (float)WIDTH / (float)HEIGHT;
    rabbit::mat4f projection =
        rabbit::opengl_perspective(rabbit::deg(100) / aspect, aspect, 0.1, 200);

    rabbit::camera camera;

    while (!glfwWindowShouldClose(window))
    {
        camera.set_camera(rabbit::vec3f{100.f * cosf(glfwGetTime()),
                                        100.f * sinf(glfwGetTime()),
                                        10},
                          {0, 0, 0});
        auto model = ralgo::rot3<float>({0, 0, 1}, rabbit::deg(0));

        glfwPollEvents();

        drawer.clean(0.2f, 0.3f, 0.3f, 1.0f);

        drawer.draw_mesh(mesh,
                         (model).to_mat4(),
                         camera.view_matrix(),
                         projection,
                         rabbit::vec4f{0.5, 0.5, 0.5, 1.0});

        /*drawer.draw_points3d(
           {mesh.vertices.data(),
           mesh.vertices.size()},
           GL_LINE_STRIP,
           (model).to_mat4(),
           camera.view_matrix(),
           projection);      */

        glfwSwapBuffers(window);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    glfwTerminate();
    return 0;
}
