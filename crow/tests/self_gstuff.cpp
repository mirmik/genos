#include <chrono>
#include <crow/gates/self_driven_gstuff.h>
#include <crow/packet.h>
#include <doctest/doctest.h>
#include <igris/util/dstring.h>
#include <thread>

static volatile int ccc = 0;

static size_t write_total = 0;
std::vector<uint8_t> writed_data = {};

static int write(void *priv, const char *data, unsigned int size)
{
    write_total += size;
    writed_data.insert(writed_data.end(), data, data + size);
    ccc = 1;
    return size;
}

static int room(void *priv)
{
    return 100;
}

TEST_CASE("self_driven_gstuff.output")
{
    crow::total_travelled = 0;
    crow::default_incoming_handler = nullptr;
    {
        crow::self_driven_gstuff<crow::header_v1> gate;

        gate.init(write, room, NULL, 100);
        gate.bind(13);

        auto *pack = crow::create_packet(nullptr, 1, 10);

        pack->set_type(0 & 0x1F);
        pack->set_quality(0);
        pack->set_ackquant(0);

        memcpy(pack->addrptr(), "\x01", 1);
        memcpy(pack->dataptr(), "helloworld", 10);

        crow::header_v1 header = pack->extract_header_v1();
        iovec arr[] = {
            {&header, sizeof(crow::header_v1)},
            {pack->addrptr(), pack->addrsize()},
            {pack->dataptr(), pack->datasize()},
        };

        auto sbuffer = gstuffing_v(arr, sizeof(arr) / sizeof(iovec));

        CHECK_EQ(ccc, 0);
        gate.send(pack);
        CHECK_EQ(ccc, 1);
        CHECK_EQ(gate.in_send(), false);
        CHECK_GT(write_total, 10);

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        crow::release(pack);
        CHECK_EQ(writed_data.size(), sbuffer.size());
        {
            auto a = igris::dstring(
                std::string((char *)writed_data.data(), writed_data.size()));
            auto b = igris::dstring(
                std::string((char *)sbuffer.data(), sbuffer.size()));
            CHECK_EQ(a, b);
        }
    }

    CHECK_EQ(crow::total_travelled, 0);
    CHECK_EQ(crow::has_untravelled(), false);
    CHECK_EQ(crow::allocated_count(), 0);
}

/*TEST_CASE("self_driven_gstuff.input")
{
    crow::total_travelled = 0;
    crow::default_incoming_handler = nullptr;
    CHECK_EQ(crow::has_allocated(), false);

    {
        char buf[64];
        char buf2[64];

        crow::self_driven_gstuff gate;

        gate.init(buf, write, NULL, 100);
        gate.bind(13);

        auto * pack = crow_create_packet(nullptr, 1, 10);

        pack->set_type(9 & 0x1F);
        pack->set_quality(0);
        pack->set_ackquant(0);

        memcpy(pack->addrptr(), "\x01", 1);
        memcpy(pack->dataptr(), "helloworld", 10);

        int gsize = gstuffing((const char*)&pack->header(), pack->header().flen,
buf2);

        for (int i = 0; i < gsize; ++i)
        {
            gate.newdata(buf2[i]);
        }

        crow::onestep();
        crow::utilize(pack);
    }

    CHECK_EQ(crow::total_travelled, 1);
    CHECK_EQ(crow::has_untravelled(), false);
    CHECK_EQ(crow::allocated_count(), 0);
}*/