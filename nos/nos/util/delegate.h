
/**
    @file delegate.h

    @brief
    Реализация делегатов.
    Работа с указателями на метод реализуется с помощью nos::horrible_cast.

    В данной реализации делегат является шаблонным классом, что ограничивает
    его возможности одной сигнатурой. Однако, позволяет возвращать результат.

    @note
    Реализация использует UB, предполагая определённый формат указатель на метод
    виртуального класса, который, строго говоря, не гарантирован стандартом.
    Использовать с осторожностью.
*/

#ifndef NOS_DELEGATE_H
#define NOS_DELEGATE_H

#include <nos/util/horrible_cast.h>
#include <utility>
#include <stdint.h>

namespace nos
{
    class AbstractDelegate
    {
    };

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
            mtd_t method = {};
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
            method.method = nos::horrible_cast<mtd_t, R (T::*)(Args...)>(mtd);
        }

        template <typename F> explicit delegate(const F &functor)
        {
            object = reinterpret_cast<obj_t>((F *)&functor);
            method.method =
                nos::horrible_cast<mtd_t, decltype(&F::operator())>(&F::operator());
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

        R operator()(Args... args) const
        { 
            return emit(std::forward<Args>(args)...); }

        R emit(Args... args) const
        {
            if (!armed())
                return R();

            if (method.part.external_attributes == (uintptr_t)-1)
            {
                return method.part.external_function(
                    external_object, std::forward<Args>(args)...);
            }

            uint8_t type = object ? METHOD : FUNCTION;
            if (type == METHOD)
                return (object->*method.method)(std::forward<Args>(args)...);
            else
                return method.part.function(std::forward<Args>(args)...);
        }

        operator bool() const { 
            return armed(); };

        bool operator==(delegate<R, Args...> b)
        {
            return method.method == b.method.method && object == b.object;
        };

        //! Вызвать единожды и сбросить обработчик.
        R emit_and_reset(Args... args)
        {
            delegate<R, Args...> copydlg = *this;
            clean();
            return copydlg(std::forward<Args>(args)...);
        };
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
