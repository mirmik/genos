#ifndef NOS_IO_BYTEARRAY_ISTREAM_H
#define NOS_IO_BYTEARRAY_ISTREAM_H

#include <nos/binary/bytearray.h>
#include <stdexcept>

namespace nos
{
    class buffer;

    template <class Source> class bytearray_istream : public nos::istream
    {
        Source &arr;
        size_t read_counter = 0;

    public:
        bytearray_istream(Source &arr) : arr(arr) {}

        nos::expected<size_t, nos::input_error> read(void *data, size_t size)
        {
            size_t read_counter_new = read_counter + size;
            if (read_counter_new > arr.size())
            {
                throw std::length_error("deserialize");
            }
            memcpy(data, arr.data() + read_counter, size);
            read_counter = read_counter_new;
            return size;
        }

        nos::buffer remaining_buffer();
    };
}

#include <nos/util/buffer.h>

template <class Source>
nos::buffer nos::bytearray_istream<Source>::remaining_buffer()
{
    return {arr.data() + read_counter, arr.size() - read_counter};
}

#endif