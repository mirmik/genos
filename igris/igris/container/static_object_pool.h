#ifndef IGRIS_OBJECT_POOL_H
#define IGRIS_OBJECT_POOL_H

#include <igris/datastruct/pool.h>
#include <vector>
#include <array>

namespace igris {   

template <typename T, size_t Capacity>
class static_object_pool
{
public:
    struct storage_type
    {
        constexpr static size_t elsize() { return std::max(sizeof(T), sizeof(slist_head)); }
        constexpr static size_t elalign() { return std::max(alignof(T), alignof(slist_head)); }
        std::array<char, elsize()> data alignas(elalign());
    };
    static_assert(sizeof(storage_type) >= sizeof(T), "Invalid storage_type size");
    static_assert(sizeof(storage_type) >= sizeof(slist_head), "Invalid storage_type size");

public:
    struct pool_head head = POOL_HEAD_INIT(head);
    std::array<storage_type, Capacity> storage;

public:
    static_object_pool() : storage()
    {
        pool_init(&head);
        pool_engage(&head, storage.data(), Capacity * sizeof(storage_type), sizeof(storage_type));
    }

    static_object_pool(const static_object_pool&) = delete;
    static_object_pool& operator=(const static_object_pool&) = delete;
    static_object_pool(static_object_pool&&) = delete;
    static_object_pool& operator=(static_object_pool&&) = delete;

    pool_head* freelist() { return &head; }

    template <class... Args>
    T* create(Args&&... args)
    {
        void* ptr = pool_alloc(&head);
        if (ptr == nullptr) return nullptr;

        T* obj = new (ptr) T(std::forward<Args>(args)...);
        return obj;
    }

    void destroy(T* obj)
    {
        obj->~T();
        pool_free(&head, obj);
    }

    size_t avail() const { return pool_avail(&head); }
};

}

#endif