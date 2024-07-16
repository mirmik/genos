#ifndef NAMED_CARTESIAN_CORRECTION_H
#define NAMED_CARTESIAN_CORRECTION_H

#include <igris/util/ab_converter.h>
#include <ndmath/cartesian_correction.h>
#include <nos/trent/trent.h>

namespace ndmath
{
    class named_cartesian_correction
    {
    private:
        std::vector<std::string> names = {};
        std::vector<std::vector<double>> grid = {};
        igris::ndarray<ndmath::vector> deltas = {};

    public:
        cartesian_correction make_correction_table() const
        {
            cartesian_correction ret;
            ret.set_grid(grid);
            ret.set_deltas(deltas);

            return ret;
        }

        static inline auto foo(const nos::trent &tr, auto it)
        {
            if (tr[0].is_numer())
            {
                ndmath::vector vec;
                for (auto &t : tr.as_list())
                    vec.push_back(t.as_numer());
                *it = vec;
                return ++it;
            }
            else
            {
                for (const auto &t : tr.as_list())
                    it = foo(t, it);
                return it;
            }
        }

    public:
        named_cartesian_correction() = default;
        named_cartesian_correction(const named_cartesian_correction &) =
            default;
        named_cartesian_correction &
        operator=(const named_cartesian_correction &) = default;

        named_cartesian_correction(std::vector<std::string> names,
                                   std::vector<std::vector<double>> grid,
                                   igris::ndarray<ndmath::vector> deltas) :
            names(names), grid(grid), deltas(deltas)
        {
        }

        named_cartesian_correction(const nos::trent &tr)
        {
            for (const auto &t : tr["names"].as_list())
                names.push_back(t.as_string());

            {
                int i = 0;
                grid.resize(tr["grid"].as_list().size());
                for (const auto &t : tr["grid"].as_list())
                {
                    for (const auto &d : t.as_list())
                    {
                        if (!d.is_numer())
                            throw std::runtime_error(
                                "Cell of grid is not a number");
                        grid[i].push_back(d.as_numer());
                    }
                    i++;
                }
            }

            {
                int grid_dimension = names.size();
                std::vector<size_t> shape(grid_dimension);

                const nos::trent *it = &tr["deltas"];
                for (int i = 0; i < grid_dimension; i++)
                {
                    shape[i] = it->as_list().size();
                    it = &(*it)[0];
                }

                deltas.resize(shape);
                foo(tr["deltas"], deltas.begin());
            }
        }
    };
}

#endif