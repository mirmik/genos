#include <doctest/doctest.h>
#include <ralgo/heimer/axis_stub_processor.h>
#include <ralgo/heimer/phase_signals.h>

using namespace heimer;

TEST_CASE("axis_stub_processor")
{
    axis_state axstate("a");
    axis_stub_processor axstub("axstub");

    axstate.ctrpos = 100;

    axstub.bind(&axstate);
    axstub.activate(0);

    axstub.serve(0);
    axstub.feedback(0);

    // CHECK_EQ(axstate.feedpos, 100);
}
