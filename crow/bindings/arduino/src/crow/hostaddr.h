/** @file */

#ifndef CROW_HOSTADDR_H
#define CROW_HOSTADDR_H

#include <cstring>
#include <igris/buffer.h>
#include <vector>

namespace crow
{
    class hostaddr_view;

    class hostaddr
    {
        std::vector<uint8_t> _addr;

      public:
        hostaddr() = default;

        const uint8_t *data() const { return _addr.data(); }
        size_t size() const { return _addr.size(); }

        hostaddr(const std::vector<uint8_t> &addr) : _addr(addr) {}

        hostaddr(const hostaddr_view &addr);

        hostaddr(std::vector<uint8_t> &&addr) : _addr(std::move(addr)) {}

        hostaddr(uint8_t *addr, size_t len) : _addr(addr, addr + len) {}

        hostaddr &operator=(std::vector<uint8_t> &&addr)
        {
            _addr = std::move(addr);
            return *this;
        }

        hostaddr &operator=(const std::vector<uint8_t> &addr)
        {
            _addr = addr;
            return *this;
        }

        hostaddr_view view();

        operator hostaddr_view();
    };
}

#endif
