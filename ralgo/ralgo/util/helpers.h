#ifndef RALGO_UTIL_FUNC_HELPERS_H
#define RALGO_UTIL_FUNC_HELPERS_H

/**
    @file
*/

#include <cmath>
#include <complex>
#include <numeric>
#include <ralgo/linalg/vector.h>
#include <vector>

#define TEMPORARY_STORAGE(type, size, name) std::vector<type> name(size)

namespace ralgo
{
    constexpr const double epsilon = std::numeric_limits<double>::epsilon();

    // Достать тип значения из контейнера.
    template <typename V> struct value
    {
        using type =
            typename std::remove_reference_t<std::remove_cv_t<V>>::value_type;
    };
    template <typename T, int N> struct value<T[N]>
    {
        using type = T;
    };
    template <typename V> using value_t = typename value<V>::type;

    template <typename V> struct vector_value
    {
        using type = std::remove_reference_t<decltype(std::declval<V>()[0])>;
    };
    template <typename T, int N> struct vector_value<T[N]>
    {
        using type = T;
    };
    template <typename V> using vector_value_t = typename vector_value<V>::type;

    // Превести тип комплексного аргумента или вектора к соответствующему
    // базовому скалярному типу.
    template <typename T> struct scalar
    {
        using type = T;
    };
    template <typename T> struct scalar<std::complex<T>>
    {
        using type = T;
    };
    template <typename T> using scalar_t = typename scalar<T>::type;

    // Шаблонные обёртки над стандартными тригонометрическими операциями.
    template <class T> constexpr T sin(T x)
    {
        return std::sin(x);
    }
    template <class T> constexpr T cos(T x)
    {
        return std::cos(x);
    }
    template <class T> constexpr T tan(T x)
    {
        return std::tan(x);
    }

    template <class T> constexpr T exp(T x)
    {
        return std::exp(x);
    }
    template <class T> constexpr T log(T x)
    {
        return std::log(x);
    }
    template <class T> constexpr T log10(T x)
    {
        return std::log10(x);
    }

    struct op_add
    {
        template <class A, class B>
        auto operator()(const A &a, const B &b) const
        {
            return a + b;
        }
    };
    struct op_sub
    {
        template <class A, class B>
        auto operator()(const A &a, const B &b) const
        {
            return a - b;
        }
    };
    struct op_mul
    {
        template <class A, class B>
        auto operator()(const A &a, const B &b) const
        {
            return a * b;
        }
    };
    struct op_div
    {
        template <class A, class B>
        auto operator()(const A &a, const B &b) const
        {
            return a / b;
        }
    };

    struct op_sin
    {
        template <class A> auto operator()(const A &a) const
        {
            return sin(a);
        }
    };
    struct op_cos
    {
        template <class A> auto operator()(const A &a) const
        {
            return cos(a);
        }
    };

    struct op_abs
    {
        template <class A> auto operator()(const A &a) const
        {
            return std::abs(a);
        }
    };

    struct op_eq
    {
        template <class A, class B>
        auto operator()(const A &a, const B &b) const
        {
            return a == b;
        }
        template <class A, class B, class T>
        auto operator()(const A &a, const B &b, T epsilon) const
        {
            return std::abs(a - b) < epsilon;
        }
    };

    struct op_not_eq
    {
        template <class A, class B>
        auto operator()(const A &a, const B &b) const
        {
            return a != b;
        }
        template <class A, class B, class T>
        auto operator()(const A &a, const B &b, T epsilon) const
        {
            return std::abs(a - b) >= epsilon;
        }
    };

    struct op_and
    {
        template <class A, class B>
        bool operator()(const A &a, const B &b) const
        {
            return a && b;
        }
    };
    struct op_or
    {
        template <class A, class B>
        bool operator()(const A &a, const B &b) const
        {
            return a || b;
        }
    };
    struct op_bin_and
    {
        template <class A, class B>
        auto operator()(const A &a, const B &b) const
        {
            return a & b;
        }
    };
    struct op_bin_or
    {
        template <class A, class B>
        auto operator()(const A &a, const B &b) const
        {
            return a | b;
        }
    };

    template <typename... T> struct rettype
    {
    };
    template <class A, class B> struct rettype<A, B>
    {
        using type = A;
    };
    ///	template <template<class> V, class T> struct rettype { using type =
    /// V<T>; };
    template <typename... T> using rettype_t = typename rettype<T...>::type;

    // Определяет тип по умолчанию. Если тип равен void, то используется
    // std::vector<>.
    template <class R, class V> struct defvec
    {
        using type = R;
    };
    template <class V> struct defvec<void, V>
    {
        using type = ralgo::vector<
            std::remove_reference_t<decltype(std::declval<V>()[0])>>;
    };
    template <class R, class V> using defvec_t = typename defvec<R, V>::type;

    // Определяет тип по умолчанию. Если тип равен void, то используется
    // std::vector<>.
    template <class R, class V> struct defvec_of
    {
        using type = R;
    };
    template <class V> struct defvec_of<void, V>
    {
        using type = ralgo::vector<V>;
    };
    template <class R, class V>
    using defvec_of_t = typename defvec_of<R, V>::type;

    // Определяет тип по умолчанию. Если тип равен void, то используется тип
    // результат функтора.
    template <class R, class F> struct fretvec
    {
        using type = R;
    };
    template <class F> struct fretvec<void, F>
    {
        using type = ralgo::vector<std::invoke_result<F>>;
    };
    template <class R, class F> using fretvec_t = typename fretvec<R, F>::type;

    // Определяет тип по умолчанию. Если тип равен void, то используется тип по
    // умолчанию.
    template <class R, class V> struct defsame
    {
        using type = R;
    };
    template <class V> struct defsame<void, V>
    {
        using type = V;
    };
    template <class R, class V> using defsame_t = typename defsame<R, V>::type;

    template <class T, int K = 0> struct optional_vector_resize_helper
    {
        static void doit(T &a, int arg)
        {
            a.resize(arg);
        }
    };

    template <class T> struct optional_vector_resize_helper<T, 0>
    {
        static void doit(T &, int) {}
    };

    template <class T> void optional_vector_resize(T &a, int b)
    {
        optional_vector_resize_helper<T>::doit(a, b);
    }

    template <class T> constexpr bool is_vector_compatible();
    template <class T> struct is_vector_compatible_checker
    {
    private:
        template <class U>
        static decltype(
            std::declval<U>().size(),
            std::declval<U>().data(),
            std::enable_if_t<
                !is_vector_compatible<decltype(std::declval<U>()[0])>(),
                bool>(),
            std::true_type())
        test(int);
        template <class> static std::false_type test(...);

    public:
        static constexpr const bool value = decltype(test<T>(0))::value;
    };

    template <class T> constexpr bool is_vector_compatible()
    {
        return is_vector_compatible_checker<std::remove_cv_t<T>>::value;
    }

    template <class T> struct is_matrix_compatible_checker
    {
    private:
        template <class U>
        static decltype(std::declval<U>().at(0, 0),
                        std::declval<U>().rows(),
                        std::declval<U>().cols(),
                        std::true_type())
        test(int);
        template <class> static std::false_type test(...);

    public:
        static constexpr const bool value = decltype(test<T>(0))::value;
    };

    template <class T> constexpr bool is_matrix_compatible()
    {
        return is_matrix_compatible_checker<std::remove_cv_t<T>>::value;
    }

    template <class C> constexpr auto size(const C &c) -> decltype(std::size(c))
    {
        return std::size(c);
    }

    template <class C> void resize(C &c, int n)
    {
        return c.resize(n);
    }

    template <class C> void resize(C &c, int m, int n)
    {
        return c.resize(m, n);
    }

    template <class C> constexpr const auto &get(const C &c, int i)
    {
        return c[i];
    }

    template <class C> constexpr const auto &get(const C &c, int i, int j)
    {
        return c.at(i, j);
    }

    template <class C> constexpr auto &get(C &c, int i)
    {
        return c[i];
    }

    template <class C> constexpr auto &get(C &c, int i, int j)
    {
        return c.at(i, j);
    }
}

#endif
