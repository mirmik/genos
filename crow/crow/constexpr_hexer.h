#ifndef CROW_CONSTEXPR_HEXER_H
#define CROW_CONSTEXPR_HEXER_H

#include <array>
#include <crow/hexer.h>
#include <ctype.h>
#include <igris/util/hexascii.h>

namespace crow
{
    template <size_t N>
    constexpr std::pair<std::array<char, N>, int>
    constexpr_hexer(const std::array<char, N> &arr)
    {
        std::array<char, N> ret = {};
        int sz = 0;
        auto it = arr.begin();
        auto dst = ret.begin();
        while (it != arr.end())
        {
            if (*it == '\r')
                return {ret, 0};
            else if (*it == '\n')
                return {ret, 0};
            else if (*it == '0')
                return {ret, 0};

            else if (*it == '.')
            {
                uint8_t byte = 0;
                int cnt = 0;
                while (isdigit(*++it))
                {
                    byte *= 10;
                    byte += hex2half(*it);
                    cnt++;
                }
                ++sz;
                *dst++ = byte;

                if (cnt > 3)
                    return {ret, CROW_HEXER_MORE3_DOT};
            }
            else if (*it == ':')
            {
                uint16_t twobyte = 0;
                while (isdigit(*++it))
                {
                    twobyte *= 10;
                    twobyte += hex2half(*it);
                }
                *dst++ = (twobyte & 0xFF00) >> 8;
                *dst++ = twobyte & 0x00FF;
                sz += 2;
            }
            else if (*it == '_')
                ++it;

            else
                return {ret, CROW_HEXER_UNDEFINED_SYMBOL};
        }

        return {ret, sz};
    }
}

#endif