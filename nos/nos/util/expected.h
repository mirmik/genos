#ifndef NOS_UTIL_EXPECTED_H
#define NOS_UTIL_EXPECTED_H

#include <algorithm>
#include <array>
#include <functional>
#include <nos/util/error.h>
#include <nos/util/unexpected.h>
#include <type_traits>

namespace nos
{
    template <typename T, typename E> class expected
    {
        template <typename U> struct ok_type_helper
        {
            using type = U;
        };

        template <typename U> struct ok_type_helper<U &>
        {
            using type = std::reference_wrapper<U>;
        };

        template <typename U> struct ok_type_helper<const U &>
        {
            using type = std::reference_wrapper<U>;
        };

        using ok_type = typename ok_type_helper<T>::type;
        using err_type = E;
        static constexpr size_t Size = sizeof(ok_type) > sizeof(err_type)
                                           ? sizeof(ok_type)
                                           : sizeof(err_type);

        bool _is_ok = false;
        std::array<uint8_t, Size> u = {};

    public:
        expected() : _is_ok(false) {}

        expected(const T &val) : _is_ok(true)
        {
            new (as_T_ptr()) ok_type(val);
        }

        bool operator==(const nos::unexpected<err_type> &u) const
        {
            return _is_ok ? false : u.error() == error();
        }

        expected &operator=(const expected &other)
        {
            if (_is_ok)
            {
                if (other._is_ok)
                {
                    *as_T_ptr() = *other.as_T_ptr();
                }
                else
                {
                    as_T_ptr()->~ok_type();
                    new (as_E_ptr()) err_type(*other.as_E_ptr());
                    _is_ok = false;
                }
            }
            else
            {
                if (other._is_ok)
                {
                    as_E_ptr()->~err_type();
                    new (as_T_ptr()) ok_type(*other.as_T_ptr());
                    _is_ok = true;
                }
                else
                {
                    *as_E_ptr() = *other.as_E_ptr();
                }
            }
            return *this;
        }

        expected(const err_type &err) : _is_ok(false)
        {
            new (as_E_ptr()) err_type(err);
        }

        expected(const expected &other) : _is_ok(other._is_ok)
        {
            if (_is_ok)
            {
                new (as_T_ptr()) ok_type(*other.as_T_ptr());
            }
            else
            {
                new (as_E_ptr()) err_type(*other.as_E_ptr());
            }
        }

        expected(expected &&other) : _is_ok(other._is_ok)
        {
            if (_is_ok)
            {
                new (as_T_ptr()) ok_type(std::move(*other.as_T_ptr()));
            }
            else
            {
                new (as_E_ptr()) err_type(std::move(*other.as_E_ptr()));
            }
        }

        ok_type *as_T_ptr()
        {
            return (ok_type *)u.data();
        }

        const ok_type *as_T_ptr() const
        {
            return (const ok_type *)u.data();
        }

        err_type *as_E_ptr()
        {
            return (err_type *)u.data();
        }

        const err_type *as_E_ptr() const
        {
            return (const err_type *)u.data();
        }

        ~expected()
        {
            if (_is_ok)
                as_T_ptr()->~ok_type();
            else
                as_E_ptr()->~err_type();
        }

        ok_type &value()
        {
            return *as_T_ptr();
        }

        const ok_type &value() const
        {
            return *as_T_ptr();
        }

        ok_type value_or(const ok_type &defval) const
        {
            return _is_ok ? value() : defval;
        }

        err_type &error()
        {
            return *as_E_ptr();
        }

        const err_type &error() const
        {
            return *as_E_ptr();
        }

        ok_type &operator*()
        {
            return value();
        }

        const ok_type &operator*() const
        {
            return value();
        }

        ok_type *operator->()
        {
            return as_T_ptr();
        }

        const ok_type *operator->() const
        {
            return as_T_ptr();
        }

        bool is_ok() const
        {
            return _is_ok;
        }

        bool is_error() const
        {
            return !_is_ok;
        }
    };

    template <class E> class expected<void, E>
    {
        using err_type = E;
        static constexpr size_t Size = sizeof(err_type);

        bool _is_ok = false;
        std::array<uint8_t, Size> u = {};

    public:
        expected() : _is_ok(true) {}

        expected(const err_type &err) : _is_ok(false)
        {
            new (as_E_ptr()) err_type(err);
        }

        err_type &error()
        {
            return *as_E_ptr();
        }

        const err_type &error() const
        {
            return *as_E_ptr();
        }

        err_type *as_E_ptr()
        {
            return (err_type *)u.data();
        }

        const err_type *as_E_ptr() const
        {
            return (const err_type *)u.data();
        }

        bool is_ok() const
        {
            return _is_ok;
        }

        bool is_error() const
        {
            return !_is_ok;
        }
    };

    template <typename T, typename E>
    expected<T, E> make_unexpected(const E &err)
    {
        return expected<T, E>(err);
    }
}

#endif