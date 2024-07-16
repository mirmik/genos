#ifndef RALGO_BINARY_HYPERCUBE_H
#define RALGO_BINARY_HYPERCUBE_H

#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <ralgo/linalg/vector.h>
#include <vector>

namespace ralgo
{
    static inline std::vector<ralgo::vector<size_t>>
    binary_hypercube_vertices(size_t dim)
    {
        std::vector<ralgo::vector<size_t>> vect = {};
        vect.resize(std::pow(2, dim));

        for (size_t i = 0; i < vect.size(); i++)
        {
            vect[i].resize(dim);
            size_t shift = 1;
            for (size_t j = 0; j < dim; j++)
            {
                auto val = !!(i & shift);
                shift <<= 1;
                vect[i][dim - j - 1] = val;
            }
        }

        return vect;
    }

    static inline auto
    cell_indices_hypercube_with_collapsed_bounds(ralgo::vector<size_t> indices,
                                                 std::vector<size_t> maxidx)
    {
        std::vector<ralgo::vector<size_t>> ret =
            ralgo::binary_hypercube_vertices(indices.size());
        for (auto &r : ret)
        {
            r += indices;
        }

        for (auto &r : ret)
        {
            for (size_t i = 0; i < r.size(); ++i)
            {
                if (r[i] >= maxidx[i])
                    r[i] = maxidx[i] - 1;
                else if (r[i] == 0)
                    r[i] = 0;
                else
                    r[i] -= 1;
            }
        }

        return ret;
    }

}

#endif