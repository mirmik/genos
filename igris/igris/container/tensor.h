#ifndef IGRIS_MARRAY_H
#define IGRIS_MARRAY_H

#include <igris/compiler.h>
#include <igris/container/array_view.h>
#include <igris/container/unbounded_array.h>
#include <vector>

namespace igris
{
    template <class T> class grid : public std::vector<std::vector<T>>
    {
    public:
        using std::vector<std::vector<T>>::operator[];

    public:
        grid() = default;
        grid(const std::vector<std::vector<T>> &other)
            : std::vector<std::vector<T>>(other)
        {
        }
        grid &operator=(const std::vector<std::vector<T>> &other)
        {
            std::vector<std::vector<T>>::operator=(other);
            return *this;
        }
        grid(const grid &other) = default;
        grid &operator=(const grid &other) = default;
        grid(grid &&other) = default;
        grid &operator=(grid &&other) = default;

        std::vector<size_t> shape()
        {
            std::vector<size_t> res;
            for (size_t i = 0; i < this->size(); ++i)
            {
                res.push_back(this->operator[](i).size());
            }
            return res;
        }

        std::vector<T> operator[](igris::array_view<size_t> idxs)
        {
            std::vector<T> res;

            assert(idxs.size() == this->size());

            for (size_t i = 0; i < idxs.size(); ++i)
            {
                auto &lay = this->operator[](i);
                auto idx = lay[idxs[i]];
                res.push_back(idx);
            }
            return res;
        }
    };

    template <class T> class tensor
    {

        class indices_iterator
        {
            tensor<T> *_tensor;
            std::vector<size_t> _idxs;

        public:
            indices_iterator(tensor<T> *tensor, std::vector<size_t> idxs)
                : _tensor(tensor), _idxs(idxs)
            {
            }

            indices_iterator(const indices_iterator &other) = default;
            indices_iterator(indices_iterator &&other) = default;
            indices_iterator &
            operator=(const indices_iterator &other) = default;
            indices_iterator &operator=(indices_iterator &&other) = default;

            T &operator*()
            {
                return _tensor->at(_idxs);
            }

            const std::vector<size_t> &indices()
            {
                return _idxs;
            }

            indices_iterator operator++(int)
            {
                auto cur = indices_iterator(_tensor, _idxs);
                std::vector<size_t> idxs = _idxs;
                for (size_t i = 0; i < _idxs.size(); ++i)
                {
                    size_t j = _idxs.size() - i - 1;

                    if (idxs[j] + 1 < _tensor->shape()[j])
                    {
                        idxs[j]++;
                        _idxs = idxs;
                        return cur;
                    }
                    else
                    {
                        idxs[j] = 0;
                    }
                }
                _idxs = {};
                return cur;
            }

            indices_iterator &operator++()
            {
                (*this)++;
                return *this;
            }

            bool operator==(const indices_iterator &other)
            {
                return _idxs == other._idxs && _tensor == other._tensor;
            }

            bool operator!=(const indices_iterator &other)
            {
                return _idxs != other._idxs || _tensor != other._tensor;
            }
        };

        std::shared_ptr<igris::unbounded_array<T>> _storage = {};
        T *_storview = {};
        std::vector<size_t> _shape = {};
        std::vector<size_t> _stride = {};

    public:
        tensor() = default;
        tensor(std::vector<size_t> shape)
        {
            _shape = shape;
            size_t storsize = shape_product();
            _storage = std::make_shared<igris::unbounded_array<T>>(storsize);
            _storview = _storage->data();
            _storage->fill(T());
            _stride = continuougs_stride_for_shape(_shape);
        }

        tensor &operator=(const tensor &other) = default;
        tensor &operator=(tensor &&other) = default;
        tensor(const tensor &other) = default;
        tensor(tensor &&other) = default;

        tensor(std::shared_ptr<igris::unbounded_array<T>> _storage,
               T *_storview,
               std::vector<size_t> _shape,
               std::vector<size_t> _stride)
            : _storage(_storage), _storview(_storview), _shape(_shape),
              _stride(_stride)
        {
        }

        tensor(const std::initializer_list<T> &container,
               const std::initializer_list<size_t> &shape)
        {
            _storage = std::make_shared<igris::unbounded_array<T>>(container);
            _storview = _storage->data();
            _shape = shape;
            _stride = continuougs_stride_for_shape(_shape);
        }

        tensor(const std::vector<T> &container,
               const std::vector<size_t> &shape)
        {
            const T *data = container.data();
            size_t size = container.size();
            _storage = std::make_shared<igris::unbounded_array<T>>(data, size);
            _storview = _storage->data();
            _shape = shape;
            _stride = continuougs_stride_for_shape(_shape);
        }

        static std::vector<size_t>
        continuougs_stride_for_shape(const std::vector<size_t> &shape)
        {
            std::vector<size_t> stride(shape.size());
            size_t st = 1;
            for (size_t i = 0; i < shape.size(); ++i)
            {
                stride[shape.size() - i - 1] = st;
                st *= shape[shape.size() - i - 1];
            }
            return stride;
        }

        tensor copy()
        {
            // copy storage
            auto new_storage =
                std::make_shared<igris::unbounded_array<T>>(*_storage);
            auto new_storview = new_storage->data();
            return tensor(new_storage, new_storview, _shape, _stride);
        }

        igris::unbounded_array<T> &storage()
        {
            return *_storage;
        }

        const igris::unbounded_array<T> &storage() const
        {
            return *_storage;
        }

        std::shared_ptr<igris::unbounded_array<T>> storage_pointer()
        {
            return _storage;
        }

        igris::array_view<T> storage_view()
        {
            return _storview;
        }

        size_t shape_product()
        {
            size_t res = 1;
            for (size_t i = 0; i < _shape.size(); ++i)
            {
                res *= _shape[i];
            }
            return res;
        }

        tensor reshape(std::vector<size_t> shape)
        {
            tensor res;
            res._storage = _storage;
            res._storview = _storview;
            res._shape = shape;
            res._stride.resize(shape.size());
            size_t stride = 1;
            for (size_t i = 0; i < shape.size(); ++i)
            {
                res._stride[shape.size() - i - 1] = stride;
                stride *= shape[shape.size() - i - 1];
            }
            return res;
        }

        indices_iterator begin()
        {
            return indices_iterator(this,
                                    std::vector<size_t>(_shape.size(), 0));
        }

        indices_iterator end()
        {
            return indices_iterator(this, std::vector<size_t>());
        }

        tensor resize(std::vector<size_t> newshape)
        {
            tensor res(newshape);

            auto it = begin();
            auto eit = end();
            for (; it != eit; ++it)
            {
                auto indices = it.indices();
                if (res.has_indices(indices))
                    res(indices) = (*this)(indices);
            }
            return res;
        }

        std::vector<size_t> shape()
        {
            return _shape;
        }

        std::vector<size_t> stride()
        {
            return _stride;
        }

        bool is_contiguous()
        {
            // tensor is contiguous if it stride array is ordered
            for (size_t i = 0; i < _stride.size() - 1; ++i)
            {
                if (_stride[i] < _stride[i + 1])
                    return false;
            }
            return true;
        }

        tensor operator[](size_t idx)
        {
            tensor res;
            res._shape = _shape;
            res._stride = _stride;
            res._storage = _storage;
            res._storview = _storview + idx * _stride[0];
            res._shape.erase(res._shape.begin());
            res._stride.erase(res._stride.begin());
            return res;
        }

        tensor operator[](std::vector<size_t> indexes)
        {
            auto t = *this;
            for (size_t i = 0; i < indexes.size(); ++i)
            {
                t = t[indexes[i]];
            }
            return t;
        }

        tensor transpose()
        {
            tensor res;
            res._storage = _storage;
            res._storview = _storview;
            res._shape = reverse_vector(_shape);
            res._stride = reverse_vector(_stride);
            return res;
        }

        static std::vector<size_t> reverse_vector(std::vector<size_t> vec)
        {
            std::vector<size_t> res;
            for (size_t i = 0; i < vec.size(); ++i)
            {
                res.push_back(vec[vec.size() - i - 1]);
            }
            return res;
        }

        tensor permute(std::vector<size_t> idxs)
        {
            tensor res;
            res._storage = _storage;
            res._storview = _storview;
            res._shape.resize(idxs.size());
            res._stride.resize(idxs.size());

            for (size_t i = 0; i < idxs.size(); ++i)
            {
                res._shape[i] = _shape[idxs[i]];
                res._stride[i] = _stride[idxs[i]];
            }

            return res;
        }

        T &at_ordinal(size_t ord)
        {
            return _storage[ordinal_to_storage_index(ord)];
        }

        size_t ordinal_to_storage_index(size_t ord)
        {
            size_t idx = 0;
            for (size_t i = 0; i < _shape.size(); ++i)
            {
                size_t j = _shape.size() - i - 1;
                idx += _stride[j] * (ord % _shape[j]);
                ord /= _shape[j];
            }
            return idx;
        }

        tensor contiguous()
        {
            tensor res(shape());
            size_t sz = res.storage().size();
            for (size_t i = 0; i < sz; ++i)
            {
                res._storview[i] = _storview[ordinal_to_storage_index(i)];
            }
            return res;
        }

        tensor unsqueeze(size_t dim)
        {
            std::vector<size_t> newshape = _shape;
            newshape.insert(newshape.begin() + dim, 1);

            std::vector<size_t> newstride = _stride;
            newstride.insert(newstride.begin() + dim, 1);

            tensor res;
            res._storage = _storage;
            res._storview = _storview;
            res._shape = newshape;
            res._stride = newstride;
            return res;
        }

        bool has_indices(std::vector<size_t> idxs)
        {
            if (idxs.size() != _shape.size())
                return false;

            for (size_t i = 0; i < idxs.size(); ++i)
            {
                if (idxs[i] >= _shape[i])
                    return false;
            }
            return true;
        }

        T &operator()(std::vector<size_t> idxs)
        {
            return at(idxs);
        }

        T &at(std::vector<size_t> idxs)
        {
            size_t idx = 0;
            for (size_t i = 0; i < idxs.size(); ++i)
            {
                idx += idxs[i] * _stride[i];
            }

            return _storview[idx];
        }

        void clear()
        {
            _storage->clear();
            _shape.clear();
        }
    };
}

#endif