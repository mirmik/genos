#ifndef RABBIT_MESH_H
#define RABBIT_MESH_H

#include <nos/trent/trent.h>
#include <rabbit/geom/surface.h>
#include <rabbit/third/linalg.h>
#include <rabbit/third/stl_reader.h>
#include <vector>

namespace rabbit
{
    template <class T = float> class mesh
    {
    private:
        std::vector<linalg::vec<T, 3>> _vertices;
        std::vector<linalg::vec<unsigned int, 3>> _triangles;

    public:
        mesh(const std::vector<linalg::vec<T, 3>> &_vertices,
             const std::vector<linalg::vec<unsigned int, 3>> &_triangles)
            : _vertices(_vertices), _triangles(_triangles)
        {
        }

        mesh() = default;

        mesh(std::vector<linalg::vec<T, 3>> &&_vertices,
             std::vector<linalg::vec<unsigned int, 3>> &&_triangles)
            : _vertices(std::move(_vertices)), _triangles(std::move(_triangles))
        {
        }

        std::vector<linalg::vec<T, 3>> vertices() const
        {
            return _vertices;
        }
        std::vector<linalg::vec<unsigned int, 3>> triangles() const
        {
            return _triangles;
        }

        linalg::vec<T, 3> center()
        {
            linalg::vec<double, 3> acc = {0, 0, 0};
            int n = _vertices.size();

            for (auto &v : _vertices)
            {
                acc += v;
            }

            return linalg::vec<T, 3>{acc / n};
        }

        void correct_center()
        {
            auto c = center();

            for (auto &v : _vertices)
            {
                v -= c;
            }
        }

        std::string to_json_string()
        {
            std::string json = "{\n";
            json += "\t\"vertices\": [\n";
            for (auto &v : _vertices)
            {
                json += "\t\t[" + std::to_string(v.x) + ", " +
                        std::to_string(v.y) + ", " + std::to_string(v.z) +
                        "],\n";
            }
            json += "\t],\n";
            json += "\t\"triangles\": [\n";
            for (auto &t : _triangles)
            {
                json += "\t\t[" + std::to_string(t.x) + ", " +
                        std::to_string(t.y) + ", " + std::to_string(t.z) +
                        "],\n";
            }
            json += "\t]\n";
            json += "}\n";
            return json;
        }

        static mesh from_trent(const nos::trent &tr)
        {
            std::vector<linalg::vec<T, 3>> vertices;
            std::vector<linalg::vec<unsigned int, 3>> triangles;

            auto &vtr = tr["vertices"].as_list();
            auto &ttr = tr["triangles"].as_list();

            for (auto &v : vtr)
            {
                vertices.emplace_back(
                    v[0].as_numer(), v[1].as_numer(), v[2].as_numer());
            }

            for (auto &t : ttr)
            {
                triangles.emplace_back(
                    t[0].as_numer(), t[1].as_numer(), t[2].as_numer());
            }

            return mesh<T>(std::move(vertices), std::move(triangles));
        }

        nos::trent to_trent()
        {
            nos::trent tr;

            auto &vtr = tr["vertices"].as_list();
            for (auto &v : _vertices)
            {
                nos::trent vtr2;
                vtr2.as_list();
                vtr2.push_back(v.x);
                vtr2.push_back(v.y);
                vtr2.push_back(v.z);
                vtr.push_back(vtr2);
            }

            auto &ttr = tr["triangles"].as_list();
            for (auto &t : _triangles)
            {
                nos::trent ttr2;
                ttr2.as_list();
                ttr2.push_back(t.x);
                ttr2.push_back(t.y);
                ttr2.push_back(t.z);
                ttr.push_back(ttr2);
            }

            return tr;
        }
    };

    template <class T = float>
    mesh<T> surface_rubic_mesh(const surface &surf,
                               T ustrt,
                               T ufini,
                               int utotal,
                               T vstrt,
                               T vfini,
                               int vtotal)
    {
        int iter;

        std::vector<linalg::vec<T, 3>> vertices;
        std::vector<linalg::vec<unsigned int, 3>> triangles;

        vertices.resize((utotal + 1) * (vtotal + 1));
        triangles.resize(2 * utotal * vtotal);

        iter = 0;
        for (int j = 0; j < vtotal + 1; ++j)
        {
            for (int i = 0; i < utotal + 1; ++i)
            {
                T ku = ((T)i / (T)utotal);
                T kv = ((T)j / (T)vtotal);

                T u = ustrt * ((T)1. - ku) + ufini * ku;
                T v = vstrt * ((T)1. - kv) + vfini * kv;

                auto val = surf.value(u, v);
                vertices[iter++] = {(T)val.x, (T)val.y, (T)val.z};
            }
        }

        iter = 0;
        for (int j = 0; j < vtotal; ++j)
        {
            for (int i = 0; i < utotal; ++i)
            {
                unsigned int a = i + (j) * (utotal + 1);
                unsigned int b = i + (j) * (utotal + 1) + 1;
                unsigned int c = i + (1 + j) * (utotal + 1);
                unsigned int d = i + (1 + j) * (utotal + 1) + 1;

                triangles[iter++] = {a, b, c};
                triangles[iter++] = {d, c, b};
            }
        }

        return mesh<T>(std::move(vertices), std::move(triangles));
    }

    template <class T = float>
    mesh<T> surface_rubic_mesh(const surface &surf, int utotal, int vtotal)
    {
        return surface_rubic_mesh<T>(surf,
                                     surf.umin(),
                                     surf.umax(),
                                     utotal,
                                     surf.vmin(),
                                     surf.vmax(),
                                     vtotal);
    }

    template <class T = float> mesh<T> mesh_from_file(const char *path)
    {
        std::vector<float> coords, normals;
        std::vector<unsigned int> tris, solids;

        std::vector<linalg::vec<T, 3>> vertices;
        std::vector<linalg::vec<unsigned int, 3>> triangles;

        stl_reader::ReadStlFile(path, coords, normals, tris, solids);

        int vertices_total = coords.size() / 3;
        int triangles_total = tris.size() / 3;

        vertices.resize(vertices_total);
        triangles.resize(triangles_total);

        for (unsigned int i = 0; i < vertices_total; ++i)
            vertices[i] = {coords[i * 3], coords[i * 3 + 1], coords[i * 3 + 2]};

        for (unsigned int i = 0; i < triangles_total; ++i)
            triangles[i] = {tris[i * 3], tris[i * 3 + 1], tris[i * 3 + 2]};

        return mesh<T>(std::move(vertices), std::move(triangles));
    }

    std::vector<linalg::vec<unsigned int, 2>> edges_from_triangles(
        const std::vector<linalg::vec<unsigned int, 3>> &triangles);
}

#endif