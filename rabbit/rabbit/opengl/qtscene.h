#ifndef RABBIT_OPENGL_QTSCENE_H
#define RABBIT_OPENGL_QTSCENE_H

#include <rabbit/camera.h>
#include <rabbit/opengl/drawer.h>
#include <rabbit/opengl/projection.h>
#include <rabbit/util.h>

namespace rabbit
{
    class scene;
    class view;

    class drawable_object
    {
    public:
        ralgo::pose3<float> pose;

    public:
        drawable_object() : pose({0, 0, 0, 1}, {0, 0, 0}) {}
        virtual void draw_on(rabbit::view &view) = 0;
        void set_pose(const ralgo::pose3<float> &p)
        {
            pose = p;
        }
        virtual ~drawable_object() {}
    };

    class view
    {
    public:
        rabbit::opengl_drawer drawer;
        rabbit::camera camera;
        rabbit::scene *scene = nullptr;
        bool _trihedron_axes_enabled = false;

    public:
        void enable_trihedron(bool en = true)
        {
            _trihedron_axes_enabled = en;
        }

        // void attach_to_window_id(int id);
        view(rabbit::scene *scene) : scene(scene)
        {
            drawer.init_opengl_context();
        }

        void clean()
        {
            drawer.clean(0.3, 0, 0.3, 0);

            if (_trihedron_axes_enabled)
            {
                auto c1 = camera_pose_matrix();
                auto c2 = camera_projection_matrix();

                drawer.draw_line({-10000.0, 0, 0},
                                 {1, 0, 0, 1},
                                 {10000.0, 0, 0},
                                 {1, 0, 0, 1},
                                 linalg::identity,
                                 c1,
                                 c2);
                drawer.draw_line({0, -10000.0, 0},
                                 {0, 1, 0, 1},
                                 {0, 10000.0, 0},
                                 {0, 1, 0, 1},
                                 linalg::identity,
                                 c1,
                                 c2);
                drawer.draw_line({0, 0, -10000.0},
                                 {0, 0, 1, 1},
                                 {0, 0, 10000.0},
                                 {0, 0, 1, 1},
                                 linalg::identity,
                                 c1,
                                 c2);
            }
        }

        rabbit::mat4f camera_pose_matrix()
        {
            return camera.view_matrix();
        }

        rabbit::mat4f camera_projection_matrix()
        {
            int WIDTH = 1600, HEIGHT = 800;
            float aspect = (float)WIDTH / (float)HEIGHT;
            return rabbit::opengl_perspective(
                rabbit::deg(100) / aspect, aspect, 0.1, 200);
        }

        void draw_line_strip(const std::vector<vec3f> &lines,
                             const rabbit::mat4f &model_matrix)
        {
            drawer.draw_points3d(
                igris::array_view<rabbit::vec3f>((rabbit::vec3f *)lines.data(),
                                                 lines.size()),
                GL_LINE_STRIP,
                model_matrix,
                camera_pose_matrix(),
                camera_projection_matrix());
        }

        void draw_mesh(const rabbit::mesh<float> &mesh,
                       const rabbit::mat4f &model_matrix,
                       const vec4f& color)
        {
            drawer.draw_mesh(mesh,
                             model_matrix,
                             camera_pose_matrix(),
                             camera_projection_matrix(),
                             color);
        }

        /*void draw_triangles(
                float * points,
                size_t nbpoints,
                int * triangles,
                size_t nbtriangles,
                const rabbit::mat4f& model_matrix
        )
        {
            drawer.draw_triangles(
                points,
                nbpoints,
                triangles,
                nbtriangles,
                model_matrix,
                camera_pose_matrix(),
                camera_projection_matrix()
            );
        }*/
    };

    class scene
    {
    public:
        std::list<std::shared_ptr<view>> views;
        std::list<drawable_object *> objects;

    public:
        std::shared_ptr<view> create_view()
        {
            std::shared_ptr<view> view = std::make_shared<rabbit::view>(this);
            views.push_back(view);
            return view;
        }

        void update()
        {
            for (auto &v : views)
            {
                v->clean();
                for (auto &obj : objects)
                {
                    obj->draw_on(*v);
                }
            }
        }

        void add_object(drawable_object *obj)
        {
            objects.push_back(obj);
        }
    };

    class polyline_drawable_object : public drawable_object
    {
        std::vector<vec3f> points;

    public:
        polyline_drawable_object(const std::vector<vec3f> &points)
            : points(points)
        {
        }

        void draw_on(rabbit::view &view) override
        {
            view.draw_line_strip(points, pose.to_mat4());
        }
    };

    class mesh_drawable_object : public drawable_object
    {
        rabbit::mesh<float> mesh;
        rabbit::vec4f color;

    public:
        mesh_drawable_object(const rabbit::mesh<float> &mesh) : mesh(mesh) {}

        void set_color(rabbit::vec4f color) 
        {
            this->color = color;
        }

        void draw_on(rabbit::view &view) override
        {
            view.draw_mesh(mesh, pose.to_mat4(), color);
        }
    };

}

#endif