#include <System.h>
#include <igris/time/stopwatch.h>
#include <nos/check.h>
#include <selftest.h>
#include <status.h>
#include <tables.h>

std::thread selftest_thread;

void system_ready_wait()
{
    while (System::instance()->status() != DeviceStatus::Ready)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void axis_operation_wait(AbstractAxis &axis)
{
    while (axis.operation_status() != Operation::NONE)
    {
        nos::println("Selftest 0: axis0 position: ", axis.last_unit_position());
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

std::string simulator_test_profile(int selftest_no)
{
    switch (selftest_no)
    {
    case 0:
        return "opu";
    case 1:
        return "planar";
    }
    return "opu";
}

int selftest_opu()
{
    igris::stopwatch sw;

    // Тест симулятора опорноповоротного устройства.
    system_ready_wait();

    // Проверка наличия устройства.
    if (axes_total() != 3)
    {
        nos::println("Selftest 0: axes_total() != 3. current value: ",
                     axes_total());
        exit(-1);
    }

    AbstractAxis &axis0 = *get_axis(0);

    NOS_CHECK_EQ(axis0.getSpeed_rpm(), 202);
    NOS_CHECK_EQ(axis0.unitRatio(), 4194304);
    NOS_CHECK_GT(axis0.getSpeed_ups(), 3);

    axis0.absoluteUnitMove(5);

    nos::println("Selftest 0: axis0 absoluteUnitMove(20) start");
    sw.start();
    axis_operation_wait(axis0);
    sw.stop();
    nos::println("Selftest 0: axis0 absoluteUnitMove(20) end. elapsed: ",
                 sw.elapsed_ms(),
                 " ms");

    NOS_CHECK(sw.elapsed_ms() > 3000 && sw.elapsed_ms() < 3700);

    nos::println("Selftest 0: passed");
    return 0;
}

int selftest_planar()
{
    return selftest_opu();
}

using selftest_handler = int (*)();
selftest_handler selftest_table[]{
    selftest_opu,
    selftest_planar,
};

void simulator_selftest_start(int selftest_no)
{
    selftest_thread = std::thread([selftest_no] {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        NOS_CHECK_ENV_START()
        {
            int status = selftest_table[selftest_no]();
            quick_exit(status);
        }
        NOS_CHECK_ENV_ERROR_HANDLER()
        {
            nos::println(
                "Selftest ", selftest_no, " failed: ", nos_check_error_msg);
            quick_exit(-1);
        }
    });
}