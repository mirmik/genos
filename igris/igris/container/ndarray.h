#ifndef IGRIS_NDARRAY_H
#define IGRIS_NDARRAY_H

#include <array>
#include <concepts>
#include <igris/container/array_view.h>
#include <stdexcept>
#include <vector>

namespace igris
{
    template <class T> concept ArrayType = requires(const T &a)
    {
        std::size(a);
        std::begin(a);
        std::end(a);
    };

    template <class C, class V> size_t array_dimension(const C &container)
    {
        if constexpr (std::convertible_to<C, V>)
            return 0;
        if constexpr (ArrayType<C>)
            return array_dimension<decltype(*std::begin(container)), V>(
                       *std::begin(container)) +
                   1;
        else
            return 0;
    }

    template <class C, class V, class S>
    void assign_array_shape(const C &container, S &shape, int idx)
    {
        if constexpr (std::convertible_to<C, V>)
            return;
        else if constexpr (ArrayType<C>)
        {
            shape[idx] = container.size();
            assign_array_shape<decltype(*std::begin(container)), V>(
                *std::begin(container), shape, idx + 1);
        }
    }

    template <typename Value> class ndarray
    {
        std::vector<size_t> _shape = {};
        std::vector<Value> _values = {};

    public:
        ndarray() = default;
        ndarray(const ndarray &other) = default;
        ndarray(ndarray &&other) = default;
        ndarray &operator=(const ndarray &other) = default;
        ndarray &operator=(ndarray &&other) = default;

        std::vector<Value> &storage()
        {
            return _values;
        }

        const std::vector<Value> &storage() const
        {
            return _values;
        }

        template <class C> void init(const C &container)
        {
            size_t dim = igris::array_dimension<C, Value>(container);

            _shape.resize(dim);
            igris::assign_array_shape<C, Value>(container, _shape, 0);

            _values.reserve(plane_size());
            set_values(container);
        }

        ndarray(const std::vector<Value> &container,
                const std::vector<size_t> &shape)
        {
            init(container);
            reshape(shape);
        }

        ndarray(const std::initializer_list<Value> &container,
                const std::initializer_list<size_t> &shape)
        {
            init(container);
            reshape(shape);
        }

        ndarray(const std::initializer_list<Value> &container)
        {
            init(container);
        }

        ndarray(const std::initializer_list<std::initializer_list<Value>>
                    &container)
        {
            init(container);
        }

        ndarray(const std::initializer_list<
                std::initializer_list<std::initializer_list<Value>>> &container)
        {
            init(container);
        }

        size_t dim() const
        {
            return _shape.size();
        }

        size_t plane_size()
        {
            size_t acc = 1;
            for (size_t i = 0; i < dim(); ++i)
                acc *= _shape[i];
            return acc;
        }

        size_t planed_index(const std::initializer_list<size_t> &indexes) const
        {
            return planed_index(indexes.begin(), indexes.size());
        }

        template <class Index>
        size_t planed_index(const Index *indexes, size_t) const
        {
            int acc = 0;
            int step = 1;
            for (int i = dim() - 1; i >= 0; --i)
            {
                acc += indexes[i] * step;
                step *= _shape[i];
            }
            return acc;
        }

        template <class Index>
        size_t planed_index(const igris::array_view<Index> &indexes) const
        {
            return planed_index(indexes.data(), indexes.size());
        }

        Value &operator()(const size_t *indexes, size_t)
        {
            return _values[planed_index(indexes)];
        }

        Value &operator()(const int *indexes, size_t)
        {
            return _values[planed_index(indexes)];
        }

        Value &operator()(const igris::array_view<size_t> &indexes)
        {
            return _values[planed_index(indexes)];
        }

        Value &operator()(const std::vector<size_t> &indexes)
        {
            return _values[planed_index(indexes.data(), indexes.size())];
        }

        template <class Indexes> Value &operator()(const Indexes &indexes)
        {
            return _values[planed_index(indexes.data(), indexes.size())];
        }

        template <class Indexes>
        const Value &operator()(const Indexes &indexes) const
        {
            return _values[planed_index(indexes.data(), indexes.size())];
        }

        Value &operator()(const std::initializer_list<size_t> &indexes)
        {
            return _values[planed_index(igris::array_view<size_t>(indexes))];
        }

        const Value &
        operator()(const std::initializer_list<size_t> &indexes) const
        {
            return _values[planed_index(igris::array_view<size_t>(indexes))];
        }

        const std::vector<size_t> &shape() const
        {
            return _shape;
        }

        void reshape(const igris::array_view<size_t> &indexes)
        {
            _shape = {indexes.data(), indexes.data() + indexes.size()};
        }

        template <class Indexes> void reshape(const Indexes &indexes)
        {
            _shape.resize(indexes.size());
            std::copy(indexes.begin(), indexes.end(), _shape.begin());
        }

        void resize(igris::array_view<size_t> newshape)
        {
            _shape.resize(newshape.size());
            std::copy(newshape.begin(), newshape.end(), _shape.begin());
            _values.resize(plane_size());
        }

        void resize(const std::vector<size_t> &newshape)
        {
            _shape.resize(newshape.size());
            std::copy(newshape.begin(), newshape.end(), _shape.begin());
            _values.resize(plane_size());
        }

        ndarray operator+(const ndarray &oth)
        {
            ndarray ret;
            ret.resize(shape());

            for (size_t i = 0; i < plane_size(); ++i)
            {
                ret._values[i] = _values[i] + oth._values[i];
            }

            return ret;
        }

        ndarray operator-(const ndarray &oth)
        {
            ndarray ret;
            ret.resize(shape());

            for (size_t i = 0; i < plane_size(); ++i)
            {
                ret._values[i] = _values[i] - oth._values[i];
            }

            return ret;
        }

        ndarray operator*(double scalar)
        {
            ndarray ret;
            ret.resize(shape());

            for (size_t i = 0; i < plane_size(); ++i)
            {
                ret._values[i] = _values[i] * scalar;
            }

            return ret;
        }

        ndarray operator/(double scalar)
        {
            ndarray ret;
            ret.resize(shape());

            for (size_t i = 0; i < plane_size(); ++i)
            {
                ret._values[i] = _values[i] / scalar;
            }

            return ret;
        }

        /*Value &operator[](size_t i)
        {
            return _values[i];
        }

        const Value &operator[](size_t i) const
        {
            return _values[i];
        }*/

        bool operator==(const ndarray &oth) const
        {
            return _shape == oth._shape && _values == oth._values;
        }

        bool operator!=(const ndarray &oth) const
        {
            return _shape != oth._shape || _values != oth._values;
        }

        auto get(std::vector<std::vector<size_t>> indexes)
        {
            std::vector<Value> rets;
            for (size_t i = 0; i < indexes.size(); ++i)
            {
                rets.push_back((*this)(indexes[i]));
            }
            return rets;
        }

        template <class Set> auto get(const Set &indexes)
        {
            std::vector<Value> rets;
            for (size_t i = 0; i < indexes.size(); ++i)
            {
                rets.push_back((*this)(indexes[i]));
            }
            return rets;
        }

        auto begin()
        {
            return _values.begin();
        }

    private:
        template <class C> void set_values(const C &val)
        {
            if constexpr (std::convertible_to<C, Value>)
                _values.push_back(val);
            else if constexpr (ArrayType<C>)
                for (auto &c : val)
                    set_values(c);
            else
                throw std::logic_error("ndarray wrong constructor");
        }
    };
}

#endif