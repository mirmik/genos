#include <doctest/doctest.h>
#include <ralgo/robo/drivers/quadgen4_arduino.h>

TEST_CASE("quadgen4_arduino")
{
    robo::quadgen4_arduino quadgen(8, 10, 9, 11);
    // quadgen.set_state(3);
}
