/**
    @file
*/
#ifndef CHARBUF_H
#define CHARBUF_H

#include <string.h>

/// Simple null terminated fixed size char buffer
template <unsigned int size> class charbuf
{
public:
    char data[size + 1];
    char &operator[](int i)
    {
        return data[i];
    };

    char *to_buf() const
    {
        return (char *)data;
    };

    char *c_str() const
    {
        return (char *)data;
    };

    int length() const
    {
        return size;
    };

    charbuf<size> &operator=(const charbuf<size> &src)
    {
        memcpy(data, src.data, size);
    };

    charbuf<size> &operator=(charbuf<size> &&src)
    {
        memcpy(data, src.data, size);
    };

    charbuf<size> &operator=(volatile const charbuf<size> &src)
    {
        memcpy(data, (const char *)src.data, size);
    };

    charbuf()
    {
        data[size] = 0;
    };

    charbuf(const char *str)
    {
        memcpy(data, str, size);
    };

    charbuf(const charbuf &cbuf)
    {
        memcpy(data, cbuf.to_buf(), size);
    };
};
#endif