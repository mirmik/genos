#include <doctest/doctest.h>
#include <ralgo/cnc/planner.h>

TEST_CASE("planner.0")
{
    igris::ring<cnc::planner_block> blocks(10);
    igris::ring<cnc::control_shift> shifts(10);

    cnc::revolver revolver;
    cnc::planner planner(&blocks, &revolver);
    planner.disable_frequency_protection();
    planner.set_dim(1);

    auto &block = blocks.head_place();

    block.nominal_velocity = 0.5;
    block.acceleration = 0.1;
    block.set_direction({1});
    block.acceleration_before_ic = 5;
    block.deceleration_after_ic = 95;
    block.block_finish_ic = 100;

    block.exact_stop = 0;
    blocks.move_head_one();
    CHECK_EQ(blocks.room(), 9);
    // CHECK_EQ(shifts.room(), 10);

    planner.serve(false);

    // CHECK_EQ(shifts.room(), 0);
}

/*TEST_CASE("planner.1")
{
    igris::ring<cnc::planner_block, 10> blocks;
    igris::ring<cnc::control_shift, 20> shifts;

    cnc::planner planner(&blocks, &shifts);
    planner.set_dim(1);

    auto & block = blocks.head_place();

    block.nominal_velocity = 0.5;

    block.acceleration = 0.1;
    block.multipliers[0] = 1;

    block.active_finish_ic = 5;
    block.acceleration_before_ic = 2;
    block.deceleration_after_ic = 5;
    block.block_finish_ic = 7;

    block.exact_stop = 0;
    blocks.move_head_one();

    planner.serve();

    CHECK_EQ(planner.iteration_counter, 7);
    CHECK_EQ(shifts.avail(), 7);
}*/

/*TEST_CASE("planner.2")
{
    igris::ring<cnc::planner_block, 10> blocks;
    igris::ring<cnc::control_shift, 20> shifts;

    cnc::planner planner(&blocks, &shifts);
    planner.set_dim(1);

    auto & block = blocks.head_place();

    block.nominal_velocity = 0.5;

    block.acceleration = 0.25;
    block.multipliers[0] = 1;

    block.active_finish_ic = 6;
    block.acceleration_before_ic = 2;
    block.deceleration_after_ic = 6;
    block.block_finish_ic = 8;

    block.exact_stop = 0;
    blocks.move_head_one();

    //planner.start_with_first_block();

    planner.iteration();
    CHECK_EQ(planner.count_of_reevaluation, 1);
    CHECK_EQ(planner.state, 0);
    CHECK(planner.active_block);
    CHECK_EQ(planner.accelerations[0], 0.25);
    CHECK_EQ(planner.velocities[0], 0.25);
    CHECK_EQ(planner.dda_counters[0], 0.125);
    CHECK_EQ(planner.steps[0], 0);

    planner.iteration();
    CHECK_EQ(planner.count_of_reevaluation, 1);
    CHECK_EQ(planner.state, 0);
    CHECK(planner.active_block);
    CHECK_EQ(planner.accelerations[0], 0.25);
    CHECK_EQ(planner.velocities[0], 0.5);
    CHECK_EQ(planner.dda_counters[0], 0.125 + 0.25 + 0.125);
    CHECK_EQ(planner.iteration_counter, 2);
    CHECK_EQ(planner.steps[0], 0);

    planner.iteration();
    CHECK_EQ(planner.count_of_reevaluation, 2);
    CHECK_EQ(planner.state, 1);
    CHECK(planner.active_block);
    CHECK_EQ(planner.count_of_postactive(), 0);
    CHECK_EQ(planner.accelerations[0], 0);
    CHECK_EQ(planner.velocities[0], 0.5);
    CHECK_EQ(planner.dda_counters[0], 0);
    CHECK_EQ(planner.steps[0], 1);

    planner.iteration();
    CHECK_EQ(planner.count_of_reevaluation, 2);
    CHECK_EQ(planner.state, 1);
    CHECK_EQ(planner.count_of_postactive(), 0);
    CHECK_EQ(planner.accelerations[0], 0);
    CHECK_EQ(planner.velocities[0], 0.5);
    CHECK_EQ(planner.dda_counters[0], 0.5);
    CHECK_EQ(planner.steps[0], 1);

    planner.iteration();
    CHECK_EQ(planner.count_of_reevaluation, 2);
    CHECK_EQ(planner.state, 1);
    CHECK_EQ(planner.count_of_postactive(), 0);
    CHECK_EQ(planner.accelerations[0], 0);
    CHECK_EQ(planner.velocities[0], 0.5);
    CHECK_EQ(planner.dda_counters[0], 0);
    CHECK_EQ(planner.steps[0], 2);

    planner.iteration();
    CHECK_EQ(planner.count_of_reevaluation, 2);
    CHECK_EQ(planner.state, 1);
    CHECK_EQ(planner.count_of_postactive(), 0);
    CHECK_EQ(planner.accelerations[0], 0);
    CHECK_EQ(planner.velocities[0], 0.5);
    CHECK_EQ(planner.dda_counters[0], 0.5);
    CHECK_EQ(planner.steps[0], 2);

    planner.iteration();
    CHECK_EQ(planner.count_of_reevaluation, 3);
    CHECK_EQ(planner.state, 0);
    CHECK_EQ(planner.count_of_postactive(), 1);
    CHECK_EQ(planner.accelerations[0], -0.25);
    CHECK_EQ(planner.velocities[0], 0.25);
    CHECK_EQ(planner.dda_counters[0], 0.875);
    CHECK_EQ(planner.steps[0], 2);

    planner.iteration();
    CHECK_EQ(planner.count_of_reevaluation, 3);
    CHECK_EQ(planner.state, 0);
    CHECK_EQ(planner.count_of_postactive(), 1);
    CHECK_EQ(planner.accelerations[0], -0.25);
    CHECK_EQ(planner.velocities[0], 0);
    CHECK_EQ(planner.dda_counters[0], 0);
    CHECK_EQ(planner.steps[0], 3);

    CHECK_EQ(shifts.get(0).step, 0);
    CHECK_EQ(shifts.get(1).step, 0);
    CHECK_EQ(shifts.get(2).step, 1);
    CHECK_EQ(shifts.get(3).step, 0);
    CHECK_EQ(shifts.get(4).step, 1);
    CHECK_EQ(shifts.get(5).step, 0);
    CHECK_EQ(shifts.get(6).step, 0);
    CHECK_EQ(shifts.get(7).step, 1);

    CHECK_EQ(shifts.get(0).direction, 0);
    CHECK_EQ(shifts.get(1).direction, 0);
    CHECK_EQ(shifts.get(2).direction, 1);
    CHECK_EQ(shifts.get(3).direction, 0);
    CHECK_EQ(shifts.get(4).direction, 1);
    CHECK_EQ(shifts.get(5).direction, 0);
    CHECK_EQ(shifts.get(6).direction, 0);
    CHECK_EQ(shifts.get(7).direction, 1);
}

TEST_CASE("planner.3")
{
    igris::ring<cnc::planner_block, 10> blocks;
    igris::ring<cnc::control_shift, 400> shifts;

    cnc::planner planner(&blocks, &shifts);
    planner.set_dim(1);

    auto & block = blocks.head_place();

    block.nominal_velocity = 0.5;

    block.acceleration = 0.25;
    block.multipliers[0] = 1;
    block.fullpath = 3;

    block.start_ic = 0;
    block.acceleration_before_ic = 2;
    block.deceleration_after_ic = 6;
    block.block_finish_ic = 8;

    block.exact_stop = 0;
    blocks.move_head_one();

    CHECK(block.validation());

    planner.serve();

    CHECK_EQ(shifts.get(0).step, 0);
    CHECK_EQ(shifts.get(1).step, 0);
    CHECK_EQ(shifts.get(2).step, 1);
    CHECK_EQ(shifts.get(3).step, 0);
    CHECK_EQ(shifts.get(4).step, 1);
    CHECK_EQ(shifts.get(5).step, 0);
    CHECK_EQ(shifts.get(6).step, 0);
    CHECK_EQ(shifts.get(7).step, 1);

    CHECK_EQ(shifts.get(0).direction, 0);
    CHECK_EQ(shifts.get(1).direction, 0);
    CHECK_EQ(shifts.get(2).direction, 1);
    CHECK_EQ(shifts.get(3).direction, 0);
    CHECK_EQ(shifts.get(4).direction, 1);
    CHECK_EQ(shifts.get(5).direction, 0);
    CHECK_EQ(shifts.get(6).direction, 0);
    CHECK_EQ(shifts.get(7).direction, 1);
}*/
