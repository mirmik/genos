#ifndef RABBIT_OPENGL_VERTEX_ATTRIBUTES_H_
#define RABBIT_OPENGL_VERTEX_ATTRIBUTES_H_

#include <rabbit/opengl/opengl_includes.h>
#include <vector>

namespace rabbit
{
    struct vertex_attribute
    {
        int index;
        int size;
        int stride;
        int offset;

    public:
        vertex_attribute(int index, int size, int stride, int offset)
            : index(index), size(size), stride(stride), offset(offset)
        {
        }
    };

    class vertex_attributes
    {
        GLuint vao = -1;
        std::vector<vertex_attribute> attributes = {};

    public:
        vertex_attributes();
        ~vertex_attributes();
        void bind() const;
        void unbind() const;
        void add_attribute(int index, int size, int stride, int offset);
        void add_attribute(vertex_attribute attribute);
        void apply_attributes() const;
        void enable_attribute(int index) const;
        void disable_attribute(int index) const;
    };
}

#endif