/**
    @file
*/

#ifndef PROTECTED_SET_H
#define PROTECTED_SET_H

#include <igris/event/delegate.h>
#include <mutex>
#include <set>

template <typename T> class protected_set : public std::set<T>
{
    std::mutex mtx = {};

public:
    void insert(const T &val)
    {
        std::lock_guard<std::mutex> lock(mtx);
        std::set<T>::insert(val);
    }

    void erase(const T &val)
    {
        std::lock_guard<std::mutex> lock(mtx);
        std::set<T>::erase(val);
    }

    void foreach (igris::delegate<void, T &> func)
    {
        std::lock_guard<std::mutex> lock(mtx);
        for (auto e : *this)
        {
            func(e);
        }
    }
};

#endif
