#ifndef RABBIT_OPENGL_VERTEX_BUFFER_OBJECT_H_
#define RABBIT_OPENGL_VERTEX_BUFFER_OBJECT_H_

#include <cstddef>
#include <vector>

namespace rabbit
{
    class vertex_buffer_object
    {
        GLuint vbo = -1;
        std::vector<float> data = {};

    public:
        vertex_buffer_object();
        ~vertex_buffer_object();
        vertex_buffer_object(const std::vector<float> &data);
        vertex_buffer_object(float *data, size_t size);
        void open(const std::vector<float> &data);
        void open(float *data, size_t size);
        void bind() const;
        void unbind() const;
    };
}

#endif