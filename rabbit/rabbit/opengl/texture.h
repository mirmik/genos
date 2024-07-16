#ifndef RABBIT_OPENGL_TEXTURE_H
#define RABBIT_OPENGL_TEXTURE_H

#include <nos/print.h>
#include <rabbit/opengl/util.h>

#if __has_include(<GL/glew.h>)
#define GLEW_STATIC
#include <GL/glew.h>
// prevent reorder
#include <GL/gl.h>
#else

#if __has_include(<EGL/egl.h>)
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>
#endif

#endif

#include <cassert>
#include <cstring>

#include <rabbit/cell3d.h>
#include <rabbit/mesh/mesh.h>

namespace rabbit
{
    class opengl_texture
    {
        unsigned char *data = nullptr;
        int width = 0;
        int height = 0;
        bool inited = false;
        int allocated_buffer = 0;
        GLuint texture = {};

        GLint opengl_format = GL_RED;
        GLint format = GL_RED;
        GLint type = GL_UNSIGNED_BYTE;

    public:
        opengl_texture() = default;

        GLuint id() const
        {
            return texture;
        }

        opengl_texture(const opengl_texture &oth)
            : width(oth.width), height(oth.height), texture(oth.texture),
              opengl_format(oth.opengl_format), format(oth.format),
              type(oth.type)
        {
            resize(width, height, point_size());
            memcpy(data, oth.data, point_size());
        }

        opengl_texture(opengl_texture &&oth)
            : width(oth.width), height(oth.height), texture(oth.texture),
              opengl_format(oth.opengl_format), format(oth.format),
              type(oth.type), data(oth.data)
        {
            oth.data = nullptr;
        }

        opengl_texture &operator=(const opengl_texture &oth)
        {
            if (this == &oth)
                return *this;

            width = oth.width;
            height = oth.height;
            texture = oth.texture;
            opengl_format = oth.opengl_format;
            format = oth.format;
            type = oth.type;

            resize(width, height, point_size());
            memcpy(data, oth.data, point_size());

            return *this;
        }

        opengl_texture &operator=(opengl_texture &&oth)
        {
            width = oth.width;
            height = oth.height;
            texture = oth.texture;
            opengl_format = oth.opengl_format;
            format = oth.format;
            type = oth.type;
            data = oth.data;

            oth.data = nullptr;
            return *this;
        }

        ~opengl_texture()
        {
            delete[](data);
        }

        int point_size()
        {
            return component_size() * components_count();
        }

        int components_count()
        {
            switch (format)
            {
            case GL_RED:
                return 1;
            case GL_RGB:
                return 3;
            }
            return 1;
        }

        void invalidate()
        {
            if (data && allocated_buffer)
                delete[](data);
            data = nullptr;
            allocated_buffer = 0;
        }

        void reference_buffer(unsigned char *buffer,
                              int w,
                              int h,
                              GLint _opengl_format,
                              GLint _format,
                              GLint _type)
        {
            invalidate();

            data = buffer;
            width = w;
            height = h;
            allocated_buffer = 0;

            opengl_format = _opengl_format;
            format = _format;
            type = _type;
        }

        int component_size()
        {
            switch (type)
            {
            case GL_UNSIGNED_BYTE:
                return 1;
            case GL_UNSIGNED_SHORT:
                return 2;
            }
            return 1;
        }

        void resize(int w, int h, int per_point)
        {
            invalidate();

            data = new unsigned char[w * h * per_point];
            allocated_buffer = 1;
            width = w;
            height = h;
        }

        uint8_t &at8(int i, int j)
        {
            return ((uint8_t *)data)[i * width + j];
        }

        void
        create(int w, int h, GLint _opengl_format, GLint _format, GLint _type)
        {
            opengl_format = _format;
            format = _format;
            type = _type;

            resize(w, h, components_count() * component_size());
        }

        void set_finish_flag_texture(int w, int h)
        {
            assert(width > 0);
            assert(height > 0);

            float wkoeff = (float)w / (float)width;
            float hkoeff = (float)h / (float)height;

            for (int i = 0; i < width * point_size(); ++i)
                for (int j = 0; j < height; ++j)
                    data[i * width + j] =
                        (int)(i * wkoeff) % 2 != (int)(j * hkoeff) % 2 ? 255
                                                                       : 0;
        }

        void bind()
        {
            if (inited)
            {
                rebind();
                return;
            }

            /*GLCHECK(glGenTextures(1, &texture));
            GLCHECK(glBindTexture(GL_TEXTURE_2D, texture));
            GLCHECK(glTexImage2D(
                GL_TEXTURE_2D,
                0,
                opengl_format,
                width,
                height,
                0,
                format,
                type,
                data
            ));*/

            // GLCHECK(glGenerateMipmap(GL_TEXTURE_2D));
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);

            // Setup filtering parameters for display
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D,
                            GL_TEXTURE_WRAP_S,
                            GL_CLAMP_TO_EDGE); // This is required on WebGL for
                                               // non power-of-two textures
            glTexParameteri(
                GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

            glTexImage2D(GL_TEXTURE_2D,
                         0,
                         opengl_format,
                         width,
                         height,
                         0,
                         format,
                         type,
                         data);

            inited = true;
        }

        void bind(GLint channel)
        {
            GLCHECK(glGenTextures(1, &texture));
            GLCHECK(glBindTexture(GL_TEXTURE_2D, texture));
            GLCHECK(glTexImage2D(GL_TEXTURE_2D,
                                 0,
                                 channel,
                                 width,
                                 height,
                                 0,
                                 channel,
                                 type,
                                 data));

            // GLCHECK(glGenerateMipmap(GL_TEXTURE_2D));
        }

        void rebind()
        {
            GLCHECK(glBindTexture(GL_TEXTURE_2D, texture));
            GLCHECK(glTexImage2D(GL_TEXTURE_2D,
                                 0,
                                 opengl_format,
                                 width,
                                 height,
                                 0,
                                 format,
                                 type,
                                 data));

            // glGenerateMipmap(GL_TEXTURE_2D);
        }

        void rebind(GLint channel)
        {
            GLCHECK(glBindTexture(GL_TEXTURE_2D, texture));
            GLCHECK(glTexImage2D(GL_TEXTURE_2D,
                                 0,
                                 channel,
                                 width,
                                 height,
                                 0,
                                 channel,
                                 type,
                                 data));

            // GLCHECK(glGenerateMipmap(GL_TEXTURE_2D));
        }

        void activate(int program_id, const char *name, int no = 0) const
        {
            GLCHECK(glActiveTexture(GL_TEXTURE0));
            GLCHECK(glBindTexture(GL_TEXTURE_2D, texture));
            GLCHECK(glUniform1i(glGetUniformLocation(program_id, name), no));
        }
    };
} // namespace rabbit

#endif