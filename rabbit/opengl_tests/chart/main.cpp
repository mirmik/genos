#include <rabbit/opengl/opengl_shader_program.h>
#include <rabbit/opengl/projection.h>
#include <rabbit/opengl/drawer.h>
#include <rabbit/opengl/shader_collection.h>
#include <rabbit/mesh.h>
#include <rabbit/util.h>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <rabbit/opengl/util.h>
#include <rabbit/opengl/drawer.h>
#include <rabbit/chart/chart.h>
#include <rabbit/camera.h>

#include <ralgo/linalg/vecops.h>
#include <nos/print.h>

#include <igris/tuple.h>
#include <igris/time/systime.h>
#include <igris/math.h>
#include <chrono>

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

	glViewport(WIDTH*0.1, HEIGHT*0.1, WIDTH*0.8, HEIGHT*0.8);






	//rabbit::opengl_drawer drawer;

	rabbit::opengl_chart_drawer drawer;
	drawer.init_opengl_context();

	float aspect = (float)WIDTH / (float)HEIGHT;
	rabbit::mat4f projection =
	    rabbit::opengl_perspective(igris::deg(100) / aspect, aspect, 0.1, 100);

	rabbit::camera camera;




	rabbit::opengl_shader_program sprg(
	    rabbit::simple_vertex_shader,
	    rabbit::simple_fragment_shader);


	igris::start_local_time();
	while (!glfwWindowShouldClose(window))
	{
		double timb = igris::local_time();
		auto foo = ralgo::vecops::vectorize<std::vector<float>>(+[](float f) { return 0.5 * sinf(f * 10); });
		auto tim = ralgo::vecops::linspace<std::vector<float>>(-1, 1, 100);
		auto tim2 = ralgo::vecops::linspace<std::vector<float>>(-1 + timb, 1 + timb, 100);
		auto sig = foo(tim2);

		auto vertices = ralgo::vecops::zip<std::vector<rabbit::xypoint>>(tim, sig);



		glfwPollEvents();

		drawer.clean(0.2f, 0.3f, 0.3f, 1.0f);

		sprg.use();

		drawer.set_vertices_stride(3);

		rabbit::opengl_viewport viewport;
		viewport.set_chart_viewport(WIDTH, HEIGHT);

		drawer.draw_xyseries(vertices.data(), vertices.size(), {0,1,0}, viewport);


		//drawer.draw_simple_triangles(vertices, 4, indices, 2);

		glUseProgram(0);
		/*drawer.draw_mesh(_mesh,
		    rabbit::pose3().to_mat4(),
		    rabbit::pose3().to_mat4(),
		    rabbit::pose3().to_mat4()
		);*/

		/*drawer.opengl_mesh_program.use();

		drawer.draw_mesh(
		    mesh,
		    (rabbit::rot3(rabbit::vec3{0.3,0.7,0}, rabbit::deg(20)) * model).to_mat4(),
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
		    (rabbit::rot3(rabbit::vec3{0.6,0.2,0}, rabbit::deg(20)) * model.inverse()).to_mat4(),
		    camera.view_matrix(),
		    projection
		);

		drawer.draw_mesh(
		    mesh5,
		    (rabbit::mov3({5.5*sin(glfwGetTime()), 5.5*cos(glfwGetTime()), 0}) * model).to_mat4(),
		    camera.view_matrix(),
		    projection
		);

		glUseProgram(0);*/

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}
