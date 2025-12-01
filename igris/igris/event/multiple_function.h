#ifndef IGRIS_MULTIPLE_FUNCTION_H
#define IGRIS_MULTIPLE_FUNCTION_H

/**
    @file multiple_function.h
    Аналог multiple_delegate для шаблона std::function
    множественных обработчиков.
*/

#include <vector>
#include <functional>

namespace igris
{
    template <typename... Args> class multiple_function
    {
    public:
        std::vector<std::function<void(Args...)>> vect = {};

        void operator()(const Args&... args)
        {
            invoke(args ...);
        }

        void invoke(const Args&... args) 
        {
            for (auto dlg : vect)
            {
                dlg(args ...);
            }            
        }

        template <typename... TArgs> void operator+=(TArgs... args)
        {
            add(std::forward<TArgs>(args)...);
        }

        template <typename... TArgs> void operator-=(TArgs... args)
        {
            erase(std::forward<TArgs>(args)...);
        }

        template <typename... TArgs> void add(const TArgs &... args)
        {
            vect.emplace_back(args...);
        }

        bool erase(const std::function<void(Args...)> &dlg)
        {
            for (auto it = vect.begin(); it != vect.end(); it++)
            {
                if (*it == dlg)
                {
                    vect.erase(it);
                    return true;
                }
            }
            return false;
        }

        void clear() { vect.clear(); }
    };
}

#endif
