// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include <rabbit/opengl/opengl_shader_program.h>
#include <rabbit/opengl/projection.h>
#include <rabbit/opengl/drawer.h>
#include <rabbit/opengl/shader_collection.h>
#include <rabbit/mesh.h>
#include <rabbit/util.h>
#include <rabbit/space/htrans3.h>

#include <nos/fprint.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <rabbit/opengl/util.h>
#include <rabbit/opengl/drawer.h>
#include <rabbit/font/naive.h>
#include <rabbit/camera.h>
#include <rabbit/font/font.h>
#include <rabbit/font/textzone.h>

#include <igris/time/systime.h>

// Window dimensions
const GLuint WIDTH = 1600, HEIGHT = 800;

int main()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    glewInit();

    glViewport(0, 0, WIDTH, HEIGHT);
    glEnable(GL_BLEND);

    rabbit::opengl_drawer drawer;
    drawer.init_opengl_context();

    auto surf = rabbit::torus_surface(4, 0.2);
    auto surf2 = rabbit::sphere_surface(2);
    auto surf3 = rabbit::torus_surface(7, 0.2);
    auto surf4 = rabbit::torus_surface(8, 0.2);
    auto surf5 = rabbit::sphere_surface(0.6);

    auto mesh = rabbit::surface_rubic_mesh<float>(surf, 30, 20);
    auto mesh2 = rabbit::surface_rubic_mesh<float>(surf2, 30, 20);
    auto mesh3 = rabbit::surface_rubic_mesh<float>(surf3, 30, 20);
    auto mesh4 = rabbit::surface_rubic_mesh<float>(surf4, 30, 20);
    auto mesh5 = rabbit::surface_rubic_mesh<float>(surf5, 30, 20);

    float aspect = (float)WIDTH / (float)HEIGHT;
    rabbit::mat4f projection =
        rabbit::opengl_perspective(rabbit::deg(100) / aspect, aspect, 0.1, 100);

    //rabbit::mat4f projection = rabbit::opengl_ortho(-100,100,-100,100,-100,100);

    rabbit::camera camera;
    rabbit::font font(rabbit::naive_font16x26_texture);

    rabbit::textzone textzone(30, 30,

    { -1, -1, -0.5},
    {1, -1, -0.5},
    { -1, 1, -0.5},
    {1, 1, -0.5}
                             );
    int i = 0;
    igris::start_local_time();

    double last_time = 0;
    while (!glfwWindowShouldClose(window))
    {
        auto model = ralgo::rot3<float>(rabbit::vec3f{0, 0, 1}, rabbit::deg(glfwGetTime() * 16));

        camera.set_eye(rabbit::vec3f{10, 0, 3});
        camera.set_target({0, 0, 0});

        glfwPollEvents();

        drawer.clean(0.2f, 0.3f, 0.3f, 1.0f);

        float lo = 0.7;
        float hi = 0.9;

        double curtime =  igris::local_time();

        /*        auto texttrans = mul(
                    linalg::rotation_matrix(linalg::rotation_quat<float>({0,1,0}, M_PI/4)),
                    linalg::scaling_matrix<float>({0.5,0.5,0.5}));*/

        auto texttrans = mul(
            projection, 
            mul(
                mul(
                    ralgo::rot3<float>({0,1,0}, -M_PI/10).to_mat4(),
                    ralgo::rot3<float>({1,0,0}, -M_PI/10).to_mat4()
                ),
                ralgo::mov3<float>({0,0,-2}).to_mat4()
            )
        );


        //linalg::scaling_matrix<float>({0.2,0.2,1})),
        //linalg::rotation_matrix(linalg::rotation_quat<float>({0,1,0},-M_PI/10))),
        //linalg::rotation_matrix(linalg::rotation_quat<float>({1,0,0},-M_PI/20))
        ;

        auto cursor = rabbit::textzone_cursor(&textzone, 0, 1);
        auto str = nos::format("Mirmik was here : {}", i++);
        drawer.print_text(font, cursor, str, {0, 1, 0}, texttrans);

        cursor = rabbit::textzone_cursor(&textzone, 0, 2);
        str = nos::format("fps : {}", 1.f / (curtime - last_time));
        str = nos::format("Mirmik was here : {}", i++);
        drawer.print_text(font, cursor, str, {0, 1, 0}, texttrans);
        drawer.print_text(font, cursor, str, {0, 1, 0}, texttrans);
        drawer.print_text(font, cursor, str, {0, 1, 0}, texttrans);
        drawer.print_text(font, cursor, str, {0, 1, 0}, texttrans);
        drawer.print_text(font, cursor, str, {0, 1, 0}, texttrans);
        drawer.print_text(font, cursor, str, {0, 1, 0}, texttrans);
        drawer.print_text(font, cursor, str, {0, 1, 0}, texttrans);
        drawer.print_text(font, cursor, str, {0, 1, 0}, texttrans);
        drawer.print_text(font, cursor, str, {0, 1, 0}, texttrans);
        drawer.print_text(font, cursor, str, {0, 1, 0}, texttrans);
        drawer.print_text(font, cursor, str, {0, 1, 0}, texttrans);
        drawer.print_text(font, cursor, str, {0, 1, 0}, texttrans);
        drawer.print_text(font, cursor, str, {0, 1, 0}, texttrans);
        drawer.print_text(font, cursor, str, {0, 1, 0}, texttrans);
        drawer.print_text(font, cursor, str, {0, 1, 0}, texttrans);
        drawer.print_text(font, cursor, str, {0, 1, 0}, texttrans);
        drawer.print_text(font, cursor, str, {0, 1, 0}, texttrans);
        drawer.print_text(font, cursor, str, {0, 1, 0}, texttrans);
        drawer.print_text(font, cursor, str, {0, 1, 0}, texttrans);

        last_time = curtime;


        drawer.opengl_mesh_program.use();

        drawer.draw_mesh(
            mesh,
            (ralgo::rot3<float>(rabbit::vec3f{0.3, 0.7, 0}, rabbit::deg(20)) * model).to_mat4(),
            camera.view_matrix(),
            projection);

        drawer.draw_mesh(
            mesh2,
            model.inverse().to_mat4(),
            camera.view_matrix(),
            projection
        );

        drawer.draw_mesh(
            mesh3,
            model.to_mat4(),
            camera.view_matrix(),
            projection
        );

        drawer.draw_mesh(
            mesh4,
            (ralgo::rot3<float>(rabbit::vec3f{0.6, 0.2, 0}, rabbit::deg(20)) * model.inverse()).to_mat4(),
            camera.view_matrix(),
            projection
        );

        drawer.draw_mesh(
            mesh5,
            (ralgo::mov3<float>(linalg::vec<float,3>{5.5f * sinf(glfwGetTime()), 5.5f * cosf(glfwGetTime()), 0}) * model).to_mat4(),
            camera.view_matrix(),
            projection
        );

        glUseProgram(0);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
