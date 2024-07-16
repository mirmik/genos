#include <rabbit/opengl/opengl_includes.h>
#include <rabbit/opengl/vertex_buffer_object.h>

using namespace rabbit;

vertex_buffer_object::vertex_buffer_object()
{
    glGenBuffers(1, &vbo);
}

vertex_buffer_object::~vertex_buffer_object()
{
    glDeleteBuffers(1, &vbo);
}

vertex_buffer_object::vertex_buffer_object(const std::vector<float> &data)
{
    glGenBuffers(1, &vbo);
    open(data);
}

vertex_buffer_object::vertex_buffer_object(float *data, size_t size)
{
    glGenBuffers(1, &vbo);
    open(data, size);
}

void vertex_buffer_object::open(const std::vector<float> &srcdata)
{
    data = srcdata;
    bind();
    glBufferData(GL_ARRAY_BUFFER,
                 data.size() * sizeof(float),
                 data.data(),
                 GL_STATIC_DRAW);
    unbind();
}

void vertex_buffer_object::open(float *srcdata, size_t size)
{
    data = std::vector<float>(srcdata, srcdata + size);
    bind();
    glBufferData(GL_ARRAY_BUFFER,
                 data.size() * sizeof(float),
                 data.data(),
                 GL_STATIC_DRAW);
    unbind();
}

void vertex_buffer_object::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

void vertex_buffer_object::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
