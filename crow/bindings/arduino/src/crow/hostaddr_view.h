/** @file */

#ifndef CROW_HOSTADDR_VIEW_H
#define CROW_HOSTADDR_VIEW_H

#include <igris/buffer.h>

namespace std
{
    template <class T, class Alloc> class vector;
}

namespace crow
{
    class hostaddr;
    class hostaddr_view
    {
    public:
        const uint8_t *addr;
        size_t alen;

        hostaddr_view() = default;
        hostaddr_view(const hostaddr &h);
        hostaddr_view(const nos::buffer &v);
        hostaddr_view(const uint8_t *addr, size_t alen);
        hostaddr_view(const char *addr, size_t alen);
        hostaddr_view(const void *addr, size_t alen);

        template <class Alloc>
        hostaddr_view(const std::vector<uint8_t, Alloc> &v);

        const uint8_t *data() const;
        size_t size() const;

        const uint8_t *begin() const;
        const uint8_t *end() const;

        bool operator==(nos::buffer buf) const;
        bool operator==(const hostaddr_view &buf) const;
    };
}

template <class Alloc>
crow::hostaddr_view::hostaddr_view(const std::vector<uint8_t, Alloc> &v)
    : addr(v.data()), alen(v.size())
{
}

#endif