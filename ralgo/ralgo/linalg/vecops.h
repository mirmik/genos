#ifndef RALGO_VECTOR_OPERATIONS_H
#define RALGO_VECTOR_OPERATIONS_H

/**
@file
*/

#include <algorithm>
#include <assert.h>
#include <iterator>
#include <ralgo/util/helpers.h>
#include <tuple>

#include <igris/math.h>
#include <igris/util/signature.h>
#include <ralgo/linalg/vecops_base.h>

namespace ralgo
{
    IGRIS_SIGNATURE_ATTRIBUTE_CHECKER(has_reserve, reserve)

    namespace vecops
    {
        template <class T> class constant_generator
        {
            T val;

        public:
            constant_generator(T v) : val(v) {}

            T operator[](int i)
            {
                (void)i;
                return val;
            }
        };

        template <class R = void, class V, class F>
        defvec_t<R, V> transform(const V &u, F &&f)
        {
            defvec_t<R, V> ret;
            std::transform(
                std::begin(u), std::end(u), std::back_inserter(ret), f);
            return ret;
        }

        template <class R = void, class V> defvec_t<R, V> list(const V &u)
        {
            defvec_t<R, V> ret(u.size());
            for (int i = 0; i < u.size(); ++i)
                ret[i] = u[i];
            return ret;
        }

        template <class V> void fill(V &arr, const value_t<V> &val)
        {
            for (auto &a : arr)
                a = val;
        }

        template <class M> void clean(M &arr)
        {
            vecops::fill(arr, typename M::value_type{});
        }

        // построить целочисленный вектор арифметической прогрессии [start;stop)
        // с шагом step.
        // TODO: дополнить нецелочисленным вариантом
        template <class V> auto arange(int start, int stop, int step)
        {
            size_t l = (stop - start) / step;
            int cur = start;
            V r(l);

            for (auto &v : r)
            {
                v = cur;
                cur += step;
            }

            return r;
        }

        template <class A, class B> void swap(A &&a, B &&b)
        {
            for (unsigned int i = 0; i < a.size(); ++i)
            {
                std::swap(a[i], b[i]);
            }
        }

        template <class V = void> defvec_of_t<V, int> arange(int stop)
        {
            defvec_of_t<V, int> r(stop);

            for (int i = 0; i < stop; ++i)
            {
                r[i] = i;
            }

            return r;
        }

        // Построить линейное пространство аля numpy.
        template <class R = void, class T>
        defvec_of_t<R, T> linspace(T s, T f, int n)
        {
            defvec_of_t<R, T> r(n);
            T k;

            for (int i = 0; i < n; ++i)
            {
                k = (T)i / ((T)n - 1);
                r[i] = s * (1 - k) + f * k;
            }

            return r;
        }

        template <class R = void, class V>
        auto copy(V &&v) -> defvec_of_t<R, value_t<V>>
        {
            defvec_of_t<R, value_t<V>> vec;
            vec.reserve(v.size());
            std::copy(std::begin(v), std::end(v), std::back_inserter(vec));
            return vec;
        }

        template <class A, class B> void copy(const A &a, B &&b)
        {
            b.resize(a.size());
            for (unsigned int i = 0; i < a.size(); ++i)
                b[i] = a[i];
        }

        template <class A, class B> void copy_without_resize(const A &a, B &&b)
        {
            size_t size = __MIN__(a.size(), b.size());
            for (unsigned int i = 0; i < size; ++i)
                b[i] = a[i];
        }

        // Создаёт векторизованный вриант функции.
        template <class V, class R = V, typename F, class... Args>
        constexpr auto vectorize(const F &f, Args &&... args)
        {
            auto lamda = [=](const V &vec) {
                return elementwise<R>(f, vec, args...);
            };
            return lamda;
        }

        // Векторизованные операции над комплексными числами.
        template <class R = void, class A> defsame_t<R, A> abs(const A &obj)
        {
            return elementwise<R>(ralgo::op_abs(), obj);
        }
        template <template <class C> class V, class T>
        auto real(const V<T> &obj)
        {
            return elementwise<V<T>>([](const auto &c) { return c.real(); },
                                     obj);
        }
        template <template <class C> class V, class T>
        auto imag(const V<T> &obj)
        {
            return elementwise<V<T>>([](const auto &c) { return c.imag(); },
                                     obj);
        }

        // Тригонометрия.
        template <class R = void, class A> defsame_t<R, A> sin(const A &obj)
        {
            return elementwise<R>(ralgo::op_sin(), obj);
        }
        template <class R = void, class A> defsame_t<R, A> cos(const A &obj)
        {
            return elementwise<R>(ralgo::op_cos(), obj);
        }

        // Вернуть реверс вектора src
        template <typename T> T reverse(const T &src)
        {
            T dst(src.size());
            auto sit = src.begin();
            auto eit = src.end();
            auto dit = dst.rbegin();

            for (; sit != eit;)
            {
                *dit++ = *sit++;
            }

            return dst;
        }

        template <class A, class B> value_t<A> dot_product(A &&a, B &&b)
        {
            value_t<A> acc;
            for (int i = 0; i < a.size(); i++)
            {
                acc += a[i] * b[i];
            }
            return acc;
        }

        template <class A, class B> value_t<A> inner_product(A &&a, B &&b)
        {
            value_t<A> acc = {};
            for (unsigned int i = 0; i < a.size(); i++)
            {
                acc += a[i] * b[i];
            }
            return acc;
        }

        template <typename T>
        T slice(const T &src, size_t start, size_t size, size_t stride = 1)
        {
            T dst(size);

            size_t idx = start;

            for (unsigned int i = 0; i < size; ++i)
            {
                dst[i] = src[idx];
                idx += stride;
            }

            return dst;
        }

        // Вычислить длину вектора по евклидовой метрике.
        template <typename V> double norm(const V &vec)
        {
            double res = 0;

            for (const auto &val : vec)
            {
                auto a = std::abs(val);
                res += a * a;
            }

            return sqrt(res);
        }
        template <typename V> double length(const V &vec)
        {
            return norm(vec);
        }

        // Вычислить расстояние между кортежами равного размера по
        // евклидовой метрике.
        template <class R = double, class A, class B>
        R distance2(const A &a, const B &b)
        {
            R accum = 0;

            for (unsigned int i = 0; i < a.size(); ++i)
            {
                auto diff = a[i] - b[i];
                accum += diff * diff;
            }

            return accum;
        }

        // Вычислить расстояние между кортежами равного размера по
        // евклидовой метрике.
        template <class R = double, class A, class B>
        R distance(const A &a, const B &b)
        {
            return sqrt(distance2<R, A, B>(a, b));
        }

        template <class R, class A> R cast(const A &a)
        {
            return elementwise<R>([](auto x) { return x; }, a);
        }

        template <class A> bool all(const A &a)
        {
            return fold(op_and(), true, a);
        }
        template <class A> bool any(const A &a)
        {
            return fold(op_or(), false, a);
        }

        // template <class A, class B> bool equal_all(const A& a, const B& b)
        //{ return fold(ralgo::op_not_eq(), a, veciter(b)); }

        template <class A, class B> bool equal(const A &a, const B &b)
        {
            return ralgo::vecops::boolean_all(op_eq(), a, b);
        }

        template <class A, class B, class T>
        bool equal(const A &a, const B &b, T epsilon)
        {
            return ralgo::vecops::boolean_all(op_eq(), a, b, epsilon);
        }

        template <class A, class B> bool equal_all(const A &a, const B &b)
        {
            return boolean_all(ralgo::op_eq(), a, b);
        }
        template <class A, class B> bool equal_any(const A &a, const B &b)
        {
            return boolean_any(ralgo::op_eq(), a, b);
        }

        template <class R = void, class A, class B>
        defsame_t<R, A> add_vs(const A &a, B b)
        {
            return elementwise<R>(ralgo::op_add(), a, b);
        }
        template <class R = void, class A, class B>
        defsame_t<R, A> sub_vs(const A &a, B b)
        {
            return elementwise<R>(ralgo::op_sub(), a, b);
        }
        template <class R = void, class A, class B>
        defsame_t<R, A> mul_vs(const A &a, B b)
        {
            return elementwise<R>(ralgo::op_mul(), a, b);
        }

        template <class R = void, class A, class B>
        defsame_t<R, A> div_vs(const A &a, B b)
        {
            return elementwise<R>(ralgo::op_div(), a, b);
        }

        template <class R = void, class A, class B>
        defsame_t<R, A> add_vv(const A &a, const B &b)
        {
            return elementwise2<R>(ralgo::op_add(), a, b);
        }
        template <class R = void, class A, class B>
        defsame_t<R, A> sub_vv(const A &a, const B &b)
        {
            return elementwise2<R>(ralgo::op_sub(), a, b);
        }
        template <class R = void, class A, class B>
        defsame_t<R, A> mul_vv(const A &a, const B &b)
        {
            return elementwise2<R>(ralgo::op_mul(), a, b);
        }
        template <class R = void, class A, class B>
        defsame_t<R, A> div_vv(const A &a, const B &b)
        {
            return elementwise2<R>(ralgo::op_div(), a, b);
        }

        template <class A, class B, class C>
        void add_vv_to(C &c, const A &a, const B &b)
        {
            return elementwise2_to(c, ralgo::op_add(), a, b);
        }
        template <class A, class B, class C>
        void sub_vv_to(C &c, const A &a, const B &b)
        {
            return elementwise2_to(c, ralgo::op_sub(), a, b);
        }
        template <class A, class B, class C>
        void mul_vv_to(C &c, const A &a, const B &b)
        {
            return elementwise2_to(c, ralgo::op_mul(), a, b);
        }
        template <class A, class B, class C>
        void div_vv_to(C &c, const A &a, const B &b)
        {
            return elementwise2_to(c, ralgo::op_div(), a, b);
        }

        template <class A, class S, class R>
        void scalar_add(const A &a, S b, R &&res)
        {
            elementwise_to(res, ralgo::op_add(), a, b);
        }
        template <class A, class S, class R>
        void scalar_sub(const A &a, S b, R &&res)
        {
            elementwise_to(res, ralgo::op_sub(), a, b);
        }
        template <class A, class S, class R>
        void scalar_div(const A &a, S b, R &&res)
        {
            elementwise_to(res, ralgo::op_div(), a, b);
        }
        template <class A, class S, class R>
        void scalar_mul(const A &a, S b, R &&res)
        {
            elementwise_to(res, ralgo::op_mul(), a, b);
        }

        template <class R = void, class A> defsame_t<R, A> normalize(const A &a)
        {
            double norm = ralgo::vecops::norm(a);
            return ralgo::vecops::div_vs<defsame_t<R, A>, A, double>(a, norm);
        }

        template <class R = void, class Head, class... Args>
        auto zip(Head &&head, Args &&... args)
            -> defsame_t<R, std::vector<std::tuple<Args...>>>
        {
            defsame_t<R, std::vector<std::tuple<Args...>>> ret;
            ret.reserve(head.size());

            for (int i = 0; i < head.size(); ++i)
            {
                ret.emplace_back(head[i], args[i]...);
            }

            return ret;
        }

        template <class R = void, class Direction, class Box>
        defsame_t<R, Direction> ray_to_box(const Direction &u, Box &&box)
        {
            double result = std::numeric_limits<double>::max();
            size_t sz = std::min(u.size(), box.size());
            for (size_t i = 0; i < sz; ++i)
            {
                auto proj = std::abs(u[i]);
                auto bound = box[i];
                if (bound == 0)
                {
                    if (proj == 0)
                        continue;
                    else
                        return defsame_t<R, Direction>(u.size(), 0);
                }
                auto quotient = bound / proj;
                if (quotient < result)
                    result = quotient;
            }
            return vecops::mul_vs(u, result);
        }

        template <class R = void, class Vector, class Box>
        defsame_t<R, Vector> bound_to_box(const Vector &u, Box &&box)
        {
            double norm_of_d = ralgo::vecops::norm(u);
            auto d = ralgo::vecops::div_vs(u, norm_of_d);
            auto r = ray_to_box(d, box);
            auto norm_of_r = ralgo::vecops::norm(r);
            return norm_of_r < norm_of_d ? r : u;
        }

        namespace inplace
        {
            // Применить функцию f ко всем элементам массива v. Допускается
            // передача дополнительных аргументов.
            template <class V, class F, class... A>
            V &elementwise(V &v, F f, A &&... a)
            {
                for (auto &s : v)
                    s = f(s, std::forward<A>(a)...);
                return v;
            }

            template <class V> void clean(V &v)
            {
                elementwise(v, [](auto &) { return typename V::value_type(); });
            }

            template <class V, class S> V &add(V &vec, S m)
            {
                for (auto &val : vec)
                    val += m;
                return vec;
            }
            template <class V, class S> V &sub(V &vec, S m)
            {
                for (auto &val : vec)
                    val -= m;
                return vec;
            }
            template <class V, class S> V &mul(V &vec, S m)
            {
                for (auto &val : vec)
                    val *= m;
                return vec;
            }
            template <class V, class S> V &div(V &vec, S m)
            {
                for (auto &val : vec)
                    val /= m;
                return vec;
            }

            template <class V> V &conj(V &vec)
            {
                for (auto &val : vec)
                    val = std::conj(val);
                return vec;
            }

            template <class V> V &sin(V &vec)
            {
                return elementwise(vec, ralgo::sin<value_t<V>>);
            }
            template <class V> V &cos(V &vec)
            {
                return elementwise(vec, ralgo::cos<value_t<V>>);
            }
            template <class V> V &tan(V &vec)
            {
                return elementwise(vec, ralgo::tan<value_t<V>>);
            }
            template <class V> V &exp(V &vec)
            {
                return elementwise(vec, ralgo::exp<value_t<V>>);
            }
            template <class V> V &log(V &vec)
            {
                return elementwise(vec, ralgo::log<value_t<V>>);
            }

            template <class V> V &log10(V &vec)
            {
                return elementwise(vec, ralgo::log10<value_t<V>>);
            }

            template <class V> V &normalize(V &vec)
            {
                double norm = ralgo::vecops::norm(vec);
                return ralgo::vecops::inplace::div(vec, norm);
            }
        }

        template <class VI, class WI, class RI>
        void merge_sorted(VI vit, const VI vend, WI wit, const WI wend, RI rit)
        {
            while (vit != vend && wit != wend)
            {
                bool cmp = *vit < *wit;

                if (cmp)
                {
                    *rit++ = *vit++;
                }
                else
                {
                    *rit++ = *wit++;
                }
            }

            while (vit != vend)
            {
                *rit++ = *vit++;
            }
            while (wit != wend)
            {
                *rit++ = *wit++;
            }
        }

        template <class V, class W, class R>
        void merge_sorted(
            const V &v, const W &w, R writer, double start, double stop)
        {
            auto vit = std::lower_bound(v.begin(), v.end(), start);
            auto wit = std::lower_bound(w.begin(), w.end(), start);
            auto vend = std::upper_bound(v.begin(), v.end(), stop);
            auto wend = std::upper_bound(w.begin(), w.end(), stop);

            return merge_sorted(vit, vend, wit, wend, writer);
        }

        template <class V, class W>
        auto merge_sorted(const V &v, const W &w, double start, double stop)
        {
            std::vector<value_t<V>> r;
            merge_sorted(v, w, std::back_inserter(r), start, stop);
            return r;
        }
    }

    /*using vecops::elementwise;
    using vecops::elementwise2;
    using vecops::vectorize;
    using vecops::arange;
    using vecops::linspace;
    using vecops::merge_sorted;*/

    namespace inplace = vecops::inplace;
}

#endif
