#include <crow/packet.h>
#include <doctest/doctest.h>
#include <igris/protocols/gstuff.h>
#include <sstream>
#include <vector>

namespace
{
    template <class Header>
    std::string packet_to_gstuff_hex(crow::packet *pack,
                                     const gstuff_context &ctx)
    {
        Header header = crow::extract_header<Header>(pack);

        struct iovec iov[] = {
            {&header, sizeof(Header)},
            {pack->addrptr(), pack->addrsize()},
            {pack->dataptr(), pack->datasize()},
        };

        auto stuffed = gstuffing_v(iov, 3, ctx);

        std::ostringstream os;
        os.setf(std::ios::uppercase);
        os << std::hex;

        bool first = true;
        for (uint8_t byte : stuffed)
        {
            if (!first)
                os << ' ';
            first = false;
            if (byte < 0x10)
                os << '0';
            os << static_cast<int>(byte);
        }

        return os.str();
    }
}

TEST_CASE("packet serialization header_v1")
{
    auto *pack = crow::allocate_packet<crow::header_v1>(2, 2);

    pack->set_type(1);
    pack->set_quality(1);
    pack->set_ackquant(200);
    pack->set_seqid(0x1234);
    pack->set_stage(0);

    uint8_t addr[] = {0x01, 0x02};
    memcpy(pack->addrptr(), addr, sizeof(addr));

    const char data[] = {'h', 'i'};
    memcpy(pack->dataptr(), data, sizeof(data));

    std::string hex =
        packet_to_gstuff_hex<crow::header_v1>(pack, gstuff_context());

    // type occupies the high 3 bits of pflag, so type=1 results in 0x20.
    CHECK_EQ(hex, "A8 20 0C 00 02 00 34 12 51 01 02 68 69 C7 B2");

    crow::deallocate_packet(pack);
}

TEST_CASE("packet serialization header_v0 sample")
{
    const uint8_t addr[] = {0xF4, 0x0C, 0x7F, 0x00, 0x00, 0x01, 0x27, 0x19};
    const uint8_t data[] = {0x00,
                            0x0B,
                            0x00,
                            0xC8,
                            0x00,
                            'r',
                            'e',
                            'm',
                            'o',
                            't',
                            'e',
                            'r',
                            '_',
                            'c',
                            'm',
                            'd'};

    auto *pack =
        crow::allocate_packet<crow::header_v0>(sizeof(addr), sizeof(data));

    pack->set_type(4); // matches pflag 0x20 in the captured sample
    pack->set_quality(0);
    pack->set_ackquant(200);
    pack->set_seqid(0x1105);
    pack->set_stage(0);

    memcpy(pack->addrptr(), addr, sizeof(addr));
    memcpy(pack->dataptr(), data, sizeof(data));

    std::string hex =
        packet_to_gstuff_hex<crow::header_v0>(pack, gstuff_context_v0());

    // Captured frame from field device; flen=0x22 and CRC=0x3F.
    CHECK_EQ(hex,
             "AC 20 22 00 08 00 C8 00 05 11 00 F4 0C 7F 00 00 01 27 19 00 0B "
             "00 C8 00 72 65 6D 6F 74 65 72 5F 63 6D 64 3F AC");

    crow::deallocate_packet(pack);
}
