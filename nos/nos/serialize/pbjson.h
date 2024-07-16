#ifndef PBJSON_SERIALIZE_H
#define PBJSON_SERIALIZE_H

#include <concepts>
#include <cstdint>
#include <nos/util/buffer.h>

namespace nos
{
    class pbjson_deserializer
    {
        nos::buffer buf;

        std::pair<int, int> get_type_and_length()
        {
            // type  located in the first 3 bits
            // length located in the last 5 bits
            uint8_t type_and_length = buf[0];
            uint8_t type = (type_and_length & 0b11100000) >> 4;
            uint8_t length = type_and_length & 0x1F; // todo length more than 15
            buf = {buf.data() + 1, buf.size() - 1};
            return {type, length};
        }

    public:
        pbjson_deserializer(nos::buffer buf) : buf(buf) {}

        template <std::integral T> bool reflect(const char *, T &value)
        {
            auto [type, length] = get_type_and_length();
            if (type != 2 && type != 4)
                return false;
            if (length == 0)
                return false;

            int64_t v = 0;
            for (int i = 0; i < length; i++)
            {
                v = (v << 8) | buf[i];
            }
            if (type == 4)
                v = -v;
            buf = {buf.data() + length, buf.size() - length};
            value = static_cast<T>(v);
            return true;
        }

        template <std::floating_point T> bool reflect(const char *, T &value)
        {
            auto [type, length] = get_type_and_length();
            if (type != 6)
                return false;
            if (length == 0)
                return false;

            value = 0; /// WHAT???
            for (int i = 0; i < length; i++)
            {
                value = (value << 8) | buf[i];
            }
            buf = {buf.data() + length, buf.size() - length};
            return true;
        }

        bool reflect(const char *, std::string &value)
        {
            auto [type, length] = get_type_and_length();
            if (type != 8)
                return false;
            if (length == 0)
                return false;

            value = std::string(buf.data(), length);
            buf = {buf.data() + length, buf.size() - length};
            return true;
        }
    };
}

#endif