#include <doctest/doctest.h>
#include <ralgo/cnc/revolver.h>
#include <ralgo/cnc/shift.h>

#include <ralgo/robo/stepper.h>

// TEST_CASE("revolver")
// {
//     robo::stepper _steppers[2];
//     robo::stepper *steppers[2] = {&_steppers[0], &_steppers[1]};

//     auto shift_ring = igris::ring<cnc::control_shift>(10);
//     auto revolver = cnc::revolver(&shift_ring);

//     revolver.set_steppers(steppers, 2);

//     revolver.push(0x2, 0x2);
//     CHECK_EQ(revolver.queue_size(), 1);

//     revolver.serve();
//     CHECK_EQ(revolver.queue_size(), 0);

//     revolver.push(0x1, 0x0);
//     revolver.push(0x3, 0x3);
//     CHECK_EQ(revolver.queue_size(), 2);

//     revolver.serve();
//     CHECK_EQ(revolver.queue_size(), 1);
//     CHECK_EQ(_steppers[0].steps_count(), -1);
//     CHECK_EQ(_steppers[1].steps_count(), 1);

//     revolver.serve();
//     CHECK_EQ(revolver.queue_size(), 0);
//     CHECK_EQ(_steppers[0].steps_count(), 0);
//     CHECK_EQ(_steppers[1].steps_count(), 2);

//     CHECK_EQ(revolver.room(), 10);
// }
