#ifndef IGRIS_SERIALIZE_SERIALIZE_STORAGE_H_
#define IGRIS_SERIALIZE_SERIALIZE_STORAGE_H_

#include <igris/compiler.h>

#if __HAS_CONCEPT
#include <concepts>
#endif

#include <cstdint>
#include <cstdlib>
#include <string>

#include <igris/buffer.h>
#include <igris/math.h>

namespace igris
{
#if __HAS_CONCEPT
    template <typename T>
    concept appendable_storage_type = requires(T a, const char *data,
                                               size_t size)
    {
        a.append(data, size);
        a.size();
    };

    static_assert(appendable_storage_type<std::string>);
    static_assert(!appendable_storage_type<const char *>);
#endif

    template <class Self> class serialize_storage_base
    {
    private:
        Self &self() { return static_cast<Self &>(*this); }

    public:
        void dumps(const std::string &str)
        {
            self().dump(str.data(), str.size());
        }
    };

#if __HAS_CONCEPT
    template <appendable_storage_type T>
#else
    template <class T>
#endif
    class appendable_storage
        : public serialize_storage_base<appendable_storage<T>>
    {
        T _storage;

    public:
        appendable_storage() = default;
        appendable_storage(const T &storage) : _storage(storage) {}

        void dump(const char *data, size_t size)
        {
            _storage.append(data, size);
        }

        const T &storage() const { return _storage; }
    };

    template <class Self> class deserialize_storage
    {
        Self &self() { return static_cast<Self &>(*this); }

    public:
        std::string loads(size_t size)
        {
            std::string ret;
            ret.resize(size);
            self().load(&*ret.begin(), size);
            return ret;
        }
    };

    class deserialize_buffer_storage
        : public deserialize_storage<deserialize_buffer_storage>
    {
        igris::buffer _storage;
        size_t cursor = 0;

    public:
        deserialize_buffer_storage(igris::buffer buf) : _storage(buf) {}
        // deserialize_buffer_storage(const std::string& buf) :
        // _storage(igris::buffer(buf)) {}

        void load(char *data, size_t size)
        {
            auto len = MIN(size, _storage.size() - cursor);
            memcpy(data, _storage.data() + cursor, len);
            cursor += len;
        }

        int avail() { return _storage.size() - cursor; }
    };

    using string_storage = appendable_storage<std::string>;

#if __HAS_CONCEPT
    template <typename T>
    concept load_storage_type = requires(T a, char *data, size_t size)
    {
        a.load(data, size);
    };
#endif

}

#endif