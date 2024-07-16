#ifndef RABBiT_OPENGL_ABSTRACT_DRAWER_H_
#define RABBiT_OPENGL_ABSTRACT_DRAWER_H_

#include <igris/container/array_view.h>
#include <rabbit/mesh/mesh.h>
#include <rabbit/types.h>
#include <vector>

namespace rabbit
{
    class opengl_abstract_drawer
    {
    public:
        virtual ~opengl_abstract_drawer() = default;

        virtual void draw_mesh(const rabbit::mesh<float> &mesh,
                               const mat4f &model,
                               const mat4f &view,
                               const mat4f &projection,
                               const vec4f &color) = 0;

        virtual void draw_points(const vec3f *pnts,
                                 int count,
                                 const mat4f &model,
                                 const mat4f &view,
                                 const mat4f &projection) = 0;

        void draw_points(const std::vector<vec3f> &pnts,
                         const mat4f &model,
                         const mat4f &view,
                         const mat4f &projection)
        {
            draw_points(pnts.data(), pnts.size(), model, view, projection);
        }

        virtual void draw_lines(const std::vector<vec3f> &lines) = 0;

        virtual void draw_points2d(const igris::array_view<vec2f> &points,
                                   GLint style) = 0;

        virtual void draw_points3d(const igris::array_view<vec3f> &points,
                                   GLint style) = 0;

        virtual void draw_points3d(const igris::array_view<vec3f> &lines,
                                   GLint style,
                                   const rabbit::mat4f &model,
                                   const rabbit::mat4f &view,
                                   const rabbit::mat4f &projective) = 0;

        void draw_lines2d(const igris::array_view<vec2f> &lines);

        void draw_lines2d(const std::vector<vec2f> &lines);

        void draw_lines3d(const igris::array_view<vec3f> &lines);

        void draw_lines3d(const std::vector<vec3f> &lines);

        void draw_line2d(const vec2f &a, const vec2f &b);

        void draw_line3d(const vec3f &a, const vec3f &b);
    };
}

#endif