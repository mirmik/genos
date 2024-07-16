#ifndef NOS_BYTE_ARRAY_H
#define NOS_BYTE_ARRAY_H

#include <vector>
#include <string.h>
#include <stdint.h>

namespace nos 
{
    class bytearray 
    {
        std::vector<uint8_t> _data = {};

    public:
        bytearray() = default;
        bytearray(const bytearray& oth) = default;
        bytearray(bytearray&& oth) = default;
        bytearray& operator=(const bytearray& oth) = default;
        bytearray& operator=(bytearray&& oth) = default;

        auto begin() { return _data.begin(); }
        auto end() { return _data.end(); }
        auto begin() const { return _data.begin(); }
        auto end() const { return _data.end(); }

        void append(uint8_t* ptr, size_t sz) 
        {
            auto oldsize = _data.size();
            _data.resize(oldsize + sz);
            memcpy(_data.data() + oldsize, ptr, sz);
        }

        auto operator[] (int i) { return _data[i]; }

        auto size() const { return _data.size(); }
        auto data() const { return _data.data(); }
    };
}

#endif