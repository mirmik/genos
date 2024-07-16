#ifndef NOS_TRENT_JSON_SERIALIZER_H
#define NOS_TRENT_JSON_SERIALIZER_H

#include <concepts>

namespace nos
{
    class trent_object_serializer
    {
        nos::trent &tr;

    public:
        trent_object_serializer(nos::trent &tr) : tr(tr) {}

        template <std::integral T> bool reflect(const char *name, T &value)
        {
            tr[name] = value;
            return true;
        }

        template <std::floating_point T>
        bool reflect(const char *name, T &value)
        {
            tr[name] = value;
            return true;
        }

        bool reflect(const char *name, const std::string &value)
        {
            tr[name] = value;
            return true;
        }

        bool reflect(std::string &value)
        {
            tr = value;
            return true;
        }

        template <class T> bool reflect(const T &ref)
        {
            return (const_cast<T &>(ref)).reflect(*this);
        }
    };

    class trent_object_deserializer
    {
        const nos::trent &tr;

    public:
        trent_object_deserializer(const nos::trent &tr) : tr(tr) {}

        template <std::integral T> bool reflect(const char *name, T &value)
        {
            value = static_cast<T>(tr[name].as_numer());
            return true;
        }

        template <std::floating_point T>
        bool reflect(const char *name, T &value)
        {
            value = static_cast<T>(tr[name].as_numer());
            return true;
        }

        bool reflect(const char *name, std::string &value)
        {
            value = tr[name].as_string();
            return true;
        }

        bool reflect(std::string &value)
        {
            value = tr.as_string();
            return true;
        }

        template <class T> bool reflect(T &ref)
        {
            return ref.reflect(*this);
        }
    };
}

#endif