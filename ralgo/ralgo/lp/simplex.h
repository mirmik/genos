#ifndef RALGO_LP_SIMPLEX_H
#define RALGO_LP_SIMPLEX_H

#include <ralgo/linalg/matrix.h>
#include <ralgo/linalg/vecops.h>
#include <ralgo/linalg/vector.h>
#include <ralgo/linalg/vector_view.h>
#include <vector>

#include <ralgo/linalg/plud.h>

namespace ralgo
{
    /**
     *  Симплексом называется выпуклая линейная оболочка натянутая в
     * пространстве на минимальное количество вершин. Для пространства
     * размерности N количество вершин равно N + 1.
     */
    class simplex
    {
        std::vector<ralgo::vector<double>> vertices;

    public:
        simplex(const std::vector<ralgo::vector<double>> &verts)
            : vertices(verts)
        {
        }

        /// Перевод кординат из евклидового пространства-N в барицентрические
        /// координаты симплекса. Перевод осуществляется путём решения уравнения
        /// | x1 x2 x3 | | l1 |   | x |
        /// | y1 y2 y3 | | l2 | = | y |     ( N=2 )
        /// |  1  1  1 | | l3 |   | 1 |
        /// @pnt - вектор-N
        /// @return - вектор-N+1
        ralgo::vector<double> baricentric(const ralgo::vector<double> &pnt)
        {
            auto extdim = vertices.size();
            ralgo::matrix<double> vertices_with_ones(extdim, extdim);
            ralgo::vector<double> pnt_with_ones(pnt);
            pnt_with_ones.push_back(1);

            for (int i = 0; i < vertices_with_ones.cols(); ++i)
            {
                auto colview = vertices_with_ones.col(i);
                ralgo::vecops::copy_without_resize(vertices[i], colview);
                colview[extdim - 1] = 1;
            }

            // Решаем систему методом Гаусса.
            auto PLUD = ralgo::plud(vertices_with_ones);
            return PLUD.solve(pnt_with_ones);
        }

        /// Точка находится внутри симплекса, если все её барицентрические
        /// координаты лежат в интервале [0,1].
        bool point_in(const ralgo::vector<double> &pnt)
        {
            auto barcoords = baricentric(pnt);

            for (auto &c : barcoords)
                if (c > 1 || c < 0)
                    return false;

            return true;
        }
    };
}

#endif