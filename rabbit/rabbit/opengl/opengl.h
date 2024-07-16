#ifndef RABBIT_OPENGL_OPENGL_H_
#define RABBIT_OPENGL_OPENGL_H_

#include <rabbit/opengl/opengl_shader_program.h>
#include <rabbit/opengl/vertex_attributes.h>
#include <rabbit/opengl/vertex_buffer_object.h>
#include <rabbit/types.h>

namespace rabbit
{
    void opengl_init();

    void opengl_draw_arrays(const opengl_shader_program &program,
                            const vertex_buffer_object &vbo,
                            const vertex_attributes &attributes,
                            GLenum mode,
                            GLint first,
                            GLsizei count);
}

#endif