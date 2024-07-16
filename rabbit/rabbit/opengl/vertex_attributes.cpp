#include <rabbit/opengl/opengl_includes.h>
#include <rabbit/opengl/vertex_attributes.h>

using namespace rabbit;

vertex_attributes::vertex_attributes()
{
    glGenVertexArrays(1, &vao);
}

vertex_attributes::~vertex_attributes()
{
    glDeleteVertexArrays(1, &vao);
}

void vertex_attributes::bind() const
{
    glBindVertexArray(vao);
}

void vertex_attributes::unbind() const
{
    glBindVertexArray(0);
}

void vertex_attributes::add_attribute(vertex_attribute attribute)
{
    attributes.push_back(attribute);
}

void vertex_attributes::add_attribute(int index,
                                      int size,
                                      int stride,
                                      int offset)
{
    vertex_attribute attribute(index, size, stride, offset);
    attributes.push_back(attribute);
}

void vertex_attributes::apply_attributes() const
{
    for (auto attribute : attributes)
    {
        enable_attribute(attribute.index);
        glVertexAttribPointer(attribute.index,
                              attribute.size,
                              GL_FLOAT,
                              GL_FALSE,
                              attribute.stride * sizeof(float),
                              (void *)(attribute.offset * sizeof(float)));
    }
}

void vertex_attributes::enable_attribute(int index) const
{
    glEnableVertexAttribArray(index);
}

void vertex_attributes::disable_attribute(int index) const
{
    glDisableVertexAttribArray(index);
}