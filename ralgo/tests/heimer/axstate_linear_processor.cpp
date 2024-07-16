#include <doctest/doctest.h>
#include <ralgo/heimer/axstate_linear_processor.h>
#include <ralgo/heimer/command.h>

using namespace heimer;

TEST_CASE("axstate_linear_processor")
{
    heimer_reinit();

    axis_state a("a"), b("b"), c("c"), d("d");
    axis_state *left[] = {&a, &b};
    axis_state *right[] = {&c, &d};

    double matrix[] = {2, 0.5, 0, 1};
    double inverse_matrix[4];

    axstate_linear_processor linproc(
        "linproc", 2, left, right, matrix, inverse_matrix);

    CHECK_EQ(linproc.name(), "linproc");
    CHECK_EQ(heimer::signal_processors_count(), 1);

    linproc.evaluate_invertion();
    CHECK_NE(inverse_matrix[0], 0);

    CHECK_EQ(linproc.iterate_left(NULL), &a);
    CHECK_EQ(linproc.iterate_left(&a), &b);
    CHECK_EQ(linproc.iterate_left(&b), nullptr);

    CHECK_EQ(linproc.iterate_right(NULL), &c);
    CHECK_EQ(linproc.iterate_right(&c), &d);
    CHECK_EQ(linproc.iterate_right(&d), nullptr);

    c.ctrpos = 200;
    d.ctrpos = 100;
    c.ctrvel = 20;
    d.ctrvel = 10;

    linproc.serve(0);

    CHECK_EQ(a.ctrpos, 450);
    CHECK_EQ(b.ctrpos, 100);
    CHECK_EQ(a.ctrvel, 45);
    CHECK_EQ(b.ctrvel, 10);

    a.feedpos = a.ctrpos;
    b.feedpos = b.ctrpos;
    a.feedvel = a.ctrvel;
    b.feedvel = b.ctrvel;

    linproc.feedback(0);

    CHECK_EQ(c.feedpos, 200);
    CHECK_EQ(d.feedpos, 100);
    CHECK_EQ(c.feedvel, 20);
    CHECK_EQ(d.feedvel, 10);

    a.deinit();
    b.deinit();
    c.deinit();
    d.deinit();
}
