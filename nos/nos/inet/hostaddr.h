#ifndef NOS_HOSTADDR_H
#define NOS_HOSTADDR_H

#include <nos/fprint.h>
#include <nos/print.h>

namespace nos
{
    class hostaddr
    {
    public:
        uint32_t addr = 0;
        hostaddr() : addr(0) {}
        hostaddr(int32_t addr_) : addr((uint32_t)addr_) {}
        hostaddr(uint32_t addr_) : addr(addr_) {}

        hostaddr(const char *ip_addr_str)
        {
            unsigned int n1, n2, n3, n4;
            sscanf(ip_addr_str, "%u.%u.%u.%u", &n1, &n2, &n3, &n4);

            addr = n1 << 24 | n2 << 16 | n3 << 8 | n4;
        }

        hostaddr(const std::string &str) : hostaddr(str.c_str()) {}

        nos::expected<size_t, nos::output_error>
        print_to(nos::ostream &os) const
        {
            return *nos::printhex_to(os, addr);
        }

        bool operator==(const hostaddr &oth) const
        {
            return oth.addr == addr;
        }

        bool operator<(const hostaddr &oth) const
        {
            if (addr < oth.addr)
                return true;
            return false;
        }

        static hostaddr any()
        {
            return hostaddr(0);
        }

        static hostaddr loopback()
        {
            return hostaddr(0x7f000001);
        }

        std::string to_string() const
        {
            return nos::format("{}.{}.{}.{}",
                               (addr >> 24) & 0xff,
                               (addr >> 16) & 0xff,
                               (addr >> 8) & 0xff,
                               (addr)&0xff);
        }
    };

    namespace inet
    {
        static constexpr const char *localhost = "127.0.0.1";
        using hostaddr = nos::hostaddr;

        struct netaddr
        {
            hostaddr addr;
            int32_t port;
            netaddr(uint32_t addr_, uint16_t port_) : addr(addr_), port(port_)
            {
            }

            netaddr(nos::inet::hostaddr addr_, uint16_t port_)
                : addr(addr_), port(port_)
            {
            }

            netaddr() = default;

            int print_to(nos::ostream &o) const
            {
                return *nos::fprint_to(o, "(h:{},p:{})", addr, port);
            }

            bool operator==(const netaddr &oth) const
            {
                return oth.addr == addr && oth.port == port;
            }

            bool operator<(const netaddr &oth) const
            {
                if (addr < oth.addr)
                    return true;
                if (port < oth.port)
                    return true;
                return false;
            }
        };
    }
}

namespace std
{
    template <> class hash<nos::inet::hostaddr>
    {
    public:
        size_t operator()(const nos::inet::hostaddr &s) const
        {
            return std::hash<int32_t>()(s.addr);
        }
    };

    template <> class hash<nos::inet::netaddr>
    {
    public:
        size_t operator()(const nos::inet::netaddr &s) const
        {
            size_t h1 = std::hash<nos::inet::hostaddr>()(s.addr);
            size_t h2 = std::hash<int32_t>()(s.port);
            return h1 ^ (h2 << 1);
        }
    };
}

#endif
