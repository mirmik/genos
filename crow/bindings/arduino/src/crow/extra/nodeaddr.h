#ifndef CROW_NODEADDR_H
#define CROW_NODEADDR_H

#include <crow/address.h>
#include <crow/types.h>
#include <vector>

namespace crow
{
    class nodeaddr
    {
      public:
        std::vector<uint8_t> naddr;
        nid_t nid;

      public:
        crow::hostaddr_view hostaddr() const
        {
            return {naddr.data(), naddr.size()};
        }

        bool operator<(const nodeaddr &oth) const
        {
            if (naddr < oth.naddr)
                return true;
            if (nid < oth.nid)
                return true;
            return false;
        }
    };
} // namespace crow

#endif
