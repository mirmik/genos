#include <GL/glew.h>
#include <rabbit/opengl/abstract_drawer.h>
#include <rabbit/types.h>

void rabbit::opengl_abstract_drawer::draw_lines2d(
    const igris::array_view<vec2f> &lines)
{
    return draw_points2d(lines, GL_LINE_STRIP);
}

void rabbit::opengl_abstract_drawer::draw_lines2d(
    const std::vector<vec2f> &lines)
{
    return draw_lines2d(
        igris::array_view<vec2f>((vec2f *)lines.data(), lines.size()));
}

void rabbit::opengl_abstract_drawer::draw_lines3d(
    const igris::array_view<vec3f> &lines)
{
    return draw_points3d(lines, GL_LINE_STRIP);
}

void rabbit::opengl_abstract_drawer::draw_lines3d(
    const std::vector<vec3f> &lines)
{
    return draw_lines3d(
        igris::array_view<vec3f>((vec3f *)lines.data(), lines.size()));
}

void rabbit::opengl_abstract_drawer::draw_line2d(const vec2f &a, const vec2f &b)
{
    return draw_points2d({a, b}, GL_LINE_STRIP);
}

void rabbit::opengl_abstract_drawer::draw_line3d(const vec3f &a, const vec3f &b)
{
    return draw_points3d({a, b}, GL_LINE_STRIP);
}