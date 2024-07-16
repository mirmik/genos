/**
    @file
*/

#ifndef DEFS_H
#define DEFS_H

// Включить для проекта ММЗ (Йошкар-Ола). Защита от вращения рамы в нижнем
// положении.
#define TRUCK_PROJECT 0

enum Direction
{
    FORWARD = 0,
    BACKWARD = 1
};

static inline Direction reverseDirection(Direction dir)
{
    return dir == FORWARD ? BACKWARD : FORWARD;
}

enum class DistanceUnit : uint8_t
{
    CommandUnit,
    ServoSideEncoderUnit
};

#endif