#include <doctest/doctest.h>
#include <ralgo/heimer/axis_stub_processor.h>
#include <ralgo/heimer/axisctr.h>
#include <ralgo/heimer/axstate_linear_processor.h>
#include <ralgo/heimer/command.h>
#include <ralgo/heimer/executor.h>
#include <ralgo/heimer/phase_signals.h>

/*
using namespace heimer;

std::string str(nos::buffer buf)
{
    return std::string(buf.data(), buf.size());
}

TEST_CASE("executor")
{
    heimer_reinit();

    axis_state state00("a");
    axis_state state01("b");
    axis_state state10("c");
    axis_state state11("d");

    axis_stub_processor xstub("xstub");
    axis_stub_processor ystub("ystub");
    xstub.bind(&state00);
    ystub.bind(&state01);

    axis_state * axctr0_states[] = { &state10, &state11 };
    axis_settings axctr0_settings[2];
    axis_controller axctr0("axctr0", axctr0_settings, 2);
    axctr0.set_controlled(axctr0_states);

    axis_state * linproc_left[] = {&state00, &state01};
    axis_state * linproc_right[] = {&state10, &state11};
    double linproc_matrix[4] = { 1, 0, 0, 1 };
    double linproc_inverse_matrix[4];
    axstate_linear_processor linproc("linproc", 2, linproc_left, linproc_right,
        linproc_matrix, linproc_inverse_matrix);
    linproc.attach_axes_from_tables();

    signal_processor * executor_table[10];

    executor.set_order_table(executor_table, 10, 0);
    executor.append_processor(&xstub);
    executor.append_processor(&ystub);
    executor.append_processor(&axctr0);
    executor.append_processor(&linproc);

    CHECK_EQ(executor.order_table_size, 4);

    CHECK_EQ(dlist_size(&signals_list), 4);

    executor.order_sort();

    CHECK_EQ(str(executor_table[0]->name()), str(xstub.name()));
    CHECK_EQ(str(executor_table[1]->name()), str(ystub.name()));
    CHECK_EQ(str(executor_table[2]->name()), str(linproc.name()));
    CHECK_EQ(str(executor_table[3]->name()), str(axctr0.name()));
}


TEST_CASE("executor: tandem sort")
{
    heimer_reinit();

    axis_state x("x");
    axis_state y("y");
    axis_state vx("vx");
    axis_state vy("vy");

    axis_stub_processor xstub("xstub");
    axis_stub_processor ystub("ystub");
    xstub.bind(&x);
    ystub.bind(&y);

    axis_settings xctr_sett[1];
    axis_settings yctr_sett[1];
    axis_settings vxctr_sett[1];
    axis_settings vyctr_sett[1];

    axis_controller xctr("xctr", xctr_sett, 1);
    axis_controller yctr("yctr", yctr_sett, 1);
    axis_controller vxctr("vxctr", vxctr_sett, 1);
    axis_controller vyctr("vyctr", vyctr_sett, 1);

    axis_state * axctr_states[] = { &x, &y, &vx, &vy };

    xctr.set_controlled(&axctr_states[0]);
    yctr.set_controlled(&axctr_states[1]);
    vxctr.set_controlled(&axctr_states[2]);
    vyctr.set_controlled(&axctr_states[3]);

    axis_state * linproc_left[] =  { &x,  &y};
    axis_state * linproc_right[] = {&vx, &vy};
    double linproc_matrix[4] = { 1, 0, 0, 1 };
    double linproc_inverse_matrix[4];
    axstate_linear_processor linproc("linproc", 2,
        linproc_left,
        linproc_right,
        linproc_matrix,
        linproc_inverse_matrix);
    linproc.attach_axes_from_tables();

    signal_processor * executor_table[10];

    executor.set_order_table(executor_table, 10, 0);
    executor.append_processor(&xctr);
    executor.append_processor(&yctr);
    executor.append_processor(&vxctr);
    executor.append_processor(&vyctr);

    executor.append_processor(&xstub);
    executor.append_processor(&ystub);

    executor.append_processor(&linproc);

    CHECK_EQ(executor.order_table_size, 7);
    CHECK_EQ(dlist_size(&signals_list), 4);

    executor.order_sort();

    CHECK_EQ(str(executor_table[0]->name()), str(xstub.name()));
    CHECK_EQ(str(executor_table[1]->name()), str(xctr.name()));
    CHECK_EQ(str(executor_table[2]->name()), str(ystub.name()));
    CHECK_EQ(str(executor_table[3]->name()), str(yctr.name()));
    CHECK_EQ(str(executor_table[4]->name()), str(linproc.name()));
    CHECK_EQ(str(executor_table[5]->name()), str(vxctr.name()));
    CHECK_EQ(str(executor_table[6]->name()), str(vyctr.name()));
}


TEST_CASE("executor: tandem activate")
{
    heimer_reinit();

    axis_state x("x");
    axis_state y("y");
    axis_state vx("vx");
    axis_state vy("vy");

    axis_stub_processor xstub("xstub");
    axis_stub_processor ystub("ystub");
    xstub.bind(&x);
    ystub.bind(&y);

    axis_settings xctr_sett[1];
    axis_settings yctr_sett[1];
    axis_settings vxctr_sett[1];
    axis_settings vyctr_sett[1];

    axis_controller xctr("xctr", xctr_sett, 1);
    axis_controller yctr("yctr", yctr_sett, 1);
    axis_controller vxctr("vxctr", vxctr_sett, 1);
    axis_controller vyctr("vyctr", vyctr_sett, 1);

    axis_state * axctr_states[] = { &x, &y, &vx, &vy };

    xctr.set_controlled(&axctr_states[0]);
    yctr.set_controlled(&axctr_states[1]);
    vxctr.set_controlled(&axctr_states[2]);
    vyctr.set_controlled(&axctr_states[3]);

    axis_state * linproc_left[] =  { &x,  &y};
    axis_state * linproc_right[] = {&vx, &vy};
    double linproc_matrix[4] = { 1, 0, 0, 1 };
    double linproc_inverse_matrix[4];
    axstate_linear_processor linproc("linproc", 2, linproc_left, linproc_right,
        linproc_matrix, linproc_inverse_matrix);
    linproc.attach_axes_from_tables();

    signal_processor * executor_table[10];

    executor.set_order_table(executor_table, 10, 0);
    executor.append_processor(&xctr);
    executor.append_processor(&yctr);
    executor.append_processor(&vxctr);
    executor.append_processor(&vyctr);

    executor.append_processor(&xstub);
    executor.append_processor(&ystub);

    executor.append_processor(&linproc);

    CHECK_EQ(executor.order_table_size, 7);
    CHECK_EQ(dlist_size(&signals_list), 4);

    executor.order_sort();

    SUBCASE("vxctr move")
    {
        double tgt = 1;
        vxctr.incmove(0, &tgt);
        xctr.incmove(0, &tgt);
        CHECK_EQ(vxctr.is_active(), true);
        CHECK_EQ(vyctr.is_active(), false);
        CHECK_EQ(xctr.is_active(), false);
        CHECK_EQ(yctr.is_active(), false);
        CHECK_EQ(linproc.is_active(), true);
        CHECK_EQ(vx.listener, &linproc);
        CHECK_EQ(vx.current_controller, &vxctr);
        CHECK_EQ(vy.listener, &linproc);
        CHECK_EQ(vy.current_controller, nullptr);
        CHECK_EQ(xstub.is_active(), true);
        CHECK_EQ(ystub.is_active(), true);
        CHECK_EQ(x.listener, &xstub);
        CHECK_EQ(x.current_controller, &linproc);
        CHECK_EQ(y.listener, &ystub);
        CHECK_EQ(y.current_controller, &linproc);

        vxctr.serve(1000000000);
        CHECK_EQ(vxctr.is_active(), false);
        CHECK_EQ(vyctr.is_active(), false);
        CHECK_EQ(xctr.is_active(), false);
        CHECK_EQ(yctr.is_active(), false);
        CHECK_EQ(linproc.is_active(), false);
        CHECK_EQ(vx.listener, &linproc);
        CHECK_EQ(vx.current_controller, nullptr);
        CHECK_EQ(xstub.is_active(), false);
        CHECK_EQ(ystub.is_active(), false);
        CHECK_EQ(x.listener, &xstub);
        CHECK_EQ(x.current_controller, nullptr);
        CHECK_EQ(y.listener, &ystub);
        CHECK_EQ(y.current_controller, nullptr);
    }


    SUBCASE("xctr move")
    {
        double tgt = 1;
        xctr.incmove(0, &tgt);
        vxctr.incmove(0, &tgt);
        CHECK_EQ(vxctr.is_active(), false);
        CHECK_EQ(vyctr.is_active(), false);
        CHECK_EQ(xctr.is_active(), true);
        CHECK_EQ(yctr.is_active(), false);
        CHECK_EQ(linproc.is_active(), false);
        CHECK_EQ(vx.listener, &linproc);
        CHECK_EQ(vx.current_controller, nullptr);
        CHECK_EQ(vy.listener, &linproc);
        CHECK_EQ(vy.current_controller, nullptr);
        CHECK_EQ(xstub.is_active(), true);
        CHECK_EQ(ystub.is_active(), false);
        CHECK_EQ(x.listener, &xstub);
        CHECK_EQ(x.current_controller, &xctr);
        CHECK_EQ(y.listener, &ystub);
        CHECK_EQ(y.current_controller, nullptr);

        xctr.serve(1000000000);
        CHECK_EQ(vxctr.is_active(), false);
        CHECK_EQ(vyctr.is_active(), false);
        CHECK_EQ(xctr.is_active(), false);
        CHECK_EQ(yctr.is_active(), false);
        CHECK_EQ(linproc.is_active(), false);
        CHECK_EQ(vx.listener, &linproc);
        CHECK_EQ(vx.current_controller, nullptr);
        CHECK_EQ(xstub.is_active(), false);
        CHECK_EQ(ystub.is_active(), false);
        CHECK_EQ(x.listener, &xstub);
        CHECK_EQ(x.current_controller, nullptr);
        CHECK_EQ(y.listener, &ystub);
        CHECK_EQ(y.current_controller, nullptr);
    }


    SUBCASE("vxctr and vyctr move")
    {
        double tgt = 1;
        vxctr.incmove(0, &tgt);
        vyctr.incmove(0, &tgt);
        CHECK_EQ(vxctr.is_active(), true);
        CHECK_EQ(vyctr.is_active(), true);
        CHECK_EQ(xctr.is_active(), false);
        CHECK_EQ(yctr.is_active(), false);
        CHECK_EQ(linproc.is_active(), true);
        CHECK_EQ(vx.listener, &linproc);
        CHECK_EQ(vx.current_controller, &vxctr);
        CHECK_EQ(vy.listener, &linproc);
        CHECK_EQ(vy.current_controller, &vyctr);
        CHECK_EQ(xstub.is_active(), true);
        CHECK_EQ(ystub.is_active(), true);
        CHECK_EQ(x.listener, &xstub);
        CHECK_EQ(x.current_controller, &linproc);
        CHECK_EQ(y.listener, &ystub);
        CHECK_EQ(y.current_controller, &linproc);

        vxctr.serve(1000000000);
        vyctr.serve(1000000000);
        CHECK_EQ(vxctr.is_active(), false);
        CHECK_EQ(vyctr.is_active(), false);
        CHECK_EQ(xctr.is_active(), false);
        CHECK_EQ(yctr.is_active(), false);
        CHECK_EQ(linproc.is_active(), false);
        CHECK_EQ(vx.listener, &linproc);
        CHECK_EQ(vx.current_controller, nullptr);
        CHECK_EQ(xstub.is_active(), false);
        CHECK_EQ(ystub.is_active(), false);
        CHECK_EQ(x.listener, &xstub);
        CHECK_EQ(x.current_controller, nullptr);
        CHECK_EQ(y.listener, &ystub);
        CHECK_EQ(y.current_controller, nullptr);
    }
}
*/