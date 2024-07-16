#ifndef NOS_BINARY_OUTPUT_H
#define NOS_BINARY_OUTPUT_H

#include <nos/print.h>

namespace nos 
{
    /// \brief Send binary data to an ostream
    /// \param out The output ostream
    /// \param buf The data to send
    /// \param sz The size of the data
    /// \return The number of bytes sent
    int64_t binary_buffer_send(nos::ostream& os, const void* buf, uint32_t sz)
    {
        int64_t res = 0;
        res += nos::write_to(os, &sz, sizeof(sz));
        res += nos::write_to(os, buf, sz);
        return res;
    }

    /// \brief Receive binary data from an istream
    /// \param is The input istream
    /// \return The received data
    std::vector<uint8_t> binary_buffer_recv(nos::istream& is)
    {
        uint32_t sz = 0;
        nos::read_from(is, &sz, sizeof(sz));
        std::vector<uint8_t> res(sz);
        nos::read_from(is, &res[0], sz);
        return res;
    }
}

#endif