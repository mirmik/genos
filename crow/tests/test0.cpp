#include <crow/address.h>
#include <crow/tower.h>
#include <doctest/doctest.h>
#include <nos/print.h>

#include <chrono>
#include <thread>

using namespace std::chrono_literals;

static auto waddr = crow::address(".12.127.0.0.1:10098");
static auto addr = crow::address(".99");
static auto addr2 = crow::address(".99.99");
static int count = 0;

void incoming(crow::packet *ptr)
{
    count++;
    crow::release(ptr);
}

TEST_CASE("ptr")
{
    {
        auto ptr = crow::create_packet(nullptr, 0, 0);
        crow::packet_ptr pptr(ptr);
        crow::tower_release(ptr);
    }
    CHECK_EQ(crow::allocated_count(), 0);
}

TEST_CASE("get_stage")
{

    std::string data = "data";
    crow::packet *pack = crow::create_packet(NULL, addr.size(), data.size());
    pack->set_type(5);
    pack->set_quality(0);
    pack->set_ackquant(0);
    memcpy(pack->addrptr(), addr.data(), addr.size());
    memcpy(pack->dataptr(), data.data(), data.size());
    CHECK_NE(pack, nullptr);
    CHECK_EQ(pack->stage(), 0);
    CHECK_EQ(*pack->stageptr(), 99);
    pack->invalidate();
    CHECK_EQ(crow::allocated_count(), 0);
}

TEST_CASE("get_stage_2")
{

    std::string data = "data";
    crow::packet *pack = crow::create_packet(NULL, waddr.size(), data.size());
    pack->set_type(5);
    pack->set_quality(0);
    pack->set_ackquant(0);
    memcpy(pack->addrptr(), waddr.data(), waddr.size());
    memcpy(pack->dataptr(), data.data(), data.size());
    CHECK_NE(pack, nullptr);
    CHECK_EQ(pack->stage(), 0);
    CHECK_EQ(*pack->stageptr(), 12);
    pack->invalidate();
    CHECK_EQ(crow::allocated_count(), 0);
}

TEST_CASE("test0" * doctest::timeout(0.5))
{
    CHECK_EQ(crow::allocated_count(), 0);
    count = 0;
    crow::total_travelled = 0;
    crow::default_incoming_handler = incoming;

    SUBCASE("0")
    {
        crow::send(addr, "data", 0, 0, 20);
        crow::onestep();

        CHECK_EQ(count, 1);
        CHECK_EQ(crow::total_travelled, 2);
        CHECK_EQ(crow::has_untravelled(), false);
        CHECK_EQ(crow::allocated_count(), 0);
    }

    SUBCASE("2_safe")
    {
        auto packptr = crow::send(addr, "data", 0, 0, 20);
        CHECK_EQ(packptr->refs, 1);

        crow::onestep();
        std::this_thread::sleep_for(10ms);
        CHECK_EQ(packptr->refs, 1);

        CHECK_EQ(count, 1);
        CHECK_EQ(crow::has_untravelled(), false);
        CHECK_EQ(crow::allocated_count(), 1);
    }

    SUBCASE("3_safe")
    {
        auto packptr = crow::send(addr, "data0", 0, 0, 20);
        packptr = crow::send(addr, "data1", 0, 0, 20);
        packptr = crow::send(addr, "data2", 0, 0, 20);
        CHECK_EQ(packptr->refs, 1);

        crow::onestep();
        std::this_thread::sleep_for(10ms);
        CHECK_EQ(packptr->refs, 1);

        CHECK_EQ(count, 3);
        CHECK_EQ(crow::has_untravelled(), false);
        CHECK_EQ(crow::allocated_count(), 1);
    }

    SUBCASE("addr2")
    {
        crow::send(addr2, "data", 0, 0, 20);

        crow::onestep();
        std::this_thread::sleep_for(10ms);

        CHECK_EQ(count, 1);
        CHECK_EQ(crow::has_untravelled(), false);
        CHECK_EQ(crow::allocated_count(), 0);
    }

    SUBCASE("qos1")
    {
        crow::send(addr, "data", 0, 1, 20);

        crow::onestep();

        CHECK_EQ(count, 1);
        CHECK_EQ(crow::total_travelled, 4); // pack * 2 + ack * 2
        CHECK_EQ(crow::has_untravelled(), false);
        CHECK_EQ(crow::allocated_count(), 0);
    }

    SUBCASE("qos2")
    {
        crow::send(addr, "data", 0, 2, 20);

        crow::onestep();
        std::this_thread::sleep_for(10ms);

        CHECK_EQ(count, 1);
        CHECK_EQ(crow::total_travelled, 6); // pack * 2 + ack * 2 + ack2 * 2
        CHECK_EQ(crow::has_untravelled(), false);
        CHECK_EQ(crow::allocated_count(), 0);
    }

    SUBCASE("undelivered_0")
    {
        crow::send(waddr, "data", 0, 0, 2);

        crow::onestep();

        CHECK_EQ(count, 0);
        CHECK_EQ(crow::total_travelled, 1);
        CHECK_EQ(crow::has_untravelled(), false);
        CHECK_EQ(crow::allocated_count(), 0);
    }

    SUBCASE("undelivered_1")
    {
        crow::send(waddr, "data", 0, 1, 2);

        CHECK_EQ(crow::incomming_stage_count(), 0);
        CHECK_EQ(crow::outers_stage_count(), 1);
        crow::onestep();

        CHECK_EQ(crow::incomming_stage_count(), 0);

        std::this_thread::sleep_for(std::chrono::milliseconds(7));
        crow::onestep();
        std::this_thread::sleep_for(std::chrono::milliseconds(7));
        crow::onestep();
        std::this_thread::sleep_for(std::chrono::milliseconds(7));
        crow::onestep();
        std::this_thread::sleep_for(std::chrono::milliseconds(7));
        crow::onestep();
        std::this_thread::sleep_for(std::chrono::milliseconds(7));
        crow::onestep();
        std::this_thread::sleep_for(std::chrono::milliseconds(7));
        crow::onestep();
        std::this_thread::sleep_for(std::chrono::milliseconds(7));
        crow::onestep();
        std::this_thread::sleep_for(std::chrono::milliseconds(7));
        crow::onestep();
        std::this_thread::sleep_for(std::chrono::milliseconds(7));
        crow::onestep();
        std::this_thread::sleep_for(std::chrono::milliseconds(7));
        crow::onestep();
        std::this_thread::sleep_for(std::chrono::milliseconds(7));
        crow::onestep();
        std::this_thread::sleep_for(std::chrono::milliseconds(7));
        crow::onestep();
        std::this_thread::sleep_for(std::chrono::milliseconds(7));
        crow::onestep();

        CHECK_EQ(crow::outers_stage_count(), 0);
        CHECK_EQ(count, 0);
        CHECK_EQ(crow::total_travelled, 5);
        CHECK_EQ(crow::has_untravelled(), false);
        CHECK_EQ(crow::allocated_count(), 0);
    }

    SUBCASE("undelivered_2")
    {
        crow::send(waddr, "data", 0, 2, 2);

        std::this_thread::sleep_for(std::chrono::milliseconds(7));
        crow::onestep();
        std::this_thread::sleep_for(std::chrono::milliseconds(7));
        crow::onestep();
        std::this_thread::sleep_for(std::chrono::milliseconds(7));
        crow::onestep();
        std::this_thread::sleep_for(std::chrono::milliseconds(7));
        crow::onestep();
        std::this_thread::sleep_for(std::chrono::milliseconds(7));
        crow::onestep();
        std::this_thread::sleep_for(std::chrono::milliseconds(7));
        crow::onestep();
        std::this_thread::sleep_for(std::chrono::milliseconds(7));
        crow::onestep();
        std::this_thread::sleep_for(std::chrono::milliseconds(7));
        crow::onestep();
        std::this_thread::sleep_for(std::chrono::milliseconds(7));
        crow::onestep();

        CHECK_EQ(count, 0);
        CHECK_EQ(crow::total_travelled, 5);
        CHECK_EQ(crow::has_untravelled(), false);
        CHECK_EQ(crow::allocated_count(), 0);
    }
}