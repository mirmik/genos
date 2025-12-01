
/**
    @file delegate.h

    @brief
    Реализация делегатов.
    Работа с указателями на метод реализуется с помощью horrible_cast.

    В данной реализации делегат является шаблонным классом, что ограничивает
    его возможности одной сигнатурой. Однако, позволяет возвращать результат.

    @note
    Реализация использует UB, предполагая определённый формат указатель на метод
    виртуального класса, который, строго говоря, не гарантирован стандартом.
    Использовать с осторожностью.
*/

#ifndef IGRIS_DELEGATE_H
#define IGRIS_DELEGATE_H

#include "igris/util/horrible_cast.h"
#include <utility>

#include "inttypes.h"

namespace igris
{

    class AbstractDelegate
    {
    };

    template <typename T, typename B> struct change_basic
    {
    };
    template <typename T, typename B, typename R, typename... V>
    struct change_basic<T, R (B::*)(V...)>
    {
        using type = R (T::*)(V...);
    };

    template <typename R, typename... Args> class delegate;
    template <typename R, typename... Args> class fastdelegate;
    template <typename R, typename... Args> class callback;

    //! Делегат. Шаблонный класс.
    //! Параметры шаблона задают сигнатуру делегируемой функции.
    //! @param R тип возвращаемый тип.
    //! @param Args - ... Типы параметров. Доступно неограниченное количество
    //! параметров.
    template <typename R, typename... Args> class delegate
    {
    protected:
        enum
        {
            METHOD,
            FUNCTION
        };

        using obj_t = AbstractDelegate *;
        using mtd_t = R (AbstractDelegate::*)(Args...);
        using fnc_t = R (*)(Args...);
        using extfnc_t = R (*)(void *, Args...);
        using absmemb_t = std::pair<mtd_t, obj_t>;

        //Соответствует истине и будет работать только в G++
        union method_union
        {
            mtd_t method = nullptr;
            struct
            {
                union
                {
                    fnc_t function;
                    extfnc_t external_function;
                };
                union
                {
                    fnc_t attributes;
                    uintptr_t external_attributes;
                };
            } part;
        };

    protected:
        union
        {
            obj_t object = {};
            void *external_object;
        };
        method_union method = {};

    public:
        void clean()
        {
            object = 0;
            method.part.function = nullptr;
            method.part.attributes = 0;
        }

        extfnc_t get_extfunc() const { return method.part.external_function; }
        void* get_object() const { return external_object; }

        //! Проверяет, установлен ли какой-либо обработчик.
        bool armed() const { return method.part.function != nullptr; }

        delegate() { clean(); }

        delegate(const delegate &d)
        {
            object = d.object;
            method.method = d.method.method;
        };

        delegate(delegate &&d)
        {
            object = d.object;
            method.method = d.method.method;
        };

        delegate(const fnc_t func)
        {
            object = 0;
            method.part.function = func;
            method.part.attributes = 0;
        };

        delegate(const extfnc_t func, void *obj)
        {
            external_object = obj;
            method.part.external_function = func;
            method.part.external_attributes = -1;
        };

        template <typename T> delegate(R (T::*mtd)(Args...), T *ptr_obj)
        {
            object = reinterpret_cast<obj_t>(ptr_obj);
            method.method = horrible_cast<mtd_t, R (T::*)(Args...)>(mtd);
        }

        template <typename F> explicit delegate(const F &functor)
        {
            object = reinterpret_cast<obj_t>((F *)&functor);
            method.method =
                horrible_cast<mtd_t, decltype(&F::operator())>(&F::operator());
        }

        delegate &operator=(const delegate &d)
        {
            object = d.object;
            method.method = d.method.method;
            return *this;
        };

        delegate &operator=(delegate &&d)
        {
            object = d.object;
            method = d.method;
            return *this;
        };

        template<class ... UArgs>
        R operator()(UArgs&&... args) const
        { 
            return invoke(std::forward<UArgs>(args)...); 
        }

        template<class ... UArgs>
        R invoke(UArgs&&... args) const
        {
            if (!armed())
                return R();

            if (method.part.external_attributes == (uintptr_t)-1)
            {
                return method.part.external_function(
                    external_object, std::forward<UArgs>(args)...);
            }

            uint8_t type = object ? METHOD : FUNCTION;
            if (type == METHOD)
                return (object->*method.method)(std::forward<UArgs>(args)...);
            else
                return method.part.function(std::forward<UArgs>(args)...);
        }

        operator bool() const { 
            return armed(); };

        bool operator==(delegate<R, Args...> b)
        {
            return method.method == b.method.method && object == b.object;
        };

        //! Вызвать единожды и сбросить обработчик.
        template<class ... UArgs>
        R invoke_and_reset(UArgs&&... args)
        {
            delegate<R, UArgs...> copydlg = *this;
            clean();
            return copydlg(std::forward<UArgs>(args)...);
        }
    };

    template <typename T, typename Ret, typename... Args>
    delegate<Ret, Args...> make_delegate(Ret (T::*mtd)(Args...), T *ptr)
    {
        return delegate<Ret, Args...>(mtd, ptr);
    }

    template <typename Ret, typename... Args>
    delegate<Ret, Args...> make_delegate(Ret (*fnc)(Args...))
    {
        return delegate<Ret, Args...>(fnc);
    }

    template <typename Ret, typename... Args>
    delegate<Ret, Args...> make_delegate(Ret (*fnc)(void *, Args...),
                                         void *extobj)
    {
        return delegate<Ret, Args...>(fnc, extobj);
    }
}

#endif
