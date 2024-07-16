#ifndef RABBIT_OPENGL_PROJECTION_H
#define RABBIT_OPENGL_PROJECTION_H

#include <rabbit/types.h>

namespace rabbit
{

    // https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glOrtho.xml
    static inline linalg::mat<float, 4, 4> opengl_ortho(
        float left, float right, float bottom, float top, float near, float far)
    {
        float tx = -(right + left) / (right - left);
        float ty = -(top + bottom) / (top - bottom);
        float tz = -(far + near) / (far - near);

        return {{2 / (left - right), 0, 0, 0},
                {0, 2 / (top - bottom), 0, 0},
                {0, 0, 2 / (far - near), 0},
                {tx, ty, tz, 1}};
    }

    // https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/gluPerspective.xml
    static inline linalg::mat<float, 4, 4>
    opengl_perspective(float fovy, float aspect, float zNear, float zFar)
    {
        float angle = fovy / 2;
        float f = cos(angle) / sin(angle);

        return {{f / aspect, 0, 0, 0},
                {0, f, 0, 0},
                {0, 0, (zFar + zNear) / (zNear - zFar), -1},
                {0, 0, 2 * zFar * zNear / (zNear - zFar), 0}};
    }
}

#endif