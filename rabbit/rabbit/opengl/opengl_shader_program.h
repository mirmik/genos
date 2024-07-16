#ifndef RABBIT_OPENGL_SHADER_H
#define RABBIT_OPENGL_SHADER_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <rabbit/types.h>

#if __has_include(<GL/glew.h>)
#include <GL/glew.h>
#else

#if __has_include(<EGL/egl.h>)
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>
#endif

#endif

namespace rabbit
{
    class opengl_shader_program
    {
        GLuint ProgramId = {};

    public:
        GLuint id()
        {
            return ProgramId;
        }

        opengl_shader_program();
        opengl_shader_program(const char *vertexShaderCode,
                              const char *fragmentShaderCode);
        void open(const char *vertexShaderCode, const char *fragmentShaderCode);
        void uniform_vec3f(const char *locname,
                           const linalg::vec<float, 3> &v) const;
        void uniform_mat4f(const char *locname,
                           const linalg::mat<float, 4, 4> &v) const;
        void use() const;
        void unuse() const;

        void draw_arrays(GLenum mode, GLint first, GLsizei count) const;
        void draw_elements(GLenum mode,
                           GLsizei count,
                           GLenum type,
                           const void *indices) const;
    };
}

#endif
