#include <rabbit/opengl/opengl.h>

using namespace rabbit;

void opengl_init()
{
    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_DEPTH_TEST);
    glLineWidth(2);
}

void opengl_draw_arrays(const opengl_shader_program &program,
                        const vertex_buffer_object &vbo,
                        const vertex_attributes &attributes,
                        GLenum mode,
                        GLint first,
                        GLsizei count)
{
    program.use();
    vbo.bind();
    attributes.bind();

    glDrawArrays(mode, first, count);
}
