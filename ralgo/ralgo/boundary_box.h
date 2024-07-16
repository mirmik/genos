#ifndef RALGO_BOUNDARY_BOX_H
#define RALGO_BOUNDARY_BOX_H

#include <cmath>
#include <igris/container/unbounded_array.h>
#include <ralgo/linalg/vector.h>
#include <ralgo/linspace.h>

namespace ralgo
{
    template <typename T> class boundary_box
    {
    public:
        igris::unbounded_array<T> _data;

    private:
        T *maxs()
        {
            return _data.data() + _data.size() / 2;
        }

        T *mins()
        {
            return _data.data();
        }

        const T *maxs() const
        {
            return _data.data() + _data.size() / 2;
        }

        const T *mins() const
        {
            return _data.data();
        }

    public:
        boundary_box(const ralgo::vector<T> &mins, const ralgo::vector<T> &maxs)
        {
            _data.resize(mins.size() * 2);
            std::copy(std::begin(mins), std::end(mins), this->mins());
            std::copy(std::begin(maxs), std::end(maxs), this->maxs());
        }

        ralgo::vector<T> lerpcoeffs(const ralgo::vector<T> &pnt)
        {
            ralgo::vector<T> coeffs(pnt.size());
            for (size_t i = 0; i < pnt.size(); i++)
            {
                coeffs[i] = ralgo::lerpcoeff(mins()[i], maxs()[i], pnt[i]);
            }
            return coeffs;
        }

        size_t dim() const
        {
            return _data.size() / 2;
        }

        ralgo::vector<T> maxs_as_vector() const
        {
            return {(T *)maxs(), _data.size() / 2};
        }

        ralgo::vector<T> mins_as_vector() const
        {
            return {(T *)mins(), _data.size() / 2};
        }
    };
}

#endif