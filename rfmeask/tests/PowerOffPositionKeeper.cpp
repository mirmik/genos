#include <PowerOffPositionKeeper.h>
#include <doctest/doctest.h>

TEST_CASE("PowerOffPositionKeeper")
{
    std::filesystem::remove_all("/tmp/poskeeper");
    PowerOffPositionKeeper keeper("/tmp/poskeeper/pos", 360);

    keeper.reinit_with_start_position(100);
    CHECK_EQ(keeper.compensation_counter_value(), 0);

    CHECK_EQ(keeper.sectorno_of_position(0), 0);
    CHECK_EQ(keeper.sectorno_of_position(20), 0);
    CHECK_EQ(keeper.sectorno_of_position(100), 1);
    CHECK_EQ(keeper.sectorno_of_position(-30), -1);

    {
        keeper.process_position(50);
        keeper.process_position(80);
        keeper.process_position(110);
        keeper.process_position(160);
        keeper.process_position(200);
        keeper.process_position(250);
        keeper.process_position(300);
        keeper.process_position(350);
        double pos = keeper.process_position(370);
        CHECK_EQ(keeper.current_sector(), 6);
        CHECK_EQ(pos, 370);
    }

    {
        keeper.reinit_with_start_position(10);
        CHECK_EQ(keeper.compensation_counter_value(), 1);
        double pos = keeper.process_position(10);
        CHECK_EQ(pos, 370);
    }
}

TEST_CASE("PowerOffPositionKeeper . stored early")
{
    std::filesystem::remove_all("/tmp/poskeeper");
    {
        std::filesystem::create_directories("/tmp/poskeeper");
        std::fstream ofs("/tmp/poskeeper/pos",
                         std::ios::binary | std::ios::out);
        ofs << -17;
    }
    PowerOffPositionKeeper keeper("/tmp/poskeeper/pos", 360);

    keeper.reinit_with_start_position(51);
    CHECK_EQ(keeper.compensation_counter_value(), -3);
    auto pos = keeper.process_position(51);

    CHECK_EQ(pos, -1029);
}

TEST_CASE("PowerOffPositionKeeper . stability")
{
    std::filesystem::remove_all("/tmp/poskeeper");
    {
        PowerOffPositionKeeper keeper("/tmp/poskeeper/pos", 360);
        keeper.reinit_with_start_position(100);
        CHECK_EQ(keeper.process_position(100), 100);
    }
    {
        PowerOffPositionKeeper keeper("/tmp/poskeeper/pos", 360);
        keeper.reinit_with_start_position(100);
        CHECK_EQ(keeper.process_position(100), 100);
    }
    {
        PowerOffPositionKeeper keeper("/tmp/poskeeper/pos", 360);
        keeper.reinit_with_start_position(100);
        CHECK_EQ(keeper.process_position(100), 100);
    }
    {
        PowerOffPositionKeeper keeper("/tmp/poskeeper/pos", 360);
        keeper.reinit_with_start_position(100);
        CHECK_EQ(keeper.process_position(100), 100);
    }

    {
        PowerOffPositionKeeper keeper("/tmp/poskeeper/pos", 360);
        keeper.reinit_with_start_position(100);
        CHECK_EQ(keeper.process_position(100), 100);
    }
    {
        PowerOffPositionKeeper keeper("/tmp/poskeeper/pos", 360);
        keeper.reinit_with_start_position(100);
        CHECK_EQ(keeper.process_position(100), 100);
    }

    {
        PowerOffPositionKeeper keeper("/tmp/poskeeper/pos", 360);
        keeper.reinit_with_start_position(0); // for disable assertation
        keeper.update_stored_current_sector(10);
        keeper.reinit_with_start_position(-160);
        CHECK_EQ(keeper.process_position(-160), 720 - 160);
    }
}
