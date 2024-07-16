#ifndef RABBIT_FONT_H
#define RABBIT_FONT_H

#include <rabbit/opengl/texture.h>

namespace rabbit
{
    using font_producer_t = opengl_texture (*)(int code);

    class font
    {
    public:
        opengl_texture textures[256];

        font() = default;

        font(font_producer_t producer)
        {
            init(producer);
        }

        void init(font_producer_t producer, GLint channel = GL_RED)
        {
            for (int i = 0; i < 256; ++i)
            {
                textures[i] = producer(i);
                textures[i].bind(channel);
            }
        }

        const opengl_texture &operator[](int code) const
        {
            return textures[code];
        }
    };
}

#endif