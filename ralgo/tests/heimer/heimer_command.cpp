#include <doctest/doctest.h>
#include <ralgo/heimer/axisctr.h>
#include <ralgo/heimer/axstate_linear_processor.h>
#include <ralgo/heimer/axstate_sincos_processor.h>
#include <ralgo/heimer/command.h>
#include <ralgo/heimer/phase_signals.h>

#include <string.h>
#include <string_view>
/*
using namespace heimer;

TEST_CASE("command")
{
    char buf[96];
    int sts;
    int ret;

    heimer_system_init();

    CHECK_EQ(heimer::signals_count(), 0);
    CHECK_EQ(heimer::signal_processors_count(), 0);

    heimer::command_exec_safe("signew axstate x", NULL, 0, &ret);

    CHECK_EQ(heimer::signals_count(), 1);
    CHECK_EQ(heimer::signal_processors_count(), 0);

    heimer::command_exec_safe("ctrnew axisctr xctr 1", NULL, 0, &ret);

    CHECK_EQ(heimer::signals_count(), 1);
    CHECK_EQ(heimer::signal_processors_count(), 1);

    sts = heimer::command_exec_safe("sig k info", buf, 96, &ret);
    CHECK_EQ(sts, 0);
    CHECK_EQ(ret, ENOENT);

    sts = heimer::command_exec_safe("sig x Info", buf, 96, &ret);
    CHECK_EQ(sts, 0);
    CHECK_EQ(ret, ENOENT);

    sts = heimer::command_exec_safe("sig x info", buf, 96, &ret);
    CHECK_EQ(sts, 0);
    CHECK_NE(strlen(buf), 0);

    signal_head * sig = signal_get_by_name("x");
    sig->info(
        buf,
        96);
    CHECK_EQ(sts, 0);
    CHECK_NE(strlen(buf), 0);
}

TEST_CASE("command")
{
    heimer_system_init();

    heimer::axis_settings set0[2];
    heimer::axis_settings set1[3];

    heimer::axis_controller axctr0("axisctr0", set0, 2);
    heimer::axstate_linear_processor linear("linear", 3);
    heimer::axis_controller axctr1("axisctr0", set1, 3);
    heimer::axstate_sincos_processor sincos("sincos");

    CHECK_EQ(axctr0.name(), "axisctr0");
    CHECK_EQ(linear.name(), "linear");
    CHECK_EQ(axctr1.name(), "axisctr0");
    CHECK_EQ(sincos.name(), "sincos");

    CHECK_EQ(heimer::signal_processors_count(), 4);
}
*/