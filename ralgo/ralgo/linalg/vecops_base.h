#ifndef RALGO_VECOPS_BASE_H
#define RALGO_VECOPS_BASE_H

/**
@file
*/

#include <functional>
#include <ralgo/util/helpers.h>
#include <type_traits>

namespace ralgo
{
    namespace vecops
    {

        /*		// Тип, соответствующий типу возвращаемому при итерации
           контейнера. template <class V> using veciter_ret_t =
           std::result_of<decltype(V::operator[])>;

                // Итератор, передаваемый в обобщенные функции, использующие
           распаковку. template <class A> struct veciter
                {
                    const A& ref;
                    constexpr inline veciter(const A& ref) : ref(ref) {}
                };

                // Функция итерирует
                template <class A> struct veciter_unpack
                {
                    constexpr auto operator()(A&& arg)
                    {
                        return std::forward<A>(arg);
                    }
                };

                template <class A> struct iter_unpack<veciter<A>>
                {
                    constexpr auto operator()(const veciter<A>& arg)
                    {
                        return arg.ref;
                    }
                };*/

        // Свёртка списка по функции func.
        template <class F, class A, class R = std::invoke_result_t<F(A)>>
        R fold(F &&func, const R &initval, const A &a)
        {
            R accum = initval;

            auto ait = std::begin(a);
            auto aeit = std::end(a);

            for (; ait != aeit; ++ait)
            {
                accum = func(accum, *ait);
            }

            return accum;
        }

        template <class F, class A, class B, class... Args>
        bool boolean_all(F &&func, const A &a, const B &b, Args &&... args)
        {
            auto ait = std::begin(a);
            auto bit = std::begin(b);
            auto aeit = std::end(a);
            for (; ait != aeit; ++ait, ++bit)
                if (func(*ait, *bit, std::forward<Args>(args)...) == false)
                    return false;
            return true;
        }
        template <class F, class A, class B, class... Args>
        bool boolean_any(F &&func, const A &a, const B &b, Args &&... args)
        {
            auto ait = std::begin(a);
            auto bit = std::begin(b);
            auto aeit = std::end(a);
            for (; ait != aeit; ++ait, ++bit)
                if (func(*ait, *bit, std::forward<Args>(args)...) == true)
                    return true;
            return false;
        }

        // Применить функцию f ко всем элементам массива a. Допускается передача
        // дополнительных аргументов.
        template <class R = void, class F, class A, class... Args>
        defsame_t<R, A> elementwise(const F &f, const A &a, Args &&... args)
        {
            defsame_t<R, A> ret(a.size());

            auto ait = std::begin(a), aend = std::end(a);
            auto cit = std::begin(ret);

            for (; ait != aend; ++ait, ++cit)
                *cit = f(*ait, std::forward<Args>(args)...);

            return ret;
        }

        // Применить функцию f ко всем элементам массивов a и b. Допускается
        // передача дополнительных аргументов.
        template <class R = void, class F, class A, class B, class... Args>
        defsame_t<R, A>
        elementwise2(const F &f, const A &a, const B &b, Args &&... args)
        {
            assert(a.size() == b.size());
            defsame_t<R, A> c(a.size());

            auto ait = std::begin(a), aend = std::end(a);
            auto bit = std::begin(b);
            auto cit = std::begin(c);

            for (; ait != aend; ++ait, ++bit, ++cit)
                *cit = f(*ait, *bit, std::forward<Args>(args)...);

            return c;
        }

        template <class C, class F, class A, class... Args>
        void elementwise_to(C &c, const F &f, const A &a, Args &&... args)
        {
            c.resize(a.size());

            auto ait = std::begin(a), aend = std::end(a);
            auto cit = std::begin(c);

            for (; ait != aend; ++ait, ++cit)
                *cit = f(*ait, std::forward<Args>(args)...);
        }

        template <class C, class F, class A, class B, class... Args>
        void elementwise2_to(
            C &c, const F &f, const A &a, const B &b, Args &&... args)
        {
            auto ait = std::begin(a), aend = std::end(a);
            auto bit = std::begin(b);
            auto cit = std::begin(c);

            for (; ait != aend; ++ait, ++bit, ++cit)
                *cit = f(*ait, *bit, std::forward<Args>(args)...);
        }

    }
}

#endif
