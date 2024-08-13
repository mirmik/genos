#ifndef CROW_NODEADDR_H
#define CROW_NODEADDR_H

#include <crow/address.h>
#include <crow/proto/node.h>
#include <vector>

namespace crow
{
    class nodeaddr
    {
    public:
        crow::hostaddr addr;
        nodeid_t nid;

    public:
        crow::hostaddr_view hostaddr() const
        {
            return {addr.data(), addr.size()};
        }

        bool operator<(const nodeaddr &oth) const
        {
            if (addr < oth.addr)
                return true;
            if (nid < oth.nid)
                return true;
            return false;
        }
    };
} // namespace crow

#endif
