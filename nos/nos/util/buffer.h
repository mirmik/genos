#ifndef NOS_UTIL_BUFFER_H
#define NOS_UTIL_BUFFER_H

#include <cstdint>
#include <cstring>
#include <nos/binary/bytearray.h>
#include <nos/util/numconvert.h>
#include <string>
#include <string_view>

namespace nos
{
    class buffer
    {
    private:
        char *_data = nullptr;
        size_t _size = 0;

    public:
        buffer() = default;
        buffer(const buffer &oth) = default;
        buffer(buffer &&oth) = default;
        buffer &operator=(const buffer &oth) = default;
        buffer &operator=(buffer &&oth) = default;

        buffer(char *data, size_t size) : _data(data), _size(size) {}

        buffer(const char *data, size_t size) : _data((char *)data), _size(size)
        {
        }

        buffer(const void *data, size_t size) : _data((char *)data), _size(size)
        {
        }

        buffer(void *data, size_t size) : _data((char *)data), _size(size) {}

        buffer(const uint8_t *data, size_t size)
            : _data((char *)data), _size(size)
        {
        }

        buffer(uint8_t *data, size_t size) : _data((char *)data), _size(size) {}

        buffer(const char *data) : _data((char *)data), _size(strlen(data)) {}

        std::string substr(size_t pos, size_t count = std::string::npos) const
        {
            if (count == std::string::npos)
                count = _size - pos;
            return std::string(_data + pos, count);
        }

        const char *data() const
        {
            return _data;
        }
        char *data()
        {
            return _data;
        }

        size_t size() const
        {
            return _size;
        }

        buffer(const std::string &str) : buffer(str.data(), str.size()) {}
        buffer(const nos::bytearray &str)
            : buffer((char *)str.data(), str.size())
        {
        }

        buffer(const std::string_view &str) : buffer(str.data(), str.size()) {}

        bool operator==(const buffer &other) const
        {
            return (_size == other._size) &&
                   (strncmp(_data,
                            other._data,
                            _size < other._size ? _size : other._size) == 0);
        }

        bool operator!=(const buffer &other) const
        {
            return (_size != other._size) ||
                   (strncmp(_data,
                            other._data,
                            _size < other._size ? _size : other._size) != 0);
        }

        bool operator==(const char *str) const
        {
            return strncmp(_data, str, _size) == 0;
        }

        bool operator!=(const char *str) const
        {
            return strncmp(_data, str, _size) != 0;
        }

        char &operator[](size_t num)
        {
            return *(_data + num);
        }

        char operator[](size_t num) const
        {
            return *(_data + num);
        }

        char *operator*()
        {
            return _data;
        }

        size_t operator+()
        {
            return _size;
        }

        char *begin()
        {
            return _data;
        }

        char *end()
        {
            return _data + _size;
        }

        const char *begin() const
        {
            return _data;
        }

        const char *end() const
        {
            return _data + _size;
        }

        bool empty()
        {
            return _data == nullptr;
        }

        std::string to_string() const
        {
            return std::string(_data, _size);
        }

        int to_int() const
        {
            return std::stoi(std::string(_data, _size));
        }

        double to_double() const
        {
            return std::stod(std::string(_data, _size));
        }

        operator std::string_view() const
        {
            return std::string_view(_data, _size);
        }

        operator std::string() const
        {
            return std::string(_data, _size);
        }
    };
}

#endif
