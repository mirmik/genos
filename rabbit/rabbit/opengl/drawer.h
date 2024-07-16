#ifndef RABBIT_OPENGL_DRAWER_H
#define RABBIT_OPENGL_DRAWER_H

#include <rabbit/font/font.h>
#include <rabbit/font/textzone.h>
#include <rabbit/interactive/interactive_object.h>
#include <rabbit/opengl/abstract_drawer.h>
#include <rabbit/opengl/opengl_shader_program.h>
#include <rabbit/opengl/texture.h>
#include <rabbit/types.h>

namespace rabbit
{
    class opengl_drawer : public rabbit::opengl_abstract_drawer
    {
    public:
        GLuint VBO = 0, VAO = 0, EBO = 0;
        int vertices_stride = 3;

        opengl_shader_program opengl_mesh_program = {};
        opengl_shader_program opengl_colored_mesh_program = {};
        opengl_shader_program opengl_simple_program = {};
        opengl_shader_program opengl_space_point3d_program = {};
        opengl_shader_program opengl_simple_2d_program = {};
        opengl_shader_program opengl_onecolored_texture = {};
        opengl_shader_program opengl_rgb_texture = {};
        opengl_shader_program opengl_grayscale_texture = {};

    public:
        opengl_drawer() = default;
        virtual ~opengl_drawer() = default;

        void set_buffers(int vao, int vbo, int ebo)
        {
            VAO = vao;
            VBO = vbo;
            EBO = ebo;
        }
        void set_vertices_stride(int arg)
        {
            vertices_stride = arg;
        }

        void draw_interactive_object(interactive_object &iobj);
        void init_opengl_context();
        void destroy_opengl_context();

        void clean(real r, real g, real b, real a);

        void draw_mesh(const rabbit::mesh<float> &mesh,
                       const mat4f &model,
                       const mat4f &view,
                       const mat4f &projection,
                       const vec4f &color) override;

        void draw_mesh_edges(std::vector<linalg::vec<float, 3>> vertices,
                             std::vector<linalg::vec<unsigned int, 2>> edges,
                             const mat4f &model,
                             const mat4f &view,
                             const mat4f &projection,
                             const vec4f &color);

        void draw_points(const vec3f *pnts,
                         int count,
                         const mat4f &model,
                         const mat4f &view,
                         const mat4f &projection) override;

        using rabbit::opengl_abstract_drawer::draw_points;

        void create_buffers();

        void draw_triangles(float *vertices,
                            int vertices_total,
                            GLuint *triangles,
                            int triangles_total);

        void draw_triangles(const std::vector<vec3> &vertices,
                            const std::vector<ivec3> &triangles);

        void draw_lines(float *vertices, int vertices_total);

        void draw_lines(const std::vector<vec3f> &lines) override;

        void draw_points2d(const igris::array_view<vec2f> &lines,
                           GLint style) override;

        void draw_points3d(const igris::array_view<vec3f> &lines,
                           GLint style) override;

        void draw_points3d(const igris::array_view<vec3f> &lines,
                           GLint style,
                           const rabbit::mat4f &model,
                           const rabbit::mat4f &view,
                           const rabbit::mat4f &projective) override;

        void draw_line(linalg::vec<float, 3> p1,
                       linalg::vec<float, 4> c1,
                       linalg::vec<float, 3> p2,
                       linalg::vec<float, 4> c2,
                       const rabbit::mat4f &model,
                       const rabbit::mat4f &view,
                       const rabbit::mat4f &projective);

        void uniform_mat4f(unsigned int loc,
                           const linalg::mat<float, 4, 4> &matrix);

        void uniform_mat4f(const char *locname,
                           int program,
                           const linalg::mat<float, 4, 4> &matrix);

        void uniform_mat4f(unsigned int loc, const GLfloat *matrix);

        void
        uniform_mat4f(const char *locname, int program, const GLfloat *matrix);

        void draw_char_texture(const std::vector<vec3> &vertices,
                               const std::vector<ivec3> &triangles,
                               const rabbit::opengl_texture);

        void draw_onecolored_texture_2d(
            const std::vector<std::pair<linalg::vec<float, 3>,
                                        linalg::vec<float, 2>>> &vertices,
            const std::vector<ivec3> triangles,
            const rabbit::opengl_texture &texture,
            const linalg::vec<float, 3> &color,
            const linalg::mat<float, 4, 4> &transform = linalg::identity);

        void draw_rgb_texture_2d(
            const std::vector<std::pair<linalg::vec<float, 3>,
                                        linalg::vec<float, 2>>> &vertices,
            const std::vector<ivec3> triangles,
            const rabbit::opengl_texture &texture,
            const linalg::mat<float, 4, 4> &transform = linalg::identity);

        void draw_grayscale_texture(
            const std::vector<std::pair<linalg::vec<float, 3>,
                                        linalg::vec<float, 2>>> &vertices,
            const std::vector<ivec3> triangles,
            const rabbit::opengl_texture &texture,
            const linalg::mat<float, 4, 4> &transform = linalg::identity);

        void print_text(
            const rabbit::font &font,
            rabbit::textzone_cursor &cursor,
            const std::string_view &message,
            const linalg::vec<float, 3> &color,
            const linalg::mat<float, 4, 4> &transform = linalg::identity);

        void draw_image_uint8(rabbit::opengl_texture &texture);
    };
}
#endif