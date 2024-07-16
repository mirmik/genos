/**
    Однопроходной парсинг последовательных данных.
*/

#ifndef IGRIS_BINREADER_H
#define IGRIS_BINREADER_H

#include <igris/compiler.h>
#include <cstddef>
#include <cstring>

__BEGIN_DECLS

void reader_memcpy(const char **ptr, void *obj, size_t objsize);
void reader_skip(const char **ptr, size_t size);
void reader_skip_while(const char **ptr, char c, const char *protector);

__END_DECLS

#ifdef __cplusplus
#include <igris/buffer.h>
#include <igris/util/numconvert.h>

namespace igris
{
    class binreader
    {
        const char *ptr=nullptr;

    public:
        binreader() {}
        binreader(const char *ptr) : ptr(ptr) {}
        binreader(const binreader& reader) =default;
        binreader& operator=(const binreader& reader) =default;

        void init(const char *ptr) { this->ptr = ptr; }

        template <class T> void read_binary(T &obj)
        {
            memcpy(&obj, ptr, sizeof(T));
            ptr += sizeof(T);
        }

        template <class Pointer> void bind_buffer(Pointer *&data, size_t sz)
        {
            data = (Pointer *)ptr;
            ptr += sz;
        }

        void bind_buffer(igris::buffer &data, size_t sz)
        {
            data = igris::buffer{ptr, sz};
            ptr += sz;
        }

        void skip(int n) { ptr += n; }

        int read_ascii_decimal_integer(int *ret)
        {
            *ret = strtol(ptr, (char **)&ptr, 10);
            return 0;
        }

        int read_ascii_decimal_float(float *ret)
        {
            *ret = igris_atof32(ptr, (char **)&ptr);
            return 0;
        }
    };
}
#endif

#endif
