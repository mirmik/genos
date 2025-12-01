#ifndef IGRIS_MULTIPLE_DELEGATE_H
#define IGRIS_MULTIPLE_DELEGATE_H

/**
    @file multiple_delegate.h
    Делегат для последовательного вызова
    множественных обработчиков.
*/

#include <igris/event/delegate.h>
#include <vector>

namespace igris
{
    template <typename... Args> class multiple_delegate
    {
    public:
        std::vector<delegate<void, Args...>> vect = {};

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

        bool erase(const delegate<void, Args...> &dlg)
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
