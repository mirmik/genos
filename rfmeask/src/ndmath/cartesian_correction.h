#ifndef RABBIT_DELTAZONE_H
#define RABBIT_DELTAZONE_H

#include <igris/container/ndarray.h>
#include <igris/util/ab_converter.h>
#include <ndmath/cartesian_correction.h>
#include <ndmath/point.h>
#include <ndmath/polysegment.h>
#include <ndmath/segment.h>
#include <nos/print.h>
#include <ralgo/binary_hypercube.h>
#include <ralgo/boundary_box.h>
#include <ralgo/cartesian_grid.h>
#include <ralgo/linalg/vecops.h>
#include <ralgo/linalg/vector.h>
#include <ralgo/linspace.h>

namespace ndmath
{
    class correction_data
    {
        std::vector<ndmath::point> _points = {};
        std::vector<ndmath::vector> _deltas = {};

    public:
        correction_data(const std::vector<ndmath::point> &pnts,
                        const std::vector<ndmath::vector> &dlts) :
            _points(pnts), _deltas(dlts)
        {
        }

        std::vector<ndmath::point> points()
        {
            return _points;
        }
        std::vector<ndmath::vector> deltas()
        {
            return _deltas;
        }

        ndmath::polysegment compile_polysegment()
        {
            ndmath::polysegment ret;
            for (size_t i = 0; i < _points.size(); ++i)
                ret.add_point(_points[i] + _deltas[i]);
            return ret;
        }
    };

    class cartesian_sliced_zone
    {
        std::vector<std::vector<double>> coords = {};
        std::vector<size_t> collapsed_dims = {};

    public:
        std::vector<std::vector<double>> get_coords() const
        {
            return coords;
        }

        cartesian_sliced_zone(std::vector<std::vector<double>> coords) :
            coords(coords)
        {
        }

        cartesian_sliced_zone() {}

        cartesian_sliced_zone(const cartesian_sliced_zone &oth) :
            coords(oth.coords), collapsed_dims(oth.collapsed_dims)
        {
        }

        cartesian_sliced_zone &operator=(const cartesian_sliced_zone &oth)
        {
            coords = oth.coords;
            collapsed_dims = oth.collapsed_dims;
            return *this;
        }

        std::vector<size_t> sizes()
        {
            std::vector<size_t> ret;
            for (auto &r : coords)
                ret.push_back(r.size());
            return ret;
        }

        void set_grid(const std::vector<std::vector<double>> &zone)
        {
            this->coords = zone;
        }

        // определяет принадлежность к измерению,
        // сетка по которому не построена.
        bool is_collapsed_dim(size_t i)
        {
            return std::find(collapsed_dims.begin(), collapsed_dims.end(), i) !=
                   collapsed_dims.end();
        }

        // определяет измерению
        // находится ли точка в границах зоны
        // по одному из измерений.
        bool in_dim_interval()
        {
            return false;
        }

        size_t dim() const
        {
            return coords.size();
        }

        // В какой ячейке находится точка?
        // Условно левые индексы начинаются с нуля.
        auto point_in_cell_indices(ndmath::point point)
        {
            return ralgo::number_of_cartesian_grid_interval_for_ndim_point<
                ralgo::vector<size_t>>(coords, point);
        }

        // Возвращает ограничивающую коробку для ячейки.
        ralgo::boundary_box<double> cellzone(ralgo::vector<size_t> indexes)
        {
            ralgo::vector<double> mins(indexes.size());
            ralgo::vector<double> maxs(indexes.size());

            for (size_t i = 0; i < indexes.size(); ++i)
            {
                if (indexes[i] == 0)
                    mins[i] = -std::numeric_limits<double>::infinity();
                else
                    mins[i] = coords[i][indexes[i] - 1];

                if (indexes[i] == coords[i].size())
                    maxs[i] = std::numeric_limits<double>::infinity();
                else
                    maxs[i] = coords[i][indexes[i]];
            }

            return {mins, maxs};
        }
    };

    class cartesian_correction
    {
        igris::ndarray<ndmath::vector> _deltas = {};
        cartesian_sliced_zone _grid = {};
        std::vector<size_t> grid_index_converter = {};

    public:
        cartesian_correction() {}
        cartesian_correction(const cartesian_correction &other) = default;
        cartesian_correction(cartesian_correction &&other) = default;
        cartesian_correction &
        operator=(const cartesian_correction &other) = default;
        cartesian_correction &operator=(cartesian_correction &&other) = default;

        cartesian_sliced_zone &grid()
        {
            return _grid;
        }

        const cartesian_sliced_zone &grid() const
        {
            return _grid;
        }

        igris::ndarray<ndmath::vector> &deltas()
        {
            return _deltas;
        }
        const igris::ndarray<ndmath::vector> &deltas() const
        {
            return _deltas;
        }

        size_t griddim() const
        {
            return _grid.dim();
        }

        // Приводит дельта вектор приведенный к полному виду. // Выпилено.
        ralgo::vector<double> fulled_delta(const ralgo::vector<double> &delta)
        {
            return delta;
        }

        size_t dim() const
        {
            return _grid.dim();
        }

        void set_deltas(const igris::ndarray<ndmath::vector> &deltas)
        {
            _deltas = deltas;
        }

        ndmath::vector
        apply_lerpcoeffs(ralgo::vector<double> coeffs,
                         std::vector<ralgo::vector<double>> celldeltas)
        {
            ndmath::vector ret(coeffs.size());
            auto cube = ralgo::binary_hypercube_vertices(coeffs.size());
            for (size_t v = 0; v < cube.size(); ++v)
            {
                double mul = 1;
                for (size_t i = 0; i < cube[v].size(); ++i)
                {
                    mul *= cube[v][i] ? coeffs[i] : 1 - coeffs[i];
                }
                ret += celldeltas[v] * mul;
            }
            return ret;
        }

        auto corrected_point(const ndmath::vector &pnt)
        {
            auto cellzone_index = _grid.point_in_cell_indices(pnt);
            auto cellzone = _grid.cellzone(cellzone_index);
            auto deltas_indices =
                ralgo::cell_indices_hypercube_with_collapsed_bounds(
                    cellzone_index, _grid.sizes());
            auto coeffs = cellzone.lerpcoeffs(pnt);
            auto celldeltas = _deltas.get(deltas_indices);
            auto pnt_delta = apply_lerpcoeffs(coeffs, celldeltas);
            return pnt + fulled_delta(pnt_delta);
        }

        auto cell_for_point(const ndmath::vector &pnt)
        {
            auto cellzone_index = _grid.point_in_cell_indices(pnt);
            return _grid.cellzone(cellzone_index);
        }

        auto inverted_corrected_point(const ndmath::vector &pnt)
        {
            auto cellzone_index = _grid.point_in_cell_indices(pnt);
            auto cellzone = _grid.cellzone(cellzone_index);
            auto deltas_indices =
                ralgo::cell_indices_hypercube_with_collapsed_bounds(
                    cellzone_index, _grid.sizes());
            auto coeffs = cellzone.lerpcoeffs(pnt);
            auto celldeltas = _deltas.get(deltas_indices);
            auto pnt_delta = apply_lerpcoeffs(coeffs, celldeltas);
            return pnt - fulled_delta(pnt_delta);
        }

        auto corrected_points(const std::vector<ndmath::vector> &pnts)
        {
            std::vector<ndmath::vector> ret(pnts.size());
            for (size_t i = 0; i < pnts.size(); ++i)
                ret[i] = corrected_point(pnts[i]);
            return ret;
        }

        void set_grid(const std::vector<std::vector<double>> &grid)
        {
            _grid.set_grid(grid);
        }

        std::vector<ndmath::vector>
        line_and_grid_collisions_unsorted(const ndmath::vector &a,
                                          const ndmath::vector &b)
        {
            std::vector<ndmath::vector> unsorted_points;
            auto gridcoords = _grid.get_coords();
            for (size_t dimindex = 0; dimindex < gridcoords.size(); ++dimindex)
            {
                auto start_level_of_this_dim = a[dimindex];
                auto finish_level_of_this_dim = b[dimindex];

                if (start_level_of_this_dim == finish_level_of_this_dim)
                    continue;

                auto &coords = gridcoords[dimindex];
                for (size_t i = 0; i < coords.size(); ++i)
                {
                    auto plane_level = coords[i];
                    if (plane_level < start_level_of_this_dim ||
                        plane_level > finish_level_of_this_dim)
                        continue;

                    auto lerp_koeff =
                        (plane_level - start_level_of_this_dim) /
                        (finish_level_of_this_dim - start_level_of_this_dim);

                    auto founded_point = a + (b - a) * lerp_koeff;
                    unsorted_points.push_back(founded_point);
                }
            }
            return unsorted_points;
        }

        std::vector<double>
        projection_coeffs_for_points(const ndmath::vector &a,
                                     const ndmath::vector &b,
                                     std::vector<ndmath::vector> points)
        {
            std::vector<double> ret(points.size());
            auto ab = b - a;
            auto ab_dist = ralgo::vecops::norm(ab);
            for (size_t i = 0; i < points.size(); ++i)
            {
                auto ap = points[i] - a;
                auto ap_dist = ralgo::vecops::norm(ap);
                ret[i] = ap_dist / ab_dist;
            }
            return ret;
        }

        static std::pair<std::vector<double>, std::vector<size_t>>
        enumerated_sort(std::vector<double> elems)
        {
            std::vector<size_t> indexes(elems.size());
            std::vector<double> sorted_elems(elems.size());

            std::iota(indexes.begin(), indexes.end(), 0);
            std::sort(
                indexes.begin(), indexes.end(), [&elems](size_t i1, size_t i2) {
                    return elems[i1] < elems[i2];
                });

            for (size_t i = 0; i < elems.size(); ++i)
            {
                sorted_elems[i] = elems[indexes[i]];
            }

            return {sorted_elems, indexes};
        }

        std::pair<std::vector<double>, std::vector<ndmath::vector>>
        filter_coeffs(std::vector<double> coeffs,
                      double E,
                      std::vector<ndmath::vector> unsorted)
        {
            __label__ next;
            std::vector<double> filtered_coeffs;
            std::vector<ndmath::vector> filtered_unsorted;
            for (size_t i = 0; i < coeffs.size(); ++i)
            {
                for (auto &r : filtered_coeffs)
                {
                    if (std::abs(r - coeffs[i]) < E)
                        goto next;
                }
                filtered_coeffs.push_back(coeffs[i]);
                filtered_unsorted.push_back(unsorted[i]);

            next:;
            }
            return {filtered_coeffs, filtered_unsorted};
        }

        std::vector<ndmath::vector>
        line_and_grid_collisions(const ndmath::vector &a,
                                 const ndmath::vector &b,
                                 bool with_a_and_b,
                                 double E = 1e-5)
        {
            auto unsorted = line_and_grid_collisions_unsorted(a, b);
            if (with_a_and_b)
            {
                unsorted.push_back(a);
                unsorted.push_back(b);
            }

            auto coeffs = projection_coeffs_for_points(a, b, unsorted);
            auto [filtered_coeffs, filtered_usorted] =
                filter_coeffs(coeffs, E, unsorted);

            auto [sorted_coeffs, indexes] = enumerated_sort(filtered_coeffs);
            std::vector<ndmath::vector> ret(filtered_usorted.size());
            for (size_t i = 0; i < filtered_usorted.size(); ++i)
            {
                ret[i] = filtered_usorted[indexes[i]];
            }

            return ret;
        }
    };
}

#endif